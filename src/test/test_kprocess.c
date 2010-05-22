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
//	pcb pcb2;
//	pcb *ptr_pcb1, *ptr_pcb2;
	uint32_t res;
	char params[4][20];
	strcpy("1", params[0]);
	strcpy("12", params[1]);
	strcpy("134", params[2]);
	strcpy("1234", params[3]);
//	char resc[3];
	

	kprintln("------TEST MODULE KPROCESS BEGIN------");

	create_pls(&pready);
	create_pls(&prunning);
	create_pls(&pterminate);
	create_pls(&pwaiting);

	kprintln("list created\t\t\t\tOK");

	res = create_proc("test1", 10, (int32_t *)params);

	kprint("create_proc");
	pcb1 = &pready.ls[0];

	err = (res == OMGROXX) &&
			(pcb1->pid == 0) &&
			(strcmp(pcb1->name, "test1") == 0) &&
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
		kprintln("\t\t\tOK");
	else
		kprintln("\t\t\tNOK");






	kprint("------TEST MODULE KPROCESS END--------\n");
}
