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
#include "kmsg_lst.h"

/**
 * \struct pcb
 * \brief Process representation.
 *
 * A process is represented by its PCB which is made of
 * its identifier, its name ...
 */
typedef struct
{
  uint32_t        pid;          /*!< Process identifier. */
  char            name[ARG_SIZE];       /*!< Process name. */
  uint32_t        pri;          /*!< Process priority. */
  int32_t         supervised[NSUPERVISED];      /*!< List of supervised processes. */
  int32_t         supervisor;   /*!< supervisor. */
  registers_t     registers;    /*!< Some data that has to be saved between
                                   a context switch. */
  mls					messages;		/*!< the message queue */
  int32_t         wait;				/*!< time before waking up or -1 means waiting for a process to terminate */
  uint32_t        wait_for;		/*!< pid of the supervised process that the process is waiting for */
  uint32_t        error;        /*!< Last error the process encountered. */
  bool            empty;			/*!< indicate if the pcb is used (FALSE) or not (TRUE) */
} pcb;

/**
 * \fn create_pcb(pcb *p, int32_t pid, char *name, uint32_t pc, int32_t supervisor, uint32_t prio, char **params)
 * \brief initialize a pcb with all the needed value,
 * 			add it to the ready queue, and ask for a 
			long term scheduling.
 *
 * The allocated space in the ready queue is supposed to be free.
 *
 * \param p the location where to create the pcb
 * \param p the pid
 * \param name the name of the program to launch
 * \param pc the address of the entry point of the program to launch
 * \param supervidor the pid of the supervisor
 * \param prio the priority of the process
 * \return params the params
 */
void create_pcb(pcb *p, int32_t pid, char *name, uint32_t pc, int32_t supervisor, uint32_t prio, char **params);


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
 * \fn int copy_p(pcb *psrc, pcb *pdest)
 * \brief copy a pcb inside an other
 *
 * \param psrc the source pcb
 * \param pdest the destination pcb
 * \return an error code
 */
uint32_t        move_p(pcb * psrc, pcb * pdest);

/**
 * \fn bool p_is_empty(pls *ls)
 * \brief Return whether the list is empty or not
 *
 * \param ls a pointer to the list
 * \return a boolean
 */
bool            p_is_empty(pcb * pcb);

 /**
 * \fn uint32_t rest_pid()
 * \brief reset the next_pid to 0
 *
 */
void            reset_next_pid();


/**
 * \fn char *argn(char **data, int num)
 * \brief Returns the argument at position num
 *
 * \param data the original array of arguments
 * \param num the number of the argument to return
 * \return the argument
 */
char           *argn(char **data, int num);

bool            is_already_supervised(pcb * p, uint32_t pid);
int32_t         search_psupervised(pcb * p, int32_t pid);
#endif
