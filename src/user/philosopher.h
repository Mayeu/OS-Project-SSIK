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
int             waiter(int argc, char *argv[]);

/**
 * Process used as a philosoopher who wants to take two forks and eat.
 * \param argc nombre d'arguments
 * \param argv arguments
 */
int             philosopher(int argc, char *argv[]);

/**
 * Program that implements the dining philospher problem
 * \param argc nombre d'arguments
 * \param argv arguments
 */
int             dining_philosopher(int argc, char *argv[]);

/**
 * Specifies whether the process can take its left/right fork or not.
 * \param argc nombre d'arguments
 * \param argv arguments
 */
bool            can_take_fork(int index, int philo_pid, int *forks,
                              int *forks_taken, int nb_philo, int rlflag);

/**
 * Generate a random number with two numbers
 * \param argc nombre d'arguments
 * \param argv arguments
 */
int             random(int a, int b);

#endif //__PHILOSOPHER_H
