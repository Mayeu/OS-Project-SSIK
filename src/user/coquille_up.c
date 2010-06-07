/**
 * \file coquille_up.c
 * \brief Contains the shell programs chg_prio, ps, tuer, malta
 * \author Adrien Forest
 * \version 0.1
 * \date 26 Mai 2010
 */

#include <stdio.h>
#include <string.h>
#include <process.h>

#include "coquille_up.h"

// params: int pid, int new_prio
int
chg_prio(int argc, char *argv[])
{
  int             pid, prio;

  if (argc < 3)
  {
    print("Need two arguments (pid and new priority)\n");
    exit(-1);
  }

  pid = stoi(get_arg(argv, 1));
  prio = stoi(get_arg(argv, 2));

  exit(0);
  return chg_pri(pid, prio);
}

// params: no param
int
ps(int argc, char *argv[])
{
  int           pid[MAXPCB];
  int           i, len, my_pid;
	pcbinfo 			pinf;

	// init the pid list
	for (i=0; i<MAXPCB; i++)
		pid[i] = -1;

  len = get_ps(pid);

  printi(len); printn();

  //my_pid = get_pid();
  my_pid = 185;

  print("PID\tNAME\n");
  print("________________\n");
  for (i = 0; i < len; i++)
  {
		get_proc_info(pid[i], &pinf);
    printi(pid[i]);
    print("\t");
    print(pinf.name);
		printn();
  }
  print("________________\n");

  exit(47);
  return 0;
}

// params: int pid
int
tuer(int argc, char *argv[])
{
  return kill(stoi(get_arg(argv, 1)));
}

// params: char* text
int
malta(int argc, char *argv[])
{
  return fprint(MALTA, get_arg(argv, 1));
}

// params: no param
int
help(int argc, char *argv[])
{
  print("List of available user programs\n");
  print("-------------------------------\n");
  print("coquille: Spawn a new shell.\n");
  print("increment n: Print a sequence from from 1 to n.\n");
  print("fibonacci n: Print the fibonacci sequence up to n numbers.\n");
  print
    ("scroller: Create a process that scroll a predefined string of the LCD.\n");
  print
    ("ring nb_proc nb_loop: Create a ring of nb_proc communicating processes.\n");
  print("philosopher nb_philo nb_loop: nb_philo philosophers ");
  print("try to eat/think nb_loop times.\n");
  print("supervision: demonstration of process supervision.\n");
  print("ps: print the list of all the running processes.\n");
  print("chg_prio p pri: Change the priority of the process of pid p ");
  print("with the new priority pri.\n");
  print("tuer p: Kill the process of pid p.\n");
  print("malta: Allow the user to change the malta LCD message.\n");

  exit(0);
  return 0;
}

// params: int pid
int
proc_info(int argc, char *argv[])
{
  int             pid, err, i;
  pcbinfo         res;

  pid = stoi(get_arg(argv, 1));
  err = get_proc_info(pid, &res);
  if (err == 0)
  {
    print("Information about process pid = ");
    printi(pid);
    print("\n\tname:\t\t\t");
    print(res.name);
    print("\n\tpriority:\t\t");
    printi(res.pri);
    print("\n\tsupervised processes:\t");
    for (i = 0; i < MAXPCB; i++)
    {
      if (res.supervised[i] != -1)
      {
        printi(res.supervised[i]);
        print(" ");
      }
    }
    print("\n\tsupervisor process:\t");
    printi(res.supervisor);
		printn();
  }
  else
  {
    print("An error occured! (code:");
    printi(err);
    print(")\n");
  }
  exit(0);
  return 0;
}
