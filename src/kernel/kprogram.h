/**
 * @file kprogram.h
 * @brief Program structure and function to search program list
 */

#ifndef __KPROGRAM_H
#define __KPROGRAM_H

#include <types.h>

/*
 * Struct
 */

/**
 * @brief A program structure
 *
 * This structure contains the name, the adress, and a description
 * of a program.
 */
typedef struct
{
  char            name[20];     /*!< name of the program */
  void           *f(int argv, char *argc);      /*!< adresse of the main function */
  char            desc[1024];   /*!< A description of the program */
} prgm;

/*
 * Function
 */

/**
 * @brief Search a program into the program list
 * @param *name a pointer to a string, wich represent a possible
 * programm name
 * @return: a pointer to the program, or NULL if not found
 */
prgm           *search(char *name);

/**
 * @brief Print all the program with description
 * @param void
 * @return void
 */
void            print_programs(void);

#endif /* __KPROGRAM_H */
