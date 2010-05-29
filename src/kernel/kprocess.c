/**
 * \file kprocess.c
 * \brief Process related functions
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#include <errno.h>
#include <string.h>
#include <process.h>
#include "kprocess.h"
#include "debug.h"
#include "kprogram.h"
#include "kernel.h"
#include "kinout.h"

/*
 * Define
 */

/**
 * @brief Size per pcb of the stack. In a number of uint32_t
 */
#define SIZE_STACK 512

/*
 * Global variable for this module
 */

/**
 * @brief the next possible pid
 */
static uint32_t next_pid = 0;

/**
 * @brief number of PCB in the system
 */
static uint32_t pcb_counter = 0;

/**
 * @brief A big stack for everybody
 */
uint32_t        stack[MAXPCB * SIZE_STACK];

/**
 * @brief Array to hold the used part of the stack. We store -1 if not use,
 * and the pid otherwise
 */
static uint32_t used_stack[MAXPCB];

/**
 * @brief the current pcb
 */
pcb            *current_pcb;

/*
 * Privates functions
 */
bool            is_already_supervised(pcb * p, uint32_t pid);
int32_t         search_psupervised(pcb * p, int32_t pid);
int32_t         get_next_pid();

/*
 * Functions
 */

/**
 * @brief Return the pcb currently running
 * @return A pointer to the pcb
 */
pcb            *
get_current_pcb()
{
  return current_pcb;
}

/**
 * @brief Set the currently running pcb
 * @param the pcb to set
 */
void
set_current_pcb(pcb * p)
{
  current_pcb = p;
}

/**
 * create a pcb with all the needed value at the specified location
 */
uint32_t
create_proc(char *name, uint32_t prio, char **params)
{
  uint32_t       *i;
  int32_t         pid;
  pcb             p;
  prgm           *prg;

  //kdebug_println("Create process in");

  if (name == NULL)
    return NULLPTR;

  if (prio > MAX_PRI || prio < MIN_PRI)
    return INVARG;

  if (params == NULL)
    return NULLPTR;


  if (pcb_counter <= MAXPCB)
  {
    /*
     * Reset the tmp pcb
     */
    pcb_reset(&p);

    /*
     * Check that the program exist
     */
    prg = search_prgm(name);

    if (prg == NULL)
      return INVARG;

    /*
     * init the program counter
     */
    p.registers.epc_reg = (uint32_t) prg->address;

    /*
     * get a pid
     */
    pid = get_next_pid();

    if (pid < 0)
      return pid;               /* contain an error code */

    pcb_set_pid(&p, pid);       /* set the pid */

    pcb_set_pri(&p, prio);      /* set the priority */

    /*
     * Set the supervisor of the process, which is the one we ask for the creation
     * or -1 if the system ask.
     * This value is in the global variable current_pcb
     */
    if (current_pcb != NULL)
      pcb_set_supervisor(&p, pcb_get_pid(current_pcb));
    else
      pcb_set_supervisor(&p, -1);

    /*
     * Set the parameters of the function
     */
    p.registers.a_reg[0] = 0;
    p.registers.a_reg[1] = (uint32_t) & params; /* the adresse of the first arg */

    /*
     * Set the stack pointer
     */

    i = allocate_stack(pcb_get_pid(&p));

    if (i == NULL)
      return OUTOMEM;

    p.registers.sp_reg = (uint32_t) i;  /* set the stack pointer */

    /*
     * Set the state
     */

    pcb_set_state(&p, READY);

    /*
     * Set the last error
     */
    pcb_set_error(&p, OMGROXX);

    /*
     * The pcb is no more empty
     */
    pcb_set_empty(&p, FALSE);

    /*
     * Now we can add the pcb to the ready list
     */
    if (pcls_add(&pclsready, &p) == OUTOMEM)
    {
      /*
       * Adding fail, don't forget to dealloc every allocated stuff
       */
      deallocate_stack(pcb_get_pid(&p));
      pcb_reset(&p);
      return OUTOMEM;
    }

    /*
     * Everything goes well, we add one to the pcb_counter
     */
    pcb_counter++;

  }
  else
    return OUTOMEM;

  //kdebug_println("Create process out");

  return pid;
}

/**
 * Search all the list to found a pcb.
 */
pcb            *
search_all_list(uint32_t pid)
{
  pcls_item      *p;

  p = pcls_search_pid(&pclsready, pid);

  if (p)
    return &(p->p);

  p = pcls_search_pid(&pclsrunning, pid);

  if (p)
    return &(p->p);

  p = pcls_search_pid(&pclswaiting, pid);

  if (p)
    return &(p->p);

  p = pcls_search_pid(&pclsterminate, pid);

  if (p)
    return &(p->p);

  return NULL;
}

/**
 * @brief tohautnhenot
 * @private
 */
uint32_t
rm_p(pcb * p)
{
  if (p == NULL)
    return NULLPTR;

  //pcls_delete_pcb(p);

  pcb_counter--;

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

  pcb_set_pri(p, pri);
  return OMGROXX;
}

/**
 * copy and give the information of a pcb into a pcbinfo.
 */
uint32_t
get_pinfo(pcb * p, pcbinfo * pi)
{
  kprint("DEPRECATED: get_pinfo don't do anything anymore !");

  return FAILNOOB;
}

/*
 * copy and give the information of a pcb into a pcbinfo.
 */
uint32_t
get_pinfo2(pcb * p, pcbinfo2 * pi)
{
  uint32_t        i;

  if (p == NULL || pi == NULL)
    return NULLPTR;

  pi->pid = p->pid;
  strcpy(p->name, pi->name);
  pi->pri = p->pri;

  for (i = 0; i < MAXPCB; i++)
    pi->supervised[i] = pcb_get_supervised(p)[i];

  pi->supervisor = pcb_get_supervisor(p);
  pi->state = pcb_get_state(p);
  pi->sleep = pcb_get_sleep(p);
  pi->waitfor = pcb_get_waitfor(p);
  pi->error = pcb_get_error(p);
  pi->empty = pcb_get_empty(p);

  return OMGROXX;
}

/*
 * add a pid to the supervise list of a process;
 */

uint32_t
add_psupervised(pcb * p, uint32_t pid)
{
  //kprintln("DEPRECATED: redondant with pcb_set_supervised in kpcb.h");
  pcb            *supervised;

  if (p == NULL)
    return NULLPTR;

  if ((supervised = search_all_list(pid)) == NULL)
    return INVARG;

  if (supervised->supervisor != -1)
    return INVARG;

  return pcb_set_supervised(p, pid);
}

/*
 * remove a pid from the supervised list of a process
 */
uint32_t
rm_psupervised(pcb * p, uint32_t pid)
{
  if (p == NULL)
    return NULLPTR;

  pcb_rm_supervised(p, pid);

  return OMGROXX;
}

/*
 * Return whether the pcb is empty or not.
 */
bool
p_is_empty(pcb * pcb)
{
  if (pcb == NULL)
    return FALSE;
  return pcb_get_empty(pcb);
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

  while (i < MAXPCB)
  {
    if (p->supervised[i] == pid)
      return i;
    i++;
  }
  return -1;
}

/**
 * @brief Return the next avaible pid, or an error code
 */

int32_t
get_next_pid()
{
  int             init = next_pid;
  while (search_all_list(next_pid) != NULL)
  {
    next_pid++;
    if (next_pid == init)
      return OUTOPID;
  }
  return next_pid;
}

/**
 * @brief Return a pointer to a stack. The pointer is set to point at the bottom of the stack
 * since the stack grow from the bottom.
 * We need the pid to mark the stack as used by this pid
 */
uint32_t       *
allocate_stack(uint32_t pid)
{
  uint32_t        i;

  i = 0;
  while (used_stack[i] != -1 && i < MAXPCB)
    i++;

  if (used_stack[i] == -1)
  {
    used_stack[i] = pid;
    return &(stack[(i + 1) * SIZE_STACK]);
  }

  return NULL;
}

/**
 * @brief Dealloc a stack, in our case consist to change used_stack[pid] to -1
 */

int32_t
deallocate_stack(uint32_t pid)
{
  uint32_t        i;

  i = 0;
  while (used_stack[i] != pid && i < MAXPCB)
    i++;

  if (used_stack[i] == 32)
  {
    used_stack[i] = -1;
    return OMGROXX;
  }

  return NOTFOUND;
}

/**
 * @brief reset to -1 all the element of used_stack
 */
void
reset_used_stack()
{
  uint32_t        i;

  for (i = 0; i < MAXPCB; i++)
    used_stack[i] = -1;
}

/**
 * reset the next_pid to 0
 */
void
reset_next_pid()
{
  next_pid = 0;
}

int32_t        *
get_used_stack()
{
  return used_stack;
}

char           *
argn(char **data, int num)
{
  return (char *) (data + num * (ARG_SIZE / sizeof(char *)));
}
