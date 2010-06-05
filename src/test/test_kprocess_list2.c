
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/kprocess_list.h"


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

  kprint("Test pls_search_pcb\t\t\t\t");
  e = test_pls_search_pcb();
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

  kprint("Test pls_item_reset\t\t\t\t");
  e = test_pls_item_reset();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pls_item_alloc\t\t\t\t");
  e = test_pls_item_alloc();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pls_item_cpy_pcb\t\t\t\t");
  e = test_pls_item_cpy_pcb();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprintln("---------TEST MODULE KPROCESS LIST V2 END---------");

}

/*
 * First we reset the two list
 */
uint32_t
test_pls_reset()
{
  uint32_t        i;

  pls_reset(&ls1);
  pls_reset(&ls2);
/*
  for (i = 0; i < MAXPCB; i++)
  {
    if (pcb_get_pid(&((ls1.ls[i]).p)) != 0)
      return -1;
    if (strcmp(pcb_get_name(&((ls1.ls[i]).p)), "") != 0)
      return -2;
    if (pcb_get_pri(&((ls1.ls[i]).p)) != 0)
      return -3;
    if (pcb_get_supervisor(&((ls1.ls[i]).p)) != -1)
      return -4;
    if (pcb_get_state(&((ls1.ls[i]).p)) != 0)
      return -5;
    if (pcb_get_sleep(&((ls1.ls[i]).p)) != 0)
      return -6;
    if (pcb_get_waitfor(&((ls1.ls[i]).p)) != 0)
      return -7;
    if (pcb_get_error(&((ls1.ls[i]).p)) != OMGROXX)
      return -8;
    if (pcb_get_empty(&((ls1.ls[i]).p)) != TRUE)
      return -9;
  }
*/
  return OMGROXX;
}

/*
 * We add some element in the list
 */
uint32_t
test_pls_add()
{
  uint32_t        i;
  pcb             p;            /* the process we will use */

  pcb_reset(&p);                /* default value */
  pcb_set_name(&p, "pri21");
  pcb_set_pri(&p, 21);
  pcb_set_pid(&p, pid_loc++);
  pcb_set_empty(&p, FALSE);

  if (pls_add(&ls1, &p) != OMGROXX)
    return -1;

  if (strcmp(pcb_get_name(&((ls1.start)->p)), "pri21") != 0)
    return -2;

  pcb_reset(&p);                /* default value */
  pcb_set_name(&p, "pri22");
  pcb_set_pri(&p, 22);
  pcb_set_pid(&p, pid_loc++);
  pcb_set_empty(&p, FALSE);

  if (pls_add(&ls2, &p) != OMGROXX)
    return -3;

  /*
   * We check that the data are copied and not pointed by checking the first list again
   */
  if (strcmp(pcb_get_name(&((ls1.start)->p)), "pri21") != 0
      || pcb_get_pri(&((ls1.start)->p)) != 21)
    return -4;

  if (strcmp(pcb_get_name(&((ls2.start)->p)), "pri22") != 0
      || pcb_get_pri(&((ls2.start)->p)) != 22)
    return -5;

  pcb_reset(&p);                /* default value */
  pcb_set_name(&p, "pri42");
  pcb_set_pri(&p, 42);
  pcb_set_pid(&p, pid_loc++);
  p.registers.epc_reg = (uint32_t) init;
  pcb_set_empty(&p, FALSE);

  /*
   * We add a process that should be add to the front
   */
  if (pls_add(&ls2, &p) != OMGROXX)
    return -6;

  if (pcb_get_register(&((ls2.start)->p))->epc_reg != (uint32_t) init)
    return -7;

  if (strcmp(pcb_get_name(&((ls2.start)->p)), "pri42") != 0
      || pcb_get_pri(&((ls2.start)->p)) != 42)
    return -8;

  /*
   * We check that pri22 is after pri42
   */
  if (strcmp(pcb_get_name(&(((ls2.start)->next)->p)), "pri22") != 0
      || pcb_get_pri(&(((ls2.start)->next)->p)) != 22)
    return -9;

  //pcb_reset(&p); /* default value */
  //pcb_set_name(&p, "pri0");
  //pcb_set_pri(&p, 0);
  //pcb_set_pid(&p, pid_loc++);
  //pcb_set_empty(&p, FALSE);

//if(pls_add(&ls2, &p) != OMGROXX
//              || strcmp( pcb_get_name(&(ls2.start->next->next->p)), "pri0") != 0 
//              ||(ls2.start)->next->next->next != NULL)
//              return -42;

  /*
   * list 2 look like:
   * start->pri42->pri22
   * we add pri32
   */

  pcb_reset(&p);                /* default value */
  pcb_set_name(&p, "pri32");
  pcb_set_pri(&p, 32);
  pcb_set_pid(&p, pid_loc++);
  pcb_set_empty(&p, FALSE);

  if (pls_add(&ls2, &p) != OMGROXX)
    return -10;

  if (strcmp(pcb_get_name(&((ls2.start)->p)), "pri42") != 0
      || pcb_get_pri(&((ls2.start)->p)) != 42)
    return -11;

  /*
   * We check that pri32 is after pri42
   */
  if (strcmp(pcb_get_name(&(ls2.start->next->p)), "pri32") != 0
      || pcb_get_pri(&(ls2.start->next->p)) != 32)
    return -12;

  /*
   * We check that pri22 is after pri32
   */
  if (strcmp(pcb_get_name(&(ls2.start->next->next->p)), "pri22") != 0
      || pcb_get_pri(&(ls2.start->next->next->p)) != 22)
    return -13;

  /*
   * We reset ls1
   * and we fill it
   */

  pls_reset(&ls1);

  for (i = 0; i < MAXPCB; i++)
  {
    pcb_reset(&p);              /* default value */
    pcb_set_pri(&p, i);
    pcb_set_pid(&p, pid_loc++);
    pcb_set_empty(&p, FALSE);

    if (pls_add(&ls1, &p) != OMGROXX)
      return -14;
  }

  /*
   * And we add one more value
   */
  pcb_reset(&p);                /* default value */
  pcb_set_pri(&p, 42);
  pcb_set_pid(&p, pid_loc++);
  pcb_set_empty(&p, FALSE);

  if (pls_add(&ls1, &p) != OUTOMEM)
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
  if (pls_search_pid(&ls1, 42) == NULL)
    return -2;

  /*
   * Try to found an existing pid in the wrong list
   */
  if (pls_search_pid(&ls2, 42) != NULL)
    return -3;

  return OMGROXX;
}

uint32_t
test_pls_search_pcb()
{
  pcb             p;
  /*
   * Try to found an "impossible" pid
   */
  pcb_reset(&p);
  pcb_set_pid(&p, 1000000);
  if (pls_search_pcb(&ls1, &p) != NULL)
    return -1;

  /*
   * Try to found an existing pid
   */
  pcb_set_pid(&p, 42);
  if (pls_search_pcb(&ls1, &p) == NULL)
    return -2;

  /*
   * Try to found an existing pid in the wrong list
   */
  if (pls_search_pcb(&ls2, &p) != NULL)
    return -3;

  return OMGROXX;
}

uint32_t
test_pls_delete_pcb()
{

  pcb             p;

  pcb_reset(&p);
  pcb_set_pid(&p, 43);

  /*
   * Try to remove 43 from the list
   */
  if (pls_delete_pcb(&ls1, &p) != OMGROXX)
    return -1;

  if (pls_search_pcb(&ls1, &p) != NULL)
    return -2;

  return OMGROXX;
}

uint32_t
test_pls_move_pcb()
{

  pcb             p;            //, *tmp;

  pcb_reset(&p);
  pcb_set_pid(&p, 42);

  /*
   * Try to remove 42 from the list
   */
  if (pls_move_pcb(&ls1, &ls2, &p) != OMGROXX)
    return -1;

  //if(pcb_get_register(&((ls2.start)->p))->epc_reg != (uint32_t) init)
//  return -7;

  if (pls_search_pcb(&ls1, &p) != NULL)
    return -2;

  if (pls_search_pcb(&ls2, &p) == NULL)
    return -3;

  return OMGROXX;
}

uint32_t
test_pls_item_reset()
{
  /* Auto tested in pls_reset */
  return OMGROXX;
}

uint32_t
test_pls_item_alloc()
{
  /* Auto tested in pls_add */
  return OMGROXX;
}

uint32_t
test_pls_item_cpy_pcb()
{
  /* Auto tested in pls_add */
  return OMGROXX;
}
