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

char            err_msgs[13][255] = {
  "No error occured",
  "General error",
  "Invalid ID",
  "Invalid argument",
  "Null pointer error",
  "Out of mid (number of messages is limited)",
  "Out of pid (number of processes is limited)",
  "Invalid priority",
  "Unknown mid (message identifier)",
  "Unknown pid (process identifier)",
  "Out of memory",
  "These are not the droid you are looking for",
  "Process has been killed"
};

 /**
 * Print the specified err_msg followed by the description of the error according to the global variable perror.
 * \private
 */
void
kperror(char *error_msg)
{
  char            res[255];
  char            num[3];
  int             err_num = (int) *p_error;

  res[0] = '\0';
  if (error_msg != NULL)
  {
    strcpy(error_msg, res);
    strcat(res, " : ");
  }

  strcat(res, err_msgs[-err_num]);
  strcat(res, " (");

  strcat(res, itos(err_num, num));

  strcat(res, ")");

  kprint(res);
  kprintn();
}

 /**
 * Return the error number.
 * \private
 */
uint32_t
kgerror(void)
{
  return (uint32_t) p_error;
}

 /**
 * Set the current error code.
 * \private
 */
void
kserror(uint32_t e)
{
  *p_error = e;
}
