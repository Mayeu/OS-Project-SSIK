/**
 * \file fibonacci.c
 * \brief blabla
 * \author Christophe Carasco
 * \version 0.1
 * \date 26 mai 2010
 */

#include <stdio.h>
#include <process.h>
#include <string.h>
#include <errno.h>

#include "fibonacci.h"

void
fibonacci(int argc, char *argv[])
{
  int             a = 0, b = 1, c, i = 0;
  int             n;

  if (argc < 2)
    exit(FAILNOOB);

  n = stoi(get_arg(argv, 1));

  while (i < n)
  {
    c = a + b;
    printi(c);
    printn();
    b = a;
    a = c;
    i++;
  }
  exit(OMGROXX);
}
