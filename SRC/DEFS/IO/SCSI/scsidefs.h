/*
 * $Header:   /h0/MWOS/SRC/DEFS/IO/SCSI/VCS/scsidefs.h_v   1.15   06 May 1998 12:10:20   allan  
 */
 
/*--------------------------------------------------------------------------,
|                                                                           |
|          Copyright 1996-1997 by Microware Systems Corporation             |
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
|                                                                           |
| definitions for SCSI                                                      |
|                                                                           |
|	Edition History:														|
|	#   Date    Comments												By  |
|	-- -------- ------------------------------------------------------- --- |
|   01 96/01/04 Created from "scsiman.h"								GbG |
|              ---- OS9000/PPC V2.0 ----                                    |
|   02 96/02/08 Added new defs Teac SCSI floppy							GbG |
|              ---- OS-9/68K V3.0.3 Released ----                           |
|              ---- OS-9000/x86 V2.1 Released ----                          |
|              ---- OS-9000/PPC V2.1.1 Released ----                        |
|   03 96/11/10 Removed rbf.h include added SCSIOPTS.                   GbG |
|              ---- OS-9000/ARMv3 V2.2 Released ----                        |
|   04 97/04/21 Added additional SCSIOPTS flags ULTRA and SLOW.         GbG |
|              ---- OS-9000/PPC V2.1.2 Released ----                        |
|              ---- OS-9000/ARMv3 V2.2.1 Released ----                      |
|              ---- OS-9000/SH3 V2.2.6 Released ----                        |
|              ---- SCSI Common Sub-Component v1.0 Released ----            |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#include 	<types.h>
#include 	<const.h>
#include    <errno.h>
#include    <scsicmds.h>
#include    <sg_codes.h>

/*
 * scsi command packet for low-level module
 */

/* WARNING: if the following structure ever changes size,
            scsiman_svcs compatibility is broken!! */

/*
 * scsi command block definitions.
 * this block is the common communication block used to
 * communicate between the high-level driver and the
 * low-level module.
 */ 

union scsipkt {
      struct std_str {
        u_char      sp_opcode;          /* the opcode */
        u_char		sp_lun;				/* the logical unit number & block address (msbs) */
        char        sp_lba[2];          /* (the rest of) the block address */
        char        sp_blkcnt;          /* the block count */
        u_char      sp_options;         /* command options */
    } std;
      struct ext_str {
        u_char      sp_opcode;          /* the opcode */
        u_char	   	sp_lun;				/* the logical unit number */
        u_char      sp_lba[4];             /* logical block address */
        u_char      sp_zero2;           /* zero for wren III */
        u_char      sp_count[2];        /* transfer count */
        u_char      sp_options;         /* command options */
    } ext;
      struct sq_str {
        u_char      sp_opcode;          /* the opcode */
        u_char		sp_lun;				/* the logical unit number & action */
        char        sp_blkcnt[3];       /* the block address */
        u_char      sp_options;         /* command options */
    } sq;
};

typedef union scsipkt *Scsipkt;         /* pointer to a packet */


/*
 * extended error details structure
 */
        
/* WARNING: if the following structure ever changes size,
            scsiman_svcs compatibility is broken!! */
                   
typedef struct errdet_str {
    u_char	        ed_errcode,    		/* Valid (bit7) | error is valid (70 or 71) */
    		        ed_seg,	            /* segment number always 0 */
					ed_main,
    		        ed_info[4],         /* info byte [lba] */
                    ed_senslen,         /* additional sense info length */
    	           	ed_zero3[4],        /* always zero */
    				ed_code,            /* error code */
                    ed_zero4[3];        /* always 0 */
} errdet;

typedef errdet *Errdet;         /* pointer to error detail packet */

/*
 * inquiry data structure
 */
        
/* WARNING: if the following structure ever changes size,
            scsiman_svcs compatibility is broken!! */
                   
typedef struct enqdet_str {
    u_char          eq_type,            /* device type */
					eq_rmbtype,			/* RMB device type qualifier */
					eq_iso,				/* ISO/ECMA/ANSI Version */
                    eq_datfmt;   	    /* response data format */					
	char	        eq_addlen,          /* additional length */
    				eq_rsvd[3],			/* reserved field */
                    eq_vendid[8],       /* vendor id */
                    eq_prodid[16],      /* product id */
                    eq_revlev[4];       /* revision level */
} enqdet;

typedef enqdet *Enqdet;         /* pointer to enquiry data packet */

/*
 *  drive capacity structure
*/
        
/* WARNING: if the following structure ever changes size,
            scsiman_svcs compatibility is broken!! */
                   
typedef struct dsize_str {
	u_int32		ds_maxlsn;				/* highest lsn on device */
	u_int32		ds_blksize;				/* physical block size */
} dsize;

typedef dsize *Dsize;

/* TEAC FLOPPY FC-xx SPECIFIC */

/*
 * mode select data structure
*/

typedef struct teac_fc_modesel_str {
    u_char          ms_res1;            /* reserved: (0) */
    u_char          ms_medtype;         /* medium type code */
    u_char          ms_res2;            /* reserved: (0) */
    u_char          ms_bdlen;           /* block descriptor length: (0) */
    u_char			ms_pgcode;			/* page code */
    u_char			ms_pglen;			/* page length */
    u_char			ms_trate[2];		/* transfer rate */
    u_char			ms_heads;			/* number of heads */
    u_char			ms_sectrk;			/* sectors per track */
    u_char			ms_secsize[2];		/* sector size */
    u_char			ms_cyls[2];			/* number of cylinders */
    u_char			ms_precomp[2];		/* starting precomp cyl */
    u_char			ms_redwrite[2];		/* starting reduced write cyl */
    u_char			ms_step[2];			/* drive step rate */
    u_char			ms_pulse;			/* step pulse width (0) */
    u_char			ms_headdly[2];		/* head settle delay */
    u_char			ms_motoron;			/* motor on delay */
    u_char			ms_motoroff;		/* motor off delay */
    u_char			ms_tsm;				/* true ready bit | start sector | motor on */
    u_char			ms_pulcyl;			/* step pulses per cly */
    u_char			ms_precomplvl;		/* precomp level */
    u_char			ms_loaddelay;		/* head load delay (0) */
    u_char			ms_unloaddelay;		/* head unload delay (0) */
    u_char			ms_pin34;			/* pin 34 & pin 2 definition */
    u_char			ms_pin4;			/* pin 4 & pin 1 definition */
    u_char			ms_res5[4];			/* reserved */
} teac_fc_modesel;

typedef teac_fc_modesel *teac_fc_Modesel;

/* SCSI DA SPECIFIC */

/*
 * mode select/sense data structure
*/
 
typedef struct da_modesel_str {
    u_char          ms_res1;            /* reserved: (0) */
    u_char          ms_medtype;         /* medium type: (0) */
    u_char          ms_res2;            /* reserved: (0) */
    u_char          ms_bdlen;           /* block descriptor length: (8) */
    u_char          ms_denscode;        /* density code: (0) */
    u_char          ms_numblks[3];      /* number of blocks: (0) */
    u_char          ms_res3;            /* reserved: (0) */
    u_char          ms_blklen[3];       /* block length */
    u_char			ms_pgcode;			/* page code */
    u_char			ms_pglen;			/* page length */
    u_char			ms_trate[2];		/* transfer rate */
    u_char			ms_heads;			/* number of heads */
    u_char			ms_sectrk;			/* sectors per track */
    u_char			ms_secsize[2];		/* sector size */
    u_char			ms_cyls[2];			/* number of cylinders */
    u_char			ms_precomp[2];		/* starting precomp cyl */
    u_char			ms_redwrite[2];		/* starting reduced write cyl */
    u_char			ms_step[2];			/* drive step rate */
    u_char			ms_pulse;			/* step pulse width (0) */
    u_char			ms_headdly[2];		/* head settle delay */
    u_char			ms_motoron;			/* motor on delay */
    u_char			ms_motoroff;		/* motor off delay */
    u_char			ms_tsm;				/* true ready bit | start sector | motor on */
    u_char			ms_pulcyl;			/* step pulses per cly */
    u_char			ms_precomplvl;		/* precomp level */
    u_char			ms_loaddelay;		/* head load delay (0) */
    u_char			ms_unloaddelay;		/* head unload delay (0) */
    u_char			ms_res5[6];			/* reserved */
} da_modesel, *Da_modesel;

typedef da_modesel *da_Modesel;

/* SCSI SQ SPECIFIC */

/*
 * mode select/sense data structure
*/

typedef struct sq_modesel_str {
	u_char			ms_res1;			/* reserved: (0) */
	u_char			ms_res2;			/* reserved: (0) */
	u_char			ms_bmode_speed;		/* buffer mode | speed */
	u_char			ms_bdlen;			/* block descriptor length: (8) */
	u_char			ms_denscode;		/* density code */
	u_char			ms_numblks[3];		/* number of blocks: (0) */
	u_char			ms_res4;			/* reserved: (0) */
	u_char			ms_blklen[3];		/* block length */
} sq_modesel, *Sq_modesel;

typedef sq_modesel *sq_Modesel;


/*
 * mode select/sense options
 */
#define		MS_UNBUFF		0			/* ms_buffmode:  unbuffered */
#define		MS_BUFF			1			/* ms_buffmode:  buffered */
#define		MS_AUTODENS		0			/* ms_denscode:  default auto */
#define		MS_QIC24		5			/* ms_denscode:  QIC-24 */
#define		MS_QIC120		0x0f		/* ms_denscode:  QIC-120 */
#define		MS_QIC150		0x10		/* ms_denscode:  QIC-150 */

#define		BUFF_MODE		MS_BUFF		/* default buffer mode */

#define				MS_PIN1		0x00		/* pin 1 definition */
#define				MS_PIN2LD	0x05		/* pin 2 definition DD format */
#define				MS_PIN2HD	0x0D		/* pin 2 definition HD ED format */
#define				MS_PIN4		0x00		/* pin 4 definition */
#define				MS_PIN34	0x02		/* pin 34 definition */

#define	MS_PIN1_OPEN	0x00		/* pin 1 definition */
#define	MS_PIN1_1_6M	0x03		/* pin 1 definition */
#define	MS_PIN1_N_1_6M	0x0B		/* pin 1 definition */

#define	MS_PIN4_LS		0x07		/* pin 4 definition 300 rpm */
#define	MS_PIN4_HS		0x0F		/* pin 4 definition 360 rpm */
#define	MS_PIN34_OMTI	0x01		/* pin 34 definition for OMTI */ 

/* Media type codes */
#define				MS_LD		0x16	/* ds/dd (uv380  pcd0) */
#define				MS_HD		0x1A    /* ds/hd disks (AT format) */
#define				MS_HD_ER	0x88    /* ds/hd disks (AT format) [Japanese std.]*/
#define				MS_ED		0x8c	/* ds/ed 3.2 mb format */

/* flags for determining FC-1/Drive styles */
/* FC-1 controller versions:  FC-1-0X (old) or FC-1-1X (new) */

#define				VERS_OLD	1		/* original controller */
#define				VERS_NEW	2		/* updated [sic] controller */

/* Firmware controller origin: FC-1-X0 (World) or FC-1-X1 (USA) */

#define 			FIRM_USA	1		/* USA origin firmware */
#define 			FIRM_WWD	2		/* World (non-USA) origin firmware */

/* Drive type (one per LUN) */

#define				KIND_JS		1		/* Teac 235JS drive [JHF] */
#define				KIND_HS		2		/* Teac 235HS drive [HF] */
#define				KIND_GS		3		/* Teac 55GS drive [GF] */

/* these are the values for page 3 rotation rates */

#define				DR250		0xFA	/* 250K b/s */
#define				DR500  		0x1F4	/* 500K b/s */
#define				DR1000 		0x3E8	/* 1000K b/s */

#define				DISK_ROTATION_RATE  0x0F  /* mask for PD_Rate - yields rotation */
#define				DATA_TRANSFER_RATE  0xF0  /* mask for PD_Rate - yields data rate */

/* END OF TEAC FLOPPY FC-xx SPECIFIC */

#ifndef				OPTS
#define				OPTS		0x00
#endif

#define				FLG_NOATN   0		/* do not assert ATN */
#define				FLG_ATN     1		/* assert ATN with command */
#define				DEFAULT     -1		/* perform select & transfer (default) */
#define				TAPE_EOF	(1<<0)	/* end of file pending */
#define				FBS			1		/* sp_action: fixed block size */
#define				IMMED		1		/* sp_action: immediate status */
#define				SPACE_BLKS	0		/* sp_action: space blocks */
#define				SPACE_FM	1		/* sp_action: space file marks */
#define				SPACE_SFM	2		/* sp_action: space sequential FM */
#define				SPACE_EOD	3		/* sp_action: space to EOD */
#define				LOAD		1		/* sp_blkcnt: load ON flag */
#define				RETEN		2		/* sp_blkcnt: retension ON flag */

/*
 * general definitions
 */

#ifdef		NULL
#undef		NULL
#endif

#define     NULL        0       /* pointer to nowhere */

#ifndef		TRUE
#define     TRUE        1       /* for boolean use */
#endif

#ifndef		FALSE
#define     FALSE       0
#endif

/*
 * scsi direction flags
*/

#define		INPUT		0			/* SCSI cmd direction (input) */
#define		OUTPUT		1			/* SCSI cmd direction (output) */

/*
 * scsi command packet types
 */

#define     CDB_STD         6       /* std cdb size */
#define     CDB_EXT         10      /* extended cdb size */

/*
 * definitions of scsi sense keys
 * These are the sense values given in byte 2 (bits 3-0)
 * of the extended sense data.
 */
#define     MAIN_NOSENSE    0x00        /* no sense */
#define     MAIN_RECOVERED  0x01        /* recovered error */
#define     MAIN_NOTRDY     0x02        /* not ready */
#define     MAIN_MEDIUM     0x03        /* medium error */
#define     MAIN_HARDWARE   0x04        /* hardware error */
#define     MAIN_ILGL       0x05        /* illegal request */
#define     MAIN_UNITATN    0x06        /* unit attention */
#define		MAIN_WRITEPROT	0x07		/* write protect error */
#define		MAIN_BLANK		0x08		/* no data recorded */
#define     MAIN_ABORT      0x0b        /* aborted command */
#define		MAIN_OVRFL		0x0d		/* volume overflow */
#define     MAIN_DATACMP    0x0e        /* data miscompare */

/*
 * definitions of scsi extended sense error codes
 * These are the sense values given in byte 12
 * of the extended sense data.
 */
#define     SENSE_RECOVER   0x00        /* no error or no sense info */
#define     SENSE_NOINDEX   0x01        /* no index from drive */
#define		SENSE_NOSEEK	0x02		/* no seek complete */
#define		SENSE_WRITEFLT	0x03		/* write fault received from drive */
#define     SENSE_NOTRDY    0x04        /* drive not ready */
#define		SENSE_UNITATN	0x06		/* tape sense unit attention */
#define		SENSE_NOTRK0	0x06		/* no track zero found */
#define		SENSE_LUNFAIL	0x08		/* LUN communication failure */
#define		SENSE_TRKFOL	0x09		/* track following error */
#define     SENSE_ID        0x10        /* ID CRC or ECC error */
#define     SENSE_DATAERR   0x11        /* uncorrectable data error */
#define     SENSE_NOADDRID  0x12        /* no address mark in ID field */
#define     SENSE_NOADDR    0x13        /* no address mark in data field */
#define     SENSE_NOREC     0x14        /* no record found */
#define     SENSE_SEEK      0x15        /* seek error */
#define     SENSE_RETRYOK   0x17        /* recovered data by retries */
#define		SENSE_ECCOK		0x18		/* recovered data by ecc correction */
#define		SENSE_DLERR		0x19		/* defect list error */
#define		SENSE_DLMISS	0x1c		/* primary defect list missing */
#define		SENSE_CMPERR	0x1d		/* compare error */
#define     SENSE_ILLREQ    0x20        /* invalid command code */
#define     SENSE_ILLLBA    0x21        /* illegal logical block addr */
#define		SENSE_ILLFUNC	0x22		/* illegal function for drive type */
#define     SENSE_ILLCDB    0x24        /* illegal field in CDB */
#define     SENSE_ILLLUN    0x25        /* invalid LUN */
#define     SENSE_ILLFLD    0x26        /* invalid field in param list */
#define		SENSE_WRTPROT	0x27		/* media is write protected */
#define		SENSE_MEDIACHG	0x28		/* media changed */
#define     SENSE_RESET     0x29        /* power-on or reset or bus-reset */
#define     SENSE_MODE      0x2a        /* mode select parameters changed */
#define		SENSE_FDC		0x2b		/* FDC error */
#define		SENSE_INCOMPAT	0x30		/* incompatible cartridge */
#define     SENSE_MEDIUM    0x31        /* medium format corrupted */
#define		SENSE_NOSPARE	0x32		/* no defect spare available */
#define		SENSE_NOMEDUIM	0x3a		/* no medium present */
#define		SENSE_DIAGRAM	0x40		/* ram failure */
#define     SENSE_DIAGDATA  0x41        /* data path diagnostic failure */
#define		SENSE_DIAGPON	0x42		/* power-on diagnostic failure */
#define		SENSE_REJECT	0x43		/* message reject error */
#define		SENSE_CONTR		0x44		/* internal controller error */
#define		SENSE_RESEL		0x45		/* select/reselect failed */
#define     SENSE_SCSIPAR   0x47        /* scsi interface parity error */
#define     SENSE_INITIATR  0x48        /* initiator detected error */
#define		SENSE_INAPPROP	0x49		/* inappropriate/illegal message */
	/* these are returned as error <main sense>:<extended sense> */
#define     SENSE_SELFTEST  0xa0        /* self test error occured */
#define     SENSE_NOTERMPW  0xa1        /* no terminator power */
#define     SENSE_SWSTRAP   0xa2        /* sw strap error */

/*
 * SCSI DEBUG FLAGS 
 *
*/

#define		SCSI_DEBUG_CMD		1
#define		SCSI_DEBUG_DATIN	2
#define		SCSI_DEBUG_DATOUT	4
#define		SCSI_DEBUG_MSGIN	8
#define		SCSI_DEBUG_MSGOUT	0x10
#define		SCSI_DEBUG_STATUS	0x20
#define		SCSI_DEBUG_INFO		0x40

#define		NOTRDY_TO	20			/* "not-ready" time-out value (initdrv) */

/*
 * SCSI OPTIONS FIELDS for OS9 and OS9000
*/

#if !defined(_SCSI_OPTS)

#ifdef _OS9000

#define SCSI_ATN     0x01    /* attention supported -- drive supports
                              * SCSI attention and standard messageing.
                              * (required for disconnect support.)
                              */

#define SCSI_PARITY  0x02	/* enable parity checking on data transfers.
                             * Drivers should set the up controllers to
                             * generate PARITY.  This flag indicates that
                             * parity should be checked on incomming data.
                             */

#define SCSI_SYNC    0x04   /* drive supports synchronous data transfer
                             * This would cause the driver to negotiate
                             * for synchronous transfers.
                             */

#define SCSI_WIDE    0x08   /* drive may support SCSI-II "wide" bus
                             * (16 bit transfers.)  This would cause
                             * the driver to negotiate for wide bus
                             * transfers. ( implies ATN is also set )
                             */

#define SCSI_TARGET  0x10   /* This unit should function as a target
                             * device on the SCSI bus.  We currently
                             * have no drivers that can do this but...
                             */

#define SCSI_ULTRA	0x20	/* drive supports SCSI FAST20/40 protocol
							 * This includes ULTRA1 and ULTRA2
							 * support
							*/
							
#define SCSI_SLOW	0x40	/* Setting this bit forces maximun period
							 * during synchronous transfers to 200ns
							 * (5MBs). NOTE: SCSI_SYNC must also be
							 * defined to use this flag.
							*/


#else 
/* OS-9'S DEFS ARE DIFFERENT !!!!!!!! */

#define     SCSI_ATN        (1<<0)  /* assert ATN supported             */
#define     SCSI_TARGET     (1<<1)  /* target mode supported            */
#define     SCSI_SYNCHR     (1<<2)  /* synchronous transfers supported  */
#define     SCSI_PARITY     (1<<3)  /* enable SCSI parity checking      */
#define     SCSI_WIDE       (1<<4)  /* enable SCSI wide operation       */
#define		SCSI_ULTRA		(1<<5)	/* enable SCSI ultra support        */
#define		SCSI_SLOW		(1<<6)	/* force max synchronous to 5MB     */

#endif /* _OS9000 */

#define     _SCSI_OPTS

#endif /* _SCSI_OPTS */

