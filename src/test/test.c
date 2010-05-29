/**
 * @file test.c
 * @brief Test a lot of stuff
 */

#include "../kernel/test.h"
#include "test_string.c"
//#include "test_kprocess.c"
#include "test_kprocess2.c"
#include "test_kpcb.c"
#include "test_uart_fifo.c"
#include "test_kprogram.c"
//#include "test_kprocess_list.c"
#include "test_kprocess_list2.c"
#include "test_ksleep.c"
#include "test_kscheduler.c"
#include "test_coquille.c"
//#include "test_kmsg.c"
//#include "test_kmsg_lst.c"


/* 
 * Usage:
 * res = function to test;
 * err = (res == OMGROXX) && (other tests);
 * test_unit(err, res);
 * Print the result as OK/NOK and if it is NOK
 * print the return value of the tested function.
 */
void
test_unit(bool err, int res)
{
  char            resc[2];
  if (err)
    kprintln("OK");
  else
  {
    kprint("NOK:");
    kprintln(itos(res, resc));
  }
}

void
test()
{
  /*
   * Call  your test here
   */

  test_string();

  test_kpcb();

  test_kprocess_list2();

  test_uart_fifo();

  test_kprogram();

  test_kprocess2();

  test_ksleep();

  test_kscheduler();

  //test_kprocess();

  //test_kprocess_list();

  //test_kmsg();

  //test_kmsg_lst();

  //test_coquille();
}
