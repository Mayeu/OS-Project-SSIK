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

#include "fibonacci.h"

int
fibonacci(int argc, char *argv[])
{
  int             i = 2;
  int             f0 = 1, f1 = 1, f;
  int             n = stoi(get_arg(argv, 1));

  printi(f0); printn();
  printi(f1); printn();

  while (i <= n)
  {
    f = f0 + f1;
    f0 = f1;
    f1 = f;
    printi(f1); printn();
    i++;
  }
  exit(0);
  return 0;
}
