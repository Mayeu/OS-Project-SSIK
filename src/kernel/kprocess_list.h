/**
 * \file kprocess_list.h
 * \brief Process list management functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 19 Avril 2010
 *
 */

#IFNDEF __KPROCESS_LIST_H
#DEFINE __KPROCESS_LIST_H

#include "kprocess.h"

#DEFINE MAX_PROC 10             /* max number of processes in a list. */

/**
 * \struct pls
 * \brief List of processes.
 *
 */
typedef struct
{
  pcb             ls[MAX_PROC]; /*!< list of processes. */
  pcb            *current;      /*!< the current process in the list. */
} pls;

/**
 * \fn int create_pls(pls *ls)
 * \brief initialize a list of pcb
 *
 * \param ls a pointer to a list
 * \return an error code
 */
int             create_pls(pls * ls);

/**
 * \fn int rm_ls(pls *ls)
 * \brief delete a list of pcb
 *
 * \param ls a pointer to a list
 * \return an error code
 */
int             rm_ls(pls * ls);

/**
 * \fn int rm_from_ls(pcb *p, pls *ls)
 * \brief delete a pcb from a list and reorder the list
 *
 * \param p the pcb to remove
 * \param ls a pointer to a list
 * \return an error code
 */
int             rm_from_ls(pcb * p, pls * ls);

/**
 * \fn pcb* empty_space(pls *ls)
 * \brief return the first empty space in a process list
 *
 * \param ls a pointer to a list
 * \return the first empty pcb
 */
pcb            *empty_space(pls * ls);

/**
 * \fn bool is_empty(pls *ls)
 * \brief Return whether the list is empty or not
 *
 * \param ls a pointer to the list
 * \return a boolean
 */
bool            is_empty(pcb * p);

/**
 * \fn pcb* search(int pid, pls *ls)
 * \brief search for a process in a list
 *
 * \param pid the pid ot the process to search
 * \param ls a pointer to the list
 * \return a pcb
 */
pcb            *search(int pid, pls * ls);

/**
 * \fn pcb* searchall(int pid)
 * \brief search for a process in all the lists
 *
 * \param pid the pid ot the process to search
 * \return a pcb
 */
pcb            *searchall(int pid);

/**
 * \fn int move(int pid, pls *src, pls *dest)
 * \brief move a process from a list to another (will search to ensure that the
 *			pcb is in the list)
 *
 * \param pid the pid ot the process to move
 * \param src the source list
 * \param dest the destination list
 * \return an error code
 */
int             move(int pid, pls * src, pls * dest);

/**
 * \fn int sort(pls *ls)
 * \brief sort a process list by priority (highest to lowest)
 *
 * \param ls the list to sort
 * \return an error code
 */
int             sort(pls * ls);