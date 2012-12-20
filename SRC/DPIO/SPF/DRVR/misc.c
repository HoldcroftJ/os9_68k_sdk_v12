/****************************************************************************
 *  ID:     %A%
 *  Date:   %G%
 ****************************************************************************
 *  SPF Device Driver miscellaneous functions.								*
 ****************************************************************************
 *  Copyright 1994 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                                By  *
 * --- -------- ------------------------------------------------------- --- *
 *  1 01/07/94 Created from SPF v1.0.2                                  bat *
 *  2 03/28/95 Reorganization of logical unit & driver static           bat *
 *  3 06/30/95 <---------------- SPF v2.0(pre) ---------------->        dwj *
 *  4 03/22/96 Made return_mbuf a macro. the done bit is now 		    	*
 *		       handled by the new mbuflib.l. The macro is in spf.h    udayn *
 *	5 05/30/96 Added _nf calls for SPF_NOFREE automatic operation       cas *
 ****************************************************************************/

/*
**	Header Files
*/

#include <types.h>
#include <errno.h>

#define SPF_DRVR
#include <SPF/spf.h>

/*
**	Macro Definitions
*/
#ifndef SUCCESS
#define SUCCESS 0
#endif

/**************************************************************************
** Get an mbuf from the system free pool. This function also automatically
** inserts the device entry that was passed in into the beginning of the
** mbuf so the SPF receive thread can deal with it.
*/
error_code get_mbuf(Dev_list dev_entry, u_int32 size, Mbuf *mb)
{
	u_int16 realsz = size + ((dev_entry == NULL) ? 0 : sizeof(Dev_list));
	int32 err;

	if ((*mb = m_getn(realsz,&err)) == NULL) {
		*mb = NULL;
		return(err);
	}

	/* put current device entry into new mbuf (if neccessary) */
	if (dev_entry != NULL) {

		Dev_list *new_de_ptr;

		new_de_ptr = mtod(*mb, Dev_list*);
		*new_de_ptr = dev_entry;
		(*mb)->m_offset += sizeof(Dev_list);
	}

	return(SUCCESS);

} /* END get_mbuf() */

/************************************************************************
**	Get a variable size mbuf from the system free pool with space 
**	at the top for the device entry, the path descriptor and any 
**	user specified parameter block. 
*/
error_code get_varsz_mbuf(Dev_list dev_entry, Spf_pdstat pd, void *pb, 
			  u_int32 pb_sz, u_int32 data_size, Mbuf *mb)
{
    u_int16 realsz = data_size + pb_sz;
    int32 err;

    realsz += ((dev_entry == NULL) ? 0 : sizeof(Dev_list));
    realsz += ((pd == NULL) ? 0 : sizeof(Spf_pdstat));

    if ((*mb = m_getn(realsz,&err)) == NULL) {
		*mb = NULL;
		return(err);
    }

    /* put current device entry into new mbuf (if neccessary) */
    if (dev_entry != NULL) {
	
	Dev_list *new_de_ptr;
	
	new_de_ptr = (Dev_list*)DATA_PTR(*mb);
		*new_de_ptr = dev_entry;
		(*mb)->m_offset += sizeof(Dev_list);
    }

    /* put current path descriptor  into new mbuf (if neccessary) */
    if (pd != NULL) {
		Spf_pdstat *new_pd_ptr;
	
		new_pd_ptr = (Spf_pdstat*)DATA_PTR(*mb);
		*new_pd_ptr = pd;
		(*mb)->m_offset += sizeof(Spf_pdstat);
    }
	
    if (pb_sz) {
		char *ptr = (char*) DATA_PTR(*mb);
		char *ptr2 = pb;
	
		(*mb)->m_offset += pb_sz;
		if (pb) 
		    while (pb_sz--)
				*ptr++ = *ptr2++;
    }

    return(SUCCESS);

} /* END get_varsz_mbuf() */

/************************************************************************
** return no free packet chain to the mbuf free pool. This function
** traverses the packet chain and clears the SPF_NOFREE bit before
** returning the mbuf.
**/
Mbuf m_free_p_nf(Mbuf mbp)
{
    Mbuf	qnext = NULL;

    if (mbp) {
        qnext = mbp->m_qnext;

        while (mbp) {
			mbp->m_flags = 0;
            mbp = m_free(mbp);
        }
    }
    return(qnext);
}

/************************************************************************
** return no free packet queue to the mbuf free pool. This function
** traverses the packet queue and clears the SPF_NOFREE bit before
** returning the mbuf.
**/
void m_free_q_nf(Mbuf *queue)
{
	Mbuf	mbp;

	while (mbp = m_deq(queue))
		m_free_p_nf(mbp);

	return;
}

/************************************************************************
** Get a no free mbuf. This function gets an mbuf, sets the SPF_NOFREE
** bit while making sure the SPF_DONE bit is cleared.
**/
Mbuf m_getn_nf(u_int16 size, int32 *status)
{
	Mbuf	mb;

	if (mb = m_getn(size, status))
		mb->m_flags |= SPF_NOFREE;

	return(mb);
}

/************************************************************************
** Set an existing mbuf to nofree. This function takes an existing mbuf
** queue and sets all mbufs in the queue to SPF_NOFREE..
**/
void m_set_nf(Mbuf mbp)
{
    Mbuf	qnext;

    while (mbp) {
        qnext = mbp->m_qnext;
        while (mbp) {
			mbp->m_flags = SPF_NOFREE;
			mbp = mbp->m_pnext;
		}
		mbp = qnext;
    }
}

/**************************************************************************
** Get an SPF_NOFREE mbuf from the system free pool. This function also
** automatically inserts the device entry that was passed in into the
** beginning of the mbuf so the SPF receive thread can deal with it.
*/
error_code get_nf_mbuf(Dev_list dev_entry, u_int32 size, Mbuf *mb)
{
	u_int16 realsz = size + ((dev_entry == NULL) ? 0 : sizeof(Dev_list));
	int32 err;

	if ((*mb = m_getn_nf(realsz,&err)) == NULL) {
		*mb = NULL;
		return(err);
	}

	/* put current device entry into new mbuf (if neccessary) */
	if (dev_entry != NULL) {

		Dev_list *new_de_ptr;

		new_de_ptr = mtod(*mb, Dev_list*);
		*new_de_ptr = dev_entry;
		(*mb)->m_offset += sizeof(Dev_list);
	}

	return(SUCCESS);

} /* END get_nf_mbuf() */

/************************************************************************
**	Get a variable size mbuf from the system free pool with space 
**	at the top for the device entry, the path descriptor and any 
**	user specified parameter block. 
*/
error_code get_varsz_nf_mbuf(Dev_list dev_entry, Spf_pdstat pd, void *pb, 
			  u_int32 pb_sz, u_int32 data_size, Mbuf *mb)
{
    u_int16 realsz = data_size + pb_sz;
    int32 err;

    realsz += ((dev_entry == NULL) ? 0 : sizeof(Dev_list));
    realsz += ((pd == NULL) ? 0 : sizeof(Spf_pdstat));

    if ((*mb = m_getn_nf(realsz,&err)) == NULL) {
		*mb = NULL;
		return(err);
    }

    /* put current device entry into new mbuf (if neccessary) */
    if (dev_entry != NULL) {
		Dev_list *new_de_ptr;
	
		new_de_ptr = (Dev_list*)DATA_PTR(*mb);
		*new_de_ptr = dev_entry;
		(*mb)->m_offset += sizeof(Dev_list);
    }

    /* put current path descriptor  into new mbuf (if neccessary) */
    if (pd != NULL) {
		Spf_pdstat *new_pd_ptr;
	
		new_pd_ptr = (Spf_pdstat*)DATA_PTR(*mb);
		*new_pd_ptr = pd;
		(*mb)->m_offset += sizeof(Spf_pdstat);
    }
	
    if (pb_sz) {
		char *ptr = (char*) DATA_PTR(*mb);
		char *ptr2 = pb;
	
		(*mb)->m_offset += pb_sz;
		if (pb) 
		    while (pb_sz--)
			*ptr++ = *ptr2++;
    }

    return(SUCCESS);

} /* END get_varsz_nf_mbuf() */
