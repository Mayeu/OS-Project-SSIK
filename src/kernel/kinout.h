/**
 * @file kinout.h
 * @brief Offer function to print in the tty and malta, and to get the stdin from the tty
 */

#ifndef __KINOUT_H
#define __KINOUT_H

#include <types.h>

/**
 * @brief Display a value on the Malta display.
 * @param an uint32_t to print
 * @return void
 */
void            kmaltaprint(uint32_t word);

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

#endif

/* end of file kinout.h*/
