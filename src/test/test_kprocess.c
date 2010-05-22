/**
 * @file test_kprocess.c
 * @brief Test kprocess module.
 */

#include <string.h>
#include <errno.h>
#include "../kernel/kprocess.h"
#include "../kernel/kinout.h"
#include "../kernel/kernel.h"

void
test_kprocess()
{
	int i;
	bool err;
	pcb *pcb1;
	pcb *pcb2;
	uint32_t res;
	char params[4][20], params2[4][20];
	strcpy("1", params[0]);
	strcpy("12", params[1]);
	strcpy("123", params[2]);
	strcpy("1234", params[3]);
	strcpy("a", params2[0]);
	strcpy("ab", params2[1]);
	strcpy("123", params2[2]);
	strcpy("1234", params2[3]);
	char resc[3];
	strcpy("", resc);	

	kprintln("------TEST MODULE KPROCESS BEGIN------");

	create_pls(&pready);
	create_pls(&prunning);
	create_pls(&pterminate);
	create_pls(&pwaiting);

	kprintln("list created\t\t\tOK");

	kprint("create_proc\t\t\t");														// testCREATE1
	res = create_proc("testCREATE1", 10, (int32_t *)params);
	pcb1 = &pready.ls[0];

	err = (res == OMGROXX) &&
			(pcb1->pid == 0) &&
			(strcmp(pcb1->name, "testCREATE1") == 0) &&
			(pcb1->pri == 10) &&
			(pcb1->supervisor == -1) &&
/*			(strcmp((char *)pcb1->registers.a_reg[0], "1")) &&
			(strcmp((char *)pcb1->registers.a_reg[1], "12")) &&
			(strcmp((char *)pcb1->registers.a_reg[2], "123")) &&
			(strcmp((char *)pcb1->registers.a_reg[3], "1234")) &&
*/			(pcb1->wait == 0) &&
			(pcb1->error == OMGROXX) &&
			(pcb1->empty == FALSE);
	if(!err)
	{
		for(i=0; i<NSUPERVISED; i++)
			if(pcb1->supervised[i] != -1)
			{
				err = FALSE;
				break;
			}
	}
	if(err)
		kprintln("OK");
	else
		kprintln("NOK");

	res = create_proc("testCREATE2", 15, (int32_t *)params2);

	kprint("create_proc test2\t\t");													// testCREATE2
	pcb2 = &pready.ls[1];

	err = (res == OMGROXX) &&
			(pcb2->pid == 1) &&
			(strcmp(pcb2->name, "testCREATE2") == 0) &&
			(pcb2->pri == 15) &&
			(pcb2->supervisor == -1) &&
/*			(strcmp((char *)pcb2->registers.a_reg[0], "1")) &&
			(strcmp((char *)pcb2->registers.a_reg[1], "12")) &&
			(strcmp((char *)pcb2->registers.a_reg[2], "123")) &&
			(strcmp((char *)pcb2->registers.a_reg[3], "1234")) &&
*/			(pcb2->wait == 0) &&
			(pcb2->error == OMGROXX) &&
			(pcb2->empty == FALSE);
	if(!err)
	{
		for(i=0; i<NSUPERVISED; i++)
			if(pcb2->supervised[i] != -1)
			{
				err = FALSE;
				break;
			}
	}
	if(err)
		kprintln("OK");
	else
		kprintln("NOK");

	kprint("rm_p\t\t\t");																// testRM1
	res = rm_p(pcb1);
	err = (res == OMGROXX) &&
			(pcb1->pid == 0) &&
			(pcb1->empty == TRUE);
	if(err)
		kprintln("OK");
	else
		kprintln("NOK");


	kprint("create_proc after remove\t");											// testCREATE3
	res = create_proc("testCREATE3", 12, (int32_t *)params2);
	pcb1 = &pready.ls[0];

	err = (res == OMGROXX) &&
			(pcb1->pid == 2) &&
			(strcmp(pcb1->name, "testCREATE3") == 0) &&
			(pcb1->pri == 12) &&
			(pcb1->supervisor == -1) &&
/*			(strcmp((char *)pcb1->registers.a_reg[0], "1")) &&
			(strcmp((char *)pcb1->registers.a_reg[1], "12")) &&
			(strcmp((char *)pcb1->registers.a_reg[2], "123")) &&
			(strcmp((char *)pcb1->registers.a_reg[3], "1234")) &&
*/			(pcb1->wait == 0) &&
			(pcb1->error == OMGROXX) &&
			(pcb1->empty == FALSE);
	if(!err)
	{
		for(i=0; i<NSUPERVISED; i++)
			if(pcb1->supervised[i] != -1)
			{
				err = FALSE;
				break;
			}
	}
	if(err)
		kprintln("OK");
	else
		kprintln("NOK");

	res = chg_ppri(pcb1, 8);
	err = (res == OMGROXX) &&
			(pcb1->pri == 8);
	if(err)
		kprintln("OK");
	else
		kprintln("NOK");

	kprintln("------TEST MODULE KPROCESS END--------");
	kprintln("");
}
