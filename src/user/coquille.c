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
#include <process.h>

#define MAX_ARG 5

char command_arg[MAX_ARG][20];

void coquille(void)
{
	int res;
	char prompt_line[255];
	char buffer[255];

	strcpy(prompt_line, "coquille> ");

	print(prompt_line);

	while (1)
	{
		// waiting for the user to enter a command
		fgets(buffer, 255);

		// split the string
		res = split_input(buffer, command_arg);

		if (res != -1)
		{
			// fourchette
		}

	}

}

int split_arg(char *str, char data[MAX_ARG][20])
{
    int i=0, cpt=0;
    char *next;
    str = trim(str);

    while (*str != '\0')
    {
        next = strchr(str, ' ');
        if (next != NULL)
        {
            if (i < MAX_ARG)
            {
                strcpyn(str, data[i], next-str);
                cpt++;
            }
            str = next;
            str++;
        }
        else{
            next = strchr(str, '\0');
            if (i < MAX_ARG)
            {
                strcpy(str, data[i]);
                cpt++;
            }
            str = next;
        }
        i++;
    }

    return (i <= MAX_ARG) ? cpt : -1;
}
