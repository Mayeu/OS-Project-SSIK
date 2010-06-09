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
#include <errno.h>
#include "../kernel/ksyscall.h"

 /**
 * Return the argument i from the char* array of arguments.
 * \private
 */
char           *
get_arg(char *argv[], int i)
{
  return (char *) argv + (i * ARG_SIZE * sizeof(char));
}

 /**
 * Kill the current process.
 * \private
 */
int
exit(int status)
{
  return syscall_one(status, EXIT);
}

 /**
 * Kill the process with the right pid.
 * \private
 */
int
kill(int pid)
{
  return syscall_one(pid, KILL);
}

 /**
 * Sleep makes the current process sleep until 'time' milliseconds seconds have elapsed.
 * \private
 */
void
sleep(int time)
{
  syscall_one(time, SLEEP);
}

 /**
 * Block the process 'pid' until someone call the wake_up() function.
 * \private
 */
int
block(int pid)
{
  return syscall_one(pid, BLOCK);
}

 /**
 * Unblock the process with the pid 'pid'.
 * \private
 */
int
unblock(int pid)
{
  return syscall_one(pid, UNBLOCK);
}

 /**
 * Wait for the process 'pid' to exit and set the status
variable with its exit code.
 * \private
 */
int
wait(int pid, int *status)
{
  int             r;
  do
    r = syscall_two(pid, (int32_t) status, WAIT);
  while (r != OMGROXX && r != NOTFOUND);

  return r;
}

 /**
 * Creates a new process with the program identified by its name 'name'. The
program must be stored in the program list of the OS.
 * \private
 */
int
fourchette(char *name, int prio, int argc, char *argv[])
{
  return syscall_three(prio, argc, (int32_t) argv, FOURCHETTE);
}

 /**
 * Fill the pcb_info structure given in parameter with the pcb information. Only
not critical information is given to the user.
 * \private
 */
int
get_proc_info(int pid, pcbinfo * res)
{
  return syscall_two(pid, (int32_t) res, GETPINFO);
}

 /**
 * Changes the priority of the process from the old one to the new priority 'prio'.
 * \private
 */
int
chg_pri(int pid, int newprio)
{
  return syscall_two(pid, newprio, CHGPPRI);
}

 /**
 * Get the current process pid.
 * \private
 */
int
get_pid(void)
{
  return syscall_none(GETPID);
}

 /**
 * Fill the int array with all the corresponding pids.
 * \private
 */
int
get_ps(int *pid)
{
  return syscall_one((int32_t) pid, GETALLPID);
}
