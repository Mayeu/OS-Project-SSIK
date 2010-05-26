/**
 * \file kmsg.c
 * \brief Message related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 23 mai 2010
 *
 */

#include <errno.h>
#include <stdio.h>
#include "kmsg.h"


/**
 * Create the msg object.
 */
int32_t create_msg(msg *m, uint32_t mid, uint32_t sdr_pid, uint32_t recv_pid, uint32_t pri, void *data, msg_t datatype)
{
	m->mid = mid;
	m->sdr_pid =  sdr_pid;
	m->recv_pid =  recv_pid;
	m->pri =  pri;
	m->empty =  FALSE;
	m->data =  data;
	m->datatype =  datatype;
	return OMGROXX;
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
 * Move the src message to dest
 */
int32_t move_msg(msg * src, msg * dest)
{
	dest->mid = src->mid;
	dest->sdr_pid = src->sdr_pid;
	dest->recv_pid = src->recv_pid;
	dest->pri = src->pri;
	dest->empty = src->empty;
	dest->data = src->data;

	src->empty = TRUE;

  return OMGROXX;
}
