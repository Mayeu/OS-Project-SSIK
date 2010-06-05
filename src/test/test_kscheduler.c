/**
 * @file uart_info_fifo.c
 * @brief test the fifo buffer of the uart module
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/debug.h"
#include "../kernel/kprocess.h"
#include "../kernel/kscheduler.h"

int32_t         test_schedule();

static pcb      parray[MAXPCB]; // A set of pcb for the test

void
test_kscheduler()
{
  int             e;
  char            c;

  kprintln("------------TEST MODULE SCHEDULER BEGIN-----------");


  kprint("Test the scheduler\t\t\t\t");
  e = test_schedule();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprintln("------------TEST MODULE SCHEDULER END-------------\n");
}

int32_t
test_schedule()
{
  /*
   * First a simple case:
   * A process with a priority of 42 is in the ready list.
   * And the running list is empty.
   */

  pcb            *p;

  pls_reset(&plsready);
  pls_reset(&plsrunning);

  /*
   * We add the pri41 to plsready
   */
  p = &parray[0];
  pcb_reset(p);
  pcb_set_name(p, "pri41");
  pcb_set_pri(p, 41);
  pcb_set_state(p, READY);
  pcb_set_empty(p, FALSE);
  pcb_set_epc(p, (uint32_t) init);
  pls_add(&plsready, p);

  /*
   * Go Go Scheduler!
   */
  schedule();

  if (strcmp(pcb_get_name(plsrunning.start), "pri41") != 0
      || pcb_get_state(plsrunning.start) != RUNNING)
    return -1;

  if (plsready.start != NULL)
    return -2;

  if (get_current_pcb() != plsrunning.start)
    return -3;

  /*
   * So now, nobody is in ready but running have on pcb
   * What if i reschedule ?
   */
  schedule();

  if (strcmp(pcb_get_name(plsrunning.start), "pri41") != 0
      || pcb_get_state(plsrunning.start) != RUNNING)
    return -4;

  if (plsready.start != NULL)
    return -5;

  if (get_current_pcb() != plsrunning.start)
    return -6;

  /*
   * Now we add pri12 inside the ready list
   */
  p = &parray[1];
  pcb_reset(p);
  pcb_set_name(p, "pri2");
  pcb_set_pri(p, 2);
  pcb_set_state(p, READY);
  pcb_set_empty(p, FALSE);
  pls_add(&plsready, p);

  schedule();

  if (strcmp(pcb_get_name(plsready.start), "pri2") != 0
      || pcb_get_state(plsready.start) != READY)
    return -6;

  if (strcmp(pcb_get_name(plsrunning.start), "pri41") != 0
      || pcb_get_state(plsrunning.start) != RUNNING)
    return -7;

  if (get_current_pcb() != plsrunning.start)
    return -8;

  /*
   * Now we add a new 41
   */

  p = &parray[2];
  pcb_reset(p);
  pcb_set_name(p, "pri412");
  pcb_set_pri(p, 41);
  pcb_set_state(p, READY);
  pcb_set_empty(p, FALSE);
  pls_add(&plsready, p);

  schedule();

  if (strcmp(pcb_get_name(plsready.start), "pri2") != 0
      || pcb_get_state(plsready.start) != READY)
    return -9;

  if (strcmp(pcb_get_name(pcb_get_next(plsrunning.start)), "pri412") != 0
      || pcb_get_state(pcb_get_next(plsrunning.start)) != RUNNING)
    return -10;

  if (strcmp(pcb_get_name(plsrunning.start), "pri41") != 0
      || pcb_get_state(plsrunning.start) != RUNNING)
    return -11;

  if (get_current_pcb() != pcb_get_next(plsrunning.start))
    return -12;

  /*
   * We add 42 to the ready list
   */

  p = &parray[3];
  pcb_reset(p);
  pcb_set_name(p, "pri42");
  pcb_set_pri(p, 42);
  pcb_set_state(p, READY);
  pcb_set_empty(p, FALSE);
  pls_add(&plsready, p);

  schedule();

  if (strcmp(pcb_get_name(plsready.start), "pri41") != 0
      || pcb_get_state(plsready.start) != READY)
    return -13;

  p = pcb_get_next(plsready.start);
  if (strcmp(pcb_get_name(p), "pri412") != 0 || pcb_get_state(p) != READY)
    return -14;

  p = pcb_get_next(p);
  if (strcmp(pcb_get_name(p), "pri2") != 0 || pcb_get_state(p) != READY)
    return -15;

  if (strcmp(pcb_get_name(plsrunning.start), "pri42") != 0
      || pcb_get_state(plsrunning.start) != RUNNING)
    return -16;

  pls_reset(&plsready);
  pls_reset(&plsrunning);

  schedule();

  if (get_current_pcb() != NULL)
    return -17;

  return OMGROXX;
}
