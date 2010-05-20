/**
 * @file kernel.h
 * @brief Define global struct and functions use in the all kernel
 * @author Matthieu Maury
 * @date
 */

#ifndef __KERNEL_H
#define __KERNEL_H

#include <types.h>

#include "kprocess_list.h"

/*
 * global variable
 */

/**
 * @brief List of ready process
 */
pls             pready;

/**
 * @brief List of running process
 */
pls             prunning;

/**
 * @brief List of waiting process
 */
pls             pwaiting;

/**
 * @brief List of terminate process
 */
pls             pterminate;

/**
 * @brief Kernel last error. Default to success
 */
int32_t         kerror;

/**
 * @brief Global pointer to the current pcb error
 */
int32_t        *perror
/*
 * Functions
 */
/**
 * @brief is called at startup, contains application-specific
 * system initialisation.
 *
 * Applications should make sure that 'kset_registers' is
 * called, to that the exception handler can save registers.
 */
void            kinit();

#endif
