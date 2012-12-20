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
 *   1 01/12/96 LANPACK                                                bnr
 *   2 02/12/96 updated according the code review                      tm
 *  16 13/01/97 Set the M_BCAST flag, for ethernet broadcast packets.  sr
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *  23 04/27/97 BSD headers use ETHERTYPE_REVARP, not ETHERTYPE_RARP   rds
 *  24 04/28/97 Split part of defs.h into bsddefs.h                    rds
 *  27 05/05/97 Fix endian problems in ethernet.c                      rds
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
 *  31 02/03/98 Removed spsin.h, removed compiler warnings             rds
 *  32 04/01/98 Cleaned up code.                                       sr
 *     04/01/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  33 09/25/98 Set M_MCAST flag when receiving multicast packets      rds
 *     09/25/98 Don't drop packet if it doesn't have our hardware addr rds
 *  37 11/22/99 Added faster check for broadcast/multicast packets     rds
 *  39 10/19/00 Fix-up word/longword alignment                         taka
 *  40 10/31/00 Removed setting of packet size based on IP header      rds
 ****************************************************************************/

#include "bsddefs.h"

/****************************************************/
/*        Process the Ethernet header               */
/*                                                  */
/****************************************************/
Mbuf process_enet(Dev_list dev_entry, Pp_enetlist enet_entry, Mbuf mb)
{
#ifdef DEBUG
	Spf_lustat      lustat = (Spf_lustat)(dev_entry->v_lu_stat);
#endif
	Spf_drstat      drstat = (Spf_drstat)(dev_entry->v_dr_stat);
	Ether_header	etherhead;

	etherhead = mtod(mb, Ether_header);

	switch(ntohs(etherhead->ether_type))
	{
		case ETHERTYPE_IP:
		{
			drstat->ip_count++;

			DEBUG_DATA(lustat->lu_dbg, "AP  IPtype", ETHERTYPE_IP);

			/*
			** If the received packet is a broadcast or multicast, set the
			** appropriate bit in the mbuf header. Prior to edition 33
			** we also dropped packets that did not match our hardware
			** address. Under normal situations that should never happen
			** and in case we ever hope to support promiscuous mode I
			** have removed the check. RDS
			*/
			if (etherhead->ether_dhost[0] & 0x1) {
				if ( (((u_int16 *)etherhead->ether_dhost)[0] == 0xffff) &&
						(((u_int16 *)etherhead->ether_dhost)[1] == 0xffff) &&
						(((u_int16 *)etherhead->ether_dhost)[2] == 0xffff)) {
					mb->m_flags |= M_BCAST;
				} else {
					mb->m_flags |= M_MCAST;
				}
			}

			/*
			** Remove the ethernet header
			**
			** We used to also strip off any Ethernet padding based on
			** indicated packet size in the IP header. This is also
			** done in spip so no reason to duplicate the code here.
			*/
			mb->m_offset += sizeof(struct ether_header);
			mb->m_size -= sizeof(struct ether_header);

#if defined(ALIGN_MBUF)
			if (mb->m_offset & 0x3) {
				/*
				** We need to make sure the IP header in the mbuf is 4
				** byte aligned. If the mbuf contains less than MAXCOPY
				** bytes of data we will shift the entire packet forward
				** 2 bytes to align. Otherwise just copy COPYSIZE bytes
				** which should give us all the protocol headers. If we
				** implement IP tunneling and use IP or TCP options this
				** may not be enough, but protocols above us will worry
				** about that.
				*/
				int32 status;
				char *ptr;
				Mbuf new_mb;
				u_int32 *old_data, *new_data;

				if (mb->m_size < MAXCOPY) {
					ptr = mtod(mb, char *);
					memmove(ptr - 2, ptr, mb->m_size);
					mb->m_offset -= 2;
				} else {
					if ((new_mb = m_getn(COPYSIZE + 2 * sizeof(Dev_list), &status)) == NULL) {
						m_free_p(mb);
						return(NULL);
					}
					/* copy the device lists from the original to the new mbuf */
					old_data = (u_int32 *)mb + MBHDR_SZ;
					new_data = mtod(new_mb, u_int32 *);
					*new_data++ = *old_data++;
					*new_data = *old_data;
					new_mb->m_offset += 2 * sizeof(Dev_list);
					/* copy COPYSIZE bytes of data from original to the new mbuf */
					new_mb->m_size = COPYSIZE;
					m_move(mb, 0, COPYSIZE, mtod(new_mb, char *));
					mb->m_offset += COPYSIZE;
					mb->m_size -= COPYSIZE;
					/* chain it up */
					new_mb->m_pnext = mb;
					return (new_mb);
				}

			}
#endif /* ALIGN_MBUF */

			return(mb);
		}

		case ETHERTYPE_ARP:
		{
#if defined(ALIGN_MBUF)
			char *ptr;
			if (!(mb->m_offset & 0x3)) {
				ptr = mtod(mb, char *);
				memmove(ptr + 2, ptr,
						sizeof(struct ether_header) + sizeof(struct ether_arp));
				mb->m_offset += 2;
			}
#endif /* ALIGN_MBUF */
			drstat->arp_count++;
			DEBUG_DATA(lustat->lu_dbg, "AP  msg   ", (u_int32) drstat->arp_count);
			process_arp(dev_entry, enet_entry, mb);
			return(NULL);
		}

		case ETHERTYPE_REVARP:
		{
#if defined(ALIGN_MBUF)
			char *ptr;
			if (!(mb->m_offset & 0x3)) {
				ptr = mtod(mb, char *);
				memmove(ptr + 2, ptr,
						sizeof(struct ether_header) + sizeof(struct ether_arp));
				mb->m_offset += 2;
			}
#endif /* ALIGN_MBUF */
			drstat->rarp_count++;
			DEBUG_DATA(lustat->lu_dbg, "AP  RevArp", 0);
			process_arp(dev_entry, enet_entry, mb);
			return(NULL);
		}

		default:
		{
			/* unsupported ethertype value */
			DEBUG_DATA(lustat->lu_dbg, "AP  unsupt",
								(u_int32)(ntohs(etherhead->ether_type)));
			drstat->unknown++;
			m_free_p(mb);
			return(NULL);
		}

	}

} /* process_enet */

/****************************************************/
/*           Add the Ethernet header                */
/*                                                  */
/****************************************************/
error_code add_enet(Dev_list dev_entry,
					u_int8      *destination_addr,
					u_int16     type,
					Pp_enetlist enet_entry,
					Mbuf        mb)
{
#ifdef DEBUG
	Spf_lustat      lustat = (Spf_lustat)(dev_entry->v_lu_stat);
#endif
	Ether_header    etherhead;
	int             i;
	error_code      err = SUCCESS;

	mb->m_size   += sizeof(struct ether_header);
	mb->m_offset -= sizeof(struct ether_header);

	etherhead = mtod(mb,Ether_header);

	for(i=0;i<6;i++)
		etherhead->ether_dhost[i] = destination_addr[i];

	for(i=0;i<6;i++)
		etherhead->ether_shost[i] = enet_entry->ether_addr[i];

	etherhead->ether_type = htons(type);

	#ifdef DEBUG
	{
		u_int32		*lxp, *lyp;

		lxp = (u_int32 *)etherhead->ether_dhost;
		lyp = (u_int32 *)etherhead->ether_shost;
    	debug_data(lustat->lu_dbg, "AP  EthAddr", 0);
	    debug_4data(lustat->lu_dbg, lxp[0], lxp[1], lyp[0], lyp[1]);
	}
	#endif

	return(err);
}
