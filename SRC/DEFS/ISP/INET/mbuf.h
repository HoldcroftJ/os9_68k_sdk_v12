/****************************************************************************
 *  $Workfile:   mbuf.h  $
 *  $Revision:   1.21  $
 *  $Modtime:   17 Feb 1998 10:22:00  $
 *  $Date:   17 Feb 1998 10:26:08  $
 ****************************************************************************
 *	OS-9/OS-9000 Networking Memory Buffer Definitions						*
 ****************************************************************************
 *  Copyright 1993 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 ??/??/??    Birth                                               ???	*
 *      09/16/93    <***** ISP 2.0 Release ***********************>			*
 *	  1 09/24/93	Merged w/ sysmbuf.h (OS-9000 defs)					bat	*
 *    2 01/09/94    Added ISM/SPF definitions                           bat *
 *      01/17/96    <***** ISP 2.1 Release ***********************>			*
 *      10/25/96    Made modifications for BSD 4.4 and SPF              pjw *
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ********>         *
 *      04/29/97    <***** OS-9000 V2.2 Release (Low Level ROM)***********> * 
 *                  ---- OS-9000/Brutus Board Support V1.0 Released ----    *
 ****************************************************************************/


#ifndef _MBUF_H
#define _MBUF_H


/* Get type & constant definitions */
#include <types.h>

/* Maximum mbuf allocation */
#define MBMAX 32768

/* Mbuf header structure */
typedef struct mbuf {
	struct mbuf 	*m_pnext;			/* mbuf packet data chain */
	struct mbuf 	*m_qnext;			/* mbuf queue chain */
	u_int16			m_alloc;			/* size of mbuf allocation */
	u_int16			m_size;				/* size of data */
	u_int8			m_flags;			/* mbuf flags */
		/* Additional flags for normal mbufs */
		#define SPF_NOFREE      0x80    /* Don't free mbuf after xmitting   */
		#define SPF_DONE        0x40    /* Mbuf has been xmitted			*/
		#define SPF_RXERR       0x08    /* Receive mbuf marked as errored   */
		/*BSD 4.4 m_flag definitions */
		#define M_EXT           0x01  /* has associated external storage */
		#define M_PKTHDR        0x02  /* start of record */
		#define M_EOR           0x04  /* end of record */
		#define M_BCAST         0x10  /* send/received as link-level broadcast */
		#define M_MCAST         0x20  /* send/received as link-level multicast */
	u_int8			m_type;				/* mbuf type */
		#define MB_NONE			0		
		#define	MB_DATA			1		
		#define MB_ADDR			2		
		#define MB_HEADER		3
		#define MB_RSV			0x80    /* Reserved type field */
		#define MB_NEWTYPE 		0xff    /* Flag for checking if the mbuf is
										   a new structure type <future impl> */
	/*BSD 4.4 m_type definitions */
		#define MBT_FREE                0       /* should be on free list */
		#define MBT_DATA                1       /* dynamic (data) allocation */
		#define MBT_HEADER      2       /* packet header */
		#define MBT_SOCKET      3       /* socket structure */
		#define MBT_PCB         4       /* protocol control block */
		#define MBT_RTABLE      5       /* routing tables */
		#define MBT_HTABLE      6       /* IMP host tables */
		#define MBT_ATABLE      7       /* address resolution tables */
		#define MBT_SONAME      8       /* socket name */
		#define MBT_SOOPTS      10      /* socket options */
		#define MBT_FTABLE      11      /* fragment reassembly header */
		#define MBT_RIGHTS      12      /* access rights */
		#define MBT_IFADDR      13      /* interface address */
		#define MBT_CONTROL     14      /* extra-data protocol message */
		#define MBT_OOBDATA     15      /* expedited data  */
	u_int16			m_offset;			/* offset to data from start of mbuf*/
} mbuf, *Mbuf;
#define MBHDR_SZ    			sizeof(struct mbuf)
#define DATA_PTR(mb)    		((u_char*)((char*)(mb)+(mb)->m_offset))

/* OS-9000 specific definitions *********************************************/
#ifdef _OS9000

/* F_MBUF service request function codes */
#include <funcs.h>					/* System function code defs */
#define MBUF_GET	0				/* Get an mbuf */
#define MBUF_GETN	1				/* Get an mbuf -- don't wait */
#define MBUF_FREE	2				/* Free an mbuf */
#define MBUF_MAP	3				/* Return SysMbuf bitmap data */

/* F_MBUF service request parameter block */
#include <srvcb.h>					/* Service control block defs */
typedef struct {
	syscb		cb;					/* Required common section */
	u_int32		mb_code;			/* Function code */
	u_int32		mb_size;			/* Request size */
	Mbuf		mb_mb;				/* Pointer to mbuf */
	void		*mb_pooladdr;		/* MAP output only .. MBuf pool address */
	u_int32		mb_poolsize;		/* MAP output only */
	u_int32		mb_currfree;		/* MAP output only */
	u_int32		mb_lowater;			/* MAP output only */
} f_mbuf_pb, *F_mbuf_pb;

#endif  /* _OS9000 **********************************************************/


/* Function prototypes for mbuflib.l */
#if (defined(_ANSI_EXT) || defined(__STDC__))
extern u_int32		m_adj(Mbuf mb, int32 count);
extern Mbuf			m_cat(Mbuf mb, Mbuf nb);
extern Mbuf			m_copy(Mbuf mb, u_int32 offset, u_int16 count, int32* status);
extern Mbuf			m_deq(Mbuf* q);
extern void			m_enq(Mbuf* q, Mbuf mb);
extern Mbuf			m_flush(Mbuf mb);
extern Mbuf			m_free(Mbuf mb);
extern int32		m_free_m(Mbuf mb);
extern Mbuf			m_free_p(Mbuf mb);
extern void			m_free_q(Mbuf* queue);
extern Mbuf			m_get(u_int16 size, int32* status);
extern Mbuf			m_getn(u_int16 size, int32* status);
extern u_int32		m_len_p(Mbuf mb);
extern u_int32		m_move(Mbuf mb, u_int32 offset, u_int32 count, char* buffer);
extern u_int32		m_msize(Mbuf mb);
extern u_int32		M_PAD(Mbuf mb, u_int16 count);
extern u_int32		m_pad(Mbuf mb, u_int16 count);
extern char*		m_ptod(Mbuf mb);
extern int32		m_pullup(Mbuf* mb, u_int16 count);
extern u_int32		M_UNPAD(Mbuf mb, u_int16 count);
extern u_int32		m_unpad(Mbuf mb, u_int16 count);
extern void			*mtod(Mbuf mb, void *type);
#else
extern u_int32		m_adj();
extern Mbuf			m_cat();
extern Mbuf			m_copy();
extern Mbuf			m_deq();
extern void			m_enq();
extern Mbuf			m_flush();
extern Mbuf			m_free();
extern int32		m_free_m();
extern Mbuf			m_free_p();
extern void			m_free_q();
extern Mbuf			m_get();
extern Mbuf			m_getn();
extern u_int32		m_len_p();
extern u_int32		m_move();
extern u_int32		m_msize();
extern u_int32		M_PAD();
extern u_int32		m_pad();
extern char*		m_ptod();
extern int32		m_pullup();
extern u_int32		M_UNPAD();
extern u_int32		m_unpad();
extern void			*mtod();
#endif

/* Macro routines to manipulate mbufs */
#define M_PAD(mb,count)		((u_int32)((mb)->m_offset += count))
#define M_UNPAD(mb,count)	((u_int32)((mb)->m_offset -= count))
#define mtod(mb,type)		((type)((mb)->m_offset + (char *)(mb)))


#endif	/* _MBUF_H */
