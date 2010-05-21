/**
 * \file error.c
 * \brief Error functions
 * \author Adrien Forest
 * \version 0.1
 * \date 20 Mai 2010
 */

#include <error.h>

 /**
 * the perror() function writes the last error that occured followed by a newline, to the standard
output. If the argument string is non-NULL, this string is prepended to the message string and
separated from it by a colon and space (“: ”); otherwise, only the error message string is printed.
 */
void perror(char *error_msg)
{
}

 /**
 * the gerror() function return the last error code encountered.
 */
int gerror(void)
{
}

 /**
 * \brief the serror() function set the error
 */
void serror(int e)
{
}
