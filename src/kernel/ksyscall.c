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
#include "kmsg.h"
#include "asm.h"

/**
 * Call by the exeption to handle the syscall
 * \private
 */
void
syscall_handler(registers_t * regs)
{
  int32_t         res = 0;
  int32_t         syscall = regs->v_reg[0];     // code of the syscall

  switch (syscall)
  {
  case FOURCHETTE:
    res =
      create_proc(get_arg((char **) regs->a_reg[2], 0), regs->a_reg[0],
                  regs->a_reg[1], (char **) regs->a_reg[2]);
    break;
  case PRINT:
    res = print_string((char *) regs->a_reg[0]);
    return;                     /* We save the good return value in the pcb */
  case READ:
    res = read_string((char *) regs->a_reg[0], regs->a_reg[1]);
    return;                     /* We save the good return value in the pcb */
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
    res =
      send_msg(pcb_get_pid(get_current_pcb()), (msg_arg *) regs->a_reg[0]);
    break;
  case RECV:
    res =
      recv_msg(pcb_get_pid(get_current_pcb()), (msg_arg *) regs->a_reg[0]);
    go_to_sleep(((msg_arg *) regs->a_reg[0])->timeout);
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
