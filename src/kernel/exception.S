
#include <mips/regdef.h>
#include <cp0_regdef.h>
#include "asm_regs.h"

#----------------------------------------------------------------------------
# Exception handler code
# - if there is currently a process running we save his context. Otherwise
#   it's mean that the kernel was running. We save the stack and pc only
# - go to the interrupt handler C code
# - come back and loads the good context
#----------------------------------------------------------------------------

	.data

pc_kernel:		.word	0
stack_kernel:	.word	0

#----------------------------------------------------------------------------
# We Save the context
#----------------------------------------------------------------------------

	.text
	.globl exception_handler

exception_handler:        
	lw k0, current_pcb       # is there a process to save ?
	beq k0, $0, ksave      # no just save the kernel
                           
	.set noat              # yes save it !
	sw k1, REG_AT(k0)
	.set at
	sw v0, REG_V0(k0)
	sw v1, REG_V1(k0)
	sw a0, REG_A0(k0)
	sw a1, REG_A1(k0)
	sw a2, REG_A2(k0)
	sw a3, REG_A3(k0)
	sw t0, REG_T0(k0)
	sw t1, REG_T1(k0)
	sw t2, REG_T2(k0)
	sw t3, REG_T3(k0)
	sw t4, REG_T4(k0)
	sw t5, REG_T5(k0)
	sw t6, REG_T6(k0)
	sw t7, REG_T7(k0)
	sw t8, REG_T8(k0)
	sw t9, REG_T9(k0)
	sw s0, REG_S0(k0)
	sw s1, REG_S1(k0)
	sw s2, REG_S2(k0)
	sw s3, REG_S3(k0)
	sw s4, REG_S4(k0)
	sw s5, REG_S5(k0)
	sw s6, REG_S6(k0)
	sw s7, REG_S7(k0)
	sw sp, REG_SP(k0)
	sw fp, REG_FP(k0)
	sw ra, REG_RA(k0) 
	mfc0 t0, epc
	sw t0, REG_EPC(k0)
	sw gp, REG_GP(k0)
	j handler              # jump to the handler code

ksave:                    # Save the stack and kernel pc
	mfc0 t0, epc
	sw t0, pc_kernel($0)
	sw sp, stack_kernel($0)

handler:                  
	jal kexception         # jump to our C exeption handler

#----------------------------------------------------------------------------
# Come back time to restore the context
#----------------------------------------------------------------------------

	lw k0, current_pcb    # is there a pcb to load ?
	beq k0,$0, kload      # nop, just reload the kernel

	lw t0, REG_EPC(k0)        #sure there is !
	mtc0 t0, epc
	.set noat
	lw $at, REG_AT(k0)
	.set at
	lw v0,REG_V0(k0)
	lw v1,REG_V1(k0)
	lw a0,REG_A0(k0)
	lw a1,REG_A1(k0)
	lw a2,REG_A2(k0)
	lw a3,REG_A3(k0)
	lw t0,REG_T0(k0)
	lw t1,REG_T1(k0)
	lw t2,REG_T2(k0)
	lw t3,REG_T3(k0)
	lw t4,REG_T4(k0)
	lw t5,REG_T5(k0)
	lw t6,REG_T6(k0)
	lw t7,REG_T7(k0)
	lw t8,REG_T8(k0)
	lw t9,REG_T9(k0)
	lw s0,REG_S0(k0)
	lw s1,REG_S1(k0)
	lw s2,REG_S2(k0)
	lw s3,REG_S3(k0)
	lw s4,REG_S4(k0)
	lw s5,REG_S5(k0)
	lw s6,REG_S6(k0)
	lw s7,REG_S7(k0)
	lw sp,REG_SP(k0)
	lw fp,REG_FP(k0)
	lw ra,REG_RA(k0)
	lw gp,REG_GP(k0)
	j end                 # ok go back to your job now !

kload:                   # no process ? Just reload the kernel !
	lw sp, stack_kernel($0)
	lw t0, pc_kernel($0)
	mtc0 t0, epc

end:	# return from interrupt
	eret
