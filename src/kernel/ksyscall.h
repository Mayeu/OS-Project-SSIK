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
  FOURCHETTE,
  KILL,
  QUIT
};

void            syscall_handler(registers_t * regs);

#endif
