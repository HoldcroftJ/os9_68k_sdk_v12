/****************************************************************************
 *  $Workfile:   bootpsave.h  $
 *  $Revision:   1.5  $
 *  $Modtime:   19 Feb 1997 21:29:00  $
 *  $Date:   19 Feb 1997 21:45:44  $
 ****************************************************************************
 *	Copyright 1993 by Microware Systems Corporation							*
 *	Reproduced Under License												*
 *																			*
 *	This source code is the proprietary confidential property of			*
 *	Microware Systems Corporation, and is provided to licensee				*
 *	solely for documentation and educational purposes. Reproduction,		*
 *	publication, or distribution in any form to any party other than		*
 *	the licensee is strictly prohibited.									*
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date   	Comments                                       	   By	*
 *  --- --------	-----------------------------------------------	------- *
 *	  0 ??/??/??	Birth											    ???	*
 *		09/16/93	<***** ISP 2.0 Release **************************>		*
 *		01/17/96	<***** ISP 2.1 Release **************************>		*
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ***********>      *
 ****************************************************************************/

#ifndef _BOOTP_H
#define _BOOTP_H

/*
 * A pointer to this struct will be available in D_Bootp when the
 * kernel is up.  Then some ifdev driver can use this to set it's
 * IP address to this one that bootp discovered.
 */
struct bootp_save {
	u_int32 magic1, magic2;	/* some magic numbers to ID this struct */
	u_int32 myip;			/* IP address used by BOOTP */
	u_int32 bootsize;		/* size of bootfile we read */
	struct bootp *bprp;		/* ptr to BOOTP server response message */
	Ifdev ifdev;			/* the device we used to boot */
};

#define BOOTP_MAGIC		"GOTBOOTP"
#define D_BootP			0x7a8

#if 0
/*
 * for reference, bootp drivers do this:
 */
	if (error == SUCCESS) {
		memcpy(&save_bootp.magic1, "GOTBOOTP", 8);
		memcpy(&save_bootp.myip, &bprp.bp_yiaddr, 4);
		save_bootp.bprp = &bprp;
		save_bootp.ifdev = ifdev;
		save_bootp.bootsize = bootsize;
		stash_ipaddr(&save_bootp);
	}

D_BootP equ 0x7a8				/* defined in 2.4 kernel */
_asm("stash_ipaddr: movem.l a0,-(sp)");
_asm(" move.l VectTbl,a0 get system global pointer");
_asm(" move.l d0,D_BootP(a0) save into ioglobs: kernel doesn't clear these (whew!)");
_asm(" movem.l (sp)+,a0");
_asm(" rts");
#endif


#endif	/* _BOOTP_H */
