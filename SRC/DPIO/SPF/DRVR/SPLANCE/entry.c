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
 *   0 04/26/95 Creation                                               amg
 *     02/27/96 modified for LANCE               Bill West Inc.        bw
 *   4 09/03/96 pop and push bug fix
 *     02/19/97 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *     05/05/97 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  10 10/23/97 Added/Uncommented BSD statistics keeping               tmd
 *              Added ITE_GETADDR & SPF_GS_SYMBOLS                     tmd
 *  11 01/08/98 Removed unused params dr_updata.
 *              dr_downdata, initialized var old_irq, removed
 *              unused var i.                                          jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  13 02/11/99 Set stk_int_flags in SPF_GS_UPDATE                     rds
 ****************************************************************************/


/*
 *	Header Files
 */
#include <SPLANCE/defs.h>

/* external function declarations */

/*----------------------------------------*/
/********************/
/* Type definitions */
/********************/

typedef struct  dev_pb  {
    u_int32     updrvr,
                deventry,
                dndrvr;
}DEV_PB,*Dev_pb;

static error_code check_access(process_id, u_int32, u_int32, void *);

/*-----------------------------------------*/

/***************************************************************************
 *	Device Driver Initialization entry point
 ***************************************************************************/
error_code dr_iniz(Dev_list dev_entry)
{
	Spf_lustat 	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Ifs 		ifstat = (Ifs)(&lustat->ifstat);
	error_code 	err;
#ifdef DEBUG
	u_int32 	*lxp;       
#endif

#if defined(DEBUG)
	if (debug_init(DEBUG,(Dbg_stat *)&lustat->lu_dbg,"dbg_spsin") != SUCCESS) {
		lustat->lu_dbg = NULL;
	}
	debug_data(lustat->lu_dbg,"MACdr_iniz ",0);
#endif


	/****************************
		Install the ISR
	 ****************************/
#if 0
	if ((err = _os9_irq(lustat->lu_vector, lustat->lu_priority, isr_os9,
									dev_entry,lustat->lu_port)) != SUCCESS)	{
        return(err);
	}
#else
	if ((err = _os_irq(lustat->lu_vector, lustat->lu_priority, isr_os9,
													dev_entry)) != SUCCESS) {
        return(err);
	}
#endif

#ifdef DEBUG
    	debug_string(lustat->lu_dbg,(u_char *)"MAC:HWaddr",(u_int32) sizeof("LANCE:HW addr"));
    	lxp=(u_int32 *)lustat->ether_addr;
    	debug_4data(lustat->lu_dbg,lxp[0],lxp[1],lxp[2],lxp[3]);
#endif


#ifdef notdef
/* should be initialized data */
	/****************************************************
		Init the MPEG UDP port so we are not
		reading in garbage.
	 ****************************************************/
	
	lustat->mpeg_udp_port = 0;
	lustat->scl_chain = NULL;
#endif

	/****************************************************
		Finish initializing if static
	 ****************************************************/

	ifstat->v_inq = ifstat->v_outq = (struct mbuf *) 0;

	/***************************************************
       	Ensure that no interrupts are received until
       	a path is opened to the device. (or else a
       	system state exception error will occur. The
       	lu_updrvr parameter in lustat is initialized by
       	SPF, but not until a path is opened. The value
       	stored in lu_updrvr is used to process the
       	received data. If you try to send data up before
       	this value is init'd, you will be writing to
       	someone elses memory (that's a BAD thing...right?)
       	The LANCE's interrupts will be turned on in the
       	SS_OPEN setstat (called every time a path open is
       	performed).
   	***************************************************/

	err = init_hdw(dev_entry);

	#ifdef DEBUG
	debug_string(lustat->lu_dbg,(u_char *)"MAC:STAT  ",(u_int32) sizeof("LANCE:STAT "));
	debug_data(lustat->lu_dbg,"lu_att_cnt",(u_int32) lustat->lu_att_cnt);
	debug_data(lustat->lu_dbg,"lu_use_cnt",(u_int32) lustat->lu_use_cnt);
	debug_data(lustat->lu_dbg,"lu_port   ",(u_int32) lustat->lu_port);
	debug_data(lustat->lu_dbg,"lu_num    ",(u_int32) lustat->lu_num);
	debug_data(lustat->lu_dbg,"lu_ioenabl",(u_int32) lustat->lu_ioenabled);
	debug_data(lustat->lu_dbg,"lu_txsize ",(u_int32) lustat->lu_txsize);
	debug_data(lustat->lu_dbg,"lu_txoffse",(u_int32) lustat->lu_txoffset);
	debug_data(lustat->lu_dbg,"lu_updrvr ",(u_int32) lustat->lu_updrvr);
	debug_data(lustat->lu_dbg,"lu_dndrvr ",(u_int32) lustat->lu_dndrvr);
	#endif

	return(err);
}	

/**************************************************************************
 *	Device Driver Termination entry point
 **************************************************************************/
error_code dr_term(Dev_list dev_entry)
{
	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Ifs		ifstat = (Ifs)(&lustat->ifstat);


	/* turn off all interrupts from the hardware */
	term_hdw( dev_entry);

	/* remove interrupt service routine */
	_os_irq(lustat->lu_vector,lustat->lu_priority,NULL,dev_entry);

	/* ensure that i/o queues are free of mbufs */
        m_free_q(&ifstat->v_inq);
        m_free_q(&ifstat->v_outq);

	#ifdef DEBUG
	    debug_data(lustat->lu_dbg,"HDW:dr_term ",0);
		debug_data(lustat->lu_dbg,"HDW:attcnt",(u_int32) lustat->lu_att_cnt);
	#endif
	return(SUCCESS);
}	

/*************************************************************************
 *  Device Driver Updata (Read) entry point !!! NOT USED !!!
 *************************************************************************/
error_code dr_updata()
{
	return(SUCCESS);
}

/*************************************************************************
 *	Device Driver Downdata (Write) entry point
 *************************************************************************/
error_code dr_downdata(Dev_list dev_entry, Mbuf mb)
{
	Spf_lustat	lustat = (Spf_lustat)(dev_entry->v_lu_stat);
	Ifs		ifstat = (Ifs)(&lustat->ifstat);
	status_reg 	old_irq = 0;
	error_code 	err = SUCCESS;
	int32 		status;
	int32 		plen;
	Mbuf		temp;

	/* sanity check */
    if (mb == NULL) 
		return 0;		/* Get a proper error code placed in here.	*/

	/* message too long for ethernet? */
    if ((plen = m_len_p(mb)) > ETHER_MAX) {
    	m_free_p(mb);                   /* sorry, drop the packet */
		lustat->if_oerrors++;
		return(EMSGSIZE);
    }

	/* message too short for ethernet? */
    if (plen < ETHER_MIN) {
    	Mbuf tmb;

    	if ((status = get_mbuf(dev_entry, ETHER_MIN - plen, &tmb))  == SUCCESS){
    		memset(mtod(tmb, char *), 0, tmb->m_size = ETHER_MIN - plen);
        	m_cat(mb, tmb);		/* add a zero padder to packet */
        	plen += tmb->m_size;
    	} else {
        	m_free_p(mb);		/* sorry, drop the packet */
			lustat->if_oerrors++;
        	return(ENOBUFS);
    	}
    }

	/*
    now scoop the mbuf chain into one contigous mbuf for xmit.
    This is only a temporary measure until the driver is changed to
    use the packet-chaining feature of the LANCE.  Scoop only if necessary,
    however.
	*/
	if (plen > mb->m_size) {
		mb = m_copy(temp = mb, 0, plen, &status);
		m_free_p(temp);
		if (mb == NULL) {
			lustat->if_oerrors++;
			/* seta6(olda6); */
			return(ENOBUFS);
		}
	}

	/* If broadcast or multicast, inc counter.  Not a great place to
	   do this since may get xmit error, but it'll do. */
	if ((mb->m_flags & M_BCAST) || (mb->m_flags & M_MCAST))
		lustat->if_omcasts++;

	/* now give mbuf data to LANCE */
	if (lustat->lu_irqmask != 0) {
	        old_irq = irq_change((status_reg)lustat->lu_irqmask);
	}

	#ifdef DEBUG
	    debug_data(lustat->lu_dbg, "LE  LoadTm", (u_int32)mb);
	#endif
	
	if( load_tmd(ifstat, mb) == 0) {
                m_enq(&ifstat->v_outq, mb);
                ifstat->v_lestat.err_xenq++;
        }
	if (lustat->lu_irqmask != 0) {
	        irq_restore(old_irq);
	}

	ifstat->v_lestat.xmit_trys++;

    status = 0;
    return(err);

}


/************************************************************************
 *  Device Driver GetStat entry point
 ************************************************************************/
error_code dr_getstat(Dev_list dev_entry, Spf_ss_pb spb)
{
	Spf_lustat lustat = (Spf_lustat)(dev_entry->v_lu_stat);

	switch (spb->code) {
	
        case SPF_SS_UPDATE: {
			Spf_update_pb upb = (Spf_update_pb)spb;

			/* Since I'm always lowest on the stack, I just fill my
             * variables and return.
             */
            upb->stk_txsize = lustat->lu_txsize;
            upb->stk_txoffset = lustat->lu_txoffset;
            upb->stk_ioenabled = lustat->lu_ioenabled;
			upb->stk_int_flags = IFF_BROADCAST;

			return(SUCCESS);
        }

		case SPF_GS_ETHADDR: {
			u_int8	*temp_ptr;

			temp_ptr = (u_int8 *)spb->param;

			memcpy(temp_ptr, lustat->ether_addr, 6);

			return(SUCCESS);
		}
 
		case ITE_GETADDR: {
			/* The format of the ite_conn_pb passed in is as follows:
			 *      code = ITE_GETADDR
			 *      size = sizeof ite_conn_pb
			 *      cctl_pb = 0
			 *      notify_pb = 0
			 *      our_num = address of user supplied Addr_type struct
			 *      their_num = address of user supplied Addr_type struct
			 */

			error_code err;
			Ite_conn_pb conn_ptr = (Ite_conn_pb) spb;
			Pd_com path_desc = &lustat->lu_pathdesc->pd_common;

			if (conn_ptr->our_num) {
				if (((path_desc->pd_state & PD_SYSTATE) == 0) &&
				   (err = check_access(lustat->LU_CURRPROC, sizeof(addr_type),
				                       S_IWRITE, conn_ptr->our_num)))
					return(err);
				*conn_ptr->our_num = lustat->lu_item_addr;
			}

			if (conn_ptr->their_num) {
				if (((path_desc->pd_state & PD_SYSTATE) == 0) &&
				   (err = check_access(lustat->LU_CURRPROC, sizeof(addr_type),
				                       S_IWRITE, conn_ptr->their_num)))
					return(err);
				conn_ptr->their_num->addr_class = ITE_ADCL_NONE;
				conn_ptr->their_num->addr_subclass = ITE_ADSUB_NONE;
			}
			return(SUCCESS);
		}
 
		case SPF_GS_SYMBOLS: {
			/* The format of the spf_ss_pb passed in is as follows:
			 *      code = SPF_GS_SYMBOLS
			 *      size = sizeof nlist *
			 *      param = nlist * (ptr to array to be set by driver)
			 */
			struct nlist *their_nl;
			struct nlist *our_nl;

#if !defined(HAS_NO_CHECKS)
			{
				u_int32 real_size = 0;
				error_code err;
				Pd_com path_desc = &lustat->lu_pathdesc->pd_common;
 
				/* walk their request list, and count how many there are */
				for (their_nl = (struct nlist *)spb->param;
					 their_nl->n_name; their_nl++)
					real_size++;

				real_size *= sizeof(*their_nl);

				if (((path_desc->pd_state & PD_SYSTATE) == 0) &&
				   (err = check_access(lustat->LU_CURRPROC, real_size,
				                       S_IREAD|S_IWRITE, spb->param)))
					return(err);
			}
#endif /* !defined(HAS_NO_CHECKS) */

			/* walk through all of their symbols */
			for (their_nl = (struct nlist *)spb->param;
				 their_nl->n_name; their_nl++) {

				/* walk through all of our symbols */
				for (our_nl = lustat->lance_symbols; our_nl->n_type; our_nl++) {                    if ((strcmp(our_nl->n_name, their_nl->n_name) == 0) &&
					   (their_nl->n_type == 0)) {
						   their_nl->n_value = our_nl->n_value;
						   their_nl->n_type = N_ABS;
					}
				}
			}     

			return(SUCCESS);
		}
		
		default: {
    		return(EOS_UNKSVC);
		}
	
	}
} /* dr_getstat */


/***********************************************************************
 *  Device Driver SetStat entry point
 ***********************************************************************/
error_code dr_setstat(Dev_list dev_entry, Spf_ss_pb ss_pb)
{
    Spf_drstat     drstat = (Spf_drstat)(dev_entry->v_dr_stat);
    Spf_lustat     lustat = (Spf_lustat)(dev_entry->v_lu_stat);

	#ifdef DEBUG
        debug_string(lustat->lu_dbg,(u_char *)"MACdr_set ",(u_int32) sizeof("LANCE:SET "));
        debug_data(lustat->lu_dbg,"MAC:code  ",(u_int32) ss_pb->code);
	#endif

    switch (ss_pb->code) {

		case SPF_SS_OPEN: {       /* hook to new upper/lower protocols    */

			#if defined(DEBUG)
				debug_data(lustat->lu_dbg, "MAC:SsOpen",ss_pb->code);
				debug_data(lustat->lu_dbg, "MAC:attcnt",(u_int32) lustat->lu_att_cnt);
			#endif

			#ifdef WHYNOT
				if(drstat->dr_use_cnt == 1)
					enable_lance(lustat);
			#endif

			return(SUCCESS);
		}

		case SPF_SS_HWEN: {

			#if defined(DEBUG)
			    debug_data(lustat->lu_dbg, "LANCEnabled",ss_pb->code);
				debug_data(lustat->lu_dbg, "MAC:attcnt",(u_int32) lustat->lu_att_cnt);
			#endif

			/* Enable the driver for I/O    */
			enable_lance(lustat);

			#if defined(DEBUG)
			    debug_data(lustat->lu_dbg, "LANCE:EnabOk",ss_pb->code);
			#endif

			return(SUCCESS);
		}

		case SPF_SS_CLOSE: {      /* unhook from upper/lower protocols    */

			#if defined(DEBUG)
				debug_data(lustat->lu_dbg, "MACSsClose",ss_pb->code);
			#endif

			if(drstat->dr_use_cnt == 0)
				disable_lance(lustat);

			return(SUCCESS);
		}

		case SPF_SS_HWDIS: {

			#if defined(DEBUG)
			    debug_data(lustat->lu_dbg, "LANCDisabled",ss_pb->code);
				debug_data(lustat->lu_dbg, "MAC:attcnt",(u_int32) lustat->lu_att_cnt);
			#endif

			/* Enable the driver for I/O    */
			disable_lance(lustat);

			#if defined(DEBUG)
			    debug_data(lustat->lu_dbg, "LANCE:DisabOk",ss_pb->code);
			#endif

			return(SUCCESS);
		}

#if 0 /* old spf 2.0 stuff */

		case SPF_SS_NEWTOP:     /* push/pop upper protocol  */
        {
            spf_ss_pb newpb;

            /* hook to new upper protocol */
            lustat->lu_updrvr = ss_pb->param;

            /* tell upper protocol we're ready for I/O */
            newpb.code = SPF_SS_UPDATE;
            newpb.param = dev_entry;
            newpb.updir = 1;
            return(SMCALL_SS(dev_entry, lustat->lu_updrvr, &newpb));
        }
#endif

		case SPF_SS_PUSH:	{
			#if defined(DEBUG)
			    debug_data(lustat->lu_dbg, "MAC:SsPush",ss_pb->code);
			#endif

			if (lustat->lu_ether_drvr == NULL) {
				lustat->lu_ether_drvr = lustat->lu_updrvr;
			}

			return(SUCCESS);
        }

        case SPF_SS_POP: {  /* push/pop the protocol above this drive   */
			#if defined(DEBUG)
			    debug_data(lustat->lu_dbg,"MAC:SsPop ",ss_pb->code);
			#endif
            return(SUCCESS);
        }

		case SS_LUOPT: {		/* set logical unit options         */ 

            /* update hardware here */

            return(SUCCESS);
        }

        case SS_READSTREAM: {
			Ss_MPEG_pb	Mpeg_params;

			Mpeg_params = (Ss_MPEG_pb)ss_pb->param;

         	lustat->scl_chain = Mpeg_params->scl_chain;
           	lustat->mpeg_udp_port = Mpeg_params->mpeg_udp_port;

            return(SUCCESS);
        }

        default: {
        	return(EOS_UNKSVC);
        }

	} /* switch */
}/* dr_setstat */

error_code enable_lance(Spf_lustat lustat)
{
	/* Enable the driver for I/O    */
	lustat->lu_ioenabled = DRVR_IOEN;
	return(SUCCESS);
}

error_code disable_lance(Spf_lustat lustat)
{
	/* Disable the driver for I/O    */
	lustat->lu_ioenabled = DRVR_IODIS;
	return(SUCCESS);
}

/* routine to see if user can read/write memory location */

static error_code
check_access(process_id proc_id, u_int32 size, u_int32 mode, void *where)
{
#if !defined(HAS_NO_CHECKS)
	pr_desc *pdesc;
	error_code err;

	if (err = _os_findpd(proc_id, &pdesc))
		return(err);

	if (err = _os_chkmem(size, mode, where, pdesc)) {
		return(err);
	}

#endif /* !defined(HAS_NO_CHECKS) */

	return(0);
}
