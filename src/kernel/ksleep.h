
/**
 * @brief Decrement sleeping time of the process in pclswaiting.
 *
 * If a process as to be waking up, he is moved to the pclsready list.
 */

#ifndef __SLEEP_H
#define __SLEEP_H
void            process_sleep();
uint32_t        go_to_sleep(uint32_t time);
#endif
