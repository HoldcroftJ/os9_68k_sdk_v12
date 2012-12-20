/* OS-9000 compatible process descriptor definitions */
#if !defined(_PROCESS_H)
#define _PROCESS_H

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


/* The pointer definitions herein are generally (unsigned char *) */

#if !defined(_PR_DESC)
#define _PR_DESC
typedef struct pdsc pr_desc, *Pr_desc;
#endif /* _PR_DESC */

#if !defined(_PROCID_H)
#include <procid.h>
#endif

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#define _CONST const
#else
#define _OP(x) ()
#define _CONST
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

error_code _os_chain _OP((u_int32, u_int32, _CONST char *, _CONST void *, u_int32, u_int32, u_int32));
error_code _os_cpymem _OP((process_id, void *, void *, u_int32));
error_code _os_exec _OP((u_int32 (*)(), u_int32, u_int32, void *, char **, char **, u_int32 , ...));
error_code _os_exit _OP((status_code));
error_code _os_fork _OP((u_int32, u_int32, _CONST char *, _CONST void *, u_int32, u_int32, process_id *, u_int32, u_int32));
error_code _os_get_prtbl _OP((void *, u_int32 *));
error_code _os_gprdsc _OP((process_id, void *, u_int32 *));
error_code _os_id _OP((process_id *, u_int16 *, u_int16 *, int32 *, u_int16 *, u_int16 *));
error_code _os_permit _OP((void *, u_int32, u_int32, process_id));
error_code _os_protect _OP((void *, u_int32, u_int32, process_id));
error_code _os_setpr _OP((process_id, u_int32));
error_code _os_setuid _OP((owner_id));
error_code _os_suspend _OP((process_id));
error_code _os_sysdbg _OP((void *, void *));
error_code _os_uacct _OP((u_int32, pr_desc *));
error_code _os_wait _OP((process_id *, status_code *));
error_code _os9_gspump _OP((process_id, u_int32, void *));
error_code _os9_id _OP((process_id *, u_int16 *, u_int16 *, u_int16 *));
error_code _os_alltsk _OP((pr_desc *));
error_code _os_alocproc _OP((process_id *, pr_desc **));
error_code _os_aproc _OP((process_id));
error_code _os_chkmem _OP((u_int32, u_int32, void *, pr_desc *));
error_code _os_deltsk _OP((pr_desc *));
error_code _os_findpd _OP((process_id, pr_desc **));
error_code _os_nproc _OP((void));
error_code _os_rtnprc _OP((process_id));
error_code _os9_allpd _OP((void *, u_int16 *, void **));
error_code _os9_aproc _OP((pr_desc *));
error_code _os9_findpd _OP((u_int32, void **, void *));
error_code _os9_ioqueue _OP((process_id));
error_code _os9_panic _OP((u_int32));
error_code _os9_retpd _OP((u_int32, void *));

#if defined(_OPT_PROTOS)
int chainc _OP((_CONST char *, int, _CONST char *, int, int, int, int, int));
int chain _OP((_CONST char *, int, _CONST char *, int, int, int, int));
int _cpymem _OP((int, int, void *, void *));
int getpid _OP((void));
int _get_process_desc _OP((int, int, pr_desc *));
int _get_process_table _OP((void *, int));
int getuid _OP((void));
int os9exec _OP((int (*)(), _CONST char *, char **, char **, unsigned int, int, int));
int os9fork _OP((_CONST char *, int, _CONST char *, int, int, int, int));
int os9forkc _OP((_CONST char *, int, _CONST char *, int, int, int, int, int));
int setpr _OP((int, int));
int setuid _OP((int));
int _sysdbg _OP((void));
int wait _OP((unsigned int *));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _PROCESS_H */
