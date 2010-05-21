/**
 * \file coquille.h
 * \brief shell functions
 * \author Adrien Forest
 * \version 0.1
 * \date 21 Mai 2010
 */

#ifndef __COQUILLE_H
#define __COQUILLE_H


#define MAX_ARG 5
#define MAX_CHAR 20

void            coquille(void);

int             split_input(char *str, char[MAX_ARG][MAX_CHAR]);

#endif //__COQUILLE_H
