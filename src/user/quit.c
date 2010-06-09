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
  char            ibuf[3];
  char            c[20];

  mypid = get_pid();

  strcpy("Hello from ", c);
  strcat(c, itos(mypid, ibuf));
  strcat(c, "!\n");

  print(c);

  exit(mypid);
}
