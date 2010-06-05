/**
 * \file kpcb_fifo.h
 * \brief kpcb fifo list functions
 *
 */

#ifndef __KPCB_FIFO_H
#define __KPCB_FIFO_H

#include <stdlib.h>
#include <process.h>
#include <errno.h>
#include "kprocess.h"
#include "include/types.h"

/**
 * \struct mls
 * \brief Message list representation.
 *
 * A message list is represented as an array with two pointers to
 * simulate a fifo. out is the index of first element to read and
 * in is the index of the array where to push the next element.
 */
typedef struct
{
  pcb            *buffer[MAXPCB];       /*!< list of processes. */
  uint32_t        length;       /*!< number of elements */
  uint32_t        in;           /*!< index where to push */
  uint32_t        out;          /*!< first element to pop */
} fifo_p;

/**
 * \brief reset the fifo list to default value
 * \param m the list of messages
 * \return void
 */
void            reset_fifo_p();

/**
 * \brief push the message to the end of the list
 *
 * \param m the list of messages
 * \param mess the list of messages
 * \return an error code
 */
uint32_t        push_fifo_p(pcb * p);


/**
 * \brief pop the pcb from the beginning of the list
 *
 * \param p the address where the pcb is
 * \return an error code
 */
pcb            *pop_fifo_p();

fifo_p         *get_fifo_p();
#endif
