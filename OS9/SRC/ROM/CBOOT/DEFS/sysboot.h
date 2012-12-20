/*
 * CBOOT system definitions
 */

/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 * history:
 *
 *
 *  History:
 *  Ed.   Date    Who    Reason
 *  --  --------  --- ----------------------------------------------------
 *  00  ??/??/??  ??? original
 *  01  93/09/15  ats Add external for loadrom().
 *                    ---- OS-9/68k V3.0 Release ----
 *                    ---- OS-9/68k V3.1 Release ----
 *                    
 */
#ifndef SYSBOOT
#include	<types.h>
#include	<ctype.h>
#include	<gendefs.h>
#include	<setjmp.h>
#include	<module.h>
#include	<errno.h>
#include	<cmdblk.h>
#include	<sg_codes.h>
#include	<rbf.h>

/*------------------!
! bootmethod codes	!
!------------------*/
#define USERSELECT		0
#define SWITCHSELECT	1
#define AUTOSELECT		2

/*----------------------!
! general definitions	!
!----------------------*/
#define OSKSECTSIZE 256			/* Standard RBF/OS9-680x0 sector size		*/
#define DEFSECTSIZE 4096		/* allow booting from up to 1024 byte sect. */

#define E_BADHEADER	0x0200
#define E_DUMMY		0x600DF00D	/* Dummy error code for undefined keys		*/

#define MIN_BFSIZE	4096		/* 4K Minimum Bootfile size					*/
#define SYNC_CRUZ	0x4372757A	/* Sector zero sync code = 'Cruz'			*/
#define ROMBUGSYNC  0x726d6267	/* rombug sync code (rmbg)                  */
#ifdef _UCC
_asm("RBGSYNC equ $726d6267");	/* same as above for asm code               */
#else
@RBGSYNC equ $726d6267          /* same as above for asm code               */
#endif

#define MINALLOC	256			/* minimum memory allocation size			*/
								/* NOTE: MINALLOC must be divisible by 16!!	*/
								
#define MAXARGS		8			/* maximum # of arguments on an input line	*/
#define IOBUFSIZE	256			/* size of input and output buffers			*/
#define CASEBIT		0x20

#define BUS_TRAP		2		/* vector to install bus trap handler		*/
#define ADDR_TRAP		3		/* vector to install address trap handler	*/
#define ILL_TRAP		4		/* vector to install ill instr trap handler	*/
#define TRAC_TRAP		9		/* vector for trace trap					*/
#define LINF_TRAP		11		/* vector to install line 'F' trap handler	*/
#define AUTO7_TRAP		31		/* vector for autovector level 7			*/

#define ASM_SYNC	0x0515B007	/* assembler boot function sync code	*/
#ifdef _UCC
_asm("ASM_SYNC equ $0515B007");	/* assembler boot function sync code    */
#else
#asm
ASM_SYNC equ 		$0515B007	/* assembler boot function sync code	*/
#endasm
#endif

/*------------------------------------------------------!
! File Descriptor format (Compatible with 6809 OS-9)	!
!------------------------------------------------------*/
typedef struct rbffd {
	u_char	fd_att;			/* Attributes					*/
	u_char	fd_own[2];		/* Owner						*/
	u_char	fd_dat[5];		/* Date last modified			*/
	u_char	fd_lnk;			/* Link count					*/
	u_char	fd_siz[4];		/* file size					*/
	u_char	fd_creat[3];	/* date created yymmdd			*/
	u_char	fd_seg;			/* Beginning of segment list	*/
} rbf_fd, *Rbf_fd;

/*--------------------------!
! Segment List Entry Format	!
!--------------------------*/
typedef struct fdslentry {
	u_char	fdsl_a[3];		/* Segment beginning physical sector number	*/
	u_char	fdsl_b[2];		/* Segment size								*/
} fdsl_entry, *Fdsl_entry;

#define FDSL_SIZE	5		/* Segment list entry size					*/

/* replaces local definitions, defaulting instead to the rbf.h version  */
typedef struct sector0 sect_zero, *Sect_zero;


/*--------------------------------------!
! Boot format identification structure	!
!--------------------------------------*/
typedef struct fmatid {
	char	*fcode;				/* format code (i.e. "5803")			*/
	u_char	type;				/* equivalent to "pd_typ"				*/
	u_char	density;			/* equivalent to "pd_dns"				*/
	u_char	cyls;				/* equivalent to "pd_cyls"				*/
	u_char	sectors;			/* equivalent to "pd_sct"				*/
	u_char	sectors_t0;			/* equivalent to "pd_t0s"				*/
	u_char	toffs;				/* equivalent to "pd_toffs"				*/
	u_char	soffs;				/* equivalent to "pd_soffs"				*/
} fmat_id, *Fmat_id;


/*----------------------------------!
! Boot driver definition structure	!
!----------------------------------*/
typedef struct bootdriv {
	error_code	(*driver_entry)();
	char		*driver_name;
	char		*menuline;
	char		*idstring;
	u_int32		reserved;
} bdrivdef, *Bdrivdef;


/*--------------------------!
! Boot jump table structure	!
!--------------------------*/
typedef struct bootjmpentries {
	u_int32	sysreset,	/* Dead Start initialization			*/
			inchar,		/* input one character					*/
			outchar,	/* output one character					*/
			extract,	/* primitive memory request				*/
			rstdebug,	/* invoke system level debugger			*/
			/* if no system level debugger, do quickstart		*/
			outhex,		/* output a nibble (d0.n) in hex		*/
			out1hex,	/* output a byte (d0.b) in hex format	*/
			out2hex,	/* output a word (d0.w) as 2 hex digits	*/
			out4hex,	/* output a long (d0.l) as 4 hex digits	*/
			inchchek;	/* input one char (unblocked, no echo)	*/
} bootjmptab, *Bootjmptab;


/*------------------------------!
! memory list data structure	!
!------------------------------*/
typedef struct dumbmem {
	struct dumbmem	*nxt;				/* where the next one is */
	u_int32			size;				/* how much there is */
} dumb_mem, *Dumb_mem;


/*--------------------------------------!
! Memory list BUFFER entry structure	!
! DO NOT CONFUSE WITH "dumb_mem"!!!		!
!--------------------------------------*/
typedef struct mlbentry {
	Dumb_mem		pointer;			/* where a memory block is		*/
	u_int32			size;				/* the size of that same block	*/
} mlb_entry, *Mlb_entry;


/*--------------------------!
! rom list data structure	!
!--------------------------*/
typedef struct rom_list {
	u_char			*addr;				/* where the rom is */
	u_int32			siz;				/* size of the block */
} rom_list, *Rom_list;

/*----------------------!
! Function declarations	!
!----------------------*/
extern error_code	sys_boot(),	diskboot(),	 tapeboot(),	 romboot(),
					portboot(),	rdiskboot(), inizbootfunc(), trappit(),
					loaderboot(), loadrom();

extern void			fixmemlist();
extern void 		sort_mem();
extern Dumb_mem		insert();

extern char			*inttoascii();
extern u_char		*malloc();
extern void			*gethexaddr();
extern char			makelower();
extern char			*instr();
extern char			*dscpy();
extern Bdrivdef		getboottype();

/*------------------------------------------------------!
! Static storage variable declarations for "sysboot"	!
!------------------------------------------------------*/
extern u_char		*sysb_gptr;		/* sysboot global data area pointer		*/
extern u_int32		sysb_dsize;		/* size of sysboot global data area		*/
extern Bootjmptab	bootjmp;		/* pointer to the boot ROM entry table	*/
extern u_char		*portaddr;		/* base address of the hardware device	*/
extern Dumb_mem		freememlist;	/* pointer to the free memory list		*/
extern void			*exjmptab;		/* pointer to the exception jump table	*/
extern void			*sysrommap;		/* start address of system ROM list		*/
extern void			*endrommap;		/* end address of system ROM list		*/
extern u_int32		chunksize;		/* memory chunk size					*/
extern u_char		*bootram;		/* pointer to start of bootfile RAM		*/
extern u_int32		bootsize;		/* size of the bootfile					*/
extern u_int32		memsize;		/* size of RAM scarfed from system		*/
extern Bdrivdef		bootdriver;		/* boot driver definition array			*/
extern u_char		bdrivers_qty;	/* number of boot drivers in array		*/
extern char			*inbuff;		/* input buffer pointer					*/
extern char			*outbuff;		/* output buffer pointer				*/
extern u_char		*hbuff;			/* tape header or sector zero buffer	*/
extern u_char		vflag;			/* verbose flag: 0 = no error messages	*/
extern u_char		ctrlr_id;		/* (usually SCSI) controller I.D.		*/
extern jmp_buf		gp_btrap;		/* general purpose bus trap longjmp struct*/
extern char			*argarray[MAXARGS];	/* input line argument array		*/

/*------------------------------------------------------------------!
! Static storage variable declarations for pointer initialization	!
!------------------------------------------------------------------*/
extern error_code	(*inizdriver)(),	/* pointer to driver's init routine	*/
					(*readdriver)(),	/* pointer to driver's read routine	*/
					(*rwnddriver)(),	/* pointer to driver's rewind routine*/
					(*termdriver)();	/* pointer to driver's term routine	*/

/*------------------------------------------------------!
! Static storage variable declarations for "miscboot"	!
!------------------------------------------------------*/
extern char			barray_count;

/*------------------------------------------------------!
! Static storage variable declarations for "diskboot"	!
!------------------------------------------------------*/
extern Sect_zero		sectzero;		/* sector zero pointer				*/
extern u_char			*pathbuf;		/* read buffer pointer				*/
extern int				drivenum;		/* logical drive number				*/
extern struct rbf_opt	pathopts;		/* pseudo path descriptor options	*/
extern struct rbf_opt	*defopts;		/* default path descriptor opts ptr	*/
extern struct rbf_opt	boot_fpd,		/* default floppy pathdesc options	*/
						boot_hpd;		/* default hard drive pathdesc opts	*/
extern struct rbfdrive	drivetable;		/* the pseudo drive table			*/
extern struct cmdblk	cmdblk;			/* high/low level SCSI command block*/
										/* communications structure			*/

/*--------------------------------------------------------------!
! Static storage variable declarations for debugger variables	!
!--------------------------------------------------------------*/
extern u_char			trapflag;		/* debugger trap handler flag		*/

#define SYSBOOT
#endif
