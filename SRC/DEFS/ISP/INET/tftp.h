/****************************************************************************
 *  $Workfile:   tftp.h  $
 *  $Revision:   1.6  $
 *  $Modtime:   17 Feb 1998 10:20:00  $
 *  $Date:   17 Feb 1998 10:25:34  $
 ****************************************************************************
 *	Trivial File Transfer Protocol (IEN-133)								*
 ****************************************************************************
 * Copyright (c) 1983 Regents of the University of California.				*
 * All rights reserved.														*
 *																			*
 * Redistribution and use in source and binary forms are permitted			*
 * provided that the above copyright notice and this paragraph are			*
 * duplicated in all such forms and that any documentation,					*
 * advertising materials, and other materials related to such				*
 * distribution and use acknowledge that the software was developed			*
 * by the University of California, Berkeley.  The name of the				*
 * University may not be used to endorse or promote products derived		*
 * from this software without specific prior written permission.			*
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR			*
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED			*
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.		*
 *																			*
 *	@(#)tftp.h	5.2 (Berkeley) 6/27/88										*
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
 *      09/16/93	<***** ISP 2.0 Release ***********************>			*
 *      01/17/96	<***** ISP 2.1 Release ***********************>			*
 *      02/19/97    <***** SPF LAN Comm Pack v3.0 Release ********>         *
 *                  ---- OS-9000/Brutus Board Support V1.0 Released ----    *
 ****************************************************************************/

#define	SEGSIZE		512		/* data segment size */

/*
 * Packet types.
 */
#define	TFTP_RRQ	01			/* read request */
#define	TFTP_WRQ	02			/* write request */
#define	TFTP_DATA	03			/* data packet */
#define	TFTP_ACK	04			/* acknowledgement */
#define	TFTP_ERROR	05			/* error code */

struct	tftphdr {
	short	th_opcode;		/* packet type */
	union {
		short	tu_block;	/* block # */
		short	tu_code;	/* error code */
		char	tu_stuff[1];	/* request packet stuff */
	} th_u;
	char	th_data[1];		/* data or error string */
};

#define	th_block	th_u.tu_block
#define	th_code		th_u.tu_code
#define	th_stuff	th_u.tu_stuff
#define	th_msg		th_data

/*
 * Error codes.
 */
#define	EUNDEF		0		/* not defined */
#define	ENOTFOUND	1		/* file not found */
#define	EACCESS		2		/* access violation */
#define	ENOSPACE	3		/* disk full or allocation exceeded */
#define	EBADOP		4		/* illegal TFTP operation */
#define	EBADID		5		/* unknown transfer ID */
#define	EEXISTS		6		/* file already exists */
#define	ENOUSER		7		/* no such user */
