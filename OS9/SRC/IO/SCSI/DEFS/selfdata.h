/*
 * definitions for SCSI self data module structure
 *
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 * history:
 *
 * ed.  date   reason.											who.
 * -- -------  ------------------------------------------------	---
 * 00 90/08/31 Pfoof!  It exists!								jal
 *				---- OS-9/68K V3.0 Release ----
 *				---- OS-9/68K V3.1 Release ----
 */


/*------------------------------------------------------!
!  definitions for the SCSI self data module structure	!
!------------------------------------------------------*/
typedef struct	selfdata {
	u_char	sd_selfid,		/* this systems SCSI self ID	*/
			rsvd[63];		/* reserved for other stuff		*/
} sdata, *Sdata;
