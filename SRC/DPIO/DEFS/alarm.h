/****************************************************************************
 *  ID:     @(#) alarm.h 1.2@(#)
 *  Date:   8/28/95
 ****************************************************************************
 *  																		*
 ****************************************************************************
 *  Copyright 1995 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date                      Comments                             By    *
 * --- -------- ------------------------------------------------- --------- *
 *  1  08/28/95	Put this stupid header in							dwj		*
 *  2  08/28/95	Added C++ support									dwj		*
 ****************************************************************************/


#if !defined(_ALARM_H)
#define _ALARM_H

#if defined(_ANSI_EXT) || defined(__STDC__)

#if !defined(_TYPES_H)
#include <types.h>
#endif

/*	C++ Support	*/
#ifdef __cplusplus
extern "C" {
#endif

error_code _os_alarm_atime(alarm_id *, signal_code, u_int32);
error_code _os_alarm_cycle(alarm_id *, signal_code, u_int32);
error_code _os_alarm_delete(alarm_id);
error_code _os_alarm_reset(alarm_id, signal_code, u_int32);
error_code _os_alarm_set(alarm_id *, signal_code, u_int32);
error_code _os_salarm_atime(alarm_id *, u_int32, u_int32, u_int32 (*)(), void *);
error_code _os_salarm_cycle(alarm_id *, u_int32, u_int32, u_int32 (*)(), void *);
error_code _os_salarm_delete(alarm_id);
error_code _os_salarm_reset(alarm_id *, u_int32, u_int32, u_int32 (*)(), void *);
error_code _os_salarm_set(alarm_id *, u_int32, u_int32, u_int32 (*)(), void *);

/*	End C++	*/
#ifdef __cplusplus
}
#endif

#endif

#endif
