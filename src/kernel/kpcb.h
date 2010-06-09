/**
 * @file kpcb.h
 * @brief Hold pcb related functions. All are getters and setters
 */

#ifndef __KPCB_H
#define __KPCB_H

#include <stdlib.h>
#include <types.h>
#include <registers.h>
#include <process.h>
#include "kmsg.h"

/*
 * Define
 */

/**
 * @brief Possible state of a process. This is hold by the pcb.
 */
#ifndef __PROCESS_STATE
#define __PROCESS_STATE
enum
{
  READY,
  RUNNING,
  BLOCKED,
  SLEEPING,
  WAITING_IO,
  DOING_IO,
  WAITING_PCB,
  OMG_ZOMBIE
};
#endif

/*
 * Structure
 */

/**
 * @brief declaration of the pcb list type, the definition is in kprocess_list.h
 */
struct _PLS;

/**
 * \struct pcb
 * \brief Process representation.
 *
 * A process is represented by its PCB which is made of
 * various value
 */
typedef struct _PCB
{
  registers_t     registers;    /*!< Some data that has to be saved between
                                   a context switch. */
  uint32_t        pid;          /*!< Process identifier. */
  char            name[ARG_SIZE];       /*!< Process name. */
  uint32_t        pri;          /*!< Process priority. */
  int32_t         supervised[MAXPCB];   /*!< List of supervised processes. */
  int32_t         supervisor;   /*!< supervisor. */
  mls             messages;     /*!< List of incoming messages. */
  struct _PLS    *head;
  struct _PCB    *prev;         /*!< pointer to the previous process(pcb) in the list where the process is */
  struct _PCB    *next;         /*!< Pointer to the next process(pcb) in the list where the process is. */
  uint32_t        state;        /*!< Current state of the process */
  uint32_t        sleep;        /*!< Time to sleep, if state == SLEEPING */
  uint32_t        waitfor;      /*!< pid of the process you are waiting for */
  int32_t         error;        /*!< Last error the process encountered. */
  bool            empty;        /*!< is this pcb empty ? */
  int32_t         ret;          /*!< return value */
} pcb;

/*
 * Functions
 */

/*
 * Getters
 */

/**
 * @brief Get the pid of the pcb
 * @param the pcb to read
 * @return the pid
 */
uint32_t        pcb_get_pid(pcb * p);

/**
 * @brief Get a pointer to the pcb name. !!!WARNING!!! Not safe if you don't
 * do a strcpy just after.
 * @param the pcb to read
 * @return a pointer to the name
 */
char           *pcb_get_name(pcb * p);

/**
 * @brief Get the priority of the process
 * @param the pcb to read
 * @return the priority of the pcb
 */
uint32_t        pcb_get_pri(pcb * p);

/**
 * \brief Get the messages of the process
 * \param the pcb to read
 * @return the messages of the pcb
 */
mls            *pcb_get_messages(pcb * p);

/**
 * Get the list where the process is
 * \param the pcb to read
 * \return the list
 */
struct _PLS    *pcb_get_head(pcb * p);

/**
 * \brief Get the next process in the same list
 * \param the pcb to read
 * @return the next process
 */
pcb            *pcb_get_next(pcb * p);

/**
 * \brief Get the previous process in the same list
 * \param the pcb to read
 * @return the previous process
 */
pcb            *pcb_get_prev(pcb * p);

/**
 * @brief Return a pointer to the list of supervised process. !!!WARNING!!!
 * Don't give this pointer to anybody !
 * @param the pcb to read
 * @return a pointer to the array of supervised element
 */
int32_t        *pcb_get_supervised(pcb * p);

/**
 * @brief Return the supervisor of the process, if any return -1
 * @param the pcb to read
 * @return the pid of the supervisor
 */
int32_t         pcb_get_supervisor(pcb * p);

/**
 * @brief Return a pointer to the registers struct of the process. !!!WARNING!!!
 * Don't give this pointer to anybody !
 * @param the pcb to read
 * @return a pointer to a registers_t structure
 */
registers_t    *pcb_get_register(pcb * p);

/**
 * @brief Return the value of the epc register.
 * @param the pcb to read
 * @return the value of epc
 */
uint32_t        pcb_get_epc(pcb * p);

/**
 * @brief Return the value of the sp register.
 * @param the pcb to read
 * @return the value of sp
 */
uint32_t        pcb_get_sp(pcb * p);

/**
 * @brief Return the value of the v0 register.
 * @param the pcb to read
 * @return the value of v0
 */
uint32_t        pcb_get_v0(pcb * p);

/**
 * @brief Return the current state of the process
 * @param the pcb to read
 * @return the current state of the pcb
 */
uint32_t        pcb_get_state(pcb * p);

/**
 * @brief return the sleeping time of the process. Not relevante if the process
 * is not sleeping.
 * @param the pcb to read
 * @return the time to sleep
 */
uint32_t        pcb_get_sleep(pcb * p);

/**
 * @brief return who the process is waiting for. Not relevante if the process
 * don't wait anybody
 * @param the pcb to read
 * @return who the pcb is waiting
 */
uint32_t        pcb_get_waitfor(pcb * p);

/**
 * @brief return the last error encounter by the process
 * @param the pcb to read
 * @return the last error code encountered by the process
 */
int32_t         pcb_get_error(pcb * p);

/**
 * @brief return if the process is empty or not
 * @param the pcb to read
 * @return TRUE if empty, FALSE otherwise
 */
bool            pcb_get_empty(pcb * p);

/**
 * @brief return the ret value of the process
 * @param the pcb to read
 * @return the content of pcb->ret
 */
int32_t         pcb_get_ret(pcb * p);

/*
 * Setters
 */

/**
 * @brief Set the registers of the pcb
 * @param the pcb to write
 * @param the value of the registers
 */
void            pcb_set_register(pcb * p, registers_t * regs);

/**
 * @brief Set a pcb to it's default value
 * @param the pcb to reset
 */
void            pcb_reset(pcb * p);

/**
 * @brief Copy a pcb in an other
 * @param the source pcb
 * @param the destination pcb
 */
void            pcb_cpy(pcb * src, pcb * dest);

/**
 * @brief Set the pid of the pcb
 * @param the pcb to write
 * @param the pid of the pcb
 */
void            pcb_set_pid(pcb * p, uint32_t pid);

/**
 * @brief Set the pcb name. 
 * @param the pcb to write
 * @param the new name to set
 */
void            pcb_set_name(pcb * p, char *name);

/**
 * @brief Set the priority of the process
 * @param the pcb to write
 * @param the new priority to set
 */
void            pcb_set_pri(pcb * p, int32_t pri);

/**
 * \brief Set the list in which the process is
 * \param the pcb to read
 * \param the list
 */
void            pcb_set_head(pcb * p, struct _PLS *pls);

/**
 * \brief Set the next process in the same list
 * \param the pcb to read
 * \param the next process address
 */
void            pcb_set_next(pcb * p, pcb * next);

/**
 * \brief Set the previous process in the same list
 * \param the pcb to read
 * \param the previous process address
 */
void            pcb_set_prev(pcb * p, pcb * prev);

/**
 * @brief Reset the list of supervised process to -1.
 * @param the pcb to write
 */
void            pcb_reset_supervised(pcb * p);

/**
 * @brief Add a pid to the list of supervised process.
 * @param the pcb to write
 * @param pid to add in the supervised list
 * @return OMGROXX if everything goes well, OUTOFMEM if
 * there is no empty space
 */
int32_t         pcb_set_supervised(pcb * p, int32_t pid);

/**
 * @brief Remove a pid from the list of supervised process.
 * @param the pcb to write
 * @param the pid to remove
 */
void            pcb_rm_supervised(pcb * p, uint32_t pid);

/**
 * @brief Set the supervisor of the process
 * @param the pcb to write
 * @param the pid to set as supervisor
 */
void            pcb_set_supervisor(pcb * p, int32_t pid);

/**
 * @brief Set the current state of the process
 * @param the pcb to write
 * @param the new status of the pcb
 */
void            pcb_set_state(pcb * p, int32_t status);

/**
 * @brief Set the epc register of the process
 * @param the pcb to write
 * @param the new value of the epc of the pcb
 */
void            pcb_set_epc(pcb * p, uint32_t epc);

/**
 * @brief Set the sp register of the process
 * @param the pcb to write
 * @param the new value of the sp of the pcb
 */
void            pcb_set_sp(pcb * p, uint32_t sp);

/**
 * @brief Set the v0 register of the process
 * @param the pcb to write
 * @param the new value of the v0 of the pcb
 */
void            pcb_set_v0(pcb * p, uint32_t v0);

/**
 * @brief Set the sleeping time of the process. Not relevante if the process
 * is not sleeping.
 * @param the pcb to write
 * @param the new time to set
 */
void            pcb_set_sleep(pcb * p, uint32_t time);

/**
 * @brief Set who the process is waiting for. Not relevante if the process
 * don't wait anybody
 * @param the pcb to write
 * @param the pid to wait
 */
void            pcb_set_waitfor(pcb * p, uint32_t pid);

/**
 * @brief Set the last error encounter by the process
 * @param the pcb to write
 * @param the last encounter error to set
 */
void            pcb_set_error(pcb * p, int32_t e);

/**
 * @brief Set if the process is empty or not
 * @param the pcb to write
 * @param a boolean TRUE = empty
 */
void            pcb_set_empty(pcb * p, bool b);

/**
 * \private
 * @brief set the return value of the pcb
 * @param the pcb to write
 * @param r the return value
 */
void            pcb_set_ret(pcb * p, int32_t r);

#endif /* __KPCB_H */

/* end of file kpcb.h */
