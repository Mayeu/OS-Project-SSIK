/**
 * \file process.c
 * \brief process functions
 * \author Adrien Forest
 * \version 0.1
 * \date 21 Mai 2010
 */

#include <process.h>

#include "../kernel/ksyscall.h"

 /**
 * Kill the current process.
 */
int
exit()
{
  return syscall_none(EXIT);
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
 * Sleep makes the current process sleep until ’time’ milliseconds seconds have elapsed.
 */
void
sleep(int time)
{
  syscall_one(time, SLEEP);
}

 /**
 * Creates a child process that differs from the parent process only in its PID and
PPID. If success, the PID of the child process is returned in the parent’s thread of execution,
and a 0 is returned in the child’s thread of execution.
 */
int
fourchette(char *name, char *argv[])
{
  return syscall_two((int32_t) name, (int32_t) argv, FOURCHETTE);
}

 /**
 * fill the pcb_info structure given in parameter with the pcb information. Only
not critical information is given to the user.
 */
int
get_proc_info(int pid, pcbinfo * res)
{
  return syscall_two(pid, (int32_t) res, GETPINFO);
}

 /**
 * Changes the priority of the process from the old one to the new priority ’prio’.
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
