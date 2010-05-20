/*
 * example_timer:
 *   An example of how to setup the MIPS/Malta board
 *   to periodically increase a number on the display.
 */  
#include "asm.h"
#include "malta.h"
#include "debug.h"


/* display_word:
 *   Display a value on the Malta display.
 */ 
  static void
display_word(uint32_t word) 
{
  
  
  
  {
    
    
  



/* example - function that prints a string to the terminal window  */ 
  static void
printstr(char print_array[]) 
{
  
  
    
  {
    
    
    
    {
      
      
    
    
  



/* kinit:
 *   Application-specific initialisation code, called
 *   from the boot code '_start' in 'asm.S'.
 */ 
  void
kinit() 
{
  
    { 'H', 'i', ' ', 'd', 'u', 'd', 'e', ',', ' ', 
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



/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */ 
  void
kexception() 
{
  
  
  
    /* Make sure that we are here because of a timer interrupt. */ 
    cause.reg = kget_cause();
  
  
  
    /* Reload timer for another 100 ms (simulated time) */ 
    kload_timer(100 * timer_msec);
  
    /* Icrease the number on the Malta display. */ 
    display_word(++i);
