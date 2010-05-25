/**
 * @file test.c
 * @brief Test a lot of stuff
 */

#include "../kernel/test.h"
#include "test_string.c"
#include "test_kprocess.c"
#include "test_kpcb.c"
#include "test_uart_fifo.c"
#include "test_kprogram.c"
#include "test_kprocess_list.c"
#include "test_kprocess_list2.c"

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

	//test_kprocess();

	//test_kprocess_list();
}
