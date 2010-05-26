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
#include "kprogram.h"

void            print_pls(pls * ls);    // Internal function (see the bottom of the file

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
	pcls_item *it, *last_it, *next_it;

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
		last_it = NULL;
		next_it = ls->start;

		/*
		 * Looking for the position
		 */
		while ( pcb_get_pri(&(next_it->p)) > pcb_get_pri(&(it->p))
				&& next_it->next != NULL)
		{
			last_it = next_it;
			next_it = next_it->next ;
		}

		/*
		 * are we at the end of the list ?
		 */
		if (pcb_get_pri(&(next_it->p)) <= pcb_get_pri(&(it->p)))
		{
			/*
			 * no
			 */
			if(last_it != NULL)
				last_it->next = it ;
			else
				ls->start = it;

			it->next = next_it;
		}
		else
			/*
			 * yes
			 */
			next_it->next = it;
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

	last = NULL;
	next = ls->start;

	while(pcb_get_pid(&(next->p)) != pcb_get_pid(p)
			&& !next->next)
	{
		last = next;
		next = next->next;
	}

	if (pcb_get_pid(&(next->p)) == pcb_get_pid(p))
	{
		if (last == NULL)
			ls->start = next->next;
		else
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
 *
 * Use the pid to found the good pcb
 *
 * @param a list
 * @param the pcb to found
 * @return NULL if not found, the pcls_item otherwise
 */
pcls_item*
pcls_search_pcb(pcls *ls, pcb *p)
{
	pcls_item *last ;

	if (ls->length == 0)
		return NULL;

	last = ls->start;

	while(pcb_get_pid(&(last->p)) != pcb_get_pid(p)
			&& last->next)
		last = last->next;

	if (pcb_get_pid(&(last->p)) == pcb_get_pid(p))
		return last;

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
pcls_search_pid(pcls *ls, uint32_t pid)
{
	pcls_item *last ;

	if (ls->length == 0)
		return NULL;

	last = ls->start;

	while(pcb_get_pid(&(last->p)) != pid
			&& last->next != NULL)
		last = last->next;

	if (pcb_get_pid(&(last->p)) == pid)
		return last;

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

/*
 * Deprecated
 */

// Internal function (see the bottom of the file)
int32_t get_next_pid(uint32_t * npid);
void print_pls(pls *ls);

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
 * initialize a pcb with all the needed value, add it to
 * the ready queue, and ask for a long term scheduling.
 */
uint32_t
//create_proc(char *name, uint32_t prio, int32_t params[4])
create_proc(char *name, uint32_t prio, char **params)
{
  pcb            *p;
	uint32_t new_pid;
	prgm *prog;
	uint32_t supervisor;
  if (name == NULL)
    return NULLPTR;
  if (prio > MAX_PRI || prio < MIN_PRI)
    return INVARG;
  if (params == NULL)
    return NULLPTR;
  p = empty_space(&pready);     // search for an empty pcb in the ready queue
  if (p == NULL)
 	return OUTOMEM;
    if (get_next_pid(&new_pid) != OMGROXX)
	return OUTOPID;

	prog = search_prgm(name); // search for the specified program
	if (prog == NULL)                                                               
		return INVARG;

	/* The supervisor is the process that has requested 
	 * The create_proc function and then it is the
	 * process that is currently running.
	 */
	if (prunning.current == NULL)
		supervisor = -1;
	else
		supervisor = prunning.current->pid;

	create_pcb(p, new_pid, name, prog->address, supervisor, prio, params);

	return new_pid;
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


/**
 * add a pid to the supervise list of a process;
 */

uint32_t
add_psupervised(pcb * p, uint32_t pid)
{
  pcb            *supervised;
  if (p == NULL)
    return NULLPTR;
  if ((supervised = searchall(pid)) == NULL)
    return INVARG;
  if (supervised->supervisor != -1)
    return INVARG;
  if (!is_already_supervised(p, pid))
  {
    /* look for the first empty (pid = -1) position */
    int             pos = search_psupervised(p, -1);
    if (pos != -1)
    {
      p->supervised[pos] = pid; // add the pid as a new supervised process
      supervised->supervisor = p->pid;  // change the supervisor of the supervised process
      return OMGROXX;
    }
    else
      return FAILNOOB;
  }
  return OMGROXX;
}

/**
 * remove a pid from the supervised list of a process
 */
uint32_t
rm_psupervised(pcb * p, uint32_t pid)
{
  if (p == NULL)
    return NULLPTR;
  int             pos = search_psupervised(p, pid);
  if (pos != -1)
  {
    pcb            *supervised = searchall(pid);
    p->supervised[pos] = -1;    // remove the supervised process
    supervised->supervisor = -1;        // remove the supervisor of the previously supervised process
    return OMGROXX;
  }
  else
    return INVARG;
}

/**
 * add a pid to the supervisor list of a process;
 */
uint32_t
add_psupervisor(pcb * p, uint32_t pid)
{
  if (p == NULL)
    return NULLPTR;
  if (p->supervisor != -1)
    return INVARG;

  return add_psupervised(searchall(pid), p->pid);
}

/**
 * remove the pid from the supervisor a process.
 */
uint32_t
rm_psupervisor(pcb * p, uint32_t pid)
{
  if (p == NULL)
    return NULLPTR;

  rm_psupervised(searchall(pid), p->pid);
  return OMGROXX;

}

 /**
 * reset the next_pid to 0
 */
void
reset_next_pid()
{
  next_pid = 0;
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
 * set nmid to the next available value of mid
 */
/*
int32_t get_next_pid(uint32_t * pmid)
{
  int             init = next_pid;
  *pmid = next_pid;
	next_pid++;
  while (searchall(next_pid) != NULL)
  {
    next_pid++;
    if (next_pid == init)
      return OUTOPID;
  }
  return OMGROXX;
}

void print_pls(pls *ls)
{
  int             i = 0;
  char            resc[10];
  for (; i < MAX_PROC; i++)
  {
    if (ls->ls[i].empty == FALSE)
      kprint(itos(ls->ls[i].pid, resc));
    else
      kprint("E");
    if (i != MAX_PROC - 1)
      kprint(" - ");
    else
      kprintln("");
  }
}*/
