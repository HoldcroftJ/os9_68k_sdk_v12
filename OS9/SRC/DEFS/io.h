/*
	I/O static storage definitions. This file created for OS-9000
	compatability.
*/
#if !defined(_IO_H)
#define _IO_H

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
|  02 01/03/02 Add const where appropriate                             mgh  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#if !defined(_SYSIO_H)
#include <sysio.h>
#endif /* _SYSIO_H */

typedef struct devicetbl dev_list, *Dev_list;

#if !defined(_PATH_H)
#include <path.h>
#endif /* _PATH_H */

typedef union pathdesc pd_com, *Pd_com;

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if !defined(_SG_CODES_H)
#include <sg_codes.h>
#endif /* _SG_CODES_H */

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
error_code _os_attach(const char *, u_int32, dev_list **);
error_code _os_detach(dev_list *);
error_code _os_getpd(path_id, pd_com **);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
error_code _os_attach();
error_code _os_detach();
error_code _os_getpd();
#endif

#endif /* _IO_H */
