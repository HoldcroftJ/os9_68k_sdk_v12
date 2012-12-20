/*
 * scsisiop.h - definitions for use by scsi710 low level scsi subroutine module.
 *             (NCR SIOP SCSI I/O Processor)
 */
 
/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1991-2001 by Microware Systems Corporation         |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 96/08/03 Added edition history.                                  rak   |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
| 02 97/03/20 Added support for MVME172.                              GbG   |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
| 03 97/04/20 Added WIDE and ULTRA support.                                 |
|             Also added debug code.                                  GbG   |
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 04 97/11/17 Added phythread_local for address translation for             |
|             the NCR53c710.  (Conditionalized with LCLMEM)           gdb   |
| 05 98/01/29 Fixed compiler warnings.    .                           GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
| 71 01/08/09 change debug code to use dbglog.                        gdw   |
| 72 09/20/01 Improve the fariness of access to disk (CF10781)        peng	|
`--------------------------------------------------------------------------*/

#if !defined  (__STDC__) && !defined (_ANSI_EXT) && !defined (volatile)
#define volatile /* the old compiler doesn't understand 'volatile' */
#endif

#if !defined  (__STDC__) && !defined (_ANSI_EXT) && !defined (const)
#define const /* the old compiler doesn't understand 'const' */
#endif


#define DBIT_ERR  0x001
#define DBIT_ENLV 0x002
#define DBIT_INIZ 0x004
#define DBIT_HW   0x008
#define DBIT_ALL  0xfff

#if defined(DEBUG)
#include <LIB/dbglog.h>
#define DPRINTF(bit, x) \
  do {                  \
    dbglog_printf x;    \
  } while (0)
#define DPUTS(bit, x) \
  do {                  \
    dbglog_puts x;    \
  } while (0)
#define DENTER(f)       \
  do {                  \
    dbglog_puts(" Entering " f " (" __FILE__ ")"); \
  } while (0)
#define DLEAVE(f)       \
  do {                  \
    dbglog_printf(" Leaving " f " (%u)", __LINE__); \
  } while (0)
#else
#define DPRINTF(bit, x)
#define DPUTS(bit, x)
#define DENTER(f)
#define DLEAVE(f)
#endif


#ifdef CBOOT
#include <sysboot.h>
#else
#include <rbf.h>

#if defined (_OSK)
#include <Machine/reg.h>
#include <path.h>
#include <procid.h>

#elif defined (_OS9000)
#include <process.h>
#include <memory.h>
#endif

#include <errno.h>
#include <cmdblk.h>
#endif
#include <const.h>
#include <scsidmod.h>
#include <signal.h>
#include <scsicmds.h>

/*
 * general definitions
 */
#ifndef TRUE 
#define TRUE  (1==1) 		/* boolean TRUE */
#define FALSE !TRUE 		/* boolean FALSE */
#endif

#ifndef MEM_ANY
#define MEM_ANY 0
#endif

#ifdef MVME1601
#define SELFID   7				/* Our SCSI ID */
#endif /* MVME1601 */

#ifdef VME167
#define SELFID   7				/* Our SCSI ID */
#endif /* VME167 */

#ifdef VME177
#ifdef ALTID6
#define	SELFID	6
#endif
#ifndef	SELFID
#define SELFID	7				/* Our SCSI ID */
#endif
#endif /* VME167 */

#ifdef VME162
#ifndef ALTID
#define SELFID	7				/* Our SCSI ID */
#else /* ALTID */
#define SELFID	3		/* temporary SCSI ID for in-house usage */
#endif /* ALTID */
#endif /* VME162 */

#ifdef VME172
#ifndef ALTID
#define SELFID	7				/* Our SCSI ID */
#else /* ALTID */
#define SELFID	3		/* temporary SCSI ID for in-house usage */
#endif /* ALTID */
#endif /* VME172 */

#ifndef SELFID
#define SELFID   7				/* Our SCSI ID */
#endif

#define B_SELFID (1<<SELFID) 	/* bit number of our ID */

#if !defined(MAXSCSI)
#if defined(NCR53C8XX)		
#define MAXSCSI	15				/* max scsi id supported */
#else
#define MAXSCSI	7				/* max scsi id supported */
#endif
#endif

#define ONE_SECOND    0x80000100  /* arguments to F$Sleep */
#define FIVE_SECONDS  0x80000500
#define ONE_TICK      1
#define UNTIL_DONE    0

#define SEMTO         5000		/* maximum number of time slices to wait */

/* access permissions for data module */
#define LLMODACCESS	(MP_OWNER_READ|MP_OWNER_WRITE|MP_GROUP_READ|MP_WORLD_READ)

/*
 * scsi flags support
 *    This defines the pd_scsiopt flags that the low-level module
 *    will support.
 */
#if defined(NCR53C8XX)		
#define SCSI_SUPPORT (SCSI_ATN | SCSI_PARITY | SCSI_SYNCHR | SCSI_WIDE | SCSI_ULTRA | SCSI_SLOW)
#else
#define SCSI_SUPPORT (SCSI_ATN | SCSI_PARITY | SCSI_SYNCHR | SCSI_SLOW)
#endif
/*
 *  definition of maximum DMA size.
 */
#define MAXDMA 0xFFFFFF		/* 24 bits */

/*
 *  For correct operation the SIOP must have the core clock divisor 
 *  properly set.  This may (mVme167) be a function of BCLK in the
 *  system or based on a separate dedicated clock.  This definition
 *  allows the setting of bus clock based on a command line option.
 */
#ifndef SCLK
#define SCLK_FREQ get_siop_clock()	/* function available to get SCLK */
#else
#define SCLK_FREQ SCLK				/* get defined SCLK */
#endif

/*
 *  These definitions pertain to the SCSI message system.
 */
#define M_COMPLETE   0x0	/* command complete */
#define M_EXTENDED   0x1   /* Extended message identifier */
#define M_SAVEPOINT  0x2	/* Save data pointers (interum discon.) */
#define M_RESTPOINT  0x3	/* Restore data pointers (interum discon. ) */
#define M_DISCONNECT 0x4	/* Disconect message (interum discon.) */
#define M_INITERROR  0x5	/* Initiator detected an error */
#define M_ABORT      0x6	/* Abort Operation Message */
#define M_REJECT     0x7	/* Message Rejected */
#define M_NOOP       0x8	/* No-Op -- no other valid message to send */
#define M_MPARITYER  0x9	/* Message Parity error */
#define M_DEVRESET   0xC	/* Tell the device to reset */

#define SYNC_REQUEST      0x1	/* Synchronus Transfer Negotiations */
#define SYNCREQ_SIZE      0x3   /* Size of additional bytes in Sync Req. */
#define IDENTIFY_NODISCON 0x80	/* Identify message, no disconnect allowed */
#define IDENTIFY_DISCON   0xC0	/* Identify message, disconnect supported */

#define WIDE_REQUEST      0x3	/* Wide data Transfer Request */
#define WIDEREQ_SIZE      0x2   /* Size of additional bytes in Sync Req. */
#define WIDE_SIZE_8BIT	  0x0	/* Wide transfer size = 8bits */
#define WIDE_SIZE_16BIT	  0x1	/* Wide transfer size = 16bits */
#define WIDE_SIZE_32BIT	  0x2	/* Wide transfer size = 32bits */


/*
 *  NOTE:  The script will place not place the entire synchronous negotiation
 *         response in the message in buffer.  The offsets below can be used
 *         to access the message in buffer with the synchronous negotiation
 *         inside.
 */
#define IN_SYN_SYNM   0x00		/* offset to synchronus identifier */
#define IN_SYN_PERIOD 0x01		/* offset to the period byte */
#define IN_SYN_OFFSET 0x02		/* offset to the offset byte */
#define SYN_ASYNC     0x0       /* if offset in message = 0 then async */

/* Definitions that are used with-in the thread structures (defined below)
 *     (lcthread.threadstate)
 */
#define TH_EMPTY    0x0000		/* Thread not in use */
#define TH_PENDING  0xD065		/* Thread pending submition to bus */
#define TH_LOGICAL  0xBA75		/* Active thread:  disconnected */
#define TH_PHYSICAL 0xCA75		/* Active thread:  currently physical */
#define TH_DONE     0x70AD		/* Thread has completed operation */

/*
 *  Definition of the thread structure.
 *
 *   There is a thread structure for each of the possible 8 SCSI id's.
 *   The structure that is associated with SELFID is the structure that
 *   is used as the IDLE thread.  There will be a changes nessesary if
 *   target mode is implemented.
 *
 *  NOTE:  The definition of the table indirect space must agree between
 *         this definition, and the scripts definition.  
 */

typedef volatile struct logical_thread {
	/*  This space used by Table Indirect mode. */
   	int32  datacount;		/* number of data bytes for this command */
	u_int8 *dataptr;		/* pointer to the data bytes for this command */
	int32  cdblength;		/* length of the SCSI command block */
	u_int8 *cdbptr;			/* pointer to the SCSI command block */
	int32  statuscount;		/* always 1 byte */
	u_int8 *statusptr;		/* pointer to the status buffer */
	int32  msgoutlen;		/* length of message out for this command */
	u_int8 *msgoptr;		/* pointer to the message out buffer */
	int32  msginlen;		/* size of message in buffer for this command */
	u_int8 *msgiptr;		/* pointer to the message in buffer */
	
	/* This field holds the SCSI ID and Synchronus register images */
	union {
		u_int32 device;				/* when used as a pointer for the script */
		struct instfields {
		unsigned  
#if !defined(NCR53C8XX)		
				  devresv:   8,		/* ..... reserved field ......        */
		          scsidev:   8,		/* SCSI Target ID                     */
	        	  sxfr_mask: 8,		/* synchronus transfer register value */
		          devres1:   8;		/* ..... reserved field ......        */
#else
				  devresv:   8,		/* ..... reserved field ......        */
	        	  sxfr_mask: 8,		/* synchronus transfer register value */
		          scsidev:   8,		/* SCSI Target ID                     */
		          scntl3:   8;		/* SCSI CONTROL REGISTER 3 */
#endif

		} devsync;
	} devfield;
	/* end of table indirect data structure */
	
	volatile u_int8 cmdstatus[4];	/* SCSI status will be here, always 1 byte */
	volatile u_int8 msgibuf[0x4];	/* message in buffer ( extended messages global) */
	volatile u_int8 msgobuf[0x10];	/* message out buffer */

	volatile u_int8 tgp_rec;		/* target period */	
	volatile u_int8 tos_rec;		/* target offset */		
	volatile u_int8 tgp_final;		/* target period */	
	volatile u_int8 tos_final;		/* target offset */		

	/* Only the CPU will access the remainder of the structure */
	u_int8 *scriptstart;	/* address within the script at which to start */
	int32  threadstate,		/* status of this thread. */
	       chiperror,		/* error indications from the chip */
	       synctried,		/* flag indicating that synch message sent */
	       widetried,		/* flag indicating that wide message sent */
	       processid,		/* process ID associated with the command */
	       thread_sem,		/* semaphore for this ID */
	       phaseflg,		/* copy of scratch register at save_state */
	       xferflags,		/* local copy of scsi flags from path descriptor */
	       sbclmask,		/* mask that contains the bits for sbcl register */
	       lthresv;			/* reserved */
	Cmdblk cbkptr;			/* pointer to command block from high level */
	u_int32 waiters;		/* the number of waiters for semaphore */ 
} lthread, *Lthread;		/* end of logical thread structure */

/*  for use with synctried (above)  see set_msg() and check_synchmsg() */
#define NEGOTIATE 0x80		/*       negotiations in progress */
#define SYNCTRIED 0x01		/*       flag that this has been attempted */
#define WIDETRIED 0x02		/*       flag that this has been attempted */

/*
 * scsi command packet for low-level module
 */
union scsipkt {
	volatile struct std_str {
		u_int8		sp_opcode;			/* the opcode */
		unsigned	sp_lun		: 3;	/* logical unit (filled in by driver)*/
		unsigned	sp_lbamsbs	: 5;	/* the block address (msbs) */
		u_int8		sp_lba[2];			/* (the rest of) the block address */
		u_int8		sp_blkcnt;			/* the block count */
		u_int8		sp_options;			/* command options */
	} std;
	volatile struct ext_str {
		u_int8		sp_opcode;			/* the opcode */
		unsigned	sp_lun		: 3;	/* logical unit (filled in by driver)*/
		unsigned	sp_zero		: 5;	/* zero */
		u_int32		sp_lba;				/* logical block address */
		u_int8		sp_zero2;			/* zero */
		u_int8		sp_count[2];		/* transfer count */
		u_int8		sp_options;			/* command options */
	} ext;
};

/*
 *  Bomblog - logs the circumstances of the last fatal problem encountered.
 *     This structure is a snapshot of the command that had a problem.
 *     see irqsup.c  bomblog()
 */
typedef volatile struct bmlog {
	int32 devf, 			/* device field of the bombed process */
	    cdsiz,				/* data size counter (at error) */
	    errlog;				/* error that created the log */
	struct cmdblk cmd;		/* command block */
	union scsipkt spkt;		/* SCSI command */
} *Bmlog;

/*
 * static storage used by low level driver
 */
#define NTHREADS  (MAXSCSI+1)	/* one structure per SCSI ID */
#define MAXEXTMSG 0x100		/* maximum size of an extended message */
#define MAXSCRIPT 2048		/* max size in bytes of script */
#define MAX_SRAM_SZ 4096	/* Max size for SCRIPTS RAM on 825/875 */

/* volatile */

struct lstat {
	Datmod   dmptr;					/* ptr to data module */
	u_int32  hdware;				/* ptr to NCR 53C710 chip */
	int32    mask_level,			/* sr mask image for disabling irqs */
	         usercount,				/* # of users of the low level driver */
	         irqlevel,				/* interrupt level used by the board */
	         irqvector,				/* interrupt vector used by the board */
	         irqprior,				/* interrupt table polling priority */
	         coreclock,				/* SCSI core clock value */
	         selfid,				/* SCSI ID of this device */
	         snatnaddr,				/* relocate will post the addresses ... */
	         swatnaddr,				/* ... of the select entry points here... */
	         lstpad,				/*    ---   pad out for eye alingment */
	         wakes,					/* number of wakeups in a buffer */
	         wakequeue[8],			/* 7 id's, 7 possible wakes.... */
	         sbclmaster,			/* master copy of the sbcl register */
	         chipbusy,				/* non zero means chip busy (reset) */
	         lsresv[1];				/* reserved */
	
		/* longword alignment MUST be maintained before this structure or
		 *   the SIOP memory move instructions will fail!
		 */
	union {
		int32 armessage;		/* the entire field: abort/reject message */
		struct {
			u_int8 resv1,		/* two reserved */
			       mpe,			/* message parity error */
			       abort,		/* abort message */
			       reject;		/* reject message */
		} msg;
	} armsg;

	union {
		u_int32 sip;		/* flag indicating select in progress */
		u_int8 sipflag[4];
	} selflg;

	/* extended message frame and stash */
	u_int8  *extmsgfram;		/* pointer to the frame (size/ptr) */
	union {
		int32 extenmessgcount;	/* size of extended message */
		struct {
			u_int8 cnt[4];		/* fields within the size */
		} mcnt;
	} extcnt;
	u_int8  *extmsgbufptr;		/* pointer to the buffer	 */

	/* script writes siop->lcrc here at reselect time */
	union {
		int32 lcrcstash;		/* lrc register at reselect time */
		struct {
#if !defined(NCR53C8XX)
		u_int8 tid,				/* target ID of reselector */
			   resv1[2],		/* reserved */
#else
		u_int8 resv1,			/* reserved */
			   tid,				/* target ID of reselector */
			   resv2,			/* reserved */
#endif
			       idmsg;		/* identify message on reselect */
		} lcrcid;
	} lcrcs;

	Lthread  phythread;						/* Physical thread (pointer) */
#if defined(NCR53C8XX)
	u_int32	phythread_swap;				/* Physical thread (pointer) */	
#endif
#if defined(NCR53C710) && defined(LCLMEM)
	u_int32	phythread_local;	/* Physical thread (pointer) */
#endif
	lthread  logthread[NTHREADS];			/* logical thread storage */
	u_int8   extmsgbuffer[MAXEXTMSG];		/* the extended message buffer */
	struct bmlog thebomb;					/* log for retryable problems */

	/*  This next MUST be LONGWORD ALIGNED!! (see script.c: relocate()) */
#if defined(NCR53C8XX)
	u_int32 pci_io_base;					/* PCI saved I/O base */
	u_int32 pci_memory_base;				/* PCI saved MEM base */
	u_int32 pci_ram_base;					/* PCI save SCRIPTS RAM base */
	u_int32 script_sz;						/* SCRIPTS size */
	u_int32 act_script;						/* pointer to the script */
#else
	u_int8 act_script[MAXSCRIPT];			/* pointer to the script */
#endif
	u_int8 lastthread;						/* last thread index */
	u_int8 tmphold[3];

#ifdef STATS
	u_int8 stats_msg[4];		/* to help find stats */
	int32  grosscmd,			/* gross count of entries into exec()   */
		   ninterr,				/* gross count of interrupts processed  */
		   nentries,			/* number of entries in the debug table */
	       *nextin,				/* pointer to the next available entry  */
#define DBLOGSZ  256			/*   number of entries allowed */
	       irqlog[DBLOGSZ];		/* Interrupt log */
	u_int8 syn_period,			/* target period */
		   syn_offset,			/* target offset */
		   scsi_timing;			/* SCSI Timing 0=async - 1=SCSI I - 2=SCSI II */
#endif /* STATS */
	u_int16 ncr_chip_type;		/* NCR Chip Type */
	u_int16 ncr_chip_rev;		/* NCR Chip Revision */
	u_int16	ncr_clock;			/* NCR Clock Freq */	
	u_int16 ncr_min_period;		/* NCR Min Period */
	u_int16 ncr_max_syncoffset;	/* NCR Max Offet */	
	u_int16 ncr_wide;			/* NCR Wide Flag */
#if defined(OS_CACHE_FLUSH) && defined(_OS9000)
	error_code (*cache_cctl)(u_int32 control, u_int32 *addr, u_int32 size);
#endif
};

typedef struct lstat *Lstat;	/* lets get object oriented!!! */

/*  for use with chipbusy above,  think of this as the next item to do */
#define DEASERTRST 1		/* SCSI Bus reset is asserted, remove it */
#define RSTDONE    2		/* Time delay after reset has completed */
#define CHIPFREE   0		/* Chip is free for use */

/*
 *  macro for removing debug code when not debugging.
 */
#ifndef STATS
#define dblog(a,b) {}
#endif

/*
 * high-level driver static usable by the low-level driver
 *
 * this is limited to the rbf defined portion and relies on the driver
 * providing the pointer to its static storage in register a6.
 * The setup of a6 is accomplished by the "glue" code of the high
 * level driver.
 *
 */
#ifndef CBOOT
extern struct rbfstatic rbfstat;
#endif

/*  DSPS messages of the error variety (usually protocol violations) */
#define NOTMSGO   0x0FF01		/* not message out after selection */
#define NOTCMD    0x0FF02		/* unexpected phase before command */
#define NOTSTAT   0x0FF03		/* not status phase after data transfer */
#define NOTMSGI   0x0FF04		/* not message in after status phase */
#define UNKNPH    0x0FF05		/* unidentified next phase */
#define BADMSG    0x0FF06		/* unidentified message in recieved */
#define NOTCON    0x0FF07		/* not connected (HARDWARE FAULT) */
#define NOTSUP    0x0FF08		/*  not supported (ie. target mode) */

/* definitions used for debugging purposes only! (see SCSI_Debug) */
#define SCSI_DEBUG 0xB00B0001
#define ILLSCRIPT  SCSI_DEBUG+1
#define DMAWATCH   SCSI_DEBUG+2
#define BADPARITY  SCSI_DEBUG+3
#define GROSSERROR SCSI_DEBUG+4
#define NOIRQCAUSE SCSI_DEBUG+5

	/* durring testing and evaluation these are used to determine 
	 *  frequency of interrupts and last known operation.
	 */
		  /* These are chip generated interrupts */
#define DMABE    0xC100		/* DMA bus Error!! */
#define DMAWD    0xC101		/* DMA watch dog timer error */
#define ILLIS    0xC102		/* Illegal script instruction */

		  /* These are SCSI error interrupts  */
#define SPARITY 0x5C510000		/* SCSI Bus parity error */
#define SRESET  0x5C510001		/* SCSI Bus reset detected */
#define SUDC    0x5C510002		/* Unexpected disconnect */
#define SGE     0x5C510003		/* SCSI chip Gross Error */
#define SRESEL  0x5C510004		/* reselect */
#define SELTO   0x5C510005		/* selection timeout */
#define SFCMP   0x5C510006		/* Function Complete */
#define SPM     0x5C510007		/* phase mismatch */
#define SPMNPT  0x5C510008		/* phase mismatch - no physical thread */

#define NOTOURINT 0xBA7FECE5


#include <ncrproto.h> 				/* get prototype information */


