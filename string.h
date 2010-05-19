/**
 * \file string.h
 * \brief String functions
 * \author Adrien Forest
 * \version 0.1
 * \date 24 Avril 2010
 *
 */

#IFNDEF __STRING_H
#DEFINE __STRING_H

 /**
 * \fn int strcpy(char *src, char *dest)
 * \brief Copy the string src to dest.
 *
 * \param src the source string
 * \param dest the destination string
 * \return the error identifier in case of any failure
 */
int strcpy(char *src, char *dest);

 /**
 * \fn int strcpyn(char *src, char *dest, int length)
 * \brief Copy the length first characters of the string src to dest.
 *
 * \param src the source string
 * \param dest the destination string
 * \param length the number of characters to copy
 * \return the error identifier in case of any failure
 */
int strcpyn(char *src, char *dest, int length);

 /**
 * \fn int strcmp(char *str1, char *str2)
 * \brief Compare the two string str1 and str2 to specify if str1 = str2 of which one of them is the first
alphabetically.
 *
 * \param str1 the first string
 * \param str2 the second string
 * \return 0 means str1 = str2, -1 means str1 < str2 and 1 means that str1 > str2
 */
int strcmp(char *str1, char *str2);

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
int strcmpn(char *str1, char *str2, int n);

 /**
 * \fn int strlen(char *str)
 * \brief Specify the number of characters of the string str.
 *
 * \param str the string
 * \return the length of the string. -1 if str invalid
 */
int strlen(char *str);

 /**
 * \fn int strchr(char *str, char c, char *res)
 * \brief res is a pointer to the first occurrence of character c in the string str.
 *
 * \param str the string
 * \param c the character to find
 * \param res the substring (result)
 * \return the error identifier in case of any failure
 */
int strchr(char *str, char c, char *res);

 /**
 * \fn bool isspace(char c)
 * \brief Checks if parameter c is a white-space character (SPC, TAB, LF, VT, FF, CR).
 *
 * \param c the char to evaluate
 * \return true means that c is a space character, false otherwise
 */
bool isspace(char c);


#__STRING_H
