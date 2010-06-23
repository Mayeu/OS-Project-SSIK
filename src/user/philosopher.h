/**
 * \file philosopher.c
 * \brief blabla
 * \author Adrien Forest
 * \version 0.1
 * \date 26 mai 2010
 */


#ifndef __PHILOSOPHER_H
#define __PHILOSOPHER_H

/**
 * Process used as a master for the philosoophers to manange the communications and the forks
 * \param argc nombre d'arguments
 * \param argv arguments
 */
void            waiter(int argc, char *argv[]);

/**
 * Process used as a philosoopher who wants to take two forks and eat.
 * \param argc nombre d'arguments
 * \param argv arguments
 */
void            philosopher(int argc, char *argv[]);

/**
 * Program that implements the dining philospher problem
 * \param argc nombre d'arguments
 * \param argv arguments
 */
void            dining_philosopher(int argc, char *argv[]);

/**
 * Generate a random number with two numbers
 * \param argc nombre d'arguments
 * \param argv arguments
 */
int             random(int a, int b);

/**
 * Specifies whether the process can take its left fork or not.
 * \param index index of the philos
 * \param phio_pid pid of the philo
 * \param fork pointer to the fork
 * \param forks_taken
 * \param num_philo
 */
bool            left_fork(int index, int philo_pid, int *fork,
                          int *forks_taken, int num_philo);

/**
 * Specifies whether the process can take its right fork or not.
 * \param index index of the philos
 * \param phio_pid pid of the philo
 * \param fork pointer to the fork
 * \param forks_taken
 * \param num_philo
 */
bool            right_fork(int index, int philo_pid, int *fork,
                           int *forks_taken);

#endif //__PHILOSOPHER_H
