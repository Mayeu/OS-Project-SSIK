/**
 * @file test_kmsg.c
 * @brief Test kmsg module.
 */

#include <string.h>
#include "../kernel/kinout.h"
#include "../kernel/kprocess.h"
#include "../kernel/kmsg.h"

void            test_unit(bool err, int res);

void
test_kmsg_lst()
{
  pcb            *pcb1;
  pcb            *pcb2;
  mls            *ls1;
  mls            *ls2;
  msg            *m1;
  msg            *m2;
  char            params[MAX_ARG + 1][ARG_SIZE];
  char            messres[10];
  int             messres2;
  char            messres3[10];
  int             messres4;
  int             messres5;
  char            messres6[10];
  char            resc[10];
  int             res;
  bool            err;
  resc[0] = '\0';
  msg_arg         msgarga = { "Hello1", CHAR_PTR, 1, 10, -1, 0 };
  msg_arg         msgargb = { "Hello2", CHAR_PTR, 0, 10, -1, 0 };
  msg_arg         msgargc = { "Hello3", CHAR_PTR, 0, 10, -1, 0 };
  msg_arg         msgarg2 = { (void *) 5, INT_T, 0, 10, -1, 0 };
  msg_arg         msgarg3 = { "other", CHAR_PTR, 0, 13, -1, 0 };
  msg_arg         msgarg4 = { (void *) 42, INT_T, 0, 15, -1, 0 };
  msg_arg         msgarg5 = { (void *) 43, INT_T, 0, 15, -1, 0 };

  msg_arg         msgargres = { (void *) messres, CHAR_PTR, 0, 0, -1, FTYPE };  // want to recv a CHAR_PTR and put it in messres
  msg_arg         msgargres2 = { (void *) &messres2, INT_T, 0, 0, -1, FTYPE };  // want to recv a INT_T and put it in messres2
  msg_arg         msgargres3 = { (void *) &messres3, CHAR_PTR, 0, 0, -1, FTYPE };       // want to recv a CHAR_T and put it in messres3
  msg_arg         msgargres4 = { (void *) &messres4, INT_T, 1, 0, -1, FPID };   // want to recv a INT_T from pid 1 and put it in messres4
  msg_arg         msgargres5 = { (void *) &messres5, INT_T, 0, 15, -1, FPRI };  // want to recv a INT_T with prio = 15 and put it in messres5
  msg_arg         msgargres6 = { (void *) &messres6, CHAR_PTR, 0, 0, -1, FPID };        // want to recv a INT_T with prio = 15 and put it in messres5

  strcpy("4", params[0]);       // nb of parameters after
  strcpy("1", params[1]);
  strcpy("12", params[2]);
  strcpy("123", params[3]);
  strcpy("1234", params[4]);
  kprintln("------------TEST MODULE KMSG_LST BEGIN------------");
  create_all_pls();
  create_proc("init", 10, (char **) params);
  create_proc("init", 11, (char **) params);
  create_proc("init", 12, (char **) params);
  pcb1 = &pready.ls[0];
  pcb2 = &pready.ls[1];
  ls1 = &pcb1->messages;
  ls2 = &pcb2->messages;
  reset_mls(ls1);
  reset_mls(ls2);




  kprintln("---------------TEST MODULE KMSG END---------------");
  kprintln("");

}
