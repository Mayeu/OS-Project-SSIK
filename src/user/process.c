/**
 * \file process.c
 * \brief process functions
 * \author Adrien Forest
 * \version 0.1
 * \date 21 Mai 2010
 */

#include <process.h>
#include <string.h>
#include <stdio.h>
#include "../kernel/ksyscall.h"

 /**
 * return the argument i from the char* array of arguments.
 */
char* get_arg(char* argv[], int i)
{
	return (char*)argv + (i * ARG_SIZE * sizeof(char));
}

 /**
 * Kill the current process.
 */
int
exit(int status)
{
  return syscall_one(status, EXIT);
}

 /**
 * Kill the process with the right pid.
 */
int
kill(int pid)
{
  return syscall_one(pid, KILL);
}

 /**
 * Sleep makes the current process sleep until 'time' milliseconds seconds have elapsed.
 */
void
sleep(int time)
{
  syscall_one(time, SLEEP);
}

 /**
 * Block the process 'pid' until someone call the wake_up() function.
 */
int
block(int pid)
{
	return syscall_one(pid, BLOCK);
}

 /**
 * Wake up the process with the pid 'pid'.
 */
int
wake_up(int pid)
{
	return syscall_one(pid, WAKEUP);
}

 /**
 * Wait for the process 'pid' to exit and set the status
variable with its exit code.
 */
int
wait(int pid, int *status)
{
	return syscall_two(pid, (int32_t)status, WAIT);
}

 /**
 * Creates a new process with the program identified by its name 'name'. The
program must be stored in the program list of the OS.
 */
int fourchette(char *name, int prio, int argc, char *argv[])
{
	int i;
	char pr[3];
	char new_args[MAX_ARG+1][ARG_SIZE];

	// add the process priority to the new arg array
	strcpy(itos(prio, pr), new_args[0]);
	// copy all the arguments
	for (i=0; i<argc; i++)
		strcpy(get_arg(argv, i), new_args[i+1]);
	
  return syscall_three((int32_t) name, argc, (int32_t) new_args, FOURCHETTE);
}

 /**
 * Fill the pcb_info structure given in parameter with the pcb information. Only
not critical information is given to the user.
 */
int
get_proc_info(int pid, pcbinfo * res)
{
  return syscall_two(pid, (int32_t) res, GETPINFO);
}

 /**
 * Changes the priority of the process from the old one to the new priority 'prio'.
 */
int
chgpri(int pid, int newprio)
{
  return syscall_two(pid, newprio, CHGPPRI);
}

 /**
 * Get the current process pid.
 */
int get_pid(void)
{
	return syscall_none(GETPID);
}

 /**
 * Fill the char* array with the name of all the processes running and
the int array with all the corresponding pids.
 */
int get_ps(char *pnames[ARG_SIZE], int *pid)
{
	return syscall_two((int32_t)pnames, (int32_t)pid, GETPS);
}
