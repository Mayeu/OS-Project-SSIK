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
#include "kinout.h"
#include "kscheduler.h"
#include "kprocess.h"


void
schedule()
{
  uint32_t        pri;
  pcls_item      *it;
  //pcb *p;
  //char c;

  //kprintln("Hmmm Schedule me!");

  /*
   * Real first
   * If running and waiting are empty, we dont have anything to do
   */
  if (pclsrunning.start == NULL && pclsready.start == NULL)
  {
    p_error = &kerror;
    set_current_pcb(NULL);
    return;
  }

  /*
   * First we take the priority of the element of running
   * In running everybody have the same priority
   */
  if (pclsrunning.start != NULL)
    pri = pcb_get_pri(&(pclsrunning.start->p));
  else
    pri = 0;

  /*
   * Is the first element of the ready list more prioritary ?
   * The list are ordered by default, so we just have to check
   * the first to know
   * If so we need to empty the running list
   */
  if (pclsready.start != NULL && pri < pcb_get_pri(&(pclsready.start->p)))
  {
    it = pclsrunning.start;

    while (it != NULL)
    {
      pcb_set_state(&(it->p), READY);
      pcls_move_pcb(&pclsrunning, &pclsready, &(it->p));
      it = pclsrunning.start;
    }

    pri = pcb_get_pri(&(pclsready.start->p));
  }

  /*
   * Now we can add the element of the highest priority in
   * running.
   * Again since the list are ordered, we just have to take
   * the first part of the element.
   */
  it = pclsready.start;

  while (it != NULL && pcb_get_pri(&(it->p)) == pri)
  {
    pcb_set_state(&(it->p), RUNNING);
    pcls_move_pcb(&pclsready, &pclsrunning, &(it->p));
    it = pclsready.start;
  }

  /*
   * Is the pclsrunning list empty ?
   */
  if (pclsrunning.length == 0)
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
    if (get_current_pcb() != NULL)
    {
      it = pcls_search_pcb(&pclsrunning, get_current_pcb());
      if (it->next != NULL)
      {
        /*
         * We set the current pcb to the next element of the list
         */
        set_current_pcb(&(it->next->p));
      }

      /*
       * hey ! p is null :/
       * DON'T PANIC ! Just take the first element in the running list ;)
       */
      else
      {
        set_current_pcb(&(pclsrunning.start->p));
      }
    }

    /*
     * Nop not in the list!
     * So just take the first of the runnig list :)
     */
    else
      set_current_pcb(&(pclsrunning.start->p));

    /*
     * Now we set the error pointer
     */
    p_error = &(get_current_pcb()->error);
  }

  /*
   * Ok we are done ! (maybe)
   */

  //kprintln("Deja finis?");
}
