/**
 * @file quit.c
 * @brief Really simple program who print "Hello from <pid>" and quit
 *
 * This program is mainly here for testing purpose
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <process.h>
#include <string.h>

void
quit_main(int argc, char **argv)
{
  int             mypid;
  char            c[20];

  mypid = get_pid();

  print("Hello from ");
  print(itos(mypid, c));
  print("!\n");

  exit(OMGROXX);
}
