/**
 * @file uart_info_fifo.c
 * @brief test the fifo buffer of the uart module
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/kprocess.h"
#include "../kernel/kscheduler.h"

int32_t test_schedule() ;

void test_kscheduler()
{
	int e;
	char c;

	kprintln("------------TEST MODULE SCHEDULER BEGIN-----------");

	kprint("Test the scheduler\t\t\t\t");
	e = test_schedule() ;
	if (e == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprintln("------------TEST MODULE SCHEDULER END-------------");
}

int32_t test_schedule()
{
	/*
	 * First a simple case:
	 * A process with a priority of 42 is in the ready list.
	 * And the running list is empty.
	 */

	pcb p;

	pcls_reset(&pclsready);
	pcls_reset(&pclsrunning);

	/*
	 * We add the pri42 to pclsready
	 */
	pcb_reset(&p);
	pcb_set_name(&p, "pri41");
	pcb_set_pri(&p, 41);
	pcb_set_state(&p, READY);
	pcb_set_empty(&p,FALSE);
	pcls_add(&pclsready, &p);

	/*
	 * Go Go Scheduler!
	 */
	schedule();

	if(strcmp(pcb_get_name(&(pclsrunning.start->p)), "pri41") != 0
			|| pcb_get_state(&(pclsrunning.start->p)) != RUNNING)
		return -1;

	if(pclsready.start != NULL)
		return -2;

	if(get_current_pcb() != &(pclsrunning.start->p))
		return -3;

	/*
	 * Now we add pri12 inside the ready list
	 */
	pcb_reset(&p);
	pcb_set_name(&p, "pri2");
	pcb_set_pri(&p, 2);
	pcb_set_state(&p, READY);
	pcb_set_empty(&p,FALSE);
	pcls_add(&pclsready, &p);

	schedule();

	if(strcmp(pcb_get_name(&(pclsready.start->p)), "pri2") != 0
			|| pcb_get_state(&(pclsready.start->p)) != READY)
		return -4;

	if(strcmp(pcb_get_name(&(pclsrunning.start->p)), "pri41") != 0
			|| pcb_get_state(&(pclsrunning.start->p)) != RUNNING)
		return -5;

	if(get_current_pcb() != &(pclsrunning.start->p))
		return -6;

	/*
	 * Now we add a new 41
	 */

	pcb_reset(&p);
	pcb_set_name(&p, "pri412");
	pcb_set_pri(&p, 41);
	pcb_set_state(&p, READY);
	pcb_set_empty(&p,FALSE);
	pcls_add(&pclsready, &p);

	schedule();

	if(strcmp(pcb_get_name(&(pclsready.start->p)), "pri2") != 0
			|| pcb_get_state(&(pclsready.start->p)) != READY)
		return -7;

	if(strcmp(pcb_get_name(&(pclsrunning.start->p)), "pri412") != 0
			|| pcb_get_state(&(pclsrunning.start->p)) != RUNNING)
		return -8;

	if(strcmp(pcb_get_name(&(pclsrunning.start->next->p)), "pri41") != 0
			|| pcb_get_state(&(pclsrunning.start->next->p)) != RUNNING)
		return -9;

	if(get_current_pcb() != &(pclsrunning.start->next->p))
		return -10;


	/*
	 * We add 42 to the ready list
	 */

	pcb_reset(&p);
	pcb_set_name(&p, "pri42");
	pcb_set_pri(&p, 42);
	pcb_set_state(&p, READY);
	pcb_set_empty(&p,FALSE);
	pcls_add(&pclsready, &p);

	schedule();

	if(strcmp(pcb_get_name(&(pclsready.start->p)), "pri41") != 0
			|| pcb_get_state(&(pclsready.start->p)) != READY)
		return -11;

	if(strcmp(pcb_get_name(&(pclsready.start->next->p)), "pri412") != 0
			|| pcb_get_state(&(pclsready.start->next->p)) != READY)
		return -12;

	if(strcmp(pcb_get_name(&(pclsready.start->next->next->p)), "pri2") != 0
			|| pcb_get_state(&(pclsready.start->next->next->p)) != READY)
		return -13;

	if(strcmp(pcb_get_name(&(pclsrunning.start->p)), "pri42") != 0
			|| pcb_get_state(&(pclsrunning.start->p)) != RUNNING)
		return -14;

	pcls_reset(&pclsready);
	pcls_reset(&pclsrunning);

	schedule();

	if(get_current_pcb() != NULL)
		return -15;

	return OMGROXX;
}
