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
#define ARG_SIZE 20
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
	int             supervisor;   /*!< List of supervisor processes. */
	int             wait;
	bool            empty;
} pcbinfo;

typedef struct
{
	int        pid;          /*!< Process identifier. */
	char            name[ARG_SIZE];       /*!< Process name. */
	int        pri;          /*!< Process priority. */
	int         supervised[MAXPCB];   /*!< List of supervised processes. */
	int         supervisor;   /*!< supervisor. */
	int        state;        /*!< Current state of the process */
	int        sleep;        /*!< Time to sleep, if state == SLEEPING */
	int        waitfor;      /*!< pid of the process you are waiting for */
	int         error;        /*!< Last error the process encountered. */
	bool            empty;        /*!< is this pcb empty ? */
} pcbinfo2;

int             kill(int pid);

int             fourchette(char *name, int prio, int argc, char *argv[]);

#endif //__PROCESS_H
