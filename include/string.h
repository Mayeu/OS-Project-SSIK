/**
 * \file string.h
 * \brief String functions
 * \author Adrien Forest
 * \version 0.1
 * \date 24 Avril 2010
 */

#ifndef __STRING_H
#define __STRING_H

#include <stdlib.h>

 /**
 * \fn int strcpy(char *src, char *dest)
 * \brief Copy the string src to dest.
 *
 * \param src the source string
 * \param dest the destination string
 * \return the error identifier in case of any failure
 */
int             strcpy(char *src, char *dest);

 /**
 * \fn int strcpyn(char *src, char *dest, int length)
 * \brief Copy the length first characters of the string src to dest.
 *
 * \param src the source string
 * \param dest the destination string
 * \param length the number of characters to copy
 * \return the error identifier in case of any failure
 */
int             strcpyn(char *src, char *dest, int length);

 /**
 * \fn int strcmp(char *str1, char *str2)
 * \brief Compare the two string str1 and str2 to specify if str1 = str2 of which one of them is the first
alphabetically.
 *
 * \param str1 the first string
 * \param str2 the second string
 * \return 0 means str1 = str2, -1 means str1 < str2 and 1 means that str1 > str2
 */
int             strcmp(char *str1, char *str2);

 /**
 * \fn int strcmpn(char *str1, char *str2, int n)
 * \brief Compare the first n characters of the two string str1 and str2 to specify if str1 = str2 of which
one of them is the first alphabetically.
 *
 * \param str1 the first string
 * \param str2 the second string
 * \param n the number of characters to compare
 * \return 0 means str1 = str2, -1 means str1 < str2 and 1 means that str1 > str2
 */
int             strcmpn(char *str1, char *str2, int n);

 /**
 * \fn int strlen(char *str)
 * \brief Specify the number of characters of the string str.
 *
 * \param str the string
 * \return the length of the string. -1 if str invalid
 */
int             strlen(char *str);

 /**
 * \fn char* strchr(char *str, char c)
 * \brief Returns a pointer to the first occurrence of character in the C string str.
 *
 * \param str the string
 * \param c the character to find
 * \return a pointer to the first occurrence of character in str
 */
char           *strchr(char *str, char c);

 /**
 * \fn char *strcat(char *destination, char *source)
 * \brief Appends a copy of the source string to the destination string.
 *
 * \param dst the string where will be append the src string
 * \param src the string that will be append to the dst string
 * \return the address of the dst string
 */
char           *strcat(char *dst, char *src);

 /**
 * \fn bool isspace(char c)
 * \brief Checks if parameter c is a white-space character (SPC, TAB, LF, VT, FF, CR).
 *
 * \param c the char to evaluate
 * \return true means that c is a space character, false otherwise
 */
bool            isspace(char c);

 /**
 * \fn char* itos(int value, char *str)
 * \brief Converts an integer value to a null-terminated string using the specified 
 base and stores the result in the array given by str parameter.
 *
 * \param value the value to be converted to a string
 * \param str the array in memory where to store the resulting null-terminated string
 * \return A pointer to the resulting null-terminated string, same as parameter str
 */
char           *itos(int value, char *str);

 /**
 * \fn int stoi(char *str)
 * \brief Parses the C string str interpreting its content as an integral
 number, which is returned as an int value.
 *
 * \param str the string beginning with the representation of an integral number
 * \return On success, the function returns the converted integral number as an int value.
 */
int             stoi(char *str);


 /**
 * \fn char *trim(char *str)
 * \brief Remove all the non-usefull spaces
 *
 * \param str the string to clean
 * \return A pointer to the resulting null-terminated string, same as parameter str
 */
char *trim(char *str);


#endif //__STRING_H
