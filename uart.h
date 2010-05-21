/**
 * @file uart.h
 * @brief All functions used to manage and use the uart system
 */

#ifndef __UART_H
#define __UART_H

/**
 * Define
 */

#define FIFO_SIZE 30

/**
 * Structure
 */

/**
 * @brief A bounded fifo structure
 */

typedef struct
{
	char buffer[FIFO_SIZE];
	uint32_t length;
} fifo;

/**
 * Functions
 */

#endif /* __UART_H */

/* end of file uart.h */

