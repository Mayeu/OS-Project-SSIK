/**
 * \file scroll.c
 * \brief blabla
 * \author Adrien Forest
 * \version 0.1
 * \date 26 mai 2010
 */

#include <stdio.h>
#include <string.h>
#include <process.h>

#define MALTA_SIZE 8

// params: char* message, int sleep
int
scroll(int argc, char *argv[])
{
  char            buf[255];
  char            screen[MALTA_SIZE + 1];
  int             len, pos_space, sleept, j;
  char           *str = get_arg(argv, 1);

  strcpy(str, buf);
  strcat(buf, "      ");        // SIZE - 2 spaces

  len = pos_space = strlen(str) + MALTA_SIZE - 2;
  sleept = stoi(get_arg(argv, 1));

  while (1)
  {
    if (pos_space < MALTA_SIZE)
    {
      strcpyn(&buf[len - pos_space], screen, pos_space);
      screen[pos_space] = ' ';
      strcpyn(buf, &screen[pos_space + 1], MALTA_SIZE - pos_space);
    }
    else
      strcpyn(&buf[len - pos_space], screen, MALTA_SIZE);

    screen[MALTA_SIZE] = '\0';

    fprint(MALTA, screen);

    pos_space = (pos_space) ? pos_space - 1 : len;

    for (j = 0; j < 6000000; j++);
    //sleep(sleept);
  }

  exit(0);
}
