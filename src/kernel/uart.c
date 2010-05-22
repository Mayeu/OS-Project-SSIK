/**
 * @file uart.c
 * @brief implemet the uart fuctions
 */

#include <stdlib.h>
#include "malta.h"
#include "uart.h"

/*
 * Global variable
 */

static fifo_buffer uart_fifo;

/*
 * Fifo functions
 */

/**
 * @brief reset the fifo buffer to default value
 * @param void
 * @return void
 */
void
reset_fifo_buffer(void)
{
  uart_fifo.in = 0;
  uart_fifo.out = 0;
  uart_fifo.length = 0;
}

/**
 * @brief push a char in the fifo buffer
 * @param the char to push
 * @return OMGROXX if there isan empty space OUTOMEM otherwise
 */
uint32_t
push_fifo_buffer(char c)
{
  if (uart_fifo.length <= UART_FIFO_SIZE)
    return OUTOMEM;

  uart_fifo.buffer[uart_fifo.in++] = c;
  uart_fifo.length++;

  if (uart_fifo.in >= UART_FIFO_SIZE)
    uart_fifo.in = 0;

  return OMGROXX;
}

/**
 * @brief pop a char from the fifo buffer
 * @parama a pointer to the char to pop
 * @return OMGROXX if the buffer is not empty, NULLPTR if the char given is null,  FAILNOOB otherwise
 */
uint32_t
pop_fifo_buffer(char *c)
{
  if (c == NULL)
    return NULLPTR;

  if (uart_fifo.length == 0)
    return FAILNOOB;

  *c = uart_fifo.buffer[uart_fifo.out++];
  uart_fifo.length--;

  if (uart_fifo.out >= UART_FIFO_SIZE)
    uart_fifo.out = 0;

  return OMGROXX;
}

/*
 * UART management functions
 */

/**
 * @brief initialize the uart
 * @param void
 * @return void
 */
void
uart_init(void)
{
  /* Set UART word length ('3' meaning 8 bits).
   * Do this early to enable debug printouts (e.g. kdebug_print).
   */
  tty->lcr.field.wls = 3;

  /* Generate interrupts when data is received by UART. */
  tty->ier.field.erbfi = 1;

  /* Some obscure bit that need to be set for UART interrupts to work. */
  tty->mcr.field.out2 = 1;
}
