/*
 * @file ns16550.h
 * @brief Structure to manipulate the serial console
 */

#ifndef NS16550_H
#define NS16550_H

#include "types.h"
typedef struct
{
  union
  {
    uint8_t         rbr;        /* RBR: Receiver Buffer Register     */
    uint8_t         thr;        /* THR: Transmitter Holding Register */
    uint8_t         dll;        /* DLL: Divisor Latch (LS)           */
  };
  union
  {

    /* IER: Interrupt Enable Register */
    union
    {
      uint8_t         reg;
      struct
      {
        uint8_t         erbfi:1;        /* bit 0: Enable RBR Data Available Interr */
        uint8_t         etbei:1;        /* bit 1: Enable THR Empty Interrupt       */
        uint8_t         elsi:1; /* bit 2: Enable Receiver Line Status Intr */
        uint8_t         edssi:1;        /* bit 3: Enable MODEM Status Interrupt    */
      } field;
    } ier;

    /* DLM: Divisor Latch (MS) */
    uint8_t         dlm;
  };
  union
  {

    /* IIR: Interrupt Identity Register */
    union
    {
      uint8_t         reg;
      struct
      {
        uint8_t         pending:1;      /* bit 0:   Interrupt Pending */
        uint8_t         id:3;   /* bit 1-3: Interrupt ID      */
      } field;
    } iir;

    /* FCR: FIFO Control Register */
    union
    {
      uint8_t         reg;
      struct
      {
        uint8_t         fe:1;   /* bit 0: FIFO Enable     */
        uint8_t         rfr:1;  /* bit 1: RCVR FIFO Reset */
        uint8_t         xfr:1;  /* bit 2: XMIT FIFO Reset */
        uint8_t         dms:1;  /* bit 3: DMA Mode Select */
      } field;
    } fcr;
  };

  /* LCR: Line Control Register */
  union
  {
    uint8_t         reg;
    struct
    {
      uint8_t         wls:2;    /* bit 0-1: Word Length Select  */
      uint8_t         stb:1;    /* bit 2:   Number of Stop Bits */
      uint8_t         pen:1;    /* bit 3:   Parity Enable       */
      uint8_t         eps:1;    /* bit 4:   Even Parity Select  */
    } field;
  } lcr;

  /* MCR: MODEM Control Register */
  union
  {
    uint8_t         reg;
    struct
    {
      uint8_t         dtr:1;    /* bit 0: Data Terminal Ready */
      uint8_t         rts:1;    /* bit 1: Request To Send     */
      uint8_t         out1:1;   /* bit 2: Out 1               */
      uint8_t         out2:1;   /* bit 3: Out 2               */
    } field;
  } mcr;

  /* LSR: Line Status Register */
  union
  {
    uint8_t         reg;
    struct
    {
      uint8_t         dr:1;     /* bit 0: Data Ready                   */
      uint8_t         oe:1;     /* bit 1: Overrun Error                */
      uint8_t         pe:1;     /* bit 2: Parity Error                 */
      uint8_t         fe:1;     /* bit 3: Framing Error                */
      uint8_t         bi:1;     /* bit 4: Break Interrupt              */
      uint8_t         thre:1;   /* bit 5: Transmitter Holding Register */
      uint8_t         temt:1;   /* bit 6: Transmitter Empty            */
    } field;
  } lsr;

  /* MSR: MODEM Status Register */
  union
  {
    uint8_t         reg;
    struct
    {
      uint8_t         dcts:1;   /* bit 0: Delta Clear to Send          */
      uint8_t         ddsr:1;   /* bit 1: Delta Data Set Ready         */
      uint8_t         teri:1;   /* bit 2: Trailing Edge Ring Indicator */
      uint8_t         ddcd:1;   /* bit 3: Delta Data Carrier Detect    */
      uint8_t         cts:1;    /* bit 4: Clear to Send                */
      uint8_t         dsr:1;    /* bit 5: Data Set Ready               */
      uint8_t         ri:1;     /* bit 6: Ring Indicator               */
      uint8_t         dcd:1;    /* bit 7: Data Carrier Detect          */
    } field;
  } msr;

  /* Scratch Register */
  uint8_t         scratch;
} ns16550_t;

#endif /*  */

/* end of file ns16550.h */
