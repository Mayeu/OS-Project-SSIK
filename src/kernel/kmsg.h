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

#include <stdlib.h>
#include <message.h>
#include <errno.h>
#include "include/types.h"

#define MAX_MSG 20

/**
 * \struct msg
 * \brief Message representation.
 *
 * A message is represented by its sender, its receiver, the content
 * of the message (with its type) and the priority
 */
typedef struct
{
	uint32_t sdr_pid;			/*!< Sender process identifier. */
	uint32_t recv_pid;		/*!< Receiver process identifier. */
	uint32_t pri;				/*!< Priority. */
	void *data;					/*!< data (content of the message). */
	msg_t datatype;			/*!< Sender process identifier. */
} msg;

/**
 * \struct mls
 * \brief Message list representation.
 *
 * A message list is represented as an array with two pointers to
 * simulate a fifo. out is the index of first element to read and
 * in is the index of the array where to push the next element.
 */
typedef struct
{
  msg             ls[MAX_MSG];/*!< list of messages. */
  int32_t         status;		/*!< the status of the list (type of message expected, see above). */
  uint32_t        length;		/*!< number of elements */
  uint32_t        in;			/*!< index where to push */
  uint32_t        out;			/*!< first element to pop */
  msg_filter filter;				/*!< filter type */
  int32_t filtervalue;			/*!< value of the filter */
  int32_t timeout;				/*!< timeout before cancelling the receiving */
} mls;

/**
 * \fn void reset_mls(mls *m)
 * \brief reset the fifo list to default value
 * \param m the list of messages
 * \return void
 */
void            reset_mls(mls *m);

/**
 * \fn uint32_t push_mls(mls* m, msg *mess)
 * \brief push the message to the end of the list
 *
 * \param m the list of messages
 * \param mess the list of messages
 * \return an error code
 */
uint32_t        push_mls(mls* m, msg *mess);


/**
 * \fn uint32_t pop_mls(mls* m, msg *mess)
 * \brief pop the message from the beginning of the list
 *
 * \param m the list of messages
 * \param mess the list of messages
 * \return an error code
 */
uint32_t        pop_mls(mls* m, msg *mess);

/**
 * \fn int32_t create_msg(msg *m, uint32_t sdr_pid, uint32_t recv_pid, uint32_t pri, void *data, msg_t datatype)
 * \brief create the message object with the given values
 *
 * \param m the list of messages
 * \param sdr_pid the pid of the sender
 * \param recv_pid the pid of the receiver
 * \param pri the priority of the message
 * \param data the content of the message
 * \param datatype the type of the content
 * \return an error code
 */
int32_t create_msg(msg *m, uint32_t sdr_pid, uint32_t recv_pid, uint32_t pri, void *data, msg_t datatype);

/**
 * \fn int32_t copy_msg(msg * src, msg * dest)
 * \brief copy a message located in src to dest.
 *
 * \param src the location of the source message
 * \param dest the location of the destination message
 * \return an error code
 */
int32_t copy_msg(msg * src, msg * dest);

/**
 * \fn int32_t send_msg(uint32_t sdr_pid, msg_arg *args)
 * \brief send a message according to the specified arguments.
 *
 * \param sdr_pid the pid of the sender
 * \param args the arguments
 * \return an error code
 */
int32_t send_msg(uint32_t sdr_pid, msg_arg *args);

/**
 * \fn int32_t recv_msg(uint32_t sdr_pid, msg_arg *args)
 * \brief receive a message according to the specified arguments.
 * Could be blocked until the message comes.
 *
 * \param recv_pid the pid of the receiver
 * \param args the arguments
 * \return an error code
 */
int32_t recv_msg(uint32_t recv_pid, msg_arg *args);

/**
 * \fn bool search_msg_filtered(msg_filter filter, int32_t filtervalue, msg *m, msg_t datatype)
 * \brief receive a message according to the specified arguments.
 * Could be blocked until the message comes.
 *
 * \param filter the type of the filter
 * \param filtervalue the value of the filter
 * \param m the message
 * \param datatype the type of the message
 * \return TRUE if a message with the good filter is found, FALSE otherwise
 */
bool search_msg_filtered(msg_filter filter, int32_t filtervalue, msg *m, msg_t datatype);

/**
 * \fn int32_t copy_mls(mls * src, mls * dest)
 * \brief Copye a list of messages from src to dest
 *
 * \param src the source message list
 * \param dest the dest message list
 * \return an error code
 */
int32_t copy_mls(mls * src, mls * dest);

#endif
