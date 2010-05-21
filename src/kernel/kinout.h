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
 * @brief Print a string on the tty
 * @param the string
 * @return void
 */
void            kprint(char print_array[]);

#endif

/* end of file kinout.h*/
