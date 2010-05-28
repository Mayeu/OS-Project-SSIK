/**
 * \file ksyscall.c
 * \brief Handle the syscall exceptions
 * \author Christophe Carasco
 * \version 0.1
 * \date 21 mai 2010
 *
 */

// used to get the BAS_PRI variable
#include <stdio.h>
#include <string.h>             // for debug
#include <process.h>

#include "ksyscall.h"
#include "kprocess.h"
#include "kscheduler.h"
#include "kernel.h"
#include "kinout.h"
#include "kerror.h"
#include "debug.h"
#include "ksleep.h"
#include "asm.h"

void
syscall_handler(registers_t * regs)
{
  int32_t         res = 0;
  int32_t         syscall = regs->v_reg[0];     // code of the syscall
  pcb            *p;
	msg_arg 			 *mres;
	char					 	buf[3];
	
  switch (syscall)
  {
  case FOURCHETTE:
	{
		// A CHANGER, POUR ENVOYER LE NB D'ARG STOQUE DS regs->a_reg[1]
		// NE PAS OUBLIER DE COPIER LES ARG DS LA STRUCT DU PCB !!
		char  *name = (char *)regs->a_reg[0];
		char **argv = (char **)regs->a_reg[2];
		int prio = stoi(get_arg(argv, 0));

		strcpy(name, get_arg(argv, 0));
    res = create_proc(name, prio, argv);
    break;
	}
  case PRINT:
    res = print_string((char *) regs->a_reg[0]);
    break;
    //case PRINTLN:
    //kprintln("Want to print");
    //res = print_string((char *) regs->a_reg[0]);
    //res = print_string("\n");
    //break;
  case FPRINT:
    if (regs->a_reg[0] == CONSOLE)
      kprint((char *) regs->a_reg[1]);
    else
      kmaltaprint8((char *) regs->a_reg[1]);
    break;
  case SLEEP:
    res = go_to_sleep(regs->a_reg[0]);
    schedule();
    break;
  case BLOCK:
    break;
  case WAKEUP:
    break;
  case WAIT:
    /*
       wait()
       p = search_pcb(regs->a_reg[0], &pterminate);
       // copy de la valeur de retour du proc
       // qu'on attendait
       *regs->a_reg[1] = p->registers->v_reg[0]; 

       rm(p)
     */
    break;
  case SEND:
    //mres = (msg_arg *) regs->a_reg[0];
    //kprint("process pid = ");
    //kprint(itos(prunning.current->pid, buf));
    //kprint(" is sending datatype ");
    //kprint(itos(mres->datatype, buf));
    //kprint(" to process pid = ");
    //kprintln(itos(mres->pid, buf));
    //kprintln((char*)mres->data);
    // res = send_msg(prunning.current->pid, msg_arg);
    break;
  case RECV:
    // res = recv_msg(prunning.current->pid, msg_arg);
    break;
  case PERROR:
    kperror((char *) regs->a_reg[0]);
    break;
  case GERROR:
    res = kgerror();
    break;
  case SERROR:
    kserror(regs->a_reg[0]);
    break;
  case GETPINFO:
    p = search_all_list(regs->a_reg[0]);
    res = get_pinfo(p, (pcbinfo *) regs->a_reg[1]);
    break;
  case GETPRI:
    p = searchall(regs->a_reg[0]);
    res = p->pri;
    break;
  case GETPID:
    res = pcb_get_pid(get_current_pcb());
    break;
  case GETPS:
	{
/*
		pcls_item* p;
		char **pnames = (char**)regs->a_reg[0];
		int *pid = (int*)regs->a_reg[1];
		int i = 0;

		for (p = pclsready.start; p != pclsready.end; p = p->next, i++)
		{
			strcpy(pcb_get_name(p), get_arg(pnames, i));
			pid[i] = pcb_get_pid(p);
		}
		res = i;
*/
		res = -1;
    break;
	}
  case CHGPPRI:
    p = search_all_list(regs->a_reg[0]);
    res = chg_ppri(p, regs->a_reg[1]);
    break;
  case KILL:
    p = search_all_list(regs->a_reg[0]);
    res = rm_p(p);
    schedule();
    break;
  case EXIT:
		// NOTIFY THE SUPERVISOR OF THE EXITING PROCESS !
		//kprint("proc exiting with value "); kprint(itos(regs->a_reg[0], buf));kprintn();
/*
    p = search_pcb(prunning.current->pid, &prunning);
		p->registers.v_reg[0] = regs->a_reg[0];
		move(prunning.current->pid, &prunning, &pterminate);
    schedule();
*/
    break;
  default:
    kprintln("ERROR: Unknown syscall");
    break;
  }

  //kdebug_println("syscal out");
  // saves the return code
  regs->v_reg[0] = res;

  return;
}
