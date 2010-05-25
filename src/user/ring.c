/**
 * \file ring.c
 * \brief blabla
 * \author Christophe Carasco
 * \version 0.1
 * \date 25 mai 2010
 *
 */

#include <stdio.h>
#include <string.h>
#include <message.h>

#define MAX 10

int pid[MAX];

void ring_proc(int pidmain, int loop)
{
	int i;
	int first;
	int pid_next, pid_prev;
	
	char mess[10] = "hello";
	char rcv[10];

	recv_from_pid((int*)&first, INT_T, pidmain, -1);
	recv_from_pid((int*)&pid_next, INT_T, pidmain, -1);
	recv_from_pid((int*)&pid_prev, INT_T, pidmain, -1);

	for (i=0; i<loop; i++)
	{
		if (first)
		{
			send(mess, CHAR_PTR, pid_next);
			print("process no "); printi(get_pid()); print("sent msg '"); print(mess); println("'");
			recv_from_pid((char*)rcv, CHAR_PTR, pid_prev, -1);
			print("process no "); printi(get_pid()); print("got msg '"); print(rcv); println("'");
		}
		else
		{
			recv_from_pid((char*)rcv, CHAR_PTR, pid_prev, -1);
			print("process no "); printi(get_pid()); print("got msg '"); print(rcv); println("'");
			send(mess, CHAR_PTR, pid_next);
			print("process no "); printi(get_pid()); print("sent msg '"); print(mess); println("'");
		}
	}

	exit(OMGROXX);
}

void ring(int nb_proc, int loop)
{
	int i;
	char args[4];

	itos(3, arg[0]);
	itos(get_pid(), arg[1]);
	itos(loop, arg[2]);

	for (i=0; i<nb_proc, i++)
	{
		pid[i] = fourchette("ring_proc", (char**)args);
	}
	for (i=0; i<nb_proc; i++)
	{
		send((void*)(i == 0), INT_T, pid[i]);
		send((void*)pid[(i+1)%nb_proc], INT_T, pid[i]);
		send((void*)pid[(i-1)%nb_proc], INT_T, pid[i]);
	}

	exit(OMGROXX);
}
