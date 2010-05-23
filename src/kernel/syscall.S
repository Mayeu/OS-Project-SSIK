
#include <mips/regdef.h>
#include <cp0_regdef.h>
#include "asm_regs.h"

	.text
	.globl syscall_none
	.globl syscall_one
	.globl syscall_two
	.globl syscall_three

syscall_none:
  move v0, a0
  syscall
	jr ra			# Back in user mode, return to caller

syscall_one:
  move v0, a1
  syscall
	jr ra			# Back in user mode, return to caller

syscall_two:
  move v0, a2
  syscall
	jr ra			# Back in user mode, return to caller

syscall_three:
  move v0, a3
  syscall
	jr ra			# Back in user mode, return to caller
