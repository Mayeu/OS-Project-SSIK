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
//	pcb pcb1, pcb2;
//	pcb *ptr_pcb1, *ptr_pcb2;
	uint32_t res;
	int32_t params[4] = {1, 2, 3, 4};
//	char resc[3];

	kprintln("------TEST MODULE KPROCESS BEGIN------");

	create_pls(&pready);
	create_pls(&prunning);
	create_pls(&pterminate);
	create_pls(&pwaiting);

	kprintln("list created\t\t\t\tOK");

	res = create_proc("test1", 10, params);

	kprint("create_proc");
	if(res == OMGROXX && pready.ls[0].pid == 0 && strcmp(pready.ls[0].name, "test1"))
		kprintln("\t\t\tOK");
	else
		kprintln("\t\t\tNOK");
		
//	kprintln(itos(res, resc));
//	kprint(atoi(pready.ls[0].pid));
//	kprint("\n");
//	kprint(pready.ls[0].name);





	kprint("------TEST MODULE KPROCESS END--------\n");
}
