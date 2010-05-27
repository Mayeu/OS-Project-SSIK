/**
 * @file uart.h
 * @brief All functions used to manage and use the uart system
 */

#ifndef __UART_H
#define __UART_H

#include <types.h>
#include <errno.h>
#include <process.h>
#include "kpcb.h"
#include "kernel.h"
#include "kprocess.h"

/**
 * Define
 */

#define UART_FIFO_SIZE 30

/**
 * Structure
 */

/**
 * @brief A bounded fifo char structure
 */

typedef struct
{
  char            buffer[UART_FIFO_SIZE];
  uint32_t        length;
  uint32_t        in;
  uint32_t        out;
} fifo_buffer;

/**
 * UART state
 */

enum
{
  UART_UNUSED,
  UART_PRINT,
  UART_READ
};

/*
 * Functions
 */

/*
 * bounded fifo function
 */

/**
 * @brief reset the fifo buffer to default value
 * @param void
 * @return void
 */
void            reset_fifo_buffer(void);

/**
 * @brief push a char in the fifo buffer
 * @param the char to push
 * @return OMGROXX if there isan empty space OUTOMEM otherwise
 */
uint32_t        push_fifo_buffer(char c);

/**
 * @brief pop a char from the fifo buffer
 * @param a pointer to the char to pop
 * @return OMGROXX if the buffer is not empty, FAILNOOB otherwise
 */
uint32_t        pop_fifo_buffer(char *c);

/**
 * @brief return a pointer to the fifo_buffer struct
 *
 * --!!! This function is here for test purpose only! Don't use it !!!--
 *
 * @param void
 * @return a pointer to the fifo buffer
 */
fifo_buffer    *get_fifo_buffer();

/*
 * UART functions
 */

/**
 * @brief initialize the uart
 * @param void
 * @return void
 */
void            uart_init(void);

/*
 *
 */
int32_t         uart_give_to(pcb * p);

void            uart_set_mode(int32_t mode, char *str, uint32_t len);
/* prints a character from out_buffer and sets the device to interrupt when it is done printing (only if there are remaining characters to print) */
void            uart_print();
/* ends the use of the device by its current owner by waking it up. It also release the device and put the return value in the PCB */
int32_t         end_of_printing(int32_t code);

void            clean_uart();

int32_t         uart_release(int32_t code);
void            uart_exception();
void            set_uart_user(pcb * p);

#endif /* __UART_H */

/* end of file uart.h */
