/**
 * \file string.h
 * \brief String functions
 * \author Adrien Forest
 * \version 0.1
 * \date 24 Avril 2010
 */

#include <string.h>
#include <errno.h>

 /**
 * Copy the string src to dest.
 */
int
strcpy(char *src, char *dst)
{
  int             i = 0;

  if ((src == NULL) || (dst == NULL))
    return NULLPTR;

  while (src[i] != '\0')
  {
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';

  return OMGROXX;
}

 /**
 * Copy the length first characters of the string src to dest.
 */
int
strcpyn(char *src, char *dst, int length)
{
  int             i = 0;

  if ((src == NULL) || (dst == NULL))
    return NULLPTR;

  while ((src[i] != '\0') && (i < length))
  {
    dst[i] = src[i];
    i++;
  }
  dst[i] = '\0';

  return OMGROXX;
}

 /**
 * Compare the two string str1 and str2 to specify if str1 = str2 of which one of them is the first
alphabetically.
 */
int
strcmp(char *str1, char *str2)
{
  if ((str1 == NULL) || (str2 == NULL))
    return NULLPTR;

  while ((*str1 == *str2) && (*str1 != '\0'))
  {
    str1++;
    str2++;
  }

  if (*str1 == '\0')
    return 0;

  return (*str1 < *str2) ? -1 : 1;
}

 /**
 * Compare the first n characters of the two string str1 and str2 to specify if str1 = str2 of which
one of them is the first alphabetically.
 */
int
strcmpn(char *str1, char *str2, int n)
{
  if ((str1 == NULL) || (str2 == NULL))
    return NULLPTR;

  while (n-- > 0)
  {
    if (*str1 != *str2)
      return (*str1 < *str2) ? -1 : 1;

    if (*str1 == '\0')
      return 0;

    str1++;
    str2++;
  }

  return 0;
}

 /**
 * Specify the number of characters of the string str.
 */
int
strlen(char *str)
{
  int             i = 0;

  if (str == NULL)
    return -1;

  while (*str++)
    i++;

  return i;
}

 /**
 * Returns a pointer to the first occurrence of character in the C string str.
 */
char           *
strchr(char *str, char c)
{
  if (str == NULL)
    return NULL;

  while (*str && *str != c)
    str++;

  if (*str == c)
    return str;

  return NULL;
}

 /**
 * Appends a copy of the source string to the destination string.
 */
char           *
strcat(char *dst, char *src)
{
  char           *d = dst;

  if ((dst == NULL) || (src == NULL))
    return 0;

  while (*d)
    ++d;
  while ((*d++ = *src++) != '\0');

  return dst;
}

 /**
 * Checks if parameter c is a white-space character (SPC, TAB, LF, VT, FF, CR).
 */
bool
isspace(char c)
{
  if (c == ' ' || c == '\t' || c == '\n')
    return TRUE;
  else
    return FALSE;
}

 /**
 * Converts an integer value to a null-terminated string using the specified 
 base and stores the result in the array given by str parameter.
 */
char           *
itos(int value, char *str)
{
  int             i = 1, r;

  if (str == NULL)
    return 0;

  if (value < 0)
  {
    str[0] = '-';
    value = -value;
    i++;
  }

  r = value / 10;
  while (r != 0)
  {
    r /= 10;
    i++;
  }

  str[i--] = '\0';

  do
  {
    str[i--] = value % 10 + '0';
    value /= 10;
  }
  while (value != 0);

  return str;
}

 /**
 * Parses the C string str interpreting its content as an integral
 number, which is returned as an int value.
 */
int
stoi(char *str)
{
  int             i = 0, id = 0, neg = 1;

  if (str == NULL)
    return NULLPTR;

  if (*str == '-')
  {
    neg = -neg;
    i++;
  }

  while (str[i] != '\0')
    id = id * 10 + str[i++] - '0';

  return neg * id;
}
