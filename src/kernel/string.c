/**
 * \file string.h
 * \brief String functions
 * \author Adrien Forest
 * \version 0.1
 * \date 24 Avril 2010
 */

#include <string.h>

 /**
 * Copy the string src to dest.
 */
int strcpy(char *src, char *dest)
{
}

 /**
 * Copy the length first characters of the string src to dest.
 */
int strcpyn(char *src, char *dest, int length)
{
}

 /**
 * Compare the two string str1 and str2 to specify if str1 = str2 of which one of them is the first
alphabetically.
 */
int strcmp(char *str1, char *str2)
{
}

 /**
 * Compare the first n characters of the two string str1 and str2 to specify if str1 = str2 of which
one of them is the first alphabetically.
 */
int strcmpn(char *str1, char *str2, int n)
{
}

 /**
 * Specify the number of characters of the string str.
 */
int strlen(char *str)
{
}

 /**
 * res is a pointer to the first occurrence of character c in the string str.
 */
int strchr(char *str, char c, char *res)
{
}

 /**
 * Checks if parameter c is a white-space character (SPC, TAB, LF, VT, FF, CR).
 */
bool isspace(char c)
{
}
