/*
 * example_uart:
 *   An example of how to setup the MIPS/Malta board
 *   to transmit and receive data to/from tty0 (terminal),
 *   using interrupts.
 */  
  
#include "asm.h"
#include "malta.h"
#include "debug.h"
static registers_t regs;

#define FIFO_SIZE 8
  
/* A simple FIFO queue of bounded size. */ 
  struct bounded_fifo
{
  uint8_t buf[FIFO_SIZE];
  uint32_t length;
};

/* bfifo_put: Inserts a character at the end of the queue. */ 
  static void
bfifo_put(struct bounded_fifo *bfifo, uint8_t ch) 
{
  
    /* Make sure the 'bfifo' pointer is not 0. */ 
    kdebug_assert(bfifo != 0);
  if (bfifo->length < FIFO_SIZE)
  {
    bfifo->buf[(bfifo->length)++] = ch;
  }
}


/* bfifo_get: Returns a character removed from the front of the queue. */ 
static          uint8_t
bfifo_get(struct bounded_fifo *bfifo) 
{
  int            i;
  uint8_t ch;
  
    /* Make sure the 'bfifo' pointer is not 0, and that queue is not empty. */ 
    kdebug_assert(bfifo != 0);
  kdebug_assert(bfifo->length > 0);
  bfifo->length--;
  ch = bfifo->buf[0];
  for (i = 0; i < bfifo->length; i++)
  {
    bfifo->buf[i] = bfifo->buf[i + 1];
  }
  return ch;
}
static struct bounded_fifo bfifo;

/* kinit:
 *   Application-specific initialisation code, called
 *   from the boot code '_start' in 'asm.S'.
 */ 
  void
kinit() 
{
  status_reg_t and, or;
  bfifo.length = 0;
  kset_registers(&regs);
  
    /* Set UART word length ('3' meaning 8 bits).
     * Do this early to enable debug printouts (e.g. kdebug_print).
     */ 
    tty->lcr.field.wls = 3;
  
    /* Below is an alternative way of setting the status register,
     * using the 'status_reg_t' type defined in 'mips4kc.h'.
     */ 
    and.reg = 0xFFFFFFFF;
  and.field.exl = 0;		// Normal level (not exception)
  and.field.erl = 0;		// Error level
  and.field.um = 0;		// Kernel mode
  and.field.im = 4;		// Disable all except HW interrupt 0
  and.field.bev = 0;		// Use normal exception vector (not bootsptrap)
  or.reg = 0;
  or.field.ie = 1;		// Enable interrupts
  or.field.im = 4;		// Enable HW interrupt 0
  or.field.cu0 = 1;		// Coprocessor 0 usable
  kset_sr(and.reg, or.reg);
  
    /* Generate interrupts when data is received by UART. */ 
    tty->ier.field.erbfi = 1;
  
    /* Some obscure bit that need to be set for UART interrupts to work. */ 
    tty->mcr.field.out2 = 1;
  while (1);
}


/* kexception:
 *   Application-specific exception handler, called after registers
 *   have been saved.
 */ 
  void
kexception() 
{
  uint8_t ch;
  cause_reg_t cause;
  
    /* Make sure interrupt is from external source. */ 
    cause.reg = kget_cause();
  kdebug_assert(cause.field.exc == 0);	/* External interrupt */
  if (cause.field.ip & 4)
  {
    
      /* UART interrupt */ 
      if (tty->lsr.field.dr)
    {
      
	/* Data ready: add character to buffer */ 
	ch = tty->rbr;
      bfifo_put(&bfifo, ch);
      if (ch == '\r')
      {
	bfifo_put(&bfifo, '\n');
      }
    }
    if (bfifo.length > 0 && tty->lsr.field.thre)
    {
      
	/* Transmitter idle: transmit buffered character */ 
	tty->thr = bfifo_get(&bfifo);
      
	/* Determine if we should be notified when transmitter becomes idle */ 
	tty->ier.field.etbei = (bfifo.length > 0);
    }
    
      /* Acknowledge UART interrupt. */ 
      kset_cause(~0x1000, 0);
  }
}


