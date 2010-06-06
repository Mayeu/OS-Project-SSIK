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
    return;                     /* We save the good return value in the pcb */
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
    res = kblock(regs->a_reg[0], BLOCKED);
    break;
  case UNBLOCK:
    kwakeup(regs->a_reg[0]);
    break;
  case WAIT:
    res = waitfor(regs->a_reg[0], (int32_t *) regs->a_reg[1]);
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
    res = get_pinfo(regs->a_reg[0], (pcbinfo *) regs->a_reg[1]);
    break;
  case GETPID:
    res = pcb_get_pid(get_current_pcb());
    break;
  case GETALLPID:
    res = get_all_pid((int *) regs->a_reg[0]);
    break;
  case CHGPPRI:
    res = chg_ppri(regs->a_reg[0], regs->a_reg[1]);
    break;
  case KILL:
    res = kkill(regs->a_reg[0]);
    break;
  case EXIT:
    // NOTIFY THE SUPERVISOR OF THE EXITING PROCESS !
    //kprint("proc exiting with value "); kprint(itos(regs->a_reg[0], buf));kprintn();
    kexit(regs->a_reg[0]);
    break;
  default:
    kprintln("ERROR: Unknown syscall");
    break;
  }

  // saves the return code
  regs->v_reg[0] = res;

  return;
}
