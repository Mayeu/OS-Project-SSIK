/**
 * \file stdio.c
 * \brief Display functions
 * \author Adrien Forest
 * \version 0.1
 * \date 20 Mai 2010
 */

#include <stdio.h>
#include <string.h>
#include "ksyscall.h"

 /**
 * Print the string str to the standard output.
 */
int
print(char *str)
{
  return syscall_one((int32_t) str, PRINT);
}

 /**
 * \brief Print the string str to the standard output.
and return line.
 */
int println(char *str)
{
  return syscall_one((int32_t) str, PRINTLN);
}

 /**
 * Print the number n to the standard output.
 */
int printi(int n)
{
	char buf[10];
	return print(itos(n, buf));
}

 /**
 * Print the number n to the standard output.
and return line.
 */
int printiln(int n)
{
	char buf[10];
	return println(itos(n, buf));
}

 /**
 * Print the string str to the specified output.
 */
int
fprint(int out, char *str)
{
  return syscall_two(out, (int32_t)str, FPRINT);
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
