/*
	@(#)settrap.h	1.1.3	6/24/91
*/
#if !defined(_SETTRAP_H)
#define _SETTRAP_H

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
!  01 88/10/28 created                                                 djl  !
|  02 98/07/29 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

/*
 * structure for settrap calls
 */
 
#if !defined(_TYPES_H)
#include <types.h>
#endif /* _TYPES_H */

typedef struct strap {
	u_int32		vector,					/* the vector number (not offset) */
				(*routine)();			/* the service routine */
} strap, *Strap;

#if defined(_OSK)
typedef struct os9strap
{
	u_int16	vector;	/* the vector number (not offset) */
	u_int16 offset;	/* the offset from the next entry to exception function */
} os9strap;
#endif /* _OSK */

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
error_code _os_strap(u_int32 *, strap *);
error_code _os9_strap(u_int32 *, os9strap *);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
error_code _os_strap();
error_code _os9_strap();
#endif

#endif /* _SETTRAP_H */
