/**
 * \file kprocess_list.c
 * \brief Process list management functions (definitions)
 */

#include <string.h>
#include <errno.h>
#include "kernel.h"
#include "kprocess_list.h"
#include "kinout.h"
#include "kprogram.h"
#include "debug.h"

//void            print_pls(pls * ls);    // Internal function (see the bottom of the file

/*
 * Private function decleration
 */
void  pls_reset_all_pcb(pcb p);

/*
 * Public functions definition
 */

/**
 * @brief Reset the list to his default value
 *
 * This function reset all the pcb in the list, and all the pointer to NULL
 */
void
pls_reset(pls * ls)
{
  uint32_t        i;

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
	pcb *next, *prev;

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
    while (pcb_get_pri(next) => pcb_get_pri(p)
           && pcb_get_next(next) != NULL)
    {
      next = pcb_get_next(next);
    }

    /*
     * are we at the end of the list ?
     */
	 if(pcb_get_next(next) == NULL)
	 {
		 /*
		  * Yes
		  */
		 pcb_set_next(next, p);
		 pcb_set_next(p, NULL);
		 pcb_set_prev(p, next);
	 }

	 else 
    {
      /*
       * no
       */
		 if(ls->start == next)
		 {
			 /*
			  * The pcb is added in the front of the list
			  */
			 ls->start = p ;
			 pcb_set_prev(p, NULL);
			 pcb_set_prev(next, p);
			 pcb_set_next(p, next);
		 }
		 else
		 {
			 pcb_set_next(p, pcb_get_next(next));
			 pcb_set_prev(pcb_get_next(next), p);
			 pcb_set_next(next, p);
			 pcb_set_prev(p, next);
		 }
    }
    
	 pcb_set_head(p, ls);
  ls->length++;

  return OMGROXX;
}

/**
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
		pcb_set_prev(pcb_get_next(p), NULL);
  }
  /*
	* otherwise
	*/
  else
  {
	  pcb_set_next(pcb_get_prev(p), pcb_get_next(p));
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
 * @brief Move a PCB from a list to an other
 */
int32_t
pls_move_pcb(pcb * p, pls * dest)
{
  int32_t         e;
  pls_item      *it_src;

  e = OMGROXX;

  it_src = pls_search_pcb(src, p);

  if (!it_src)
    return NOTFOUND;
  
  if (!p)
    return NULLPTR;

  /*
	* If the pcb is on the front of the list
	*/
  if (pcb_get_prev(p) == NULL)
  {
		pcb_get_head(p)->start = pcb_get_next(p);
		pcb_set_prev(pcb_get_next(p), NULL);
  }
  /*
	* otherwise
	*/
  else
  {
	  pcb_set_next(pcb_get_prev(p), pcb_get_next(p));
	  pcb_set_prev(pcb_get_next(p), pcb_get_prev(p));
  }

  /*
	* Shorting the list
	*/
  pcb_get_head(p)->length--;



  /*
   * we add the pcb in the list
   */
  e = pls_add(dest, p);

  if (e != OMGROXX)
    return e;

  /*
   * We delete the old pcb
   */
  e = pls_delete_pcb(src, p);

  return e;
}

/**
 * @brief Search a pcb in a list and return the pls_item associated
 *
 * Use the pid to found the good pcb
 *
 * @param a list
 * @param the pcb to found
 * @return NULL if not found, the pls_item otherwise
 */
pls_item      *
pls_search_pcb(pls * ls, pcb * p)
{
  pls_item      *last;

  if (ls->length == 0)
    return NULL;

  last = ls->start;

  while (pcb_get_pid(&(last->p)) != pcb_get_pid(p) && last->next)
    last = last->next;

  if (pcb_get_pid(&(last->p)) == pcb_get_pid(p))
    return last;

  else
    return NULL;
}

/**
 * @brief Search a pid in a list and return the pls_item associated
 * @param a list
 * @param the pid to found
 * @return NULL if not found, the pls_item otherwise
 */
pls_item      *
pls_search_pid(pls * ls, uint32_t pid)
{
  pls_item      *last;

  if (ls->length == 0)
    return NULL;

  last = ls->start;

  while (pcb_get_pid(&(last->p)) != pid && last->next != NULL)
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
pls_item_reset(pls_item * it, uint32_t id)
{
  it->item_id = id;
  it->next = NULL;
  pcb_reset(&(it->p));
}

/**
 * @brief Return the first empty pls_item in the static array of pls
 * @param a list
 * @return the adress or NULL if no space
 */
pls_item      *
pls_item_alloc(pls * ls)
{
  uint32_t        i;

  for (i = 0; i < MAXPCB; i++)
    if (pcb_get_empty(&((ls->ls[i]).p)))
      return &(ls->ls[i]);

  return NULL;
}

/**
 * @brief Copy a pcb in a pls_item
 * @param the source pcb
 * @param the destination item
 */
void
pls_item_cpy_pcb(pcb * p, pls_item * it)
{
  pcb_cpy(p, &(it->p));
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
void  pls_reset_all_pcb(pcb p)
{
	if (!p)
		return;

	pls_reset_all_pcb(pcb_get_next(p));

	pcb_reset(p);

	return;
}

