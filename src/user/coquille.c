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
#include <error.h>
#include <message.h>

#include "coquille_up.h"

#include "scroll.h"
#include "increment.h"
#include "fibonacci.h"


char            command_arg[MAX_SHELL_ARG][MAX_CHAR];

void
coquille(void)
{
  int             nb_arg, pid;
  char            prompt_line[255];
	char 						ibuf[3];
  char            prog_name[20];
  char            *command = "init arg1 arg2 arg3 arg4";
	char						proc_inf[2][20];
	char						scroll_inf[3][20];
	char						incr_inf[2][20];
	char						fib_inf[2][20];

	strcpy("proc_info", proc_inf[0]);

	strcpy("scroll", scroll_inf[0]);
	strcpy("phrase qui scroll!", scroll_inf[1]);
	strcpy(itos(500, ibuf), scroll_inf[2]);

	strcpy("increment", incr_inf[0]);
	strcpy(itos(10, ibuf), incr_inf[1]);

	strcpy("fibonacci", fib_inf[0]);
	strcpy(itos(8, ibuf), fib_inf[1]);

  strcpy("coquille> ", prompt_line);

  print(prompt_line);

  nb_arg = split_args(command, command_arg);

  println(command);

	ps(1, NULL);

	help(1, NULL);

	// INCREMENT
	increment(2, (char**)incr_inf);

	// FIBONACCI
	fibonacci(2, (char**)fib_inf);

	// TEST SCROLL
	//scroll(3, (char**)scroll_inf);

  if (nb_arg != -1)
  {
    pid = fourchette(command_arg[0], BAS_PRI, nb_arg, (char**) command_arg);

		// FAIRE WAIT SUR LE PID (???)

		strcpy(itos(pid, ibuf), proc_inf[1]);
		
		proc_info(2, (char**)proc_inf);

    println("changed prio to 30");
    chg_pri(pid, 30);
    printiln(gerror());
    perror("Erreur !");

		proc_info(2, (char**)proc_inf);

		send(prog_name, CHAR_PTR, 5);
		send((void*)42, INT_T, 2);
  }

/*
  while (1)
  {
    // waiting for the user to enter a command
    //fgets(buffer, 255);

    // split the string
    //res = split_args(buffer, command_arg);
		res = split_args(command, command_arg);

    if (res != -1)
    {
      // fourchette
    }

  }
*/
}

int
split_args(char *str, char data[MAX_SHELL_ARG][MAX_CHAR])
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
