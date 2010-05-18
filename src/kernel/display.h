
#ifndef DISPLAY_H
#define DISPLAY_H
  
#include "types.h"
  
/* Display driver for 8 digit LED on Malta board       */ 
/* (See pages 14-16 of the Malta board's User's Guide) */ 
typedef struct 
{
  uint8_t pad1[8];
  
    /* LEDBAR: 8 bits, 1 = LED on */ 
    union
  {
    uint8_t reg;
    struct
    {
      uint8_t led0:1;
      uint8_t led1:1;
      uint8_t led2:1;
      uint8_t led3:1;
      uint8_t led4:1;
      uint8_t led5:1;
      uint8_t led6:1;
      uint8_t led7:1;
    } field;
                 } ledbar;
                 uint8_t pad2[7];
                
    /* ASCIIWORD: Value written here will be displayed in hex */ 
                  uint32_t asciiword;
                 uint8_t pad3[4];
                
    /* ASCIIPOSn : ASCII value for LED position 'n'. */ 
    struct
  {
    uint8_t value;
    uint8_t pad[7];
  } asciipos[8];
               } display_t;

#endif /*  */
