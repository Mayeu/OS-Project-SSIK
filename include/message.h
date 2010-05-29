/**
 * \file message.h
 * \brief message functions
 * \author Adrien Forest
 * \version 0.1
 * \date 25 Mai 2010
 */

#ifndef __MESSAGE_H
#define __MESSAGE_H

#define MIN_MPRI 0
#define MAX_MPRI 30
#define BAS_MPRI 15

/**
 * \brief enum of all the types of data a user can send using messages functions.
It must be specified when sending or receiving messages.
 */
typedef enum
{
  INT_T,                        /*!< Type integer */
  CHAR_PTR,                     /*!< Type char* */
  //OTHER_T
} msg_t;

/**
 * \brief enum of all the type of message filters.
 */
typedef enum
{
  FPRI,                         /*!< Filter message by priority */
  FPID,                         /*!< Filter message by pid */
  FTYPE,                        /*!< Filter message by type (msg_t structure) */
  FNONE                         /*!< No filter */
} msg_filter;

/**
 * \brief structure used in all the send and receive functions.
It includes all the requiered fields used for message communication.
 */
typedef struct
{
  void           *data;         /*!< User data */
  msg_t           datatype;     /*!< Type of message */
  int             pid;          /*!< Pid of the sender/receiver */
  int             pri;          /*!< Priority of the message */
  int             timeout;      /*!< Message timeout */
  msg_filter      filter;       /*!< Message filter to apply */
} msg_arg;

/**
 * \fn send(void* data, int pid)
 * \brief Send the data to the process 'pid'.
 *
 * \param data the data to send
 * \param pid the pid of the receiver
 * \return an error code
 */
int             send(void *data, msg_t tdata, int pid);

/**
 * \fn sendp(void* data, int pid, int prio)
 * \brief Send the data to the process 'pid' with priority 'prio'.
 *
 * \param data the data to send
 * \param tdata data type of the 'data' variable
 * \param pid the pid of the receiver
 * \param the priority of the message to send
 * \return an error code
 */
int             sendp(void *data, msg_t tdata, int pid, int prio);

/**
 * \fn int recv(void* data, msg_t tdata, int timeout)
 * \brief Receives a message of type 'tdata'.
 *
 * \param data the data to send
 * \param tdata data type of the 'data' variable
 * \param timeout time to wait before stopping receive process
 * \return the pid of the receiver or an error code otherwise
 */
int             recv(void *data, msg_t tdata, int timeout);

/**
 * \fn int recv_from_pid(void* data, msg_t tdata, int pid, int timeout)
 * \brief Receives a message of type 'tdata' from the specific process 'pid'.
 *
 * \param data the data to send
 * \param tdata data type of the 'data' variable
 * \param pid the pid of the receiver
 * \param timeout time to wait before stopping receive process
 * \return the pid of the receiver or an error code otherwise
 */
int             recv_from_pid(void *data, msg_t tdata, int pid, int timeout);

/**
 * \fn int recv_fromp_pri(void* data, msg_t tdata, int pri, int timeout)
 * \brief Receives a message of type 'tdata' from the specific priority 'pri'.
 *
 * \param data the data to send
 * \param tdata data type of the 'data' variable
 * \param pri the priority of the message to wait
 * \param timeout time to wait before stopping receive process
 * \return the pid of the receiver or an error code otherwise
 */
int             recv_fromp_pri(void *data, msg_t tdata, int pri, int timeout);


#endif //__MESSAGE_H
