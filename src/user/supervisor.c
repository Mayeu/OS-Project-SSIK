/**
 * \file supervisor.
 * \brief supervisor user program.
 * \author Adrien Forest
 * \version 0.1
 * \date 07 juin 2010
 */

#include <stdio.h>
#include <errno.h>
#include <process.h>
#include <string.h>

#include "supervisor.h"

int get_random(int m_z, int m_w)
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;  /* 32-bit result */
}

int Rand32(void)
/****************************************************************************
 *
 *  A 32 bit random number generator.  An implementation in C of the
 *  algorithm given by Knuth, the art of computer programming, vol. 2,
 *  pp. 26-27.  We use e=32, so we have to evaluate y(n) = y(n-24) + y(n-55)
 *  mod 2^32, which is implicitly done by unsigned arithmetic.
 *
 ****************************************************************************/
{

   static unsigned int x[55] =
   {
	1410651636UL,
	3012776752UL,
	3497475623UL,
	2892145026UL,
	1571949714UL,
	3253082284UL,
	3489895018UL,
	387949491UL,
	2597396737UL,
	1981903553UL,
	3160251843UL,
	129444464UL,
	1851443344UL,
	4156445905UL,
	224604922UL,
	1455067070UL,
	3953493484UL,
	1460937157UL,
	2528362617UL,
	317430674UL,
	3229354360UL,
	117491133UL,
	832845075UL,
	1961600170UL,
	1321557429UL,
	747750121UL,
	545747446UL,
	810476036UL,
	503334515UL,
	4088144633UL,
	2824216555UL,
	3738252341UL,
	3493754131UL,
	3672533954UL,
	29494241UL,
	1180928407UL,
	4213624418UL,
	33062851UL,
	3221315737UL,
	1145213552UL,
	2957984897UL,
	4078668503UL,
	2262661702UL,
	65478801UL,
	2527208841UL,
	1960622036UL,
	315685891UL,
	1196037864UL,
	804614524UL,
	1421733266UL,
	2017105031UL,
	3882325900UL,
	810735053UL,
	384606609UL,
	2393861397UL
   };
   static int init = 1;
   static unsigned int y[55];
   static int j, k;
   unsigned int ul;

   if (init)
   {
      int i;
      init = 0;
      for (i = 0; i < 55; i++)
         y[i] = x[i];
      j = 24 - 1;
      k = 55 - 1;
   }
   ul = (y[k] += y[j]);
   if (--j < 0) j = 55 - 1;
   if (--k < 0) k = 55 - 1;
   return (ul);
}

void supervisor(int argc, char *argv[])
{
	int             i, j;
	int							pid[MAX_SUP];
  pcbinfo         pcbi;

  get_proc_info(get_pid(), &pcbi);
  get_proc_info(pcbi.supervisor, &pcbi);

  // if the supervisor process is not 'supervisor', case parent
  if (strcmp(pcbi.name, "supervisor") != 0)
	{
    int             nb_proc;
		int							nb_lives;
		char 						buffer[255];
		char						num[3];
		int 						status;
		int							lives[MAX_SUP];
    char            args[3][ARG_SIZE];

    // number of proc in argv[0]
    nb_proc = stoi(get_arg(argv, 1));
		// number of 'lives' for each process
    nb_lives = stoi(get_arg(argv, 2));

    if (nb_proc > MAX_SUP)
    {
      print("Error: Number of supervised must be at most ");
      printi(MAX_SUP);
      printn();
      exit(-1);
    }

    // fill the argument array for the childs
    strcpy("supervisor", args[0]);
    itos(get_pid(), get_arg(argv, 1));
    strcpy(get_arg(argv, 2), args[2]);
/*
		pid[0] = fourchette("supervisor", BAS_PRI, 3, (char **) args);
		wait(pid[0], &status);

		printi(status);print(" hum\n");
*/

    // creating the children
    for (i = 0; i < nb_proc; i++)
		{
			lives[i] = nb_lives;
      pid[i] = fourchette("supervisor", BAS_PRI, 3, (char **) args);
		}

		// wait for them and restard the dead ones
		for (j = 0; j< nb_lives; j++)
		{
		for (i = 0; i < nb_proc; i++)
		{
    	if (wait(pid[i], &status) == OMGROXX)
			{
				strcpy("My child ", buffer);
				strcat(buffer, itos(i, num));

				if (status < 0)
					strcat(buffer, " died in a bad way ");
				else
					strcat(buffer, " died in a good way ");

				lives[i] = lives[i] - 1;
				if (lives[i] >= 0)
				{
					strcat(buffer, "- Lets make him revive (");
					strcat(buffer, itos(lives[i], num));
					strcat(buffer, " lives left)\n");

					pid[i] = fourchette("supervisor", BAS_PRI, 3, (char **) args);
					//i = 0;
				}
				else
				{
					strcat(buffer, " And doesn't have any life left :(\n");
				}
				print(buffer);
			}
      else
			{
				strcpy("My child ", buffer);
				strcat(buffer, itos(i, num));
      	strcpy(buffer, " get lost :(\n");
				print(buffer);
			}
    } }

		exit(0);

	}
	else
	{
		int r;
		r = Rand32() % 5;
		//printi(r);
		if (r < 2)
			exit(-100);
		else
			exit(0);
	}
}
