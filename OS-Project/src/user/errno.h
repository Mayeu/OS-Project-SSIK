/**
 * \file errno.h
 * \brief Error codes
 * \author Adrien Forest
 * \version 0.1
 * \date 24 Avril 2010
 *
 */

#IFNDEF __ERRNO_H
#DEFINE __ERRNO_H

 /**
 * \fn SUCCESS
 * \brief No error occured
 */
#DEFINE SUCCESS 0

 /**
 * \fn OUTOMEM
 * \brief Out of memory
 */
#DEFINE OUTOMEM 1

 /**
 * \fn UNKNPID
 * \brief Unknown pid (process identifier)
 */
#DEFINE UNKNPID 2

 /**
 * \fn UNKNMID
 * \brief Unknown mid (message identifier)
 */
#DEFINE UNKNMID 3

 /**
 * \fn INVPRI
 * \brief Invalid priority
 */
#DEFINE INVPRI 4

 /**
 * \fn OUTOPID
 * \brief Out of pid (number of processes is limited)
 */
#DEFINE OUTOPID 5

 /**
 * \fn OUTOMID
 * \brief Out of mid (number of messages is limited)
 */
#DEFINE OUTOMID 6

 /**
 * \fn NULLPTR
 * \brief Null pointer error
 */
#DEFINE NULLPTR 7

 /**
 * \fn EINVALL
 * \brief Invalid argument
 */
#DEFINE EINVALL 8

 /**
 * \fn INVEID
 * \brief Invalid eid (error identifier)
 */
#DEFINE INVEID 9

#__ERRNO_H
