/*
 * definitions for use by ncs53c94 low level scsi subroutine module
 *
 * Copyright 1992, 1994, 1995 by Microware Systems (UK) Ltd.
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
 * ed.   date    reason                                           who.
 * --  --------  ---------------------------------------------    ---
 * 01  92/01/08  Converted from SCSI33C93 for ncr53c94/68340      AM
 * 02  92/03/12  Changed DMA channel used from 1 to 2             AM
 * 03  92/03/17  Changed fields in the control structure          AM
 *					---- OS-9/68K V3.0 Release ----
 * 04  94/08/02  Added MAXSCSI definition.                        wwb
 *					---- OS-9/68K V3.0.1 Release ----
 * 05  95/06/08  Corrected pathname case of reg.h.                ats
 *					---- OS-9/68K V3.0.2 Release ----
 *					---- OS-9/68K V3.1 Release ----
 */

#include	<rbf.h>
#include	<MACHINE/reg.h>
#include	<procid.h>
#include	<path.h>
#include	<errno.h>
#include	<ncr53c94.h>
#include	<dma68340.h>
#include	<scsidmod.h>
#include	<cmdblk.h>

/*
 * non-int function declarations
 */
u_char	*_f_trans ();

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
	       ll_cmd,          /* ncr53c94 command to use. */
	       nxt_phase,       /* next expected phase ... may not be cmd done */
	       syc_tried;		/* attempted synchronous arbitration */

	/* This is the "save pointers area.  All info nessesary to
	*   reactivate the command after a disconnect is stored here
	*/
	int    cntreg_valid;	  /* flag indicating cnt register is data xfer */
	u_char *curr_addr;        /* saved address, points to next data */
	long   curr_cnt;          /* count down counter of bytes to be transfered */
	u_char curr_mode;         /* current mode value */
	u_char curr_res[3];       /* reserved area ( preserve alignment ) */

	struct cmdblk cmdblock;   /* command block info from caller */

	u_char spcmd_phase,         /* last phase */
	       ext_msg,				/* extended message flag */
	       syc_per,				/* synchronous period register */
	       syc_off,				/* sinchronous offset register */
	       sp_xtrol,			/* control register byte */
	       dmastatus,           /* dma command completion status */
		   sp_term,		        /* terminal count reached */
		   sp_res;		        /* reserved ( for alignment ) */

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
	Hardware hdware;          /* ptr to ncr53c94 chip ( for kick() ) */
	int      mask_level,      /* sr mask image for disabling irqs */
	         usercount,       /* number of users of the low level driver */
	         busfree,         /* TRUE = device free, FALSE = device busy */
	         freshcmd,        /* on TRUE, a new command has been started */
	         statusreg,		  /* irq's scratch hold for status */
	         seqstep;		  /* irq's scratch hold for sequence step */
	CntrlBlk curr_cmd;        /* this is the currently active command */
	Dmahardware dmabase;      /* dma  hardware pointer */
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
 * pointer to ncr53c94 registers
 */
#define		HW(scsi_reg) (hw->scsi_reg)		/* macro for ncr53c94 regs */

/*
 * general definitions
 */
#define		TRUE				1			/* boolean TRUE */
#define		FALSE				0			/* boolean FALSE */
#ifdef NULL
#undef NULL
#endif
#define		NULL				0			/* pointer to nowhere */

#define		SELFID				7			/* id of the ncr53c94 on scsi bus */
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
#define IDENTIFY_DISCONN 0xC0		/* identify with disconnect enable */
#define BDRESET_MESSAGE  0x0C		/* bus device reset */
#define EXTENDED_MESSAGE 0x01		/* extended message flag */
#define REJECTED_MESSAGE 0x07		/* message byte reject */

/*
 * scsi bus transfer results
 */
#define		BUS_DONE			0			/* transfer completed */
#define		BUS_RERUN			1			/* rerun command */
#define		BUS_FATAL			2			/* fatal abort */

/*
 * scsi flags support
 *
 * this defines the pd_scsiopt flags that the low-level module
 * will support.
 */
/* #define		SCSI_SUPPORT		(SCSI_ATN | SCSI_PARITY | SCSI_SYNCHR) */
#define		SCSI_SUPPORT		(SCSI_ATN | SCSI_PARITY)

#define DMA340		0x3007A0	/* Address of 68340 DMA hardware */
#define DMA340VECT	65

#ifdef DMAIRQ
#define DMA_OUT		(CCR_INTN + CCR_INTE + CCR_DSTREQ + CCR_SAPI + \
					 CCR_SWORD + CCR_DWORD + CCR_REQEXTC + CCR_DUAL)

#define DMA_IN		(CCR_INTN + CCR_INTE + CCR_SRCREQ + CCR_DAPI + \
					 CCR_SWORD + CCR_DWORD + CCR_REQEXTC + CCR_DUAL)
#else
#define DMA_OUT		(CCR_DSTREQ + CCR_SAPI + \
					 CCR_SWORD + CCR_DWORD + CCR_REQEXTC + CCR_DUAL)

#define DMA_IN		(CCR_SRCREQ + CCR_DAPI + \
					 CCR_SWORD + CCR_DWORD + CCR_REQEXTC + CCR_DUAL)
#endif
				 
#define DMA_STOP	(CCR_STOP)

#define DMA_START	(CCR_STR)


/*
 * special phase indicators
 */

#define RESELECTED 0x80             /* reselected by target */
#define DATA_OUT 0x88				/* data out to target */
#define DATA_IN  0x89				/* data in from target */
#define CMD_OUT  0x8A				/* command out to target */
#define STAT_IN  0x8B				/* status in from target */
#define MESSAGE_OUT 0x8E			/* message out to target */
#define MESSAGE_IN  0x8F			/* message in from target */

#define TERM_DISCON 0x85			/* command termination disconnect */

#define SUSPEND 0xa5				/* marked for target disconnect */

/* end of file: scsi53c94.h */

