/**
 * \file kprocess_list.h
 * \brief Process list management functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 19 Avril 2010
 *
 */

#ifndef __KPROCESS_LIST_H
#define __KPROCESS_LIST_H

#include <stdlib.h>
#include "kprocess.h"
#include "kpcb.h"

/**
 * \struct pls
 * \brief List of processes.
 *
 */
typedef struct _PLS
{
  pcb            *start;        /*!< First pcb of the list */
  uint32_t        length;       /*!< number of processes currently in the list */
} pls;

/**
 * \brief Reset the list to his default value
 *
 * This function reset all the pcb in the list, and all the pointer to NULL
 * \param a pointer to the pcb to add to the list
 */
void            pls_reset(pls * ls);

/**
 * \brief Add an element to the list.
 *
 * The list is always sorted by priority (highest to lowest).
 * The added pcb will be a copy of the passed pcb
 *
 * \param ls the list where we add an element
 * \param a pointer to the pcb to add to the list
 * \return OMGROXX if everything goes well.
 */
int32_t         pls_add(pls * ls, pcb * p);

/**
 * \brief Delete a pcb from a list. (Reset it to his default value)
 *
 * \param ls the list where we delete an element
 * \param a pointer to the pcb to delete from the list
 * \return OMGROXX if everything goes well.
 */
uint32_t        pls_delete_pcb(pcb * p);

/**
 * \brief Move a PCB from a list to an other
 *
 * 
 *
 * \param a pointer to the pcb to move
 * \param dest the destination list
 * \return OMGROXX if everything goes well.
 */
int32_t         pls_move_pcb(pcb * p, pls * dest);

/**
 * @brief Search a pid in a list and return the associated pcb
 * 
 * @param a list
 * @param the pid to found
 * @return NULL if not found, the pls_item otherwise
 */
pcb            *pls_search_pid(pls * ls, uint32_t pid);

#endif
