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
#include "include/types.h"

typedef struct
{
  uint32_t        mid;
  uint32_t        sdr_pid;
  uint32_t        recv_pid;
  uint32_t        pri;
  uint32_t        empty;
  void           *data;
  msg_t           datatype;
} msg;


/**
 * \fn int32_t create_msg(msg *m, uint32_t mid, uint32_t sdr_pid, uint32_t recv_pid, uint32_t pri, void *data, msg_t datatype)
 * \brief Create the msg object.
 *
 * \param m the msg address where to write the message.
 * \param mid the message identifier
 * \param sdr_pid the pid of the sender
 * \param rcv_pid the pid of the receiver
 * \param pri the priority of the message
 * \param data the content of the message
 * \return an error code or the message identifier
 */
int32_t         create_msg(msg * m, uint32_t mid, uint32_t sdr_pid,
                           uint32_t recv_pid, uint32_t pri, void *data,
                           msg_t datatype);

/**
 * \fn int32_t rm_msg(uint32_t mid)
 * \brief Delete the msg object.
 *
 * \param mid the message identifier
 * \return an error code
 */
int32_t         rm_msg(msg * m);

/**
 * \fn uint32_t move_msg(msg * src, msg * dest);
 * \brief Move the src message to dest
 *
 * \param src the source message
 * \param dest the dest message
 * \return an error code
 */
int32_t         move_msg(msg * src, msg * dest);

#endif
