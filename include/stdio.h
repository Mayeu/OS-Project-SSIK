/**
 * \file stdio.h
 * \brief Display functions
 * \author Adrien Forest
 * \version 0.1
 * \date 24 Avril 2010
 *
 */

#IFNDEF __STDIO_H
#DEFINE __STDIO_H

 /**
 * \fn int printf(char *str)
 * \brief Print the string str to the standard output.
 *
 * \param str the string to print
 * \return the error identifier in case of any failure
 */
int             printf(char *str);

 /**
 * \fn int fprintf(int out, char *str)
 * \brief Print the string str to the specified output.
 *
 * \param ou the output where to print (0 = console, 1 = Malta)
 * \param str the string to print
 * \return the error identifier in case of any failure
 */
int             fprintf(int out, char *str);

 /**
 * \fn char getc(void)
 * \brief Returns the character currently pointed by the internal file position indicator of the input stream
(keyboard).
 *
 * \param void
 * \return the character rade
 */
char            getc(void);

 /**
 * \fn int fgets(char *str, int num)
 * \brief Reads characters from stream and stores them as a string into str until (num-1) characters have
been readen or either a newline or a the End-of-File is reached, whichever comes first. stream
(keyboard). A null character is added to the end.
 *
 * \param str the string rade from the input steam (keyboard)
 * \param num the number of characters to be rade
 * \return the error identifier in case of any failure
 */
int             fgets(char *str, int num);

#__STDIO_H
