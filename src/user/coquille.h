/**
 * \file coquille.h
 * \brief Shell functions.
 * \author Adrien Forest
 * \version 0.1
 * \date 21 Mai 2010
 */

#ifndef __COQUILLE_H
#define __COQUILLE_H

#define MAX_SHELL_ARG 5
#define MAX_CHAR 20

 /**
 * \fn void coquille(void)
 * \brief Shell program.
 */
void            coquille(void);

 /**
 * \fn int split_args(char *str, char[MAX_SHELL_ARG][MAX_CHAR])
 * \brief Shell program.
 *
 * \param str string to parse and split
 * \param char** array Output array where will be stored the different arguments
 */
int             split_args(char *str, char[MAX_SHELL_ARG][MAX_CHAR]);

#endif //__COQUILLE_H
