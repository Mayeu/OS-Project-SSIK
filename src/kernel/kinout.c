/**
 * @file kinout.c
 * @brief Implementation of function to print in tty and the malta
 */

#include <types.h>
#include <string.h>
#include <errno.h>

#include "malta.h"
#include "kernel.h"
#include "kinout.h"
#include "kprocess.h"
#include "debug.h"
#include "uart.h"
#include "kscheduler.h"


/**
 * @brief Display 8 char on the Malta display.
 *
 * If the data are too long (more than 8 char) only the 8th firts are printed
 * @param a string to print
 * @return void
 */
void
kmaltaprint8(const char *str)
{
  int             i = 0;

  malta->ledbar.reg = 0xFF;

  /*
   * Print 8 character or less
   */
  for (i = 0; i < 8 && str[i] != '\0'; i++)
    malta->asciipos[i].value = str[i];

  /*
   * if the string is less than 8 char, complete with whitespace
   */
  for (; i < 8; i++)
    malta->asciipos[i].value = 0x20;
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

/**
 * @brief Print function using interrupt.
 * @param the string to print
 * @return
 */
uint32_t
print_string(char *str)
{
  pcb            *caller;
  uint32_t        e;

  //kprintln("k, let's go!");

  /*
   * The string == NULL, we are done
   */
  if (!str)
    return OMGROXX;

  /*
   * The string start with \0, we don't have anything to do
   */
  if (*str == '\0')
    return OMGROXX;

  /*
   * Know we can really start
   */
  caller = get_current_pcb();

  e = uart_give_to(caller);

  if (e == OMGROXX)
  {
    //kprintln("You receive the uart! Enjoy");
    /*
     * The caller is the owner of the uart
     * We move it to the wait list
     */

    if (pls_move_pcb(caller, &plswaiting) != OMGROXX)
      //kprintln("Dude! just go in wait!");
      return FAILNOOB;

    /*
     * Reschedule
     */
    //kprintln("Reschedule");
    schedule();
    kdebug_assert_at(!get_current_pcb(), "kinout.c", 162);

    /*
     * Set the uart in read mode
     */
    uart_set_mode(UART_PRINT, str, 0);

    /*
     * We can print! Finally!
     */
    uart_print();

    /*
     * This is not a "real" return value, since the good one will be inserted at the
     * end of printing
     */
    return 3;
  }

  return e;
}

/**
 * @brief Read a string from the user input. Interrupt driven.
 * @param size of the reading
 * @param a buffer to keep the string
 * @return an error code, OMGROXX, INVARG, or any error returned by calling functions
 */
uint32_t
read_string(uint32_t length, char *buf)
{
  pcb            *caller;
  uint32_t        e;            /*error code */

  /*
   * If length == 0 or buf == NULL we can not use the buffer
   */
  if (!length || !buf)
    return INVARG;

  /*
   * If the length == 1, the pcb ask for an empty string
   * so we return an empty string
   */
  if (length == 1)
  {
    *buf = '\0';
    return OMGROXX;
  }

  /*
   * Know we can really start
   */
  caller = get_current_pcb();

  /*e = uart_give_to(caller); *//*TODO: We try to give the uart to the caller */

  if (e == 1)
  {
    /*
     * The caller is the owner of the uart
     */
    /*pcb_wait(caller, PCB_WAIT_IO); *//*TODO: We block the process in the reading state */
    /*uart_set_mode(UART_READ, buf, length); *//*TODO: set the uart in reading mode */

    /*
     * We can read! Finally!
     */
    /*uart_read(); */ /*TODO*/
      return OMGROXX;
  }

  return e;
}

/* end of file kinout.c */
