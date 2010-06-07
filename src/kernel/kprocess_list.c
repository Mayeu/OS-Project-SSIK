/**
 * \file kprocess_list.c
 * \brief Process list management functions (definitions)
 */

#include <string.h>
#include <errno.h>
#include "kprocess_list.h"
#include "debug.h"

/*
 * Private function decleration
 */
void            pls_reset_all_pcb(pcb * p);

/*
 * Public functions definition
 */

/**
 * @private
 * @brief Reset the list to his default value
 *
 * This function reset all the pcb in the list, and all the pointer to NULL
 */
void
pls_reset(pls * ls)
{
  /*
   * Delete all the pcb element in the list
   */
  pls_reset_all_pcb(ls->start);

  /*
   * Delete the element of the list
   */
  ls->start = NULL;
  ls->length = 0;
}

/**
 * @private
 * @brief Add an element to the list.
 *
 * The list is always sorted by priority (highest to lowest).
 * The added pcb will be added at the end of the pcb of equivalent priority
 *
 * @param The the list where we add an element
 * @param a pointer to the pcb to add to the list
 * @return OMGROXX if everything goes well.
 */
int32_t
pls_add(pls * ls, pcb * p)
{
  pcb            *next;

  if (!ls || !p)
    return NULLPTR;

  /*
   * Found the position of the element
   */
  if (ls->start == NULL)
  {
    /*
     * Nothing in the list, we add the pcb at the begin of the list
     */
    ls->start = p;
    pcb_set_next(p, NULL);
    pcb_set_prev(p, NULL);
  }

  /*
   * We search the good position in the list
   */
  else
  {
    next = ls->start;

    /*
     * Looking for the position
     */
    while (pcb_get_pri(next) >= pcb_get_pri(p) && pcb_get_next(next) != NULL)
    {
      next = pcb_get_next(next);
    }

    /*
     * are we at the end of the list ?
     */
    if (pcb_get_pri(next) < pcb_get_pri(p))
    {
      //kprintln("I should pass here");
      /*
       * no
       */
      if (ls->start == next)
      {
        /*
         * The pcb is added in the front of the list
         */
        ls->start = p;
        pcb_set_prev(p, NULL);
        pcb_set_prev(next, p);
        pcb_set_next(p, next);
      }
      else
      {
        pcb_set_next(pcb_get_prev(next), p);
        pcb_set_prev(p, pcb_get_prev(next));
        pcb_set_next(p, next);
        pcb_set_prev(next, p);
      }
    }

    else
    {
      /*
       * Yes
       */
      pcb_set_next(next, p);
      pcb_set_next(p, NULL);
      pcb_set_prev(p, next);
    }


  }

  pcb_set_head(p, ls);
  ls->length++;

  return OMGROXX;
}

/**
 * @private
 * @brief Delete a pcb from a list. (Reset it to his default value)
 */
uint32_t
pls_delete_pcb(pcb * p)
{

  if (!p)
    return NULLPTR;

  /*
   * If the pcb is on the front of the list
   */
  if (pcb_get_prev(p) == NULL)
  {
    pcb_get_head(p)->start = pcb_get_next(p);
    if (pcb_get_next(p) != NULL)
      pcb_set_prev(pcb_get_next(p), NULL);
  }
  /*
   * otherwise
   */
  else
  {
    pcb_set_next(pcb_get_prev(p), pcb_get_next(p));
    if (pcb_get_next(p) != NULL)
      pcb_set_prev(pcb_get_next(p), pcb_get_prev(p));
  }

  /*
   * Shorting the list
   */
  pcb_get_head(p)->length--;

  /*
   * Reset the pcb
   */
  pcb_reset(p);

  return OMGROXX;
}

/**
 * @private
 * @brief Move a PCB from a list to an other
 */
int32_t
pls_move_pcb(pcb * p, pls * dest)
{
  if (!p || !dest)
    return NULLPTR;

  /*
   * First we recreate the source list
   */

  /*
   * If the pcb is on the front of the list
   */
  if (pcb_get_prev(p) == NULL)
  {
    pcb_get_head(p)->start = pcb_get_next(p);
    if (pcb_get_next(p) != NULL)
      pcb_set_prev(pcb_get_next(p), NULL);
  }
  /*
   * otherwise
   */
  else
  {
    pcb_set_next(pcb_get_prev(p), pcb_get_next(p));
    if (pcb_get_next(p) != NULL)
      pcb_set_prev(pcb_get_next(p), pcb_get_prev(p));
  }

  /*
   * Shorting the list
   */
  pcb_get_head(p)->length--;

  /*
   * Now we add the pcb to his new list
   */
  return pls_add(dest, p);
}

/**
 * @private
 * @brief Search a pid in a list and return the pls_item associated
 * @param a list
 * @param the pid to found
 * @return NULL if not found, the pls_item otherwise
 */
pcb            *
pls_search_pid(pls * ls, uint32_t pid)
{
  pcb            *p;

  if (ls->length == 0)
    return NULL;

  p = ls->start;

  while (pcb_get_pid(p) != pid && pcb_get_next(p) != NULL)
    p = pcb_get_next(p);

  if (pcb_get_pid(p) == pid)
    return p;

  else
    return NULL;
}

/*
 * Private functions definition
 */

/**
 * @brief This function is recursively called to reset all pcb in a list
 *
 * This private function go to the last element of the list and start to
 * reset them recursively
 *
 * @param the pcb to reset
 */
void
pls_reset_all_pcb(pcb * p)
{
  if (!p)
    return;

  pls_reset_all_pcb(pcb_get_next(p));

  pcb_reset(p);

  return;
}
