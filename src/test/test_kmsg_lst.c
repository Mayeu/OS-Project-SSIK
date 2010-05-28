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
	pcb *pcb1;
	pcb *pcb2;
	mls *ls1;
	mls *ls2;
	msg *m1;
	msg *m2;
	char params[MAX_ARG+1][ARG_SIZE];
	char messres[10];
	int messres2;
	char messres3[10];
	int messres4;
	int messres5;
	char messres6[10];
	char resc[10];
	int res;
	bool err;
	resc[0] = '\0';
	msg_arg msgarga = {"Hello1", CHAR_PTR, 1, 10, -1, 0};
	msg_arg msgargb = {"Hello2", CHAR_PTR, 0, 10, -1, 0};
	msg_arg msgargc = {"Hello3", CHAR_PTR, 0, 10, -1, 0};
	msg_arg msgarg2 = {(void *)5, INT_T, 0, 10, -1, 0};
	msg_arg msgarg3 = {"other", CHAR_PTR, 0, 13, -1, 0};
	msg_arg msgarg4= {(void *)42, INT_T, 0, 15, -1, 0};
	msg_arg msgarg5= {(void *)43, INT_T, 0, 15, -1, 0};

	msg_arg msgargres = {(void *)messres, CHAR_PTR, 0, 0, -1, FTYPE};		// want to recv a CHAR_PTR and put it in messres
	msg_arg msgargres2 = {(void *)&messres2, INT_T, 0, 0, -1, FTYPE};	// want to recv a INT_T and put it in messres2
	msg_arg msgargres3 = {(void *)&messres3, CHAR_PTR, 0, 0, -1, FTYPE};	// want to recv a CHAR_T and put it in messres3
	msg_arg msgargres4 = {(void *)&messres4, INT_T, 1, 0, -1, FPID};		// want to recv a INT_T from pid 1 and put it in messres4
	msg_arg msgargres5 = {(void *)&messres5, INT_T, 0, 15, -1, FPRI};		// want to recv a INT_T with prio = 15 and put it in messres5
	msg_arg msgargres6 = {(void *)&messres6, CHAR_PTR, 0, 0, -1, FPID};		// want to recv a INT_T with prio = 15 and put it in messres5

	strcpy("4", params[0]);	// nb of parameters after
	strcpy("1", params[1]);
	strcpy("12", params[2]);
	strcpy("123", params[3]);
	strcpy("1234", params[4]);
	kprintln("------------TEST MODULE KMSG_LST BEGIN------------");
	create_all_pls();
	create_proc("init", 10, (char **)params);
	create_proc("init", 11, (char **)params);
	create_proc("init", 12, (char **)params);
	pcb1 = &pready.ls[0];
	pcb2 = &pready.ls[1];
	ls1 = &pcb1->messages;
	ls2 = &pcb2->messages;
	reset_mls(ls1);
	reset_mls(ls2);

	kprint("send_msg string 1\t\t\t\t");	
	res = send_msg(0, &msgarga); 		// process 0 send a message to process 1
	m2 = &ls2->ls[ls2->out];
	err = (res == OMGROXX) &&
			(m2->sdr_pid == 0) &&
			(m2->recv_pid == 1) &&
			(m2->pri == 10) &&
			(strcmp(m2->data, "Hello1") == 0) &&
			(m2->datatype == CHAR_PTR);
	test_unit(err, res);

	kprint("send_msg string 2\t\t\t\t");	
	res = send_msg(1, &msgargb); 		// process 1 send a message to process 0
	m1 = &ls1->ls[ls1->out];
	err = (res == OMGROXX) &&
			(m1->sdr_pid == 1) &&
			(m1->recv_pid == 0) &&
			(m1->pri == 10) &&
			(strcmp(m1->data, "Hello2") == 0) &&
			(m1->datatype == CHAR_PTR);
	test_unit(err, res);


	kprint("send_msg string 3\t\t\t\t");												//SEND_MSG
	res = send_msg(1, &msgargc);		// process 1 send a message to process 0
	m1 = &ls1->ls[ls1->out+1];
	err = (res == OMGROXX) &&
			(m1->sdr_pid == 1) &&
			(m1->recv_pid == 0) &&
			(m1->pri == 10) &&
			(strcmp(m1->data, "Hello3") == 0) &&
			(m1->datatype == CHAR_PTR);
	test_unit(err, res);

	kprint("recv_msg string 1\t\t\t\t");												//RECV_MSG
	res = recv_msg(0, &msgargres);	// process 0 wants to receive a CHAR_PTR and put it in messres
	err = (res == OMGROXX) &&
			(strcmp(messres, "Hello2") == 0);
	test_unit(err, res);

	kprint("send_msg int 1\t\t\t\t\t");												//SEND_MSG
	res = send_msg(1, &msgarg2);
	m1 = &ls1->ls[ls1->out+1];
	err = (res == OMGROXX) &&
			(m1 != NULL) &&
			(m1->sdr_pid == 1) &&
			(m1->recv_pid == 0) &&
			(m1->pri == 10) &&
			((int)m1->data == 5) &&
			(m1->datatype == INT_T);
	test_unit(err, res);

	kprint("recv_msg int 1\t\t\t\t\t");												//RECV_MSG
	res = recv_msg(0, &msgargres2);	// process 0 wants to receive a INT_T and put it in messres2
	
	err = (res == OMGROXX) &&
			(messres2 == 5);
	test_unit(err, res);

	kprint("send_msg string 4\t\t\t\t");												//SEND_MSG
	res = send_msg(1, &msgarg3);
	m1 = &ls1->ls[ls1->out];
	err = (res == OMGROXX) &&
			(m1->sdr_pid == 1) &&
			(m1->recv_pid == 0) &&
			(m1->pri == 13) &&
			(strcmp(m1->data, "other") == 0) &&
			(m1->datatype == CHAR_PTR);
	test_unit(err, res);

	kprint("recv_msg string 4\t\t\t\t");												//RECV_MSG
	res = recv_msg(0, &msgargres3);	// process 0 wants to receive a CHAR_T and put it in messres3
	err = (res == 0) &&
			(strcmp(messres3, "other") == 0);
	test_unit(err, res);

	kprint("send_msg by PID\t\t\t\t\t");												//SEND_MSG
	res = send_msg(1, &msgarg4);
	m1 = &ls1->ls[ls1->out];
	err = (res == OMGROXX) &&
			(m1->sdr_pid == 1) &&
			(m1->recv_pid == 0) &&
			(m1->pri == 15) &&
			((int)m1->data == 42) &&
			(m1->datatype == INT_T);
	test_unit(err, res);

	kprint("send_msg by PID\t\t\t\t\t");												//SEND_MSG
	res = send_msg(1, &msgarg5);
	m1 = &ls1->ls[ls1->out+1];
	err = (res == OMGROXX) &&
			(m1->sdr_pid == 1) &&
			(m1->recv_pid == 0) &&
			(m1->pri == 15) &&
			((int)m1->data == 43) &&
			(m1->datatype == INT_T);
	test_unit(err, res);

	kprint("recv_msg by PID\t\t\t\t\t");												//RECV_MSG
	res = recv_msg(0, &msgargres4);	// process 0 wants to receive a INT_T from pid = 1 and put it in messres4
	
	err = (res == 1) &&
			((int)messres4 == 42);
	test_unit(err, res);

	kprint("recv_msg by PRIO\t\t\t\t");												//RECV_MSG
	res = recv_msg(0, &msgargres5);	// process 0 wants to receive a INT_T wit prio = 15 and put it in messres5
	
	err = (res == 0) &&
			((int)messres5 == 43);
	test_unit(err, res);
	
	kprint("recv_msg by PID\t\t\t\t\t");												//RECV_MSG
	res = recv_msg(1, &msgargres6);	// process 0 wants to receive a INT_T wit prio = 15 and put it in messres5
	err = (res == 0) &&
			(strcmp(messres6, "Hello1") == 0);
	test_unit(err, res);

	kprintln("---------------TEST MODULE KMSG END---------------");
	kprintln("");

}
