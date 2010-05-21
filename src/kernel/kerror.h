/**
 * \file kerror.h
 * \brief kernel error functions
 * \author Adrien Forest
 * \version 0.1
 * \date 21 Mai 2010
 */

#ifndef __KERROR_H
#define __KERROR_H

#include <kernel.h>
#include <types.h>

 /**
 * \fn void kperror(char *error_msg)
 * \brief Print the specified err_msg followed by the description of the error according to the global
variable perror.
 *
 * \param error_msg error message to print
 */
void            kperror(char *error_msg);

 /**
 * \fn int kgerror(void)
 * \brief Return the error number
 *
 * \return the error number
 */
uint8_t         kgerror(void);

 /**
 * \fn void kserror(int32_t e)
 * \brief Set the current error code.
 *
 * \param e the error number to set
 */
void            kserror(uint8_t e);

#endif /* __KERROR_H */
