/**
 * \file increment.c
 * \brief blabla
 * \author Christophe Carasco
 * \version 0.1
 * \date 26 mai 2010
 */

#include <stdio.h>
#include <process.h>
#include <string.h>

#include "increment.h"

int increment(int argc, char *argv[])
{
  int i = 1;
	int n = stoi(get_arg(argv, 1));
  for (; i <= n; i++)
    printiln(i);

	exit(0);
	return 0; 
}
