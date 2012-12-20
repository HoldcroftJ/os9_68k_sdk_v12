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
!                                                                           !
!  Edition History:                                                         !
!   #   Date    Comments                                                By  !
!  -- -------- -------------------------------------------------------- --- !
!   1 88/02/23 Began modifications for Portable OS9                     DJN !
!   2 88/03/03 Added path delimiter '/' definition                      djl !
!   3 88/04/04 Added more path element definitions                      djl !
!   4 88/04/19 Added polling table definitions                          rg  !
!   5 90/01/30 Added DT_GFM                                             tah !
!   6 90/03/07 Added "pd_plist" pathlist variable to path descritpors.  afh !
!   7 93/01/04 Added DT_MPFM                                            rg  !
!   8 93/01/03 Added DT_RTNFM and DT_SPF defines.                       afh !
!   9 94/05/12 Added DT_INET define.                                    afh !
!  10 95/08/29 Changed DT_11 to DT_NRF                                  dwj !
!  11 96/07/18 added PD_SYSTATE                                         uday!
!  12 00/05/11 Added missing OS-9000 dev_list fields                    rds !
!              Added DT_MFM, DT_DVM, DT_NULL, DT_DVDFM, and DT_MODFM    rds !
!  13 00/11/03 Change DT_xx values to be 68k values instead of 9000's   mgh !
!  14 01/11/13 Added pd_callback, and related definitions and typedefs  rds !
`--------------------------------------------------------------------------*/
#if !defined(_IO_H)
#define _IO_H



#if !defined(_TYPES_H)
#include <types.h>
#endif
#if !defined(_MODULE_H)
#include <module.h>
#endif
#if !defined(_PROCESS_H)
#include <process.h>
#endif
#if !defined(_LOCK_H)
#include <lock.h>
#endif

/* file open mode and access permission values */
/*#define READ_	1
#define WRITE_	2
#define EXEC_	4
#define UPDAT_	(READ_|WRITE_)
#define PREAD_	8
#define PWRIT_	16
#define PEXEC_	32
#define PUPDAT_	(PREAD_|PWRIT_)
#define ISIZE_	32
#define SHARE_	64
#define DIR_	128
*/

/*
 * Device Type Conversion numbers.  Convert 9000 DT Names To 68k Values
 * Keep in sync with OS9/68k module.a and sg_codes.h (also 9000 io.h)
 * NOTE: Names for 9/9000 are the same though the values may differ
 * NOTE: Previous version of the file were incorrectly returning 9000 values.
 */
#ifndef DT_SCF
#define DT_SCF		 0	/* sequential character file type		*/
#define DT_RBF		 1	/* random block file type				*/
#define DT_PIPE		 2	/* pipe file type						*/
#define DT_SBF		 3	/* sequential block file type			*/
#define DT_NFM		 4	/* network file type					*/
#define DT_CDFM		 5	/* compact disc file type				*/
#define DT_UCM		 6	/* user communication manager			*/
#define DT_SOCK		 7	/* socket communication manager			*/
#define DT_PTTY		 8	/* pseudo-keyboard manager				*/
#define DT_INET 	 9	/* Inet file manager					*/
#define DT_NRF		10	/* Non-volatile ram file manager		*/
#define DT_GFM		11	/* graphics file manager				*/
#define DT_ISDN		12	/* ISDN file manager					*/
#define DT_MPFM		13	/* MPFM file manager					*/
#define DT_RTNFM 	14 	/* Real-Time Network file manager		*/
#define DT_SPF		15	/* Serial Protocol file manager			*/
#define DT_MFM		16	/* Multi-media file manager				*/
#define DT_DVM		17	/* Generic device file manager			*/
#define DT_DVDFM 	18	/* DVD file manager						*/
#define DT_MODFM 	19	/* Module directory file system manager	*/
#define DT_NULL		20	/* Null file manager					*/
#define	DT_PCF		21	/* Reserved FUTUR ... 68k PCF Returns DT_RBF */

/** DT-CODES UP TO 127 RESERVED FOR MICROWARE USE ONLY **/
#endif

/* path delimiter definitions */
#define	PATH_DELIM		'/'
#define	PATH_ENTIRE		'@'
#define	PATH_DIR		'.'

/* pd_state bit field definitions */
#define PD_SYSTATE 0x00000001	/* indicates I/O request from system state	*/
#define PD_CALLBACK 0x00000002	/* ioman has callback support				*/

/* ioman callback function codes */
#define IOCB_LOCK_RSRC		0x01	/* unlock path, lock resource, lock path */
#define IOCB_UNLOCK_RSRC	0x02	/* unlock the specified resource		*/
#define IOCB_LOCK_PATH		0x03	/* re-lock specified path				*/
#define IOCB_UNLOCK_PATH	0x04	/* unlock the specified path			*/
#define IOCB_FLUSH_LOCK		0x05	/* block until no waiters on lock		*/

/* ioman callback save area */
typedef struct iocb_save_area {
	u_int32		data[8];			/* reserved for ioman use				*/
} iocb_save_area, *Iocb_save_area;

/* I/O process descriptor pointer definition */
typedef struct io_proc *Io_proc;

/* Device descriptor data definitions */
/*
** !!!!  This structure is duplicated in:                  !!!!
** !!!!             /mwos/SRC/DPIO/LIB/DEFS/os9000_defs.h  !!!!
** !!!!  Any changes here should be mirrored there.        !!!!
*/
typedef struct {
	void
		*dd_port;		/* device port address */
	u_int16
		dd_lu_num,		/* logical unit number */
		dd_pd_size,		/* path descriptor size */
		dd_type,		/* device type */
		dd_mode;		/* device mode capabilities */
	u_int32
		dd_fmgr,		/* file manager name offset */
		dd_drvr;		/* device driver name offset */
	u_int16
		dd_class,		/* sequential or random */
		dd_dscres;		/* (reserved) */
} *Dd_com, dd_com;

/* Device list element definition */
/*
** !!!!  This structure is duplicated in:                  !!!!
** !!!!             /mwos/SRC/DPIO/LIB/DEFS/os9000_defs.h  !!!!
** !!!!  Any changes here should be mirrored there.        !!!!
*/
typedef struct devtab {
	struct devtab
		*v_nxt_dev;		/* pointer to next element in list */
	mh_fman
		*v_fm_mod;		/* pointer to file manager module */
	void
		*v_fm_stat;		/* pointer to file manager static storage */
	mh_drvr
		*v_dr_mod;		/* pointer to driver module */
	void
		*v_dr_stat;		/* pointer to driver static storage (jump table) */
	mh_dev
		*v_dd_mod;		/* pointer to device descriptor module */
	void
		*v_lu_stat,		/* pointer to logical unit static storage */
		*v_port;		/* port address */
	struct pathcom
		*v_paths;		/* Linked list of open paths on this device. */
	u_int16
		v_lu_num,		/* logical unit number */
		v_mode,			/* Device mode capabilities */
		v_pd_size,		/* Size of path descriptor */
		v_use_cnt;		/* number of paths open on this device */
	lock_id
		v_lid;			/* Queue id for device list entry. */
	process_id
		v_cproc;		/* current process */
	u_int32
		v_lu_size;		/* size of logical unit static storage */
	u_int16
		v_type,			/* device type */
		v_class,		/* device class */
		v_iuse_cnt,		/* count of intended users */
		v_res;			/* reserved word */
	u_int32
		v_fm_globs,		/* file manager total static globals pointer*/
		v_dr_globs,		/* driver globals pointer */
		v_rsrv[2];		/* reserved space */
} dev_list,*Dev_list;

/* Path descriptor common header definition */
/*
** !!!!  This structure is duplicated in:                  !!!!
** !!!!             /mwos/SRC/DPIO/LIB/DEFS/os9000_defs.h  !!!!
** !!!!  Any changes here should be mirrored there.        !!!!
*/
typedef struct pathcom pd_com, *Pd_com;
struct pathcom {
	path_id
		pd_id;			/* path number */
	Dev_list
		pd_dev;			/* device list element pointer */
	owner_id
		pd_own;			/* group/user number of path's creator */
	struct pathcom
		*pd_paths,		/* list of open paths on device */
		*pd_dpd;		/* default directory path */
	u_int16
		pd_mode,		/* mode (read/write/update) */
		pd_count,		/* actual number of open images */
		pd_type,		/* device type */
		pd_class;		/* device class (sequential or random) */
	process_id
		pd_cproc;		/* current process id */
	u_char
		*pd_plbuf,		/* pointer to partial pathlist */
		*pd_plist;		/* pointer to complete pathlist */
	u_int32
		pd_plbsz;		/* size of pathlist buffer */
	lk_desc
		pd_lock;		/* Queue ID for path descriptor */
	void
		*pd_async;		/* asynchronous I/O resource pointer */
	u_int32	
		pd_state;		/* status bits (eg. PD_SYSTATE) */
	error_code
		(*pd_callback)(pd_com *, iocb_save_area *, u_int32, ...);
						/* ioman callback function pointer */
	void
		*pd_callbackdata; /* ioman callback data (reserved) */
	/* OS-9000 defines 5 u_int32's of reserved space. I don't know all  */
	/* the side effects of expanding this structure so I'm not going to */
};

/* Alias list element definition */
typedef struct list_as *As_list;
typedef struct list_as {
	u_int32
		as_size;		/* size of list element */
	As_list
		as_prev,		/* previous element in list */
		as_next;		/* Next element in list */
	u_int16
		as_scnt,		/* number of elements in source string */
		as_ssize,		/* size of source (alias) string */
		as_tsize;		/* size of target string */
} as_list;

/* I/O process descriptor definition */
typedef struct io_proc {
	process_id
		iop_id;					/* process (and I/O process) id */
	Io_proc
		iop_pproc,				/* previous process in new I/O queue */
		iop_nproc;				/* next process in new I/O queue */
	process_id
		iop_dlkn;				/* next process in deadlock chain */
	u_int32
		iop_sched;				/* process scheduling priority */
	owner_id
		iop_owner;				/* process owner */
	u_int16
		iop_prior,				/* process priority */
		iop_rsrvd;				/* reserved */
	u_int32
		iop_rbytes,				/* bytes read by process */
		iop_wbytes;				/* bytes written by process */
	path_id
		iop_de_path,			/* default execution directory path */
		iop_dd_path,			/* default data directory path */
		iop_paths[NUMPATHS];	/* path translation table */
	As_list
		iop_as_cur;				/* current alias list element when reading */
	u_int32
		iop_resvd[4];			/* reserved */
} io_proc;

/* Interrupt Polling Table format */
/*   Used for both vectored and autovector interrupts. */
/*   Warning: these variables must be declared in the following */
/*   order due to optimized code in the exception handler. */

typedef struct pol_tbl {
	struct pol_tbl	
		*q_link;		/* ptr to next entry in polling tbl */
	void
		*q_serv,		/* Absolute service routine address */
		*q_poll;		/* Absolute polling address */
	u_char
		*q_stat,		/* Static storage address */
		q_prty,			/* Priority (lower numbers are polled first) */
		q_rsv1;			/* reserved */
} pol_tbl, *Pol_tbl;

/* These definitions are for device class.  Software which checks */
/* this field should test the bits only, as the rest will probably */
/* be defined in the future. */
#define DC_SEQ		0x0001
#define DC_RND		0x0002

#if defined(_ANSI_EXT) || defined(__STDC__)

/*	C++ support	*/
#ifdef __cplusplus
extern "C" {
#endif

error_code _os_attach(char *, u_int32, dev_list **);
error_code _os_detach(dev_list *);
error_code _os_getpd(path_id, pd_com **);
error_code _os_get_ioproc(process_id, io_proc **);
error_code _os_alias(char *, char *);
error_code _os_getdl(dev_list **);
error_code _os_rdalst(char *, u_int32 *);
error_code _os_tranpn(process_id, path_id, path_id *);

/*	End C++	*/
#ifdef __cplusplus
}
#endif

#endif

#endif
