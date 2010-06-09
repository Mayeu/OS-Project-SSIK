/**
 * \file philosopher.c
 * \brief blabla
 * \author Adrien Forest
 * \version 0.1
 * \date 26 mai 2010
 */


#ifndef __PHILOSOPHER_H
#define __PHILOSOPHER_H

int             waiter(int argc, char *argv[]);
int             philosopher(int argc, char *argv[]);
int             dining_philosopher(int argc, char *argv[]);
bool            can_take_fork(int index, int philo_pid, int *forks,
                              int *forks_taken, int nb_philo, int rlflag);
int             random(int a, int b);

#endif //__PHILOSOPHER_H
