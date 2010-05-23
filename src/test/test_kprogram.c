/**
 * @file uart_info_fifo.c
 * @brief test the fifo buffer of the uart module
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/kprogram.h"

uint32_t test_search_prgm(void);

void test_kprogram()
{
	int e;
	char c;

	kprintln("------------TEST MODULE KPROGRAM BEGIN------------");

	kprint("Test search_prgm\t\t\t\t");
	e = test_search_prgm() ;

	if (e == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprintln("-------------TEST MODULE KPROGRAM END-------------");
}

uint32_t test_search_prgm(void)
{
	prgm *tmp;

	tmp = search_prgm("init");

	if(tmp == NULL)
		return -1;

	if(strcmp(tmp->name, "init") != 0)
		return -2;

	if(search_prgm("flipitou") != NULL)
		return -3;

			return OMGROXX ;
}
