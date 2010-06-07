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

#include "ring.h"

#define MAX 	 10
#define TIMER 500

int             pid[MAX];

// params: int nb_proc, int loop -> main program
// params: int pidmain, int loop -> child programs
int
ring(int argc, char *argv[])
{
  int             i;
  pcbinfo         pcbi;

  get_proc_info(get_pid(), &pcbi);
  get_proc_info(pcbi.supervisor, &pcbi);

  // if the supervisor process is not ring (it should be the shell), case parent
  if (strcmp(pcbi.name, "ring") != 0)
  {
    int             nb_proc;
    char            args[2][ARG_SIZE];

    if (argc < 2)
    {
      print
        ("Error: You must input the number of processes and the number of loops to do\n");
      exit(-1);
    }

    // number of proc in argv[0]
    nb_proc = stoi(argv[0]);

    if (nb_proc > MAX)
    {
      print("Error: Number of processes must be at most ");
      printi(MAX);
      printn();
      exit(-1);
    }

    // fill the argument array for the childs
    itos(get_pid(), args[0]);
    strcpy(get_arg(argv, 2), args[1]);

    // creating the children
    for (i = 0; i < nb_proc; i++)
      pid[i] = fourchette("ring", BAS_PRI, 2, (char **) args);

    // data to send to all the children
    for (i = 0; i < nb_proc; i++)
    {
      // are you the first child process ?
      send((void *) (i == 0), INT_T, pid[i]);
      // pid of the process to send the message
      send((void *) pid[(i + 1) % nb_proc], INT_T, pid[i]);
      // pid of the process to wait the message
      send((void *) pid[(i - 1) % nb_proc], INT_T, pid[i]);
    }
  }
  // if the supervisor process is the program ring, case child
  else
  {
    int             pidmain, loop;
    int             first, pid_next, pid_prev;

    char            mess[10] = "hello";
    char            rcv[10];

    pidmain = stoi(argv[0]);
    loop = stoi(argv[1]);

    // params sent by the main process
    recv_from_pid((int *) &first, INT_T, pidmain, 0);
    recv_from_pid((int *) &pid_next, INT_T, pidmain, 0);
    recv_from_pid((int *) &pid_prev, INT_T, pidmain, 0);

    for (i = 0; i < loop; i++)
    {
      // if we are the first child, send then receive
      if (first)
      {
        send(mess, CHAR_PTR, pid_next);
        print("Process ");
        printi(get_pid());
        print("sent '");
        print(mess);
        print("' to Process ");
        printi(pid_next);
        printn();
        sleep(TIMER);
        recv_from_pid((char *) rcv, CHAR_PTR, pid_prev, -1);
        print("Process ");
        printi(get_pid());
        print("received '");
        print(rcv);
        print("' from Process ");
        printi(pid_prev);
        printn();

        sleep(TIMER);
      }
      // if not, receive then send
      else
      {
        recv_from_pid((char *) rcv, CHAR_PTR, pid_prev, -1);
        print("Process ");
        printi(get_pid());
        print("received '");
        print(rcv);
        print("' from Process ");
        printi(pid_prev);
        printn();
        sleep(TIMER);
        send(mess, CHAR_PTR, pid_next);
        print("Process ");
        printi(get_pid());
        print("sent '");
        print(mess);
        print("' to Process ");
        printi(pid_next);
        printn();
        sleep(TIMER);
      }
    }
  }

  exit(0);
  return 0;
}
