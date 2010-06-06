#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/kernel.h"
#include "../kernel/kinout.h"
#include "../kernel/kprocess.h"
#include "test_kprocess2.h"

uint32_t        test_get_next_pid();
uint32_t        test_reset_next_pid();
uint32_t        test_create_proc();
uint32_t        test_reset_used_stack();
uint32_t        test_alloc_dealloc();
uint32_t        test_create_proc();

static pcb      pcb_array[MAXPCB];

void
test_kprocess2()
{
  int             e;
  char            c;

  pls_reset(&plsready);
  pls_reset(&plsrunning);
  pls_reset(&plswaiting);
  pls_reset(&plsterminate);

  kprintln("----------TEST MODULE KPROCESS V2 BEGIN----------");

  kprint("Test next_pid\t\t\t\t\t");
  e = test_get_next_pid();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test reset_pid\t\t\t\t\t");
  e = test_reset_next_pid();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test reset_used_stack\t\t\t\t");
  e = test_reset_used_stack();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test allocate/dealloc stack\t\t\t");
  e = test_alloc_dealloc();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test create_proc\t\t\t\t");
  e = test_create_proc();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprintln("-----------TEST MODULE KPROCESS V2 END------------\n");
}

/*
 * First we reset the two list
 */
uint32_t
test_get_next_pid()
{
  pcb            *p;

  /*
   * What the next pid ?
   */
  if (get_next_pid() != 0)
    return -1;

  /*
   * ok i did not use it, so give it again
   */
  if (get_next_pid() != 0)
    return -2;

  /*
   * now I use it
   */
  p = &pcb_array[0];
  pcb_reset(p);
  pcb_set_pid(p, get_next_pid());
  pls_add(&plsready, p);

  /*
   * Now gimme mooaaaaaare
   */
  if (get_next_pid() != 1)
    return -3;

  /*
   * Reset everything
   */
  pls_reset(&plsready);
  pcb_reset(p);

  /*
   * The next_pid will be reset in the next test
   */
  return OMGROXX;
}

uint32_t
test_reset_next_pid()
{
  reset_next_pid();

  if (get_next_pid() != 0)
    return -1;

  reset_next_pid();

  return OMGROXX;
}

uint32_t
test_reset_used_stack()
{
  int32_t        *stack;
  uint32_t        i;

  reset_used_stack();

  stack = get_used_stack();

  for (i = 0; i < MAXPCB; i++)
    if (stack[i] != -1)
      return -1 * i;

  return OMGROXX;
}

uint32_t
test_alloc_dealloc()
{
  int32_t        *stack;
  uint32_t       *a_stack;

  stack = get_used_stack();

  a_stack = allocate_stack(32);

  if (stack[0] != 32)
    return -1;

  if (deallocate_stack(32) != OMGROXX)
    return -2;

  if (stack[0] != -1)
    return -3;

  if (deallocate_stack(32) != NOTFOUND)
    return -4;

  return OMGROXX;
}


uint32_t
test_create_proc()
{
  uint32_t        i;
  //int32_t e;
  //char c;
  char            arg[4][5] = {
    "arg1",
    "arg2",
    "arg3",
    "arg4"
  };

  if (create_proc(NULL, 78, NULL) != NULLPTR)
    return -1;

  if (create_proc("name", 78, NULL) != INVARG)
    return -2;

  if (create_proc("name", 12, NULL) != NULLPTR)
    return -3;

  if (create_proc("name", 12, (char **) arg) != INVARG)
    return -4;

  if (create_proc("init", 12, (char **) arg) < 0)
    return -5;

  if (strcmp(pcb_get_name(plsready.start), "init") != 0)
    return -42;

  for (i = 0; i < MAXPCB - 1; i++)
    if (create_proc("init", 12, (char **) arg) < 0)
      return -6;

  if (create_proc("init", 12, (char **) arg) != OUTOMEM)
    return -7;

  return OMGROXX;
}
