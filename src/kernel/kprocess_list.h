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
 * @brief An element of a linked list of process
 *
 * This linked list will not be dynamic but in a bounded array. 
 */

typedef struct linked_item
{
	uint32_t item_id; /*!< use because we are in a bounded array, to know the adress of the element */
	pcb p;
	struct linked_item *next;
} pcls_item;

/**
 * \struct pls
 * \brief List of processes.
 *
 */
typedef struct
{
	pcls_item ls[MAXPCB];
	pcls_item *start;
	pcls_item *end;
	uint32_t length;
} pcls;

/**
 * \struct pls
 * \brief List of processes.
 *
 */
typedef struct
{
	pcb ls[MAXPCB];
	pcb *current;
} pls;


/**
 * \fn int create_pls(pls *ls)
 * \brief initialize a list of pcb
 *
 * \param ls a pointer to a list
 * \return an error code
 */
uint32_t        create_pls(pls * ls);

/**
 * \fn int rm_pls(pls *ls)
 * \brief delete a list of pcb
 *
 * \param ls a pointer to a list
 * \return an error code
 */
uint32_t        rm_pls(pls * ls);

/**
 * \fn void rm_all_pls()
 * \brief remove all the lists and reset next_pid
 */
void            rm_all_pls();

/**
 * \fn void create_all_pls()
 * \brief create all the lists and reset next_pid
 */
void            create_all_pls();

/**
 * \fn int rm_from_pls(pcb *p, pls *ls)
 * \brief delete a pcb from a list and reorder the list
 *
 * \param p the pcb to remove
 * \param ls a pointer to a list
 * \return an error code
 */
uint32_t        rm_from_pls(int32_t pid, pls * ls);

/**
 * \fn pcb* empty_space(pls *ls)
 * \brief return the first empty space in a process list
 *
 * \param ls a pointer to a list
 * \return the first empty pcb
 */
pcb            *empty_space(pls * ls);

/**
 * \fn bool pls_is_empty(pls *ls)
 * \brief Return whether the list is empty or not
 *
 * \param ls a pointer to the list
 * \return a boolean
 */
bool            pls_is_empty(pls * ls);

/**
 * \fn pcb* search(int pid, pls *ls)
 * \brief search for a process in a list
 *
 * \param pid the pid ot the process to search
 * \param ls a pointer to the list
 * \return a pcb
 */
pcb            *search_pcb(uint32_t pid, pls * ls);

/**
 * \fn pcb* searchall(int pid)
 * \brief search for a process in all the lists
 *
 * \param pid the pid ot the process to search
 * \return a pcb
 */
pcb            *searchall(uint32_t pid);

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
uint32_t        move(uint32_t pid, pls * src, pls * dest);

/**
 * \fn int sort(pls *ls)
 * \brief sort a process list by priority (highest to lowest)
 *
 * \param ls the list to sort
 * \return an error code
 */
uint32_t        sort(pls * ls);

/**
 * @brief Reset the list to his default value
 *
 * This function reset all the pcb in the list, and all the pointer to NULL
 */
void pcls_reset(pcls *ls);

/**
 * @brief Add an element to the list.
 *
 * The list is always sorted by priority (highest to lowest).
 * The added pcb will be a copy of the passed pcb
 *
 * @param The the list where we add an element
 * @param a pointer to the pcb to add to the list
 * @return OMGROXX if everything goes well.
 */
int32_t pcls_add(pcls *ls, pcb *p);

/**
 * @brief Delete a pcb from a list. (Reset it to his default value)
 */
	uint32_t
pcls_delete_pcb(pcls *ls, pcb *p);

/**
 * @brief Move a PCB from a list to an other
 */
int32_t
pcls_move_pcb(pcls *src, pcls *dest, pcb *p);

/**
 * @brief Reset an list element to it's default value.
 */
void
pcls_item_reset(pcls_item *it, uint32_t id);

/**
 * @brief Search a pcb in a list and return the pcls_item associated
 * @param a list
 * @param the pcb to found
 * @return NULL if not found, the pcls_item otherwise
 */
pcls_item*
pcls_search_pcb(pcls *ls, pcb *p);

/**
 * @brief Search a pid in a list and return the pcls_item associated
 * @param a list
 * @param the pid to found
 * @return NULL if not found, the pcls_item otherwise
 */
pcls_item*
pcls_search_pid(pcls *ls, int32_t pid);

/**
 * @brief Return the first empty pcls_item in the static array of pcls
 * @param a list
 * @return the adress or NULL if no space
 */
pcls_item * pcls_item_alloc(pcls *ls);

/**
 * @brief Copy a pcb in a pcls_item
 * @param the source pcb
 * @param the destination item
 */
	void pcls_item_cpy_pcb(pcb *p, pcls_item *it);

#endif
