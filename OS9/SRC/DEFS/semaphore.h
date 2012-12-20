#if !defined(_SEMAPHORE_H)
#define _SEMAPHORE_H

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
| Event Processing Routines.												|
|																			|
| Edition History															|
|  #   date   Comments                                          		by	|
| -- -------- ---------------------------------------------------------	---	|
| 01 91/08/26 Created.													afh |
| 02 92/09/27 removed keyword items (causes problems with pput).		wwb	|
| 03 92/12/08 Minor changes to the semaphore structure.					afh |
|              ---- OS-9//68K V2.5 (Discovery Pak) released ----			|
| 04 93/02/01 Reworked for std defs usage.								wwb	|
| 05 93/07/07 Added the "s_flags" field.								afh |
| 06 93/07/16 Made the first reserved field available to users			pcd |
| 07 93/07/21 Added the "s_sync" field.									afh |
| 08 93/07/27 Added function prototypes (for robb).						wwb	|
|              ---- OS-9/68K V3.0 released ----								|
| 09 98/07/29 Updated for new copyright info.							wwb	|
|				---- OS-9/68K V3.1 Release ----								|
|				$$			<RELEASE_INFO>			$$						|
|																			|
`--------------------------------------------------------------------------*/

#ifndef _TYPES_H
#include <types.h>
#endif

#ifndef _PROCID_H
#include <procid.h>
#endif

/* System call operations */
#define P_OPERATION 1 	/* Reserve semaphore */
#define V_OPERATION 2 	/* Release semaphore */

/* s_flags bit field definitions */
#define S_CASFLAG	0x00000001	/* cas instruction available */

/* Semaphore structure definition */
typedef struct semaphore  {
	u_int32
		s_value,		/* semaphore value (free/busy satus) */
		s_lock;			/* semaphore structure lock (use count) */
	procid
		*s_qnext,		/* wait queue for process descriptors */
		*s_qprev;		/* wait queue for process descriptors */
	u_int32
		s_length,		/* current length of wait queue */
		s_owner,		/* current owner of semphore */
		s_user,			/* reserved for users        */
		s_flags,		/* general purpose bit-field flags */
		s_sync,			/* integrity sync code */
		s_reserved[3];	/* reserved for system use */
} semaphore, *Semaphore;


#if defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
error_code _os_sema_init(semaphore *);
error_code _os_sema_p(semaphore *);   
error_code _os_sema_term(semaphore *); 
error_code _os_sema_v(semaphore *);   
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
error_code _os_sema_init();
error_code _os_sema_p();   
error_code _os_sema_term(); 
error_code _os_sema_v();   
#endif

#endif /* _SEMAPHORE_H */

