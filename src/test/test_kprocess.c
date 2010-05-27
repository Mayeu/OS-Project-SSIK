/**
 * @file test_kprocess.c
 * @brief Test kprocess module.
 */

#include <string.h>
#include <errno.h>
#include "../kernel/kprocess.h"
#include "../kernel/kinout.h"
#include "../kernel/kernel.h"
#include "../kernel/kprogram.h"

void
test_kprocess()
{
  int             i;
  bool            err;
  pcb            *pcb1;
  pcb            *pcb2;
  pcb            *pcb3;
  pcbinfo         pcbi1;
  uint32_t        res;
  char            params[MAX_ARG + 1][ARG_SIZE],
    params2[MAX_ARG + 1][ARG_SIZE];
  uint32_t        prog_address = (uint32_t) init;
  char            resc[3];
  strcpy("4", params[0]);
  strcpy("1", params[1]);
  strcpy("12", params[2]);
  strcpy("123", params[3]);
  strcpy("1234", params[4]);
  strcpy("3", params2[0]);
  strcpy("a", params2[1]);
  strcpy("ab", params2[2]);
  strcpy("abc", params2[3]);
  strcpy("", resc);

  kprintln("------------TEST MODULE KPROCESS BEGIN------------");
  create_pls(&pready);
  create_pls(&prunning);
  create_pls(&pterminate);
  create_pls(&pwaiting);

  kprintln("list created\t\t\t\t\tOK");

  kprint("create_proc\t\t\t\t\t");      // testCREATE1
  res = create_proc("init", 10, (char **) params);
  pcb1 = &pready.ls[0];

  err = (res == OMGROXX || res >= 0) &&
    (pcb1->pid == 0) &&
    (strcmp(pcb1->name, "init") == 0) &&
    (pcb1->pri == 10) &&
    (pcb1->supervisor == -1) &&
    (pcb1->registers.epc_reg == prog_address) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 1), "1") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 2), "12") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 3), "123") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 4), "1234") == 0) &&
    (pcb1->sleep == 0) && (pcb1->error == OMGROXX) && (pcb1->empty == FALSE);
  if (!err)
  {
    for (i = 0; i < MAXPCB; i++)
      if (pcb1->supervised[i] != -1)
      {
        err = FALSE;
        break;
      }
  }
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");


  kprint("create_proc test2\t\t\t\t");  // testCREATE2
  res = create_proc("init", 15, (char **) params2);
  pcb2 = &pready.ls[1];

  err = (res = OMGROXX || res >= 0) &&
    (pcb2->pid == 1) &&
    (strcmp(pcb2->name, "init") == 0) &&
    (pcb2->pri == 15) &&
    (pcb2->supervisor == -1) &&
    (pcb2->registers.epc_reg == prog_address) &&
    (strcmp(argn((char **) pcb2->registers.a_reg[0], 1), "a") == 0) &&
    (strcmp(argn((char **) pcb2->registers.a_reg[0], 2), "ab") == 0) &&
    (strcmp(argn((char **) pcb2->registers.a_reg[0], 3), "abc") == 0) &&
    (pcb2->sleep == 0) && (pcb2->error == OMGROXX) && (pcb2->empty == FALSE);
  if (!err)
  {
    for (i = 0; i < MAXPCB; i++)
      if (pcb2->supervised[i] != -1)
      {
        err = FALSE;
        break;
      }
  }
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("rm_p\t\t\t\t\t\t");   // testRM1
  res = rm_p(pcb1);
  err = (res == OMGROXX) && (pcb1->pid == 0) && (pcb1->empty == TRUE);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");


  kprint("create_proc after remove\t\t\t");     // testCREATE3
  res = create_proc("init", 12, (char **) params2);
  pcb1 = &pready.ls[0];

  err = (res == OMGROXX || res >= 0) &&
    (pcb1->pid == 2) &&
    (strcmp(pcb1->name, "init") == 0) &&
    (pcb1->pri == 12) &&
    (pcb1->supervisor == -1) &&
    (pcb1->registers.epc_reg == prog_address) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 1), "a") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 2), "ab") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 3), "abc") == 0) &&
    (pcb1->sleep == 0) && (pcb1->error == OMGROXX) && (pcb1->empty == FALSE);
  if (!err)
  {
    for (i = 0; i < MAXPCB; i++)
      if (pcb1->supervised[i] != -1)
      {
        err = FALSE;
        break;
      }
  }
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("change_ppri\t\t\t\t\t");      // testCHANGEPRI1
  res = chg_ppri(pcb1, 8);
  err = (res == OMGROXX) && (pcb1->pri == 8);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("get_pcbinfo\t\t\t\t\t");      // testCHANGEPRI1
  res = get_pinfo(pcb1, &pcbi1);
  err = (res == OMGROXX) &&
    (pcb1->pid == pcbi1.pid) &&
    (strcmp(pcb1->name, pcbi1.name) == 0) &&
    (pcb1->pri == pcbi1.pri) &&
    (pcb1->supervisor == pcbi1.supervisor) &&
    (pcb1->sleep == pcbi1.wait) && (pcb1->empty == pcbi1.empty);
  if (!err)
  {
    for (i = 0; i < MAXPCB; i++)
      if (pcb1->supervised[i] != pcbi1.supervised[i])
      {
        err = FALSE;
        break;
      }
  }
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");


  kprint("move_p\t\t\t\t\t\t"); // testMOVE3
  pcb3 = &pready.ls[2];
  res = move_p(pcb1, pcb3);
  err = (res == OMGROXX) &&
    (pcb3->pid == 2) &&
    (strcmp(pcb3->name, "init") == 0) &&
    (pcb3->pri == 8) &&
    (pcb3->supervisor == -1) &&
    (strcmp(argn((char **) pcb3->registers.a_reg[0], 1), "a") == 0) &&
    (strcmp(argn((char **) pcb3->registers.a_reg[0], 2), "ab") == 0) &&
    (strcmp(argn((char **) pcb3->registers.a_reg[0], 3), "abc") == 0) &&
    (pcb3->sleep == 0) &&
    (pcb3->error == OMGROXX) &&
    (pcb3->empty == FALSE) && (pcb1->empty == TRUE);
  if (!err)
  {
    for (i = 0; i < MAXPCB; i++)
      if (pcb3->supervised[i] != -1)
      {
        err = FALSE;
        break;
      }
  }
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("create_proc - after move\t\t\t");     // testCREATE4
  res = create_proc("init", 20, (char **) params2);
  pcb1 = &pready.ls[0];

  err = (res == OMGROXX || res >= 0) &&
    (pcb1->pid == 3) &&
    (strcmp(pcb1->name, "init") == 0) &&
    (pcb1->pri == 20) &&
    (pcb1->supervisor == -1) &&
    (pcb1->registers.epc_reg == prog_address) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 1), "a") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 2), "ab") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 3), "abc") == 0) &&
    (pcb1->sleep == 0) && (pcb1->error == OMGROXX) && (pcb1->empty == FALSE);
  if (!err)
  {
    for (i = 0; i < MAXPCB; i++)
      if (pcb1->supervised[i] != -1)
      {
        err = FALSE;
        break;
      }
  }
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("add_psupervised\t\t\t\t\t");  // testADDSUPERVISE4
  res = add_psupervised(pcb1, pcb3->pid);
  err = (res == OMGROXX) &&
    (pcb1->supervised[0] == pcb3->pid) && (pcb3->supervisor == pcb1->pid);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("rm_psupervised\t\t\t\t\t");   // testRMSUPERVISE4
  res = rm_psupervised(pcb1, pcb3->pid);
  err = (res == OMGROXX) &&
    (pcb1->supervised[0] == -1) && (pcb3->supervisor == -1);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("add_psupervisor\t\t\t\t\t");  // testRMSUPERVISE4
  res = add_psupervisor(pcb3, pcb1->pid);
  err = (res == OMGROXX) &&
    (pcb3->supervisor == pcb1->pid) && (pcb1->supervised[0] == pcb3->pid);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("add_psupervisor - already exist\t\t\t");
  res = add_psupervisor(pcb3, pcb1->pid);
  res = add_psupervisor(pcb3, pcb1->pid);
  err = (res == INVARG);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("p_is_empty - succeed\t\t\t\t");
  res = p_is_empty(pcb1);
  if (res == FALSE)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("p_is_empty - fail\t\t\t\t");
  pcb2 = &pready.ls[5];
  res = p_is_empty(pcb2);
  if (res == TRUE)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("create_proc - after reset pid\t\t\t");        // testCREATE5
  reset_next_pid();
  res = create_proc("init", 5, (char **) params);
  pcb1 = &pready.ls[3];

  err = (res == OMGROXX || res >= 0) &&
    (pcb1->pid == 0) &&
    (strcmp(pcb1->name, "init") == 0) &&
    (pcb1->pri == 5) &&
    (pcb1->supervisor == -1) &&
    (pcb1->registers.epc_reg == prog_address) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 1), "1") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 2), "12") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 3), "123") == 0) &&
    (strcmp(argn((char **) pcb1->registers.a_reg[0], 4), "1234") == 0) &&
    (pcb1->sleep == 0) && (pcb1->error == OMGROXX) && (pcb1->empty == FALSE);
  if (!err)
  {
    for (i = 0; i < MAXPCB; i++)
      if (pcb1->supervised[i] != -1)
      {
        err = FALSE;
        break;
      }
  }
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprintln("------------TEST MODULE KPROCESS END--------------");
  kprintln("");


  rm_all_pls();
}
