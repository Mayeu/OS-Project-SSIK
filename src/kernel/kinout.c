/**
 * @file kinout.c
 * @brief Implementation of function to print in tty and the malta
 */

#include <types.h>
#include "malta.h"

#include "kinout.h"

/**
 * @brief Display a value on the Malta display.
 * @param an uint32_t to print
 * @return void
 */
void
kmaltaprint(uint32_t word)
{
}

/**
 * @brief Print a char on the tty
 * @param the string
 * @return void
 */
void
kprint_char(char c)
{
  while (!tty->lsr.field.thre); /* poll untill we can print */
  tty->thr = c;                 /* print the char */
}

/**
 * @brief Print a string on the tty
 * @param the string
 * @return void
 */
void
kprint(char *text)
{
  while (*text != '\0')
  {
    if (*text == '\n')
      kprint_char('\r');
    kprint_char(*text);
    text++;
  }
}

/**
 * @brief Print a string on the tty with an \n at the end
 * @param the string
 * @return void
 */
void
kprintn(char *text)
{
  kprint(text);
  kprint("\n");
}

/* end of file kinout.c */
