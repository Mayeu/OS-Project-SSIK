/*
 * example_uart:
 *   An example of how to setup the MIPS/Malta board
 *   to transmit and receive data to/from tty0 (terminal),
 *   using interrupts.
 */  
  
#include "asm.h"
#include "malta.h"
#include "debug.h"


#define FIFO_SIZE 8
  
/* A simple FIFO queue of bounded size. */ 
  struct bounded_fifo
{
  
  


/* bfifo_put: Inserts a character at the end of the queue. */ 
  static void
bfifo_put(struct bounded_fifo *bfifo, uint8_t ch) 
{
  
    /* Make sure the 'bfifo' pointer is not 0. */ 
    kdebug_assert(bfifo != 0);
  
  {
    
  



/* bfifo_get: Returns a character removed from the front of the queue. */ 
static          uint8_t
bfifo_get(struct bounded_fifo *bfifo) 
{
  
  
  
    /* Make sure the 'bfifo' pointer is not 0, and that queue is not empty. */ 
    kdebug_assert(bfifo != 0);
  
  
  
  
  {
    
  
  




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
  
    /* Below is an alternative way of setting the status register,
     * using the 'status_reg_t' type defined in 'mips4kc.h'.
     */ 
    and.reg = 0xFFFFFFFF;
  
  and.field.erl = 0;            // Error level
  and.field.um = 0;             // Kernel mode
  and.field.im = 4;             // Disable all except HW interrupt 0
  and.field.bev = 0;            // Use normal exception vector (not bootsptrap)
  
  
  or.field.im = 4;              // Enable HW interrupt 0
  or.field.cu0 = 1;             // Coprocessor 0 usable
  
  
    /* Generate interrupts when data is received by UART. */ 
    tty->ier.field.erbfi = 1;
  
    /* Some obscure bit that need to be set for UART interrupts to work. */ 
    tty->mcr.field.out2 = 1;
  



/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */ 
  void
kexception() 
{
  
  
  
    /* Make sure interrupt is from external source. */ 
    cause.reg = kget_cause();
  
  
  {
    
      /* UART interrupt */ 
      if (tty->lsr.field.dr)
    {
      
        /* Data ready: add character to buffer */ 
        ch = tty->rbr;
      
      
      {
        
      
    
    
    {
      
        /* Transmitter idle: transmit buffered character */ 
        tty->thr = bfifo_get(&bfifo);
      
        /* Determine if we should be notified when transmitter becomes idle */ 
        tty->ier.field.etbei = (bfifo.length > 0);
    
    
      /* Acknowledge UART interrupt. */ 
      kset_cause(~0x1000, 0);
  


