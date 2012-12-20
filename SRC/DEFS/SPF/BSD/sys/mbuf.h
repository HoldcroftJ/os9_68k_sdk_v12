/****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *  01 10/07/96 Creation
 *  02 10/28/96 Ifdef to avoid macro define conflicts with INET/mbuf.h sr
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  03 06/30/97 Added new copyright header                             apw
 *  04 07/01/97 Rewrote M* macros to preserve pkthdr, general clean up rds
 *  05 10/08/97 Changed MALLOC and FREE to save size                   rds
 *  06 02/14/98 Moved MSIZE from minfuncs.h to here                    rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  07 01/31/01 Removed unused Unix defines, added MBT_ZCOPY           rds
 *  08 05/10/01 Fixed MGETHDR_N to allocate sizeof(pkthdr) more space  rds
 ****************************************************************************/
/*
 * Copyright (c) 1982, 1986, 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)mbuf.h	8.3 (Berkeley) 1/21/94
 */

#ifndef _SYS_MBUF_H
#define _SYS_MBUF_H

/*
** This should probably be somewhere else. RDS
*/
#ifndef ssize_t
#define ssize_t int
#endif

#ifndef M_WAITOK
#define	M_WAITOK	0x0000
#define	M_NOWAIT	0x0001
#endif /* M_WAITOK */

/*
** On OS-9 mbufs can be any size, but we maintain thise defines for
** compatibility with the BSD source code.
**
** Mbufs are of a single size, MSIZE (machine/machparam.h), which
** includes overhead.  An mbuf may add a single "mbuf cluster" of size
** MCLBYTES (also in machine/machparam.h), which has no additional overhead
** and is used instead of the internal data area; this is done when
** at least MINCLSIZE of data must be stored.
*/

#define	MSIZE		128								/* size of an mbuf */
#define	MLEN		(MSIZE - sizeof(struct m_hdr))	/* normal data len */
#define	MHLEN		(MLEN - sizeof(struct pkthdr))	/* data len w/pkthdr */

#define	MINCLSIZE	(MHLEN + MLEN)	/* smallest amount to put in cluster */
#define	M_MAXCOMPRESS	(MHLEN / 2)	/* max amount to copy for compression */

/*
** Header at beginning of each bsd_mbuf. These fields are the same
** fields defined in normal OS-9 mbufs, but using BSD names.
*/
struct m_hdr {
	struct	bsd_mbuf *mh_next;		/* next buffer in chain */
	struct	bsd_mbuf *mh_nextpkt;	/* next chain in queue/record */
	u_int16	mh_alloc;		/* size of mbuf allocation */
	u_int16	mh_len;			/* amount of data in this mbuf */
	u_int8	mh_flags;		/* flags; see below */
	u_int8	mh_type;		/* type of data in this mbuf */
	u_int16	mh_data;		/* location of data */
};

/* record/packet header in first mbuf of chain; valid if M_PKTHDR set */
struct	pkthdr {
	int	len;		/* total packet length */
	struct	ifnet *rcvif;	/* rcv interface */
};

/* description of external storage mapped into mbuf, valid if M_EXT set */
struct m_ext {
	caddr_t	ext_buf;		/* start of buffer */
	void	(*ext_free)();		/* free routine if not the usual */
	u_int	ext_size;		/* size of buffer, for ext_free */
};

struct bsd_mbuf {
	struct	m_hdr m_hdr;
	union {
		struct {
			struct	pkthdr MH_pkthdr;	/* M_PKTHDR set */
			union {
				struct	m_ext MH_ext;	/* M_EXT set */
				char	MH_databuf[MHLEN];
			} MH_dat;
		} MH;
		char	M_databuf[MLEN];		/* !M_PKTHDR, !M_EXT */
	} M_dat;
};
#define	mb_next		m_hdr.mh_next
#define mb_alloc	m_hdr.mh_alloc
#define	mb_len		m_hdr.mh_len
#define	mb_data		m_hdr.mh_data
#define	mb_type		m_hdr.mh_type
#define	mb_flags	m_hdr.mh_flags
#define	mb_nextpkt	m_hdr.mh_nextpkt
#define	mb_act		mb_nextpkt
#define	mb_pkthdr	M_dat.MH.MH_pkthdr
#define	mb_ext		M_dat.MH.MH_dat.MH_ext
#define	mb_pktdat	M_dat.MH.MH_dat.MH_databuf
#define	mb_dat		M_dat.M_databuf

/* Define only if they have not been previously defined in SPF/mbuf.h */
#ifndef _MBUF_H
/* mbuf flags */
#define	M_EXT		0x0001	/* has associated external storage */
#define	M_PKTHDR	0x0002	/* start of record */
#define	M_EOR		0x0004	/* end of record */

/* mbuf pkthdr flags, also in m_flags */
#define	M_BCAST		0x0010	/* send/received as link-level broadcast */
#define	M_MCAST		0x0020	/* send/received as link-level multicast */
#endif	/* ndef _MBUF_H */

/* flags copied when copying mb_pkthdr */
#define	M_COPYFLAGS	(M_PKTHDR|M_EOR|M_BCAST|M_MCAST)

/* mbuf types */
#define	MBT_FREE	0	/* should be on free list */
#define	MBT_DATA	1	/* dynamic (data) allocation */
#define	MBT_SONAME	2	/* socket name */
#define	MBT_HEADER	3	/* packet header */
#define	MBT_PCB		4	/* protocol control block */
#define	MBT_RTABLE	5	/* routing tables */
#define	MBT_ZCOPY	6	/* TCP zero copy mbuf */
#define	MBT_ATABLE	7	/* address resolution tables */
#define	MBT_SOCKET	8	/* socket structure */
#define	MBT_SOOPTS	10	/* socket options */
#define	MBT_FTABLE	11	/* fragment reassembly header */
#define	MBT_RIGHTS	12	/* access rights */
#define	MBT_IFADDR	13	/* interface address */
#define MBT_CONTROL	14	/* extra-data protocol message */
#define MBT_OOBDATA	15	/* expedited data */

/* flags to m_get/MGET */
#define	M_DONTWAIT	M_NOWAIT
#define	M_WAIT		M_WAITOK

/*
** mbuf utility macros:
**
**	MBUFLOCK(code)
** prevents a section of code from from being interrupted by network
** drivers.
*/
#define	MBUFLOCK(code)												\
	{ int ms = splimp();											\
		{ code }													\
		splx(ms);													\
	}

/*
** mbuf allocation/deallocation macros:
**
**	MGET(struct bsd_mbuf *m, int how, int type)
** allocates an mbuf and initializes it to contain internal data.
**
**	MGETHDR(struct bsd_mbuf *m, int how, int type)
** allocates an mbuf and initializes it to contain a packet header
** and internal data.
*/
#define MGET(m, size, type) {										\
	int32 mg_err;													\
	m = (struct bsd_mbuf *)m_get(size, &mg_err);					\
	if (m) {														\
		(m)->mb_type = (type);										\
	}																\
}

#define MGET_N(m, size, type) {										\
	int32 mg_err;													\
	m = (struct bsd_mbuf *)m_getn(size, &mg_err);					\
	if (m) {														\
		(m)->mb_type = (type);										\
	}																\
}

#define MGETHDR_N(m, size, type) {									\
	int32 mghd_err;													\
	int32 mghd_temp = size + sizeof(struct pkthdr);					\
	m = (struct bsd_mbuf *)m_getn(mghd_temp, &mghd_err);			\
	if (m) {														\
		(m)->mb_type = (type);										\
		(m)->mb_data = sizeof(struct m_hdr) + sizeof(struct pkthdr);\
		(m)->mb_flags = M_PKTHDR;									\
	}																\
}

/*
** Set the mb_data pointer of a newly-allocated mbuf (m_get/MGET) to place
** an object of the specified size at the end of the mbuf, longword aligned.
*/
#define	M_ALIGN(m, len)												\
	{ (m)->mb_data += (MLEN - (len)) &~ (sizeof(long) - 1); }
/*
** As above, for mbufs allocated with m_gethdr/MGETHDR
** or initialized by M_COPY_PKTHDR.
*/
#define	MH_ALIGN(m, len)											\
	{ (m)->mb_data += (MHLEN - (len)) &~ (sizeof(long) - 1); }


#define MALLOC(memptr, size) {										\
	u_int32 x;														\
	u_int32 *y;														\
	x = (u_int32)size + sizeof(u_int32);							\
	if ( _os_srqmem(&x, (void**)&y, MEM_ANY) != 0) {				\
		memptr = NULL;												\
	} else {														\
		*y = x;														\
		memptr = (void *)((u_int32)y + sizeof(u_int32));			\
	}																\
}

#define FREE(memptr, size) {										\
	u_int32 *x;														\
	x = (u_int32 *)((u_int32)memptr - sizeof(u_int32));				\
	(void)_os_srtmem(*x, (void *)x);								\
}

#define	MFREE(m, nn) { (nn) = (struct bsd_mbuf *)m_free((mbuf *)m); }

/*
 * Mbuf statistics.
 */
struct mbstat {
	u_long	m_mbufs;	/* mbufs obtained from page pool */
	u_long	m_clusters;	/* clusters obtained from page pool */
	u_long	m_spare;	/* spare field */
	u_long	m_clfree;	/* free clusters */
	u_long	m_drops;	/* times failed to find space */
	u_long	m_wait;		/* times waited for space */
	u_long	m_drain;	/* times drained protocols for space */
	u_short	m_mtypes[256];	/* type specific mbuf allocations */
};

#ifdef	SPF_DRVR
extern	struct bsd_mbuf *mbutl;		/* virtual address of mclusters */
extern	char *mclrefcnt;			/* cluster reference counts */
EXTERN struct	mbstat mbstat;
extern	int nmbclusters;
EXTERN union	mcluster *mclfree;
#ifdef SPIP
EXTERN int	max_linkhdr;			/* largest link-level header */
EXTERN int	max_protohdr;			/* largest protocol header */
EXTERN int	max_hdr;				/* largest link+protocol header */
EXTERN int	max_datalen;			/* MHLEN - max_hdr */
#endif /* SPIP */
extern	int mbtypes[];				/* XXX */

#ifdef MBTYPES
int mbtypes[] = {	/* XXX */
	M_FREE,			/* MBT_FREE		0	should be on free list */
	M_MBUF,			/* MBT_DATA		1	dynamic (data) allocation */
	M_MBUF,			/* MBT_HEADER	2	packet header */
	M_SOCKET,		/* MBT_SOCKET	3	socket structure */
	M_PCB,			/* MBT_PCB		4	protocol control block */
	M_RTABLE,		/* MBT_RTABLE	5	routing tables */
	M_ZCOPY,		/* MBT_ZCOPY	6	TCP zero copy mbuf */
	0,				/* MBT_ATABLE	7	address resolution tables */
	M_MBUF,			/* MBT_SONAME	8	socket name */
	0,				/* 				9	*/
	M_SOOPTS,		/* MBT_SOOPTS	10	socket options */
	M_FTABLE,		/* MBT_FTABLE	11	fragment reassembly header */
	M_MBUF,			/* MBT_RIGHTS	12	access rights */
	M_IFADDR,		/* MBT_IFADDR	13	interface address */
	M_MBUF,			/* MBT_CONTROL	14	extra-data protocol message */
	M_MBUF,			/* MBT_OOBDATA	15	expedited data */
#ifdef DATAKIT
	25, 26, 27, 28, 29, 30, 31, 32		/* datakit ugliness */
#endif /* DATAKIT */
};
#endif /* MBTYPES */
#endif /* KERNEL */

#endif /*_SYS_MBUF_H*/
