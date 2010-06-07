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

/*
 * Define
 */

#define NUM_PROG 6

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
   * Philosopher
   */
  {
   "philosopher",
   (uint32_t) philosopher,
   "The philosopher problem"},

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
