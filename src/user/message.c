/**
 * \file message.c
 * \brief message functions
 * \author Adrien Forest
 * \version 0.1
 * \date 25 Mai 2010
 */

#include <message.h>

#include "../kernel/ksyscall.h"

/**
 * Send the data to the process 'pid'.
 */
int send(void* data, msg_t tdata, int pid)
{
	msg_arg res = {data, tdata, pid, 0, 0};
	return syscall_one((int32_t)&res, SEND);
}

/**
 * Send the data to the process 'pid' with priority 'prio'.
 */
int sendp(void* data, msg_t tdata, int pid, int pri)
{	
	msg_arg res = {data, tdata, pid, pri, -1, 0};
	return syscall_one((int32_t)&res, SEND);
}

/**
 * Receives a message of type 'tdata'.
 */
int recv(void* data, msg_t tdata, int timeout)
{
	msg_arg res = {data, tdata, 0, 0, timeout, FTYPE};
	return syscall_one((int32_t)&res, RECV);
}

/**
 * Receives a message of type 'tdata' from the specific process 'pid'.
 */
int recv_from_pid(void* data, msg_t tdata, int pid, int timeout)
{
	msg_arg res = {data, tdata, pid, 0, timeout, FPID};
	return syscall_one((int32_t)&res, RECV);
}

/**
 * Receives a message of type 'tdata' from the specific priority 'pri'.
 */
int recv_fromp_pri(void* data, msg_t tdata, int pri, int timeout)
{
	msg_arg res = {data, tdata, 0, pri, 0, FPRI};
	return syscall_one((int32_t)&res, RECV);
}

