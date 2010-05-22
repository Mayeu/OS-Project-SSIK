/**
 * @file uart.h
 * @brief All functions used to manage and use the uart system
 */

#ifndef __UART_H
#define __UART_H

#include <types.h>
#include <errno.h>

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
  char            buffer[FIFO_SIZE];
  uint32_t        length;
  uint32_t start;
  uint32_t end;
} fifo_buffer;

/*
 * Functions
 */

/**
 * @brief reset the fifo buffer to default value
 * @param void
 * @return void
 */
void reset_fifo_buffer(void);

/**
 * @brief push a char in the fifo buffer
 * @param the char to push
 * @return OMGROXX if there isan empty space OUTOMEM otherwise
 */
uint32_t push_fifo_buffer(char c);

/**
 * @brief pop a char from the fifo buffer
 * @parama a pointer to the char to pop
 * @return OMGROXX if the buffer is not empty, FAILNOOB otherwise
 */
uint32_t pop_fifo_buffer(char *c);

/*
 * bounded fifo function
 */

/*
 * UART functions
 */

/**
 * @brief initialize the uart
 * @param void
 * @return void
 */
void uart_init(void);

#endif /* __UART_H */

/* end of file uart.h */
