/**
 * @file kpcb.c
 * @brief Hold pcb related functions. All are getters and setters
 */

#include <stdlib.h>
#include <types.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include "kinout.h"
#include "kpcb.h"


/**
 * \struct pcb
 * \brief Process representation.
 *
 * A process is represented by its PCB which is made of
 * various value
 */
struct _PCB
{
  registers_t     registers;    /*!< Some data that has to be saved between
                                   a context switch. */
  uint32_t        pid;          /*!< Process identifier. */
  char            name[ARG_SIZE];       /*!< Process name. */
  uint32_t        pri;          /*!< Process priority. */
  int32_t         supervised[MAXPCB];   /*!< List of supervised processes. */
  int32_t         supervisor;   /*!< supervisor. */
  mls             messages;     /*!< List of incoming messages. */
  pcb            *prev;         /*!< pointer to the previous process(pcb) in the list where the process is */
  pcb            *next;         /*!< Pointer to the next process(pcb) in the list where the process is. */
  pcb            *head;         /*!< Pointer to the first element of the list in which the process is. */
  uint32_t        state;        /*!< Current state of the process */
  uint32_t        sleep;        /*!< Time to sleep, if state == SLEEPING */
  uint32_t        waitfor;      /*!< pid of the process you are waiting for */
  int32_t         error;        /*!< Last error the process encountered. */
  bool            empty;        /*!< is this pcb empty ? */
};

/*
 * Functions
 */

/*
 * Getters
 */

/**
 * @brief Get the pid of the pcb
 */
uint32_t
pcb_get_pid(pcb * p)
{
  return p->pid;
}

/**
 * @brief Get a pointer to the pcb name. !!!WARNING!!! Not safe if you don't
 * do a strcpy just after.
 */
char           *
pcb_get_name(pcb * p)
{
  return p->name;
}

/**
 * @brief Get the priority of the process
 */
uint32_t
pcb_get_pri(pcb * p)
{
  return p->pri;
}

/**
 * \brief Get the messages of the process
 * \param the pcb to read
 * @return the messages of the pcb
 */
mls            *
pcb_get_messages(pcb * p)
{
  return &p->messages;
}

/**
 * Get the next process in the same list
 */
pcb            *
pcb_get_next(pcb * p)
{
  return p->next;
}

/**
 * Get the previous process in the same list
 */
pcb            *
pcb_get_prev(pcb * p)
{
  return p->prev;
}

/**
 * Get the first process of the list in which the process is 
 * the first process is the same as the address of the list.
 */
pcb            *
pcb_get_head(pcb * p)
{
  return p->head;
}

/**
 * @brief Return a pointer to the list of supervised process. !!!WARNING!!!
 * Don't give this pointer to anybody !
 */
int32_t        *
pcb_get_supervised(pcb * p)
{
  return p->supervised;
}

/**
 * @brief Return the supervisor of the process, if any return -1
 */
int32_t
pcb_get_supervisor(pcb * p)
{
  return p->supervisor;
}

/**
 * @brief Return a pointer to the registers struct of the process. !!!WARNING!!!
 * Don't give this pointer to anybody !
 */
registers_t    *
pcb_get_register(pcb * p)
{
  return &(p->registers);
}

/**
 * @brief Return the value of the epc register.
 * @param the pcb to read
 * @return the value of epc
 */
uint32_t
pcb_get_epc(pcb * p)
{
  return p->registers.epc_reg;
}

/**
 * @brief Return the value of the sp register.
 * @param the pcb to read
 * @return the value of sp
 */
uint32_t
pcb_get_sp(pcb * p)
{
  return p->registers.sp_reg;
}

/**
 * @brief Return the value of the v0 register.
 * @param the pcb to read
 * @return the value of v0
 */
uint32_t
pcb_get_v0(pcb * p)
{
  return p->registers.v_reg[0];
}

/**
 * @brief Return the current state of the process
 */
uint32_t
pcb_get_state(pcb * p)
{
  return p->state;
}

/**
 * @brief return the sleeping time of the process. Not relevante if the process
 * is not sleeping.
 */
uint32_t
pcb_get_sleep(pcb * p)
{
  return p->sleep;
}

/**
 * @brief return who the process is waiting for. Not relevante if the process
 * don't wait anybody
 */
uint32_t
pcb_get_waitfor(pcb * p)
{
  return p->waitfor;
}

/**
 * @brief return the last error encounter by the process
 */
int32_t
pcb_get_error(pcb * p)
{
  return p->error;
}

/**
 * @brief return if the process is empty or not
 */
bool
pcb_get_empty(pcb * p)
{
  return p->empty;
}

/*
 * Setters
 */

/**
 * @brief Set a pcb to it's default value
 */
void
pcb_reset(pcb * p)
{
  pcb_set_pid(p, 0);
  pcb_set_name(p, "");
  pcb_set_pri(p, 0);
  pcb_reset_supervised(p);
  pcb_set_supervisor(p, -1);
  pcb_set_state(p, 0);
  pcb_set_sleep(p, 0);
  pcb_set_waitfor(p, 0);
  pcb_set_error(p, OMGROXX);
  pcb_set_empty(p, TRUE);
}

/**
 * @brief Copy a pcb in an other
 */
void
pcb_cpy(pcb * src, pcb * dest)
{
  int32_t        *s, i;

  pcb_set_pid(dest, pcb_get_pid(src));
  pcb_set_name(dest, pcb_get_name(src));
  pcb_set_pri(dest, pcb_get_pri(src));
  pcb_reset_supervised(dest);
  s = pcb_get_supervised(src);

  for (i = 0; i < MAXPCB; i++)
    pcb_set_supervised(dest, s[i]);

  pcb_set_supervisor(dest, pcb_get_supervisor(src));
  pcb_set_state(dest, pcb_get_state(src));
  pcb_set_sleep(dest, pcb_get_sleep(src));
  pcb_set_waitfor(dest, pcb_get_waitfor(src));
  pcb_set_error(dest, pcb_get_error(src));
  pcb_set_empty(dest, pcb_get_empty(src));
  pcb_set_register(dest, &(src->registers));
}

/**
 * @brief Set the pid of the pcb
 */
void
pcb_set_pid(pcb * p, uint32_t pid)
{
  p->pid = pid;
}

/**
 * @brief Set the pcb name. 
 */
void
pcb_set_name(pcb * p, char *name)
{
  /*
   * We only have ARG_SIZE -1 letters
   */
  if (strlen(name) > ARG_SIZE - 1)
    strcpyn(name, p->name, ARG_SIZE - 1);
  else
    strcpy(name, p->name);
}

/**
 * @brief Set the priority of the process
 */
void
pcb_set_pri(pcb * p, int32_t pri)
{
  p->pri = pri;
}

/**
 * \brief Set the next process in the same list
 * \param the pcb to read
 * \param the next process address
 */
void
pcb_set_next(pcb * p, pcb * next)
{
  p->next = next;
}

/**
 * \brief Set the previous process in the same list
 * \param the pcb to read
 * \param the previous process address
 */
void
pcb_set_prev(pcb * p, pcb * prev)
{
  p->prev = prev;
}

/**
 * \brief Set the first process in the same list
 * \param the pcb to read
 * \param the first process address
 */
void
pcb_set_head(pcb * p, pcb * head)
{
  p->head = head;
}

/**
 * @brief Reset the list of supervised process to -1.
 */
void
pcb_reset_supervised(pcb * p)
{
  uint32_t        i;

  /*
   * We try to found if the process is already in the list
   * At the same time we save the first empty space
   */
  for (i = 0; i < MAXPCB; i++)
    p->supervised[i] = -1;
}

/**
 * @brief Add a pid to the list of supervised process.
 */
int32_t
pcb_set_supervised(pcb * p, int32_t pid)
{
  int32_t         i;
  int32_t         first_empty;

  /*
   * We try to found if the process is already in the list
   * At the same time we save the first empty space
   */
  i = 0;
  first_empty = -1;
  while (p->supervised[i] != pid && i < MAXPCB)
  {
    if (first_empty == -1 && p->supervised[i] == -1)
      first_empty = i;
    i++;
  }

  /*
   * We reach i and first_empty stayed at -1 : out of memory !
   */
  if (i >= MAXPCB && first_empty == -1)
    return OUTOMEM;

  /*
   * Already inside, not a fail !
   */
  if (first_empty == -1 && p->supervised[i] == pid)
    return OMGROXX;

  /*
   * Any other case
   */
  p->supervised[first_empty] = pid;

  return OMGROXX;
}

/**
 * @brief Remove a pid from the list of supervised process.
 */
void
pcb_rm_supervised(pcb * p, uint32_t pid)
{
  uint32_t        i;

  i = 0;
  while (p->supervised[i] != pid && i < MAXPCB)
    i++;

  /*
   * We found it, we remove it
   */
  if (p->supervised[i] == pid)
    p->supervised[i] = -1;

  /*
   * We did not found it. But hey not anymore :)
   */
}

void
pcb_set_register(pcb * p, registers_t * regs)
{
  uint32_t        i;

  p->registers.at_reg = regs->at_reg;

  for (i = 0; i < 2; i++)
    p->registers.v_reg[i] = regs->v_reg[i];

  for (i = 0; i < 4; i++)
    p->registers.a_reg[i] = regs->a_reg[i];

  for (i = 0; i < 10; i++)
    p->registers.t_reg[i] = regs->t_reg[0];

  for (i = 0; i < 8; i++)
    p->registers.s_reg[i] = regs->s_reg[i];

  p->registers.fp_reg = regs->fp_reg;
  p->registers.ra_reg = regs->ra_reg;
  p->registers.sp_reg = regs->sp_reg;
  p->registers.epc_reg = regs->epc_reg;
  p->registers.gp_reg = regs->gp_reg;

}

/**
 * @brief Set the supervisor of the process
 */
void
pcb_set_supervisor(pcb * p, int32_t pid)
{
  p->supervisor = pid;
}

/**
 * @brief Set the current state of the process
 */
void
pcb_set_state(pcb * p, int32_t status)
{
  p->state = status;
}

/**
 * @brief Set the epc register of the process
 * @param the pcb to write
 * @param the new value of the epc of the pcb
 */
void
pcb_set_epc(pcb * p, uint32_t epc)
{
  p->registers.epc_reg = epc;
}

/**
 * @brief Set the sp register of the process
 * @param the pcb to write
 * @param the new value of the sp of the pcb
 */
void
pcb_set_sp(pcb * p, uint32_t sp)
{
  p->registers.sp_reg = sp;
}

/**
 * @brief Set the v0 register of the process
 * @param the pcb to write
 * @param the new value of the v0 of the pcb
 */
void
pcb_set_v0(pcb * p, uint32_t v0)
{
  p->registers.v_reg[0] = v0;
}

/**
 * @brief Set the sleeping time of the process. Not relevante if the process
 * is not sleeping.
 */
void
pcb_set_sleep(pcb * p, uint32_t time)
{
  p->sleep = time;
}

/**
 * @brief Set who the process is waiting for. Not relevante if the process
 * don't wait anybody
 */
void
pcb_set_waitfor(pcb * p, uint32_t pid)
{
  p->waitfor = pid;
}

/**
 * @brief Set the last error encounter by the process
 */
void
pcb_set_error(pcb * p, int32_t e)
{
  p->error = e;
}

/**
 * @brief Set if the process is empty or not
 */
void
pcb_set_empty(pcb * p, bool b)
{
  p->empty = b;
}

/* end of file kpcb.h */
