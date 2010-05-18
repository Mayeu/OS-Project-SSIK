
#ifndef REGISTERS_H
#define REGISTERS_H

#include "types.h"

typedef struct
{
  uint32_t        at_reg;	/* offset   0 */
  uint32_t        v_reg[2];	/* offset   4 */
  uint32_t        a_reg[4];	/* offset  12 */
  uint32_t        t_reg[10];	/* offset  28 */
  uint32_t        s_reg[8];	/* offset  68 */
  uint32_t        sp_reg;	/* offset 100 */
  uint32_t        fp_reg;	/* offset 104 */
  uint32_t        ra_reg;	/* offset 108 */
  uint32_t        epc_reg;	/* offset 112 */
  uint32_t        gp_reg;	/* offset 116 */
} registers_t;			/* size   120 */

#endif
