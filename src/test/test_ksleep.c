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

  kprintln("--------------TEST MODULE KSLEEP END---------------\n");

  pls_reset(&plsready);
  pls_reset(&plswaiting);

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

  pcb            *p;
  pcb             parray[MAXPCB];

  pls_reset(&plsready);
  pls_reset(&plswaiting);
  /*
   * First populate the list
   */
  p = &parray[0];
  pcb_reset(p);
  pcb_set_name(p, "pri42");
  pcb_set_pri(p, 42);
  pcb_set_state(p, READY);
  pcb_set_empty(p, FALSE);
  pls_add(&plsready, p);

  p = &parray[1];
  pcb_reset(p);
  pcb_set_name(p, "pri2");
  pcb_set_pri(p, 2);
  pcb_set_state(p, READY);
  pcb_set_empty(p, FALSE);
  pls_add(&plsready, p);

  p = &parray[2];
  pcb_reset(p);
  pcb_set_name(p, "pri41");
  pcb_set_pri(p, 41);
  pcb_set_state(p, SLEEPING);
  pcb_set_sleep(p, QUANTUM - 1);
  pcb_set_empty(p, FALSE);
  pls_add(&plswaiting, p);

  p = &parray[3];
  pcb_reset(p);
  pcb_set_name(p, "pri32");
  pcb_set_pri(p, 32);
  pcb_set_state(p, SLEEPING);
  pcb_set_sleep(p, 2 * QUANTUM);
  pcb_set_empty(p, FALSE);
  pls_add(&plswaiting, p);

  p = &parray[4];
  pcb_reset(p);
  pcb_set_name(p, "pri21");
  pcb_set_pri(p, 21);
  pcb_set_state(p, BLOCKED);
  pcb_set_empty(p, FALSE);
  pls_add(&plswaiting, p);

  p = &parray[5];
  pcb_reset(p);
  pcb_set_name(p, "pri12");
  pcb_set_pri(p, 12);
  pcb_set_state(p, SLEEPING);
  pcb_set_sleep(p, 3 * QUANTUM);
  pcb_set_empty(p, FALSE);
  pls_add(&plswaiting, p);

  /*
   * Ok, fire in the hole!
   */
  process_sleep();

  /*
   * Now the long and painfull check
   */

  if (strcmp(pcb_get_name(plsready.start), "pri42") != 0)
    return -1;

  p = pcb_get_next(plsready.start);

  if (strcmp(pcb_get_name(p), "pri41") != 0
      || pcb_get_sleep(p) != 0 || pcb_get_state(p) != READY)
    return -2;

  p = pcb_get_next(p);

  if (strcmp(pcb_get_name(p), "pri2") != 0)
    return -3;

  if (strcmp(pcb_get_name(plswaiting.start), "pri32") != 0
      || pcb_get_sleep(plswaiting.start) != QUANTUM
      || pcb_get_state(plswaiting.start) != SLEEPING)
    return -4;

  p = pcb_get_next(plswaiting.start);

  if (strcmp(pcb_get_name(p), "pri21") != 0)
    return -5;

  p = pcb_get_next(p);

  if (strcmp(pcb_get_name(p), "pri12") != 0
      || pcb_get_sleep(p) != 2 * QUANTUM || pcb_get_state(p) != SLEEPING)
    return -6;

  return OMGROXX;

}
