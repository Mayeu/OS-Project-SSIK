/**
 * \file kscheduler.c
 * \brief Scheduler functions to manage concurrent processes.
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#include <string.h>
#include "kernel.h"
#include "debug.h"
#include "kinout.h"
#include "kscheduler.h"
#include "kprocess.h"


void
schedule()
{
  uint32_t        pri;
  pcb            *p;
  //char c;

  //kdebug_println("Scheduler in");

  /*
   * Real first
   * If running and waiting are empty, we dont have anything to do
   */
  if (plsrunning.start == NULL && plsready.start == NULL)
  {
    p_error = &kerror;
    set_current_pcb(NULL);
    //kprintln("Scheduler: Nothing to do");
    return;
  }

  /*
   * First we take the priority of the element of running
   * In running everybody have the same priority
   */
  if (plsrunning.start != NULL)
  {
    pri = pcb_get_pri(plsrunning.start);
  }
  else
  {
    pri = 0;
  }

  /*
   * Is the first element of the ready list more prioritary ?
   * The list are ordered by default, so we just have to check
   * the first to know
   * If so we need to empty the running list
   */
  if (plsready.start != NULL && pri < pcb_get_pri(plsready.start))
  {
    p = plsrunning.start;

    while (p != NULL)
    {
      pcb_set_state(p, READY);
      pls_move_pcb(p, &plsready);
      p = plsrunning.start;
    }

    pri = pcb_get_pri(plsready.start);
  }

  /*
   * Now we can add the element of the highest priority in
   * running.
   * Again since the list are ordered, we just have to take
   * the first part of the element.
   */
  p = plsready.start;
  kdebug_assert_at(pcb_get_next(p) == NULL, "schedule()", 84);

  while (p != NULL && pcb_get_pri(p) == pri)
  {
    pcb_set_state(p, RUNNING);
    pls_move_pcb(p, &plsrunning);
    p = plsready.start;
  }

  /*
   * Is the plsrunning list empty ?
   */
  if (plsrunning.start == NULL)
  {
    /*
     * Set the current pcb to null
     */
    set_current_pcb(NULL);

    /*
     * Set the error pointer to the kernel error
     */
    p_error = &kerror;
  }

  /*
   * Nop not at all !
   */
  else
  {
    /*
     * The current pcb is in the list of running process ?
     */
    if (get_current_pcb() != NULL
        && pcb_get_head(get_current_pcb()) == &plsrunning)
    {

      p = get_current_pcb();

      if (pcb_get_next(p) != NULL)
        /*
         * We set the current pcb to the next element of the list
         */
        set_current_pcb(pcb_get_next(p));

      /*
       * hey ! p is null :/
       * DON'T PANIC ! Just take the first element in the running list ;)
       */
      else
        set_current_pcb(plsrunning.start);
    }

    /*
     * Nop not in the list!
     * So just take the first of the runnig list :)
     */
    else
      set_current_pcb(plsrunning.start);

    /*
     * Now we set the error pointer
     */
    p_error = &(get_current_pcb()->error);
  }

  /*
   * Ok we are done ! (maybe)
   */

  // kdebug_println("Scheduler out");
}
