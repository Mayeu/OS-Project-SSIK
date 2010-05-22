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
	int i, res = 1;
	bool bres;
	pcb *pres;
	char params[4][20];
	char buf[255];
	strcpy("1", params[0]);
	strcpy("2", params[1]);
	strcpy("3", params[2]);
	strcpy("4", params[3]);
//	uint32_t res;
//	char resc[3];

	create_all_pls();

	kprint("------TEST MODULE KPROCESS_LIST BEGIN------\n");

	kprint("create_pls:\t");
	create_pls(&pready);
	for (i=0; i<MAX_PROC && res ; i++)
	{
		if (pready.ls[i].empty == FALSE)
			res = 0;
	}
	if (pready.current != NULL)
		res = 0;
	if (res == 1)
		kprintln("OK");
	else
		kprintln("ERROR");

	kprint("rm_pls:\t\t");

	rm_pls(&pready);
	for (i=0; i<MAX_PROC && res ; i++)
	{
		if (pready.ls[i].empty == FALSE)
			res = 0;
	}
	if (pready.current != NULL)
		res = 0;
	if (res == 1)
		kprintln("OK");
	else
		kprintln("ERROR");

	kprint("rm_from_pls:\tEN ATTENTE\n");
	
	kprint("empty_space:\t");
	pres = empty_space(&pready);
	if (pres->empty == TRUE)
		kprintln("OK");
	else
		kprintln("ERROR");

	kprint("pls_is_empty:\t");

	bres = pls_is_empty(&pready);
	if (bres == TRUE)
		kprintln("OK");
	else
		kprintln("ERROR");

	kprint("search_pls:\t");

	create_proc("test", 15, (int32_t*)params);
	pres = search_pcb(0, &pready);
	if (pres->pid == 0)
		kprintln("OK");
	else
		kprintln("ERROR");

	kprint("searchall:\t");

	create_proc("test2", 15, (int32_t*)params);
	pres = searchall(1);
	if (pres->pid == 1)
		kprintln("OK");
	else
		kprintln("ERROR");

	kprint("searchall pcb inexistant:\t");
	pres = searchall(3);
	if (pres == NULL)
		kprintln("OK");
	else
		kprintln("ERROR");

	kprint("move:\t\t");
	move(1, &pready, &prunning);
	res = 1;
	pres = search_pcb(1, &pready);
	if (pres != NULL)
		res = 0;
	pres = search_pcb(1, &prunning);
	if (pres == NULL)
		res = 0;
	if (res == 1)
		kprintln("OK");
	else
		kprintln("ERROR");

	kprint("sort:\t\t");

	create_proc("test3", 20, (int32_t*)params);
	create_proc("test4",  5, (int32_t*)params);
	create_proc("test5", 10, (int32_t*)params);

	//sort(&pready);
	
	kprintln(itos(prunning.ls[0].pid, buf));
	kprintln(itos(pready.ls[0].pid, buf));
	kprintln(itos(pready.ls[1].pid, buf));
	kprintln(itos(pready.ls[2].pid, buf));
	kprintln(itos(pready.ls[3].pid, buf));

	kprint("------TEST MODULE KPROCESS_LIST END--------\n");

	rm_all_pls();

}
