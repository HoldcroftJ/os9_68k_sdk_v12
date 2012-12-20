#if !defined(_ALARM_H)
#define _ALARM_H

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
|  02 01/01/30 Added _os_salarm_delete_sp prototype                    mgh	|
|				---- OS-9/68K V3.1 Release ----								|
|  03 01/01/30 Add TH_SPOWN definition to allow the user to delete     mgh  |
|                system process alarms via os_salarm_delete_sp				|
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#if !defined(_REG_H)
#include <MACHINE/reg.h>
#endif
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

error_code _os_alarm_atime _OP((alarm_id *, signal_code, u_int32));
error_code _os_alarm_cycle _OP((alarm_id *, signal_code, u_int32));
error_code _os_alarm_delete _OP((alarm_id));
error_code _os_alarm_set _OP((alarm_id *, signal_code, u_int32));
error_code _os9_alarm_atdate _OP((alarm_id *, signal_code, u_int32, u_int32));
error_code _os9_alarm_atjul _OP((alarm_id *, signal_code, u_int32, u_int32));

error_code _os_salarm_delete _OP((alarm_id));
error_code _os_salarm_delete_sp _OP((alarm_id, u_int32));
error_code _os9_salarm_atdate _OP((alarm_id *, u_int32, u_int32, REGISTERS *));
error_code _os9_salarm_atjul _OP((alarm_id *, u_int32, u_int32, REGISTERS *));
error_code _os9_salarm_cycle _OP((alarm_id *, u_int32, REGISTERS *));
error_code _os9_salarm_set _OP((alarm_id *, u_int32, REGISTERS *));

#if defined(_OPT_PROTOS)
int alm_atdate _OP((int, int, int));
int alm_atjul _OP((int, int, int));
int alm_cycle _OP((int, int));
int alm_delete _OP((int));
int alm_set _OP((int, int));
#endif

/* definition of flags for use in parameters to _os_salarm_xxx calls */
/*   for compatibility with OS9000 Bit definitions */
#define B_TH_DELPB 0 			/* Bit number for TH_DELPB reserved flag */
#define B_TH_SPOWN 1 			/* Bit number for TH_SPOWN 	 */
#define TH_DELPB (1<<B_TH_DELPB) /* Reserved for compatibility */
#define TH_SPOWN (1<<B_TH_SPOWN) /* system process owns the thread block */
 

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif
