/**
 * @file test_kmsg.c
 * @brief Test kmsg module.
 */

#include <string.h>
#include "../kernel/kinout.h"
#include "../kernel/kinout.h"
#include "../kernel/kprocess.h"
#include "../kernel/kmsg_lst.h"

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
  msg            *m;
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
  msg_arg         msgarga = { "Hello1", CHAR_PTR, 1, 10, -1, 0 };       // msg with mid 0
  msg_arg         msgargb = { "Hello2", CHAR_PTR, 0, 10, -1, 0 };       // msg with mid 1
  msg_arg         msgargc = { "Hello3", CHAR_PTR, 0, 10, -1, 0 };       // msg with mid 2
  msg_arg         msgarg2 = { (void *) 5, INT_T, 0, 10, -1, 0 };        // msg with mid 3
  msg_arg         msgarg3 = { "other", CHAR_PTR, 0, 13, -1, 0 };        // msg with mid 4
  msg_arg         msgarg4 = { (void *) 42, INT_T, 0, 15, -1, 0 };       // msg with mid 5
  msg_arg         msgarg5 = { (void *) 43, INT_T, 0, 15, -1, 0 };       // msg with mid 6

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
  m1 = &pcb1->messages.ls[0];
  m2 = &pcb1->messages.ls[1];
  init_msg_lst(ls1);
  init_msg_lst(ls2);
  res = send_msg(0, &msgarga);  //CREATE 1 mid = 0                         process 0 send a message to process 0
  send_msg(1, &msgargb);        //CREATE 2 mid = 1                               process 1 send a message to process 0

  kprint("search_msg\t\t\t\t\t");       //SEARCH_MSG
  m = search_msg(1, ls1);
  err = (m != NULL) &&
    (m->mid == 1) &&
    (m->sdr_pid == 1) &&
    (m->recv_pid == 0) &&
    (m->pri == 10) &&
    (m->empty == FALSE) &&
    (strcmp(m->data, "Hello2") == 0) && (m->datatype == CHAR_PTR);
  test_unit(err, 0);

  kprint("search_msg_all\t\t\t\t\t");   //SEARCH_MSG_ALL
  m = search_msg_all(0);
  err = (m != NULL) &&
    (m->mid == 0) &&
    (m->sdr_pid == 0) &&
    (m->recv_pid == 1) &&
    (m->pri == 10) &&
    (m->empty == FALSE) &&
    (strcmp(m->data, "Hello1") == 0) && (m->datatype == CHAR_PTR);
  test_unit(err, 0);

  kprint("empty_msg_space\t\t\t\t\t");  //EMPTY_MSG_SPACE
  m = empty_msg_space(ls1);
  err = (m != NULL) && (m == &ls1->ls[1]);
  test_unit(err, res);

  kprint("is_empty_mls\t\t\t\t\t");     //IS_EMPTY_MLS
  res = is_empty_mls(ls1);
  err = (res == FALSE);
  test_unit(err, res);
  //EMPTY_MSG_SPACE
  //m = empty_msg_space(ls1);
  kprint("send_msg string\t\t\t\t\t");  //SEND_MSG
  res = send_msg(1, &msgargc);  // mid = 2                              process 1 send a message to process 0
  err = (res == OMGROXX) &&
    (m->mid == 2) &&
    (m->sdr_pid == 1) &&
    (m->recv_pid == 0) &&
    (m->pri == 10) &&
    (m->empty == FALSE) &&
    (strcmp(m->data, "Hello3") == 0) && (m->datatype == CHAR_PTR);
  test_unit(err, res);

  kprint("recv_msg string\t\t\t\t\t");  //RECV_MSG
  res = recv_msg(0, &msgargres);        // process 0 wants to receive a CHAR_PTR and put it in messres
  err = (res == OMGROXX) && (strcmp(messres, "Hello2") == 0);
  test_unit(err, res);

  m = empty_msg_space(ls1);
  kprint("send_msg int\t\t\t\t\t");     //SEND_MSG
  res = send_msg(1, &msgarg2);  // mid = 3
  err = (res == OMGROXX) &&
    (m->mid == 3) &&
    (m->sdr_pid == 1) &&
    (m->recv_pid == 0) &&
    (m->pri == 10) &&
    (m->empty == FALSE) && ((int) m->data == 5) && (m->datatype == INT_T);
  test_unit(err, res);

  kprint("recv_msg int\t\t\t\t\t");     //RECV_MSG
  res = recv_msg(0, &msgargres2);       // process 0 wants to receive a INT_T and put it in messres2

  err = (res == OMGROXX) && (messres2 == 5);
  test_unit(err, res);

  m = empty_msg_space(ls1);
  kprint("send_msg string2\t\t\t\t");   //SEND_MSG
  res = send_msg(1, &msgarg3);  // mid = 4
  err = (res == OMGROXX) &&
    (m->mid == 4) &&
    (m->sdr_pid == 1) &&
    (m->recv_pid == 0) &&
    (m->pri == 13) &&
    (m->empty == FALSE) &&
    (strcmp(m->data, "other") == 0) && (m->datatype == CHAR_PTR);
  test_unit(err, res);

  kprint("recv_msg string2\t\t\t\t");   //RECV_MSG
  res = recv_msg(0, &msgargres3);       // process 0 wants to receive a CHAR_T and put it in messres3
  err = (res == OMGROXX) && (strcmp(messres3, "other") == 0);
  test_unit(err, res);

  m = empty_msg_space(ls1);
  kprint("send_msg by PID\t\t\t\t\t");  //SEND_MSG
  res = send_msg(1, &msgarg4);  // mid = 5
  err = (res == OMGROXX) &&
    (m->mid == 5) &&
    (m->sdr_pid == 1) &&
    (m->recv_pid == 0) &&
    (m->pri == 15) &&
    (m->empty == FALSE) && ((int) m->data == 42) && (m->datatype == INT_T);
  test_unit(err, res);
  //kprint(itos(,resc));

  m = empty_msg_space(ls1);
  kprint("send_msg by PID\t\t\t\t\t");  //SEND_MSG
  res = send_msg(1, &msgarg5);  // mid = 6
  err = (res == OMGROXX) &&
    (m->mid == 6) &&
    (m->sdr_pid == 1) &&
    (m->recv_pid == 0) &&
    (m->pri == 15) &&
    (m->empty == FALSE) && ((int) m->data == 43) && (m->datatype == INT_T);
  test_unit(err, res);

  kprint("recv_msg by PID\t\t\t\t\t");  //RECV_MSG
  res = recv_msg(0, &msgargres4);       // process 0 wants to receive a INT_T from pid = 1 and put it in messres4

  err = (res == OMGROXX) && ((int) messres4 == 42);
  test_unit(err, res);

  kprint("recv_msg by PRIO\t\t\t\t");   //RECV_MSG
  res = recv_msg(0, &msgargres5);       // process 0 wants to receive a INT_T wit prio = 15 and put it in messres5

  err = (res == OMGROXX) && ((int) messres5 == 43);
  test_unit(err, res);

  kprint("recv_msg by PID\t\t\t\t\t");  //RECV_MSG
  res = recv_msg(1, &msgargres6);       // process 0 wants to receive a INT_T wit prio = 15 and put it in messres5
  err = (res == OMGROXX) && (strcmp(messres6, "Hello1") == 0);
  test_unit(err, res);

  kprintln("---------------TEST MODULE KMSG END---------------");
  kprintln("");

}
