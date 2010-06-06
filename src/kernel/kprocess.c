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
#include "kscheduler.h"

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
static uint32_t stack[MAXPCB * SIZE_STACK];

/**
 * @brief Array to hold the used part of the stack. We store -1 if not use,
 * and the pid otherwise
 */
static uint32_t used_stack[MAXPCB];

/**
 * @brief The pcb memory
 */
static pcb      pmem[MAXPCB];

/**
 * @brief the current pcb
 */
pcb            *current_pcb;

/*
 * \private
 * Privates functions
 */
int32_t         get_next_pid();
int32_t        *get_used_stack();
uint32_t       *allocate_stack(uint32_t pid);
int32_t         deallocate_stack(uint32_t pid);
//pcb*       allocate_pcb();

/*
 * Functions
 */

/**
 * \private
 * @brief Return the pcb currently running
 * @return A pointer to the pcb
 */
pcb            *
get_current_pcb()
{
  return current_pcb;
}

/**
 * \private
 * @brief Set the currently running pcb
 * @param the pcb to set
 */
void
set_current_pcb(pcb * p)
{
  current_pcb = p;
}

/**
 * \private
 * create a pcb with all the needed value at the specified location
 */
uint32_t
create_proc(char *name, uint32_t prio, char **params)
{
  uint32_t       *i;
  int32_t         pid;
  pcb            *p;
  prgm           *prg;

  //kdebug_println("Create process in");

  if (name == NULL)
    return NULLPTR;

  if (prio > MAX_PRI || prio < MIN_PRI)
    return INVARG;


  if (pcb_counter <= MAXPCB)
  {
    /*
     * Allocate a pcb
     */
    p = alloc_pcb();

    if (p == NULL)
      return OUTOMEM;

    /*
     * Reset the tmp pcb
     */
    pcb_reset(p);

    /*
     * Check that the program exist
     */
    prg = search_prgm(name);

    if (prg == NULL)
      return INVARG;

    /*
     * Set the name
     */
    pcb_set_name(p, name);

    /*
     * init the program counter
     */
    pcb_set_epc(p, (uint32_t) prg->address);

    /*
     * get a pid
     */
    pid = get_next_pid();

    if (pid < 0)
      return pid;               /* contain an error code */

    pcb_set_pid(p, pid);        /* set the pid */

    pcb_set_pri(p, prio);       /* set the priority */

    /*
     * Set the supervisor of the process, which is the one we ask for the creation
     * or -1 if the system ask.
     * This value is in the global variable current_pcb
     */
    if (current_pcb != NULL)
      pcb_set_supervisor(p, pcb_get_pid(current_pcb));
    else
      pcb_set_supervisor(p, -1);

    /*
     * Set the parameters of the function
     */
    p->registers.a_reg[0] = 0;
    p->registers.a_reg[1] = (uint32_t) & params;        /* the adresse of the first arg */

    /*
     * Set the stack pointer
     */

    i = allocate_stack(pcb_get_pid(p));

    if (i == NULL)
      return OUTOMEM;

    pcb_set_sp(p, (uint32_t) i);        /* set the stack pointer */

    /*
     * Set the state
     */

    pcb_set_state(p, READY);

    /*
     * Set the last error
     */
    pcb_set_error(p, OMGROXX);

    /*
     * The pcb is no more empty
     */
    pcb_set_empty(p, FALSE);

    /*
     * Now we can add the pcb to the ready list
     */
    if (pls_add(&plsready, p) == OUTOMEM)
    {
      /*
       * Adding fail, don't forget to dealloc every allocated stuff
       */
      deallocate_stack(pcb_get_pid(p));
      pcb_reset(p);
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
 * \private
 * Search all the list to found a pcb.
 */
pcb            *
search_all_list(uint32_t pid)
{
  pcb            *p;

  p = pls_search_pid(&plsready, pid);

  if (p)
    return p;

  p = pls_search_pid(&plsrunning, pid);

  if (p)
    return p;

  p = pls_search_pid(&plswaiting, pid);

  if (p)
    return p;

  p = pls_search_pid(&plsterminate, pid);

  if (p)
    return p;

  return NULL;
}

/**
 * \private
 * @brief tohautnhenot
 * @private
 */
uint32_t
rm_p(pcb * p)
{
  if (p == NULL)
    return NULLPTR;

  //pls_delete_pcb(p);

  pcb_counter--;

  return OMGROXX;
}

/**
 * \private
 * change the priority of a process.
 */
uint32_t
chg_ppri(uint32_t pid, uint32_t pri)
{
  pcb            *p;
  if (p == NULL)
    return NULLPTR;

  if (pri > MAX_PRI || pri < MIN_PRI)
    return INVARG;

  p = search_all_list(pid);

  if (p == NULL)
    return NOTFOUND;

  pcb_set_pri(p, pri);

  return OMGROXX;
}

/*
 * \private
 * copy and give the information of a pcb into a pcbinfo.
 */
uint32_t
get_pinfo(uint32_t pid, pcbinfo * pi)
{
  pcb            *p;
  uint32_t        i;

  p = search_all_list(pid);

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

/**
 * \brief Returns an array of all the processes in the system (in all lists).
 *
 * \param tab the table of pids
 * \return the number of pids
 */
uint32_t
get_all_pid(uint32_t * tab)
{
  return pcb_counter;
}


/*
 * \private
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
 * \private
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
 * @brief sleep the current_process
 */
uint32_t
go_to_sleep(uint32_t time)
{
  pcb            *p;

  //kdebug_println("go to sleep in");

  p = get_current_pcb();

  if (p == NULL)
  {
    //kprintln("bubuuu");
    return FAILNOOB;
  }

  pcb_set_state(p, SLEEPING);
  pcb_set_sleep(p, time * timer_msec);
  pls_move_pcb(p, &plswaiting);

  schedule();

  //kdebug_println("go to sleep out");

  return OMGROXX;
}

/**
 * @private
 * @brief Block a pcb. A blocked pcb can not execute code until he wake up
 *
 * Block take different kind of state in argument to allow different kind
 * of blocking (state describe in kpcb.h) :
 *     - BLOCKED
 *     - WAITING_IO
 *     - DOING_IO
 * Only this state are accepted as blocked.
 * If the pcb wich is blocked is the current pcb, schedule is called.
 *
 * @param p the process to block
 * @param state the state to set the process
 * @return an error code
 */
int32_t
kblock(uint32_t pid, int32_t state)
{
  pcb            *p = search_all_list(pid);
  return kblock_pcb(p, state);
}

/**
 * @private
 * @brief Block a pcb. A blocked pcb can not execute code until he wake up
 *
 * Block take different kind of state in argument to allow different kind
 * of blocking (state describe in kpcb.h) :
 *     - BLOCKED
 *     - WAITING_IO
 *     - DOING_IO
 * Only this state are accepted as blocked.
 * If the pcb wich is blocked is the current pcb, schedule is called.
 *
 * @param p the process to block
 * @param state the state to set the process
 * @return an error code
 */
int32_t
kblock_pcb(pcb * p, int32_t state)
{
  if (p == NULL)
    return FAILNOOB;

  pcb_set_state(p, state);
  pls_move_pcb(p, &plswaiting);

  if (pcb_get_pid(get_current_pcb()) == pcb_get_pid(p))
    schedule();

  return OMGROXX;
}

/**
 * @brief Set the currently used pcb to wait for an other pcb to terminate
 *
 * The waitfor field in the pcb will be get the pid to wait.
 * And the process will be move in the waiting list with the state
 * WAITING_PCB.
 * After this sechedule is called.
 *
 * @param pid the pid to wait
 * @return an error code
 */
int32_t
waitfor(uint32_t pid)
{
  pcb            *p;

  p = search_all_list(pid);

  if (p == NULL)
    return FAILNOOB;

  pcb_set_state(p, WAITING_PCB);
  pls_move_pcb(p, &plswaiting);

  return OMGROXX;
}

/**
 * @brief Kill the current process
 *
 * The process passed in arg is moved in the terminated list and get the zombie
 * state. Waiting for his parent to read the return register.
 * The return register is set to the KILLED error code.
 * 
 * @param pid the pid of the process to kill
 * @return an error code
 */
int32_t
kkill(uint32_t pid)
{
  pcb            *p = search_all_list(pid);
  return kkill_pcb(p);
}

/**
 * @brief Kill the current process
 *
 * The process passed in arg is moved in the terminated list and get the zombie
 * state. Waiting for his parent to read the return register.
 * The return register is set to the KILLED error code.
 * 
 * @param pid the pid of the process to kill
 * @return an error code
 */
int32_t
kkill_pcb(pcb * p)
{
  //int i = 0;
  pcb            *pi = plswaiting.start;

  pcb_set_state(p, OMG_ZOMBIE);
  pls_move_pcb(p, &plsterminate);

  while (pi != NULL)
  {
    /* Check for the supervisor to wake it up */
    if (pcb_get_state(pi) == WAITING_PCB
        && pcb_get_waitfor(pi) == pcb_get_pid(p))
    {
      kwakeup(pcb_get_pid(pi));
      return OMGROXX;
    }
  }

  /* TODO : make the child adopted by the init */

  return OMGROXX;
}

/**
 * @brief exit the current process and set the return value in the register
 *
 * The process caling exit is moved to the terminated list and is return value
 * is set in the apropriate register. The process get the state OMG_ZOMBIE.
 *
 * @param the returned value to set
 */
void
kexit(int32_t return_value)
{
  /* TODO */
}

/**
 * @brief 
 * @param the returned value to set
 */
void
kwakeup_pcb(pcb * p)
{
  if (p == NULL)
    return;

  pcb_set_state(p, READY);
  pls_move_pcb(p, &plsready);

  return;
}

/**
 * @brief 
 * @param the returned value to set
 */
void
kwakeup(uint32_t pid)
{
  pcb            *p = search_all_list(pid);
  kwakeup_pcb(p);
}

/*
 * \private
 * Return whether the pcb is empty or not.
 */
bool
p_is_empty(pcb * pcb)
{
  if (pcb == NULL)
    return FALSE;
  return pcb_get_empty(pcb);
}

void
init_mem()
{
  uint32_t        i;

  for (i = 0; i < MAXPCB; i++)
    pcb_reset(&pmem[i]);
}

/*
 * Functions private to this file
 */

/**
 * \private
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
 * \private
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
 * \private
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
 * \private
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
 * \private
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

pcb            *
alloc_pcb()
{
  uint32_t        i;

  i = 0;
  while (pcb_get_empty(&pmem[i]) && i < MAXPCB)
    i++;

  if (i >= MAXPCB)
    return NULL;

  return &pmem[i];
}

char           *
argn(char **data, int num)
{
  return (char *) (data + num * (ARG_SIZE / sizeof(char *)));
}
