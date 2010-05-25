/**
 * \file kmsg.c
 * \brief Message related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 23 mai 2010
 *
 */

#include <errno.h>
#include "kmsg.h"
#include "kprocess.h"
#include "kprocess_list.h"
#include "kernel.h"

static uint32_t next_mid = 0;

/**
 * Create the msg object.
 */
int32_t send_msg(uint32_t mid, uint32_t sdr_pid,	uint32_t recv_pid, uint32_t pri, void *data)
{
	pcb *receiver;
	msg *m;
	if(data == NULL)
		return NULLPTR;
	if(pri >= MIN_PRI && pri <= MAX_PRI)
		return INVPRI;
	receiver = searchall(sdr_pid);
	if(receiver == NULL)
		return INVARG;
	m = empty_msg_space(receiver->messages);
	if(m == NULL)
		return OUTOMEM;
	if(get_next_mid(&m->mid) != OMGROXX)
		return OUTOPID;
	m->sdr_pid =  sdr_pid;
	m->recv_pid =  recv_pid;
	m->pri =  pri;
	m->empty =  FALSE;
	m->data =  data;
	return 0;
}

/**
 * search the first empty space of the list
 */
msg *empty_msg_space(msg *ls)
{
	int             i = 0;
  if (ls == NULL)
    return NULL;
  while (ls[i].empty == FALSE && i < MAX_MSG)
    i++;
  if (i == MAX_MSG)
    return NULL;
  else
    return &ls[i];
}
/**
 * set nmid to the next available value of mid
 */
int32_t get_next_mid(uint32_t * nmid)
{
  int             init = next_mid;
  while (search_msg_all(next_mid) != NULL)
  {
    next_mid++;
    if (next_mid == init)
      return OUTOMID;
  }
  *nmid = next_mid;
  return OMGROXX;

}

/**
 * Search for the message identified by mid in the list ls
 */
msg *search_msg(uint32_t mid, msg *ls)
{
	int j;
	for(j = 0 ; j<MAX_MSG ; j++)
	{
		if(ls[j].mid == mid && ls[j].empty == FALSE)
			return &ls[j];
	}
	return NULL;
}

/**
 * Search for the message identified by mid in all lists
 */
msg *search_msg_all(uint32_t mid)
{
	msg *m;
	int i;
	for(i = 0; i< MAX_PROC ; i++)
	{
	m = search_msg(mid, pready.ls[i].messages);
	if(m != NULL)
		return m;
	m = search_msg(mid, prunning.ls[i].messages);
	if(m != NULL)
		return m;
	m = search_msg(mid, pwaiting.ls[i].messages);
	if(m != NULL)
		return m;
	m = search_msg(mid, pterminate.ls[i].messages);
	if(m != NULL)
		return m;
	}
	return NULL;
}

 /**
 * reset the next_mid to 0
 */
void reset_next_mid()
{
  next_mid = 0;
}

/**
 * Return whether the list is empty or not
 */
bool is_empty_mls(msg *ls)
{
  int             i;
  if (ls == NULL)
    return FALSE;

  while (i < MAX_MSG)
  {
    if (ls[i].empty == FALSE)
      return FALSE;
    i++;
  }
  return TRUE;
}

/**
 * Delete the msg object.
 */
int32_t rm_msg(msg *m)
{
	m->empty = TRUE;
	return 0;
}

/**
 * Reset the specified message list
 */
int32_t reset_msg_lst(msg *ls)
{
  int             i;
  if (ls == NULL)
    return NULLPTR;

  for (i = 0; i < MAX_MSG; i++)
    rm_msg(&ls[i]);
  return OMGROXX;
}

/**
 * Move the src message to dest
 */
int32_t move_msg(msg * src, msg * dest)
{
  if (src == NULL || dest == NULL)
    return NULLPTR;

	dest->mid = src->mid;
	dest->sdr_pid = src->sdr_pid;
	dest->recv_pid = src->recv_pid;
	dest->pri = src->pri;
	dest->empty = src->empty;
	dest->data = src->data;

	src->empty = TRUE;

  return OMGROXX;
}
/**
 * Sort the list according to the priority
 */
int32_t sort_msg(msg *ls)
{
  bool            ordered = FALSE;
  int             i;
  int             size = MAX_MSG;
  msg             tmp;
  if (ls == NULL)
    return NULLPTR;
  while (!ordered)
  {
    ordered = TRUE;
    for (i = 0; i < size; i++)
      if (ls[i + 1].pri > ls[i].pri)
      {
        // Swap the two pcb
        move_msg(&(ls[i]), &tmp);
        move_msg(&(ls[i + 1]), &(ls[i]));
        move_msg(&tmp, &(ls[i + 1]));
        ordered = FALSE;
      }
    size--;
  }
  return OMGROXX;
}


/** TODO:  NOT IMPLEMENTED */

/**
 * Wait fot a message with the specified priority.
 */
int32_t recv_msg(uint32_t timeout, uint32_t pri, msg *m)
{
	return 0;
}


