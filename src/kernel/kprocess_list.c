/**
 * \file kprocess_list.c
 * \brief Process list management functions (definitions)
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#include <string.h>
#include <errno.h>
#include "kernel.h"
#include "kprocess_list.h"
#include "kinout.h"

void            print_pls(pls * ls);    // Internal function (see the bottom of the file

/**
 * \fn int create_pls(pls *ls)
 * \brief initialize a list of pcb
 *
 * \param ls a pointer to a list
 * \return an error code
 */
	uint32_t
create_pls(pls * ls)
{
	int             i;
	kprintln("DEPRECATED: create_pls is deprecated, pleaze refers to the kprocess_list.h for new functions");
	if (ls == NULL)
		return NULLPTR;

	/** TODO: init registers ... */

	for (i = 0; i < MAXPCB; i++)
		ls->ls[i].empty = TRUE;

	ls->current = NULL;
	return OMGROXX;
}

/**
 * \fn int rm_pls(pls *ls)
 * \brief delete a list of pcb
 *
 * \param ls a pointer to a list
 * \return an error code
 */
	uint32_t
rm_pls(pls * ls)
{
	int             i;
	kprintln("DEPRECATED: rm_pls is deprecated, pleaze refers to the kprocess_list.h for new functions");
	if (ls == NULL)
		return NULLPTR;

	for (i = 0; i < MAXPCB; i++)
		rm_p(&ls->ls[i]);
	ls->current = NULL;
	return OMGROXX;
}

/**
 * \fn void rm_all_pls()
 * \brief remove all the lists and reset next_pid
 */
	void
rm_all_pls()
{
	kprintln("DEPRECATED: rm_all_pls is deprecated, pleaze refers to the kprocess_list.h for new functions");
	rm_pls(&pready);
	rm_pls(&prunning);
	rm_pls(&pwaiting);
	rm_pls(&pterminate);
	reset_next_pid();
}

/**
 * \fn void create_all_pls()
 * \brief create all the lists and reset next_pid
 */
	void
create_all_pls()
{
	kprintln("DEPRECATED: create_all is deprecated, pleaze refers to the kprocess_list.h for new functions");
	create_pls(&pready);
	create_pls(&prunning);
	create_pls(&pwaiting);
	create_pls(&pterminate);
	reset_next_pid();
}

/**
 * \fn int rm_from_pls(pcb *p, pls *ls)
 * \brief delete a pcb from a list and reorder the list
 *
 * \param pid the pid of the process to remove
 * \param ls a pointer to a list
 * \return an error code
 */
	uint32_t
rm_from_pls(int32_t pid, pls * ls)
{
	pcb            *to_rm;
	kprintln("DEPRECATED: rm_from_pls is deprecated, pleaze refers to the kprocess_list.h for new functions");
	if (ls == NULL)
		return NULLPTR;

	to_rm = search_pcb(pid, ls);

	return rm_p(to_rm);
}

/**
 * \fn pcb* empty_space(pls *ls)
 * \brief return the first empty space in a process list
 *
 * \param ls a pointer to a list
 * \return the first empty pcb or NULL if no empty pcb
 */
	pcb            *
empty_space(pls * ls)
{
	int             i = 0;
	kprintln("DEPRECATED: empty_space is deprecated, pleaze refers to the kprocess_list.h for new functions");
	if (ls == NULL)
		return NULL;
	while (ls->ls[i].empty == FALSE && i < MAXPCB)
		i++;
	if (i == MAXPCB)
		return NULL;
	else
		return &ls->ls[i];
}

/**
 * \fn bool is_empty(pls *ls)
 * \brief Return whether the list is empty or not
 *
 * \param ls a pointer to the list
 * \return a boolean
 */
	bool
pls_is_empty(pls * ls)
{
	int             i;
	kprintln("DEPRECATED: pls_is_empty is deprecated, pleaze refers to the kprocess_list.h for new functions");
	if (ls == NULL)
		return FALSE;

	while (i < MAXPCB)
	{
		if (!p_is_empty(&ls->ls[i]))
			return FALSE;
		i++;
	}
	return TRUE;
}

/**
 * \fn pcb* search_pcb(int pid, pls *ls)
 * \brief search for a process in a list
 *
 * \param pid the pid ot the process to search
 * \param ls a pointer to the list
 * \return a pcb
 */
	pcb            *
search_pcb(uint32_t pid, pls * ls)
{
	int             i = 0;
	kprintln("DEPRECATED: search_pcb is deprecated, pleaze refers to the kprocess_list.h for new functions");
	while (i < MAXPCB)
	{
		if (ls->ls[i].pid == pid)
		{
			if (ls->ls[i].empty == FALSE)
				return &ls->ls[i];
		}
		i++;
	}
	return NULL;
}

/**
 * \fn pcb* searchall(int pid)
 * \brief search for a process in all the lists
 *
 * \param pid the pid ot the process to search
 * \return a pcb
 */
	pcb            *
searchall(uint32_t pid)
{
	pcb            *t;
	kprintln("DEPRECATED: searchall is deprecated, pleaze refers to the kprocess_list.h for new functions");
	t = search_pcb(pid, &pready);
	if (t != NULL)
		return t;
	t = search_pcb(pid, &prunning);
	if (t != NULL)
		return t;
	t = search_pcb(pid, &pwaiting);
	if (t != NULL)
		return t;
	t = search_pcb(pid, &pterminate);
	if (t != NULL)
		return t;
	return NULL;
}

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
	uint32_t
move(uint32_t pid, pls * src, pls * dest)
{
	uint32_t        ret;
	pcb            *src_space, *dest_space;
	kprintln("DEPRECATED: move is deprecated, pleaze refers to the kprocess_list.h for new functions");
	if (src == NULL || dest == NULL)
		return NULLPTR;
	if ((src_space = search_pcb(pid, src)) == NULL)
		return FAILNOOB;
	if ((dest_space = empty_space(dest)) == NULL)
		return OUTOMEM;
	ret = move_p(src_space, dest_space);
	return ret;
}

/**
 * \fn int sort(pls *ls)
 * \brief sort a process list by priority (highest to lowest)
 *
 * \param ls the list to sort
 * \return an error code
 */
	uint32_t
sort(pls * ls)
{
	bool            ordered = FALSE;
	int             i;
	int             size = MAXPCB;
	pcb             tmp;
	kprintln("DEPRECATED: sort is deprecated, pleaze refers to the kprocess_list.h for new functions");
	if (ls == NULL)
		return NULLPTR;
	while (!ordered)
	{
		ordered = TRUE;
		for (i = 0; i < size; i++)
			if (ls->ls[i + 1].pri > ls->ls[i].pri)
			{
				// Swap the two pcb
				move_p(&(ls->ls[i]), &tmp);
				move_p(&(ls->ls[i + 1]), &(ls->ls[i]));
				move_p(&tmp, &(ls->ls[i + 1]));
				ordered = FALSE;
			}
		size--;
	}
	return OMGROXX;
}

/* Internal functions */
	void
print_pls(pls * ls)
{
	int             i = 0;
	char            resc[10];
	kprintln("DEPRECATED: print_pls is deprecated, pleaze refers to the kprocess_list.h for new functions");
	for (; i < MAXPCB; i++)
	{
		if (ls->ls[i].empty == FALSE)
			kprint(itos(ls->ls[i].pid, resc));
		else
			kprint("E");
		if (i != MAXPCB - 1)
			kprint(" - ");
		else
			kprintln("");
	}
}


/**
 * @brief Reset the list to his default value
 *
 * This function reset all the pcb in the list, and all the pointer to NULL
 */
void pcls_reset(pcls *ls)
{
	uint32_t i;

	ls->start = NULL;
	ls->end = NULL;
	ls->length = 0 ;

	for(i = 0; i < MAXPCB; i++)
		pcls_item_reset(&(ls->ls[i]), i);
}

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
int32_t pcls_add(pcls *ls, pcb *p)
{
	pcls_item *it, *last, *next;

	it = pcls_item_alloc(ls);

	if(!it)
		return OUTOMEM;

	pcls_item_cpy_pcb(p, it);

	/*
	 * Found the position of the element
	 */
	if (ls->start == NULL) //Nothing in the list
		ls->start = it;
	else
	{
		last = ls->start;
		next = last->next;

		while ( pcb_get_pri(&(next->p)) > pcb_get_pri(&(it->p))
				&& next->next != NULL)
		{
			last = next;
			next = next->next ;
		}

		if (pcb_get_pri(&(next->p)) <= pcb_get_pri(&(it->p)))
		{
			last->next = it ;
			it->next = next;
		}
		else
			next->next = it;
	}
	ls->length++;

	return OMGROXX;
}

/**
 * @brief Delete a pcb from a list. (Reset it to his default value)
 */
	uint32_t
pcls_delete_pcb(pcls *ls, pcb *p)
{
	pcls_item *next, *last ;

	if (ls->length == 0)
		return NOTFOUND;

	last = ls->start;
	next = last->next;

	while(pcb_get_pid(&(next->p)) != pcb_get_pid(p)
			&& !next->next)
	{
		last = next;
		next = next->next;
	}

	if (pcb_get_pid(&(next->p)) == pcb_get_pid(p))
	{
		last->next = next->next;
		pcls_item_reset(next, next->item_id);
	}
	else
		return NOTFOUND;

	ls->length--;

	return OMGROXX;
}

/**
 * @brief Move a PCB from a list to an other
 */
int32_t
pcls_move_pcb(pcls *src, pcls *dest, pcb *p)
{
	int32_t e;
	pcls_item  *it_src;

	e = OMGROXX;

	it_src = pcls_search_pcb(src, p);

	if(!it_src)
		return NOTFOUND;

	/*
	 * we add the pcb in the list
	 */
	e = pcls_add(dest, p); 
	
	if(e != OMGROXX)
		return e;

	/*
	 * We delete the old pcb
	 */
	e = pcls_delete_pcb(src, p);

	return e;
}

/**
 * @brief Search a pcb in a list and return the pcls_item associated
 * @param a list
 * @param the pcb to found
 * @return NULL if not found, the pcls_item otherwise
 */
pcls_item*
pcls_search_pcb(pcls *ls, pcb *p)
{
	pcls_item *next, *last ;

	if (ls->length == 0)
		return NULL;

	last = ls->start;
	next = last->next;

	while(pcb_get_pid(&(next->p)) != pcb_get_pid(p)
			&& !next->next)
	{
		last = next;
		next = next->next;
	}

	if (pcb_get_pid(&(next->p)) == pcb_get_pid(p))
		return next;

	else
		return NULL;
}

/**
 * @brief Search a pid in a list and return the pcls_item associated
 * @param a list
 * @param the pid to found
 * @return NULL if not found, the pcls_item otherwise
 */
pcls_item*
pcls_search_pid(pcls *ls, int32_t pid)
{
	pcls_item *next, *last ;

	if (ls->length == 0)
		return NULL;

	last = ls->start;
	next = last->next;

	while(pcb_get_pid(&(next->p)) != pid
			&& !next->next)
	{
		last = next;
		next = next->next;
	}

	if (pcb_get_pid(&(next->p)) == pid)
		return next;

	else
		return NULL;
}

/**
 * @brief Reset an list element to it's default value.
 */
	void
pcls_item_reset(pcls_item *it, uint32_t id)
{
	it->item_id = id;
	it->next = NULL;
	pcb_reset(&(it->p));
}

/**
 * @brief Return the first empty pcls_item in the static array of pcls
 * @param a list
 * @return the adress or NULL if no space
 */
pcls_item * pcls_item_alloc(pcls *ls)
{
	uint32_t i;

	for( i = 0; i < MAXPCB; i++)
		if (pcb_get_empty(&((ls->ls[i]).p)))
			return &(ls->ls[i]);

	return NULL;
}

/**
 * @brief Copy a pcb in a pcls_item
 * @param the source pcb
 * @param the destination item
 */
void pcls_item_cpy_pcb(pcb *p, pcls_item *it)
{
	pcb_cpy(p, &(it->p));
}
