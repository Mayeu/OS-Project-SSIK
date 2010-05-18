/*
 * @file mips4kc.h
 * @brief Structure to manipulate the register of the mips processor
 */

#ifndef MIPS4KC_H
#define MIPS4KC_H

#include "types.h"

/* status register */
typedef union
{
  uint32_t        reg;
  struct
  {
    uint32_t        ie:1;       /* Bit 0:     Interrupt Enable */
    uint32_t        exl:1;      /* Bit 1:     Exception Level */
    uint32_t        erl:1;      /* Bit 2:     Error Level */
    uint32_t        pad0:1;     /* Bit 3:     Reserved */
    uint32_t        um:1;       /* Bit 4:     User Mode */
    uint32_t        pad1:3;     /* Bit 5-7:   Reserved */
    uint32_t        im:8;       /* Bit 8-15:  Interrupt Mask */
    uint32_t        pad2:3;     /* Bit 16-18: Reserved */
    uint32_t        nmi:1;      /* Bit 19:    Indicates a Non-Maskable Interrupt */
    uint32_t        sr:1;       /* Bit 20:    Soft Reset */
    uint32_t        ts:1;       /* Bit 21:    TLB Shutdown */
    uint32_t        bev:1;      /* Bit 22:    Bootstrap Exception Vector */
    uint32_t        pad3:2;     /* Bit 23-24: Reserved */
    uint32_t        re:1;       /* Bit 25:    Reverse Endian */
    uint32_t        pad4:1;     /* Bit 26:    Reserved */
    uint32_t        rp:1;       /* Bit 27:    Reduced Power mode */
    uint32_t        cu0:1;      /* Bit 28:    Coprocessor 0 Usable */
    uint32_t        cu1:1;      /* Bit 29:    Coprocessor 1 Usable */
    uint32_t        cu2:1;      /* Bit 30:    Coprocessor 2 Usable */
    uint32_t        cu3:1;      /* Bit 31:    Coprocessor 3 Usable */
  } field;
} status_reg_t;

/* cause register */
typedef union
{
  uint32_t        reg;
  struct
  {
    uint32_t        pad0:2;     /* Bit 0-1:   Reserved */
    uint32_t        exc:5;      /* Bit 2-6:   Exception code */
    uint32_t        pad1:1;     /* Bit 7:     Reserved */
    uint32_t        ip:8;       /* Bit 8-15:  Interrupt pending */
    uint32_t        pad2:6;     /* Bit 16-21: Reserved */
    uint32_t        wp:1;       /* Bit 22:    Watch Prevent */
    uint32_t        iv:1;       /* Bit 23:    Interrupt Vector */
    uint32_t        pad3:4;     /* Bit 24-27: Reserved */
    uint32_t        ce:2;       /* Bit 28-29: Coprocessor exception */
    uint32_t        pad4:1;     /* Bit 30:    Reserved */
    uint32_t        bd:1;       /* Bit 31:    Branch Delay */
  } field;
} cause_reg_t;

/* Exception codes for MIPS4KC */
#define EXC4_Int    0           /* Interrupt */
#define EXC4_Mod    1           /* TLB modification exception */
#define EXC4_TLBL   2           /* TLB load/fetch exception */
#define EXC4_TLBS   3           /* TLB store exception */
#define EXC4_AdEL   4           /* Address error (load/fetch) */
#define EXC4_AdES   5           /* Address error (store) */
#define EXC4_IBE    6           /* Bus error (fetch) */
#define EXC4_DBE    7           /* Bus error (data load/store) */
#define EXC4_Sys    8           /* syscall */
#define EXC4_Bp     9           /* Breakpoint exception */
#define EXC4_RI     10          /* Reserved instruction exception */
#define EXC4_CpU    11          /* Coprocessor unusable exception */
#define EXC4_Ov     12          /* Integer overflow exception */
#define EXC4_Tr     13          /* Trap exception */
#define EXC4_WATCH  23          /* Reference to WatchHi/WatchLo address */
#define EXC4_MCheck 24          /* Machine check */

/* TLB registers are only defined if you ask for them: */
#ifdef MIPS4KC_TLB

/* TLB index register */
typedef union
{
  uint32_t        reg;
  struct
  {
    uint32_t        index:4;    /* Bit 0-3:   TLB index */
    uint32_t        pad:27;     /* Bit 4-30:  Reserved */
    uint32_t        p:1;        /* Bit 31:    Probe failure */
  } field;
} index_reg_t;

/* TLB random register */
typedef union
{
  uint32_t        reg;
  struct
  {
    uint32_t        random:4;   /* Bit 0-3:   TLB random index */
    uint32_t        pad:28;     /* Bit 4-31:  Reserved */
  } field;
} random_reg_t;

/* TLB EntryLo0/EntryLo1 registers */
typedef union
{
  uint32_t        reg;
  struct
  {
    uint32_t        g:1;        /* Bit 0:     Global (ignore ASID if BOTH entries are global...) */
    uint32_t        v:1;        /* Bit 1:     Valid bit */
    uint32_t        d:1;        /* Bit 2:     Dirty bit (write enabled) */
    uint32_t        c:3;        /* Bit 3-5:   Coherency attribute */
    uint32_t        pfn:20;     /* Bit 6-25:  Page Frame Number (bits 12-31 of physical address) */
    uint32_t        pad0:4;     /* Bit 26-29: Unused PFN bits */
    uint32_t        pad1:2;     /* Bit 30-31: Reserved */
  } field;
} entrylo_reg_t;

/* TLB EntryHi register */
typedef union
{
  uint32_t        reg;
  struct
  {
    uint32_t        asid:8;     /* Bit 0-7:   Current ASID */
    uint32_t        pad:5;      /* Bit 8-12:  Reserved */
    uint32_t        vpn2:19;    /* Bit 13-31: (Virtual Page Number/2) */
  } field;
} entryhi_reg_t;

/* TLB context register */
typedef union
{
  uint32_t        reg;
  struct
  {
    uint32_t        pad:4;      /* Bit 0-3:   Reserved */
    uint32_t        bad_vpn2:19;        /* Bit 4-22:  On TLB miss: 'bad' (missing) VPN2 */
    uint32_t        pte_base:8; /* Bit 23-31: Page Table Entry base address */
  } field;
} context_reg_t;

/* TLB page mask register */
typedef union
{
  uint32_t        reg;
  struct
  {
    uint32_t        pad0:13;    /* Bit 0-12:  Reserved */
    uint32_t        mask:12;    /* Bit 13-24: Page mask */
    uint32_t        pad1:7;     /* Bit 25-31: Reserved */
  } field;
} pagemask_reg_t;

/* TLB wired register */
typedef union
{
  uint32_t        reg;
  struct
  {
    uint32_t        wired:4;    /* Bit 0-3:   TLB wired boundary */
    uint32_t        pad:28;     /* Bit 4-31:  Reserved */
  } field;
} wired_reg_t;

#endif /*  */

#endif /*  */
