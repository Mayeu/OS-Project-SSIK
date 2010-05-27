/**
 * \file philosopher.c
 * \brief blabla
 * \author Adrien Forest
 * \version 0.1
 * \date 26 mai 2010
 */

#include <stdio.h>
#include <string.h>
#include <message.h>
#include <process.h>

#define MAX 	 5
#define TIMER 500
// params: int nb_philop, int loop -> main program
// params: int pidmain, int loop -> child programs
int philosopher(int argc, char *argv[])
{
return 1;
/*
	int i;
	pcbinfo pcbi;
	
	get_proc_info(get_pid(), &pcbi);
	get_proc_info(pcbi.supervisor, &pcbi);
	
	// if the supervisor process is not philosopher (it should be the shell), case parent
	if (strcmp(pcbi.name, "philosopher") != 0)
	{
		int nb_philo;
		char args[3][ARG_SIZE];

		if (argc < 2)
		{
			println("Error: You must input nb_philo and loop");
			exit(-1);
		}

		// number of proc in argv[0]
		nb_philo = stoi(argv[0]);

		if (nb_philo > MAX)
		{
			print("Error: Number of philosophers must be at most ");printiln(MAX);
			exit(-1);
		}

		// fill the argument array for the childs
		itos(get_pid(), args[0]);						// pid of the parent
		strcpy(get_arg(argv, 2), args[1]);	// nb loops
	
		// creating the children
		for (i=0; i<nb_philo; i++)
		{
			itos(i, args[2]);
			pid[i] = fourchette("philosopher", 2, (char**)args);
		}

		// data to send to all the children
		for (i=0; i<nb_philo; i++)
		{
			// are you the first child process ?
			send((void*)(i == 0), INT_T, pid[i]);
			// pid of the process to send the message
			send((void*)pid[(i+1)%nb_proc], INT_T, pid[i]);
			// pid of the process to wait the message
			send((void*)pid[(i-1)%nb_proc], INT_T, pid[i]);
		}
	}
	// if the supervisor process is the program ring, case child
	else
	{
		int pidmain, loop;
		int first, pid_next, pid_prev;
		int status; // 0 = thinking, 1 = eating
		
		char rcv[10];

		pidmain = stoi(argv[0]);
		loop = stoi(argv[1]);
		
		// params sent by the main process
		recv_from_pid((int*)&first,    INT_T, pidmain, 0);
		recv_from_pid((int*)&pid_next, INT_T, pidmain, 0);
		recv_from_pid((int*)&pid_prev, INT_T, pidmain, 0);

	}
*/
}

