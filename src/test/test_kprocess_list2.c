
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/kprocess_list.h"
#include "../kernel/debug.h"


uint32_t        test_pls_reset();
uint32_t        test_pls_add();
uint32_t        test_pls_delete_pcb();
uint32_t        test_pls_move_pcb();
uint32_t        test_pls_item_reset();
uint32_t        test_pls_search_pcb();
uint32_t        test_pls_search_pid();
uint32_t        test_pls_item_alloc();
uint32_t        test_pls_item_cpy_pcb();

/*
 * List use for the test
 */
static pls      ls1;
static pls      ls2;
static uint32_t pid_loc = 0;
static pcb      p_array[MAXPCB];        // A set of pcb for the test

void
test_kprocess_list2()
{
  int             e;
  char            c;

  kprintln("--------TEST MODULE KPROCESS LIST V2 BEGIN--------");

  kprint("Test pls_reset\t\t\t\t\t");
  e = test_pls_reset();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pls_add\t\t\t\t\t");
  e = test_pls_add();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pls_search_pid\t\t\t\t");
  e = test_pls_search_pid();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pls_delete_pcb\t\t\t\t");
  e = test_pls_delete_pcb();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pls_move_pcb\t\t\t\t");
  e = test_pls_move_pcb();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprintln("---------TEST MODULE KPROCESS LIST V2 END---------\n");

}

/*
 * First we reset the two list
 */
uint32_t
test_pls_reset()
{

  pls_reset(&ls1);
  pls_reset(&ls2);

  if (ls1.start != NULL)
    return -1;

  if (ls1.length != 0)
    return -2;

  if (ls2.start != NULL)
    return -3;

  if (ls2.length != 0)
    return -4;

  return OMGROXX;
}

/*
 * We add some element in the list
 */
uint32_t
test_pls_add()
{
  //uint32_t        i;
  pcb            *p, *p2;       /* we use it to quickly access the array */

  p = &p_array[0];

  pcb_reset(p);                 /* default value */
  pcb_set_name(p, "pri21");
  pcb_set_pri(p, 21);
  pcb_set_pid(p, pid_loc++);
  pcb_set_empty(p, FALSE);


  if (pls_add(&ls1, p) != OMGROXX)
    return -1;

  if (strcmp(pcb_get_name(ls1.start), "pri21") != 0)
    return -2;

  if (pcb_get_head(ls1.start) != &ls1)
    return -3;

  if (ls1.length != 1)
    return -4;

  p = &p_array[1];

  pcb_reset(p);                 /* default value */
  pcb_set_name(p, "pri22");
  pcb_set_pri(p, 22);
  pcb_set_pid(p, pid_loc++);
  pcb_set_empty(p, FALSE);

  if (pls_add(&ls2, p) != OMGROXX)
    return -5;

  p = &p_array[2];

  pcb_reset(p);                 /* default value */
  pcb_set_name(p, "pri42");
  pcb_set_pri(p, 42);
  pcb_set_pid(p, pid_loc++);
  pcb_set_empty(p, FALSE);

  /*
   * We add a process that should be add to the front
   */
  if (pls_add(&ls2, p) != OMGROXX)
    return -6;

  if (strcmp(pcb_get_name(ls2.start), "pri42") != 0
      || pcb_get_pri(ls2.start) != 42)
    return -7;

  /*
   * We check that pri22 is after pri42
   */
  if (strcmp(pcb_get_name(pcb_get_next(ls2.start)), "pri22") != 0
      || pcb_get_pri(pcb_get_next(ls2.start)) != 22)
    return -8;


  /*
   * list 2 look like:
   * start->pri42->pri22
   * we add pri32
   */

  p = &p_array[3];

  pcb_reset(p);                 /* default value */
  pcb_set_name(p, "pri32");
  pcb_set_pri(p, 32);
  pcb_set_pid(p, pid_loc++);
  pcb_set_empty(p, FALSE);

  if (pls_add(&ls2, p) != OMGROXX)
    return -9;

  //kprintln("I should pass here");
  if (strcmp(pcb_get_name(ls2.start), "pri42") != 0
      || pcb_get_pri(ls2.start) != 42)
    return -10;

  /*
   * We check that pri32 is after pri42
   */

  p = pcb_get_next(ls2.start);

  if (strcmp(pcb_get_name(p), "pri32") != 0 || pcb_get_pri(p) != 32)
    return -11;

  /*
   * We check that pri22 is after pri32
   */

  p = pcb_get_next(p);

  if (strcmp(pcb_get_name(p), "pri22") != 0 || pcb_get_pri(p) != 22)
    return -12;

  /*
   * We add a new process with the 22 priority
   * It should be place after the first 22 priority process
   */

  p2 = &p_array[4];

  pcb_reset(p2);                /* default value */
  pcb_set_name(p2, "pri22-2");
  pcb_set_pri(p2, 22);
  pcb_set_pid(p2, pid_loc++);
  pcb_set_empty(p2, FALSE);

  if (pls_add(&ls2, p2) != OMGROXX)
    return -13;

  p = pcb_get_next(p);

  if (strcmp(pcb_get_name(p), "pri22-2") != 0 || pcb_get_pri(p) != 22)
    return -14;

  if (ls2.length != 4)
    return -15;

  return OMGROXX;
}

uint32_t
test_pls_search_pid()
{
  /*
   * Try to found an "impossible" pid
   */
  if (pls_search_pid(&ls1, 1000000) != NULL)
    return -1;

  /*
   * Try to found an existing pid
   */
  if (pls_search_pid(&ls2, 2) == NULL)
    return -2;

  /*
   * Try to found an existing pid in the wrong list
   */
  if (pls_search_pid(&ls1, 2) != NULL)
    return -3;

  return OMGROXX;
}

uint32_t
test_pls_delete_pcb()
{

  pcb            *p, *prev, *next;

  p = pls_search_pid(&ls2, 3);  //pri = 32
  prev = pcb_get_prev(p);
  next = pcb_get_next(p);

  /*
   * Try to remove 43 from the list
   */
  if (pls_delete_pcb(p) != OMGROXX)
    return -1;

  if (pcb_get_empty(p) != TRUE)
    return -2;

  if (pls_search_pid(&ls2, 3) != NULL)
    return -3;

  if (ls2.length != 3)
    return -4;

  if (pcb_get_next(prev) != next)
    return -5;

  if (pcb_get_prev(next) != prev)
    return -6;

  /*
   * We readd it
   */

  p = &p_array[3];

  pcb_reset(p);                 /* default value */
  pcb_set_name(p, "pri32");
  pcb_set_pri(p, 32);
  pcb_set_pid(p, pid_loc++);
  pcb_set_empty(p, FALSE);

  /*
   * Try to delete the head of the list
   */

  p = pls_search_pid(&ls2, pcb_get_pid(ls2.start));
  next = pcb_get_next(p);

  if (pls_delete_pcb(p) != OMGROXX)
    return -7;

  if (pcb_get_empty(p) != TRUE)
    return -8;

  if (ls2.start != next)
    return -9;

  if (pcb_get_prev(next) != NULL)
    return -10;

  return OMGROXX;
}

uint32_t
test_pls_move_pcb()
{

  pcb            *p;            //, *tmp;

  p = pls_search_pid(&ls1, 0);

  /*
   * Try to remove 0 from the list
   */
  if (pls_move_pcb(p, &ls2) != OMGROXX)
    return -1;

  if (pls_search_pid(&ls1, 0) != NULL)
    return -2;

  if (pls_search_pid(&ls2, 0) == NULL)
    return -3;

  return OMGROXX;
}
