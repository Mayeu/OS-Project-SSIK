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
#include <error.h>

#include "coquille_up.h"

// params: int pid, int new_prio
void
chg_prio(int argc, char *argv[])
{
  int             pid, prio, res;

  if (argc < 3)
  {
    print("Need two arguments (pid and new priority)\n");
    exit(-1);
  }

  pid = stoi(get_arg(argv, 1));
  prio = stoi(get_arg(argv, 2));

  res = chg_pri(pid, prio);

  if (res < 0)
  {
    print("An error occured! (code:");
    printi(res);
    print(")\n");
  }
  exit(0);
}

// params: no param
void
ps(int argc, char *argv[])
{
  int             pid[MAXPCB];
  int             i;
  char            buf[255];
  char            num[15];
  int             i, len;
  pcbinfo         pinf;

  // init the pid list
  for (i = 0; i < MAXPCB; i++)
    pid[i] = -1;

  len = get_ps(pid);

  print("Process: ");
  printi(len);
  printn();
  print("PID\tNAME\tSTATE\n");
  print("_________________________\n");
  for (i = 0; i < len; i++)
  {
    if (pid[i] != -1)
    {
      get_proc_info(pid[i], &pinf);/*
      strcpy(itos(pid[i], num), buf);
      strcat(buf, "\t");
      strcat(buf, itos(pinf.pri, num));
      strcat(buf, "\t");
      strcat(buf, itos(pinf.state, num));
      strcat(buf, "\t");
      strcat(buf, pinf.name);
      strcat(buf, "\n");
      print(buf);*/
      printi(pid[i]);
      print("\t");
      print(pinf.name);
      print("\t");

      switch (pinf.state)
      {
      case READY:
        print("READY");
        break;
      case RUNNING:
        print("RUNNING");
        break;
      case BLOCKED:
        print("BLOCKED");
        break;
      case SLEEPING:
        print("SLEEPING");
        break;
      case WAITING_IO:
        print("WAITING_IO");
        break;
      case DOING_IO:
        print("DOING_IO");
        break;
      case WAITING_PCB:
        print("WAITING_PCB");
        break;
      case OMG_ZOMBIE:
        print("OMG_ZOMBIE");
        break;
      }
      printn();
    }
  }
  print("_________________________\n");

  exit(0);
}

// params: int pid
int
tuer(int argc, char *argv[])
{
  return kill(stoi(get_arg(argv, 1)));
}

// params: char* text
void
malta(int argc, char *argv[])
{
  fprint(MALTA, get_arg(argv, 1));
  exit(0);
}

// params: no param
void
help(int argc, char *argv[])
{
  print("List of available user programs\n");
  print("-------------------------------\n");
  print("coquille\t\t\tSpawn a new shell.\n");
  print("increment n\t\t\tPrint a sequence from from 1 to n.\n");
  print("fibonacci n\t\t\tPrint the fibonacci sequence up to n numbers.\n");
  print("scroller\t\t\tCreate a process that scroll a predefined\n");
  print("\t\t\t\tstring of the LCD.\n");
  print
    ("ring nb_proc nb_loop\t\tCreate a ring of nb_proc communicating procs.\n");
  print
    ("philosopher nb_philo nb_loop:\tnb_philo philosophers try to eat/think\n");
  print("\t\t\t\tnb_loop times.\n");
  print
    ("supervision nb_sup nb_lives:\tDemonstration of process supervision.\n");
  print("ps\t\t\t\tprint the list of all the running processes.\n");
  print("chg_prio p pri\t\t\tChange the priority of the process of pid p\n");
  print("\t\t\t\twith the new priority pri.\n");
  print("tuer p\t\t\t\tKill the process of pid p.\n");
  print("malta msg\t\t\tAllow the user to write on the malta LCD.\n");
  print("-------------------------------\n");

  exit(0);
}

// params: int pid
void
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
    print("\n\tprocess state:\t\t");
    printi(res.state);
    print("\n\ttime to sleep:\t\t");
    printi(res.sleep);
    print("\n\twaiting for process:\t");
    printi(res.waitfor);
    //print("\n\tlast error:\t\t");
    //printi(res.error);
    printn();
  }
  else
  {
    print("An error occured! (code:");
    printi(err);
    print(")\n");
  }
  exit(0);
}
