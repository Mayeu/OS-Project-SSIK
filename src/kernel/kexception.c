/**
 * \file kexception.c
 * \brief Handles the exceptions and interruptions.
 * \author Christophe Carasco
 * \version 0.1
 * \date 21 mai 2010
 *
 */

#include <string.h>
#include "asm.h"
#include "debug.h"
#include "kinout.h"
#include "mips4kc.h"
#include "kscheduler.h"
#include "ksyscall.h"
#include "kscheduler.h"
#include "ksleep.h"
#include "kprogram.h"

void
kexception()
{
  cause_reg_t     cause;
  cause.reg = kget_cause();

  //kdebug_println("Exception in");

  if (cause.field.exc == 8)     // external exception (syscall)
  {
    registers_t    *reg;
    reg = &(get_current_pcb()->registers);
    reg->epc_reg += 4;

    syscall_handler(reg);

    kset_cause(~0x60, 0);       // Acknowledge
  }
  else if (cause.field.exc == 0)        // internal exception
  {
    if (cause.field.ip & 4)     // uart interrupt
    {
      //uart_exception();    /** TODO: add uart_exception to uart file */
      kset_cause(~0x1000, 0);   // Acknowledge UART interrupt.
    }
    else if (cause.field.ip & 0x80)     // timer exception
    {
      // check if there are some processes to wake up and reschedule all processes.
      process_sleep();
      schedule();
      /* Reload timer for another QUANTUM ms (simulated time) */
      kload_timer(QUANTUM);
      kset_cause(~0x8000, 0);   //clear the flag for timer interrupt
    }
  }
  //kdebug_println("Exception out");
}
