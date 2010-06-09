/**
 * @file kprogram.c
 * @brief Implementation of the kprogram module
 */

#include <string.h>
#include "kprogram.h"
#include "kernel.h"
#include "../user/coquille.h"
#include "../user/fibonacci.h"
#include "../user/philosopher.h"
#include "../user/increment.h"
#include "../user/coquille_up.h"
#include "../user/increment.h"
#include "../user/scroll.h"
#include "../user/quit.h"
#include "../user/ring.h"
#include "../user/supervisor.h"

/*
 * Define
 */

#define NUM_PROG 17

/*
 * Global variable
 */

/*
 * Available programs
 */
static prgm     prgm_ls[NUM_PROG] = {
  /*
   * The init program
   */
  {
   "init",
   (uint32_t) init,
   "Initialize the system."},

  /*
   * The scroll program
   */
  {
   "scroll",
   (uint32_t) scroll,
   "Scroll a string on the malta LCD."},

  /*
   * The chg_prio program
   */
  {
   "chg_prio",
   (uint32_t) chg_prio,
   "Change the priority of a process."},

  /*
   * The ps program
   */
  {
   "ps",
   (uint32_t) ps,
   "Give information about all the processes runing."},
  /*
   * The kill program
   */
  {
   "tuer",
   (uint32_t) tuer,
   "Kill a process."},

  /*
   * The malta program
   */
  {
   "malta",
   (uint32_t) malta,
   "Print a string on the malta LCD."},

  /*
   * The help program
   */
  {
   "help",
   (uint32_t) help,
   "List all the commands available."},

  /*
   * The proc_info program
   */
  {
   "proc_info",
   (uint32_t) proc_info,
   "Return information about a process."},

  /*
   * The shell program
   */
  {
   "coquille",
   (uint32_t) coquille,
   "The shell program"},

  /*
   * Fibonacci program
   */
  {
   "fibonacci",
   (uint32_t) fibonacci,
   "Fibonacci computation program"},

  /*
   * Increment program
   */
  {
   "increment",
   (uint32_t) increment,
   "Increment a number to a limit"},

  /*
   * Ring
   */
  {
   "ring",
   (uint32_t) ring,
   "The ring program"},

  /*
   * Dining Philosopher
   */
  {
   "philos",
   (uint32_t) dining_philosopher,
   "The philosopher problem"},

  /*
   * Philosopher
   */
  {
   "philosopher",
   (uint32_t) philosopher,
   "The philosopher processes in the dining_philospher problem"},

  /*
   * Waiter
   */
  {
   "waiter",
   (uint32_t) waiter,
   "The waiter process in the dining_philosopher problem"},

  /*
   * Supervisor
   */
  {
   "supervisor",
   (uint32_t) supervisor,
   "The supervisor demonstration"},

  /*
   * Quit, a test for quitting a program
   */
  {
   "quit",
   (uint32_t) quit_main,
   "Quit, a test for quitting a program"}
};

/**
 * Search a program into the program list
 * \private
 */
prgm           *
search_prgm(char *name)
{
  uint32_t        i;

  for (i = 0; i < NUM_PROG; i++)
    if (strcmp(name, prgm_ls[i].name) == 0)
      return &(prgm_ls[i]);

  return NULL;
}

/**
 * all the program with description
 * \private
 */
void
print_programs(void)
{

}
