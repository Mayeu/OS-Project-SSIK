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
  //msg_arg        *mres;
  char            buf[3];

  //kdebug_println("syscal in");
  kprintln(itos(syscall, buf));

  switch (syscall)
  {
  case FOURCHETTE:
    // A CHANGER, POUR ENVOYER LE NB D'ARG STOQUE DS regs->a_reg[1]
    // NE PAS OUBLIER DE COPIER LES ARG DS LA STRUCT DU PCB !!
    res =
      create_proc((char *) regs->a_reg[0], BAS_PRI, (char **) regs->a_reg[2]);
    break;
  case PRINT:
    kprint((char *) regs->a_reg[0]);
    break;
  case PRINTLN:
    kprintln((char *) regs->a_reg[0]);
    break;
  case FPRINT:
    if (regs->a_reg[0] == CONSOLE)
      kprint((char *) regs->a_reg[1]);
    else
      kmaltaprint8((char *) regs->a_reg[1]);
    break;
  case SLEEP:
    //prunning.current->wait = regs->a_reg[0] * timer_msec;
    go_to_sleep(regs->a_reg[0]);
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
  case GETPID:
    res = pcb_get_pid(get_current_pcb());
    break;
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
    //p = pcls_search_pcb(prunning.current->pid, &prunning);
    //p->registers.v_reg[0] = regs->a_reg[0];
    //move(prunning.current->pid, &prunning, &pterminate);
    schedule();
    break;
  default:
    kprintln("ERROR: Unknown syscall");
    break;
  }

  //kdebug_println("syscal out");
  // saves the return code
  regs->v_reg[0] = res;
}
