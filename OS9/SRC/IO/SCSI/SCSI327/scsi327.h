/*
 * definitions for use by vme327 low level scsi subroutine module
 *
 * Copyright 1990, 1994 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 */

#include	<rbf.h>
#include	<MACHINE/reg.h>
#include	<procid.h>
#include	<path.h>
#include	<errno.h>
#include	<scsidmod.h>
#include	<cmdblk.h>

/*
 * general definitions
 */
#define		TRUE				1			/* boolean TRUE */
#define		FALSE				0			/* boolean FALSE */
#define		NULL				0			/* pointer to nowhere */

#define		SELFID				7			/* id of the vme327 on scsi bus */
#define		MAXTIME				100			/* bus timeout value (800mS) */
#define		SUCCESS				0
#define		SUPSTAT				0x2000		/* supervisor state (68xxx SR) */
#define		MAXSCSI				7			/* max scsi id supported */

		/* vme address modifiers */
#define		VME_SUPDAT			0x3D		/* supervisory data access */
#define		VME_EXSUPDAT		0x0D		/* supervisory data access */

#define     EXTMSK              0xff000000  /* used to select add. mod */

#define LLMODACCESS (MP_OWNER_READ|MP_OWNER_WRITE|MP_GROUP_READ|MP_WORLD_READ)

		/* event values */
#define CBFREE 0xDEAD		/* event is free */
#define CBWAKE 0xD065		/* event wakeup value */
#define CBBUSY 0xCA15		/* event in use */

/*  These are used in constructing the  SCSI identify message */
#define IDENTIFY 0x80		/* SCSI  identify  */
#define ID_DISC  0x40		/* bit which indicates disconnect allowed
                             *  when included in SCSI ID message 
                             */
#define DISCON (IDENTIFY|ID_DISC)
#define NODISCON IDENTIFY

		/* device types */
#define SCSIDEVICE 0x5		/* this is device on the SCSI bus */

#ifndef __STDC__
#ifndef volatile
#define volatile /* the old compiler doesn't understand 'volatile' */
#endif
#endif

#ifndef SERIALIZE_M
#define SERIALIZE_M()		/* need { _asm(" nop "); } for 68060? */
#endif

/*
 *	This is the register layout of the VME327 board 
 */
typedef volatile struct hware {
	char    	*madd_reg;			/* address register (MSW/LSW */
	u_char		addr_mod,			/* address modifier/ bus width */
				buss_wth,			/* bus width */
	    		xtrl_reg,			/* control register */
	            reser,				/* reserved byte */
	    		stat_reg,			/* status register */
	      		diag_reg;			/* diagnostic register */
	u_short		not_use1,			/* next two not used by the board */
				not_use2,
				tas_reg;			/* test and set register. */
} hardware, *Hardware;

/* usefull bits and such by register for the hardware */
#define CSR_ATTEN 0x20 		/* tells board to wake up */
#define CSR_BUSY 0x80		/* control register busy flag */

/* define the bus width for DMA transfers
 */
#define DMA_16 1  /* bus width is 16 bits only */
#define DMA_32 2  /* bus width is 32 bits. */
#define DMA_WIDTH DMA_16

/* bits in tas register, must be defed as byte size */
#define TAS_BIT 0x8000		/* the test and set bit */
#define TAS_VCM 0x4000		/* valid command bit */
#define TAS_STV 0x2000	/* bit indicates status byte valid */
#define TAS_CCP 0x1000		/* command complete */

#define TAS_CMD_MASK 0x0fff		/* these bits are the command bits */

/*
 *   These are the CSR based command that use the TAS register.
 */
#define CREATE_CHAN 0x1		/* create a command channel */
#define DELETE_CHAN 0x2		/* delete a command channel */

/* 
 *  These are the values that can be expected in the CSR status register.
 *     NOTE:  $AA through $EE are catastrophic errors
 */
#define	CMD_COMPL	0	/* correct command completion */
#define BAD_CMAND	1	/* invalid command */
#define BAD_RHEAD	2	/* could not read channel header */
#define BAD_WHEAD	3	/* could not write channel header */
#define DMA_RFAIL	4	/* VME DMA read failure */
#define DMA_WFAIL	5	/* VME DMA write failure */
#define CHAN_OVFL	6	/* no free channels available */
#define BAD_CHANL	7	/* channel does not exist */
#define FATAL_LAE	$AA	/* local address error ( onboard the vme327 ) */
#define FATAL_ACF	$AC	/* local AC Fail seen  "  "   "   "  "    "   */
#define FATAL_LBE	$BB	/* local Bus Error     "  "   "   "  "    "   */
#define FATAL_CTE	$CC	/* local confidence test failure              */
#define FATAL_UEE	$EE	/* unexpected Exception occured	              */


/* 
 *  Here we define the SCSI Specific Packet.  This is used to talk to
 *  the devices on the SCSI bus as opposed to the "High Level" command
 *  set which proves to be much more specific than we need.
 */
typedef volatile struct sspack {
	u_char *ssp_link;		/* for linked SCSI commands (not used) */
	u_short ssp_cntrl;		/* SCSI control word (see below) */ 
	u_char  ssp_cmdlen,		/* length of the SCSI command block */
	        ssp_hole,		/* reserved for future use by motorola */
	        ssp_cdb[12];	/* the SCSI command block ( from driver ) */
	long    ssp_datlen;		/* 24 bits, size of the data (if any) */
	char   *ssp_datptr;		/* buffer pointer ( data xfer phase ) */
	u_char  ssp_scsist,		/* SCSI status word */
	        ssp_initID,		/* initiator ID  ( used in target mode only ) */
	        ssp_miflg,		/* flag indicating message_in is:  0 in sspack
	                   		 * or $ff in separate area pointed to by.... */
	        ssp_moflg;		/* flag indicating message_out is:  0 in sspack
	                   		 * or $ff in separate area pointed to by.... */
	u_short ssp_misiz;			/* size of message in data */
	u_char *ssp_miptr,			/* .... pointer to message in data */
	        ssp_mi[6];			/* space for 6 bytes of message in */
	u_short ssp_mosiz;			/* size of message in data */
	u_char *ssp_moptr,			/* .... pointer to message out data */
	        ssp_mo[6],			/* space for 6 bytes of message out */
#define SCRIPT_SIZE 8
	        ssp_script[SCRIPT_SIZE];	/* SCSI command bus phase script */

	    /* in order to allow the IRQ to connect the status envelope
	     *   with a process we extend this structure to include the
	     *   event ID that started up this command.
	     */
	int    ssp_ievent;
} scsispec, *SCSIspec;

/*
 *  These are definitions for the SCSI control word.
 */
#define SCT_DMA  0x8000 /* if set, data phase uses DMA controller ( use only
                      	 *   for reads and writes  )
                      	 */

#define SCT_SYNC 0x4000		/* if set, data phase is synchronous */
#define SCT_PAR  0x2000		/* if set, parity checking is enabled */
#define SCT_SCHK 0x1000		/* if set status checking is done by the user */
#define SCT_LDAT 0x0800		/* if set, can do longword DMA */
#define SCT_BSWP 0x0400		/* if set, perform a byte swap on data phase */
#define SCT_SG   0x0200		/* if set, do scatter gather operation */
#define SCT_LINK 0x0100		/* if set, do a linked SCSI command */
#define SCT_NATN 0x0080		/* if set, do not select with attention */
    /* bits 6 -> 0 are reserved */
    
/* 
 * these are definitions for he values that will be in the script.
 */
#define SCRIPT_DO	0		/* data out */
#define SCRIPT_DI	1		/* data in  */
#define SCRIPT_CM	2		/* command  */
#define SCRIPT_ST	3		/* status   */
#define SCRIPT_MO	6		/* message out */
#define SCRIPT_MI	7		/* message in  */
#define SCRIPT_TM	8		/* script terminator */
#define SCRIPT_PD   0x56	/* script pad value */

/*
 *   This is the Cutsom SCSI command packet
 */
typedef volatile struct sccp {
#define LLCMDCODE 0x26
	u_char  cp_cmd,			/* command */
	        cp_re1,			/* reserved - reX should be set to 0 */
#define SCSIDEV 0x5
	        cp_dtp,			/* device type  = 0x5 */
	        cp_unt,			/* unit number */
	        cp_re2,
	        cp_re3,
	        cp_amf,			/* address modifier */
	        cp_dbw;			/* data bus width */
	SCSIspec cp_packt;		/* packet to send */
	long    cp_sgl,			/* scatter/gather list pointer */
	        cp_re4;
	u_short cp_sgct;		/* scatter/gather count */
	long    cp_re5;
	u_short cp_re6;
	u_char  cp_fec,			/* fatal error code */
	        cp_rst;			/* recovered error status */
    u_short cp_ast;			/* additional error code/status */
	u_char  cp_rty,			/* retry count */
	        cp_re7;
	long    cp_esa,			/* error status address */
	        cp_ttc,			/* termination transfer count */
	        cp_re8;
	u_short cp_sp3;			/* status parameter 3 from target */
} spcmd, *Spcmd;

/* grouped together for allocation, these are used for each board
 *    access 
 */
typedef struct cpkt {
	spcmd         sc;			/* the board level command for vme327 */
	scsispec      sp;			/* SCSI level command for target */
} cpack, *Cpack;

/* 
 *  This is a BPP envelope.
 */
typedef volatile struct envel {
	volatile struct envel *link_ptr;/* this is a pointer to the next envelope */
	Spcmd        cmd_stat;			/* pointer to the command/status packet */
	u_char       env_valid,			/* envelope valid flag */
	             envreserve[3];		/* three reserved bytes */
} envelope, *Envelope;
	
/*
 *  This is a command channel header.
 */
typedef volatile struct cch {
	Envelope cmd_head,	 		/* command pipe head pointer */
	         cmd_tail,			/* command pipe tail pointer */
	         stat_head,			/* status pipe head pointer */
	         stat_tail;			/* status pipe tail pointer */
	u_char   int_level,			/* interrupt level for the channel */
	         int_vector,		/* interrupt vector for the channel */
	         chan_prior,		/* channel priority */
	         addr_mod,			/* address modifier */
	         chan_number,		/* channel number */
	         valid_flag,		/* channel valid flag */
	         data_width,		/* data bus width */
	         cch_res;			/* reserved */
} cmdchan, *CmdChan;

/*
 * static storage used by low level driver.
 */
typedef volatile struct lstat {
	Datmod   dmptr;				/* ptr to data module (for STATS) */
	Hardware hdware;			/* ptr to vme327 chip */
	int      mask_level,		/* sr mask image for disabling irqs */
	         irqlevel,			/* level that the device is to run */
	         irqvector,			/* vector for the device */
   	         dma_mode;			/* copy of pd_tmf field */
	long     reloc;				/* relocation constant for this storage space */
	cmdchan  sc_chanel;      	/* command channel for the SCSI devices */
	int sc_events[8];			/* event IDs associated with the SCSI devices */
	int sc_opts[8];				/* options word */
	Envelope freptr;			/* pointer to next free packet */
#define NEVL 32
	envelope sc_env[NEVL];	/* envelope pool for SCSI devices */
#define MAX_SC 12
	cpack    sc_cmst[MAX_SC];  /* SCSI command/status packets */
#ifdef DEBUGDRVR
#define BUGGERS 30
	int ndbgpak;					/* number of debug packets in buffer */
	cpack buggers[BUGGERS];	/* place to hold the scsi packets */
#endif	
} lstat, *Lstat;

/* these are the expected options bits in sc_opts */
#define SCOPT_SYNCTRIED 1			/* have attempted sync negotiations */
/* Change at Ed #17: SCOPT_SYNCOPER no longer used */
/* #define SCOPT_SYNCOPER  2			/* device is running synchronous */

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
 *   mVme327 Fatal Error Codes - these are specific to the mVme327
 *      and have no exact analog to any other board.  These error
 *      codes are returned in the status section of the command block.
 *      These are used by errstat().
 */
 
            /*  Command parameter errors */
#define FEC_BADDES  0x1		/* bad descriptor */
#define FEC_BADCMD  0x2		/* bad command */
#define FEC_UNICMD  0x3		/* unimplimented command */
#define FEC_BADDRV  0x4		/* bad drive */
#define FEC_LOGADR  0x5		/* bad logical address */
#define FEC_BADSGT  0x6		/* bad scatter/gather table */
#define FEC_UNIDEV  0x7		/* unimplemented device */
#define FEC_NOTINT  0x8		/* unit not initialized */

           /*  Media Errors */
#define FEC_NOID    0x10	/* no ID found on track */
#define FEC_SEEK    0x11	/* seek error */
#define FEC_RELTRK  0x12	/* relocated track error */
#define FEC_BADID   0x13	/* Record not found, bad ID */
#define FEC_BADSNC  0x14	/* data sync fault */
#define FEC_BADECC  0x15	/* ECC error */
#define FEC_RNF     0x16	/* record not found */
#define FEC_MEDIA   0x17	/* media error */

			/*  drive errors */
#define FEC_DRVFLT  0x20	/* drive fault */
#define FEC_WRPROT  0x21	/* Write protected media */
#define FEC_MTROFF  0x22	/* motor not on */
#define FEC_DOOR    0x23	/* door open */
#define FEC_DRVNTR  0x24	/* drive not ready */
#define FEC_DRVBSY  0x25	/* drive busy */

			/*  VME DMA Errors */
#define FEC_VMEBERR 0x30	/* bus error!! */
#define FEC_ADDRERR 0x31	/* bad address alignment */
#define FEC_BUSTOUT 0x32	/* bus time out */
#define FEC_BADCONT 0x33	/* Invalid DMA xfer count */

			/* Disk Format Errors */
#define FEC_NOALTS  0x40	/* not enough alternates */
#define FEC_FMTERR  0x41	/* format failed */
#define FEC_VERERR  0x42	/* verify failed */
#define FEC_BFPARM  0x43	/* bad format parameters */
#define FEC_CFBS    0x44	/* can't fix bad spot */
#define FEC_TMDEF   0x45	/* to many defects */

			/* mVme327 Specific Error Codes */
#define FEC_SCSIASA 0x80	/* SCSI error, additional status available */
#define FEC_INENAI  0x81	/* Indeterminate media error, no additional */
#define FEC_IHE     0x82	/* Indeterminate hardware error */
#define FEC_BLNKCK  0x83	/* Blank Check (EOD or corrupted WORM ) */
#define FEC_IEXMES  0x84	/* Incomplete extended message from target */
#define FEC_INVRES  0x85	/* invalid reselection by unthreaded target */
#define FEC_NSTST   0x86	/* no status returned from the target */
#define FEC_MONTR   0x87	/* message out not transefered to target */
#define FEC_MINREC  0x88	/* message in not recieved from target */
#define FEC_ICDATR  0x89	/* incomplete data read to private buffer */
#define FEC_ICDATW  0x8A	/* incomplete data write to private buffer */
#define FEC_UDPHAS  0x8B	/* undefined phase requested */
#define FEC_BADCSZ  0x8C	/* incorrect size for CDB */
#define FEC_TOSEL   0x8D	/* timeout on select phase */
#define FEC_DRESET  0x8E	/* command terminated due to SCSI reset */
#define FEC_IVMESS  0x8F	/* invalid message recieved */

#define FEC_CMDNR   0x90	/* command not recieved */
#define FEC_UESTAT  0x91	/* unexpected status phase */
#define FEC_SCRIPT  0x92	/* script mismatch */
#define FEC_UEDISC  0x93	/* unexpected disconnect caused termination */
#define FEC_RSFAIL  0x94	/* request sense command failed */
#define FEC_NWDESC  0x95	/* no write descriptor for this drive */
#define FEC_ICDAT   0x96	/* incomplete data transfer */
#define FEC_SOL     0x97	/* out of local resources for command processing*/
#define FEC_LMF     0x98	/* local resources lost */
#define FEC_CRHOST  0x99	/* channel reserved for another VME host */
#define FEC_DVHOST  0x9A	/* device reserved for another SCSI device */
#define FEC_AETARG  0x9B	/* already enabled, expecting target respon. */
#define FEC_NOTARG  0x9C	/* target not enabled */
#define FEC_UCT     0x9D	/* unsupported controller type */
#define FEC_UPT     0x9E	/* unsupported peripheral device type */
#define FEC_BSMIS   0x9F	/* block size mismatch */

#define FEC_IVCYL   0xA0	/* invalid cylinder number in defect list */
#define FEC_IVHD    0xA1	/* invalid head number in defect list */
#define FEC_NFBLKM  0xA2	/* non-fatal block size mismatch */
#define FEC_SCSINC  0xA3	/* our ID was not changed by command */
#define FEC_SCSIIC  0xA4	/* our ID was changed by command */
#define FEC_NTE     0xA5	/* no target enable has been completed */
#define FEC_NLWT    0xA6	/* cannot do long word transfers */
#define FEC_NDMAT   0xA7	/* cannot do DMA transfers */
#define FEC_ILBS    0xA8	/* illegal logical block size */
#define FEC_STMISM  0xA9	/* sectors/track mismatch */
#define FEC_NHMM    0xAA	/* Number of heads mismatch */
#define FEC_NCMM    0xAB	/* Number of cylinders mismatch */
#define FEC_IFPAR   0xAC	/* invalid floppy parameters */
#define FEC_ISRES   0xAD	/* is already reserved */
#define FEC_NOTRES  0xAE	/* is not reserved */
#define FEC_IVSN    0xAF	/* invalid sector number  */

#define FEC_STFAIL  0xcc	/* self test failed */

#define AST_PHSMSM	0x92	/* phase mismatch during SYNC neg */

/* end of file: scsi327.h */
