/**
 * @file ksleep.c
 * @brief Contains functions to manage sleeping process during time exception
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "kprocess.h"
#include "debug.h"
#include "kernel.h"
#include "kinout.h"
#include "kscheduler.h"
#include "ksleep.h"

/**
 * @brief Decrement sleeping time of the process in plswaiting.
 *
 * If a process as to be waking up, he is moved to the plsready list.
 */
void
process_sleep()
{
  pcb            *p, *last;

  /*
   * First : maybe we don't have anything to do ?
   */
  if (plswaiting.length == 0)
    return;

  /*
   * Ok the list contains stuff
   */
  last = NULL;
  p = plswaiting.start;

  //kprintln("Time to remove time");

  while (p != NULL)
  {
    /*
     * Is the pcb sleeping ?
     */
    if (pcb_get_state(p) == SLEEPING)
    {
      /*
       * Can we remove a QUANTUM from is sleeping time ?
       */
      if (pcb_get_sleep(p) >= QUANTUM)
        pcb_set_sleep(p, (pcb_get_sleep(p) - QUANTUM));

      else
      {
        /*
         * Oh did I wake you up ?
         */
        pcb_set_sleep(p, 0);
        pcb_set_state(p, READY);
        pls_move_pcb(p, &plsready);

        /*
         * set it to the "real" next one but don't update last !
         */
        if (last == NULL)
          p = plswaiting.start;
        else
          p = last;

        continue;
      }
    }

    /*
     * These are not the droids you are looking for
     */
    last = p;
    p = pcb_get_next(last);
  }
}
