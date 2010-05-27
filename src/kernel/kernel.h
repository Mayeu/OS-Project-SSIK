/**
 * @file kernel.h
 * @brief Define global struct and functions use in the all kernel
 * @author Matthieu Maury
 * @date
 */

#ifndef __KERNEL_H
#define __KERNEL_H

#include <types.h>
#include <stdio.h>

#include "asm.h"
#include "kprocess_list.h"

/* Time of one clock tick */
#define QUANTUM 100*timer_msec

/*
 * global variable
 */

/*
 * declarationo of the pcb lists
 */

pcls            pclsready;      /*!< @brief List of ready process */
pcls            pclsrunning;    /*!< @brief List of running process */
pcls            pclswaiting;    /*!< @brief List of waiting process */
pcls            pclsterminate;  /*!< @brief List of terminate process */


/**
 * @brief Kernel last error. Default to success
 */
uint32_t        kerror;

/**
 * @brief Global pointer to the current pcb error
 */
uint32_t       *p_error;

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

/**
 * @brief The ancestor of the ancestor.
 *
 * First process to be launched
 */
void            init();

void            pinit();

#endif
