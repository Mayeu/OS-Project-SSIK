/**
 * \file kscheduler.c
 * \brief Scheduler functions to manage concurrent processes.
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#include "kscheduler.h"

void timer_exception()
{
	int i;
	for(i=0 ; i<MAX_PROC ; i++)
	{
		pcb *p = pwaiting.ls[i];
		if(!p_is_empty(p))
		{
			p->wait -= QUANTUM;
		}
		if(p->wait <= 0 & empty_space(&pready))
		{
			move(p->pid, &pwaiting, &pready);
		}
			break;	// No space free in the ready queue.
	}

	schedule();
}
