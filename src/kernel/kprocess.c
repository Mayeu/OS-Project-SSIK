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
int32_t         search_psupervised(pcb * p, int8_t pid);
int8_t          get_next_pid(uint8_t * npid);


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
    if (get_next_pid(&p->pid) == OMGROXX)
    {
      strcpy(name, p->name);                            /** TODO: check the program exists */
      p->pri = BAS_PRI;
      p->supervisor = first(&prunning);
      //p->registers = ;                      /** TODO: init the registers */
      p->wait = 0;
      p->error = OMGROXX;
      p->empty = FALSE;

      return p.pid;
    }
    else
      return OUTOPID;
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
  return OMGROXX;

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
  return OMGROXX;

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
  return OMGROXX;

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
  return OMGROXX;

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
    /* look for the first empty (pid = -1) position */
    int             pos = search_psupervised(p, -1);
    if (pos != -1)
    {
      p->supervised[pos] = pid;
      return OMGROXX;
    }
    else
      return FAILNOOB;
  }
}

/**
 * remove a pid from the supervised list of a process
 */
uint8_t
rm_psupervised(pcb * p, uint8_t pid)
{
  if (p == NULL)
    return NULLPTR;
  int             pos = search_psupervised(p, pid);
  if (pos != -1)
  {
    p->supervised[pos] = -1;
    return OMGROXX;
  }
  else
    return INVARG;
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
  return OMGROXX;
}

/**
 * remove the pid from the supervisor a process.
 */
uint8_t
rm_psupervisor(pcb * p, uint8_t pid)
{
  if (p == NULL)
    return NULLPTR;

  p->supervisor = -1;
  return OMGROXX;

}

 /**
 * add a pid to the supervisor list of a process;
 */
// uint8_t add_psupervisor(pcb * p, uint8_t pid)
// {

// }

/**
 * Return whether the pcb is empty or not.
 */
bool
is_empty(pcb * pcb)
{
  if (pcb == NULL)
    FALSE;
  return pcb->empty;
}


/** Functions private to this file */
bool
is_already_supervised(pcb * p, uint8_t pid)
{
  if (search_psupervised(p, pid) == -1)
    return FALSE;
  return TRUE;
}

int32_t
search_psupervised(pcb * p, int8_t pid)
{
  uint32_t        i = 0;

  while (i < NSUPERVISED)
  {
    if (p->supervised[i] == pid)
      return i;
  }
  return -1;
}

int8_t
get_next_pid(uint8_t * npid)
{
  int             init = next_pid;
  while (searchall(next_pid) == NULL)
  {
    next_pid++;
    if (next_pid == init)
      return OUTOPID;
  }
  *npid = next_pid;
  return OMGROXX;

}
