/*
	$Header:   /h0/MWOS/OS9/SRC/IO/BETA_PCF/DEFS/VCS/pathdesc.h_v   1.7   24 Jul 1998 12:13:26   dibble  $
	$Log:   /h0/MWOS/OS9/SRC/IO/BETA_PCF/DEFS/VCS/pathdesc.h_v  $
 *  *    Rev 1.7   24 Jul 1998 12:13:26   dibble * Fix errors resulting from 0-length file bug fix. * Sometimes PCF could not find FDs for 0-lenght files, * and locking a zero-length file locked the root directory. *  *  * 
 *    Rev 1.6   28 Dec 1997 19:46:34   dibble
 * 
 *    Rev 1.0   28 Dec 1997 19:43:54   dibble
 * Initial revision.
 * 
 *    Rev 1.5   30 Jun 1992 11:40:10   peter
 * 
 *    Rev 1.4   11 Nov 1991 14:36:42   peter
 * Take DIRFIX option out.  I'm going to try making
 * Dos file name rules the default.
 * (that is, almost no restriction on file names except length
 * and placement of .)
 * 
 *    Rev 1.3   06 Nov 1991 15:32:30   peter
 * Add PD_FIXDIR macros to turn on/off translation of
 * illegal file name characters to legal file name characters.
 * 
 *    Rev 1.2   03 Oct 1991  9:29:18   peter
 * Make FD_CCluster and FD_..offset fields u_int16.
 * 
 *    Rev 1.1   29 Jul 1991 14:41:46   peter
 * Convert PD_NewLine into a general-purpose flag byte, and add macros
 * for the newline-processing option and the 16bit FAT option.
 * 
 *    Rev 1.0   13 Mar 1991 14:08:10   peter
 * Initial revision.
*/
#define PATHDELIM		'/'
#define ENTIRE_DELIM	'@'

#define ALWAYS_STAMP	/* Always update the FD's time stamp
			   Stamp it even when it isn't changed */
#define STRICT_DOS	/* Convert all file names to upper case */
#define STAMP_ON_OPEN


typedef struct {
	u_char 	vfd_att,
	 	vfd_own[2],
	 	vfd_date[5],
	 	vfd_link,
	 	vfd_fsize[4],
	 	vfd_dcr[3];
	u_int32	vfd_cluster;
} VirFD, *VirFDPtr;

typedef struct PDTYPE {
	u_int16			PD_PD; 			/* Path number 						*/
	u_char			PD_MOD; 		/* Mode (read/write/update)			*/
	u_char			PD_CNT;			/* number of open images			*/
	struct DEVTAB	*PD_DEV;		/* Device table entry address 		*/
	u_int16			PD_CPR;			/* Current process id 				*/
	POINTER			PD_RGS;			/* caller's register stack pointer 	*/
	char			*PD_BUF;		/* buffer address 					*/
	u_int32			PD_USER;		/* User ID of path's creator 		*/
	struct PDTYPE	*PD_Paths;		/* L-List of paths to this device	*/
	u_int16			PD_COUNT;		/* Actual number of open images 	*/
	u_int16			PD_LProc;		/* Last active process ID 			*/
	short			PD_Reserved[6];

	/*==================================================================*
	 *                 File manager storage                             *
	 *==================================================================*/

	u_char			PD_SMF;			/* State flags						*/
	u_char			PD_SMF2;		/* Additional SMF Flags				*/
	u_int16			PD_Parent;		/* FCluster of parent dir */
	u_int32			PD_CSector;		/* Number of sector in the buffer 	*/
	u_int32			PD_CP;			/* Current logical byte position 	*/
	u_int32			PD_SIZ;			/* File size 						*/
	u_int16			PD_CCluster;	/* Current cluster					*/
	MSDirE			PD_FDBUF;		/* Buffer for file descriptor info	*/
	u_int32			PD_FDBufSect;	/* Sector buffered in FDBUF */
						/* zero signifies no valid data */
	u_int16			PD_FDOffset;	/* Offset in sector for FD below    */
	u_int16			PD_ClaimCt;	/* Depth of nested CLAIM_DEVICEs   */
	DTBPtrType		PD_DTB;			/* Drive table pointer 				*/
	u_int32			PD_FDHash;		/* Combined sector/offset */
	VirFD			PD_FD;			/* First part of virtual OS-9 FD	*/
	u_char			PD_Accs;		/* Allowable file access permissions*/
	u_char			PD_XMode;		/* Extended Open mode */
	u_int16 		PD_ClusterOff;	/* Current cluster's offset in file */
	char			PD_Unused2[24];	 
	
	/*==================================================================*	
	 *             Path descriptor's options section                    *
	 *==================================================================*/
	 
	u_char			PD_DTP; 		/* Device type 						*/
	u_char			PD_DRV;			/* Drive number 					*/
	u_char			PD_STP;			/* Step rate 						*/
	u_char			PD_TYP;			/* Disk device type 				*/
	u_char			PD_DNS;			/* Density capability 				*/
	u_char			PD_Flags;		/* Flags for PCF                    */
	u_int16			PD_CYL;			/* Number of cylinders 				*/
	u_char			PD_SID;			/* Number of sides 					*/
	u_char			PD_VFY;			/* 0=verify disk writes 			*/
	u_int16			PD_SCT;			/* default sectors per track 		*/
	u_int16			PD_TOS;			/* ""           ""        (tr0, s0) */
	u_int16			PD_SAS;			/* Segment allocation size 			*/
	u_char			PD_ILV;			/* Sector interleave offset 		*/
	u_char			PD_TFM;			/* DMA transfer mode 				*/
	u_char			PD_TOffs;		/* Track base offset 				*/
	u_char			PD_SOffs;		/* Sector base offset 				*/
	u_int16			PD_SSize;		/* Size of sector in bytes 			*/
	u_int16			PD_Cntl;		/* Control word 					*/
	u_char			PD_Trys;		/* Number of tries (1=no error corr)*/
	u_char			PD_LUN;			/* SCSI unit number of drive 		*/
	u_int16			PD_WPC;			/* First cylinder using wrt precomp */
	u_int16			PD_RWC;			/* ""      "" reduced write current */
	u_int16			PD_Park;		/* Park cylinder for hard disks 	*/
	u_int32			PD_LSNOffs;		/* LSN offset for partition 		*/
	u_int16			PD_TotCyls;		/* Total  cylinders on device 		*/
	u_char			PD_CtrlrID;		/* SCSI controller ID 				*/
	u_char			PD_Rate;		/* data transfer & rotational speed */
	u_int32			PD_SCSIOpts;	/* SCSI options                     */
	u_int32			PD_MaxCount;	/* maximum byte count driver can handle */
	u_char			PD_reserved3[5];
	u_char			PD_ATT;			/* File attributes 					*/
	u_int16			PD_FCluster;	/* Starting cluster	(was PD_FD)		*/
	u_int16			PD_Padding;		/* just filler                      */
	u_int32			PD_DFD;			/* Directory FD psn 	*/
	u_int32			PD_DCP;			/* Directory entry pointer 			*/
	POINTER			PD_DVT;			/* Device table pointer (copy) 		*/
	u_char 			PD_reserved5[2];
	u_int32			PD_sctsiz;		/* Sector size */
	u_char			PD_reserved4[20];
	u_char			PD_Name[12]; 	/* Filename 						*/
	char			PD_NotName[20];	/* leftover space					*/
} *PD_TYPE;

/*
	Flags defined in PD_SMF
*/
#define PD_RAWMODE	0x01
#define PD_DIR_MODE	0x02			/* Any directory 					*/
#define PD_RDIR_MODE	0x04		/* The ROOT directory 				*/
#define PD_DIRTYFD	0x08			/* The FD copy in the PD  is dirty 	*/
#define PD_OS9FMTD	0x10			/* The data in PD_BUF is OS-9 FMT Dir records */
#define PD_PENDING_DRIVETABLE 0x20	/* The drivetable isn't set yet */
#define PD_DIRTYBUF 0x40			/* The data in PD_BUF has not been written to disk */
#define PD_EXCLUSIVE 0x80		/* This path has exclusive access to the file */
#define DIRTYFD(pd)	(pd)->PD_SMF|=PD_DIRTYFD
#define CLEANFD(pd)	(pd)->PD_SMF&=~PD_DIRTYFD

/*
	Flags defined in PD_SMF2
*/
#define PD_VALIDBUF	0x01	/*  PD_BUF contains valid data */
#define PD_MUSTCOMPLETE 0x02	/* I/O operations must not be aborted */

/*
	Flags defined in PD_Flags

   Since a drive can be divided into multiple partitions with
   different FAT types, we can't make the FAT type a drive characteristic.
   Probably the _right_ thing to do is figure a way to get IOMan to
   understand partitions, but that's not real likely.

*/

#define PD_WRITETHROUGH	0x80
#define PD_16BIT	0x40		/* This file is controlled by a 16-bit FAT */
#define PD_ENOUGHSTACK	0x20		/* fmmain does not need to switch stacks */
#define PD_PERMIT_GUESSING 0x10		/* PCF can guess drive characteristics */
#define PD_FMTNEWLINE	0x08

#define PD_NEWLINE(pd)	(((pd)->PD_Flags & PD_FMTNEWLINE) != 0)
#define PD_16BITFLAG(pd)	(((pd)->PD_Flags & PD_16BIT) != 0)
#define PD_WRITETHROUGHFLAG(pd) (((pd)->PD_Flags & PD_WRITETHROUGH) != 0)

/* This #define refers to the PD_TYP field */
#define TYPE_HARDDISK	0x080

/*
	Flags defined in PD_Cntl
*/
#define MULTISECTOR_IO(pd)	((pd->PD_Cntl & 0x02) != 0)

/*
	Flags defined in PD_XMode
*/
#define PD_XMODE_TEXT		1

typedef struct DEVTAB {
	POINTER V_DRIV;
	STATICSTORETYPE V_STAT;
	POINTER V_DESC;
	POINTER V_FMGR;
	short	V_USRS;
} *DEVTABTYPE;

/*
	Misc flags and magic numbers
*/
#define CCLUSTER_UNKNOWN	0
