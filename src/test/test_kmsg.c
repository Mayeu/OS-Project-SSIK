/**
 * @file test_kmsg.c
 * @brief Test kmsg module.
 */

#include <string.h>
#include "../kernel/kernel.h"
#include "../kernel/kinout.h"
#include "../kernel/kprocess.h"
#include "../kernel/kmsg.h"

void            test_unit(bool err, int res);

void
test_kmsg()
{
  pcb            *pcb0, *pcb1;
  msg            m1;
  msg            m2;
  msg            m3;
  msg            *m, *mb;
  char            params[MAX_ARG + 1][ARG_SIZE];
  char            mess1[10] = "Hello";
  int             res, res0, res1, res2;
  bool            err;
  msg_arg         msgarga = { "Hello1", CHAR_PTR, 1, 10, -1, 0 };
//  msg_arg         msgargb = { "Hello2", CHAR_PTR, 0, 10, -1, 0 };
//  msg_arg         msgargc = { "Hello3", CHAR_PTR, 0, 10, -1, 0 };
//  msg_arg         msgarg2 = { (void *) 5, INT_T, 0, 10, -1, 0 };
//  msg_arg         msgarg3 = { "other", CHAR_PTR, 0, 13, -1, 0 };
//  msg_arg         msgarg4 = { (void *) 42, INT_T, 0, 15, -1, 0 };
//  msg_arg         msgarg5 = { (void *) 43, INT_T, 0, 15, -1, 0 };
 // msg_arg         msgargres = { (void *) messres, CHAR_PTR, 0, 0, -1, FTYPE };  // want to recv a CHAR_PTR and put it in messres
//  msg_arg         msgargres2 = { (void *) &messres2, INT_T, 0, 0, -1, FTYPE };  // want to recv a INT_T and put it in messres2
//  msg_arg         msgargres3 = { (void *) &messres3, CHAR_PTR, 0, 0, -1, FTYPE };       // want to recv a CHAR_T and put it in messres3
//  msg_arg         msgargres4 = { (void *) &messres4, INT_T, 1, 0, -1, FPID };   // want to recv a INT_T from pid 1 and put it in messres4
//  msg_arg         msgargres5 = { (void *) &messres5, INT_T, 0, 15, -1, FPRI };  // want to recv a INT_T with prio = 15 and put it in messres5
//  msg_arg         msgargres6 = { (void *) &messres6, CHAR_PTR, 0, 0, -1, FPID };        // want to recv a INT_T with prio = 15 and put it in messres5
  strcpy("4", params[0]);       // nb of parameters after
  strcpy("1", params[1]);
  strcpy("12", params[2]);
  strcpy("123", params[3]);
  strcpy("1234", params[4]);
  kprintln("--------------TEST MODULE KMSG BEGIN--------------");
  
  create_proc("init", 10, (char **) params);
  create_proc("init", 11, (char **) params);
  create_proc("init", 12, (char **) params);

  kprint("create_msg\t\t\t\t\t");
  pcb0 = search_all_list(0);
  pcb1 = search_all_list(1);

  res0 = create_msg(NULL, 0, 1, 10, mess1, CHAR_PTR);      //CREATE 1
  res1 = create_msg(&m1, 0, 1, 10, NULL, CHAR_PTR);      //CREATE 1
  res2 = create_msg(&m1, 0, 1, 10, mess1, CHAR_PTR);      //CREATE 1
  err = (res0 == NULLPTR) &&
    (res1 == NULLPTR) && 
    (res2 == OMGROXX) &&
    (m1.sdr_pid == 0) &&
    (m1.recv_pid == 1) &&
    (m1.pri == 10) && (strcmp(m1.data, mess1) == 0) && (m1.datatype == CHAR_PTR);
  test_unit(err, res);

  kprint("copy_msg\t\t\t\t\t"); //COPY
  res = copy_msg(&m1, &m2);
  err = (res == OMGROXX) &&
    (m2.sdr_pid == 0) &&
    (m2.recv_pid == 1) &&
    (m2.pri == 10) && (strcmp(m2.data, mess1) == 0) && (m2.datatype == CHAR_PTR);
  test_unit(err, res);

  m2.sdr_pid = 0;
  m2.recv_pid = 0;
  m2.pri = 0;

  kprint("push_mls\t\t\t\t\t"); //PUSH
  res = push_mls(&pcb0->messages, &m1);
  m  = &pcb0->messages.ls[0];
  err = (res == OMGROXX) &&
    (m->sdr_pid == 0) &&
    (m->recv_pid == 1) &&
    (m->pri == 10) &&
    (strcmp(m->data, mess1) == 0) &&
    (m->datatype == CHAR_PTR) &&
    (pcb0->messages.length == 1);
  test_unit(err, res);

  kprint("pop_mls\t\t\t\t\t\t"); //POP
  res = pop_mls(&pcb0->messages, &m3);
  err = (res == OMGROXX) &&
    (m3.sdr_pid == 0) &&
    (m3.recv_pid == 1) &&
    (m3.pri == 10) &&
    (strcmp(m3.data, mess1) == 0) &&
    (m3.datatype == CHAR_PTR) &&
    (pcb0->messages.length == 0);
  test_unit(err, res);

  reset_mls(&pcb0->messages);
  reset_mls(&pcb1->messages);

  kprint("send_msg string 1\t\t\t\t");
  res = send_msg(0, &msgarga);  // process 0 send a message to process 1
  mb = &pcb1->messages.ls[0];
  err = (res == OMGROXX) &&
    (mb->sdr_pid == 0) &&
    (mb->recv_pid == 1) &&
    (mb->pri == 10) &&
    (strcmp(mb->data, "Hello1") == 0) &&
    (mb->datatype == CHAR_PTR) &&
    (pcb0->messages.length == 1);
  test_unit(err, res);


  kprintln("---------------TEST MODULE KMSG END---------------");
  kprintln("");

}
