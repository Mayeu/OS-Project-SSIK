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

char            command_arg[MAX_SHELL_ARG][MAX_CHAR];

void
coquille(void)
{
  int             res, pid;
  char            prompt_line[255];
  char            buffer[255];
  char            prog_name[20];
  char           *command = "init arg1 arg2 arg3 arg4";
  pcbinfo         pcbi;

  strcpy("coquille> ", prompt_line);

  print(prompt_line);

  fprint(MALTA, "ABC DEF");

  res = split_args(command, command_arg);

  println(command);

  if (res != -1)
  {
    // copy the program name from the array
    strcpy(command_arg[0], prog_name);
    // copy the number of arguments at position 0, replacing the progr name
    strcpy(itos(res, buffer), command_arg[0]);

    pid = fourchette(prog_name, (char**) command_arg);

    // print the new pid
    print("pid : ");
    println(itos(pid, buffer));

    get_proc_info(pid, &pcbi);
    printiln(pcbi.empty);
    printiln(pcbi.pid);
    printiln(pcbi.pri);
    println(pcbi.name);

    println("changed prio to 30");
    chgpri(pid, 30);
    printiln(gerror());
    perror("Erreur !");

    get_proc_info(pid, &pcbi);
    printiln(pcbi.empty);
    printiln(pcbi.pid);
    printiln(pcbi.pri);
    println(pcbi.name);

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
