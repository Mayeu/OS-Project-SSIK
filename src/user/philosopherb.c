#include "eat.h"

int             left_fork(int, int, int *, int *, int);
int             right_fork(int, int, int *, int *);
unsigned int    get_random(int *, int *);
int             get_philo_id(int, int *, int);

/* main program. Creates the philosophers and the waiter and waits for termination of its children. */
void
dinner(int num_philo, int num_cycles)
{
  if (num_philo < 2 || num_cycles < 1)
  {
    ecrire("Parameters with impossible values\n");
    quitter(FAILURE);
  }
  int            *philos;       //to store the philosophers' pid
  int             waiter;
  int             my_prio = obtenir_priorite(obtenir_pid());
  int             child_prio = (my_prio == MAX_PRIORITY ? my_prio : my_prio + 1);       //let's give an lower priority to the children in order to minimize the number of preemptions before the start of the dinner.
  int             status;
  int             args[4] = { 0, 0, 0, 0 };

  philos = allouer(num_philo * sizeof(int));
  if (philos == NULL)
  {
    ecrire("Allouer philos failed\n");
    quitter(FAILURE);
  }

  int             i, j;
  ecrire("The dining philosophers. \nBon appétit !\n");
  // we need to spawn the waiter first
  args[0] = num_philo;
  waiter = fourchette(child_prio, "waiter", args);
  if (waiter < 1)
  {
    ecrire("An error occured. The program will stop.\n");
    quitter(FAILURE);
  }
  args[0] = waiter;             //the waiter's pid is a parameter of the philosophers
  args[2] = num_cycles;
  // spawns the philosophers
  for (i = 0; i < num_philo; i++)
  {
    args[1] = i;                //philosopher's id
    philos[i] = fourchette(child_prio, "philosopher", args);
    if (philos[i] < 1)
    {
      ecrire
        ("An error occured. Impossible to create the philosophers. The process will exit now.\n");
      //clean up
      for (j = 0; j < i; j++)
        tuer(philos[j], FAILURE);
      tuer(waiter, FAILURE);
      quitter(FAILURE);
    }
  }
  //everything's done, let's wait for the end 
  for (i = 0; i < num_philo; i++)
    attendre(philos[i], &status);
  //the waiter can stop
  envoyer_entier(waiter, END_OF_DINNER, 10);
  attendre(waiter, &status);
  ecrire("The dinner ends.\n");
  quitter(SUCCESS);
}

void
waiter(int num_philo)
{
  int             i, j, code, philo_pid, philo_id, in, do_packing, end = 0;
  int            *forks, *philos_pid;   //the waiter knows the status of each fork. Will contain FORK_TAKEN or FORK_FREE
  message_t       temp, recv, *buffer;  //the waiter will need to buffer the requests that can't be satisfied when he gets the message. At most num_philo-1 messages can be buffered at the same time. One more and it's a deadlock. 
  int             forks_taken = 0;      //counts the number of forks taken

  //let's allocate the arrays
  forks = allouer(num_philo * sizeof(int));
  if (forks == NULL)
  {
    ecrire("Allouer forks failed\n");
    quitter(FAILURE);
  }
  philos_pid = allouer(num_philo * sizeof(int));
  if (philos_pid == NULL)
  {
    ecrire("Allouer philos_pid failed\n");
    quitter(FAILURE);
  }
  buffer = allouer((num_philo - 1) * sizeof(message_t));
  if (buffer == NULL)
  {
    ecrire("Allouer buffer failed\n");
    quitter(FAILURE);
  }

  //we mark the forks as being free
  for (i = 0; i < num_philo; i++)
  {
    forks[i] = FORK_FREE;
  }

  //first, each philosopher sends its philo_id (NOT its pid) to the waiter
  for (i = 0; i < num_philo; i++)
  {
    recevoir_premier(&recv);
    philo_id = message_get_int_value(recv);
    philo_pid = message_get_from(recv);
    liberer_message(recv);
    philos_pid[philo_id] = philo_pid;
  }
  ecrire("Waiter: I have everything I need, let's go !\n");
  //starts the dinner
  for (i = 0; i < num_philo; i++)
    envoyer_entier(philos_pid[i], GO_FOR_IT, 10);

  in = 0;
  while (!end)
  {
    //the waiter loops until it receives the order to stop
    recevoir_premier(&recv);
    code = message_get_int_value(recv);
    philo_pid = message_get_from(recv);
    philo_id = get_philo_id(philo_pid, philos_pid, num_philo);
    switch (code)
    {
    case WANT_LEFT_FORK:
      if (!left_fork(philo_id, philo_pid, forks, &forks_taken, num_philo))
      {
        //fork taken or only one fork left. In the last case, it can only be taken by a philosopher who already has a fork. Since the left forks are requested first, this is not the case here. We put the request in the buffer
        buffer[in] = recv;
        in++;
      }
      else
        liberer_message(recv);
      break;
    case WANT_RIGHT_FORK:
      if (!right_fork
          ((philo_id + 1) % num_philo, philo_pid, forks, &forks_taken))
      {
        buffer[in] = recv;
        in++;
      }
      else
        liberer_message(recv);
      break;
    case PUT_FORKS_BACK:
      liberer_message(recv);
      forks[philo_id] = FORK_FREE;
      forks[(philo_id + 1) % num_philo] = FORK_FREE;
      forks_taken -= 2;
      //look if some of the buffered requests can be satisfied now
      for (i = 0; i < in; i++)
      {
        do_packing = 0;
        temp = buffer[i];
        code = message_get_int_value(temp);
        philo_pid = message_get_from(temp);
        philo_id = get_philo_id(philo_pid, philos_pid, num_philo);
        if (code == WANT_LEFT_FORK)
        {
          if (left_fork(philo_id, philo_pid, forks, &forks_taken, num_philo))
            //this request has been satisfied
            do_packing = 1;
        }
        else
          if (right_fork
              ((philo_id + 1) % num_philo, philo_pid, forks, &forks_taken))
          do_packing = 1;
        if (do_packing)
        {
          liberer_message(temp);
          //the request number i has been satisfied, let's pack the array
          for (j = i + 1; j < in; j++)
            buffer[j - 1] = buffer[j];
          in--;
          i--;
        }
      }
      break;
    case END_OF_DINNER:
      end = 1;
      liberer_message(recv);
      break;
    }
  }
}


/* An average philosopher: thinking, eating, thinking, eating and so on. What a livelocked mind ! */
void
philosopher(int waiter_pid, int philo_id, int num_cycles)
{
  int             m_w = 4000 - philo_id, m_z = philo_id + 2000; //initializers for random numbers generation
  int             count = 0;    //each philosopher does num_cycles cycles before exiting
  int             my_pid = obtenir_pid();
  char            s[MAX_STRING], buf[4];
  message_t       mess;

  itoa(my_pid, buf, 4);

  //each philosopher gives its philo_id to the waiter. Those id represent the id of the forks
  strncpy(s, "Process ", MAX_STRING);
  strncat(s, buf, MAX_STRING - strlen(s));
  strncat(s, " is sending its id to the waiter\n", MAX_STRING - strlen(s));
  ecrire(s);
  envoyer_entier(waiter_pid, philo_id, 10);
  //wait until the waiter says the show can begin
  if (recevoir_de_delai(waiter_pid, &mess, 1000) != SUCCESS)
  {
    ecrire("The waiter took too long to answer. Dinner canceled\n");
    quitter(FAILURE);
  }
  liberer_message(mess);
  strncpy(s, "Process ", MAX_STRING);
  strncat(s, buf, MAX_STRING - strlen(s));
  strncat(s, " starts the dinner\n", MAX_STRING - strlen(s));
  ecrire(s);

  while (count < num_cycles)
  {
    strncpy(s, "Process ", MAX_STRING);
    strncat(s, buf, MAX_STRING - strlen(s));
    strncat(s, " is thinking\n", MAX_STRING - strlen(s));
    ecrire(s);
    retarder(my_pid, random(&m_w, &m_z) % 5000);
    strncpy(s, "Process ", MAX_STRING);
    strncat(s, buf, MAX_STRING - strlen(s));
    strncat(s, " is trying to get its left fork\n", MAX_STRING - strlen(s));
    ecrire(s);
    if (envoyer_entier(waiter_pid, WANT_LEFT_FORK, 10) != SUCCESS)
    {
      ecrire("Envoyer_entier failed\n");
      quitter(FAILURE);
    }
    recevoir_de(waiter_pid, &mess);
    liberer_message(mess);      //the content of a message from the waiter is always GO_FOR_IT
    strncpy(s, "Process ", MAX_STRING);
    strncat(s, buf, MAX_STRING - strlen(s));
    strncat(s, " is trying to get its right fork\n", MAX_STRING - strlen(s));
    ecrire(s);
    if (envoyer_entier(waiter_pid, WANT_RIGHT_FORK, 10) != SUCCESS)
    {
      ecrire("Envoyer_entier failed\n");
      quitter(FAILURE);
    }
    recevoir_de(waiter_pid, &mess);
    liberer_message(mess);
    strncpy(s, "Process ", MAX_STRING);
    strncat(s, buf, MAX_STRING - strlen(s));
    strncat(s, " is eating\n", MAX_STRING - strlen(s));
    ecrire(s);
    retarder(my_pid, random(&m_w, &m_z) % 5000);
    strncpy(s, "Process ", MAX_STRING);
    strncat(s, buf, MAX_STRING - strlen(s));
    strncat(s, " puts its forks back\n", MAX_STRING - strlen(s));
    ecrire(s);
    envoyer_entier(waiter_pid, PUT_FORKS_BACK, 10);
    count++;
  }
  strncpy(s, "Process ", MAX_STRING);
  strncat(s, buf, MAX_STRING - strlen(s));
  strncat(s, ", thanks for the food !\n", MAX_STRING - strlen(s));
  ecrire(s);
}

/* check if philo_pid can take its left fork. Returns 1 if he can, 0 otherwise */
int
left_fork(int index, int philo_pid, int *forks, int *forks_taken,
          int num_philo)
{
  //the left fork has the same id as the philosopher 
  if (forks[index] == FORK_FREE && *forks_taken < (num_philo - 1))
  {
    //fork free and at least two forks remaining, go for it
    forks[index] = FORK_TAKEN;
    (*forks_taken)++;
    envoyer_entier(philo_pid, GO_FOR_IT, 10);
    return 1;
  }
  //this fork is not available/shouldn't be taken
  return 0;
}

/* check if philo_pid can take its right fork. Returns 1 if he can, 0 otherwise */
int
right_fork(int index, int philo_pid, int *forks, int *forks_taken)
{
  //the right is at the index (philo_pid+1)%NUM_PHILO
  if (forks[index] == FORK_FREE)
  {
    // no need to check for forks_taken because a process requesting its right fork already has the left one
    forks[index] = FORK_TAKEN;
    (*forks_taken)++;
    envoyer_entier(philo_pid, GO_FOR_IT, 10);
    return 1;
  }
  return 0;
}

/* Looks into the philos_pid array in order to find philo_pid. Once found, it returns the index, that is the philo_id */
int
get_philo_id(int philo_pid, int *philos_pid, int num_philo)
{
  int             i;
  for (i = 0; i < num_philo && philos_pid[i] != philo_pid; i++);
  return i;
}

unsigned int
get_random(int *m_w, int *m_z)
{

  *m_z = 36969 * (*m_z & 65535) + (*m_z >> 16);
  *m_w = 18000 * (*m_w & 65535) + (*m_w >> 16);
  return (*m_z << 16) + *m_w;   /* 32-bit result */
}
