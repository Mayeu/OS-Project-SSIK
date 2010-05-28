/**
 * \file kscheduler.h
 * \brief Scheduler functions to manage concurrent processes.
 * \author Christophe Carasco
 * \version 0.1
 * \date 20 mai 2010
 *
 */

#ifndef __KSCHEDULER_H
#define __KSCHEDULER_H

/**
 * @brief Schedule the process
 *
 * The process are scheduling using high priority first, and Round Robin when
 * the process have the same priority.
 */
void            schedule();

#endif
