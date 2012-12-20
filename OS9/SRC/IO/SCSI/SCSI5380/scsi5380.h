/*
 * definitions for use by ncr5380 low level scsi subroutine module
 *
 * Copyright 1988, 1989, 1990, 1991, 1992, 1994, 1995 by 
 * Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * history:
 *
 * ed.   date    reason for change									who
 * --  --------  -------------------------------------------------- ---
 * 01  88/12/06  In the beginning...								jal
 * 04  89/10/31  Changed registers used for interrupt jive for new	jal
 *				 players
 * 05  90/06/19  Made some includes conditional on CBOOT            Rwb
 *               ---- OS-9/68k V2.4 Release ----
 * 06  91/05/28  Added definition of SCSI mask for port address     Rwb
 * 07  91/05/28  The Optimage people changed the system defaults
 *                 without letting us know, hence, JNMS defs were
 *                 incorrect.                                       Rwb
 * 08  91/05/30  For the JNMS player only:  An alarm is used to
 *               insure that DMA errors are caught.  The JNMS
 *               defs have been changed to make this possible       Rwb
 * 09  91/06/14  Changed DMA_MAX_TIME to 63.5 sec for JNMS player   Rwb
 *               due to EXABYTE operational characteristics.
 *               ---- OS-9/68k V2.4.3 Release ----
 * 10  92/08/07  fixed problems from new defs.						wwb
 *               ---- OS-9/68k V3.0 Release ----
 * 11  94/08/02  Added MAXSCSI definition.							wwb
 *               ---- OS-9/68k V3.0.1 Release
 * 12  95/06/08  Corrected pathname case of reg.h.                  ats
 *               ---- OS-9/68K V3.0.2 Release ----
 *               ---- OS-9/68K V3.1 Release ----
 */

#include	<types.h>

#ifdef OSK
typedef u_char			*Hardware;
#endif

#include	<rbf.h>
#include	<errno.h>
#include	<MACHINE/reg.h>
#include	<procid.h>
#include	<path.h>
#include	<ncr5380.h>
#if defined(JNMS) || defined(PCB1782)
#include	<dma68450.h>
#endif
#include	<scsidmod.h>
#include	<selfdata.h>
#include	<cmdblk.h>

/* 
 * declarations of routines 
 */
int		init(),
		exec(),
		term(),
		irqsvc();
		dmasvc();

/*
 * static storage used by low level driver
 */

typedef struct lstat {
	u_char		*v_buffer;			/* data transfer buffer					*/
	u_int		v_datalen;			/* data transfer size					*/
	u_int		v_cmdlen;			/* SCSI command block size				*/
	Datmod		v_dmptr;			/* ptr to data module (for STATS)		*/
 	int			v_mask_level;		/* status reg mask for disabling irqs 	*/
	int			v_irqevt;			/* event for use by interrupt			*/
#ifdef USEDMA
#if defined(PCB1782)
	int			v_dmaevt;			/* event for use by dma interrupt		*/
#endif
	Dmahardware v_dmabase;			/* dma hardware pointer					*/
	u_char		v_cer,				/* DMA Channel Error register image		*/
				v_csr,				/* DMA Channel Status register image	*/
				v_dspare[2];		/* keep structure to known alignment	*/
#endif
#ifdef USEDMA
#ifdef JNMS
	int         v_dmaactive,		/* has start time if dma active else, 0 */
	            v_monda,			/* alarm: is value of v_dmaactive       */
	            v_monds,			/* alarm: last start                    */
	            v_monct,			/* alarm: # of times same dma active    */
	            v_monAID,			/* alarm:  Alarm ID                     */
	            v_monDTO,			/* alarm:  timeout on this DMA          */
	            v_SCSIReset,		/* Flag:  if TRUE, SCSI Reset seen      */
	            v_CmdActive,		/* command being processed              */
				v_NotMeCnt,			/* count of Not-Me entries into IRQ     */
	            v_SCSIResCnt,		/* count of SCSI Bus Resets seen        */
				v_DMATOs;			/* count of the DMA time outs           */
#endif USEDMA
#endif JNMS defined
	u_char		v_initcmd,			/* image of 5380 initiator command reg	*/
				v_statbyte,			/* status byte returned from controller	*/
				v_msgin,			/* message byte returned from controller*/
				v_dir,				/* I/O direction flag					*/
				v_phasechg,			/* phase change occurred flag			*/
				v_selfid,			/* this system's SCSI self ID			*/
				v_spare2[8];		/* reserved								*/
} *Lstat;

/*
 * high-level driver static usable by the low-level driver
 *
 * this is limited to the rbf defined portion and relies on the driver
 * providing the pointer to its static storage in register a6.
 * The setup of a6 is accomplished by the "glue" code of the high
 * level driver.
 */

extern struct rbfstatic rbfstat;

/* pointer to NCR5380 registers	*/
#define		HW(ncr_register) (hw[ncr_register]) /* macro for NCR5380 regs */

/*
 * general definitions
 */
#define		TRUE				1			/* boolean TRUE			*/
#define		FALSE				0			/* boolean FALSE		*/
#define		NULL				0			/* pointer to nowhere	*/
#define		SUCCESS				0
#define LLMODACCESS (MP_OWNER_READ|MP_OWNER_WRITE|MP_GROUP_READ|MP_WORLD_READ)

#ifdef		OSK
#define		E_NOTME				1			/* Psuedo-error code for OS-9	*/
#endif

#define     SCSIIDMASK          ~0x7        /* mask low three bits of address */

#define		SUPSTAT				0x2000		/* supervisor state (68xxx SR)	*/

#define		RESDELAY			200000 		/* delay for reset				*/
#define     SELECT_DELAY		2000		/* controller select delay */

#define		SELFID				7			/* our own I.D. on the SCSI Bus */
#define		MAXSCSI				7			/* max scsi id supported */

/* a useful macro */
#define freedmod(err)	_fev_signl(dmod->dm_devent, 0); return err

#ifdef JNMS
/* these are for the DMA monitor alarm */
#define HALF_SECOND  128	/* # of 256ths of a second for cyclic alarm. */
#define DMA_MAX_TIME 127	/* # of times the same DMA may be seen as active */

/* External interrupt registers (16 bits wide) */
#define EXIR4 0x1FFC50
#define EXIR3 0x1FFC52
#define EXIR2 0x1FFC54
#define EXIR1 0x1FFC56

#define HDDINTSET	EXIR3		/* HDD uses external interrupt register 4	*/

#define DMA_DEVICE	0x00400000	/* Base of HD68450 dma controller	*/
#define BW			2	/* buswidth is 2				*/
#define BO			0	/* addressed on the even bytes	*/
#endif

#ifdef VME117
#define BW			2	/* buswidth is 2				*/
#define BO			1	/* addressed on the odd bytes	*/
#endif

#ifdef VME107
#define BW			2	/* buswidth is 2				*/
#define BO			1	/* addressed on the odd bytes	*/
#endif

#ifdef PCB1782
/* External DMA controller definitions */
#define DMA_VECT	71	/* vector for dma device */
#define DMAWINLVL	5	/* interrupt level 5 */
#define DMACHNL		1	/* dma channel # 1 */
#define DMA_DEVICE	0x00fe0000	/* Base of HD68450 dma controller	*/

#define BW			2	/* buswidth is 2				*/
#define BO			1	/* addressed on the odd bytes	*/
#endif

/*
 *  These define the return values for the io_xxxxx.c modules
 */
extern int transact();
extern u_char hs_input();
extern error_code sendcmd();
extern int data_io();
extern int set_dma();
extern int reset_dma();

/* end of file: scsi5380.h */

