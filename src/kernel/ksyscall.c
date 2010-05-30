/**
 * \file ksyscall.c
 * \brief Handle the syscall exceptions
 * \author Christophe Carasco
 * \version 0.1
 * \date 21 mai 2010
 *
 */

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

  switch (syscall)
  {
  case FOURCHETTE:
    {
      // A CHANGER, POUR ENVOYER LE NB D'ARG STOQUE DS regs->a_reg[1]
      // NE PAS OUBLIER DE COPIER LES ARG DS LA STRUCT DU PCB !!
      char           *name = (char *) regs->a_reg[0];
      char          **argv = (char **) regs->a_reg[2];
      int             prio = stoi(get_arg(argv, 0));

      strcpy(name, get_arg(argv, 0));
      res = create_proc(name, prio, argv);
      break;
    }
  case PRINT:
    res = print_string((char *) regs->a_reg[0]);
    break;
  case FPRINT:
    if (regs->a_reg[0] == CONSOLE)
      kprint((char *) regs->a_reg[1]);
    else
      kmaltaprint8((char *) regs->a_reg[1]);
    break;
  case SLEEP:
    res = go_to_sleep(regs->a_reg[0]);
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
    // TODO : get_pinfo() TAKES A PID INSTEAD OF A PCB
    //res = get_pinfo(regs->a_reg[0], (pcbinfo *) regs->a_reg[1]);
    break;
  case GETPRI:
    // TODO : int kgetpri(int pid)
    //res = kgetpri(regs->a_reg[0]);
    break;
  case GETPID:
    res = pcb_get_pid(get_current_pcb());
    break;
  case GETPS:
    {
/*
		pls_item* p;
		char **pnames = (char**)regs->a_reg[0];
		int *pid = (int*)regs->a_reg[1];
		int i = 0;

		for (p = plsready.start; p != plsready.end; p = p->next, i++)
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
    // TODO : chg_ppri() TAKES A PID INSTEAD OF A PCB
    //res = chg_ppri(regs->a_reg[0], regs->a_reg[1]);
    break;
  case KILL:
    // TODO : rm_p() TAKES A PID INSTEAD OF A PCB
    //res = kkill(regs->a_reg[0]);
    break;
  case EXIT:
    // NOTIFY THE SUPERVISOR OF THE EXITING PROCESS !
    //kprint("proc exiting with value "); kprint(itos(regs->a_reg[0], buf));kprintn();

    // TODO : int kexit()
    //res = kexit()
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
