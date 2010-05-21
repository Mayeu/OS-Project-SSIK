/**
 * @file test_kprocess.c
 * @brief Test kprocess module.
 */

#include "../kernel/kprocess.h"
#include "../kernel/kinout.h"
#include "../kernel/kernel.h"
#include <string.h>

void
test_kprocess()
{
//	pcb pcb1, pcb2;
//	pcb *ptr_pcb1, *ptr_pcb2;
	uint32_t res;
//	uint32_t params[4] = {1, 2, 3, 4};
	char resc[3];

	create_pls(&pready);
	create_pls(&prunning);
	create_pls(&pterminate);
	create_pls(&pwaiting);

//	res = create_proc("test1", 10, params);

	kprint("------TEST MODULE KPROCESS BEGIN------\n");

	kprint(itos(res, resc));
//	kprint(atoi(pready.ls[0].pid));
//	kprint("\n");
//	kprint(pready.ls[0].name);





	kprint("------TEST MODULE KPROCESS END--------\n");
}
