/**
 * \file kerror.c
 * \brief kernel error functions
 * \author Adrien Forest
 * \version 0.1
 * \date 24 Avril 2010
 */

#include <string.h>

#include "kerror.h"
#include "kinout.h"
#include "kernel.h"

 /**
 * Print the specified err_msg followed by the description of the error according to the global
variable perror.
 */
void
kperror(char *error_msg)
{
  char            res[255];
  char            num[10];

  if (error_msg != NULL)
  {
    strcpy(error_msg, res);
    strcat(res, " : ");
  }
  itos((int)p_error, num);
  strcat(res, num);

  kprint(res);
  kprintn();
}

 /**
 * Return the error number.
 */
uint32_t
kgerror(void)
{
  return (uint32_t)p_error;
}

 /**
 * Set the current error code.
 */
void
kserror(uint32_t e)
{
	prunning.current->error = e;
}
