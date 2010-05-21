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
#include "test.h"

static registers_t regs;

/* kinit:
 *   Application-specific initialisation code, called
 *   from the boot code '_start' in 'asm.S'.
 */
void
kinit()
{
  char            hello[] = "Hello World\nHello SSIK\n", str[] = "<>\n";

  /* Set UART word length ('3' meaning 8 bits).
   * Do this early to enable debug printouts (e.g. kdebug_print).
   */
  tty->lcr.field.wls = 3;

  /* example of call to a print function */
  kprint(hello);
  kprint(str);

  /* Setup storage-area for saving registers on exception. */
  kset_registers(&regs);

  /**
	* Launch test
	*/

  test();

  /* Forever do nothing. */
  while (1);
}
