/**
 * \file process.h
 * \brief process functions
 * \author Adrien Forest
 * \version 0.1
 * \date 21 Mai 2010
 */

#ifndef __PROCESS_H
#define __PROCESS_H

#define MIN_PRI 0
#define MAX_PRI 42
#define BAS_PRI 21
#define MAX_ARG 4
#define ARG_SIZE 60
#define MAXPCB 40

#include <stdlib.h>

/**
 * \struct pcbinfo
 * \brief Copy of a process pcb.
 *
 * A copy has to be created and passed to make sure that the real pcb will not
 * be affected in any way.
 */
typedef struct
{
  int             pid;          /*!< Process identifier. */
  char            name[ARG_SIZE];       /*!< Process name. */
  int             pri;          /*!< Process priority. */
  int             supervised[MAXPCB];   /*!< List of supervised processes. */
  int             supervisor;   /*!< supervisor. */
  int             state;        /*!< Current state of the process */
  int             sleep;        /*!< Time to sleep, if state == SLEEPING */
  int             waitfor;      /*!< pid of the process you are waiting for */
  int             error;        /*!< Last error the process encountered. */
  bool            empty;        /*!< is this pcb empty ? */
  int             nb_msg;       /*!< number of messages */
} pcbinfo;

#ifndef __PROCESS_STATE
#define __PROCESS_STATE
enum
{
  READY,
  RUNNING,
  BLOCKED,
  SLEEPING,
  WAITING_IO,
  DOING_IO,
  WAITING_PCB,
  OMG_ZOMBIE
};
#endif

 /**
 * \fn char* get_arg(char* argv[], int i)
 * \brief Return the argument i from the char* array of arguments.
 *
 * \param argv the array of arguments
 * \param i the index of the char* that will be returned
 * \return the argument i
 */
char           *get_arg(char *argv[], int i);

 /**
 * \fn int exit(int status)
 * \brief Kill the current process.
 *
 * \param status the value returned by the process when exiting
 * \return the error identifier in case of any failure
 */
int             exit(int status);

 /**
 * \fn int kill(int pid)
 * \brief Kill the process with the right pid.
 *
 * \param pid the pid of the process to kill
 * \return the error identifier in case of any failure
 */
int             kill(int pid);

 /**
 * \fn void sleep(int time)
 * \brief Sleep makes the current process sleep until ’time’ milliseconds seconds have elapsed.
 *
 * \param time sleep for the specified number of milliseconds
 */
void            sleep(int time);

 /**
 * \fn int block(int pid)
 * \brief Block the process 'pid' until someone call the wake_up() function.
 *
 * \param pid the pid of the process to block
 * \return the error identifier in case of any failure
 */
int             block(int pid);

 /**
 * \fn int unblock(int pid)
 * \brief Unblock the process with the pid 'pid'.
 *
 * \param pid the pid of the process to wake up
 * \return the error identifier in case of any failure
 */
int             unblock(int pid);

 /**
 * \fn int wait(int pid, int *status)
 * \brief Wait for the process 'pid' to exit and set the status
variable with its exit code.
 *
 * \param pid the pid of the process to wait for
 * \param status the value of the waited process return code
 * \retun the error identifier in case of any failure
 */
int             wait(int pid, int *status);

 /**
 * \fn int fourchette(char *name, int argc, char *argv[])
 * \brief Creates a new process with the program identified by its name 'name'. The program must be stored in the program list of the OS.
 *
 * \param name the process name
 * \param prio the priority of the process to create
 * \param argc the number of arguments in the argv array
 * \param the arguments list (first arg is the name of the program)
 * \return the process pid (>0) or an negative error in case of any failure
 */
int             fourchette(char *name, int prio, int argc, char *argv[]);

 /**
 * \fn int get_proc_info(int pid)
 * \brief Fill the pcb_info structure given in parameter with the pcb information. Only
not critical information is given to the user.
 *
 * \param pid the pid of the process
 * \param res the pcbinfo structure to fill
 * \return the error identifier in case of any failure
 */
int             get_proc_info(int pid, pcbinfo * res);

 /**
 * \fn int chg_pri(int pid, int newprio)
 * \brief Changes the priority of the process from the old one to the new priority ’prio’.
 *
 * \param pid the pid of the process
 * \param newprio the new priority of the process
 * \return the error identifier in case of any failure
 */
int             chg_pri(int pid, int newprio);

 /**
 * \fn int get_pid(void)
 * \brief Get the current process pid.
 *
 * \return the process pid
 */
int             get_pid(void);

 /**
 * \fn int get_ps(int *pid)
 * \brief Fill the int array with all the corresponding pids.
 *
 * \param pid  the array of processes pid to fill
 * \return the size of the array
 */
int             get_ps(int *pid);

#endif //__PROCESS_H
