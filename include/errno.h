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

enum {
	SUCCESS, // No error occured
	OUTOMEM, // Out of memory
	UNKNPID, // Unknown pid (process identifier)
	UNKNMID, // Unknown mid (message identifier)
	INVPRI,  // Invalid priority
	OUTOPID, // Out of pid (number of processes is limited)
	OUTOMID, // Out of mid (number of messages is limited)
	NULLPTR, // Null pointer error
	EINVALL, // Invalid argument
	INVEID
};

#__ERRNO_H
