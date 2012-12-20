/****************************************************************************
 *           Copyright 1989-1997 by Microware Systems Corporation           *
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
 *   8 10/11/96 Updated arp_msg struct in arp.h to resolve alignment    sr
 *              /padding issues on ppc.  Associated code changes.
 *  09 10/16/96 Fixed casting issues associated with changes in       sr/tmd
 *              struct of arp_msg in edition 8. (arp.c)
 *  10 10/22/96 Add hardware txoffset in ARP request packet
 *  11 10/20/96 Bug fix for arp request timer
 *  17 01/17/97 Zero out memory allocated by srqmem.                    sr
 *  18 01/23/97 Changes to ppstg to make new style spenet work.         sr
 *  21 02/03/97 Return ENOBUFS if cannot allocate mbuf.                 sr
 *     02/19/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  23 04/27/97 Get arp source address from per path storage           rds
 *              Convert from ISP to BSD header files                   rds
 *  24 04/28/97 Split part of defs.h into bsddefs.h                    rds
 *  25 04/29/97 Fix host to net conversions in send_arp                 sr
 *  26 05/03/97 Fix source address code added in edition 23            rds
 *              Replace get_mbuf with m_getn, clean up unused code     rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  28 05/19/97 Fix RARP problem with freeing mbuf twice               rds
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  29 12/05/97 Fixed family check in arp_delete & arp_change          tmd
 *     12/09/97 Made process_arp_request answer "published" entries    tmd
 *     12/09/97 Nuked trailer code that was #if 0-ed                   tmd
 *     12/09/97 Fixed a lot of confusion in process_arp_reply where    tmd
 *              the code stolen from ISP was handling an arp request   tmd
 *  30 12/10/97 Added code to retrieve a single arp entry              tmd
 *              arptbl_copy now sets size of arp table in SPF pb       tmd
 *  31 02/03/98 Removed #if 0 code from process_arp_reply              rds
 *              Removed compiler warnings                              rds
 *  32 04/01/98 Cleaned up code.                                       sr
 *     04/01/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  35 01/24/99 Set arp_op to ARP_REPLY in proxy arp response          rds
 *  37 11/22/99 Many changes to support caching and new style spenet   rds
 *  41 02/04/01 Send routing socket update message for arp add/delete  rds
 *  43 06/04/01 CF11330 increase held mbuf size to 6                   taka
 *  44 08/10/01 CF12061: arp cache now updated when HW addr changes,   kimk
 *                       update arp cache for new responses.
 *  45 08/16/01 Fixed alignment problems in edition 44 changes         rds
 ****************************************************************************/

#define EXTERN extern
#include "bsddefs.h"
#include <SPF/spf.h>
#include <SPF/spinet.h>
#include <SPF/spsocket.h>
#include <net/route.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <sys/mbuf.h>
void arp_entry_remove(Dev_list, Arptab);
void rt_msg_send(Dev_list, Arptab, u_int32);

#define satosin(sa) ((struct sockaddr_in *)(sa))
/*
	ARP timeout routine.  This function is responsible for flushing
	out old ARP entries periodically.
*/

void arptick(Timer_pb tpb) {
	Dev_list   dev_entry = (Dev_list) tpb->t_pb;
	Spf_drstat drstat = (Spf_drstat) (dev_entry->v_dr_stat);
	Arpstat arpstat = &drstat->arpstat;
	Arptab  arproot;
	Arptab  arpent;
	Arptab  arpnext;

	if (arpstat->busy_flag == TRUE) {
		arpstat->busy_count++;
		return;
	}

	arproot = &arpstat->arproot;
	for (arpent = arproot->next; arpent != arproot; arpent = arpnext) {
		arpnext = arpent->next;
		if (arpent->at_flags & ATF_PERM) {
			/* Skip permanent entries */
			continue;
		}
		/*
		** Increment at_timer and delete the arp entry if it has
		** expired. Make sure we clear the cached entry if needed,
		** free any queued mbufs, and return the allocated memory.
		*/
		arpent->at_timer += arpstat->busy_count;
		if (++arpent->at_timer > ((arpent->at_flags & ATF_COM) ?
									drstat->kill_c : drstat->kill_i)) {
			if (drstat->arpent_cache == arpent)
				drstat->arpent_cache = &drstat->arpstat.arproot;

			arp_entry_remove(dev_entry, arpent);
		}
	}
	arpstat->busy_count = 0;
}


int verify_protoaddr(struct in_addr *srcip, Pp_enetlist enet_entry,
												u_int32 numprotoaddr) {
	struct addr_list *addrlist = enet_entry->addr_list;
	int cnt;

	for (cnt = 0; cnt < numprotoaddr; cnt++) {
		if ((srcip->s_addr == addrlist[cnt].proto_addr.s_addr) &&
		                                   (addrlist[cnt].in_useflag != 0)) {
			/* IP address matches with one of the interface protocol addresses*/
			return(cnt);
		}
	}
	/* None of the addresses in the protocol array match. */
	return (ERROR);
}

int
isEtherBroadcastAddr(void *eaP)
{
	u_int8 *ea = eaP;

	if (ea[0] == 255 && ea[1] == 255 && ea[2] == 255 &&
		ea[3] == 255 && ea[4] == 255 && ea[5] == 255)
		return 1;

	return 0;
}	

/****************************************************************************/
/* our machine received arp request.  We match the message destination ip   */
/* address with our ip address.  If the arp request is meant for us, create */
/* a response packet and send it.											*/
/****************************************************************************/
int process_arp_request(Dev_list dev_entry, Pp_enetlist enet_entry, Mbuf mb)
{
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	int        taddr_ptr;
	struct ether_arp* arpmsg;
	struct arptab     *at;
	int SaveInCache = TRUE;
	struct in_addr addr;

	DEBUG_DATA(lustat->lu_dbg, "AP  ProReq", 0);

	/* remove the Ethernet header (it will be added again if we reply) */
	mb->m_size   -= ETHERHEADSIZE;
	mb->m_offset += ETHERHEADSIZE;

	/* Set up a pointer to the ARP request/reply    */
	arpmsg = mtod(mb, struct ether_arp*);

	/*
	** The source protocol address may not be 4-byte aligned in the mbuf,
	** but we really want to do a 4-byte compare in arptab_look. To make
	** this work we will pull out the IP address one byte at a time and
	** stuff it in a locally declared structure. All entries in the arp
	** table are stored in network byte order so we need to juggle the
	** bytes around on little endian machines. We could have used htonl
	** here, but that would just make us shift and or bits around twice.
	** Because this will be done for *every* ARP packet on the network
	** it might be worth the ugliness to save a couple instructions.
	*/
#ifdef _BIG_END
	addr.s_addr = arpmsg->arp_spa[0] << 24 | arpmsg->arp_spa[1] << 16 |
				arpmsg->arp_spa[2] << 8 | arpmsg->arp_spa[3];
#else
	addr.s_addr = arpmsg->arp_spa[3] << 24 | arpmsg->arp_spa[2] << 16 |
				arpmsg->arp_spa[1] << 8 | arpmsg->arp_spa[0];
#endif
	/*
	** check arp table to see if we have an entry for the requestor
	** if we do, update the hardware address field in the table in
	** case it has changed since we cached it.
	*/
	if ((at = arptab_look(dev_entry, &addr))) {
		/*
		** if this is not a permenant entry &&
		** the requestor is not using the
		** broadcast address.
		*/
		if ((at->at_flags & ATF_PERM) == 0 && !isEtherBroadcastAddr(arpmsg->arp_sha)) {
			/*
			** if HA address is changing,
			** update entry and send routing
			** notification
			*/
			if (memcmp(at->at_enaddr, arpmsg->arp_sha, sizeof(arpmsg->arp_sha))) 
			{
				/*
				** update arp cache entry with
				** new hardware address
				*/
			 	memcpy(at->at_enaddr, arpmsg->arp_sha, sizeof(arpmsg->arp_sha));
				at->at_flags |= ATF_COM;	/* if it wasn't complete, it is now */

				/*
				** N.B., leave the at->at_timer alone. This
				** is done to prevent an unused entry from
				** sticking in the cache only because other nodes
				** keep arping it.  If we actually send to the dude
				** in the cache, the timer will be updated then.
				**
				** Hopefully, the linear arptab_lookup will be improved
				** so leaving entries cached won't hurt too badly.
				*/

				/*
				** Send routing socket notification
				**   (but only if the HA actually changed)
				*/
				rt_msg_send(dev_entry, at, RTM_ADD);
			}
		}
		SaveInCache = FALSE;		/* no need to save this entry */
 	}

	if ((taddr_ptr = verify_protoaddr((struct in_addr *) arpmsg->arp_tpa,
	                           enet_entry, lustat->lu_numprotoaddr)) != -1) {
		/*
		** if we do not have an arp cache entry
		** for this request and we're going to
		** answer it, make a cache entry now since
		** it's almost certain that the requestor will
		** be sending to us.  This will prevent us from
		** arping for him later
		*/
		if (SaveInCache) 
		{
			at = arptnew(dev_entry, &addr);
			if (at)
			{
				if (!isEtherBroadcastAddr(arpmsg->arp_sha)) {
					/*
					** set arp cache entry to
					** current hardware address
					*/
				 	memcpy(at->at_enaddr, arpmsg->arp_sha, sizeof(arpmsg->arp_sha));
					at->at_flags |= ATF_COM;

					/* Send routing socket notification */
					rt_msg_send(dev_entry, at, RTM_ADD);
				}
			}
		}

		DEBUG_DATA(lustat->lu_dbg, "AP  FOR US", 0);
		/* Create the response packet */
		arpmsg->arp_op = htons(ARP_REPLY);
		memcpy(arpmsg->arp_tha, arpmsg->arp_sha, ETHERHARDSIZE);
		memcpy(arpmsg->arp_tpa, arpmsg->arp_spa, sizeof(arpmsg->arp_spa));
		memcpy(arpmsg->arp_sha, enet_entry->ether_addr, ETHERHARDSIZE);
		memcpy(arpmsg->arp_spa,
		    &(enet_entry->addr_list[taddr_ptr].proto_addr.s_addr),
		          sizeof(enet_entry->addr_list[taddr_ptr].proto_addr));

		DEBUG_DATA(lustat->lu_dbg, "AP  RPLYPK", 0);
		return(TRUE);
	}

	/*
	** Similar to above, we need to protect against the target protocol
	** address not being 4-byte aligned in the mbuf.
	*/
#ifdef _BIG_END
	addr.s_addr = arpmsg->arp_tpa[0] << 24 | arpmsg->arp_tpa[1] << 16 |
				arpmsg->arp_tpa[2] << 8 | arpmsg->arp_tpa[3];
#else
	addr.s_addr = arpmsg->arp_tpa[3] << 24 | arpmsg->arp_tpa[2] << 16 |
				arpmsg->arp_tpa[1] << 8 | arpmsg->arp_tpa[0];
#endif

	/* check arp table to see if we have an entry we should "publish" */
	if ((at = arptab_look(dev_entry, &addr)) && (at->at_flags & ATF_PUBL)) {
		/* Create the response packet */
		arpmsg->arp_op = htons(ARP_REPLY);
		memcpy(arpmsg->arp_tha, arpmsg->arp_sha, ETHERHARDSIZE);
		memcpy(arpmsg->arp_tpa, arpmsg->arp_spa, sizeof(arpmsg->arp_spa));
		memcpy(arpmsg->arp_sha, at->at_enaddr, sizeof(arpmsg->arp_sha));
		memcpy(arpmsg->arp_spa, &at->at_iaddr.s_addr, sizeof(arpmsg->arp_spa));
		DEBUG_DATA(lustat->lu_dbg, "AP  PUBPKT", 0);
		return(TRUE);
	}

	/* not for us, ditch the mbuf and return */
	DEBUG_DATA(lustat->lu_dbg, "AP  Not4Us", 0);
	m_free_p(mb);
	return(FALSE);

} /* End process_arp_request */


/* Find an entry to the ENET per path list by the lower layer driver's
 * device entry address.
 */
Pp_enetlist find_enet_ppstg(Spf_drstat drstat, Dev_list dndrvr_deventry)
{
	Pp_enetlist  enet_list = drstat->dr_pp_list;

	while (enet_list != NULL) {
		if (enet_list->enet_dndrvr == dndrvr_deventry) {
			return(enet_list);
		}
		enet_list = enet_list->next;
	}

	return(NULL);
}


/****************************************************************************/
/* Our arp reply for the request came back.  We process this reply message  */
/* by inserting the information into our arp cache							*/
/****************************************************************************/
void process_arp_reply(Dev_list dev_entry, Pp_enetlist enet_entry, Mbuf mb)
{
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	struct ether_arp *ea;
	struct arptab    *at;
	struct in_addr   isaddr, itaddr;

	/* Get rid of Ethernet header first */
	mb->m_offset += sizeof(struct ether_header);
	mb->m_size   -= sizeof(struct ether_header);

	ea = mtod(mb, struct ether_arp *);
	memcpy(&isaddr, ea->arp_spa, sizeof (ea->arp_spa));
	memcpy(&itaddr, ea->arp_tpa, sizeof (ea->arp_spa));

#if 0
	/* the request is from myself, ignore it */
	if (!memcmp(ea->arp_sha, enet_entry->ether_addr, sizeof (ea->arp_sha)))
		goto out;
#endif

	/* someone says his interface address is the broadcast address! */
	if (isEtherBroadcastAddr(ea->arp_sha))
		goto out;

	/* If the reply address matches one of our addresses, drop it */
	if (verify_protoaddr(&isaddr, enet_entry, lustat->lu_numprotoaddr) != -1)
		goto out;

	/* see if we have an outstanding entry for this source address */
	at = arptab_look(dev_entry, &isaddr);
	if (at != NULL) {
		memcpy(at->at_enaddr, ea->arp_sha, sizeof(ea->arp_sha));
		at->at_flags |= ATF_COM;
		/* should we also zero the timer?  We may have overheard this msg */

		/* Send routing socket notification */
		rt_msg_send(dev_entry, at, RTM_ADD);

		/* if we are holding a packet waiting on the arp reply, send it */
		while (at->at_hold != NULL) {
			Mbuf	send_mbp = at->at_hold;
			at->at_hold = at->at_hold->m_qnext;
			send_mbp->m_qnext = NULL;

			/* add the ethernet header */
			if(add_enet(dev_entry,at->at_enaddr, ETHERTYPE_IP,
			                         enet_entry, send_mbp)!=SUCCESS) {
				m_free_p(send_mbp);
				return;
			}
			/* send the data down to lower protocols */
			SMCALL_DNDATA(dev_entry, enet_entry->enet_dndrvr, send_mbp);
		}
		at->at_hold = 0;
	}

	/* just free the mbuf */
out:
	m_free_p(mb);
	return;

} /* End arp_reply */

/****************************************************************************/
/* We received reverse arp request.  We must send the reply back with their */
/* ip address in it.  We will have to figure out, a way to look up, if we   */
/* have the proto address of the other end, and if we are authorized to     */
/* service the request.                                                     */
/****************************************************************************/
int process_rarp_request(Dev_list dev_entry, Pp_enetlist enet_entry, Mbuf mb)
{
#ifdef DEBUG
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);

	debug_data(lustat->lu_dbg, "AP  RAPREQ", 0);
#endif

	/* Free the mbuf and get out of here */
	m_free_p(mb);
	return(FALSE);

} /* End process_RARP_request */

/****************************************************************************/
/* Since we don't send reverse arp request (why do we know some machine     */
/* ethernet address but not knowing IP address?), we have no need of		*/
/* processing reverse arp reply.											*/
/****************************************************************************/
void process_rarp_reply(Dev_list dev_entry, Pp_enetlist enet_entry, Mbuf mb)
{
#ifdef DEBUG
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);

	debug_data(lustat->lu_dbg, "AP  RAPRPY", 0);
#endif

	/* Reverse ARP reply not yet supported */

	/* free the mbuf and return */
	m_free_p(mb);

	return;
}


/****************************************************************************/
/* We want to communicate with a machine with only IP address knowing.      */
/* So we send arp request package with broadcast, and wait for reply.		*/
/****************************************************************************/
error_code send_arp_request(Dev_list dev_entry, struct in_addr *destip,
							Pp_enetlist enet_entry)
{
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	int32             status;
	struct ether_arp* arpmsg;
	int               arp_request_msg_size, i;
	Mbuf              mb;
	u_int8            broadcast_addr[6]={255,255,255,255,255,255};
	struct in_addr	  srcip;

	DEBUG_DATA(lustat->lu_dbg, "AP  SndReq", 0);

	/* allocate an mbuf big enough to hold the ARP */
	/* request and the ethernet header */
	arp_request_msg_size = sizeof(struct ether_arp)+sizeof(struct ether_header)
								+ enet_entry->hw_txoffset;

	if((mb = m_getn(arp_request_msg_size, &status)) == NULL) {
		return (status);
	}

	/*
	** We need to grab the source address out of the per path storage
	** and not the packet because we may be a gateway fowarding a
	** packet that was sourced somewhere else.
	*/
	for(i=0; i<lustat->lu_numprotoaddr; i++){
		if (enet_entry->addr_list[i].in_useflag){
			srcip = enet_entry->addr_list[i].proto_addr;
			break;
		}
	}
	if (i == lustat->lu_numprotoaddr){
		m_free_p(mb);
		return(SUCCESS);
	}

	/* now adjust the mbuf size and offset to ignore the ethernet portion */
	mb->m_size = sizeof(struct ether_arp);
	mb->m_offset += enet_entry->hw_txoffset;
	mb->m_offset += sizeof(struct ether_header);

	/* Set up a pointer to the ARP request/reply    */
	arpmsg = mtod(mb, struct ether_arp*);

	arpmsg->arp_hrd = htons(ETHERHARDTYPE);
	arpmsg->arp_pro = htons(IPPROTTYPE);
	arpmsg->arp_hln = ETHERHARDSIZE;
	arpmsg->arp_pln = IPPROTSIZE;
	arpmsg->arp_op = htons(ARP_REQUEST);
	memcpy(arpmsg->arp_sha, enet_entry->ether_addr, ETHERHARDSIZE);
	memcpy(arpmsg->arp_spa, &srcip, sizeof(struct in_addr));
	for(i=0;i<6;i++)
		arpmsg->arp_tha[i]=0x00;
	memcpy(arpmsg->arp_tpa, destip, sizeof(struct in_addr));

	DEBUG_DATA(lustat->lu_dbg, "AP  SndPkt", destip->s_addr);

	/* add the ethernet header */
	add_enet(dev_entry, broadcast_addr, ETHERTYPE_ARP, enet_entry, mb);

	/* write the data to the network */
	return(SMCALL_DNDATA(dev_entry,enet_entry->enet_dndrvr,mb));

} /* End send_arp_request */

/****************************************************************************/


/*
 * PUBLIC Function:
 *      process_arp(Dev_list dev_entry, Pp_enetlist enet_entry, Mbuf mb)
 *
 * Declared in:
 *      arp.h
 */
void process_arp( Dev_list dev_entry, Pp_enetlist enet_entry, Mbuf mb)
{
	struct ether_header *etherhead;
	struct ether_arp*   arpmsg;
	u_int16             op;
	u_int8              ethernet_source_addr[6];

	/* Set up a pointer to the ethernet header    */
	etherhead = mtod(mb, struct ether_header *);

	/* Capture the source address of the packet */
	memcpy(ethernet_source_addr, etherhead->ether_shost, ETHERHARDSIZE);

	/* Set up a pointer to the ARP request/reply    */
	arpmsg = (struct ether_arp*)((u_int32)etherhead+(u_int32)ETHERHEADSIZE);
	/* Find out if we got a reply or a request    */
	op = ntohs(arpmsg->arp_op);
	switch (op) {

		case ARP_REQUEST:
			if(process_arp_request(dev_entry, enet_entry, mb)==FALSE)
				break;

			/* now add the ethernet header */
			add_enet(dev_entry, ethernet_source_addr, ETHERTYPE_ARP,
			                                               enet_entry, mb);

			/* send the data down to lower protocols */
			SMCALL_DNDATA(dev_entry,enet_entry->enet_dndrvr,mb);
			break;

		case ARP_REPLY:
			process_arp_reply(dev_entry, enet_entry, mb);
			break;

		case RARP_REQUEST:
			if(process_rarp_request(dev_entry,enet_entry, mb)==FALSE)
				break;

			/* now add the ethernet header */
			add_enet(dev_entry, ethernet_source_addr, ETHERTYPE_ARP,
			                                              enet_entry, mb);

			/* send the data down to lower protocols */
			SMCALL_DNDATA(dev_entry,enet_entry->enet_dndrvr,mb);
			break;

		case RARP_REPLY:
			process_rarp_reply(dev_entry, enet_entry, mb);
			break;

		default:
			m_free_p(mb);
			break;
	}
	return;
} /* End process_arp */

error_code
arpent_copy(struct arpstat* arpstat, struct arptab* arpcpy)
{
	error_code err = EOS_PNNF;
	Arptab arproot, arpent;

	arproot = &arpstat->arproot;
	arpstat->busy_flag = TRUE;

 	for (arpent = arproot->next; arpent != arproot; arpent = arpent->next) {
		if (arpcpy->at_iaddr.s_addr == arpent->at_iaddr.s_addr) {
			*arpcpy = *arpent;
			err = 0;
			break;
		}
	}
	arpstat->busy_flag = FALSE;
	return(err);
}

error_code
arptbl_copy(struct arpstat* arpstat, struct arptab* arpcpy, u_int32 *size_ptr)
{
	Arptab arproot, arpent;
	u_int32 size = *size_ptr;
	u_int32 tot_size = 0;

	arproot = &arpstat->arproot;
	arpstat->busy_flag = TRUE;

 	for (arpent = arproot->next; arpent != arproot; arpent = arpent->next) {
		tot_size += sizeof(struct arptab);
		if (size >= sizeof(struct arptab)) {
			*arpcpy++ = *arpent;
			size -= sizeof(struct arptab);
		}
	}
	/* indicate how bit the current arp table is */
	*size_ptr = tot_size;

	if (size >= sizeof(struct arptab))
		arpcpy->next = 0;
	arpstat->busy_flag = FALSE;
	return(SUCCESS);
}

struct arptab *arptab_look(Dev_list dev_entry, struct in_addr* destip)
{
	Arpstat arpstat = &((Spf_drstat)dev_entry->v_dr_stat)->arpstat;
	Arptab arproot;
	Arptab arpent;

	if (!arpstat) return 0;

	arpstat->busy_flag = TRUE;
	arproot = &arpstat->arproot;
 	for (arpent = arproot->next; arpent != arproot; arpent = arpent->next) {
		if (arpent->at_iaddr.s_addr == destip->s_addr) {
			if (arproot->next != arpent) {        /* if not at head of list */
				/* delete from current position */
				arpent->prev->next = arpent->next;
				arpent->next->prev = arpent->prev;
				/* move to head of list */
				arpent->prev = arproot;
				arpent->next = arproot->next;
				arproot->next->prev = arpent;
				arproot->next = arpent;
			}
			arpstat->busy_flag = FALSE;
			return arpent;
		}
	}
	arpstat->busy_flag = FALSE;
	return(NULL);
}

Arptab arptnew(Dev_list dev_entry, struct in_addr* destip)
{
	Arpstat arpstat = &((Spf_drstat)dev_entry->v_dr_stat)->arpstat;
	Arptab arproot;
	Arptab arpent;
	u_int32 size;

	if (!arpstat)
		return 0;

	arpstat->busy_flag = TRUE;
	arproot = &arpstat->arproot;

	/* get memory for new arp entry */
	size = sizeof(struct arptab);
	if (_os_srqmem(&size,(void**)&arpent,MEM_ANY) != SUCCESS) {
		arpstat->busy_flag = FALSE;
		return(NULL);
	}

	/* Zero out memory to be safe. */
	memset((void *)arpent, 0, size);

	/* link into list of arp entries */
	arpent->prev = arproot;
	arpent->next = arproot->next;
	arproot->next->prev = arpent;
	arproot->next = arpent;

	/* fill in data portions */
	arpent->at_grant = size;
	arpent->at_hold = 0;
	arpent->at_timer = arpent->at_flags = 0;
	arpent->at_iaddr.s_addr = destip->s_addr;
	arpstat->busy_flag= FALSE;
	return arpent;
}

/*
** Resolve an IP address into an ethernet address.  If success,
** desten is filled in.  If there is no entry in arptab,
** set one up and broadcast a request for the IP address.
** Hold onto this mbuf and resend it once the address
** is finally resolved.  A return value of 0(SUCCESS) indicates
** that desten has been filled in and the packet should be sent
** normally; a 1 return indicates that the packet has been
** taken over here, either now or for later transmission.
*/
Arptab arpresolve(Dev_list dev_entry, Pp_enetlist enet_entry, Mbuf mbp,
	          struct in_addr* destip, u_int8* desten)
{
	Spf_drstat drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Arptab at;

	/* see if the address is in the arp cache table */
	if ((at = arptab_look(dev_entry, destip)) == 0) {   /* not found */
		if ((at = arptnew(dev_entry, destip)) == 0) {    /* no memory? */
			m_free_p(mbp);            /* sorry... */
			return (NULL);
		}
		at->at_hold = mbp;
		at->at_asked = *drstat->timer_sec;
		send_arp_request(dev_entry, destip, enet_entry);
		return (NULL);
	}
	at->at_timer = 0;		/* restart the timer */
	if (at->at_flags & ATF_COM) {
		desten[0] = at->at_enaddr[0];
		desten[1] = at->at_enaddr[1];
		desten[2] = at->at_enaddr[2];
		desten[3] = at->at_enaddr[3];
		desten[4] = at->at_enaddr[4];
		desten[5] = at->at_enaddr[5];
		return (at);
	}
	/*
	** There is an arptab entry, but no ethernet address response yet.
	** Replace the held mbuf with this latest one. If drstat->timer_sec
	** has changed since we last asked, ask again.
	*/
	if (at->at_hold) {
		int		mbuf_count = 2;
		Mbuf	hold_mbp = at->at_hold;
		Mbuf	free_mbp;

		/* save up to 6 mbuf on q-chain */
		while (hold_mbp->m_qnext != NULL) {
			if (mbuf_count++ == 6) {
				free_mbp = at->at_hold; /* oldest mbuf */
				at->at_hold = at->at_hold->m_qnext; /* move to the next mbuf */
				free_mbp->m_qnext = NULL; /* terminate q-chain */
				m_free_p(free_mbp);
			}
			hold_mbp = hold_mbp->m_qnext;
		}
		hold_mbp->m_qnext = mbp; /* add the mbuf at the bottom of q-chain */
	} else {
		at->at_hold = mbp;
	}
	if (at->at_asked != *drstat->timer_sec) {
		at->at_asked = *drstat->timer_sec;
		send_arp_request(dev_entry, destip, enet_entry);
	}

	return (NULL);
}


/*
**	direct call from user to delete ARP table entry.  The structure passed
**	uses struct sockaddr to ease supporting other hardware/protocols.
*/
int arp_delete(Dev_list dev_entry, struct arpreq* arq)
{
	Spf_drstat drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Arptab at;

	/* accept only INET for now */
	if (arq->arp_pa.sa_family != AF_INET)
			return EAFNOSUPPORT;

	/* lookup the entry */
	if ((at = arptab_look(dev_entry,&satosin(&arq->arp_pa)->sin_addr)) == NULL)
		return E_PNNF;		/* not found */

	/*
	** If we are deleting the cached entry reset it.
	*/
	if (drstat->arpent_cache == at) {
		drstat->arpent_cache = &drstat->arpstat.arproot;
	}

	arp_entry_remove(dev_entry, at);
	return 0;
}

/*
** Remove arp entry
*/
void arp_entry_remove(Dev_list deventry, Arptab at)
{
	if (at->at_hold) {
		/* Free pending packet */
		m_free_q(&at->at_hold);
	}
	at->prev->next = at->next;
	at->next->prev = at->prev;

	/* Send routing socket notification */
	if (at->at_flags & ATF_COM) {
		/* We don't send a notification message if the entry was incomplete */
		rt_msg_send(deventry, at, RTM_DELETE);
	}

	_os_srtmem(at->at_grant, at);
	return;
}

/*
**	direct call from user to change/set/add ARP table entry.  The structure
**	passed uses struct sockaddr to ease supporting other hardware/protocols.
*/
int arp_change(Dev_list dev_entry, struct arpreq* arq)
{
	Spf_drstat drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Arptab at;

	/* accept only INET for now */
	if (arq->arp_pa.sa_family != AF_INET)
			return EAFNOSUPPORT;

	/* should check to see if we are connected to the target network */

	/* lookup the entry, create one if not found */
	if ((at = arptab_look(dev_entry, &satosin(&arq->arp_pa)->sin_addr)) == NULL)
		if ((at = arptnew(dev_entry, &satosin(&arq->arp_pa)->sin_addr)) == NULL)
			return EADDRNOTAVAIL;

	/*
	** If we are changing the cached entry reset it.
	*/
	if (drstat->arpent_cache == at) {
		drstat->arpent_cache = &drstat->arpstat.arproot;
	}

	/* copy in the hardware (ethernet) address */
	memcpy(at->at_enaddr, arq->arp_ha.sa_data, sizeof(at->at_enaddr));

	/* set the flags */
	at->at_flags = ATF_COM | (arq->arp_flags & (ATF_PERM|ATF_PUBL));
	at->at_timer = 0;

	/* Send routing socket notification */
	rt_msg_send(dev_entry, at, RTM_ADD);

	return 0;
}

void rt_msg_send(Dev_list deventry, Arptab at, u_int32 type)
{
	Spf_drstat drstat = (Spf_drstat)(deventry->v_dr_stat);
	struct rt_msghdr *rtmsghdr;
	struct sockaddr_dl *sdl;
	struct sockaddr_in *sin;
	spf_ss_pb spb;
	char *cp;

	MALLOC(rtmsghdr, sizeof(struct rt_msghdr) + sizeof(struct sockaddr) * RTAX_MAX);
	if (rtmsghdr == NULL){
		return;
	}

	bzero((caddr_t)rtmsghdr, sizeof(struct rt_msghdr) +
					sizeof(struct sockaddr) * RTAX_MAX);

	rtmsghdr->rtm_version = RTM_VERSION;
	rtmsghdr->rtm_type = type;
	rtmsghdr->rtm_seq = 1;
	rtmsghdr->rtm_flags = RTF_HOST|RTF_DONE|RTF_LLINFO;
	rtmsghdr->rtm_pid = 1;
	rtmsghdr->rtm_addrs = RTA_DST | RTA_GATEWAY;
	rtmsghdr->rtm_msglen = sizeof(struct rt_msghdr) + sizeof(struct sockaddr)
								+ sizeof(struct sockaddr_dl);
	cp = (char *)rtmsghdr + sizeof(struct rt_msghdr);

	sin = (struct sockaddr_in *)cp;
	sin->sin_len = sizeof (struct sockaddr_in);
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = at->at_iaddr.s_addr;

	cp = cp + sizeof(struct sockaddr);
	sdl = (struct sockaddr_dl *)cp;
	sdl->sdl_len = sizeof(struct sockaddr_dl);
	sdl->sdl_family = AF_LINK;
#if 0
	/* It would be nice to set this, but do we have this information? */
	sdl->sdl_index = ??;
#endif
	sdl->sdl_type = IFT_ETHER;
	sdl->sdl_alen = 6; /* Should be a define somewhere */
	bcopy(&at->at_enaddr, &sdl->sdl_data, 6); /* This 6 should be a define too */

	bzero((caddr_t)&spb, sizeof(spf_ss_pb));
	spb.code = IP_SS_RTMSG;
	spb.updir = SPB_GOINGUP;
	spb.param = rtmsghdr;
	SMCALL_SS(deventry, (drstat->spip_deventry), &spb);
	FREE(rtmsghdr, sizeof(struct rt_msghdr) + sizeof(struct sockaddr) * RTAX_MAX);
}
