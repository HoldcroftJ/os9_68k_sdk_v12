/*
 * $Header:   /h0/MWOS/SRC/DEFS/IO/SCSI/VCS/cmdblk.h_v   1.11   06 May 1998 12:09:58   allan  $
 * $Revision:   1.11  $
 */
/*
 * definitions for scsi command block passed to physical driver
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|           Copyright 1989-1996 by Microware Systems Corporation            |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
|                                                                           |
| ...OS-9 related...                                                        |
|                                                                           |
| 01 89/05/15 added TRANS direction defs for _f_trans.                wwb   |
| 02 90/04/10 repaired typo on errptr entry                           Rwb   |
|				---- OS-9/68K V3.0 Release ----                             |
|				---- OS-9/68K V3.1 Release ----                             |
|                                                                           |
| ...OS9000 related...                                                      |
|                                                                           |
| 01 96/08/03 Added edition history.                                  rak   |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/
 
#ifndef _types
#include <types.h>
#endif
 
typedef struct cmdblk {
	void		*_cb_cmdptr;		/* pointer to the scsi command */
	void		*_cb_datptr;		/* pointer to the data area */
	void		*_cb_errptr;		/* pointer to the error data area */
	u_int32		_cb_cmdlen;			/* length of the command block */
	u_int32		_cb_datlen;			/* expected data length */
	u_int32		_cb_errlen;			/* expected error length */
	u_int32		_cb_scsi_id;		/* the target id */
	u_char		_cb_pd_lun;			/* the target lun */
	u_char		_cb_xfer;			/* data input/output flag */
#define		INPUT		0
#define		OUTPUT		1
} *Cmdblk;

/*
 * _f_trans direction definitions
 */
#define	TRANS_LOCAL		0			/* translate local to external address */
#define	TRANS_EXTERN	1			/* translate external to local address */


