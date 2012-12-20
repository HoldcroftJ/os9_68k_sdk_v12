/* dcmd.h - direct command definitions */
#if !defined(_DCMD_H)
#define _DCMD_H

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 98/07/29 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

/* this is the structure that is passed to the getstat and putstat
 * routines.
 */
typedef struct d_cmd { 
	int  dcmdsync,		/* sink code - must be DCMDSYNC */
	     manager,		/* file manager code,  pd_dtype from _gs_opts() */
	     device;		/* device type code */
	void *dcmdblk;		/* device driver specific command */
} *Dcmd;

/* 
 *  General definitions
 */
 
#define DCMDSYNC   0xCA7CA11		/* this code validates the command block */

/* device type codes:
 *    These are specific to the devices.  The drivers can use this
 *    field to validate that the call that arrived is for the device
 *    that they control.
 */
#define SCSIdevice 0x5C51			/* SCSI bus device */
#define SLIPdevice 0x534C4950		/* SLIP device */

#endif /* _DCMD_H */
