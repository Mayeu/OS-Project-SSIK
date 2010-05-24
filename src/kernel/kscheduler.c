/**
 * \file kscheduler.c
 * \brief Scheduler functions to manage concurrent processes.
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#include "kernel.h"
#include "kscheduler.h"
#include "kprocess_list.h"

void
timer_exception()
{
  int             i;
  pcb            *p;
  for (i = 0; i < MAXPCB; i++)
  {
    p = &pwaiting.ls[i];
    if (!p_is_empty(p))
    {
      p->sleep -= QUANTUM;
    }
    if (empty_space(&pready))
    {
      if (p->sleep <= 0)
        move(p->pid, &pwaiting, &pready);
    }
    else
    {
      break;                    // No space free in the ready queue.
    }
  }

  schedule();
}

void
schedule()
{

}
