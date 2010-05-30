
/**
 * @brief Decrement sleeping time of the process in plswaiting.
 *
 * If a process as to be waking up, he is moved to the plsready list.
 */

#ifndef __SLEEP_H
#define __SLEEP_H

/**
 * @brief Decrease the sleep variable in the pcb. Wake up the process
 */
void            process_sleep();

#endif
