/*
 * definitions for 68340 DMA
 */

/*
 * Copyright 1992 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 * history:
 *
 * ed.  date   reason.                                         who.
 * -- -------  ----------------------------------------------  ---
 * 00 92/06/16 history starts.                                 ???
 *				---- OS-9/68K V3.0 Release ----
 *				---- OS-9/68K V3.1 Release ----
 */

/*------------------------------!
! 68340 DMA device definitions	!
!------------------------------*/

/*----------------------------------------------------------------------!
! NOTE: All the register offsets (except general control register) must	!
! have the channel number appended.  The channel number times 64 plus	!
! the register offset given here will reach the correct register.		!
!----------------------------------------------------------------------*/

typedef struct dmachannel {
	u_short		dh_mcr;			/* Module Config Register (word)	*/
	unsigned	:16;			/* reserved							*/
	u_char		dh_intl;		/* Interrupt Level (byte)			*/
	u_char		dh_intv;		/* Interrupt Vector (byte)			*/
	unsigned	:16;			/* reserved							*/
	u_short		dh_ccr;			/* Channel Control Register (word)	*/
	u_char		dh_csr;			/* Channel Status Register (byte)	*/
	u_char		dh_fcr;			/* Function Code Register (byte)	*/
	u_char *	dh_sar;			/* Source Address Register (long)	*/
	u_char *	dh_dar;			/* Dest. Address Register (long)	*/
	u_long		dh_btc;			/* Byte Transfer Count (long)		*/
	unsigned	:16;			/* reserved							*/
	unsigned	:16;			/* reserved							*/
	unsigned	:16;			/* reserved							*/
	unsigned	:16;			/* reserved							*/
} dmahardware, *Dmahardware;

#define CHNLSIZE	0x20		/* size of one channel register set	*/

/*------------------------------------------+
|	Module configuration register (MCR)		|
+------------------------------------------*/

#define MCR_STP		(1<<15)		/* Stop bit */
#define MCR_FRZ1	(1<<14)		/* Freeze bit 1 */
#define MCR_FRZ0	(1<<13)		/* Freeze bit 0 */
#define MCR_SE		(1<<12)		/* Single address bus enable */
#define b_MCR_ISM	8			/* Interrupt service mask (3 bits) */
#define MCR_SUPV	(1<<7)		/* Supervisor access */
#define b_MCR_MAID	4			/* Master arbitration ID (3 bits) */
#define b_MCR_IARB	0			/* Interrupt arbitration ID (4 bits) */

/*----------------------------------+
|	Channel Control Register (CCR)	|
+----------------------------------*/

#define CCR_INTB	(1<<15)		/* Interrupt breakpoint */
#define CCR_INTN	(1<<14)		/* Interrupt normal */
#define CCR_INTE	(1<<13)		/* Interrupt error */
#define CCR_ECO		(1<<12)		/* External control option */
#define CCR_SRCREQ	(1<<12)		/* Source generates req (dual addr mode) */
#define CCR_DSTREQ	(0<<12)		/* Dest generates req (dual addr mode) */
#define CCR_MEM2DEV	(1<<12)		/* Memory to device (single addr mode) */
#define CCR_DEV2MEM	(0<<12)		/* Device to memory (single addr mode) */
#define CCR_SAPI	(1<<11)		/* Source address pointer increment */
#define CCR_DAPI	(1<<10)		/* Destination address pointer increment */
#define CCR_SSIZE	(3<<8)		/* Source size control mask */
#define b_CCR_SSIZE	8			/* Source size control bit */
#define CCR_DSIZE	(3<<6)		/* Destination size control mask */
#define b_CCR_DSIZE	6			/* Destination size control bit */
#define CCR_REQINT	(0<<4)		/* Internal DMA request */
#define CCR_REQEXTB	(2<<4)		/* External DMA burst request */
#define CCR_REQEXTC	(3<<4)		/* External DMA cycle steal request */
#define CCR_BB25	(0<<2)		/* 25% bus bandwidth */
#define CCR_BB50	(1<<2)		/* 50% bus bandwidth */
#define CCR_BB75	(2<<2)		/* 75% bus bandwidth */
#define CCR_BB100	(3<<2)		/* 100% bus bandwidth */
#define CCR_SINGLE	(1<<1)		/* Single address transfers */
#define CCR_DUAL	(0<<1)		/* Dual address transfers */
#define CCR_STR		(1<<0)		/* Start DMA transfer */
#define CCR_STOP	(0<<0)		/* Stop DMA transfer */

#define CCR_SBYTE	((1<<b_CCR_SSIZE)&CCR_SSIZE)
#define CCR_SWORD	((2<<b_CCR_SSIZE)&CCR_SSIZE)
#define CCR_SLONG	((4<<b_CCR_SSIZE)&CCR_SSIZE)

#define CCR_DBYTE	((1<<b_CCR_DSIZE)&CCR_DSIZE)
#define CCR_DWORD	((2<<b_CCR_DSIZE)&CCR_DSIZE)
#define CCR_DLONG	((4<<b_CCR_DSIZE)&CCR_DSIZE)

/*----------------------------------+
|	Channel Status Register (CSR)	|
+----------------------------------*/
#define	CSR_IRQ		(1<<7)		/* Interrupt request */
#define CSR_DONE	(1<<6)		/* DMA done */
#define CSR_BES		(1<<5)		/* Bus error on source */
#define CSR_BED		(1<<4)		/* Bus error on destination */
#define CSR_CONF	(1<<3)		/* Configuration error */
#define CSR_BRKP	(1<<2)		/* DMA breakpoint */

#define CSR_ERRS	(CSR_BES + CSR_BED + CSR_CONF + CSR_BRKP)

/*------------------------------+
| Function Code Register (FCR)	|
+------------------------------*/
#define USERDATA	1		/* user data address access				*/
#define USERPROG	2		/* user program address access			*/
#define SUPVDATA	5		/* supervisor data address access		*/
#define SUPVPROG	6		/* supervisor program address access	*/

#define b_FCR_SRC	4		/* Source bit number */
#define b_FCR_DST	0		/* Destination bit number */

/* End of File */
