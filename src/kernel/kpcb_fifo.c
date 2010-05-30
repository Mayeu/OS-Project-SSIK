/**
 * \file kpcb_fifo.c
 * \brief kpcb fifo list functions
 *
 */

#include "kpcb_fifo.h"


/*
 * Global variable
 */

static fifo_p   fifo_pcb;

/**
 * \private
 * \brief reset the fifo list to default value
 * \param m the list of messages
 * \return void
 */
void
reset_fifo_p()
{
  fifo_pcb.in = 0;
  fifo_pcb.out = 0;
  fifo_pcb.length = 0;
}

/**
 * \private
 * \brief push the message to the end of the list
 *
 * \param m the list of messages
 * \param mess the list of messages
 * \return an error code
 */
uint32_t
push_fifo_p(pcb * p)
{
  if (fifo_pcb.length >= MAXPCB)
    return OUTOMEM;

  fifo_pcb.buffer[fifo_pcb.in] = p;
  fifo_pcb.length++;
  fifo_pcb.in = (fifo_pcb.in + 1) % MAXPCB;

  return OMGROXX;
}


/**
 * \private
 * \brief pop the message from the beginning of the list
 *
 * \param m the list of messages
 * \param mess the list of messages
 * \return an error code
 */
uint32_t
pop_fifo_p(pcb * p)
{
  if (fifo_pcb.length >= MAXPCB)
    return OUTOMEM;

  fifo_pcb.buffer[fifo_pcb.in] = p;
  fifo_pcb.length++;
  fifo_pcb.in = (fifo_pcb.in + 1) % MAXPCB;

  return OMGROXX;
}
