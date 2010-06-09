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
arg_test_main(int argc, char **argv)
{
  int             mypid;

  mypid = get_pid();

  sleep(800);                   /* To ensure the father get the time to change the arg */

  print((char *) argv);

  if (strcmp(get_arg(argv, 1), "arg1") != 0)
    // || strcmp(argv[1], "arg1") != 0 || strcmp(argv[2], "arg2") != 0)
    exit(FAILNOOB);

  exit(OMGROXX);
}
