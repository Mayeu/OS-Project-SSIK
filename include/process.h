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

int             kill(int pid);

int             fourchette(char *name, int prio, int argc, char *argv[]);

#endif //__PROCESS_H
