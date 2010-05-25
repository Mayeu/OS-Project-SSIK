/**
 * \file kmsg_lst.c
 * \brief Message list related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 24mai 2010
 *
 */

#include <string.h>
#include "kmsg_lst.h"
#include "kernel.h"
#include "kinout.h"
static uint32_t next_mid = 0;


	char resc[2];
/* Privates functions */
int32_t         get_next_mid(uint32_t * nmid);


int32_t init_msg_lst(mls *ls)
{
	int             i = 0;
  if (ls == NULL)
    return NULLPTR;
	ls->status = NOT_WAITING;
  for(i=0 ; i<MAX_MSG ; i++)
	ls->ls[i].empty = TRUE;
	return OMGROXX;
}

/**
 * Search for the message identified by mid in the list ls
 */
msg *search_msg(uint32_t mid, mls *ls)
{
	int j;
	for(j = 0 ; j<MAX_MSG ; j++)
	{
		if(ls->ls[j].mid == mid && ls->ls[j].empty == FALSE)
			return &ls->ls[j];
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
		m = search_msg(mid, &pready.ls[i].messages);
		if(m != NULL)
			return m;
	}
	for(i = 0; i< MAX_PROC ; i++)
	{
		m = search_msg(mid, &prunning.ls[i].messages);
		if(m != NULL)
			return m;
	}
	for(i = 0; i< MAX_PROC ; i++)
	{
		m = search_msg(mid, &pwaiting.ls[i].messages);
		if(m != NULL)
			return m;
	}
	for(i = 0; i< MAX_PROC ; i++)
	{
		m = search_msg(mid, &pterminate.ls[i].messages);
		if(m != NULL)
			return m;
	}
	return NULL;
}

/**
 * Search for the message identified by mid in the list ls
 */
msg *search_msg_filtered(msg_filter filter, int32_t filtervalue, mls *ls, msg_t datatype)
{
	int i;
	for(i = 0 ; i<MAX_MSG ; i++)
	{
		if(ls->ls[i].empty == FALSE && ls->ls[i].datatype == datatype)
		{
			if(filter == FPRI && ls->ls[i].pri == filtervalue)
					return &ls->ls[i];

			else if(filter == FPID && ls->ls[i].sdr_pid == filtervalue)
				return &ls->ls[i];

			else if(filter == FTYPE && ls->ls[i].datatype == filtervalue )
				return &ls->ls[i];
		}
	}
	return NULL;
}

/**
 * search the first empty space of the list
 */
msg *empty_msg_space(mls *ls)
{
	int             i = 0;
  if (ls == NULL)
    return NULL;
  while (ls->ls[i].empty == FALSE && i < MAX_MSG)
    i++;
  if (i == MAX_MSG)
    return NULL;
  else
    return &ls->ls[i];
}

/**
 * Return whether the list is empty or not
 */
bool is_empty_mls(mls *ls)
{
  int             i = 0;
  if (ls == NULL)
    return FALSE;
  while (i < MAX_MSG)
  {
    if (ls->ls[i].empty == FALSE)
      return FALSE;
    i++;
  }
  return TRUE;
}

/**
 * Send the msg object.
 */
int32_t send_msg(uint32_t sdr_pid, msg_arg *args)
{
	pcb *receiver;
	msg *m;
	uint32_t new_mid;
	uint32_t pri = args->pri;
	uint32_t recv_pid = args->pid;
	if(args->data == NULL)
		return NULLPTR;
	if(pri >= MAX_MPRI && pri <= MIN_MPRI)
		return INVPRI;
	receiver = searchall(recv_pid);
	if(receiver == NULL)
		return INVARG;
	m = empty_msg_space(&receiver->messages);
	if(m == NULL)
		return OUTOMEM;
	if(get_next_mid(&new_mid) != OMGROXX)
		return OUTOPID;
	create_msg(m, new_mid, sdr_pid, recv_pid, pri, args->data, args->datatype);

	// Signal the recv_process that the message is arrived (if he wanted this one)
	/*if(receiver->message.status ==  WAITING)
	{
		receiver->messages.timeout = 0;
		receiver->messages.status = NOT_WAITING;
	}*/
	return OMGROXX;
}




														/** TODO:  IMPLEMENT RECV_MSG */
/**
 * Wait fot a message with the specified priority.
 */
int32_t recv_msg(uint32_t recv_pid, msg_arg *args)
{
	msg_filter filter = args->filter;
	int filtervalue;
	pcb *p;
	msg *m;
	if(filter == FPRI)
	{
		filtervalue = args->pri;
		
	}else if(filter == FPID)
	{
		filtervalue = args->pid;

	}else
	{
		filtervalue = (int)args->datatype;
	}
	p = searchall(recv_pid);
	if(p == NULL)
		return UNKNPID;
	// look for a message according to the filter.
	m = search_msg_filtered(filter, filtervalue, &p->messages, args->datatype);

/*	if(m == NULL)
	{
		// if not found
		p->messages.status = WAITING;
		p->messages.filter = filter;
		p->messages.filtervalue = filtervalue;
		p->messages.timeout = (args->timeout >=0 )? args->timeout : 0;

		// WAIT NOW UNTIL THE MESSAGE IS IN THE MAILBOX
		//if(timeout > 0)
		//sleep (args->timeout);	// status WAITING

		//check pk reveil
		//return OMGROXX;
		m = search_msg_filtered(filter, filtervalue, &p->messages, args->datatype);
	}
*/
	if(m != NULL)
	{
		if(args->datatype == CHAR_PTR)		// case char *
			strcpy(m->data, args->data);
		else if(args->datatype == INT_T)		// case int
		{
			int *b = (int *) args->data;
			*b = (int) m->data;
		}
/* NOT WORKING FOR NOW
		else{										// case other (struct ...)
			args->data = m->data;
			kprintln(itos((int)args->data, resc));
		}
*/
		rm_msg(m);
		return OMGROXX;
	}

	return FAILNOOB;
}
/*
void func()
{
	while(timeout >= 0 && !t);
}*/

/**
 * Move the mid message from src list to dest list
 */
int32_t move_one_msg_lst(uint32_t mid, mls * src, mls * dest)
{
	msg *from, *to;
  if (src == NULL || dest == NULL)
    return NULLPTR;

	from = search_msg(mid, src);
	if(from == NULL)
		return INVARG;		// INVMID or wrong list
	to = empty_msg_space(dest);
	if(to == NULL)
		return OUTOMEM;
	move_msg(from, to);
  return OMGROXX;
}

/**
 * Move the src message list to dest list
 */
int32_t move_msg_lst(mls * src, mls * dest)
{
	int i;
	for(i=0 ; i<MAX_MSG ; i++)
		move_msg(&src->ls[i], &dest->ls[i]);
	dest->status = src->status;
	dest->filter = src-> filter;
	dest->filtervalue = src->filtervalue;
	dest->timeout = src->timeout;
  return OMGROXX;
}

/**
 * Sort the list according to the priority
 */
int32_t sort_msg(mls *ls)
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
      if (ls->ls[i + 1].pri > ls->ls[i].pri)
      {
        // Swap the two pcb
        move_msg(&(ls->ls[i]), &tmp);
        move_msg(&(ls->ls[i + 1]), &(ls->ls[i]));
        move_msg(&tmp, &(ls->ls[i + 1]));
        ordered = FALSE;
      }
    size--;
  }
  return OMGROXX;
}

/**
 * Reset the specified message list
 */
int32_t reset_msg_lst(mls *ls)
{
  int             i;
  if (ls == NULL)
    return NULLPTR;

  for (i = 0; i < MAX_MSG; i++)
    rm_msg(&ls->ls[i]);
  return OMGROXX;
}

 /**
 * reset the next_mid to 0
 */
void reset_next_mid()
{
  next_mid = 0;
}




/* private function */

/**
 * set nmid to the next available value of mid
 */
int32_t get_next_mid(uint32_t * nmid)
{
  int             init = next_mid;
  *nmid = next_mid;
	next_mid++;
  while (search_msg_all(next_mid) != NULL)
  {
    next_mid++;
    if (next_mid == init)
      return OUTOMID;
  }
  return OMGROXX;
}
/**
 * set nmid to the next available value of mid
 */
/*int32_t get_next_mid(uint32_t * nmid)
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
}*/
