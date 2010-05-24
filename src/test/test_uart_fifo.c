/**
 * @file uart_info_fifo.c
 * @brief test the fifo buffer of the uart module
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/uart.h"

uint32_t test_uart_reset_fifo(void);
uint32_t test_uart_push_fifo(void);
uint32_t test_uart_pop_fifo(void);

void test_uart_fifo()
{
	int e;
	char c;

	kprintln("-----------TEST MODULE UART FIFO BEGIN------------");

	kprint("Test reset_fifo_buffer\t\t\t\t");
	e = test_uart_reset_fifo() ;
	if (e == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprint("Test push_fifo_buffer\t\t\t\t");
	e = test_uart_push_fifo();
	if ( e == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprint("Test pop_fifo_buffer\t\t\t\t");
	e = test_uart_pop_fifo();
	if (e  == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprintln("------------TEST MODULE UART FIFO END-------------");
	kprintln("");
}

/* Test the uart reset */
uint32_t test_uart_reset_fifo(void)
{
	fifo_buffer *fifo;
	uint32_t i;

	i = 0;

	reset_fifo_buffer();
	fifo = get_fifo_buffer();

	if (fifo->in != 0)
		return -1;

	if (fifo->out != 0)
		return -2;

	if (fifo->length != 0)
		return -3;

	return OMGROXX;
}

uint32_t test_uart_push_fifo(void)
{
	fifo_buffer *fifo;
	uint32_t j;

	reset_fifo_buffer();
	fifo = get_fifo_buffer();

	if(push_fifo_buffer('a') != OMGROXX)
	{
		return -1;
	}

	if(fifo->buffer[fifo->out] != 'a')
		return -2;

	if(fifo->out != 0)
		return -3;

	if(fifo->in != 1)
		return -4;

	if(fifo->length != 1)
		return -5;

	/* fill all the buffer wiht 'a' */
	for (j = 1; j < 30; j++)
		push_fifo_buffer('a') ;

	if(fifo->length != 30)
		return -6;

	if(push_fifo_buffer('a') != OUTOMEM)
		return -7;

	return OMGROXX;
}

uint32_t test_uart_pop_fifo(void)
{
	return OMGROXX;
}

