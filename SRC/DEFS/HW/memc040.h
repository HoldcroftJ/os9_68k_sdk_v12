/*
 * $Header:   /h0/MWOS/SRC/DEFS/HW/VCS/memc040.h_v   1.8   30 Jun 1997 01:15:12   packager  $
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1996 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
/* memc040.h - Definitions for MEMC040 DRAM Controller Chip 
 *          ( mVme167/mVme187/mVme166 eta'al )
 *
 * History:
 * Date       Who     Reason
 * ---------  ----  ------------------------------------------------
 * 21 Feb 94  Rwb   Created based on memc040.d from OS9 port (file ed.3)
 *                  ---- OS-9/68K V3.0.2 Released ----
 *                 ---- OS-9000/x86 V2.1 Released ----                  
 *                 ---- OS-9000/PPC V2.1.1 Released ----                
 *                 ---- OS-9000/ARMv3 V2.2 Released ----                
 *                 ---- OS-9000/ARMv3 V2.2.1 Released ----              
 *                 $$                 <RELEASE_INFO>                  $$
 */

#ifndef _MEMC040_H_

#define _MEMC040_H_

/*
 *  Conventions used in this file:
 *
 *   B_XXXX  denotes a OR maskable bit definition (note access size quoted)
 *   M_XXXX  denotes a bit mask (again, note access size quoted)
 */

/*
 *  The MEMC040 chip is a four-way interleaving DRAM controller ASIC
 *  for the MC68040-type bus.  It was designed in part for use with
 *  the mVme167/187 processors.
 */

/*
 *  The hardware looks like this....
 */
typedef struct memc040chip {

	u_int32  mcid,		/* chip ID register (read) sw interr (write $F) (00) */
	         mcrv,		/* chip revision register                       (04) */
	         mccfg,		/* Memory Configuration Register                (08) */
	         mcasr,		/* Alternate Status Register                    (0C) */
	         mcacr,		/* Alternate Control Register                   (10) */
	         mcbadr,	/* Base Address Register  ( Address bits 31-24 )(14) */
	         mcrcr,		/* RAM Control Register                         (18) */
	         mcbcr;		/* Bus Clock Register                           (1C) */

} memc_t, *Memc_p;


#define c_MEMC040 0x80		/* Value of MCID register for MEMC040 */

/*
 * MCCFG - Memory Configuration Register
 */
#define B_MCCFG_FSTRD 0x20	/* Reflects the state of the FASTREAD input pin */
#define B_MCCFG_EXTPE 0x10	/* Reflects the state of the EXTPEN pin */
#define B_MCCFG_WPB   0x08	/* Reflects the write-per-bit mode. */
#define M_MCCFG_MSIZ  0x07	/* Reflects the size strapping (low 3 bits) */

/*  NOTE WELL: size is based at 4MB and is doubled per increment with values
 *  6 and 7 reserved  (i.e 1 = 8MB, 2 = 16MB ..... 5 = 128Mb)
 */


/*
 * MCRCR - RAM Control Register
 */

#define M_MCRCR_AD22   0xC0	/* These two bits determine address (23-22) */
#define B_MCRCR_SWAIT  0x10	/* Snoop Wait: 1 = wait for MI*, 0 = don't wait */
#define B_MCRCR_WWP    0x08	/* Write Wrong Parity (1 = do it - Testing only!) */
#define B_MCRCR_PARINT 0x04	/* Parity Interrupt enable  (see table below) */
#define B_MCRCR_PAREN  0x02	/* Parity Enable (see table below) */
#define B_MCRCR_RAMEN  0x01	/* RAM enable (1 == enable, 0 == disable) */

/*
 * b_MCRCR_paren and b_MCRCR_parint work together to produce the
 *   following truth table:
 *
 *  paren     parint    CPU     Alternate
 *    0         0      None     None
 *    0         1      Int      None
 *    1         0      CHECK    CHECK
 *    1         1      Int      CHECK
 *
 * where:  None  == No checking done
 *         Int   == CPU recieves parity interrupt on parity error
 *         CHECK == TEA generated on parity error
 *
 *  If interrupt is to be generated, must enable the parity error interrupt
 *  in the VMEChip2.
 *
 *  If b_MCCFG_expe is 0, all parity checking is disabled reguardless of
 *    these bits.
 */

/*
 * MCBCR - Bus Clock Register
 *    Should be programmed to the operating clock frequency.
 *    (ie. $21 for 33MHz)
 */

#endif /* _MEMC040_h_ */
