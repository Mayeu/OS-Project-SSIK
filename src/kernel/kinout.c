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
  if (c == '\n')
  {
    tty->thr = '\r';            /* print the char */
    while (!tty->lsr.field.thre);       /* poll untill we can print */
  }
  tty->thr = c;
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
    kprint_char(*text);
    text++;
  }
}

/**
 * @brief Print a \n on the tty
 * @param void
 * @return void
 */
void
kprintn(void)
{
  kprint_char('\n');
}

/**
 * @brief Print a string on the tty with an \n at the end
 * @param the string
 * @return void
 */
void
kprintln(char *text)
{
  kprint(text);
  kprintn();
}

/* end of file kinout.c */
