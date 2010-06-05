/**
 * @file uart_info_fifo.c
 * @brief test the fifo buffer of the uart module
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/kprocess.h"
#include "../kernel/ksleep.h"

int32_t         test_process_sleep();

void
test_ksleep()
{
  int             e;
  char            c;

  kprintln("--------------TEST MODULE SLEEP BEGIN-------------");

  kprint("Test process_sleep\t\t\t\t");
  e = test_process_sleep();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprintln("--------------TEST MODULE KSLEEP END---------------");
}

int32_t
test_process_sleep()
{
  /*
   * We set the following list :
   * plsready :
   * Name       Pri       State     time to sleep
   * Pri42      42        READY     0
   * Pri2       2         READY     0
   *
   * plswaiting:
   * PRI41      41        SLEEP     QUANTUM - 1
   * PRI32      32        SLEEP     2*QUANTUM
   * PRI21      21        BLOCK     0
   * PRI12      12        SLEEP     3*QUANTUM
   *
   * After the test the list will look like this:
   * plsready :
   * Name       Pri       State     time to sleep
   * Pri42      42        READY     0
   * PRI41      41        READY     0
   * Pri2       2         READY     0
   *
   * plswaiting:
   * PRI32      32        SLEEP     QUANTUM
   * PRI21      21        BLOCK     0
   * PRI12      12        SLEEP     2*QUANTUM
   */

  pcb             p;

  pls_reset(&plsready);
  pls_reset(&plswaiting);
  /*
   * First populate the list
   */
  pcb_reset(&p);
  pcb_set_name(&p, "pri42");
  pcb_set_pri(&p, 42);
  pcb_set_state(&p, READY);
  pcb_set_empty(&p, FALSE);
  pls_add(&plsready, &p);

  pcb_reset(&p);
  pcb_set_name(&p, "pri2");
  pcb_set_pri(&p, 2);
  pcb_set_state(&p, READY);
  pcb_set_empty(&p, FALSE);
  pls_add(&plsready, &p);

  pcb_reset(&p);
  pcb_set_name(&p, "pri41");
  pcb_set_pri(&p, 41);
  pcb_set_state(&p, SLEEPING);
  pcb_set_sleep(&p, QUANTUM - 1);
  pcb_set_empty(&p, FALSE);
  pls_add(&plswaiting, &p);

  pcb_reset(&p);
  pcb_set_name(&p, "pri32");
  pcb_set_pri(&p, 32);
  pcb_set_state(&p, SLEEPING);
  pcb_set_sleep(&p, 2 * QUANTUM);
  pcb_set_empty(&p, FALSE);
  pls_add(&plswaiting, &p);

  pcb_reset(&p);
  pcb_set_name(&p, "pri21");
  pcb_set_pri(&p, 21);
  pcb_set_state(&p, BLOCKED);
  pcb_set_empty(&p, FALSE);
  pls_add(&plswaiting, &p);

  pcb_reset(&p);
  pcb_set_name(&p, "pri12");
  pcb_set_pri(&p, 12);
  pcb_set_state(&p, SLEEPING);
  pcb_set_sleep(&p, 3 * QUANTUM);
  pcb_set_empty(&p, FALSE);
  pls_add(&plswaiting, &p);

  /*
   * Ok, fire in the hole!
   */
  process_sleep();

  /*
   * Now the long and painfull check
   */

  if (strcmp(pcb_get_name(&(plsready.start->p)), "pri42") != 0)
    return -1;

  if (strcmp(pcb_get_name(&(plsready.start->next->p)), "pri41") != 0
      || pcb_get_sleep(&(plsready.start->next->p)) != 0
      || pcb_get_state(&(plsready.start->next->p)) != READY)
    return -2;

  if (strcmp(pcb_get_name(&(plsready.start->next->next->p)), "pri2") != 0)
    return -3;

  if (strcmp(pcb_get_name(&(plswaiting.start->p)), "pri32") != 0
      || pcb_get_sleep(&(plswaiting.start->p)) != QUANTUM
      || pcb_get_state(&(plswaiting.start->p)) != SLEEPING)
    return -4;

  if (strcmp(pcb_get_name(&(plswaiting.start->next->p)), "pri21") != 0)
    return -5;

  if (strcmp(pcb_get_name(&(plswaiting.start->next->next->p)), "pri12") != 0
      || pcb_get_sleep(&(plswaiting.start->next->next->p)) != 2 * QUANTUM
      || pcb_get_state(&(plswaiting.start->next->next->p)) != SLEEPING)
    return -6;


  return OMGROXX;

}
