/**
 * \file error.h
 * \brief Error functions
 * \author Adrien Forest
 * \version 0.1
 * \date 21 Mai 2010
 */

#ifndef __ERROR_H
#define __ERROR_H

 /**
 * \fn void perror(char *error_msg)
 * \brief the perror() function writes the last error that occured followed by a newline, to the standard
output. If the argument string is non-NULL, this string is prepended to the message string and
separated from it by a colon and space (“: ”); otherwise, only the error message string is printed.
 *
 * \param error_msg error message to print
 */
void            perror(char *error_msg);

 /**
 * \fn int gerror(void)
 * \brief the gerror() function return the last error code encountered.
 *
 * \param void
 * \return the error code
 */
int             gerror(void);

 /**
 * \fn void serror(int e)
 * \brief the serror() function set the error
 *
 * \param e an error code
 * \return void
 */
void            serror(int e);

#endif //__ERROR_H
