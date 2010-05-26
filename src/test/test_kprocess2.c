
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../kernel/kprocess.h"
#include "../kernel/kernel.h"

uint32_t test_get_next_pid         ();
uint32_t test_reset_next_pid       ();
uint32_t test_create_proc          ();
uint32_t test_reset_used_stack();
uint32_t test_alloc_dealloc();
uint32_t test_create_proc();

void test_kprocess2()
{
	int e;
	char c;

	kprintln("----------TEST MODULE KPROCESS V2 BEGIN----------");

	kprint("Test next_pid\t\t\t\t\t");
	e = test_get_next_pid       ();
	if (e == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprint("Test reset_pid\t\t\t\t\t");
	e = test_reset_next_pid       ();
	if (e == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprint("Test reset_used_stack\t\t\t\t");
e = test_reset_used_stack();
	if (e == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprint("Test allocate/dealloc stack\t\t\t");
e = test_alloc_dealloc();
	if (e == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprint("Test create_proc\t\t\t\t");
e = test_create_proc();
	if (e == OMGROXX)
		kprintln("OK");
	else
	{
		kprint("FAIL: ");
		kprintln(itos(e,&c));
	}

	kprintln("-----------TEST MODULE KPROCESS V2 END------------");
}

/*
 * First we reset the two list
 */
uint32_t test_get_next_pid       ()
{
	pcb p;

	/*
	 * What the next pid ?
	 */
	if( get_next_pid() != 0)
		return -1;

	/*
	 * ok i did not use it, so give it again
	 */
	if( get_next_pid() != 0)
		return -2;

	/*
	 * now I use it
	 */
	pcb_reset(&p);
	pcb_set_pid(&p, get_next_pid());
	pcls_add(&pclsready, &p);

	/*
	 * Now gimme mooaaaaaare
	 */
	if( get_next_pid() != 1)
		return -3;

	/*
	 * Reset everything
	 */
	pcls_reset(&pclsready);
	pcb_reset(&p);

	/*
	 * The next_pid will be reset in the next test
	 */
	return OMGROXX;
}

uint32_t test_reset_next_pid()
{
	reset_next_pid();

	if( get_next_pid() != 0)
		return -1;

	reset_next_pid();

	return OMGROXX;
}

uint32_t 
test_reset_used_stack()
{
	int32_t *p;
	uint32_t i;

	reset_used_stack();

	p = get_used_stack();

	for(i = 0; i < MAXPCB; i++)
		if ( p[i] != -1)
			return i ;

	return OMGROXX;
}

uint32_t test_alloc_dealloc()
{
	int32_t *t;
	uint32_t *a;

	t = get_used_stack();

	a = allocate_stack(32);

	if(t[0] != 32)
		return -1;

	if(deallocate_stack(32) != OMGROXX)
		return -2;

	if(t[0] != -1)
		return -3;

	if(deallocate_stack(32) != NOTFOUND)
		return -4;

	return OMGROXX;
}

uint32_t test_create_proc()
{
	uint32_t i;
	//int32_t e;
	//char c;
	char arg[4][5] = {
		"arg1",
		"arg2",
		"arg3",
		"arg4"
	};

	if(create_proc(NULL, 78, NULL) != NULLPTR)
		return -1;

	if(create_proc("name", 78, NULL) != INVARG)
		return -2;

	if(create_proc("name", 12, NULL) != NULLPTR)
		return -3;

	if(create_proc("name", 12,(char **)arg) != INVARG)
		return -4;

	if(create_proc("init", 12,(char **)arg) < 0)
		return -5;

	for (i = 0; i < MAXPCB-1; i++)
		if( create_proc("init", 12,(char **)arg) < 0)
			return -6;

	if(create_proc("init", 12,(char **)arg) != OUTOMEM)
		return -7;

	return OMGROXX;
}
