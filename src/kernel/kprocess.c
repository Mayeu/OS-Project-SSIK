/**
 * \file kprocess.h
 * \brief Process related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#include <errno.h>
#include <string.h>
#include "kprocess.h"


/**
 * create a pcb with all the needed value at the specified location
 */
void
create_pcb(pcb *p, int32_t pid, char *name, uint32_t pc, int32_t supervisor, uint32_t prio, char **params)
{
	int i;
	p->pid = pid;
  	strcpy(name, p->name);
 	p->pri = prio;
	p->supervisor = supervisor;
	for (i = 0; i < NSUPERVISED; i++)
        p->supervised[i] = -1;
            /** TODO: init the registers */
	p->registers.epc_reg = pc;
	// init the parameters
	p->registers.a_reg[0] = (uint32_t) params;
	init_msg_lst(&p->messages);
	p->wait = 0;
	p->error = OMGROXX;
	p->empty = FALSE;
}

/**
 * deallocate a pcb.
 */
uint32_t
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
uint32_t
chg_ppri(pcb * p, uint32_t pri)
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
uint32_t
get_pinfo(pcb * p, pcbinfo * pi)
{
  uint32_t        i;
  if (p == NULL || pi == NULL)
    return NULLPTR;

  pi->pid = p->pid;
  strcpy(p->name, pi->name);
  pi->pri = p->pri;
  for (i = 0; i < NSUPERVISED; i++)
    pi->supervised[i] = p->supervised[i];
  pi->supervisor = p->supervisor;
/*
  pi->wait = p->wait;
  pi->wait = p->wait_for;
  pi->empty = p->empty;*/
  return OMGROXX;

}

/**
 * move a pcb inside an other.
 */
uint32_t
move_p(pcb * psrc, pcb * pdest)
{
  uint32_t        i;
  if (psrc == NULL || pdest == NULL)
    return NULLPTR;
  if (psrc->empty == TRUE)
    return INVARG;

  pdest->pid = psrc->pid;
  strcpy(psrc->name, pdest->name);
  pdest->pri = psrc->pri;
  for (i = 0; i < NSUPERVISED; i++)
    pdest->supervised[i] = psrc->supervised[i];
  pdest->supervisor = psrc->supervisor;
	/** TODO: move the registers */
  pdest->registers.at_reg = psrc->registers.at_reg;  
	for(i=0 ; i<2 ; i++)           
  pdest->registers.v_reg[i] = psrc->registers.v_reg[i];
	for(i=0 ; i<4 ; i++)           
  pdest->registers.a_reg[i] = psrc->registers.a_reg[i];
	for(i=0 ; i<10 ; i++)      
  	pdest->registers.t_reg[i] = psrc->registers.t_reg[0];  
	for(i=0 ; i<8 ; i++)      
  pdest->registers.s_reg[i] = psrc->registers.s_reg[i];
  pdest->registers.sp_reg = psrc->registers.sp_reg;
  pdest->registers.fp_reg = psrc->registers.fp_reg;
  pdest->registers.ra_reg = psrc->registers.ra_reg;
  pdest->registers.sp_reg = psrc->registers.sp_reg;
  pdest->registers.epc_reg = psrc->registers.epc_reg;
  pdest->registers.gp_reg = psrc->registers.gp_reg;

	move_msg_lst(&psrc->messages, &pdest->messages);
  pdest->wait = psrc->wait;
  pdest->wait = psrc->wait_for;
  pdest->error = psrc->error;
  pdest->empty = psrc->empty;

  psrc->empty = TRUE;
  return OMGROXX;
}


/**
 * Return whether the pcb is empty or not.
 */
bool
p_is_empty(pcb * pcb)
{
  if (pcb == NULL)
    return FALSE;
  return pcb->empty;
}


/** Functions private to this file */
bool
is_already_supervised(pcb * p, uint32_t pid)
{
  if (search_psupervised(p, pid) == -1)
    return FALSE;
  return TRUE;
}

int32_t
search_psupervised(pcb * p, int32_t pid)
{
  uint32_t        i = 0;

  while (i < NSUPERVISED)
  {
    if (p->supervised[i] == pid)
      return i;
    i++;
  }
  return -1;
}


char           *
argn(char **data, int num)
{
  return (char *) (data + num * (ARG_SIZE / sizeof(char *)));
}
