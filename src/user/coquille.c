/**
 * \file shell.c
 * \brief shell functions
 * \author Adrien Forest
 * \version 0.1
 * \date 21 Mai 2010
 */

#include <coquille.h>
#include <string.h>
#include <stdio.h>

#define MAX_ARG 5

char command_arg[MAX_ARG][20];

void coquille(void)
{
	char prompt_line[255];
	char buffer[255];

	strcpy(prompt_line, "coquille> ");

	print(prompt_line);

	while (1)
	{
		// waiting for the user to enter a command
		fgets(buffer, 255);

		// split the string
		split_input(buffer, command_arg);

	}

}

char* skipwhite(char* s)
{
	while (isspace(*s)) s++;
	return s;
}


void split_arg(char *str, char data[MAX_ARG][20])
{
    int i=0;
    char *next = skipwhite(str);

    while ((next = strchr(str, ' ')) && (i < MAX_ARG-1))
    {
        strcpyn(str, data[i++], next-str);
        str += next-str;
        str = skipwhite(str);
    }

    next = strchr(str, '\0');
    strcpy(str, data[i]);
}
