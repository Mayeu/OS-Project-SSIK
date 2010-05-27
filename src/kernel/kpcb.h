/**
 * @file kpcb.h
 * @brief Hold pcb related functions. All are getters and setters
 */

#ifndef __KPCB_H
#define __KPCB_H

#include <stdlib.h>
#include <types.h>
#include <registers.h>

/*
 * Define
 */

/**
 * @brief Possible state of a process
 */
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

/*
 * Structure
 */

/**
 * \struct pcb
 * \brief Process representation.
 *
 * A process is represented by its PCB which is made of
 * its identifier, its name ...
 */
typedef struct
{
  registers_t     registers;    /*!< Some data that has to be saved between
                                   a context switch. */
  uint32_t        pid;          /*!< Process identifier. */
  char            name[ARG_SIZE];       /*!< Process name. */
  uint32_t        pri;          /*!< Process priority. */
  int32_t         supervised[MAXPCB];   /*!< List of supervised processes. */
  int32_t         supervisor;   /*!< supervisor. */
  uint32_t        state;        /*!< Current state of the process */
  uint32_t        sleep;        /*!< Time to sleep, if state == SLEEPING */
  uint32_t        waitfor;      /*!< pid of the process you are waiting for */
  int32_t         error;        /*!< Last error the process encountered. */
  bool            empty;        /*!< is this pcb empty ? */
} pcb;

/*
 * Functions
 */

/*
 * Getters
 */

/**
 * @brief Get the pid of the pcb
 */
uint32_t        pcb_get_pid(pcb * p);

/**
 * @brief Get a pointer to the pcb name. !!!WARNING!!! Not safe if you don't
 * do a strcpy just after.
 */
char           *pcb_get_name(pcb * p);

/**
 * @brief Get the priority of the process
 */
uint32_t        pcb_get_pri(pcb * p);

/**
 * @brief Return a pointer to the list of supervised process. !!!WARNING!!!
 * Don't give this pointer to anybody !
 */
int32_t        *pcb_get_supervised(pcb * p);

/**
 * @brief Return the supervisor of the process, if any return -1
 */
int32_t         pcb_get_supervisor(pcb * p);

/**
 * @brief Return a pointer to the registers struct of the process. !!!WARNING!!!
 * Don't give this pointer to anybody !
 */
registers_t    *pcb_get_register(pcb * p);

/**
 * @brief Return the current state of the process
 */
uint32_t        pcb_get_state(pcb * p);

/**
 * @brief return the sleeping time of the process. Not relevante if the process
 * is not sleeping.
 */
uint32_t        pcb_get_sleep(pcb * p);

/**
 * @brief return who the process is waiting for. Not relevante if the process
 * don't wait anybody
 */
uint32_t        pcb_get_waitfor(pcb * p);

/**
 * @brief return the last error encounter by the process
 */
int32_t         pcb_get_error(pcb * p);

/**
 * @brief return if the process is empty or not
 */
bool            pcb_get_empty(pcb * p);

/*
 * Setters
 */
void            pcb_set_register(pcb * p, registers_t * regs);


/**
 * @brief Set a pcb to it's default value
 */
void            pcb_reset(pcb * p);

/**
 * @brief Copy a pcb in an other
 */
void            pcb_cpy(pcb * src, pcb * dest);

/**
 * @brief Set the pid of the pcb
 */
void            pcb_set_pid(pcb * p, uint32_t pid);

/**
 * @brief Set the pcb name. 
 */
void            pcb_set_name(pcb * p, char *name);

/**
 * @brief Set the priority of the process
 */
void            pcb_set_pri(pcb * p, int32_t pri);

/**
 * @brief Reset the list of supervised process to -1.
 */
void            pcb_reset_supervised(pcb * p);

/**
 * @brief Add a pid to the list of supervised process.
 */
int32_t         pcb_set_supervised(pcb * p, int32_t pid);

/**
 * @brief Remove a pid to the list of supervised process.
 */
void            pcb_rm_supervised(pcb * p, uint32_t pid);

/**
 * @brief Set the supervisor of the process
 */
void            pcb_set_supervisor(pcb * p, int32_t pid);

/**
 * @brief Set the current state of the process
 */
void            pcb_set_state(pcb * p, int32_t status);

/**
 * @brief Set the sleeping time of the process. Not relevante if the process
 * is not sleeping.
 */
void            pcb_set_sleep(pcb * p, uint32_t time);

/**
 * @brief Set who the process is waiting for. Not relevante if the process
 * don't wait anybody
 */
void            pcb_set_waitfor(pcb * p, uint32_t pid);

/**
 * @brief Set the last error encounter by the process
 */
void            pcb_set_error(pcb * p, int32_t e);

/**
 * @brief Set if the process is empty or not
 */
void            pcb_set_empty(pcb * p, bool b);

#endif /* __KPCB_H */

/* end of file kpcb.h */
