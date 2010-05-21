/**
 * \file ksyscall.c
 * \brief Handle the syscall exceptions
 * \author Christophe Carasco
 * \version 0.1
 * \date 21 mai 2010
 *
 */

#include "ksyscall.h"
#include "kprocess.h"
#include "kprocess_list.h"
#include "kscheduler.h"
#include "kernel.h"

void
syscall_handler(registers_t * regs)
{
  int32_t         res = 0;
  int32_t         syscall = regs->v_reg[0];     // code of the syscall
  pcb            *p;
  switch (syscall)
  {
  case FOURCHETTE:
    res = create_proc((char *) regs->a_reg[0], regs->a_reg[1], (int32_t *) regs->a_reg[2]);     // name in a0, pri in a1, params in a3
    break;
  case KILL:
    p = searchall(regs->a_reg[0]);      // pid in a0
    res = rm_p(p);
    schedule();
    break;
  case QUIT:
    p = searchall(first(&prunning));
    res = rm_p(p);
    break;
  default:
    ;
  }
  // saves the return code
  regs->v_reg[0] = res;
}
