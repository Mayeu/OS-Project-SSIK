/**
 * @file kernel.h
 * @brief Define global struct and functions use in the all kernel
 * @author Matthieu Maury
 * @date
 */

#include <types.h>

#include "kprocess.h"

/*
 * global variable
 */

/**
 * @brief List of ready process
 */
pls pready;

/**
 * @brief List of running process
 */
pls prunning;

/**
 * @brief List of waiting process
 */
pls pwaiting;

/**
 * @brief List of terminate process
 */
pls pterminate;

/**
 * @brief Kernel last error. Default to success
 */
int32_t kerror;

/**
 * @brief Global pointer to the current pcb error
 */
int32_t *perror
