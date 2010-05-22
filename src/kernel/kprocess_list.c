/**
 * \file kprocess_list.c
 * \brief Process list management functions (definitions)
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#include <errno.h>
#include "kernel.h"
#include "kprocess_list.h"

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
  if (ls == NULL)
    return NULLPTR;

                                                                                                                        /** TODO: init registers ... */

  for (i = 0; i < MAX_PROC; i++)
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
  if (ls == NULL)
    return NULLPTR;

  for (i = 0; i < MAX_PROC; i++)
    rm_p(&ls->ls[i]);
  ls->current = NULL;
  return OMGROXX;
}

/**
 * \fn void rm_all_pls()
 * \brief remove all the lists and reset next_pid
 */
void rm_all_pls()
{
	rm_pls(&pready);
	rm_pls(&prunning);
	rm_pls(&pwaiting);
	rm_pls(&pterminate);
	next_pid = 0;
}

/**
 * \fn void create_all_pls()
 * \brief create all the lists and reset next_pid
 */
void create_all_pls()
{
	create_pls(&pready);
	create_pls(&prunning);
	create_pls(&pwaiting);
	create_pls(&pterminate);
	next_pid = 0;
}

/**
 * \fn int rm_from_pls(pcb *p, pls *ls)
 * \brief delete a pcb from a list and reorder the list
 *
 * \param p the pcb to remove
 * \param ls a pointer to a list
 * \return an error code
 */
uint32_t
rm_from_pls(pcb * p, pls * ls)
{
  pcb            *to_rm;
  if (p == NULL || ls == NULL)
    return NULLPTR;
  to_rm = search_pcb(p->pid, ls);
  if (to_rm == NULL)
    return FAILNOOB;

  to_rm->empty = TRUE;
  return OMGROXX;
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
  if (ls == NULL)
    return NULL;
  while (ls->ls[i].empty == FALSE && i < MAX_PROC)
    i++;
  if (i == MAX_PROC)
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
  if (ls == NULL)
    return FALSE;

  while (i < MAX_PROC)
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
  while (i < MAX_PROC)
  {
    if ((ls->ls[i].pid == pid) && (ls->ls[i].empty == FALSE))
      return &ls->ls[i];
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
  t = search_pcb(pid, &pready);
  if (t != NULL)
    return t;
  t = search_pcb(pid, &prunning);
  if (t == NULL)
    return t;
  t = search_pcb(pid, &pwaiting);
  if (t == NULL)
    return t;
  t = search_pcb(pid, &pterminate);
  if (t == NULL)
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
  if (src == NULL || dest == NULL)
    return NULLPTR;
  if ((src_space = search_pcb(pid, src)) == NULL)
    return FAILNOOB;
  if ((dest_space = empty_space(dest)) == NULL)
    return OUTOMEM;
  ret = copy_p(src_space, dest_space);
  src_space->empty = TRUE;
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
  int             size = MAX_PROC;
  pcb             tmp;
  if (ls == NULL)
    return NULLPTR;
  while (!ordered)
  {
    ordered = TRUE;
    for (i = 0; i < size; i++)
      if (ls->ls[i].pri > ls->ls[i + 1].pri)
      {
        // Swap the two pcb
        copy_p(&(ls->ls[i]), &tmp);
        copy_p(&(ls->ls[i + 1]), &(ls->ls[i]));
        copy_p(&tmp, &(ls->ls[i + 1]));
        ordered = FALSE;
      }
    size--;
  }
  return OMGROXX;
}

/**
 * return the pid of the first pcb in the list.
 */
int32_t
first(pls * ls)
{
  if (ls == NULL)
    return NULLPTR;
  return ls->ls[0].pid;
}
