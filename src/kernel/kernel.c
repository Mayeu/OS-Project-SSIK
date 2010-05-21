/*
 * example_timer:
 *   An example of how to setup the MIPS/Malta board
 *   to periodically increase a number on the display.
 */

#include "asm.h"
#include "malta.h"
#include "debug.h"
#include "kernel.h"
#include "kinout.h"

static registers_t regs;

/* kinit:
 *   Application-specific initialisation code, called
 *   from the boot code '_start' in 'asm.S'.
 */
void
kinit()
{
  char           *hello;

  hello = "Hello World";

  /* Set UART word length ('3' meaning 8 bits).
   * Do this early to enable debug printouts (e.g. kdebug_print).
   */
  tty->lcr.field.wls = 3;

  /* example of call to a print function */
  kprint(hello);

  /* Setup storage-area for saving registers on exception. */
  kset_registers(&regs);

  /* Forever do nothing. */
  while (1);
}


/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */
void
kexception()
{
  cause_reg_t     cause;

  /* Make sure that we are here because of a timer interrupt. */
  cause.reg = kget_cause();
  kdebug_assert(cause.field.exc == 0);  /* External interrupt */
  kdebug_assert(cause.field.ip & 0x80); /* Timer interrupt */

  /* Reload timer for another 100 ms (simulated time) */
  kload_timer(100 * timer_msec);

  /* Icrease the number on the Malta display. */
} /**/
