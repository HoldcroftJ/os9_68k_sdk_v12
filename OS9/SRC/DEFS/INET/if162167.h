/****************************************************************************
 *  $Workfile:   if162167.h  $
 *  $Revision:   1.4  $
 *  $Modtime:   10 Oct 1993 18:44:00  $
 *  $Date:   10 Oct 1993 18:45:30  $
 ****************************************************************************
 *	array size specification constants, used only in booter					*
 ****************************************************************************
 *	Copyright 1993 by Microware Systems Corporation							*
 *	Reproduced Under License												*
 *																			*
 *	This source code is the proprietary confidential property of			*
 *	Microware Systems Corporation, and is provided to licensee				*
 *	solely for documentation and educational purposes. Reproduction,		*
 *	publication, or distribution in any form to any party other than		*
 *	the licensee is strictly prohibited.									*
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date		Comments                                       	   By	*
 *  --- --------	-----------------------------------------------	------- *
 *	  0 ??/??/??	Birth												???	*
 *	  1	08/10/93	merged booter and driver ie162.h.  Added MAX_CBL		*
 *					and MAX_RFD which are only used by the booter.  		*
 *					I82596_CHANATTN for the booter did not used the 		*
 *					nop(), but an extra nop() should not cause a 			*
 *					problem in the booter.  Rest was identical except 		*
 *					that the booter does not use LANC_ICLR or LANC_EoL.		*
 *					File moved to mwos/os9/src/defs/inet.				gkm	*
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 ****************************************************************************/

#define MAX_CBL     1
#define MAX_RFD     4

/* chip access macros */

#define I82596_RESET(xx)			doport_command(xx, 0, 0)
#define I82596_SELFTEST(xx, addr)	doport_command(xx, addr, 1)
#define I82596_ASCP(xx, addr)		doport_command(xx, addr, 2)
#define I82596_DUMP(xx, addr)		doport_command(xx, addr, 3)

#define I82596_CHANATTN(xx)\
		nop();\
		*((u_int32 *)xx->v_ca) = 0;

/* i82596 DMA adjustment macro */

#define I82596_DMA_ADJ(ifstat, value) ((void *)(value))

/* board address offsets */

#define LANC_ERR_STATUS				((u_int8  *)0xfff42028)
#define LANC_IRQ_CONTROL			((u_int8  *)0xfff4202a)
#define LANC_BEIRQ_CONTROL			((u_int8  *)0xfff4202b)

#define LANC_ICLR		0x08	/* clear LANC interrupt */
#define LANC_IEN		0x10	/* enable IRQ */
#define LANC_INT		0x20	/* IRQ asserted */
#define LANC_EoL		0x40	/* edge=1 level=0 sensitive irq */

#define LANC_SNOOP		(1<<6)	/* turn on local bus snooping */

#define BBRAM_ETHERADDR				((u_int8  *)0xfffc1f2c)

/* bit definitions */

extern u_int32 ram_getlong();

/* debugging */

/* #define BUSMARK(x) (*((u_int8 *)0xffff8080) = (x))		/* vmebus write */

/* VME162 hardware options */

#define	MCOPTIONS	0xfff4202e
#define	NOLANC		0x08

#define	nolanc()	((int) (*((u_char *)MCOPTIONS) & NOLANC))

