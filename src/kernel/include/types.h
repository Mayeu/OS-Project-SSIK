/**
 * @file types.h
 * @brief Defines types use in the kernel
 */

#ifndef TYPES_H
#define TYPES_H
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;


#ifndef __BOOL_TYPES_
#define __BOOL_TYPES_

typedef enum
{
  FALSE,
  TRUE
} bool;

#endif /* __BOOL_TYPES_  */


#endif /*  */

/* end of file types.h */
