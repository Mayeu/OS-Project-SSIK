/**
 * \file kmsg_lst.h
 * \brief Message list related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 24 mai 2010
 *
 */

#ifndef __KMSG_LST_H
#define __KMSG_LST_H

#include <stdlib.h>
#include <errno.h>
#include <message.h>
#include "include/types.h"
#include "kmsg.h"

#define MAX_MSG 20
#define MIN_MPRI 0
#define MAX_MPRI 30
#define BAS_MPRI 15

enum                            // status
{
  WAITING,
  NOT_WAITING
};

/**
 * \struct mls
 * \brief List of messages.
 *
 */
typedef struct
{
  msg             ls[MAX_MSG];  /*!< list of messages. */
  int32_t         status;       /*!< the status of the list (type of message expected, see above). */
  msg_filter      filter;
  int32_t         filtervalue;  //value of the filter
  int32_t         timeout;
} mls;

/**
 * \brief Init the message list
 * \param ls the message list
 * \return an error code
 */
int32_t         init_msg_lst(mls * ls);

/**
 * \fn msg *search_msg(uint32_t mid, mls *ls)
 * \brief Search for the message identified by mid in the list ls
 *
 * \param mid the message identifier
 * \param ls the message list
 * \return the message or NULL if not found
 */
msg            *search_msg(uint32_t mid, mls * ls);

/**
 * \fn msg *search_msg_all(uint32_t mid)
 * \brief Search for the message identified by mid in all lists
 *
 * \param mid the message identifier
 * \return the message or NULL if not found
 */
msg            *search_msg_all(uint32_t mid);

/**
 * \fn msg *empty_msg_space(mls *ls)
 * \brief Search for the first empty space
 *
 * \param ls the message list
 * \return the message of NULL if error
 */
msg            *empty_msg_space(mls * ls);

/**
 * \fn bool is_empty_mls(mls *ls)
 * \brief Return whether the list of messages is empty or not
 *
 * \param ls the message list
 * \return TRUE if ls is empty, false otherwise
 */
bool            is_empty_mls(mls * ls);

/**
 * \fn int32_t send_msg(uint32_t sdr_pid, msg_arg *args)
 * \brief Create the msg object in the destination list of messages.
 *
 * \param mid the message identifier
 * \param sdr_pid the pid of the sender
 * \param rcv_pid the pid of the receiver
 * \param pri the priority of the message
 * \param data the content of the message
 * \return an error code or the message identifier
 */
int32_t         send_msg(uint32_t sdr_pid, msg_arg * args);

/**
 * \fn int32_t recv_msg(uint32_t sdr_pid, msg_arg *args)
 * \brief Wait fot a message with the specified priority.
 *
 * \param timeout the maximum time to spend waiting for a message
 * \param pri the priority
 * \param m the content of the message received
 * \return an error code
 */
int32_t         recv_msg(uint32_t recv_pid, msg_arg * args);

/**
 * \fn int32_t move_one_msg_lst(uint32_t mid, mls * src, mls * dest)
 * \brief Move the message identified by mid from src to dest
 *
 * \param mid the mid
 * \param src the source message list
 * \param dest the dest message list
 * \return an error code
 */
int32_t         move_one_msg_lst(uint32_t mid, mls * src, mls * dest);

/**
 * \fn int32_t move_msg_lst(uint32_t mid, mls * src, mls * dest)
 * \brief Move all the messages identified of src to dest
 *
 * \param src the source message list
 * \param dest the dest message list
 * \return an error code
 */
int32_t         move_msg_lst(mls * src, mls * dest);

/**
 * \fn int32_t sort_msg(mls *ls)
 * \brief Sort the list according to the priority
 *
 * \param ls the message list
 * \return an error code
 */
int32_t         sort_msg(mls * ls);

/**
 * \fn int32_t reset_msg_lst(mls *ls)
 * \brief Reset the specified message list
 *
 * \param ls the message list
 * \return an error code
 */
int32_t         reset_msg_lst(mls * ls);

/**
 * \fn void reset_next_mid()
 * \brief Reset next_mid to 0
 *
 */
void            reset_next_mid();

#endif
