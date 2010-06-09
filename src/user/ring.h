/**
 * \file ring.h
 * \brief Ring user program.
 * \author Christophe Carasco
 * \version 0.1
 * \date 25 mai 2010
 */

#ifndef __RING_H
#define __RING_H

/**
 * Program that tests the messages. It creates the specified number of process
 * that will send a message to the next process (and receive from the previous)
 * as a ring.
 * \param argc the number of arguments
 * \param argv the arguments
 */
void            ring(int argc, char *argv[]);

#endif //__RING_H
