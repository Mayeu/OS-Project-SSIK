/**
 * \file ksyscall.c
 * \brief Handle the syscall exceptions
 * \author Christophe Carasco
 * \version 0.1
 * \date 21 mai 2010
 *
 */

// used to get the BAS_PRI variable
#include <process.h>
#include <stdio.h>

#include "ksyscall.h"
#include "kprocess.h"
#include "kprocess_list.h"
#include "kscheduler.h"
#include "kernel.h"
#include "kinout.h"
#include "asm.h"

int32_t
syscall_none(int32_t scode)
{
  asm("move $v0, $a0\n\t syscall\n\t");
}

int32_t
syscall_one(int32_t p1, int32_t scode)
{
  asm("move $v0, $a1\n\t syscall\n\t");
}

int32_t
syscall_two(int32_t p1, int32_t p2, int32_t scode)
{
  asm("move $v0, $a2\n\t syscall\n\t");
}

int32_t
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
  	case FOURCHETTE:
    	//res = create_proc((char *)regs->a_reg[0], BAS_PRIO, (char **)regs->a_reg[1]);
    	res = ktest((char*)regs->a_reg[0], BAS_PRI, (char **)regs->a_reg[1]);
    break;
  	case PRINT:
			kprint((char*)regs->a_reg[0]);
    break;
  	case PRINTLN:
    	kprintln((char*)regs->a_reg[0]);
    break;
		case FPRINT:
			if (regs->a_reg[0] == CONSOLE)
				kprint((char*)regs->a_reg[1]);
			else
				kmaltaprint8((char*)regs->a_reg[1]);
		break;
  	case SLEEP:
    	prunning.current->wait = regs->a_reg[0] * timer_msec;
    break;
  	case KILL:
    	p = searchall(regs->a_reg[0]);
    	res = rm_p(p);
    	schedule();
    break;
  	case EXIT:
   		p = search_pcb(prunning.current->pid, &prunning);
    	res = rm_p(p);
    	schedule();
    break;
  default:
    ;
  }
  // saves the return code
  regs->v_reg[0] = res;
}
