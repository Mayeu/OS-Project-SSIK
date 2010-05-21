/**
 * \file kexception.c
 * \brief Handles the exceptions and interruptions.
 * \author Christophe Carasco
 * \version 0.1
 * \date 21 mai 2010
 *
 */

#include "asm.h"
#include "mips4kc.h"
#include "scheduler.h"

#define QUANTUM 100

cause_reg_t cause;

void kexception()
{
	cause_reg_t cause;
	cause.reg = kget_cause();
	if(cause.field.exc == 8)	// intenal exception (syscall)
	{
		/* Make epc point to the next instruction (after the syscall) */
		registers_t* reg;
		reg = (registers_t *)kget_registers();

		syscall_handler(reg);

		reg->epc_reg += 4;
		kset_cause(~0x60, 0); // Acknowledge
	} else if(cause.field.exc == 0)	// internal exception
	{
		if(cause.field.ip & 4)				// uart interrupt
		{
			uart_exception();
      	kset_cause(~0x1000,0);      	// Acknowledge UART interrupt.
		} else if(cause.field.ip & 0x80)	// timer exception
		{
			// check if there are some processes to wake up and reschedule all processes.
			timer_exception();
			/* Reload timer for another QUANTUM ms (simulated time) */ 
      	kload_timer(QUANTUM * timer_msec);

		}
	}    
}
