/**
 * \file ksyscall.h
 * \brief Handle the syscall interrupts.
 * \author Christophe Carasco
 * \version 0.1
 * \date 21 mai 2010
 *
 */

#ifndef __KSYSCALL_H
#define __KSYSCALL_H

#include "include/registers.h"
#include "include/types.h"

/**
 * @brief the list of syscall code
 */
enum
{
  FOURCHETTE,                   /*!< Create a new process */
  PRINT,                        /*!< Print a line */
  FPRINT,                       /*!< Print a line in the specified output (malta or console) */
  SLEEP,                        /*!< Go to sleep */
  BLOCK,                        /*!< Block the process  */
  UNBLOCK,                      /*!< Unblock the process */
  WAIT,                         /*!< The process wait for an other process */
  SEND,                         /*!< Send a message to a process */
  RECV,                         /*!< Receive a message */
  PERROR,                       /*!< Print the current error */
  GERROR,                       /*!< Get the current error */
  SERROR,                       /*!< Set the current error */
  GETPINFO,                     /*!< Get info of a pcb */
  GETPID,                       /*!< Get the pid of the current process */
  GETALLPID,                    /*!< Get an array of all the pids */
  CHGPPRI,                      /*!< Change the priority of a process */
  KILL,                         /*!< Kill a process */
  EXIT                          /*!< Exit the current process */
};

/**
 * @brief Syscal without argument
 * @param a Syscall code
 */
int32_t         syscall_none(int32_t scode);

/**
 * @brief Syscal with 1 argument
 * @param a Syscall code
 * @param a the first arg to pass
 */
int32_t         syscall_one(int32_t p1, int32_t scode);

/**
 * @brief Syscal with 2 argument
 * @param a Syscall code
 * @param a the first arg to pass
 * @param a the second arg to pass
 */
int32_t         syscall_two(int32_t p1, int32_t p2, int32_t scode);

/**
 * @brief Syscal with 3 argument
 * @param a Syscall code
 * @param a the first arg to pass
 * @param a the second arg to pass
 * @param a the third arg to pass
 */
int32_t         syscall_three(int32_t p1, int32_t p2, int32_t p3,
                              int32_t scode);

/**
 * @brief Call by the exeption to handle the syscall
 * @param the registers used by the current pcb
 */
void            syscall_handler(registers_t * regs);

#endif
