/**
 * \file message.c
 * \brief message functions
 * \author Adrien Forest
 * \version 0.1
 * \date 25 Mai 2010
 */

#include <message.h>
#include <errno.h>
#include <process.h>

#include "../kernel/ksyscall.h"

/**
 * Send the data to the process 'pid'.
 * \private
 */
int
send(void *data, msg_t tdata, int pid)
{
  msg_arg         res = { data, tdata, pid, 0, -1, 0 };
  return syscall_one((int32_t) & res, SEND);
}

/**
 * Send the data to the process 'pid' with priority 'prio'.
 * \private
 */
int
sendp(void *data, msg_t tdata, int pid, int pri)
{
  msg_arg         res = { data, tdata, pid, pri, -1, 0 };
  return syscall_one((int32_t) & res, SEND);
}

/**
 * Receives a message of type 'tdata'.
 * \private
 */
int
recv(void *data, msg_t tdata, int timeout)
{
  int             res2;
  msg_arg         res = { data, tdata, 0, 0, timeout, FTYPE };
  res2 = syscall_one((int32_t) & res, RECV);
  while (res2 == NOTFOUND && timeout <= 0)
  {
    sleep(10);
    res.timeout = res.timeout - 10;
    res2 = syscall_one((int32_t) & res, RECV);
  }
  return res2;
}

/**
 * Receives a message of type 'tdata' from the specific process 'pid'.
 * \private
 */
int
recv_from_pid(void *data, msg_t tdata, int pid, int timeout)
{
  int             res2;
  msg_arg         res = { data, tdata, pid, 0, timeout, FPID };
  res2 = syscall_one((int32_t) & res, RECV);
  if (res2 == NOTFOUND)
    res2 = syscall_one((int32_t) & res, RECV);
  return res2;
}

/**
 * Receives a message of type 'tdata' from the specific priority 'pri'.
 * \private
 */
int
recv_fromp_pri(void *data, msg_t tdata, int pri, int timeout)
{
  int             res2;
  msg_arg         res = { data, tdata, 0, pri, timeout, FPRI };
  res2 = syscall_one((int32_t) & res, RECV);
  if (res2 == NOTFOUND)
    res2 = syscall_one((int32_t) & res, RECV);
  return res2;
}
