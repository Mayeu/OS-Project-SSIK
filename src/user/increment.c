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
#include <errno.h>

#include "increment.h"

/**
 * Program that prints the number from 1 to the specified N
 * \private
 */
void
increment(int argc, char *argv[])
{
  int             i = 1;
  int             n = stoi(get_arg(argv, 1));
  for (; i <= n; i++)
  {
    printi(i);
    printn();
  }

  exit(OMGROXX);
}
