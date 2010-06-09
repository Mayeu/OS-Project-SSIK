/**
 * \file ring.c
 * \brief Ring user program.
 * \author Christophe Carasco
 * \version 0.1
 * \date 25 mai 2010
 */

#include <stdio.h>
#include <string.h>
#include <message.h>
#include <process.h>
#include <errno.h>

#include "ring.h"

#define MAX 	 10
#define TIMER 50

int             pid[MAX];

// params: int nb_proc, int loop -> main program
// params: int pidmain, int loop -> child programs
void
ring(int argc, char *argv[])
{
  int             i;
  pcbinfo         pcbi, pcbis;
  char            tmp[10];
  char            text[300];
  char            proctext[100];

  get_proc_info(get_pid(), &pcbi);
  get_proc_info(pcbi.supervisor, &pcbis);

  // if the supervisor process is not ring (it should be the shell), case parent
  if (strcmp(pcbis.name, "ring") != 0)
  {
    int             nb_proc;
    int             loop;
//      int status;
    char            args[3][ARG_SIZE];

    if (argc < 3)
    {
      print
        ("Error: You must input the number of processes and the number of loops to do\n");
      exit(-1);
    }

    // number of proc in argv[0]
    nb_proc = stoi(get_arg(argv, 1));
    loop = stoi(get_arg(argv, 2));

    if (nb_proc > MAX)
    {
      strcpy("Error: Number of processes must be at most ", text);
      strcat(text, itos(MAX, tmp));
      strcat(text, "\n");
      print(text);
      exit(-1);
    }
    if (loop < 0)
      loop = 0;

    strcpy("Program ring running ", text);
    strcat(text, itos(nb_proc, tmp));
    strcat(text, " process(es) for ");
    strcat(text, itos(loop, tmp));
    strcat(text, "loop(s)\n");
    print(text);

    // fill the argument array for the childs
    strcpy("ring", args[0]);
    itos(get_pid(), args[1]);
    strcpy(get_arg(argv, 2), args[2]);

/*	strcpy("Args: progname->", text);
	strcat(text, args[0]);
	strcat(text, " -- pidmain->");
	strcat(text, args[1]);
	strcat(text, " -- loops->");
	strcat(text, args[2]);
	strcat(text, "\n");
	print(text);
*/
    // creating the children
    for (i = 0; i < nb_proc; i++)
    {
      pid[i] = fourchette("ring", MAX_PRI, 3, (char **) args);
      if (pid[i] < 1)
      {
        strcpy("Error creating proc_", text);
        strcat(text, itos(i, tmp));
        strcat(text, " : ");
        strcat(text, itos(pid[i], tmp));
        strcat(text, "\n");
        print(text);
      }
    }

    // data to send to all the children
    for (i = 0; i < nb_proc; i++)
    {
      strcpy("Master sending inits to :", text);
      strcat(text, itos(pid[i], tmp));
      strcat(text, "\n");
      print(text);
      // are you the first child process ?
      send((void *) i, INT_T, pid[i]);
      // pid of the process to send the message
      send((void *) pid[(i + 1) % nb_proc], INT_T, pid[i]);
      // pid of the process to wait the message
      send((void *) pid[(i - 1) % nb_proc], INT_T, pid[i]);
    }
/*
	for(i=0; i<nb_proc ;i++)
	{
		wait(pid[i], &status);
		if(status != OMGROXX)
			print("ERROR PROCESS");
	}
*/
  }
  // if the supervisor process is the program ring, case child
  else
  {
    int             pidmain, loop;
    int             res;
    int             first = -1, pid_next = -1, pid_prev = -1;
    char            prog[20];
    char            mess[10];
    char            rcv[10];

    strcpy(get_arg(argv, 0), prog);             /** TODO : remove the hardcoded values */
    pidmain = stoi(get_arg(argv, 1));
    loop = stoi(get_arg(argv, 2));
    //strcpy("ring", prog);
    //pidmain = 3;
    //loop = 1;

/*	strcpy("Argschild: progname->", text);
	strcat(text, prog);
	strcat(text, " -- pidmain->");
	strcat(text, itos(pidmain, tmp));
	strcat(text, " -- loops->");
	strcat(text, itos(loop, tmp));
	strcat(text, "\n");
	print(text);
*/

    strcpy("Process no_", proctext);
    strcat(proctext, itos(get_pid(), tmp));
    strcat(proctext, ": ");

    // params sent by the main process
    res = recv_from_pid((int *) &first, INT_T, pidmain, 10000);
    /*if(res != pidmain)
       {
       strcpy("FAIL1", text);
       strcat(text, itos(res, tmp));
       strcat(text, "\n");
       print(text);
       exit(FAILNOOB);
       } */
    res = recv_from_pid((int *) &pid_prev, INT_T, pidmain, 10000);
    res = recv_from_pid((int *) &pid_next, INT_T, pidmain, 10000);

    strcpy(proctext, text);
    strcat(text, "first: ");
    strcat(text, itos(first, tmp));
    strcat(text, "prev: ");
    strcat(text, itos(pid_prev, tmp));
    strcat(text, "next: ");
    strcat(text, itos(pid_next, tmp));
    strcat(text, "\n");
    print(text);

    for (i = 0; i < loop; i++)
    {
      // if we are the first child, send then receive
      if (first == 0)
      {
        strcpy("Hello", mess);
        send(mess, CHAR_PTR, pid_next);

        strcpy(proctext, text);
        strcat(text, "sent '");
        strcat(text, mess);
        strcat(text, "' to Process no_");
        strcat(text, itos(pid_next, tmp));
        strcat(text, "\n");
        print(text);

        sleep(TIMER);
        res = recv_from_pid((char *) rcv, CHAR_PTR, pid_prev, 10000);

        strcpy(proctext, text);
        strcat(text, "received '");
        strcat(text, rcv);
        strcat(text, "' from Process no_");
        strcat(text, itos(pid_prev, tmp));
        strcat(text, "\n");
        print(text);

        sleep(TIMER);
      }
      // if not, receive then send
      else
      {
        res = recv_from_pid((char *) rcv, CHAR_PTR, pid_prev, 10000);

        strcpy(proctext, text);
        strcat(text, "received '");
        strcat(text, rcv);
        strcat(text, "' from Process no_");
        strcat(text, itos(pid_prev, tmp));
        strcat(text, "\n");
        print(text);

        sleep(TIMER);
        send(mess, CHAR_PTR, pid_next);

        strcpy(proctext, text);
        strcat(text, "sent '");
        strcat(text, mess);
        strcat(text, "' to Process no_");
        strcat(text, itos(pid_next, tmp));
        strcat(text, "\n");
        print(text);

        sleep(TIMER);
      }
    }
  }

  exit(0);
}
