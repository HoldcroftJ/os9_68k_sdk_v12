/*
	$Header:   /h0/MWOS/OS9/SRC/IO/BETA_PCF/DEFS/VCS/format.h_v   1.6   24 Jul 1998 12:13:18   dibble  $
	$Log:   /h0/MWOS/OS9/SRC/IO/BETA_PCF/DEFS/VCS/format.h_v  $
 *     Rev 1.7  08 Aug 2000 GbG * Added NO_REGS_DEFINED option.
 *  
 *    Rev 1.6   24 Jul 1998 12:13:18   dibble * Fix errors resulting from 0-length file bug fix. * Sometimes PCF could not find FDs for 0-lenght files, * and locking a zero-length file locked the root directory. *  *  * 
 *    Rev 1.5   28 Dec 1997 19:46:30   dibble
 * 
 *    Rev 1.0   28 Dec 1997 19:43:52   dibble
 * Initial revision.
 * 
 *    Rev 1.4   03 Oct 1991  9:28:58   peter
 * Changes to fix E_FULL bugs
 * 
 *    Rev 1.3   16 Jul 1991 18:58:06   peter
 * Make alignment byte between DD_FirstFAT and V_TRAK explicit
 * and remove reference to old V_MapMax field.
 * 
 *    Rev 1.2   22 May 1991 16:16:56   peter
 * Add a non-typedef'ed structure type to the
 * DriveTable entry definition.
 * 
 *    Rev 1.1   13 Mar 1991 14:34:08   peter
 * Reorganize sector 0 part of drive table structure to better
 * match the rbf structure.  This will require changes to
 * drivetable.c where the fields are initialized.
 * (their alignment is not so good now.)
 * 
 *    Rev 1.0   13 Mar 1991 14:08:14   peter
 * Initial revision.
*/
#define BOOTSECTOR 0
#define FATSTART 1
#define FILERASED (unsigned char)'\345'
#define	DEFAULT_DRIVES	2
#define DIR_ENT_PER_SECTOR(SS)	(SS/32)


/* 
	User and group that will own PCF files for FD-reporting purposes.
   	All accesses will be treated as super user, but the FD owner
	will prevent super-user modules from being loaded from PCF disks.
*/
#define FILE_GROUP	1
#define FILE_USER	1

#define FAT_EMPTYFILE	0x0000	/* Per Microsoft usage */
#define FAT_ROOTDIR		0x0000	/* Magic number used internally. */
								/* Root directory doesn't really appear in the FAT */
								/* If a file has the dir attr and 0 start sector */
								/* It's the root sector. */
								/* If it has a 0 start sector and no dir */
								/* Attr, it's an empty file */
#define FAT_BADTRACK	0x0fff7
#define FAT_BADFAT	0x0fff8 /* We really shouldn't be using this like this */
#define FAT_FIRST_EOF	0x0fff9 /* and this is why.  Theoretically fff8 is the */
							   /* first EOF flag, but in practice only fffF */
							   /* is used. */
#define FAT_IN_EOF_RANGE(x)	((x) >= FAT_FIRST_EOF && (x) <= FAT_LASTSECTOR)
#define FAT_LASTSECTOR	0x0fffF
#define FAT_LASTSECTOR9	0x0fff9
#define FAT_LASTSECTORA	0x0fffA
#define FAT_LASTSECTORB	0x0fffB
#define FAT_LASTSECTORC	0x0fffC
#define FAT_LASTSECTORD	0x0fffD
#define FAT_LASTSECTORE	0x0fffE

#define T_FAT_BADTRACK	0x0ffffff7
#define T_FAT_BADFAT	0x0ffffff8 /* We really shouldn't be using this like this */
#define T_FAT_FIRST_EOF	0x0ffffff9 
#define T_FAT_LASTSECTOR9	0x0ffffff9
#define T_FAT_LASTSECTORA	0x0ffffffA
#define T_FAT_LASTSECTORB	0x0ffffffB
#define T_FAT_LASTSECTORC	0x0ffffffC
#define T_FAT_LASTSECTORD	0x0ffffffD
#define T_FAT_LASTSECTORE	0x0ffffffE
#define T_FAT_LASTSECTOR 	0x0ffffffF
#define MAGIC_NUMBER	0x0C0DE
#define V_INIT_VALUE	0x02
#define DIR_ADDR_TERMINATOR	FAT_BADTRACK


typedef struct {
	unsigned char	ActiveFlag;
	unsigned char	StartHead;
	unsigned char	StartCylSect[2];
	unsigned char	Type; /* 0: not used, 1: 12-bit FAT, 4: 16-bit FAT,
					 5: extended partition, 6: huge partition
				  */
	unsigned char	EndHead;
	unsigned char	EndCylSect[2];
	unsigned char	StartSect[4];
	unsigned char 	PartLength[4]; /* Length of partition in sectors */
} PartDescType;
	

typedef struct {
	unsigned char 	Reserved1[3]; 		/* a branch instruction */
	char 	SystemID[8];
	unsigned char 	SectorSize[2];		/* Bytes per sector */
	unsigned char 	SectorsPerCluster;
	unsigned char 	ReservedSectors[2]; 	/* Number of res. sectors at start */
	unsigned char 	FATCopies;
	unsigned char 	RootDirSize[2];		/* Number of entries in root directory */
	unsigned char 	TotSectors[2];		/* Sectors on the disk */
	unsigned char 	FormatID;				/* F8..FF */
	unsigned char 	SectorsPerFAT[2];
	unsigned char 	SectorsPerTrack[2];
	unsigned char 	Sides[2];
	unsigned char 	S_ReservedSectors[2];	/* special reserved sectors */
	
	/* DOS 4.0-only fields */
	unsigned char	More_S_ReservedSectors[2]; /* special reserved sectors for V4.0 */
									   /* These are added to S_ReservedSectors */
									   /* to give a 4-byte field */
	unsigned char	XTotSectors[4];			   /* TotSectors when the disk is > 32 MB */
	unsigned char	PDriveNum;				   /* Physical drive number (DOS 4.0) */
	unsigned char 	Res40;					   /* A reserved field for DOS 4.0) */
	unsigned char	XBSig;					   /* Extended boot signature $29 */
	unsigned char	VolID[4];				   /* Binary volume ID */
	unsigned char	VolLabel[11];			   /* Volume label */
	unsigned char	MoreRes40[8];				
	
	unsigned char	BootCode[384];			/* reserved stuff */
	PartDescType PartDesc[4];
	unsigned char	Signature[2]; /* $AA55 for recent versions of DOS */
} *BootSectorType;

typedef struct {
	unsigned char 	FileName[8];
	unsigned char 	FileExtension[3];
	unsigned char 	FileAttr;		
	char 	Reserved[10];
	unsigned char 	Time[2];		
	unsigned char 	Date[2];		
	unsigned char 	StartCluster[2];
	unsigned char 	FileSize[4];
} *MSDirE, MSDirEntry;

/* File attributes */
#define MODIFIED		0x20
#define SUB_DIRECTORY	0x10
#define VOL_LABEL		0x08
#define SYSTEM_FILE		0x04
#define HIDDEN			0x02
#define READ_ONLY		0x01
#define LONG_FILENAME_ATTR 0x0f

typedef unsigned char SmallFAT_Entrys[3];

#if !defined(NO_REGS_DEFINED)

typedef struct {
	long 	R_d0, R_d1, R_d2, R_d3, R_d4, R_d5, R_d6, R_d7;
	char 	*R_a0, *R_a1, *R_a2, *R_a3, *R_a4, *R_a5, *R_a6, *R_a7;
	unsigned char 	R_ssr;		 	/* status register -- system part         	*/
	unsigned char 	R_cc; 			/* status register -- condition code part 	*/
	short 	*R_pc; 			/* program counter register               	*/
	short 	R_fmt;			/* 68010 exception format and vector      	*/
} *REGS, REGISTERS;

#endif

typedef struct DriveTable_Type {
	unsigned char	DD_TOT[3];		/* Total number of sectors on device 		*/
	unsigned char	DD_TKS;			/* Track size in sectors 					*/
	unsigned short	DD_FATSIZ;		/* Number of sectors in FAT 				*/
	unsigned short	DD_SPC;			/* Number of sectors per cluster 			*/
	unsigned short	DD_DIR;			/* Address of root directory  				*/
							/* The address is actually an lsn: 24 bits	*/
							/* but since this lsn is small, 16 bits suffice */
	unsigned char 	Reserved;
	unsigned char	DD_OWN[2];			/* Owner ID (meaningless) 					*/
	unsigned char	DD_ATT;			/* Attributes								*/
	unsigned short	DD_DSK;			/* Disk ID (probably meaningless	 		*/
	unsigned char	DD_FMT;			/* Disk format; density/sides 				*/
	unsigned char	DD_SPT[2];		/* Sectors per track 						*/
	unsigned char	DD_FATCnt;		/* Copies of FAT 							*/
	unsigned char	DD_FirstFAT; 	/* First FAT Sector 						*/
	unsigned char	DD_Alignment1;	/* Padding to aline next unsigned short             */
	unsigned short	V_TRAK;			/* Current track 							*/
	POINTER V_FileHd;		/* This seems to overlap with V_Paths       */
	unsigned short	V_DirEntries;	/* Number of directory entries in root dir	*/
	unsigned short	V_FATSz;		/* FAT size 								*/
	unsigned short	V_DataStart;	/* First cluster in the data space			*/
	unsigned short	V_FATLinks;		/* FAT use counter 							*/
	POINTER	V_ScZero;		/* Pointer to sector zero buffer 			*/
	boolean	V_ZeroRd;		/* Reserved for the driver					*/
	unsigned char	V_Init;			/* Drive initialized flag 					*/
	unsigned char	V_Flags;		
#define	V_FAT_DIRTY	0x01	/* FAT buffer has been changed 				*/
#define	V_16BIT		0x02	/* Disk uses 16-bit FAT entries				*/
			
	unsigned char	V_FMInit;		/* Drive initialized flag for pcfm			*/
	unsigned long	V_SoftEr;
	unsigned long	V_HardEr;
	POINTER V_Cache;		/* Reserved for the driver 					*/
	POINTER V_DTExt;		/* Driver's extension area					*/
	unsigned short	V_SectSize;		/* Sector Size								*/
	unsigned char  V_RateHint;	/* The last value PCF used for PD_Rate */
	unsigned char  V_Reserved1;
	POINTER V_FATPtr;		/* Pointer to the drive's FAT				*/
	POINTER V_DirMap;		/* Pointer to the drive's directory map */
	unsigned short	V_Reserved[6];
} DriveTableType, *DTBPtrType;

typedef struct {
	/* I/O Device Static storage required by the kernel for all
	device types. */	
	POINTER		V_PORT;		/* Device base port address 				*/		
	unsigned short		V_LPRC;		/* Last active process ID 					*/
	unsigned short		V_BUSY;		/* Current process ID (0=idle) 				*/
	unsigned short		V_WAKE;		/* Active process ID if driver must wakeup 	*/
	POINTER		V_Paths;	/* Linked list of open paths on device 		*/
	unsigned long		V_Reserved[8];
							/* Static storage for RBF drivers 			*/
	unsigned char		V_NDRV;		/* Number of drives 						*/
	unsigned char		V_DReserved[7];
	DriveTableType	V_DRIVES[DEFAULT_DRIVES]; /* this may be the wrong size,
												 but that's ok */
						    /* Followed by device driver static storage */
} *STATICSTORETYPE;

typedef struct {
	char 	*I_DevTbl;		/* Pointer to Default device table          */
	unsigned long	DirCluster;		/* First cluster for directory 				*/
	unsigned char	Flags;			/* Flags for the directory				*/
	unsigned char	Unused[3];
	unsigned long	Extra;			/* Unused space.							*/
} DefaultDescriptor;
	

