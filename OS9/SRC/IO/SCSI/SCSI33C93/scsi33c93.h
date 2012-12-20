/*
 * definitions for use by wd33c93 low level scsi subroutine module
 *
 * Copyright 1988, 1989, 1990, 1991, 1994, 1995 by Microware Systems 
 * Corporation.  Reproduced Under License.
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * history:
 *
 * ed.   date    reason                                           who.
 * --  --------  ---------------------------------------------    ---
 * 01  88/10/24  history starts, added ptr to data module in      wwb
 *               low-level statics (for STATS).
 * 02  88/10/30  reorganized.                                     wwb
 * 03  88/11/09  made mask_level an int (stack alignment issue).  wwb
 * 04  88/11/10  added <scsidma.h>.                               wwb
 * 05  88/12/08  converted <scsidma.h> to "dmapcc.h".             wwb
 * 06  88/12/20  added VME620 includes.                           wwb
 * 07  89/05/15  added declaration of _f_trans type.              wwb
 * 08  89/05/17  added SCSI_SUPPORT flags.                        wwb
 * 09  89/05/25  fixed problem with ed.8.                         wwb
 * 10  89/07/14  added PCC_DELAY for fast cpu's, changed DELAYVAL wwb
 *               to WD_DELAY (irq.c).
 * 11  89/11/21  Revised for major rewrite of low level driver    Rwb
 * 12  90/05/23  Revised definition of CBK_STATIN and removed
 *               message in buffer that is no longer used         Rwb
 *               ---- OS-9/68K V2.4 released ----
 * 13  91/01/07  Added SCSI reset counter to static storage       Rwb
 * 14  91/03/19  Added support for the WD33C983A part             Rwb
 *               ---- OS-9/68K V2.4.3 released ----
 *               ---- OS-9/68K V3.0 released ----
 * 15  94/08/02 Added MAXSCSI definition.                         wwb
 * 16  95/02/14 Made ANSI conformant.							  rws
 * 17  95/02/28 Further qualified the inclusion of dmapcc.h		  rws
 * 18  95/06/08 Corrected pathname case of reg.h.                 ats
 *              ---- OS-9/68K V3.0.2 Release ----
 *              ---- OS-9/68K V3.1 Release ----
 */

#include	<rbf.h>
#include	<MACHINE/reg.h>
#include	<procid.h>
#include	<path.h>
#include	<errno.h>
#include	<wd33c93.h>
#if defined (VME147) && defined (USEDMA)
#include	"dmapcc.h"
#endif
#ifdef VME620
#include	"vme620.h"
#endif
#include	<scsidmod.h>
#include	<cmdblk.h>

/*
 * addrress translator function, used in exec.c, located in syscalls.a
 */
extern u_char *_f_trans ();	

/*
 *  This is the structure that is used by the state machine to maintain
 *     the bus activity.  The routines within the low level driver will
 *     talk to these structures except at bus free time, in which case
 *     the execute entry point will perform a 'kick start' of the device.
 *     There will be one of these per SCSI ID.
 */

struct cntrlblk {
	u_int event_id,			/* event ID associated with this SCSI ID */

	      cmd_status,		/* final disposition of the command. */
	      cmd_cmpcode,		/* completion code for high level driver */
	      scsi_flags;		/* SCSI flags from the driver */

	u_char cbk_status,		/* status for this control block */
	       ll_cmd,          /* wd33c93 command to use. */
	       nxt_phase,       /* next expected phase ... may not be cmd done */
	       syc_tried;		/* attempted synchronous arbitration */

	/* This is the "save pointers area.  All info neccessary to
  	 * reactivate the command after a disconnect is stored here
	 */
	int    cntreg_valid;	  /* flag indicating cnt register is data xfer */
	char   *curr_addr;        /* saved address, points to next data */
	long   curr_cnt;          /* count down counter of bytes to be transfered */
	u_char curr_mode;         /* current mode value */
	u_char curr_res[3];       /* reserved area ( preserve alignment ) */

	struct cmdblk cmdblock;   /* command block info from caller */

	u_char spcmd_phase,         /* last phase */
	       sp_xcount[3],        /* number of bytes remaining to be transfered */
	       spsyc_register,		/* synchronous transfer register */
	       sp_xtrol,			/* control register byte */
#ifdef VME147
	       dmastatus,           /* dma command completion status */
		   sp_res[2];	        /* reserved ( for alignment ) */
#else		   
	       sp_res[3];			/* reserved, (maintain alignment) */
#endif 

	u_char mess_out[16];    /* buffer for special message to target */
};

typedef struct cntrlblk *CntrlBlk;		/* define pointer type */

/* These definitions are used within the structure defined above */

       /* cbk_status: */
#define CBK_FREE   0        /* block not busy */
#define CBK_DISC   0x1      /* Current state of command is disconnected */
#define CBK_SUBMIT 0x2      /* block has been submited to the BUS */
#define CBK_VALID  0x4      /* IRQ will not use block until this is set */
#define CBK_STATIN 0x8      /* Command status has arrived */
#define CBK_RESET  0x40		/* Flag device reset being performed */
#define CBK_BUSY   0x80     /* control block is in use */

       /* Event values - values that the events may have */
#define CBLOCKFREE 0x0      /* the command block is free */
#define CBLOCKBUSY 0x20     /* command block is busy - set by driver */
#define CBLOCKWAKE 0x40     /* command in block complete - set by IRQ */

/*
 * static storage used by low level driver
 */
typedef struct lstat {
	Datmod   dmptr;           /* ptr to data module (for STATS) */
	Hardware hdware;          /* ptr to 33c93 chip ( for kick() ) */
	int      mask_level,      /* sr mask image for disabling irqs */
	         usercount,       /* number of users of the low level driver */
	         busfree,         /* TRUE = device free, FALSE = device busy */
	         freshcmd,        /* on TRUE, a new command has been started */
	         auxstatus,		  /* irq's scratch hold for status */
	         apart;           /* we have a WD33C93A rather than the old part */
	CntrlBlk curr_cmd;        /* this is the currently active command */
#if defined (VME147) && defined (USEDMA)
	Dmahardware dmabase;      /* dma  hardware pointer */
#endif 
	int         scsiresets;   /* number of SCSI interrupts that have occured */
	int         lstres[2];    /* align first control block */
	struct cntrlblk cntb[8];  /* allows initiator at other than SCSI ID 7 */
#ifdef RECORD
#define STATMAX 32767
	int     statcount;			/*  number of stati in array */
	u_char  *nxtstat;			/* 	pointer to next status byte */
	u_char  stats[STATMAX];		/*  the status bytes */
#endif	
} *Lstat;

	/* bus states ( busstate above ) */
#define BUS_FREE 0      /* no SCSI activity in progress */
#define BUS_DISCON 1    /* command transaction suspended due to disconntect */
#define BUS_BUSY 2      /* command being transacted */

	/* message types */
#define SYNC_XFER_REQ 0x1	/* transacting synchronous messages */	

/*
 * high-level driver static usable by the low-level driver
 *
 * this is limited to the rbf defined portion and relies on the driver
 * providing the pointer to its static storage in register a6.
 * The setup of a6 is accomplished by the "glue" code of the high
 * level driver.
 *
 */

extern struct rbfstatic rbfstat;

/*
 * pointer to wd33c93 registers
 */
#define		HW(wd_register) (hw->wd_register) /* macro for WD 33c93 regs */

/*
 * general definitions
 */
#define		TRUE				1			/* boolean TRUE */
#define		FALSE				0			/* boolean FALSE */
#define		NULL				0			/* pointer to nowhere */
#define LLMODACCESS (MP_OWNER_READ|MP_OWNER_WRITE|MP_GROUP_READ|MP_WORLD_READ)

#define		SELFID				7			/* id of the wd33c93 on scsi bus */
#define		MAXTIME				100			/* bus timeout value (800mS) */
#define		WD_DELAY			20			/* wd cmd delay (irq.c) */
#define		PCC_DELAY			10			/* pcc access delay (irq.c) */

#define		SUCCESS				0

#define		SUPSTAT				0x2000		/* supervisor state (68xxx SR) */

#define     OMTI_KLUDGE         ~0x60       /* because OMTI uses bits 5 and
                                             *  6 to return the LUN that the
                                             *  status word is for we must
                                             *  mask each incomming status
                                             *  byte.
                                             */
#define		MAXSCSI				7			/* max scsi id supported */

/*  message byte definitions */
#define IDENTIFY_MESSAGE 0x80		/* identify */
#define BDRESET_MESSAGE  0x0C		/* bus device reset */
#define EXTENDED_MESSAGE 0x01		/* extended message flag */
#define REJECTED_MESSAGE 0x07		/* message byte reject */

#define MINPERIOD        0x20		/* define minimum SCSI period */

/*
 * scsi bus transfer results
 */
#define		BUS_DONE			0			/* transfer completed */
#define		BUS_RERUN			1			/* rerun command */
#define		BUS_FATAL			2			/* fatal abort */

/*
 * scsi completion byte definitions
 */
#define		COMPL_MASK			0x1f		/* scsi completion status mask */

/* The wd33c93 has bit3 of the request word always set in the interrupt
 *   status.  For each of the 4 groups, bit 3 set is more or less a 
 *   flag that the status/error occured durring the phase described by
 *   the M/C/I bits.  These defs define bit 3 as a one.
 */
#define MCI_DO  0x8		/* data out to target */
#define MCI_DI  0x9		/* data in from target */
#define MCI_CT  0xA		/* command to target */
#define MCI_ST  0xB		/* status from target */

    /* NOTE: 0xC and 0xD are reserved codes by the SCSI spec */
#define MCI_MO  0xE		/* message out */
#define MCI_MI  0xF     /* message in */

/* These are the various request interrupts that are serviced 
 *    within group4().
 */
#define RESELECTED 0x80             /* reselected by target */
#define RESELECTEDADF 0x81			/* as above, advanced features (A) */
#define DATA_OUT 0x88				/* data out to target */
#define DATA_IN  0x89				/* data in from target */
#define CMD_OUT  0x8A				/* command out to target */
#define STAT_IN  0x8B				/* status in from target */
#define MESSAGE_OUT 0x8E			/* message out to target */
#define MESSAGE_IN  0x8F			/* message in from target */

#define TERM_DISCON 0x85			/* command termination disconnect */

#define SUSPEND 0xa5				/* marked for target disconnect */

#define ADV_MODE CR_IDI				/* the 33c93 has a hidden bit in the
                                     * control word that modifies the
                                     * IRQ rules slightly.
                                     */

/*
 * scsi flags support
 *
 * this defines the pd_scsiopt flags that the low-level module
 * will support.
 */
#define		SCSI_SUPPORT		(SCSI_ATN | SCSI_PARITY | SCSI_SYNCHR)


/*
 * scsi port interrupt register on the MVME147:
 * bit 3 - enables interrupts from the wd33c93.
 * bits 0, 1 & 2 supply the interrupt level
 */
#define	SCSI_ICR			0xfffe102a	/* address of irq control reg */
#define SCSI_ICR_ENAB		0x08		/* irq enable flag */
#define	SCSI_ICR_MASK		0x07		/* irq level mask */
#define SCSI_ICR_REIP       0x40        /* SCSI Reset has been asserted */
#define SCSI_ICR_REST       0x80        /* SCSI Reset interrupt asserted */
	/* this is an "and" mask for the contents of the interrupt control
	 *  register, in order to clear the interrupt must write 1 to the
	 *  status bit.
	 */
#define SCSI_ICR_CLR  (SCSI_ICR_MASK | SCSI_ICR_ENAB)

/* end of file: scsi33c93.h */

