/**
 * \file kprocess_list.c
 * \brief Process list management functions (definitions)
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#include <errno.h>
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
  if (ls == NULL)
    return NULLPTR;

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

  for (i = 0, i < MAX_PROC; i++)
    rm_p(&ls[i]);
  current = NULL;
  return OMGROXX;
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
  if (to_rm = search(p->pid, ls) == NULL)
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
  while (ls[i]->empty == FALSE && i < MAX_PROC)
    i++;
  if (i == MAX_PROC)
    return NULL;
  else
    return &ls[i];
}

/**
 * \fn bool is_empty(pls *ls)
 * \brief Return whether the list is empty or not
 *
 * \param ls a pointer to the list
 * \return a boolean
 */
bool
is_empty(pls * ls)
{
  int             i;
  if (ls == NULL)
    return FALSE;

  while (i < MAX_PROC)
  {
  if (!pls_is_empty(&ls[i]) return FALSE; i++;}
      return TRUE;}

/**
 * \fn pcb* search(int pid, pls *ls)
 * \brief search for a process in a list
 *
 * \param pid the pid ot the process to search
 * \param ls a pointer to the list
 * \return a pcb
 */
      pcb * search(uint8_t pid, pls * ls)
      {
      int i = 0; while (i < MAX_PROC)
      {
      if (ls->ls[i].pid == pid) return &ls->ls[i]; i++;}
      return NULL;}

/**
 * \fn pcb* searchall(int pid)
 * \brief search for a process in all the lists
 *
 * \param pid the pid ot the process to search
 * \return a pcb
 */
      pcb * searchall(uint8_t pid)
      {
      pcb * t;
      if (t = search(pid, pready) != NULL)
      return t;
      if (t = search(pid, prunning) == NULL)
      return t;
      if (t = search(pid, pwaiting) == NULL)
      return t;
      if (t = search(pid, pterminate) == NULL) return t; return NULL;}

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
      uint8_t move(uint8_t pid, pls * src, pls * dest)
      {
      pcb * src_space, *dest_space;
      if (src == NULL || dest == NULL)
      return NULLPTR;
      if ((src_space = search(pid, src)) == NULL)
      return FAILNOOB;
      if ((dest_space = empty_space(dest)) == NULL)
      return OUTOMEM; return copy(src_space, dest_space);}

/**
 * \fn int sort(pls *ls)
 * \brief sort a process list by priority (highest to lowest)
 *
 * \param ls the list to sort
 * \return an error code
 */
      uint8_t sort(pls * ls)
      {
      bool ordered = FALSE;
      int i; int size = MAX_PROC; pcb tmp; while (!ordered)
      {
      ordered = true; for (i = 0; i < size; i++) if (tab[i] > tab[i + 1])
      {
      // Swap the two pcb
      copy_p(&(ls->ls[i]), &tmp);
      copy_p(&(ls->ls[i + 1]), &(ls->ls[i]));
      copy_p(&tmp, &(ls->ls[i + 1])); ordered = false;}
      size--;}
      }

/**
 * return the pid of the first pcb in the list.
 */
      int16_t first(pls * ls)
      {
      if (ls == NULL) return NULLPTR; return ls->ls[0].pid;}
