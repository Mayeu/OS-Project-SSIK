/**
 * \file coquille.c
 * \brief shell functions
 * \author Adrien Forest
 * \version 0.1
 * \date 21 Mai 2010
 */

#include <string.h>
#include <stdio.h>
#include <process.h>
#include <error.h>
#include <message.h>

#include "coquille.h"
#include "coquille_up.h"

#include "scroll.h"
#include "increment.h"
#include "fibonacci.h"


char            command_arg[MAX_SHELL_ARG][ARG_SIZE];

void
coquille(void)
{
  int             nb_arg, pid, status, cexit = 0;
  char            prompt_line[255];
  char            buffer[255];
  // char            ibuf[3];

  strcpy("coquille> ", prompt_line);
  print(prompt_line);

  while (!cexit)
  {
    // waiting for the user to enter a command
    gets(buffer, 255);
    printn();

    // split the string
    nb_arg = split_args(buffer, command_arg);

    if (nb_arg != -1)
    {
      // if the command is exit, exit the shell
      if (strcmp("exit", command_arg[0]) == 0)
        cexit = 1;
      // otherwise create the desirated process
      else
      {
        pid =
          fourchette(command_arg[0], BAS_PRI, nb_arg, (char **) command_arg);
        if (pid > 0)
          wait(pid, &status);
        else
          perror("Error");
      }
    }
    if (!cexit)
      print("coquille> ");
  }
  exit(0);
}

int
split_args(char *str, char data[MAX_SHELL_ARG][ARG_SIZE])
{
  int             i = 0, cpt = 0;
  char           *next;
  str = trim(str);

  while (*str != '\0')
  {
    next = strchr(str, ' ');
    if (next != NULL)
    {
      if (i < MAX_SHELL_ARG)
      {
        strcpyn(str, data[i], next - str);
        cpt++;
      }
      str = next;
      str++;
    }
    else
    {
      next = strchr(str, '\0');
      if (i < MAX_SHELL_ARG)
      {
        strcpy(str, data[i]);
        cpt++;
      }
      str = next;
    }
    i++;
  }

  return (i <= MAX_SHELL_ARG) ? cpt - 1 : -1;
}
