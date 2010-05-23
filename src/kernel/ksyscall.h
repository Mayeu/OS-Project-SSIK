/**
 * \file ksyscall.h
 * \brief Handle the syscall interrupts.
 * \author Christophe Carasco
 * \version 0.1
 * \date 21 mai 2010
 *
 */

#ifndef __KSYSCALL_H
#define __KSYSCALL_H

#include "registers.h"

enum
{
  TEST,
  FOURCHETTE,
  PRINT,
  KILL,
  QUIT
};

void            syscall_none(int32_t scode);
void            syscall_one(int32_t p1, int32_t scode);
void            syscall_two(int32_t p1, int32_t p2, int32_t scode);
void            syscall_three(int32_t p1, int32_t p2, int32_t p3,
                              int32_t scode);

void            syscall_handler(registers_t * regs);

#endif
