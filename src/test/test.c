/**
 * @file test.c
 * @brief Test a lot of stuff
 */

#include "../kernel/test.h"
#include "test_string.c"
#include "test_kprocess.c"
#include "test_uart_fifo.c"
//#include "test_kprocess_list.c"

	void
test()
{
	/*
	 * Call  your test here
	 */

	test_string();

	test_kprocess();
	//test_kprocess_list();
	test_uart_fifo();

}
