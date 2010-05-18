/**
 * @file ds12887.h
 * @brief Structure for accessing the timer
 */

#ifndef DS12887_H
#define DS12887_H

#include "types.h"
typedef struct
{
  struct
  {
    uint8_t         seconds;
    uint8_t         seconds_alarm;
    uint8_t         minutes;
    uint8_t         minutes_alarm;
    uint8_t         hours;
    uint8_t         hours_alarm;
    uint8_t         day_of_week;
    uint8_t         day_of_month;
    uint8_t         month;
    uint8_t         year;
  } data;
  union
  {
    uint8_t         reg;
    struct
    {
      uint8_t         rs:4;
      uint8_t         dv:3;
      uint8_t         uip:1;
    } field;
  } regA;
  union
  {
    uint8_t         reg;
    struct
    {
      uint8_t         dse:1;
      uint8_t         fmt:1;    /* 24 or 12 hr format */
      uint8_t         dm:1;
      uint8_t         sqwe:1;
      uint8_t         uie:1;
      uint8_t         aie:1;
      uint8_t         pie:1;
      uint8_t         set:1;
    } field;
  } regB;
  union
  {
    uint8_t         reg;
    struct
    {
      uint8_t         pad:4;
      uint8_t         uf:1;
      uint8_t         af:1;
      uint8_t         pf:1;
      uint8_t         irqf:1;
    } field;
  } regC;
  uint8_t         regD;
  uint8_t         non_volatile[114];
} ds12887_t;

#endif /*  */

/* end of file ds12887.h */
