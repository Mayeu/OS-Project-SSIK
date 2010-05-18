/*
 * example_syscall:
 *   An example of how the MIPS 'syscall' exceptions can be
 *   handled. The code displays '66' on the Malta display, then
 *   loops forever. The purpose is to show how arguments can be
 *   transferred from user mode to kernel mode.
 *
 *   See 'syscall.S' for definitions of 'my_system_call' and
 *   'ksyscall_handler'.
 */  
  
#include "asm.h"
#include "malta.h"
#include "debug.h"
static registers_t regs;

/* display_word:
 *   Display a value on the Malta display.
 */ 
  static void
display_word(uint32_t word) 
{
  int            i;
  malta->ledbar.reg = 0xFF;
  for (i = 7; i >= 0; --i)
  {
    malta->asciipos[i].value = '0' + word % 10;
    word /= 10;
  }
}


/* Declaration of system call handler (defined in 'syscall.S'). */ 
void            ksyscall_handler(registers_t * reg);

/* Declaration of my system call for user processes. */ 
void            my_system_call(uint32_t v);

/* Kernels internal definition of my system call (prefix 'k'). */ 
  void
kmy_system_call(uint32_t v) 
{
  
    /* Implementation of my_system_call: */ 
    /* Displays value of its argument.   */ 
    display_word(v);
} 

/* kinit:
 *   Application-specific initialisation code, called
 *   from the boot code '_start' in 'asm.S'.
 */ 
  void
kinit() 
{
  
    /* Set UART word length ('3' meaning 8 bits).
     * Do this early to enable debug printouts (e.g. kdebug_print).
     */ 
    tty->lcr.field.wls = 3;
  
    /* Setup storage-area for saving registers on exception. */ 
    kset_registers(&regs);
  
    /* Update the status register to enable exceptions. */ 
    kset_sr(0xFFBF00E8, 0x10008001);
  
    /* Display 0x42 using my system call */ 
    my_system_call(0x42);
  while (1);
}


/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */ 
  void
kexception() 
{
  registers_t * reg;
  cause_reg_t cause;
  
    /* Make sure that we are here because of a syscall exception. */ 
    cause.reg = kget_cause();
  kdebug_assert(cause.field.exc == 8);
  
    /* Get pointer to stored registers. */ 
    reg = kget_registers();
  
    /* Handle the system call (see syscall.S). */ 
    ksyscall_handler(reg);
  
    /* Return from exception to instruction following syscall. */ 
    reg->epc_reg += 4;
  
    /* Acknowledge syscall exception. */ 
    kset_cause(~0x60, 0);
} 
