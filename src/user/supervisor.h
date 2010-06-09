/**
 * \file supervisor.h
 * \brief supervisor user program.
 * \author Adrien Forest
 * \version 0.1
 * \date 07 juin 2010
 */

#ifndef __SUPERVISOR_H
#define __SUPERVISOR_H

#define MAX_SUP 10

/**
 * Test the supervision mechanisms (exit with a return value
 * and undersand what happenned).
 * \param argc the number of arguments
 * \param argv the arguments
 */
void            supervisor(int argc, char *argv[]);

#endif //__SUPERVISOR_H
