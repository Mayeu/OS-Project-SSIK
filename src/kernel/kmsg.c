/**
 * \file kmsg.c
 * \brief Message related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 23 mai 2010
 *
 */

#include <string.h>
#include "kmsg.h"
#include "kprocess.h"
#include "kprocess_list.h"

/**
 * reset the fifo buffer to default value
 */
void
reset_mls(mls *m)
{
  m->in = 0;
  m->out = 0;
  m->length = 0;
}

/**
 * push a char in the fifo buffer
 */
uint32_t
push_mls(mls* m, msg *mess)
{
  if (m->length >= MAX_MSG)
    return OUTOMEM;

  copy_msg(mess, &m->ls[m->in++]);
  m->length++;

  if (m->in >= MAX_MSG)
    m->in = 0;

  return OMGROXX;
}

/**
 * pop a char from the fifo buffer
 */
uint32_t
pop_mls(mls* m, msg *mess)
{
  if (mess == NULL)
    return NULLPTR;

  if (m->length == 0)
    return FAILNOOB;

  copy_msg(&m->ls[m->out++], mess);
  m->length--;

  if (m->out >= MAX_MSG)
    m->out = 0;

  return OMGROXX;
}

/**
 * Create the msg object.
 */
int32_t create_msg(msg *m, uint32_t sdr_pid, uint32_t recv_pid, uint32_t pri, void *data, msg_t datatype)
{
	if(m == NULL || data == NULL)
		return NULLPTR;

	m->sdr_pid =  sdr_pid;
	m->recv_pid =  recv_pid;
	m->pri =  pri;
	m->data =  data;
	m->datatype =  datatype;

	return OMGROXX;
}

/**
 * Copy the src message to dest
 */
int32_t copy_msg(msg * src, msg * dest)
{
  if (src == NULL || dest == NULL)
    return NULLPTR;

	dest->sdr_pid = src->sdr_pid;
	dest->recv_pid = src->recv_pid;
	dest->pri = src->pri;
	dest->data = src->data;
	dest->datatype = src->datatype;

  return OMGROXX;
}

/**
 * Send the msg object.
 */
int32_t send_msg(uint32_t sdr_pid, msg_arg *args)
{
	pcb *receiver;
	msg m;
	uint32_t pri = args->pri;
	uint32_t recv_pid = args->pid;
	int32_t res;
	if(args->data == NULL)
		return NULLPTR;
	if(pri >= MAX_MPRI && pri <= MIN_MPRI)
		return INVPRI;
	receiver = searchall(recv_pid);
	if(receiver == NULL)
		return UNKNPID;
	res = create_msg(&m, sdr_pid, recv_pid, pri, args->data, args->datatype);

	res = push_mls(&receiver->messages, &m);
	if(res != OMGROXX)
		return res;

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
	msg m;
	int32_t res;
	bool res2;
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
	do
	{
		res = pop_mls(&p->messages, &m);
		if(res == OMGROXX)
			res2 = search_msg_filtered(filter, filtervalue, &m, args->datatype);
		else
			res2 = FALSE;
	} while(res2 == FALSE && p->messages.length != 0);
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
	if(res2 != FALSE)
	{
		if(args->datatype == CHAR_PTR)		// case char *
			strcpy(m.data, args->data);
		else if(args->datatype == INT_T)		// case int
		{
			int *b = (int *) args->data;
			*b = (int) m.data;
		}
/* NOT WORKING FOR NOW
		else{										// case other (struct ...)
			args->data = m.data;
			kprintln(itos((int)args->data, resc));
		}
*/
		return OMGROXX;
	}

	return FAILNOOB;
}


/**
 * Search for the message with a specific filter
 */
bool search_msg_filtered(msg_filter filter, int32_t filtervalue, msg *m, msg_t datatype)
{
	if(m->datatype == datatype)
	{
		if((filter == FPRI && m->pri == filtervalue) ||
		(filter == FPID && m->sdr_pid == filtervalue) ||
		(filter == FTYPE && m->datatype == filtervalue ))
			return TRUE;
	}
	return FALSE;
}

/**
 * Copy the src message list to dest list
 */
int32_t copy_mls(mls * src, mls * dest)
{
	int i;
	for(i=0 ; i<MAX_MSG ; i++)
		copy_msg(&src->ls[i], &dest->ls[i]);
	dest->status = src->status;
	dest->filter = src-> filter;
	dest->filtervalue = src->filtervalue;
	dest->timeout = src->timeout;
	dest->length = src->length;
	dest->in = src->in;
	dest->out = src->out;
  return OMGROXX;
}

