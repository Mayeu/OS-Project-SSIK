/**
 * \file kmsg.h
 * \brief Message related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 23 mai 2010
 *
 */

#ifndef __KMSG_H
#define __KMSG_H

#include "include/types.h"
#include <stdlib.h>

typedef struct
{
	uint32_t	mid;
	uint32_t sdr_pid;
	uint32_t recv_pid;
	uint32_t pri;
	uint32_t empty;
	void *data;
} msg;

/**
 * \fn int32_t send_msg(uint32_t mid, uint32_t sdr_pid, uint32_t recv_pid, uint32_t pri, void *data)
 * \brief Create the msg object in the destination list of messages.
 *
 * \param mid the message identifier
 * \param sdr_pid the pid of the sender
 * \param rcv_pid the pid of the receiver
 * \param pri the priority of the message
 * \param data the content of the message
 * \return an error code or the message identifier
 */
int32_t send_msg(uint32_t mid, uint32_t sdr_pid, uint32_t recv_pid, uint32_t pri, void *data);

/**
 * \fn msg *empty_msg_space(msg *ls)
 * \brief Search for the first empty space
 *
 * \param ls the message list
 * \return the message of NULL if error
 */
msg *empty_msg_space(msg *ls);

/**
 * \fn int32_t get_next_mid(uint32_t * nmid)
 * \brief set nmid to the next available value of mid
 *
 * \param nmid the new value of mid
 * \return an error code
 */
int32_t get_next_mid(uint32_t * nmid);

/**
 * \fn msg *search_msg(uint32_t mid, msg *ls)
 * \brief Search for the message identified by mid in the list ls
 *
 * \param mid the message identifier
 * \param ls the message list
 * \return the message or NULL if not found
 */
msg *search_msg(uint32_t mid, msg *ls);

/**
 * \fn msg *search_msg_all(uint32_t mid)
 * \brief Search for the message identified by mid in all lists
 *
 * \param mid the message identifier
 * \return the message or NULL if not found
 */
msg *search_msg_all(uint32_t mid);

/**
 * \fn void reset_next_mid()
 * \brief Reset next_mid to 0
 *
 */
void reset_next_mid();

/**
 * \fn bool is_empty_mls(msg *ls)
 * \brief Return whether the list of messages is empty or not
 *
 * \param ls the message list
 * \return TRUE if ls is empty, false otherwise
 */
bool is_empty_mls(msg *ls);

/**
 * \fn int32_t rm_msg(uint32_t mid)
 * \brief Delete the msg object.
 *
 * \param mid the message identifier
 * \return an error code
 */
int32_t rm_msg(msg *m);

/**
 * \fn uint32_t reset_msg_lst(msg *ls)
 * \brief Reset the specified message list
 *
 * \param ls the message list
 * \return an error code
 */
int32_t reset_msg_lst(msg *ls);

/**
 * \fn uint32_t move_msg(msg * src, msg * dest);
 * \brief Move the src message to dest
 *
 * \param src the source message
 * \param dest the dest message
 * \return an error code
 */
int32_t move_msg(msg * src, msg * dest);

/**
 * \fn int32_t sort_msg(msg *ls)
 * \brief Sort the list according to the priority
 *
 * \param ls the message list
 * \return an error code
 */
int32_t sort_msg(msg *ls);

/**
 * \fn int32_t recv_msg(uint32_t timeout, uint32_t pri, msg *m)
 * \brief Wait fot a message with the specified priority.
 *
 * \param timeout the maximum time to spend waiting for a message
 * \param pri the priority
 * \param m the content of the message received
 * \return an error code
 */
int32_t recv_msg(uint32_t timeout, uint32_t pri, msg *m);

#endif
