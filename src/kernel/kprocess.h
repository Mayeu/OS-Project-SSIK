/**
 * \file kprocess.h
 * \brief Process related functions
 *
 * This module manage process and list of process for the os.
 *
 * \author Christophe Carasco
 * \version 0.1
 * \date 19 Avril 2010
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
 * Functions
 */

/**
 * @brief Create a new process and add it to the ready list
 * @param the name of the process
 * @param the priority of the process
 * @param the parameter to pass to the process
 * @return the pid of the process or a negative error code
 * (INVARG, OUTOMEM, FAILNOOB)
 */
uint32_t        create_proc(char *name, uint32_t prio, uint32_t argc, char **params);

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
uint32_t        chg_ppri(uint32_t pid, uint32_t pri);

/**
 * \brief copy and give the information of a pcb into a pcbinfo
 *
 * \param p the pointer to the pcb
 * \param pi the pointer to the pcbinfo
 * \return an error code
 */
uint32_t        get_pinfo(uint32_t pid, pcbinfo * pi);

/**
 * \brief Returns an array of all the processes in the system (in all lists).
 *
 * \param tab the table of pids
 * \return the number of pids
 */
uint32_t        get_all_pid(uint32_t * tab);

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
 * @brief The process go to sleep
 *
 * The current process will sleep for the specified time. It will be moved
 * to the waiting process list and the scheduler is called.
 *
 * @param p the pcb
 * @param time the time to sleep
 * @return an error code
 */
uint32_t        go_to_sleep(uint32_t time);

/**
 * @brief Block a pcb. A blocked pcb can not execute code until he wake up
 *
 * Block take different kind of state in argument to allow different kind
 * of blocking (state describe in kpcb.h) :
 *     - BLOCKED
 *     - WAITING_IO
 *     - DOING_IO
 * Only this state are accepted as blocked.
 * If the pcb wich is blocked is the current pcb, schedule is called.
 *
 * @param p the process to block
 * @param state the state to set the process
 * @return an error code
 */
int32_t         kblock(uint32_t pid, int32_t state);

/**
 * @brief Block a pcb. A blocked pcb can not execute code until he wake up
 *
 * Block take different kind of state in argument to allow different kind
 * of blocking (state describe in kpcb.h) :
 *     - BLOCKED
 *     - WAITING_IO
 *     - DOING_IO
 * Only this state are accepted as blocked.
 * If the pcb wich is blocked is the current pcb, schedule is called.
 *
 * @param p the process to block
 * @param state the state to set the process
 * @return an error code
 */
int32_t         kblock_pcb(pcb * p, int32_t state);

/**
 * @brief Set the currently used pcb to wait for an other pcb to terminate
 *
 * The waitfor field in the pcb will be get the pid to wait.
 * And the process will be move in the waiting list with the state
 * WAITING_PCB.
 * After this sechedule is called.
 *
 * @param pid the pid to wait
 * @return an error code
 */
int32_t         waitfor(uint32_t pid, int32_t * status);

/**
 * @brief Kill the current process
 *
 * The process passed in arg is moved in the terminated list and get the zombie
 * state. Waiting for his parent to read the return register.
 * The return register is set to the KILLED error code.
 * 
 * @param pid the pid of the process to kill
 * @return an error code
 */
int32_t         kkill(uint32_t pid);

/**
 * @brief Kill the current process
 *
 * The process passed in arg is moved in the terminated list and get the zombie
 * state. Waiting for his parent to read the return register.
 * The return register is set to the KILLED error code.
 * 
 * @param pid the pid of the process to kill
 * @return an error code
 */
int32_t         kkill_pcb(pcb * p);

/**
 * @brief exit the current process and set the return value in the register
 *
 * The process caling exit is moved to the terminated list and is return value
 * is set in the apropriate register. The process get the state OMG_ZOMBIE.
 *
 * @param the returned value to set
 */
void            kexit(int32_t return_value);

/**
 * @brief wake up a pcb.
 * @param the pcb to wake up
 */
void            kwakeup_pcb(pcb * p);

/**
 * @brief wake up a pcb.
 * @param the pid of the pcb to wake up
 */
void            kwakeup(uint32_t pid);

/*
 * Private functions
 */

/*
 * \brief Init the memory for the pcb
 */
void            init_mem();

/**
 * @brief Allocate a space for a pcb, and reset the pcb
 *
 * This function will look for a empty space in the static pcb, reset it
 * and return a pointer to this space.
 *
 * @return a pointer to a pcb
 */
pcb            *alloc_pcb();

/**
 * @brief Deallocte a pcb.
 * @param p the pcb to dealloc
 * @return void
 */
void            dealloc_pcb(pcb * p);

 /**
 * \brief reset the next_pid variable to 0
 * @return void
 */
void            reset_next_pid();

/**
 * @brief reset to -1 all the element of used_stack
 * @return void
 */
void            reset_used_stack();

#endif /* __KPROCESS_H */

/* end of file kprocess.h */
