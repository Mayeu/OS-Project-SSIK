/**
 * \file kprocess.h
 * \brief Process related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 19 Avril 2010
 *
 */

#IFNDEF __KPROCESS_H
#DEFINE __KPROCESS_H

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
  uint32_t        supervise[NSUPERVISE];        /*!< List of supervised processes. */
  uint32_t        supervisor[NSUPERVISE];       /*!< List of supervisor processes. */
  registers_t     registers;         /*!< Some data that has to be saved between
                                       a context switch. */
  uint32_t        wait;
  uint8_t         error;        /*!< Last error the process encountered. */
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
  uint32_t        supervise[NSUPERVISE];        /*!< List of supervised processes. */
  uint32_t        supervisor[NSUPERVISE];       /*!< List of supervisor processes. */
  uint32_t        wait;
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
uint8_t             create_proc(char *name, pcb * p);

 /**
 * \fn int rm_p(pcb *p)
 * \brief deallocate a pcb.
 *
 * \param p the process to delete
 * \return an error code
 */
uint8_t             rm_p(pcb * p);

 /**
 * \fn int chg_ppri(pcb *p, int pri)
 * \brief change the priority of a process.
 *
 * \param p the pointer to the pcb
 * \param pri the new priority
 * \return an error code
 */
uint8_t             chg_ppri(pcb * p, int pri);

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

 /**
 * \fn int add_psupervise(pcb *p, int pid)
 * \brief add a pid to the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to add
 * \return an error code
 */
     uint8_t             add_psupervise(pcb * p, int pid);

 /**
 * \fn int add_psupervisor(pcb *p, int pid)
 * \brief add a pid to the supervisor list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to add
 * \return an error code
 */
     uint8_t             add_psupervisor(pcb * p, int pid);

 /**
 * \fn int rm_psupervise(pcb *p, int pid)
 * \brief remove a pid from the supervise list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to remove
 * \return an error code
 */
     uint8_t             rm_psupervise(pcb * p, int pid);

 /**
 * \fn int rm_psuperviser(pcb *p, int pid)
 * \brief remove a pid from the superviser list of a process
 *
 * \param p the pointer to the process
 * \param pid the pid to remove
 * \return an error code
 */
     uint8_t             rm_psuperviser(pcb * p, int pid);

#ENDIF
