/**
 * \file kprocess.h
 * \brief Process related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#include "kprocess.h"
#include "kprocess_list.h"
#include "kernel.h"

 /* Privates functions */
bool            is_already_supervised(pcb * p, uint8_t pid);

/**
 * initialize a pcb with all the needed value, add it to
 * the ready queue, and ask for a long term scheduling.
 */
uint8_t
create_proc(char *name, pcb * p)
{
  uint8_t         i = 0;
  if (name == NULL || p == NULL)
    return NULLPTR;

  if (p = empty_space(&pready))
  {
    p->pid = get_next_pid();
    strcpy(name, p->name);                      /** TODO: check the program exists */
    p->pri = BAS_PRI;
    p->supervisor = prunning.first();
    //p->registers = ;                      /** TODO: init the registers */
    p->wait = 0;
    p->error = SUCCESS;
    p->empty = FALSE;

    return p.pid;
  }
  else
    /* No free space */
    return OUTOFMEM;
}

/**
 * deallocate a pcb.
 */
uint8_t
rm_p(pcb * p)
{
  if (p == NULL)
    return NULLPTR;

  p->empty = TRUE;
  return SUCCESS;
}

/**
 * change the priority of a process.
 */
uint8_t
chg_ppri(pcb * p, uint8_t pri)
{
  if (p == NULL)
    return NULLPTR;
  if (pri > MAX_PRI || pri < MIN_PRI)
    return INVARG;

  p->pri = pri;
  return SUCCESS;
}

/**
 * copy and give the information of a pcb into a pcbinfo.
 */
uint8_t
get_pinfo(pcb * p, pcbinfo * pi)
{
  if (p == NULL || pi == NULL)
    return NULLPTR;

  pi->pid = p->pid;
  strcpy(p->name, pi->name);
  pi->pri = p->pri;
  for (i = 0; i < NSUPERVISE; i++)
    pi->supervised[i] = p->supervised[i];
  pi->supervisor = p->supervisor;
  pi->wait = p->wait;
  pi->empty = p->empty;
}

/**
 * copy a pcb inside an other.
 */
uint8_t
copy_p(pcb * psrc, pcb * pdest)
{
  if (psrc == NULL || pdest == NULL)
    return NULLPTR;

  pdest->pid = psrc->pid;
  strcpy(psrc->name, pdest->name);
  pdest->pri = psrc->pri;
  for (i = 0; i < NSUPERVISE; i++)
    pdest->supervises[i] = psrc->supervised[i];
  pdest->supervisor = psrc->supervisor;
  //pdest->registers = psrc->registers;           /** TODO: copy the registers */
  pdest->wait = psrc->wait;
  pdest->error = psrc->error;
  pdest->empty = psrc->empty;
}

/**
 * add a pid to the supervise list of a process;
 */
uint8_t
add_psupervised(pcb * p, uint8_t pid)
{
  if (p == NULL)
    return NULLPTR;
  if (!is_already_supervised(p, pid))
  {

  }


}

/**
 * add a pid to the supervisor list of a process;
 */
uint8_t
chg_psupervisor(pcb * p, uint8_t pid)
{
  if (p == NULL)
    return NULLPTR;
  p->supervisor = pid;
  return SUCCESS;
}

 /**
 * add a pid to the supervisor list of a process;
 */
// uint8_t add_psupervisor(pcb * p, uint8_t pid)
// {

// }

/**
 * remove the pid from the supervisor a process.
 */
uint8_t
rm_psupervisor(pcb * p, uint8_t pid)
{
  if (p == NULL)
    return NULLPTR;

  p->supervisor = -1;
}



/**
 * remove a pid from the supervised list of a process
 */
// uint8_t rm_psupervised(pcb * p, uint8_t pid)
// {
// }


/** Functions private to this file */
bool
is_already_supervised(pcb * p, uint8_t pid)
{

}
