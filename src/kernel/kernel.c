/*
 * example_timer:
 *   An example of how to setup the MIPS/Malta board
 *   to periodically increase a number on the display.
 */

#include <process.h>
#include <stdio.h>
#include <mips4kc.h>
#include "asm.h"
#include "malta.h"
#include "debug.h"
#include "kernel.h"
#include "kinout.h"
#include "kprocess.h"
#include "test.h"
#include "splash.h"
#include "kscheduler.h"

static registers_t regs;

/* kinit:
 *   Application-specific initialisation code, called
 *   from the boot code '_start' in 'asm.S'.
 */
void
kinit()
{
  char            hello[] = "Hello World";

  status_reg_t    and, or;
  and.reg = 0xFFFFFFFF;
  and.field.exl = 0;            // EXL clear
  and.field.um = 0;             // Kernel mode
  and.field.im = 0;             // Clear IMs
  and.field.bev = 0;            // normal exception vector
  and.field.erl = 0;            // ERL clear

  or.reg = 0x00000000;
  or.field.ie = 1;              //interrupts enabled
  or.field.im = 0x84;           //useful interrupts enabled (those used by tty0 and the timer)
  kset_sr(and.reg, or.reg);

  /* Set UART word length ('3' meaning 8 bits).
   * Do this early to enable debug printouts (e.g. kdebug_print).
   */
  tty->lcr.field.wls = 3;

  /* example of call to a print function */
  kprintln(hello);

  /* Setup storage-area for saving registers on exception. */
  kset_registers(&regs);

  /* Update the status register to enable exceptions. */
  kset_sr(0xFFBF00E8, 0x10008001);

  /*
   * Print SSIK on the malta
   */
  kmaltaprint8("  SSIK  ");

  /*
   * Init the four list of pcb
   */
  /*
     create_pls(&pready);
     create_pls(&prunning);
     create_pls(&pwaiting);
     create_pls(&pterminate);
   */
  pcls_reset(&pclsready);
  pcls_reset(&pclsrunning);
  pcls_reset(&pclswaiting);
  pcls_reset(&pclsterminate);

  reset_used_stack();

  set_current_pcb(NULL);
  p_error = &kerror;

  char            arg[4][20];

  if (create_proc("init", MAX_PRI, (char **) arg) < 0)
  {
    kprintln("FAILNOOB");
    while (1);
  }
  /*
   * Launch test
   */
  //test();

  /*
   * Print the splash screen
   */

  splash();

  /*
   * Fire the scheduler
   */
  kload_timer(QUANTUM);
  schedule();

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
  while (1)
  {
    print("Hey! I'm going to sleep a little :)");
    sleep(500);
  }
}
