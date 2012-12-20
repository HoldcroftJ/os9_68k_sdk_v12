#if !defined(_ALLOC_H)
#define _ALLOC_H

/*
 * $Header:   /h0/MWOS/SRC/DPIO/DEFS/VCS/alloc.h_v   1.0   01 Nov 1994 14:49:28   BrentT  $
 * $Revision:   1.0  $
 */

/*--------------------------------------------------------------------------,
!																			!
!				Copyright 1989 by Microware Systems Corporation				!
!							Reproduced Under License						!
!																			!
!  This source code is the proprietary confidential property of Microware	!
!  Systems Corporation, and is provided to licensee for documentation and	!
!  educational purposes only. Reproduction, publication, or distribution	!
!  in any form to any party other than the licensee is strictly prohibited.	!
!																			!
!---------------------------------------------------------------------------!
!																			!
!	OS-9/68k kernel memory allocation definitions							!
!																			!
! edition history															!
!	#	date     comments												by  !
!  -- -------- -------------------------------------------------------- --- !
!	1 03-xx-88 Initial toy version created.								rfd !
|   2 06/08/99 Modified B_defs to use OS9 names for values              mgh !
`--------------------------------------------------------------------------*/

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if !defined(_MODULE_H)
#include <module.h>
#endif

/*------------------------------------------------------------------*/
/* basic (old-style) free memory and fragment list					*/
typedef struct memlist {
	struct memlist
		*nxt,		/* ptr to next (higher addr) free block */
		*prv;		/* ptr to previous (lower addr) free block */
	u_int32
		blksiz,		/* size of this block */
		resv[1];	/* free block continues; at least 4 more bytes */
} mem_list, *Mem_list;


/*------------------------------------------------------------------*/
/* basic memory list - one node per 'color' of memory				*/
typedef struct memcolor {
	u_char
		*lolim,		/* beginning absolute address for this type */
		*hilim;		/* ending absolute address +1 for this type */
	struct memcolor
		*nxt,		/* ptr to next color node */
		*prv;		/* ptr to previous color node */
	Mem_list
		free[2];	/* head of free list ptr */
	u_int32
		frags,		/* total number of memory fragments */
		totfree;	/* total amount of memory in list */
	u_char
		*dma_addr;	/* translation address for dma's, etc.		*/
	u_int16
		access,		/* accessability */
		type,		/* memory type code */
		prior;		/* memory allocation priority */
} mem_color, *Mem_color;

/*------------------------------------------------------------------*/
/* Shared and NVRAM need this at *lolim */
struct mem2 {
	u_int16	inized;		/* check pattern; memory initialized? */
	u_int16	status;		/* access semaphore */
	u_int32	free;		/* OFFSET (from lolim) to free list ptr */
	void	*mdir;		/* auxillary module directory ptr ? */
	void	*resv;		/* other stuff ? (ram disk ptr, ...) */
};

/*------------------------------------------------------------------*/
/* MemDefs module													*/
typedef mh_com *Mh_mem, mh_mem;

/* initialization table (in memdefs module data area) */
typedef struct mem_table {
	u_int16
		type,		/* memory type code */
		prior,		/* memory allocation priority */
		access,		/* access permissions */
		blksiz;		/* search block size */
	u_char
		*lolim,		/* beginning absolute address for this type */
		*hilim;		/* ending absolute address +1 for this type */
	u_int32
		descr;		/* optional description string offset */
	u_int32
		dma_addr,	/* address translation address for dma's, etc. */
		rsvd2[2];	/* reserved, must be zero */
} *Mem_tbl, mem_tbl;


/* access bit definitions */
#define B_USERRAM	B_USER	/* memory allocatable by user procs */
#define B_SHARED	B_SHARE	/* shared memory */

#if 0 						/* These values match OS9/68k names */
#define B_PARITY	(0x02)	/* parity memory; must be initialized */
#define B_ROM		(0x04)	/* read-only memory; searched for modules */
#define B_NVRAM		(0x08)	/* non-volatile RAM; searched for modules */
#endif

#endif
