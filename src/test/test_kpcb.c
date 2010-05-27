
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/kpcb.h"

uint32_t        test_pcb_set_get_pid();
uint32_t        test_pcb_set_get_name();
uint32_t        test_pcb_set_get_pri();
uint32_t        test_pcb_set_get_supervised();
uint32_t        test_pcb_set_get_supervisor();
uint32_t        test_pcb_set_get_register();
uint32_t        test_pcb_set_get_state();
uint32_t        test_pcb_set_get_sleep();
uint32_t        test_pcb_set_get_waitfor();
uint32_t        test_pcb_set_get_error();
uint32_t        test_pcb_set_get_empty();

void
test_kpcb()
{
  int             e;
  char            c;

  kprintln("--------------TEST MODULE KPCB BEGIN--------------");

  kprint("Test pcb_get_pid\t\t\t\t");
  e = test_pcb_set_get_pid();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pcb_get_name\t\t\t\t");
  e = test_pcb_set_get_name();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pcb_get_pri\t\t\t\t");
  e = test_pcb_set_get_pri();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }


  kprint("Test pcb_get_supervised\t\t\t\t");
  e = test_pcb_set_get_supervised();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pcb_get_supervisor\t\t\t\t");
  e = test_pcb_set_get_supervisor();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }
  /*
     kprint("Test pcb_get_register\t\t\t\t");
     e = test_pcb_set_get_register();
     if (e == OMGROXX)
     kprintln("OK");
     else
     {
     kprint("FAIL: ");
     kprintln(itos(e,&c));
     }
   */
  kprint("Test pcb_get_state\t\t\t\t");
  e = test_pcb_set_get_state();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pcb_get_sleep\t\t\t\t");
  e = test_pcb_set_get_sleep();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pcb_get_waitfor\t\t\t\t");
  e = test_pcb_set_get_waitfor();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pcb_get_error\t\t\t\t");
  e = test_pcb_set_get_error();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }

  kprint("Test pcb_get_empty\t\t\t\t");
  e = test_pcb_set_get_empty();
  if (e == OMGROXX)
    kprintln("OK");
  else
  {
    kprint("FAIL: ");
    kprintln(itos(e, &c));
  }
  kprintln("---------------TEST MODULE KPCB END---------------");

}

/*
 * For all the get test we use the fact that the compiler set everything
 * to zero
 */
uint32_t
test_pcb_set_get_pid()
{
  pcb             p;

  pcb_set_pid(&p, 2);

  if (pcb_get_pid(&p) != 2)
    return -1;

  return OMGROXX;
}

uint32_t
test_pcb_set_get_name()
{
  pcb             p;
  char            str1[] = "Yop Yop Yop Yop Yop Yop ";
  char            str2[] = "Yop Yop Yop Yop Yop ";

  pcb_set_name(&p, str1);

  if (strcmp(pcb_get_name(&p), "Yop Yop Yop Yop Yop Yop") != 0)
    return -1;

  pcb_set_name(&p, str2);

  if (strcmp(pcb_get_name(&p), "Yop Yop Yop Yop Yop Yop") != 0)
    return -2;

  return OMGROXX;
}

uint32_t
test_pcb_set_get_pri()
{
  pcb             p;

  pcb_set_pri(&p, 10);

  if (pcb_get_pri(&p) != 10)
    return -1;

  return OMGROXX;
}


uint32_t
test_pcb_set_get_supervised()
{
  pcb             p;
  int32_t        *s, i;

  pcb_reset_supervised(&p);
  s = pcb_get_supervised(&p);

  for (i = 0; i < MAXPCB; i++)
    if (s[i] != -1)
      return -1;

  pcb_set_supervised(&p, 0);

  s = pcb_get_supervised(&p);
  if (s[0] != 0)
    return -2;

  for (i = 0; i < MAXPCB; i++)
    pcb_set_supervised(&p, i);

  s = pcb_get_supervised(&p);

  for (i = 0; i < MAXPCB; i++)
    if (s[i] != i)
      return -3;

  if (pcb_set_supervised(&p, 123) != OUTOMEM)
    return -4;

  pcb_rm_supervised(&p, 32);
  s = pcb_get_supervised(&p);

  if (s[32] != -1)
    return -5;

  if (pcb_set_supervised(&p, 123) != OMGROXX)
    return -6;

  return OMGROXX;
}

uint32_t
test_pcb_set_get_supervisor()
{
  pcb             p;
  pcb_reset(&p);

  if (pcb_get_supervisor(&p) != -1)
    return -1;

  return OMGROXX;
}

/*
	uint32_t    test_pcb_set_get_register()
	{
	pcb p;

	if (pcb_get_pid(&p) != 0)
	return -1;

	return OMGROXX;
	}
	*/

uint32_t
test_pcb_set_get_state()
{
  pcb             p;

  pcb_set_state(&p, OMG_ZOMBIE);

  if (pcb_get_state(&p) != OMG_ZOMBIE)
    return -1;

  return OMGROXX;
}

uint32_t
test_pcb_set_get_sleep()
{
  pcb             p;

  pcb_set_sleep(&p, 1000);

  if (pcb_get_sleep(&p) != 1000)
    return -1;

  return OMGROXX;
}

uint32_t
test_pcb_set_get_waitfor()
{
  pcb             p;

  pcb_set_waitfor(&p, 42);

  if (pcb_get_waitfor(&p) != 42)
    return -1;

  return OMGROXX;
}

uint32_t
test_pcb_set_get_error()
{
  pcb             p;

  pcb_set_error(&p, FAILNOOB);

  if (pcb_get_error(&p) != FAILNOOB)
    return -1;

  return OMGROXX;
}

uint32_t
test_pcb_set_get_empty()
{
  pcb             p;

  pcb_set_empty(&p, TRUE);

  if (pcb_get_empty(&p) != TRUE)
    return -1;

  return OMGROXX;
}
