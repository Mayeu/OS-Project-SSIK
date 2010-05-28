/**
 * @file kinout.h
 * @brief Offer function to print in the tty and malta, and to get the stdin from the tty
 */

#ifndef __KINOUT_H
#define __KINOUT_H

#include <types.h>

/**
 * @brief Display 8 char on the Malta display.
 *
 * If the data are too long (more than 8 char) only the 8th firts are printed
 * @param a string to print
 * @return void
 */
void            kmaltaprint8(const char *str);

/**
 * @brief Print a char on the tty
 * @param the string
 * @return void
 */
void            kprint_char(char c);

/**
 * @brief Print a string on the tty
 * @param the string
 * @return void
 */
void            kprint(char *text);

/**
 * @brief Print a \n on the tty
 * @param void
 * @return void
 */
void            kprintn(void);

/**
 * @brief Print a string on the tty with a \n at the end
 * @param the string
 * @return void
 */
void            kprintln(char *text);

/**
 * @brief Print function using interrupt.
 * @param the string to print
 * @return OMGROXX if everything goes well
 */
uint32_t        print_string(char *str);

/*
 * @brief Read function using interrupt
 * @param the length of the string
 * @param the string to print
 */
uint32_t        read_string(uint32_t length, char *buf);

#endif

/* end of file kinout.h*/
