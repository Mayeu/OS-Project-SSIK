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
#include "kprocess_list.h"
#include "test.h"
#include "splash.h"

static registers_t regs;

/* kinit:
 *   Application-specific initialisation code, called
 *   from the boot code '_start' in 'asm.S'.
 */
void
kinit()
{
  char            hello[] = "Hello World";

  /* Set UART word length ('3' meaning 8 bits).
   * Do this early to enable debug printouts (e.g. kdebug_print).
   */
  tty->lcr.field.wls = 3;

  /* example of call to a print function */
  kprintln(hello);

  /* Setup storage-area for saving registers on exception. */
  kset_registers(&regs);

  /*
   * Print SSIK on the malta
   */

  kmaltaprint8("  SSIK  ");

  /*
   * Init the four list of pcb
   */

  create_pls(&pready);
  create_pls(&prunning);
  create_pls(&pwaiting);
  create_pls(&pterminate);

  /*
	* Launch test
	*/

  test();

  /*
	* Print the splash screen
	*/

  splash();

  /* Forever do nothing. */
  while (1);
}

/**
 * @brief The ancestor of the ancestor.
 *
 * First process to be launched
 */
void
init()
{
}
