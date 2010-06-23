/**
 * \file philosopher.c
 * \brief blabla
 * \author Adrien Forest
 * \version 0.1
 * \date 26 mai 2010
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <message.h>
#include <process.h>
#include "philosopher.h"

#define MAX_PHILO 20
#define TIMEOUT 20000

enum
{
  START,                        /*!< session is starting */
  FORK_L,                       /*!< The philosopher want the left fork */
  FORK_R,                       /*!< The philosopher want the right fork */
  RELEASE,                      /*!< The philosopher realease her fork */
  FORK_FREE,                    /*!< This fork is free */
  FORK_TAKEN,                   /*!< This fork is taken */
  TAKE_THE_FORK,                /*!< Yes you can take the fork */
  END                           /*!< End of the party guy! */
};

/**
 * \private
 * Program that implements the dining philospher problem
 * \param argc nombre d'arguments
 * \param argv arguments
 */
void
dining_philosopher(int argc, char *argv[])
{

  int             nb_philo = stoi(get_arg(argv, 1));
  int             loop = stoi(get_arg(argv, 2));
  int             waiter;       //pid waiter
  int             philos[MAX_PHILO];    //pids philos
  int             status;
  char            args[2][ARG_SIZE];
  char            philos_args[MAX_PHILO][4][ARG_SIZE];
  int             i, j;
  char            tmp[10];
  char            text[200];

  if (argc != 3)
  {
    print
      ("Invalid arguments number: excepted number of philosophers and the number of cycles\n");
    exit(FAILNOOB);
  }

  if (nb_philo < 2 || loop < 1)
  {
    print("Invalid arguments\n");
    exit(INVARG);
  }

  /*
   * Creating the waiter's args
   */
  strcpy("waiter", args[0]);    /* prog_name */
  strcpy(get_arg(argv, 1), args[1]);    /* nb_philo */

  /*
   * Create the waiter
   */
  waiter = fourchette("waiter", BAS_PRI, 2, (char **) args);

  /*
   * Something went wrong ?
   */
  if (waiter < 1)
  {
    strcpy("Error creating waiter", text);
    strcat(text, " : ");
    strcat(text, itos(waiter, tmp));
    strcat(text, "\n");
    print(text);
    exit(waiter);
  }

  /*
   * Spawn the philos
   */
  for (i = 0; i < nb_philo; i++)
  {
    /*
     * Build the args for the philos
     */
    strcpy("philosopher", philos_args[i][0]);
    itos(waiter, philos_args[i][1]);
    itos(i, philos_args[i][2]); /* index of the pid in the philos id array */
    strcpy(get_arg(argv, 2), philos_args[i][3]);        /* number of loop */
    philos[i] =
      fourchette("philosopher", BAS_PRI, 4, (char **) philos_args[i]);

    /*
     * Something went wrong ?
     */
    if (philos[i] < 1)
    {
      strcpy("Error creating a philosopher", text);
      strcat(text, " : ");
      strcat(text, itos(philos[i], tmp));
      strcat(text, "\n");
      print(text);
      /*
       * Clean it up
       */
      for (j = 0; j < i; j++)
        kill(philos[j]);

      kill(waiter);
      exit(FAILNOOB);
    }
  }

  /*
   * Wait for the end
   */
  for (i = 0; i < nb_philo; i++)
    wait(philos[i], &status);

  /*
   * Stop the waiter
   */
  send((void *) END, INT_T, waiter);
  wait(waiter, &status);
  print("THE END\n");

  exit(OMGROXX);
}

/**
 * \private
 * Process used as a master for the philosoophers to manange the communications and the forks
 * \param argc nombre d'arguments
 * \param argv arguments
 */
void
waiter(int argc, char *argv[])
{
  if (argc < 2)
    exit(FAILNOOB);
  char            tmp[10], text[100];
  int             nb_philo = stoi(get_arg(argv, 1));
  int             philo_id, philo_pid;
  int             fork[nb_philo], philos[nb_philo];

  int             i, j, code, in, do_packing, end = 0;
  int             buf_req[nb_philo];
  int             buf_phi[nb_philo];    /* buffer used to postpone unsatisfied request */
  int             fork_taken = 0;       /* counts the number of fork taken */

  strcpy("Waiter no_", text);
  strcat(text, itos(get_pid(), tmp));
  strcat(text, ": serving ");
  strcat(text, itos(nb_philo, tmp));
  strcat(text, " philosophers\n");
  print(text);

  /*
   * All the fork are free
   */
  for (i = 0; i < nb_philo; i++)
    fork[i] = FORK_FREE;

  /*
   * The philosopher send their pid
   */
  strcpy("Received pids: ", text);

  for (i = 0; i < nb_philo; i++)
  {
    philo_pid = recv((int *) &philo_id, INT_T, TIMEOUT);

    if (philo_pid < 0)
    {
      print("Failed to receive philosopher pid\n");
      exit(FAILNOOB);
    }
    philos[philo_id] = philo_pid;
    strcat(text, itos(philo_pid, tmp));
    strcat(text, " ");
  }
  strcat(text, "\n");
  print(text);

  /*
   * Start the philos
   */
  for (i = 0; i < nb_philo; i++)
    send((void *) START, INT_T, philos[i]);

  in = 0;
  while (!end)
  {
    /*
     * Waiting for a request
     */
    philo_pid = recv(&code, INT_T, 10000);

    /*
     * No request
     */
    if (philo_pid < 1)
      continue;

    /*
     * We look for the index of the philos who send the message
     */
    for (i = 0; i < nb_philo && philos[i] != philo_pid; i++);

    philo_id = i;

    /*
     * If the philo ask for a fork
     */
    if (code == FORK_L || code == FORK_R)
    {
      bool            t;
      if (code == FORK_L)
        t = left_fork(philo_id, philo_pid, fork, &fork_taken, nb_philo);
      else
        t =
          right_fork((philo_id + 1) % nb_philo, philo_pid, fork, &fork_taken);

      if (!t)
      {
        /*
         * In case all fork are taken, or only one fork left and asked by a philo without any fork, we add
         * the request to the buffer
         */
        buf_req[in] = code;
        buf_phi[in] = philo_id;
        in++;
      }
    }

    /*
     * The philos release her fork
     */
    else if (code == RELEASE)
    {
      fork[philo_id] = FORK_FREE;       /* release the left fork */
      fork[(philo_id + 1) % nb_philo] = FORK_FREE;      /* release the right fork */
      fork_taken -= 2;

      /*
       * Look for buffered request to satisfy
       */
      for (i = 0; i < in; i++)
      {
        do_packing = 0;
        code = buf_req[i];
        philo_id = buf_phi[i];
        philo_pid = philos[philo_id];

        /*
         * Same as before
         */
        if (code == FORK_L || code == FORK_R)
        {
          bool            t;
          if (code == FORK_L)
            t = left_fork(philo_id, philo_pid, fork, &fork_taken, nb_philo);
          else
            t =
              right_fork((philo_id + 1) % nb_philo, philo_pid, fork,
                         &fork_taken);

          if (t)
            do_packing = 1;
        }

        /*
         * If request are satisfied we can pack the array
         */
        if (do_packing)
        {
          for (j = i + 1; j < in; j++)
          {
            buf_req[j - 1] = buf_req[j];
            buf_phi[j - 1] = buf_phi[j];
          }
          in--;
          i--;
        }
      }
    }

    /*
     * The end code is sent
     */
    else if (code == END)
      end = 1;
  }

  exit(OMGROXX);
}

/**
 * \private
 * Process used as a philosoopher who wants to take two forks and eat.
 * \param argc nombre d'arguments
 * \param argv arguments
 */
void
philosopher(int argc, char *argv[])
{
  int             count = 0;    /* each philosopher does loop cycles before exiting */
  char            tmp[10], proctext[50], text[200];
  int             mess;
  int             philo_id, waiter_pid, loop;

  if (argc < 4)
  {
    print
      ("Invalid arguments number: expected waiter pid, philosopher id and number of cycles\n");
    exit(INVARG);
  }

  waiter_pid = stoi(get_arg(argv, 1));
  philo_id = stoi(get_arg(argv, 2));
  loop = stoi(get_arg(argv, 3));

  strcpy("Process no_", proctext);
  strcat(proctext, itos(get_pid(), tmp));
  strcat(proctext, ": ");

  /*
   * each philosopher gives its philo_id to the waiter.
   */
  send((void *) philo_id, INT_T, waiter_pid);
  strcpy(proctext, text);
  strcat(text, "sent its id '");
  strcat(text, itos(philo_id, tmp));
  strcat(text, "' to the waiter no_");
  strcat(text, itos(waiter_pid, tmp));
  strcat(text, "\n");
  print(text);

  /*
   * wait until the waiter says the show can begin
   */
  if (recv_from_pid(&mess, INT_T, waiter_pid, 1000) != waiter_pid
      && mess != START)
  {
    print("Failed to receive order to proceed from the waiter\n");
    exit(FAILNOOB);
  }

  while (count < loop)
  {
    strcpy(proctext, text);
    strcat(text, "is thinking\n");
    print(text);

    sleep((unsigned int) random((int) &count, (int) &loop) % 2000);

    strcpy(proctext, text);
    strcat(text, "is hungry\n");
    print(text);

    if (send((void *) FORK_L, INT_T, waiter_pid) != OMGROXX)
    {
      print("Failed to send the request for the left fork\n");
      exit(FAILNOOB);
    }
    recv_from_pid(&mess, INT_T, waiter_pid, 10000);

    strcpy(proctext, text);
    strcat(text, "got left fork\n");
    print(text);

    if (send((void *) FORK_R, INT_T, waiter_pid) != OMGROXX)
    {
      print("Failed to send the request for the right fork\n");
      exit(FAILNOOB);
    }
    recv_from_pid(&mess, INT_T, waiter_pid, 10000);

    strcpy(proctext, text);
    strcat(text, "is eating\n");
    print(text);

    sleep((unsigned int) random((int) &mess, (int) &waiter_pid) % 2000);

    strcpy(proctext, text);
    strcat(text, "finished eating\n");
    print(text);

    send((void *) RELEASE, INT_T, waiter_pid);

    count++;
  }

  strcpy(proctext, text);
  strcat(text, "Was fun! I'm leaving.\n");
  print(text);

  exit(OMGROXX);
}

/**
 * \private
 * Specifies whether the process can take its right fork or not.
 * \param index index of the philos
 * \param phio_pid pid of the philo
 * \param fork pointer to the fork
 * \param forks_taken
 * \param num_philo
 */
bool
left_fork(int index, int philo_pid, int *fork, int *fork_taken, int num_philo)
{
  /*
   * The left fork have the id of the philo
   */
  if (fork[index] == FORK_FREE && *fork_taken < (num_philo - 1))
  {
    /*
     * The fork is free, we take it
     */
    fork[index] = FORK_TAKEN;
    (*fork_taken)++;
    send((void *) TAKE_THE_FORK, INT_T, philo_pid);
    return TRUE;
  }
  /*
   * Bad philosopher! No fork for you !
   */
  return FALSE;
}

/**
 * \private
 * Specifies whether the process can take its right fork or not.
 * \param index index of the philos
 * \param phio_pid pid of the philo
 * \param fork pointer to the fork
 * \param forks_taken
 * \param num_philo
 */
bool
right_fork(int index, int philo_pid, int *fork, int *fork_taken)
{
  /*
   * The right fork is at (philo_pid+1)%NUM_PHILO
   */
  if (fork[index] == FORK_FREE)
  {
    /*
     * The fork is free, we take it
     */
    fork[index] = FORK_TAKEN;
    (*fork_taken)++;
    send((void *) TAKE_THE_FORK, INT_T, philo_pid);
    return TRUE;
  }

  /*
   * Bad philosopher! No fork for you !
   */
  return FALSE;
}

/**
 * Generate a random number with two numbers
 * \param argc nombre d'arguments
 * \param argv arguments
 */
int
random(int a, int b)
{
  return ((a * 63949 + 26227) % 1000) * ((b * 26227 + 63949) % 1000);
}
