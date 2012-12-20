/*--------------------------------------------------------------------------!
! scsi33c93.h: defs for low level SCSI code for the wd33c93 SCSI Interface	!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	01 88/10/24	history starts, added ptr to data module in				wwb	!
!				low-level statics (for STATS).								!
!	02 88/10/30	reorganized.											wwb	!
!	03 88/11/09	made mask_level an int (stack alignment issue).			wwb	!
!	04 88/11/10	added <scsidma.h>.										wwb	!
!	05 88/12/08	converted <scsidma.h> to "dmapcc.h".					wwb	!
!	06 88/12/20	added VME620 includes.									wwb	!
!	07 89/02/17 Hack and clone, hack and clone...						jal	!
!   08 90/06/22 Removed DMA conditionals                                Rwb !
!				<<<---- OS-9/68k V2.4 Release ---->>>>						!
!   09 91/03/20 Added support for the WD33C93A chip                     Rwb !
!				<<<---- OS-9/68k V2.4.3 Release ---->>>>					!
!				<<<---- OS-9/68k V3.0 Release ---->>>>					    !
!				<<<---- OS-9/68k V3.1 Release ---->>>>					    !
!---------------------------------------------------------------------------!
!                                                                           !
!     Copyright 1988, 1989, 1990, 1991 by Microware Systems Corporation     !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#ifdef CBOOT
#include	<sysboot.h>
#else
#include	<rbf.h>
#include	<Machine/reg.h>
#include	<procid.h>
#include	<path.h>
#include	<errno.h>
#include	<cmdblk.h>
#endif

#include	<scsidmod.h>
#include	<wd33c93.h>
#include	<gendefs.h>

#ifdef VME147
#include	"dmapcc.h"
#endif
#ifdef VME620
#include	"vme620.h"
#endif

extern void wd_command();
extern int	chkstat();

#ifdef CBOOT
/*----------------------------------------------------------!
! global static storage used by the low level boot driver	!
!----------------------------------------------------------*/
extern u_char			compbyte,	/* scsi completion status byte		*/
						cmdstatus;	/* wd command completion status		*/
extern struct cmdblk	cmdblk;		/* command block					*/
extern u_char			*scsi_icr;
extern Hardware			scsibase;

#else
/*------------------------------------------!
! static storage used by low level driver	!
!------------------------------------------*/
typedef struct lstat {
	u_int		verfbuff;			/* write verify buffer pointer */
	u_int		datalen;			/* data transfer size */
	int			st_irqevt;			/* event for use by interrupt */
	Datmod		dmptr;				/* ptr to data module (for STATS) */
	Dmahardware dmabase;			/* dma hardware pointer */
	int			mask_level;			/* sr mask image for disabling irqs */
	int			spare[2];			/* reserved */
	u_char		compbyte;			/* scsi completion status byte */
	u_char		auxstatus;			/* wd33c93 aux register status */
	u_char		cmdstatus;			/* wd33c93 command completion status */
	u_char		dmastatus;			/* dma command completion status */
	u_char		spare2[12];			/* reserved */
} *Lstat;

/*--------------------------------------------------------------------------!
! high-level driver static usable by the low-level driver					!
+---------------------------------------------------------------------------+
! this is limited to the rbf defined portion and relies on the driver		!
! providing the pointer to its static storage in register a6.  The setup of	!
! a6 is accomplished by the "glue" code of the high level driver.			!
!--------------------------------------------------------------------------*/
extern struct rbfstatic rbfstat;
#endif	/* CBOOT */

/*------------------------------!
! pointer to wd33c93 registers	!
!------------------------------*/
#define HW(wd_register) (hw->wd_register) /* macro for WD 33c93 registers	*/

#define SELFID		7			/* id of the wd33c93 on scsi bus	*/
#define MAXTIME		100			/* bus timeout value (800mS)		*/
#define DELAYVAL	20			/* used by irq.c					*/

/*--------------------------!
! scsi bus transfer results	!
!--------------------------*/
#define BUS_DONE	0			/* transfer completed			*/
#define BUS_RERUN	1			/* rerun command				*/
#define BUS_FATAL	2			/* fatal abort					*/

/*----------------------------------!
! scsi completion byte definitions	!
!----------------------------------*/
#define COMPL_MASK	0x1f		/* scsi completion status mask	*/


