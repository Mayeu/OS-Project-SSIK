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
#define MAX_PRI 30
#define BAS_PRI 15
#define MAX_ARG 4
#define ARG_SIZE 20
#define NSUPERVISED 40

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
  int       pid;          /*!< Process identifier. */
  char            name[ARG_SIZE];       /*!< Process name. */
  int        pri;          /*!< Process priority. */
  int        supervised[NSUPERVISED];      /*!< List of supervised processes. */
  int        supervisor;   /*!< List of supervisor processes. */
  int        wait;
  bool            empty;
} pcbinfo;

 /**
 * \fn int exit()
 * \brief Kill the current process.
 *
 * \return the error identifier in case of any failure
 */
int             exit();

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
 * \fn fourchette(char *name, int prio, char *argv[])
 * \brief creates a child process that differs from the parent process only in its PID and
PPID. If success, the PID of the child process is returned in the parent’s thread of execution,
and a 0 is returned in the child’s thread of execution.
 *
 * \param name the process name
 * \param the arguments list (first arg is number of arg)
 * \return the process pid
 */
int             fourchette(char *name, char *argv[]);

#endif //__PROCESS_H
