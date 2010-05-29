/**
 * \file kprocess.h
 * \brief Process related functions
 *
 * This module manage process and list of process for the os.
 *
 * \author Christophe Carasco
 * \version 0.1
 * \date 19 Avril 2010
 *
 */

#ifndef __KPROCESS_H
#define __KPROCESS_H

#include <types.h>
#include <registers.h>
#include <stdlib.h>
#include <process.h>
#include "kprocess_list.h"
#include "kpcb.h"

/*
 * Functiuns
 */

/**
 * @brief Create a new process and add it to the ready list
 * @param the name of the process
 * @param the priority of the process
 * @param the parameter to pass to the process
 * @return the pid of the process or a negative error code
 * (INVARG, OUTOMEM, FAILNOOB)
 */
uint32_t create_proc(char *name, uint32_t prio, char **params);

/**
 * @brief Return the pcb currently running
 * @return A pointer to the pcb
 */
pcb            *get_current_pcb();

/**
 * @brief Set the currently running pcb
 * @param the pcb to set
 */
void            set_current_pcb(pcb * p);

/**
 * @brief Get the array showing which part of the
 * stack is in use. This functions is here for test purpose only !
 */
int32_t        *get_used_stack();

/**
 * @brief Allocate the stack for a pcbs. In our case keep in an array which
 * part of the stack is used
 * @param the pid of the process who need a stack
 */
uint32_t       *allocate_stack(uint32_t pid);

/**
 * @brief Deallocate the stack for a certain pid
 * @param the pid of the pcb
 */
int32_t         deallocate_stack(uint32_t pid);

/**
 * @brief Search for a pid in all the list of the kernel
 * @param the pid
 * @return the pcb
 */
pcb            *search_all_list(uint32_t pid);

/**
 * \brief deallocate a pcb.
 * \param p the process to delete
 * \return an error code
 */
uint32_t        rm_p(pcb * p);

/**
 * \brief change the priority of a process.
 *
 * \param p the pointer to the pcb
 * \param pri the new priority
 * \return an error code
 */

uint32_t        chg_ppri(pcb * p, uint32_t pri);

/**
 * \brief copy and give the information of a pcb into a pcbinfo
 *
 * \param p the pointer to the pcb
 * \param pi the pointer to the pcbinfo
 * \return an error code
 */
uint32_t        get_pinfo(pcb * p, pcbinfo * pi);

/**
 * \brief add a pid to the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to add
 * \return an error code
 */
uint32_t        add_psupervised(pcb * p, uint32_t pid);

 /**
 * \brief remove a pid from the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to remove
 * \return an error code
 */
uint32_t        rm_psupervised(pcb * p, uint32_t pid);

/**
 * \brief Return whether the list is empty or not
 *
 * \param ls a pointer to the list
 * \return a boolean
 */
bool            p_is_empty(pcb * pcb);

/**
 * @brief Return the next avaible pid, or an error code
 * @return the next avaible pid
 */
int32_t         get_next_pid();

 /**
 * \brief reset the next_pid variable to 0
 */
void            reset_next_pid();

/**
 * @brief reset to -1 all the element of used_stack
 */
void            reset_used_stack();

/**
 * \brief Returns the argument at position num
 *
 * \param data the original array of arguments
 * \param num the number of the argument to return
 * \return the argument
 */
char           *argn(char **data, int num);

#endif
