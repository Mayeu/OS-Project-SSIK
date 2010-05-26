/**
 * \file kprocess.h
 * \brief Process related functions
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

/**
 * \fn int create_proc(char *name, pcb *p)
 * \brief initialize a pcb with all the needed value,
 * 			add it to the ready queue, and ask for a 
			long term scheduling.
 *
 * The allocated space in the ready queue is supposed to be free.
 *
 * \param name the name of the program to launch
 * \param p the pointer to the pcb
 * \return the pid of the newly created process(>0), or an error (<0)
 */

//uint32_t        create_proc(char *name, uint32_t prio, int32_t params[MAX_ARG]);
uint32_t        create_proc(char *name, uint32_t prio, char **params);

pcb *get_current_pcb();
void
set_current_pcb(pcb *p);

int32_t *
get_used_stack();
	uint32_t *
allocate_stack(uint32_t pid);
	int32_t
deallocate_stack(uint32_t pid);


/**
 * \fn int rm_p(pcb *p)
 * \brief deallocate a pcb.
 *
 * \param p the process to delete
 * \return an error code
 */

uint32_t        rm_p(pcb * p);

/**
 * \fn int chg_ppri(pcb *p, int pri)
 * \brief change the priority of a process.
 *
 * \param p the pointer to the pcb
 * \param pri the new priority
 * \return an error code
 */

uint32_t        chg_ppri(pcb * p, uint32_t pri);

/**
 * \fn int get_pinfo(pcb *p, pcbinfo *pi)
 * \brief copy and give the information of a pcb into a pcbinfo
 *
 * \param p the pointer to the pcb
 * \param pi the pointer to the pcbinfo
 * \return an error code
 */
uint32_t        get_pinfo(pcb * p, pcbinfo * pi);

/**
 * \fn int add_psupervise(pcb *p, int pid)
 * \brief add a pid to the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to add
 * \return an error code
 */
uint32_t        add_psupervised(pcb * p, uint32_t pid);

 /**
 * \fn int rm_psupervised(pcb *p, int pid)
 * \brief remove a pid from the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to remove
 * \return an error code
 */
uint32_t        rm_psupervised(pcb * p, uint32_t pid);

/**
 * \fn int add_psupervisor(pcb *p, int pid)
 * \brief change the pid of the process supervisor
 *
 * \param p the pointer to the process
 * \param pid the pid to add
 * \return an error code
 */
uint32_t        add_psupervisor(pcb * p, uint32_t pid);

 /**
 * \fn int rm_psuperviser(pcb *p, int pid)
 * \brief remove a pid from the superviser list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to remove
 * \return an error code
 */
uint32_t        rm_psupervisor(pcb * p, uint32_t pid);


/**
 * \fn bool p_is_empty(pls *ls)
 * \brief Return whether the list is empty or not
 *
 * \param ls a pointer to the list
 * \return a boolean
 */
bool            p_is_empty(pcb * pcb);

/**
 * @brief Return the next avaible pid, or an error code
 */

	int32_t
get_next_pid();

 /**
 * \fn uint32_t rest_pid()
 * \brief reset the next_pid to 0
 *
 */
void            reset_next_pid();

/**
 * @brief reset to -1 all the element of used_stack
 */
void
reset_used_stack();

/**
 * \fn char *argn(char **data, int num)
 * \brief Returns the argument at position num
 *
 * \param data the original array of arguments
 * \param num the number of the argument to return
 * \return the argument
 */
char           *argn(char **data, int num);

/*
 * Deprecated
 */

/**
 * \fn int copy_p(pcb *psrc, pcb *pdest)
 * \brief copy a pcb inside an other
 *
 * \param psrc the source pcb
 * \param pdest the destination pcb
 * \return an error code
 */
uint32_t        move_p(pcb * psrc, pcb * pdest);

#endif
