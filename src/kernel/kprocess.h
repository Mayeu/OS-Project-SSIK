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

#include "types.h"

#define MIN_PRI 0
#define MAX_PRI 30
#define BAS_PRI 15
#define NSUPERVISED 40

/**
 * \struct pcb
 * \brief Process representation.
 *
 * A process is represented by its PCB which is made of
 * its identifier, its name ...
 */
typedef struct
{
  uint8_t         pid;          /*!< Process identifier. */
  char            name[20];     /*!< Process name. */
  uint8_t         pri;          /*!< Process priority. */
  int16_t         supervised[NSUPERVISED];        /*!< List of supervised processes. */
  int16_t         supervisor;       /*!< supervisor. */
  registers_t     registers;         /*!< Some data that has to be saved between
                                       a context switch. */
  uint32_t        wait;
  uint8_t         error;        /*!< Last error the process encountered. */
  bool            empty;
} pcb;

/**
 * \struct pcbinfo
 * \brief Copy of a process pcb.
 *
 * A copy has to be created and passed to make sure that the real pcb will not
 * be affected in any way.
 */
typedef struct
{
  uint8_t         pid;          /*!< Process identifier. */
  char            name[20];     /*!< Process name. */
  uint8_t         pri;          /*!< Process priority. */
  uint32_t        supervised[NSUPERVISED];        /*!< List of supervised processes. */
  uint32_t        supervisor[NSUPERVISED];       /*!< List of supervisor processes. */
  uint32_t        wait;
  bool            empty;
} pcbinfo;

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
uint8_t create_proc(char *name, pcb * p);

/**
 * \fn int rm_p(pcb *p)
 * \brief deallocate a pcb.
 *
 * \param p the process to delete
 * \return an error code
 */
uint8_t rm_p(pcb * p);

/**
 * \fn int chg_ppri(pcb *p, int pri)
 * \brief change the priority of a process.
 *
 * \param p the pointer to the pcb
 * \param pri the new priority
 * \return an error code
 */
uint8_t chg_ppri(pcb * p, uint8_t pri);

/**
 * \fn int get_pinfo(pcb *p, pcbinfo *pi)
 * \brief copy and give the information of a pcb into a pcbinfo
 *
 * \param p the pointer to the pcb
 * \param pi the pointer to the pcbinfo
 * \return an error code
 */
uint8_t get_pinfo(pcb * p, pcbinfo * pi);

/**
 * \fn int copy_p(pcb *psrc, pcb *pdest)
 * \brief copy a pcb inside an other
 *
 * \param psrc the source pcb
 * \param pdest the destination pcb
 * \return an error code
 */
     uint8_t             copy_p(pcb * psrc, pcb * pdest);

uint8_t chg_psupervisor(pcb * p, uint8_t pid);
	 
/**
 * \fn int add_psupervise(pcb *p, int pid)
 * \brief add a pid to the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to add
 * \return an error code
 */
//     uint8_t             add_psupervised(pcb * p, uint8_t pid);

/**
 * \fn bool is_empty(pcb * p)
 * \brief check whether the pcb space is empty or not
 *
 * \param p the pointer to the process space
 * \return true or false
 */
bool is_empty(pcb * p);	 
	 
	 
/**
 * \fn int add_psupervisor(pcb *p, int pid)
 * \brief add a pid to the supervisor list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to add
 * \return an error code
 */
     uint8_t             add_psupervisor(pcb * p, uint8_t pid);

 /**
 * \fn int rm_psupervise(pcb *p, int pid)
 * \brief remove a pid from the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to remove
 * \return an error code
 */
 //    uint8_t             rm_psupervised(pcb * p, uint8_t pid);

 /**
 * \fn int rm_psuperviser(pcb *p, int pid)
 * \brief remove a pid from the superviser list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to remove
 * \return an error code
 */
     uint8_t             rm_psuperviser(pcb * p, uint8_t pid);

#endif
