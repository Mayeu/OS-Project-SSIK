/* Copyright (c) 1997 by Marcus Nilsson.  All Rights Reserved */

/***************************************************************************
 *   NAME
 *     cp0_regdef.h
 *   PURPOSE
 *     Symbolic names for cp0 registers
 *   NOTES
 *     
 *   HISTORY
 *     Marcus Nilsson (marcusn@csd.uu.se) - Apr 04, 1997: Created.
 ****************************************************************************/

#ifndef CP0_REGDEF_H
#define CP0_REGDEF_H

#define index           $0      /* MMU registers */
#define random          $1
#define entrylo		$2
#define	entrylo0	$2
#define	entrylo1	$3
#define	pagemask	$5
#define	entryhi		$10
#define sr              $12     /* interrupt registers */
#define cause           $13
#define epc             $14

#define count           $9      /* timer registers */
#define compare         $11


#endif /* CP0_REGDEF_H */
