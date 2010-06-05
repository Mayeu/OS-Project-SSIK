/**
 * @file uart_info_fifo.c
 * @brief test the fifo buffer of the uart module
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/kpcb_fifo.h"

uint32_t        test_reset_fifo_p(void);
uint32_t        test_push_fifo_p(void);
uint32_t        test_pop_fifo_p(void);

void
test_kpcb_fifo_p()
{
  int             e;
  char            c;

  kprintln("\n-----------TEST MODULE KPCB FIFO BEGIN------------");

  kprint("Test reset_fifo_buffer\t\t\t\t");
  e = test_reset_fifo_p();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test push_fifo_buffer\t\t\t\t");
  e = test_push_fifo_p();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pop_fifo_buffer\t\t\t\t");
  e = test_pop_fifo_p();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprintln("------------TEST MODULE KPCB FIFO END-------------\n");
}

/* Test the uart reset */
uint32_t
test_reset_fifo_p(void)
{
  fifo_p         *fifo;
  uint32_t        i;

  i = 0;

  reset_fifo_p();
  fifo = get_fifo_p();

  if (fifo->in != 0)
    return -1;

  if (fifo->out != 0)
    return -2;

  if (fifo->length != 0)
    return -3;

  return OMGROXX;
}

uint32_t
test_push_fifo_p(void)
{
  fifo_p         *fifo;
  uint32_t        j;
  pcb             pcbs[MAXPCB + 1];

  reset_fifo_p();
  fifo = get_fifo_p();

  if (push_fifo_p(&pcbs[0]) != OMGROXX)
  {
    return -1;
  }

  if (fifo->buffer[fifo->out] != &pcbs[0])
    return -2;

  if (fifo->out != 0)
    return -3;

  if (fifo->in != 1)
    return -4;

  if (fifo->length != 1)
    return -5;

  for (j = 1; j < MAXPCB; j++)
    if (push_fifo_p(&pcbs[j]) != OMGROXX)
      return -6;

  if (fifo->length != MAXPCB)
    return -7;

  if (push_fifo_p(&pcbs[50]) != OUTOMEM)
    return -8;

  return OMGROXX;
}

uint32_t
test_pop_fifo_p(void)
{
  fifo_p         *fifo;
  uint32_t        j;
  pcb             pcbs[MAXPCB + 1], *p;

  fifo = get_fifo_p();
  reset_fifo_p();

  if (pop_fifo_p() != NULL)
    return -1;

  push_fifo_p(&pcbs[0]);

  p = pop_fifo_p();

  if (p == NULL)
    return -2;

  if (p != &pcbs[0])
    return -3;

  if (fifo->out != 1)
    return -4;

  if (fifo->length != 0)
    return -5;

  for (j = 0; j < MAXPCB; j++)
    push_fifo_p(&pcbs[j]);

  p = pop_fifo_p();

  if (fifo->length != MAXPCB - 1)
    return -6;

  return OMGROXX;
}
