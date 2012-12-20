#if !defined(_RBF_H)
#define _RBF_H

/*
 * $Header:   /h0/OS9000/SRC/DEFS/VCS/rbf.h_v   1.1   22 Sep 1993 14:00:00   robb  $
 * $Revision:   1.1  $
 */

/*--------------------------------------------------------------------------,
!                                                                           !
!              Copyright 1989 by Microware Systems Corporation              !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!---------------------------------------------------------------------------!
! preliminary definitions for rbf file manager disk structure               !
!  Edition History:                                                         !
!   #   Date    Comments                                                By  !
!  -- -------- -------------------------------------------------------- --- !
! 	1 88/02/27  created for portable OS-9                              djl  !
! 	2 88/03/02  added path descriptor & static storage defs    		   djl  !
!   3 88/03/07  removed leading _ from typedefs                        djl  !
!   4 88/03/16  added pd_buf field to rbf path descriptor              djl  !
!   5 88/03/16  added v_free free list pointer and v_freegrp free list      !
!               group pointer to drive table definitions               djl  !
!   6 88/03/24  added extra fd buffer pointer for fd containing current     !
!               segment                                                djl  !
!   7 88/05/09  changed some path descriptor definitions for dealing        !
!               with fd's that are always in memory                    djl  !
!   8 88/05/25  added device descriptor structure definition           djl  !
!   9 88/06/09  added format definitions and allowed for device             !
!               specific information in device descriptors             djl  !
!  10 88/06/13  added rbf_desc_fields typedef                          djl  !
!  11 88/07/11  changed directory format                               djl  !
!  12 88/07/28  divided options into path and device options           djl  !
!  13 88/09/14  added info needed for locking resources                djl  !
!  14 88/09/21  added info for block buffer lists                      djl  !
!  15 88/11/04  added drive use count so that we know the total number      !
!               of paths open to a device                              djl  !
!  16 88/12/06  rearranged layout of this file and changed some struct-     !
!               ures to be similar to scf where possible               djl  !
!  17 89/03/19  added more fields to pd_ctrl for format                djl  !
!  18 89/03/23  more fields for pd_ctrl                                djl  !
!  19 89/04/21  added cache resource id into drive info structure      djl  !
!  20 89/05/02  changed max name length to 43 to allow for the null         !
!               terminator                                             djl  !
!  21 89/07/31  added field to fd for last backup date                 djl  !
!  22 89/08/02  added v_dopts pointer to default path opts             djl  !
!  23 89/08/25  added defs for switching media between big and little       !
!               endian machines                                        djl  !
!  24 89/10/17  three bytes of unused after little/big endian flag in       !
!               the idblock structure                                  djl  !
!  25 89/10/17  added FMT_THREEINCH, FMT_FIVEINCH and FMT_HIGHDENS     djl  !
!  26 89/10/18  added field to indicate the max transfer size of a          !
!               drive in terms of bytes                                djl  !
!  27 89/10/31  added STP_XXXX constant definitions for step rate      djl  !
!  28 90/06/18  expanded some byte and word fields to words or longs   djl  !
!  29 90/10/01  put lock structures into rbf data structures           djl  !
!  30 95/08/28  Added C++ support									   dwj	!
`--------------------------------------------------------------------------*/



#if !defined(_TYPES_H)
#include	<types.h>
#endif

#if !defined(_IO_H)
#include	<io.h>
#endif

#if !defined(_TIME_H)
#include <time.h>
#endif

/*
 * sync code for id block
 */
 
#define		IDB_SYNC	0xadb0b0ad		/* the sync field */

/*
 * big/little endian flags
 */
 
#define		BIG_END		0x42			/* big endian */
#define		LITTLE_END	0x4c			/* little endian */

/*
 * length that indicates entire file should be locked
 */
 
#define		LOCK_ENTIRE		-1l			/* lock the entire file */

/*
 * rbf path name equates 
 */
 
#define		MAXNAME		43				/* size of name */
#define		DIRENTSIZE	64				/* size of directory entry */

/*
 * buffer state flags
 */
 
#define		BK_BUFMOD		(1<<0)		/* buffer modified */
#define 	BK_BINBUF		(1<<1)		/* block in buffer */
#define 	BK_FDBUF		(1<<2)		/* file descriptor in buffer */
#define		BK_BUFBUSY		(1<<4)		/* buffer is busy; may not be swapped */
#define 	BK_INDRIVER		(1<<5)		/* in device driver */
#define		BK_IODIRECT		(1<<6)		/* I/O to caller's buffer; don't swap */
#define		BK_DELETED		(1<<7)		/* file is being deleted */

/*
 * mode state flags
 */
 
#define		MODE_BLKMODE	(1<<0)		/* block mode for @ */
#define		MODE_DIRIO		(1<<1)		/* i/o being done for directories */

/*
 * record locking definitions
 */
 
#define		B_rcdlock	(1<<0)		/* record from lolock to hilock is locked */
#define		B_filelock	(1<<1)		/* entire file is locked */
#define		B_eoflock	(1<<2)		/* end of file is locked */
#define		B_locksleep	(1<<7)		/* sleeping waiting for record */

/*
 * file descriptor sync code 
 */
 
#define		FD_SYNC		0xfdb0b0fd		/* low nibble is fmt rev */

/*
 *  definitions for fd type
 */
 
#define		FD_SMALLFILE		0x10000		/* small file (data fits in FD) */
#define		FD_DIRECTORY		0x8000		/* directory file */
#define		FD_EXCLUSIVE		0x4000		/* exclusize access flag */
#define		FD_SYMLINK			0x2000		/* symbolic link file */

/*
 * permission word definitions
 */
 
#define		PERM_WORLD_EXEC		0x0400		/* world execute permission */
#define		PERM_WORLD_SRCH		0x0400		/* world search permission */
#define		PERM_WORLD_WRITE	0x0200		/* world write permission */
#define		PERM_WORLD_READ		0x0100		/* world read permission */
#define		PERM_WORLD_MASK		0x0f00		/* world mask permission */
#define		PERM_GROUP_EXEC		0x0040		/* group execute permission */
#define		PERM_GROUP_SRCH		0x0040		/* group search permission */
#define		PERM_GROUP_WRITE	0x0020		/* group write permission */
#define		PERM_GROUP_READ		0x0010		/* group read permission */
#define		PERM_GROUP_MASK		0x00f0		/* group mask permission */
#define		PERM_OWNER_EXEC		0x0004		/* owner execute permission */
#define		PERM_OWNER_SRCH		0x0004		/* owner search permission */
#define		PERM_OWNER_WRITE	0x0002		/* owner write permission */
#define		PERM_OWNER_READ		0x0001		/* owner read permission */
#define		PERM_OWNER_MASK		0x000f		/* owner mask permission */

/*
 * RBF open access modes
 */
 
#define		O_RDONLY		0x0001		/* open for read access */
#define		O_WRONLY		0x0002		/* open for write access */
#define		O_RDWR			0x0003		/* open for update access */
#define		O_EXECDIR		0x0004		/* open for execute */
#define		O_TRUNC			0x0100		/* truncate file to 0 if exists */
#define		O_CONTIG		0x0200		/* ensure contiguous create */
#define		O_EXCL			0x0400		/* error if file exists on create */
#define		O_CREAT			0x0800		/* create file */
#define		O_APPEND		0x1000		/* force writes to eof (append) */
#define		O_ISIZE			0x2000		/* give initial size on create */
#define		O_SINGLE		0x4000		/* exclusive access */
#define		O_DIRECT		0x8000		/* directory access */
#define		O_BLKMODE		0x00010000	/* block oriented @ mode */

/*
 * device format word bit definitions
 */
 
#define		FMT_DBLTRK0		0x0001		/* track 0 is double density */
#define		FMT_DBLBITDNS	0x0002		/* device is double bit density */
#define		FMT_DBLTRKDNS	0x0004		/* device is double track density */
#define		FMT_DBLSIDE		0x0008		/* device is double sided */
#define		FMT_EIGHTINCH	0x0010		/* drive is eight inch */
#define		FMT_FIVEINCH	0x0020		/* drive is five inch */
#define		FMT_THREEINCH	0x0040		/* drive is three inch */
#define		FMT_HIGHDENS	0x1000		/* device is high density */
#define		FMT_STDFMT		0x2000		/* device is standard format */
#define		FMT_REMOVABLE	0x4000		/* media can be removed */
#define		FMT_HARDISK		0x8000		/* device is a hard disk */

/*
 * control word bit definitions
 */
 
#define		CTRL_FMTDIS		0x0001		/* device cannot be formatted */
#define		CTRL_MULTI		0x0002		/* device can transfer multi sectors */
#define		CTRL_AUTOSIZE	0x0004		/* device can find its size */
#define		CTRL_FMTENTIRE	0x0008		/* can format entire device */
#define		CTRL_TRKWRITE	0x0010		/* do track writes for format */

/*
 * step rate constant definitions
 */
 
#define		STEP_30MS		0x00		/* 30 ms */
#define		STEP_20MS		0x01		/* 20 ms */
#define		STEP_12MS		0x02		/* 12 ms */
#define		STEP_6MS		0x03		/* 6 ms */

/*
 * number of entry points into rbf file manager
 */
 
#define		RBF_ENTRY_POINTS	16		

/*
 * pointer to structure typedefs 
 */
 
typedef struct fd_stats			*Fd_stats;		/* file descriptor info */
typedef struct fd_segment		*Fd_segment;	/* segment in fd block */
typedef struct fd_links			*Fd_links;		/* link segment in fd block */
typedef struct fd_blkinfo		*Fd_blkinfo;	/* information about fd block */
typedef struct fdl_list			*Fdl_list;		/* fd block list element */
typedef struct idblock			*Idblock;		/* disk id block */
typedef struct dirent			*Dirent;		/* directory entry */
typedef struct blockbuf			*Blockbuf;		/* block buffer structure */
typedef struct rbf_fm_stat		*Rbf_fm_stat;	/* file manager static storage */
typedef struct rbf_drvr_stat	*Rbf_drvr_stat;	/* rbf driver static storage */
typedef struct rbf_lu_stat		*Rbf_lu_stat;	/* descriptor contents */
typedef struct rbf_lu_opts		*Rbf_lu_opts;	/* rbf logical unit options */
typedef struct rbf_drv_info		*Rbf_drv_info;	/* rbf disk static storage */
typedef struct rbf_path_opts	*Rbf_path_opts;	/* the path options */
typedef struct rbf_path_desc	*Rbf_path_desc;	/* the path descriptor */
typedef struct rbf_desc			*Rbf_desc;		/* device descriptor */

/*
 * File descriptor structure definition.  File descriptor
 * blocks for a particular file are organized in a doubly
 * linked list.  A file descriptor may contain file data 
 * if the file is sufficently small.  The back link of the
 * first file descriptor points to the last fd block.  The 
 * forward link of the last file descriptor is the first 
 * file descriptor.
 */
 
typedef struct fd_stats {
	u_int32 	fd_sync,					/* file descriptor sync field */
				fd_parity,					/* validation parity */
				fd_flag;					/* flag word */
	u_int16		fd_host,					/* file host owner */
				fd_group,					/* file group number */
				fd_owner,					/* file owner number */
				fd_links;					/* number of links to FD */
	u_int32		fd_size;					/* size of file in bytes */
	time_t		fd_ctime,					/* creation timestamp */
				fd_atime,					/* last access timestamp */
				fd_mtime,					/* last modified timestamp */
				fd_utime,					/* last changed timestamp */
				fd_btime;					/* last backed up timestamp */
	u_int16		fd_rev,						/* rbf revision that wrote this */
				fd_unused;					/* spare */
} fd_stats;

/*
 * structure for a segment in the FD block
 */
 
typedef struct fd_segment {
	u_int32		seg_offset,						/* segment block offset */
				seg_count;						/* segment block count */
} fd_segment;

/*
 * structure for the next and previous fd block pointers in
 * the last segment.
 */
 
typedef struct fd_links {
	u_int32		link_prev,			/* previous fd block */
				link_next;			/* next fd block */
} fd_links;

/*
 * structure used for segment list info.
 */
 
typedef struct fd_blkinfo {
	Fdl_list	fdl_prev,			/* previous link in segment list */
				fdl_next;			/* next link in segment list */
	u_int32		fdl_lbeg,			/* logical beginning */
				fdl_size,			/* total size of all segments in this blk */
				fdl_blknum;			/* block number for this block */
	Blockbuf	fdl_blklist;		/* list of buffered blocks */
	lk_desc		fdl_blk_rsrc_lk,	/* the block list's lock */
				fdl_rsrc_lk;		/* the fd list's lock */
} fd_blkinfo;

/*
 * structure for a segment list element 
 */
 
typedef struct fdl_list {
	fd_blkinfo	fdl_info;			/* the info about this fd block */
	fd_stats	fdl_blk;			/* the actual fd block */	
} fdl_list;

/*
 * Structure for RBF ID block.  This block is stored at 
 * various locations on the disk specified at format time.
 */
 
typedef struct idblock {
	u_int32		rid_sync,		/* ID block sync pattern */
				rid_diskid,		/* disk ID number (psuedo random) */
				rid_totblocks;	/* total blocks on media */
	u_int16		rid_cylinders,	/* number of cylinders */
				rid_cyl0size,	/* cylinder 0 size in blocks */
				rid_cylsize,	/* cylinder size in blocks */
				rid_heads,		/* number of surfaces on disk */
				rid_blocksize,	/* the size of a block in bytes */
				rid_format,		/* disk format */
				rid_flags,		/* various flags */
				rid_unused1;	/* 32 bit padding */
	u_int32		rid_bitmap,		/* block offset to bitmap FD */
				rid_firstboot,	/* block offset to low level booter FD */
				rid_bootfile,	/* block offset to bootfile FD */
				rid_rootdir;	/* block offset to root directory */
	u_int16		rid_group,		/* group owner of media */
				rid_owner;		/* owner of media */
	time_t		rid_ctime,		/* creation time of media */
				rid_mtime;		/* time of last write to ID block */
	char		rid_name[32],	/* volume name */
				rid_endflag,	/* big/little endian padding */
				rid_unused2[3];	/* long word padding */
	u_int32		rid_parity;		/* id block parity */
} idblock;

/*
 * directory entry structure 
 */
 
typedef struct dirent {
	char		dir_name[MAXNAME + 1],			/* name of file */
				dir_unused[DIRENTSIZE - MAXNAME - sizeof(u_int32) - 1];	
	u_int32		dir_fd_addr;					/* where file's fd is */
} dirent;

/*
 * buffered block structure
 */
 
typedef struct blockbuf {
	u_int32		bk_lsn,				/* file relative lsn */
				bk_users,			/* number of processes wanting this blk */
				bk_flags,			/* block state flags */
				bk_size;			/* size of the block */
	lk_desc		bk_rsrc_lk;			/* lock for this block */
	path_id		bk_lastpath;		/* last path using this block */
	Blockbuf	bk_next;			/* next block in the block list */
	u_char		*bk_buffer;			/* buffer for this block */
} blockbuf;

/*
 * definitions for rbf drive table
 */
 
typedef	struct rbf_drv_info {
	idblock			v_0;	/* standard id block stuff */
			/* --> note alignment here <-- */
	lk_desc			v_file_rsrc_lk;	/* the file list lock */
	Rbf_path_desc	v_filehd;		/* list of open files on drive */
	lk_desc			v_free_rsrc_lk;	/* free list lock */
	struct freeblk
					*v_free,		/* pointer to free list structure */
					*v_freesearch;	/* start search here for free space */
	u_int32			v_diskid;		/* disk ID number */
	fd_segment		v_mapseg;		/* the bitmap segment */
	Idblock			v_bkzero;		/* ptr to block zero buffer */
	u_int32			v_resbit,		/* reserved bitmap block number (if any) */
					v_trak,			/* current track number */
					v_softerr,		/* recoverable error count */
					v_harderr;		/* non-recoverable error count */
	struct cachedriv
					*v_cache;		/* drive cache information ptr */
	lk_desc			v_crsrc_lk;		/* cache lock */
	u_int16			v_numpaths;		/* number of open paths on this device */
	u_char			v_zerord,		/* block zero read flag */
					v_init;			/* drive initialized flag */
	Rbf_path_opts	v_dopts;		/* copy of the default opts */
	u_char			v_endflag,		/* big/little endian flag */
					v_dumm2[3];		/* reserved */
	lk_desc			v_fd_free_rsrc_lk;	/* lock for fd free list */
	Fdl_list		v_fd_free_list;	/* list of free fd blocks */
	lk_desc			v_blks_rsrc_lk;	/* free block buffer list lock */
	Blockbuf		v_blks_list;	/* free block buffer list */
	u_int32			v_reserved[4];	/* for future expansions */
} rbf_drv_info;

/*
 * file manager static storage definitions
 */
 
typedef struct rbf_fm_stat {
	u_int32	count;							/* number of entries in table */
	u_int32	(*func[RBF_ENTRY_POINTS])();	/* the dispatch table */
} rbf_fm_stat;

/*
 * device driver static storage definitions
 */
 
typedef struct rbf_drvr_stat {
	u_int32	funcs,					/* number of functions */
			(*v_init)(),			/* address of driver init routine */
			(*v_read)(),			/* address of driver read routine */
			(*v_write)(),			/* address of driver write routine */
			(*v_getstat)(),			/* address of driver getstat routine */
			(*v_setstat)(),			/* address of driver setstat routine */
			(*v_term)();			/* address of device terminate routine */
	lock_id	v_drvr_rsrc_id;			/* the driver resource id */
	process_id
			v_busy,					/* process using the device */
			v_wake;					/* for use by the driver */
} rbf_drvr_stat;

/*
 * rbf logical unit options
 */
 
typedef struct rbf_lu_opts {
	u_char		lu_stp,			/* the step rate */
				lu_tfm,			/* the dma transfer mode */
				lu_lun,			/* drive logical unit number */
				lu_ctrlrid;		/* the controller id */
	u_int32		lu_totcyls,		/* total number of cylinders */
				lu_reserved[4];	/* reserved for future expansion */
} rbf_lu_opts;

/*
 * rbf device descriptor logical unit static storage
 */
 
typedef struct rbf_lu_stat {
	rbf_drv_info	v_driveinfo;	/* the drive's information */
	u_char			v_vector,		/* the interrupt vector */
					v_irqlevel,		/* the interrupt level */
					v_priority,		/* the interrupt priority */
					v_unused;		/* unused byte */
	rbf_lu_opts		v_luopt;		/* logical unit options */
	u_int32			v_reserved[2];	/* reserved for future expansions */
#ifdef		DEV_SPECIFIC
	dev_specific	v_devspec;		/* device specific info structure */
#endif
} rbf_lu_stat;

/*
 * rbf path options section
 */
 
typedef struct rbf_path_opts {
	u_int32		pd_sid,			/* number of surfaces */
				pd_vfy,			/* 0=verify disk writes */
				pd_format,		/* device format */
				pd_cyl,			/* number of cylinders */
				pd_blk,			/* default blocks/track */
				pd_t0b,			/* default blocks/track for trk0/sec0 */
				pd_sas,			/* segment allocation size */
				pd_ilv,			/* block interleave offset */
				pd_toffs,		/* track base offset */
				pd_boffs,		/* block base offset */
				pd_trys,		/* # tries */
				pd_bsize,		/* size of block in bytes */
				pd_cntl,		/* control word */
				pd_wpc,			/* first write precomp cylinder */
				pd_rwr,			/* first reduced write current cylinder */
				pd_park,		/* park cylinder for hard disks */
				pd_lsnoffs,		/* lsn offset for partition */
				pd_xfersize,	/* max transfer size in terms of bytes */
				pd_reserved[4];	/* reserved for future expansions */
} rbf_path_opts;

/*
 * Structure for RBF path descriptor
 */
 
typedef struct rbf_path_desc {
	pd_com			pd_io;			/* ioman section */
	u_char			*pd_buf,		/* buffer pointer */
					pd_fsmf,		/* fd state flags */
					pd_lock,		/* path lockout status */
					pd_req,			/* requested lockout status */
					pd_bsbt,		/* disk block size (bit number) */
					pd_openok,		/* file was opened without error */
					pd_append,		/* file is being appended */
					pd_xfermode;	/* flags for disk mode */
	Blockbuf		pd_cblk;		/* current block */
	Fdl_list		pd_rfd,			/* root FD structure pointer */
					pd_cfd;			/* pointer to FD structure containing pd_seg */
	u_int32			pd_cp,			/* current logical byte position */
					pd_siz,			/* file size */
					pd_sbl,			/* segment beginning lsn */
					pd_sbp,			/* segment beginning psn */
					pd_ssz,			/* segment size */
					pd_dsk;			/* disk ID */
	lock_id			pd_conf_rsrc_id;	/* the conflict list resource id */
	Rbf_path_desc	pd_nxfil,		/* drive open file list ptr */
					pd_conf;		/* file conflict list ptr */
	Fd_segment		pd_seg;			/* the current segment */
	process_id		pd_owner;		/* process ID of owner of desired segment */
	u_int32			pd_accs,		/* allowable file access permissions */
					pd_lolck,		/* low locked logical address */
					pd_hilck;		/* high locked logical address */
	lock_id			pd_wait_rsrc_id;	/* wait list resource id */
	Rbf_path_desc	pd_wait;		/* ptr to next locked out pathdesc */
	u_int32			pd_tmout,		/* max ticks to wait for locked segment */
					pd_prior,		/* temp for proc priority while in driver */
					pd_fd,			/* lsn of file descriptor */
					pd_dfd,			/* directory file descriptor lsn */
					pd_dcp,			/* directory entry ptr */
					pd_dvt,			/* device table ptr (copy) */
					pd_att;			/* file attributes */
	char			pd_name[MAXNAME];	/* filename */
	u_int32			pd_reserved[4];	/* reserved for future expansions */
	rbf_path_opts	pd_opts;		/* the option section */
} rbf_path_desc;

/*
 * rbf device descriptor module structure definitions
 */
 
typedef struct rbf_desc {
	dd_com			dd_descom;		/* common header info */
	rbf_path_opts	dd_pathopt;		/* the path options */
} rbf_desc;

/*
 * cache information of general interest
 */
 
#ifndef		_cache	
typedef struct cachestats {
	u_int32		c_read_requests;
	u_int32		c_blocks_read;
	u_int32		c_read_hits;
	u_int32		c_read_zaps;
	u_int32		c_multi_sector_reads;
	u_int32		c_sector_zero_reads;
	u_int32		c_write_requests;
	u_int32		c_blocks_written;
	u_int32		c_write_zaps;
	u_int32		c_multi_sector_writes;
	u_int32		c_dir_reads;
	u_int32		c_dir_blocks_read;
	u_int32		c_dir_hits;
	u_int32		c_dir_writes;
	u_int32		c_dir_blocks_written;
	u_int32		c_woofs;
	u_int32		c_errors;
	u_int32		c_misses;
	u_int32		c_hit_compares;
	u_int32		c_miss_compares;
} cachestats, *Cachestats;
#endif		/* ifndef _cache */

/* OS-9 file descriptor for those who want to us _gs_gfd and/or _ss_pfd */
typedef struct fd {
	u_char	fd_att;				/* file attributes */
	union {
		struct {
			u_char	group;		/* file owner (group) */
			u_char	user;		/* file owner (user) */
		} owner;
		u_char own[2];
	} fd_own;
	u_char	fd_date[5];			/* data last modified */
	u_char	fd_link;			/* link count */
	u_int	fd_fsize;			/* file size */
	u_char	fd_dcr[3];			/* data created */
	struct {
			/* segment entry structure */
			u_char	addr[3];	/* segment begin LSN */
			u_char	size[2];	/* segment size in sectors */
	} fdseg[48];				/* 256 byte fd */
} *Fd;

#if defined(_ANSI_EXT) || defined(__STDC__)

/*	C++ support	*/
#ifdef __cplusplus
extern "C" {
#endif

error_code _os_gs_cstats(path_id, cachestats *);
error_code _os_gs_dsize(path_id, u_int32 *, u_int32 *);
error_code _os_gs_fd(path_id, u_int32, fd_stats *);
error_code _os_gs_fdaddr(path_id, u_int32 *);
error_code _os_gs_fdinf(path_id, u_int32, u_int32, fd_stats *);
error_code _os_gs_parity(path_id, fd_stats *, u_int32 *);
error_code _os_ss_cache(path_id, u_int32, u_int32);
error_code _os_ss_fd(path_id, fd_stats *);
error_code _os_ss_flushmap(path_id);
error_code _os_ss_hdlink(path_id, char **);
error_code _os_ss_lock(path_id, u_int32);
error_code _os_ss_ticks(path_id, u_int32);
error_code _os_ss_wtrack(path_id, void *, void *, u_int32, u_int32, u_int32);

/*	End C++	*/
#ifdef __cplusplus
}
#endif

#endif

#endif		/* ifdef rbf */
