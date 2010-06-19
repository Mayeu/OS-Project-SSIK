/**
 * \file philosopher.c
 * \brief blabla
 * \author Adrien Forest
 * \version 0.1
 * \date 26 mai 2010
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <message.h>
#include <process.h>
#include "philosopher.h"

#define MAX_PHILO 20
#define TIMEOUT 20000

enum
{
	START,
	FORK_L,
	FORK_R,
	RELEASE,
	FORK_FREE,
	FORK_TAKEN,
	GO_FOR_IT,
	END
};

// params: 
	void
dining_philosopher(int argc, char *argv[])
{

	int             nb_philo = stoi(get_arg(argv, 1));
	int             loop = stoi(get_arg(argv, 2));
	int             waiter;       //pid waiter
	int             philos[MAX_PHILO];    //pids philos
	int             status;
	char            args[2][ARG_SIZE];
	char            philos_args[MAX_PHILO][4][ARG_SIZE];
	int             i, j;
	char            tmp[10];
	char            text[200];

	if (argc != 3)
	{
		print
			("Invalid arguments number: excepted number of philosophers and the number of cycles\n");
		exit(FAILNOOB);
	}

	if (nb_philo < 2 || loop < 1)
	{
		print("Invalid arguments\n");
		exit(INVARG);
	}

	// build the arguments for the waiter
	strcpy("waiter", args[0]);    //prog_name
	strcpy(get_arg(argv, 1), args[1]);    //nb_philo

	waiter = fourchette("waiter", BAS_PRI, 2, (char **) args);
	if (waiter < 1)
	{
		strcpy("Error creating waiter", text);
		strcat(text, " : ");
		strcat(text, itos(waiter, tmp));
		strcat(text, "\n");
		print(text);
		exit(waiter);
	}

	for (i = 0; i < nb_philo; i++)
	{
		//build arguments for the child
		strcpy("philosopher", philos_args[i][0]);
		itos(waiter, philos_args[i][1]);
		itos(i, philos_args[i][2]); //index of the pid in the philos id array
		strcpy(get_arg(argv, 2), philos_args[i][3]);        //loop
		philos[i] =
			fourchette("philosopher", BAS_PRI, 4, (char **) philos_args[i]);
		if (philos[i] < 1)
		{
			strcpy("Error creating a philosopher", text);
			strcat(text, " : ");
			strcat(text, itos(philos[i], tmp));
			strcat(text, "\n");
			print(text);
			//clean up
			for (j = 0; j < i; j++)
				kill(philos[j]);
			kill(waiter);
			exit(FAILNOOB);
		}
	}
	// wait for the child to end
	for (i = 0; i < nb_philo; i++)
		wait(philos[i], &status);
	//stop the waiter       
	send((void *) END, INT_T, waiter);
	wait(waiter, &status);
	print("THE END\n");

	exit(OMGROXX);
}

	void
waiter(int argc, char *argv[])  //int nb_philo
{
	if (argc < 2)
		exit(FAILNOOB);
	char            tmp[10], text[100];
	int             nb_philo = stoi(get_arg(argv, 1));
	int             philo_id, philo_pid;
	int             fork[nb_philo], philos[nb_philo];     //fork[i] = 0 means the fork is not available

	int             i, j, code, in, do_packing, end = 0;
	int             buf_req[nb_philo];
	int             buf_phi[nb_philo];    //the waiter will need to buffer the requests that can't be satisfied when he gets the message. At most nb_philo-1 messages can be buffered at the same time. One more and it's a deadlock. 
	int             fork_taken = 0;      //counts the number of fork taken

	strcpy("Waiter no_", text);
	strcat(text, itos(get_pid(), tmp));
	strcat(text, ": serving ");
	strcat(text, itos(nb_philo, tmp));
	strcat(text, " philosophers\n");
	print(text);

	//we mark the fork as being free
	for (i = 0; i < nb_philo; i++)
	{
		fork[i] = FORK_FREE;
	}

	/*
	 * The philosopher send their pid
	 */
	strcpy("Received pids: ", text);
	for (i = 0; i < nb_philo; i++)
	{
		//philo_pid = recv(&philo_id, INT_T, TIMEOUT);
		philo_pid = recv((int *) &philo_id, INT_T, TIMEOUT);
		if (philo_pid < 0)
		{
			print("Failed to receive philosopher pid\n");
			exit(FAILNOOB);
		}
		philos[philo_id] = philo_pid;
		strcat(text, itos(philo_pid, tmp));
		strcat(text, " ");
	}
	strcat(text, "\n");
	print(text);

	//start
	for (i = 0; i < nb_philo; i++)
		send((void *) START, INT_T, philos[i]);

	in = 0;
	while (!end)
	{
		//the waiter waits for a request
		philo_pid = recv(&code, INT_T, 10000);
		//print("Timeout\n");
		if (philo_pid < 1)
			continue;

		for (i = 0; i < nb_philo && philos[i] != philo_pid; i++);   // get the index of the philosopher that sent the message
		philo_id = i;
		//print("there 1\n");

		if (code == FORK_L || code == FORK_R)
		{
			//print("there 2\n");
			bool            t;
			if (code == FORK_L)
				t = left_fork (philo_id, philo_pid, fork, &fork_taken, nb_philo);
			else
				t = right_fork ((philo_id+1)%nb_philo, philo_pid , fork, &fork_taken);

			if (!t)
			{
				//fork taken or only one fork left. In the last case, it can only be taken by a philosopher who already has a fork. Since the left fork are requested first, this is not the case here. We put the request in the buffer
				buf_req[in] = code;
				buf_phi[in] = philo_id;
				in++;
			}
		}

		else if (code == RELEASE)
		{
			print("tess\n");
			fork[philo_id] = FORK_FREE;       // release the left fork
			fork[(philo_id + 1) % nb_philo] = FORK_FREE;      // release the right fork
			fork_taken -= 2;

			//look if some of the buffered requests can be satisfied now
			for (i = 0; i < in; i++)
			{
				do_packing = 0;
				code = buf_req[i];
				philo_id = buf_phi[i];
				philo_pid = philos[philo_id];

				if (code == FORK_L || code == FORK_R)
				{
					//print("there 2\n");
					bool            t;
					if (code == FORK_L)
						t = left_fork (philo_id, philo_pid, fork, &fork_taken, nb_philo);
					else
						t = right_fork ((philo_id+1)%nb_philo, philo_pid , fork, &fork_taken);

					if(t)
						do_packing = 1;
				}

				if (do_packing)
				{
					//the request number i has been satisfied, let's pack the array
					for (j = i + 1; j < in; j++)
					{
						buf_req[j - 1] = buf_req[j];
						buf_phi[j - 1] = buf_phi[j];
					}
					in--;
					i--;
				}
			}
		}
		else if (code == END)
			end = 1;
	}

	exit(OMGROXX);
}

/* An average philosopher: thinking, eating, thinking, eating and so on. What a livelocked mind ! */
	void
philosopher(int argc, char *argv[])
{
	int             count = 0;    //each philosopher does loop cycles before exiting
	char            tmp[10], proctext[50], text[200];
	int             mess;
	int             philo_id, waiter_pid, loop;

	if (argc < 4)
	{
		print
			("Invalid arguments number: expected waiter pid, philosopher id and number of cycles\n");
		exit(INVARG);
	}

	waiter_pid = stoi(get_arg(argv, 1));
	philo_id = stoi(get_arg(argv, 2));
	loop = stoi(get_arg(argv, 3));

	strcpy("Process no_", proctext);
	strcat(proctext, itos(get_pid(), tmp));
	strcat(proctext, ": ");

	//each philosopher gives its philo_id to the waiter.
	send((void *) philo_id, INT_T, waiter_pid);
	strcpy(proctext, text);
	strcat(text, "sent its id '");
	strcat(text, itos(philo_id, tmp));
	strcat(text, "' to the waiter no_");
	strcat(text, itos(waiter_pid, tmp));
	strcat(text, "\n");
	print(text);

	//wait until the waiter says the show can begin
	if (recv_from_pid(&mess, INT_T, waiter_pid, 1000) != waiter_pid
			&& mess != START)
	{
		print("Failed to receive order to proceed from the waiter\n");
		exit(FAILNOOB);
	}

	while (count < loop)
	{
		strcpy(proctext, text);
		strcat(text, "is thinking\n");
		print(text);

		sleep( (unsigned int) random((int) &count, (int) &loop) % 2000);

		strcpy(proctext, text);
		strcat(text, "is hungry\n");
		print(text);

		if (send((void *) FORK_L, INT_T, waiter_pid) != OMGROXX)
		{
			print("Failed to send the request for the left fork\n");
			exit(FAILNOOB);
		}
		recv_from_pid(&mess, INT_T, waiter_pid, 10000);

		strcpy(proctext, text);
		strcat(text, "got left fork\n");
		print(text);

		if (send((void *) FORK_R, INT_T, waiter_pid) != OMGROXX)
		{
			print("Failed to send the request for the right fork\n");
			exit(FAILNOOB);
		}
		recv_from_pid(&mess, INT_T, waiter_pid, 10000);

		strcpy(proctext, text);
		strcat(text, "is eating\n");
		print(text);

		sleep((unsigned int)random((int) &mess, (int) &waiter_pid) % 2000);

		strcpy(proctext, text);
		strcat(text, "finished eating\n");
		print(text);

		send((void *) RELEASE, INT_T, waiter_pid);

		count++;
	}

	exit(OMGROXX);
}

/* check if philo_pid can take its left fork. Returns 1 if he can, 0 otherwise */
bool left_fork(int index,int philo_pid, int* fork, int* fork_taken, int num_philo){
	//the left fork has the same id as the philosopher 
	if(fork[index] == FORK_FREE && *fork_taken < (num_philo - 1)){
		//fork free and at least two fork remaining, go for it
		fork[index] = FORK_TAKEN;
		(*fork_taken) ++;
		send((void *) GO_FOR_IT, INT_T, philo_pid);
		return TRUE;
	}
	//this fork is not available/shouldn't be taken
	return FALSE;
}

/* check if philo_pid can take its right fork. Returns 1 if he can, 0 otherwise */
bool right_fork(int index, int philo_pid, int* fork, int* fork_taken){
	//the right is at the index (philo_pid+1)%NUM_PHILO
	if(fork[index] == FORK_FREE){
		// no need to check for fork_taken because a process requesting its right fork already has the left one
		fork[index] = FORK_TAKEN;
		(*fork_taken) ++;
		send((void *) GO_FOR_IT, INT_T, philo_pid);
		return TRUE;
	}
	return FALSE;
}

/* Looks into the philos_pid array in order to find philo_pid. Once found, it returns the index, that is the philo_id */
int get_philo_id(int philo_pid, int* philos_pid, int num_philo){
	int i;
	for(i=0; i < num_philo && philos_pid[i] != philo_pid;i++);
	return i;
}

	int
random(int a, int b)
{
	return ((a * 63949 + 26227) % 1000) * ((b * 26227 + 63949) % 1000);
}
