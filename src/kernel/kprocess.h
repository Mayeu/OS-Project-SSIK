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


#define NB_REG 30               /* number of registers. */

/**
 * \struct pcb
 * \brief Process representation.
 *
 * A process is represented by its PCB which is made of
 * its identifier, its name ...
 */
typedef struct
{
  int             pid;          /*!< Process identifier. */
  char            name[20];     /*!< Process name. */
  int             pri;          /*!< Process priority. */
  int             supervise[NSUPERVISE];        /*!< List of supervised processes. */
  int             supervisor[NSUPERVISE];       /*!< List of supervisor processes. */
  int register[NB_REG];         /*!< Some data that has to be saved between
                                   a context switch. */
  int             error;        /*!< Last error the process encountered. */
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
  int             pid;          /*!< Process identifier. */
  char            name[20];     /*!< Process name. */
  int             pri;          /*!< Process priority. */
  int             supervise[NSUPERVISE];        /*!< List of supervised processes. */
  int             supervisor[NSUPERVISE];       /*!< List of supervisor processes. */
  int register[N];              /*!< Some data that has to be saved between
                                   a context switch. */
  int             error;        /*!< Last error the process encountered. */
} pcbinfo;

 /**
 * \fn int create_proc(char *name, pcb *p)
 * \brief initialize a pcb with all the needed value,
 * 			add it to the ready queue, and ask for a 
			long term scheduling.
 *
 * \param name the name of the program to launch
 * \param p the pointer to the pcb
 * \return the pid of the newly created process(>0), or an error (<0)
 */
int             create_proc(char *name, pcb * p);

 /**
 * \fn int rm_p(pcb *p)
 * \brief deallocate a pcb.
 *
 * \param p the process to delete
 * \return an error code
 */
int             rm_p(pcb * p);

 /**
 * \fn int chg_ppri(pcb *p, int pri)
 * \brief change the priority of a process.
 *
 * \param p the pointer to the pcb
 * \param pri the new priority
 * \return an error code
 */
int             chg_ppri(pcb * p, int pri);

 /**
 * \fn int get_pinfo(pcb *p, pcbinfo *pi)
 * \brief copy and give the information of a pcb into a pcbinfo
 *
 * \param p the pointer to the pcb
 * \param pi the pointer to the pcbinfo
 * \return an error code
 */
int
get_pinfo(pcb * p, pcbinfo * pi)
 /**
 * \fn int copy_p(pcb *psrc, pcb *pdest)
 * \brief copy a pcb inside an other
 *
 * \param psrc the source pcb
 * \param pdest the destination pcb
 * \return an error code
 */
     int             copy_p(pcb * psrc, pcb * pdest);

 /**
 * \fn int add_psupervise(pcb *p, int pid)
 * \brief add a pid to the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to add
 * \return an error code
 */
     int             add_psupervise(pcb * p, int pid);

 /**
 * \fn int add_psupervisor(pcb *p, int pid)
 * \brief add a pid to the supervisor list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to add
 * \return an error code
 */
     int             add_psupervisor(pcb * p, int pid);

 /**
 * \fn int rm_psupervise(pcb *p, int pid)
 * \brief remove a pid from the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to remove
 * \return an error code
 */
     int             rm_psupervise(pcb * p, int pid);

 /**
 * \fn int rm_psuperviser(pcb *p, int pid)
 * \brief remove a pid from the superviser list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to remove
 * \return an error code
 */
     int             rm_psuperviser(pcb * p, int pid);

#endif
