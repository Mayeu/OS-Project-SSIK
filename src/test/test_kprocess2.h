#ifndef __TEST_KPROCESS_H
#define __TEST_KPROCESS_H
/**
 * @brief Return the next avaible pid, or an error code
 * @return the next avaible pid
 */
int32_t         get_next_pid();

/**
 * @brief Get the array showing which part of the
 * stack is in use. This functions is here for test purpose only !
 */
int32_t        *get_used_stack();

/**
 * @brief Allocate the stack for a pcbs. In our case keep in an array which
 * part of the stack is used
 * @param the pid of the process who need a stack
 */
uint32_t       *allocate_stack(uint32_t pid);

/**
 * @brief Deallocate the stack for a certain pid
 * @param the pid of the pcb
 */
int32_t         deallocate_stack(uint32_t pid);

#endif /* __TEST_KPROCESS_H */
