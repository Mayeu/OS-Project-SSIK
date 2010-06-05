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

void
test_kscheduler()
{
  int             e;
  char            c;

  kprintln("------------TEST MODULE SCHEDULER BEGIN-----------");

  kdebug_println("Test in");

  kprint("Test the scheduler\t\t\t\t");
  e = test_schedule();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kdebug_println("Test out");

  kprintln("------------TEST MODULE SCHEDULER END-------------");
}

int32_t
test_schedule()
{
  /*
   * First a simple case:
   * A process with a priority of 42 is in the ready list.
   * And the running list is empty.
   */

  pcb             p;

  pls_reset(&plsready);
  pls_reset(&plsrunning);

  /*
   * We add the pri42 to plsready
   */
  pcb_reset(&p);
  pcb_set_name(&p, "pri41");
  pcb_set_pri(&p, 41);
  pcb_set_state(&p, READY);
  pcb_set_empty(&p, FALSE);
  p.registers.epc_reg = (uint32_t) init;
  pls_add(&plsready, &p);

  /*
   * Go Go Scheduler!
   */
  schedule();

  if (strcmp(pcb_get_name(&(plsrunning.start->p)), "pri41") != 0
      || pcb_get_state(&(plsrunning.start->p)) != RUNNING)
    return -1;

  if (plsready.start != NULL)
    return -2;

  if (get_current_pcb() != &(plsrunning.start->p))
    return -3;

  /*
   * So now, nobody is in ready but running have on pcb
   * What if i reschedule ?
   */
  schedule();

  if (strcmp(pcb_get_name(&(plsrunning.start->p)), "pri41") != 0
      || pcb_get_state(&(plsrunning.start->p)) != RUNNING)
    return -142;

  if (plsready.start != NULL)
    return -242;

  if (get_current_pcb() != &(plsrunning.start->p))
    return -342;

  kdebug_assert_at(get_current_pcb()->registers.epc_reg == (uint32_t) init,
                   "test_scheduler.c", 53);

  /*
   * Now we add pri12 inside the ready list
   */
  pcb_reset(&p);
  pcb_set_name(&p, "pri2");
  pcb_set_pri(&p, 2);
  pcb_set_state(&p, READY);
  pcb_set_empty(&p, FALSE);
  pls_add(&plsready, &p);

  schedule();

  if (strcmp(pcb_get_name(&(plsready.start->p)), "pri2") != 0
      || pcb_get_state(&(plsready.start->p)) != READY)
    return -4;

  if (strcmp(pcb_get_name(&(plsrunning.start->p)), "pri41") != 0
      || pcb_get_state(&(plsrunning.start->p)) != RUNNING)
    return -5;

  if (get_current_pcb() != &(plsrunning.start->p))
    return -6;

  /*
   * Now we add a new 41
   */

  pcb_reset(&p);
  pcb_set_name(&p, "pri412");
  pcb_set_pri(&p, 41);
  pcb_set_state(&p, READY);
  pcb_set_empty(&p, FALSE);
  pls_add(&plsready, &p);

  schedule();

  if (strcmp(pcb_get_name(&(plsready.start->p)), "pri2") != 0
      || pcb_get_state(&(plsready.start->p)) != READY)
    return -7;

  if (strcmp(pcb_get_name(&(plsrunning.start->p)), "pri412") != 0
      || pcb_get_state(&(plsrunning.start->p)) != RUNNING)
    return -8;

  if (strcmp(pcb_get_name(&(plsrunning.start->next->p)), "pri41") != 0
      || pcb_get_state(&(plsrunning.start->next->p)) != RUNNING)
    return -9;

  if (get_current_pcb() != &(plsrunning.start->next->p))
    return -10;


  /*
   * We add 42 to the ready list
   */

  pcb_reset(&p);
  pcb_set_name(&p, "pri42");
  pcb_set_pri(&p, 42);
  pcb_set_state(&p, READY);
  pcb_set_empty(&p, FALSE);
  pls_add(&plsready, &p);

  schedule();

  if (strcmp(pcb_get_name(&(plsready.start->p)), "pri41") != 0
      || pcb_get_state(&(plsready.start->p)) != READY)
    return -11;

  if (strcmp(pcb_get_name(&(plsready.start->next->p)), "pri412") != 0
      || pcb_get_state(&(plsready.start->next->p)) != READY)
    return -12;

  if (strcmp(pcb_get_name(&(plsready.start->next->next->p)), "pri2") != 0
      || pcb_get_state(&(plsready.start->next->next->p)) != READY)
    return -13;

  if (strcmp(pcb_get_name(&(plsrunning.start->p)), "pri42") != 0
      || pcb_get_state(&(plsrunning.start->p)) != RUNNING)
    return -14;

  pls_reset(&plsready);
  pls_reset(&plsrunning);

  schedule();

  if (get_current_pcb() != NULL)
    return -15;

  return OMGROXX;
}
