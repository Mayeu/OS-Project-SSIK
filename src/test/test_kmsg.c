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
test_kmsg()
{
	pcb *pcb1;
	msg *m1;
	msg *m2;
	char params[MAX_ARG+1][ARG_SIZE];
	char mess1[10] = "Hello";
	int res;
	bool err;
	strcpy("4", params[0]);	// nb of parameters after
	strcpy("1", params[1]);
	strcpy("12", params[2]);
	strcpy("123", params[3]);
	strcpy("1234", params[4]);
	kprintln("--------------TEST MODULE KMSG BEGIN--------------");
	create_all_pls();
	create_proc("init", 10, (char **)params);
	create_proc("init", 11, (char **)params);
	create_proc("init", 12, (char **)params);

	kprint("create_msg\t\t\t\t\t");
	pcb1 = &pready.ls[0];
	m1 = &pcb1->messages.ls[0];
	res = create_msg(m1, 0, 1, 10, mess1, CHAR_PTR);								//CREATE 1
	err = (res == OMGROXX) &&
			(m1->sdr_pid == 0) &&
			(m1->recv_pid == 1) &&
			(m1->pri == 10) &&
			(m1->data == mess1) &&
			(m1->datatype == CHAR_PTR);
	test_unit(err, res);

	m2 = &pcb1->messages.ls[1];
	res = create_msg(m2, 0, 1, 11, mess1, CHAR_PTR);								//CREATE 2

	kprint("copy_msg\t\t\t\t\t");															//COPY
	res = copy_msg(m1, m2);
	err = (res == OMGROXX) &&
			(m2->sdr_pid == 0) &&
			(m2->recv_pid == 1) &&
			(m2->pri == 10) &&
			(m2->data == mess1) &&
			(m2->datatype == CHAR_PTR);
	test_unit(err, res);

  kprintln("---------------TEST MODULE KMSG END---------------");
  kprintln("");

}
