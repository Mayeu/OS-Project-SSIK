/**
 * \file stdio.c
 * \brief Display functions
 * \author Adrien Forest
 * \version 0.1
 * \date 20 Mai 2010
 */

#include <stdio.h>
#include "ksyscall.h"

 /**
 * Print the string str to the standard output.
 */
int
print(char *str)
{
  syscall_one((int32_t) str, PRINT);
  return 0;
}

 /**
 * Print the string str to the specified output.
 */
int
fprint(int out, char *str)
{
  return 0;
}

 /**
 * Returns the character currently pointed by the internal file position indicator of the input stream
 */
char
getc(void)
{
  return 'c';
}

 /**
 * Reads characters from stream and stores them as a string into str until (num-1) characters have
 */
int
gets(char *str, int num)
{
  return 0;
}
