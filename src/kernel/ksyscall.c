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
#include "kinout.h"


void
syscall_none(int32_t scode)
{
  asm("move $v0, $a0\n\t syscall\n\t");
}

void
syscall_one(int32_t p1, int32_t scode)
{
  asm("move $v0, $a1\n\t syscall\n\t");
}

void
syscall_two(int32_t p1, int32_t p2, int32_t scode)
{
  asm("move $v0, $a2\n\t syscall\n\t");
}

void
syscall_three(int32_t p1, int32_t p2, int32_t p3, int32_t scode)
{
  asm("move $v0, $a3\n\t syscall\n\t");
}


void
syscall_handler(registers_t * regs)
{
  int32_t         res = 0;
  int32_t         syscall = regs->v_reg[0];     // code of the syscall
  pcb            *p;
  switch (syscall)
  {
  case TEST:
    ktest((char *) regs->a_reg[0], regs->a_reg[1], (char **) regs->a_reg[2]);
    break;
  case FOURCHETTE:
    res = create_proc((char *) regs->a_reg[0], regs->a_reg[1], (char **) regs->a_reg[2]);     // name in a0, pri in a1, params in a3
    break;
  case KILL:
    p = searchall(regs->a_reg[0]);      // pid in a0
    res = rm_p(p);
    schedule();
    break;
  case QUIT:
    p = searchall(prunning.current->pid);
    res = rm_p(p);
    break;
  default:
    ;
  }
  // saves the return code
  regs->v_reg[0] = res;
}
