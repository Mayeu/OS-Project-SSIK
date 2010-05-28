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
int chg_prio(int argc, char* argv[])
{
	int pid, prio;

	if (argc < 3)
	{
		println("Need two arguments (pid and new priority)");
		exit(-1);
	}

	pid  = stoi(get_arg(argv, 1));
	prio = stoi(get_arg(argv, 2));

	exit(0);
	return chg_pri(pid, prio);
}

// params: no param
int ps(int argc, char* argv[])
{
	char *pnames[ARG_SIZE];
	int  *pid;
	int  i, len, my_pid;

	len = get_ps(pnames, pid);
		
	printiln(len);

	//my_pid = get_pid();
	my_pid = 185;

	println("PID\tNAME");
	println("________________");
	for (i=0; i<len; i++)
	{
		if (my_pid == pid[i])
			print("* ");
		printi(pid[i]);
		print("\t");
		println(pnames[i]);
	}
	println("________________");

	exit(47);
	return 0;
}
// params: int pid
int tuer(int argc, char* argv[])
{
	return kill(stoi(get_arg(argv, 1)));
}

// params: char* text
int malta(int argc, char* argv[])
{
	return fprint(MALTA, get_arg(argv, 1));
}

// params: no param
int help(int argc, char* argv[])
{
	println("List of available user programs");
	println("-------------------------------");
	println("coquille: Spawn a new shell.");
	println("increment n: Print a sequence from from 1 to n.");
	println("fibonacci n: Print the fibonacci sequence up to n numbers.");
	println("scroller: Create a process that scroll a predefined string of the LCD.");
	println("ring nb_proc nb_loop: Create a ring of nb_proc communicating processes.");
	print("philosopher nb_philo nb_loop: nb_philo philosophers ");
		println("try to eat/think nb_loop times.");
	println("supervision: demonstration of process supervision.");
	println("ps: print the list of all the running processes.");
	print("chg_prio p pri: Change the priority of the process of pid p ");
		println("with the new priority pri.");
	println("tuer p: Kill the process of priority p.");
	println("malta str: Print the string str on the malta LCD.");

	exit(0);
	return 0;
}

// params: int pid
int proc_info(int argc, char* argv[])
{
	int pid, err, i;
	pcbinfo res;
	
	pid = stoi(get_arg(argv, 1));
	err = get_proc_info(pid, &res);
	if (err == 0)
	{
		print("Information about process pid = ");printiln(pid);
		print("\tname:\t\t\t");println(res.name);
		print("\tpriority:\t\t");printiln(res.pri);
		print("\tsupervised processes:\t");
		for (i=0; i<MAXPCB; i++)
		{
			if (res.supervised[i] != -1)
			{
				printiln(res.supervised[i]);
				print(" ");
			}
		}
		print("\n\tsupervisor process:\t");printiln(res.supervisor);
	}
	else
	{
		print("An error occured! (code:");
		printi(err);
		println(")");
	}
	exit(0);
	return 0;
}

