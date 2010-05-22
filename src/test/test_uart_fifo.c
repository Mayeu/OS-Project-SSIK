/**
 * @file uart_info_fifo.c
 * @brief test the fifo buffer of the uart module
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../uart.h"

void test_uart_reset_fifo(void)
{
	kprint("Test reset_fifo_buffer: ");

}

void test_uart_fifo()
{
	kprintln("\n------TEST MODULE UART FIFO BEGIN------");

	test_uart_reset_fifo();
	test_uart_pop_fifo();
	test_uart_push();

	kprintln("-------TEST MODULE UART FIFO END-------");
}
