/*
 * example_timer:
 *   An example of how to setup the MIPS/Malta board
 *   to periodically increase a number on the display.
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


/* example - function that prints a string to the terminal window  */ 
  static void
printstr(char print_array[]) 
{
  int            i = 0;
  while (print_array[i] != '\0')
    
  {
    while (!tty->lsr.field.thre);
    tty->thr = print_array[i];
    if (print_array[i] == '\n')
    {
      while (!tty->lsr.field.thre);
      tty->thr = '\r';
    }
    i++;
  }
}


/* kinit:
 *   Application-specific initialisation code, called
 *   from the boot code '_start' in 'asm.S'.
 */ 
  void
kinit() 
{
  char           a[] =
    { 'H', 'i', ' ', 'd', 'u', 'd', 'e', ',', ' ', 'w', 'h', 'a', 't', '\'',
    's', ' ', 'u', 'p', '?', '\n', '\0'
  };
  
    /* Set UART word length ('3' meaning 8 bits).
     * Do this early to enable debug printouts (e.g. kdebug_print).
     */ 
    tty->lcr.field.wls = 3;
  
    /* example of call to a print function */ 
    printstr(a);
  
    /* Put '0' on the Malta display. */ 
    display_word(0);
  
    /* Setup storage-area for saving registers on exception. */ 
    kset_registers(&regs);
  
    /* Initialise timer to interrupt in 100 ms (simulated time). */ 
    kload_timer(100 * timer_msec);
  
    /* Update the status register to enable timer interrupts. */ 
    kset_sr(0xFFBF00E8, 0x10008001);
  
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
  static int     i = 0;
  cause_reg_t cause;
  
    /* Make sure that we are here because of a timer interrupt. */ 
    cause.reg = kget_cause();
  kdebug_assert(cause.field.exc == 0);	/* External interrupt */
  kdebug_assert(cause.field.ip & 0x80);	/* Timer interrupt */
  
    /* Reload timer for another 100 ms (simulated time) */ 
    kload_timer(100 * timer_msec);
  
    /* Icrease the number on the Malta display. */ 
    display_word(++i);
} 
