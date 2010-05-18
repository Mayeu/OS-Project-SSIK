
#ifndef MALTA_H
#define MALTA_H
  
#include "mips4kc.h"
#include "ds12887.h"
#include "ns16550.h"
#include "display.h"
  
#define IO_BASE1 0xb8000000
#define IO_BASE2 0xbf000000
#define IO_BASE3 0xbbe00000
  
#define IO_DEVICE(Type,Name,Address) \
static volatile Type *const Name = (Type *) (Address)  
#define IO_DEVICE1(Type,Name,Offset) IO_DEVICE(Type, Name, IO_BASE1 + Offset)
#define IO_DEVICE2(Type,Name,Offset) IO_DEVICE(Type, Name, IO_BASE2 + Offset)
#define IO_DEVICE3(Type,Name,Offset) IO_DEVICE(Type, Name, IO_BASE3 + Offset)
  
/* rtc: Real-Time Clock */ 
  IO_DEVICE1(ds12887_t, rtc, 0x70);

/* tty: UART */ 
  IO_DEVICE1(ns16550_t, tty, 0x3f8);

/* malta: 8 digit LED */ 
  IO_DEVICE2(display_t, malta, 0x400);

/* Make sure we don't clutter preprocessor namespace */ 
#undef IO_BASE1
#undef IO_BASE2
#undef IO_DEVICE
#undef IO_DEVICE1
#undef IO_DEVICE2
  
#endif /*  */
