/**
 * \file kexception.c
 * \brief Handles the exceptions and interruptions.
 * \author Christophe Carasco
 * \version 0.1
 * \date 21 mai 2010
 *
 */

#include "asm.h"
#include "kinout.h"
#include "mips4kc.h"
#include "kscheduler.h"
#include "ksyscall.h"
#include "kscheduler.h"
#include "ksleep.h"


cause_reg_t     cause;

void            kset_timer(uint32_t time);

void
kexception()
{
  cause_reg_t     cause;
  cause.reg = kget_cause();
  if (cause.field.exc == 8)     // external exception (syscall)
  {
    registers_t    *reg;
    reg = (registers_t *) kget_registers();

    syscall_handler(reg);

    reg->epc_reg += 4;          // Make epc point to the next instruction (after the syscall)
    kset_cause(~0x60, 0);       // Acknowledge
  }
  else if (cause.field.exc == 0)        // internal exception
  {
    if (cause.field.ip & 4)     // uart interrupt
    {
      //uart_exception();                                                                                                                                                             /** TODO: add uart_exception to uart file */
      kset_cause(~0x1000, 0);   // Acknowledge UART interrupt.
    }
    else if (cause.field.ip & 0x80)     // timer exception
    {
      // kprintln("I'm in your OS, calling your Scheduler");
      // check if there are some processes to wake up and reschedule all processes.
      process_sleep();
      schedule();
      /* Reload timer for another QUANTUM ms (simulated time) */
      kload_timer(QUANTUM);
      kset_cause(~0x8000, 0);   //clear the flag for timer interrupt

    }
  }
}
