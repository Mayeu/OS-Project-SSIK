/**
 * @file test_kprocess.c
 * @brief Test kprocess module.
 */

#include "../kernel/kprocess.h"
#include "../kernel/kinout.h"
#include "../kernel/kernel.h"
#include <string.h>


void
test_kprocess_list()
{
  int             i, res;
  bool            err;
  pcb            *pcb1;
  pcb            *pcb2;
  pcb            *pcb3;
  char            params[4][20];
  strcpy("1", params[0]);
  strcpy("2", params[1]);
  strcpy("3", params[2]);
  strcpy("4", params[3]);

  create_all_pls();

  kprintln("----------TEST MODULE KPROCESS_LIST BEGIN---------");

  kprint("create_pls\t\t\t\t\t");       // testCREATE1
  res = create_pls(&pready);
  err = (res == OMGROXX) && (pready.current == NULL);
  if (!err)
  {
    for (i = 0; i < MAXPCB && res; i++)
      if (pready.ls[i].empty == FALSE)
      {
        err = FALSE;
        break;
      }
  }
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("rm_pls\t\t\t\t\t\t"); // testRM1
  res = rm_pls(&pready);
  err = (res == OMGROXX) && (pready.current == NULL);
  if (!err)
  {
    for (i = 0; i < MAXPCB && res; i++)
      if (pready.ls[i].empty == FALSE)
      {
        err = FALSE;
        break;
      }
  }
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  // recreate the list to continue the tests
  create_pls(&pready);

  kprint("rm_from_pls\t\t\t\t\t");      // testRM_FROM_PLS1
  create_proc("init", 15, (char **) params);
  res = rm_from_pls(0, &pready);
  err = (res == OMGROXX) &&
    (pready.ls[0].pid == 0) && (pready.ls[0].empty == TRUE);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("empty_space\t\t\t\t\t");
  pcb1 = empty_space(&pready);
  err = (pcb1 == &pready.ls[0]) && (pcb1->empty == TRUE);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("pls_is_empty true\t\t\t\t");
  res = pls_is_empty(&pready);
  err = (res == TRUE);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("pls_is_empty false\t\t\t\t");
  create_proc("init", 15, (char **) params);
  res = pls_is_empty(&pready);
  err = (res == FALSE);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("search_pcb\t\t\t\t\t");
  pcb1 = search_pcb(1, &pready);
  err = (pcb1 != NULL) && (pcb1 == &pready.ls[0]);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("searchall\t\t\t\t\t");
  pcb1 = searchall(1);
  err = (pcb1 != NULL) && (pcb1 == &pready.ls[0]);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("searchall not found\t\t\t\t");
  pcb3 = searchall(0);
  if (pcb3 == NULL)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("move\t\t\t\t\t\t");
  res = move(1, &pready, &prunning);
  pcb2 = search_pcb(1, &prunning);
  pcb3 = search_pcb(1, &pready);
  err = (res == OMGROXX) &&
    (pcb3 == NULL) &&
    (pcb2 != NULL) &&
    (pcb2->pid == pcb1->pid) &&
    (strcmp(pcb2->name, pcb1->name) == 0) &&
    (pcb2->pri == pcb1->pri) &&
    (pcb1->supervisor == -1) &&
    (strcmp
     (argn((char **) pcb2->registers.a_reg[0], 1),
      argn((char **) pcb1->registers.a_reg[0], 1)) == 0)
    &&
    (strcmp
     (argn((char **) pcb2->registers.a_reg[0], 2),
      argn((char **) pcb1->registers.a_reg[0], 2)) == 0)
    &&
    (strcmp
     (argn((char **) pcb2->registers.a_reg[0], 3),
      argn((char **) pcb1->registers.a_reg[0], 3)) == 0)
    && (pcb2->sleep == pcb1->sleep) && (pcb2->error == pcb1->error)
    && (pcb2->empty == FALSE);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprint("sort\t\t\t\t\t\t");
  create_proc("init", 15, (char **) params);
  create_proc("init", 10, (char **) params);
  create_proc("init", 20, (char **) params);
  res = sort(&pready);
  pcb1 = &pready.ls[0];
  pcb2 = &pready.ls[1];
  pcb3 = &pready.ls[2];
  err = (res == OMGROXX) &&
    (pcb1->pid == 4) && (pcb2->pid == 2) && (pcb3->pid == 3);
  if (err)
    kprintln("OK");
  else
    kprintln("NOK");

  kprintln("----------TEST MODULE KPROCESS_LIST END-----------\n");
  kprintln("");

  rm_all_pls();
}
