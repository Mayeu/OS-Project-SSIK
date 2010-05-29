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
  char            buffer[UART_FIFO_SIZE];       /*!< A buffer to keep the character to print */
  uint32_t        length;       /*!< length of the buffer */
  uint32_t        in;           /*!< the in position */
  uint32_t        out;          /*!< the out position */
} fifo_buffer;

/**
 * UART state
 */

/**
 * @brief UART possible Status
 */
enum
{
  UART_UNUSED,                  /*!< Uart is unused */
  UART_PRINT,                   /*!< Currently printing */
  UART_READ                     /*!< Currently running */
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

/**
 * @brief Give the uart to the pcb, or if in use, block the pcb to wait
 * @param the pcb
 * @return OMGROXX if ok, FAILNOOB if the pcb is block
 */
int32_t         uart_give_to(pcb * p);

/**
 * @brief set the mode of the uart
 * @param The mode to set
 */
void            uart_set_mode(int32_t mode, char *str, uint32_t len);

/**
 * @brief Print a string using interupt
 * @param the string to print
 */

void            uart_print();

/**
 * @brief Terminated the current printing
 * @param an error code to set in the current pcb
 */
int32_t         end_of_printing(int32_t code);

/**
 * @brief Clean the register of the uart
 */
void            clean_uart();

/**
 * @brief Release the uart from is current user, and try to find a new user
 */
int32_t         uart_release(int32_t code);

/**
 * @brief This function is called by the exception
 */
void            uart_exception();

/**
 * @brief Set the current uart user
 * @param a pcb
 */
void            set_uart_user(pcb * p);

#endif /* __UART_H */

/* end of file uart.h */
