/**
 * \file errno.h
 * \brief List of all the error codes used by the OS.
 * \author Adrien Forest
 * \version 0.1
 * \date 24 Avril 2010
 *
 */

#ifndef __ERRNO_H
#define __ERRNO_H

/**
 * \brief  enum of the error codes.
 */
enum
{
  KILLED = -12,                 /*!< Process has been killed */
  NOTFOUND,                     /*!< These are not the droid you are looking for */
  OUTOMEM,                      /*!< Out of memory */
  UNKNPID,                      /*!< Unknown pid (process identifier) */
  UNKNMID,                      /*!< Unknown mid (message identifier) */
  INVPRI,                       /*!< Invalid priority */
  OUTOPID,                      /*!< Out of pid (number of processes is limited) */
  OUTOMID,                      /*!< Out of mid (number of messages is limited) */
  NULLPTR,                      /*!< Null pointer error */
  INVARG,                       /*!< Invalid argument */
  INVEID,                       /*!< Invalid ID */
  FAILNOOB,                     /*!< General error */
  OMGROXX                       /*!< No error occured */
};

// DON'T FORGET TO ADD THE CORRESPONDING ERROR MESSAGE 
// IN kerror.c IN THE ARRAY err_msgs

#endif //__ERRNO_H
