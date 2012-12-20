/*
	OS-9 Event and Semaphores

*/
#if !defined(_EVENTS_H)
#define _EVENTS_H

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
|  02 98/08/25 added include of process.h so that pr_desc can be       wwb	|
|              defined by that file (and not this one).						|
|				---- OS-9/68K V3.1 Release ----								|
|  03 01/03/02 Add const where appropriate							   mgh  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#if !defined(_PR_DESC)
#include <process.h>
#endif

#define EV_SIZE		32			/* event entry size */
#define EV_ALLPROCS	0x8000		/* activate all processes in range */

typedef struct _evstr {
	unsigned short _ev_eid;		/* event id number */
	char _ev_name[12];			/* event name */
	int _ev_value;				/* current event value */
	short _ev_winc;				/* wait increment value */
	short _ev_sinc;				/* signal increment value */
	unsigned short _ev_link;	/* event use count */
	pr_desc *queN;				/* next event in queue */
	pr_desc *queP;				/* previous event in queue */
} event;

/* typedef for OS-9000 compatability */
typedef struct _evstr ev_infostr, *Ev_infostr;

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

error_code _os_ev_creat _OP((int32, int32, u_int32, event_id *, _CONST char *, int32, u_int32));
error_code _os_ev_delete _OP((_CONST char *));
error_code _os_ev_info _OP((event_id, u_int32, void *));
error_code _os_ev_link _OP((_CONST char *, event_id *));
error_code _os_ev_pulse _OP((event_id, int32 *, int32));
error_code _os_ev_read _OP((event_id, int32 *));
error_code _os_ev_set _OP((event_id, int32 *, u_int32));
error_code _os_ev_setr _OP((event_id, int32 *, u_int32));
error_code _os_ev_signal _OP((event_id, int32 *, u_int32));
error_code _os_ev_unlink _OP((event_id));
error_code _os9_ev_wait _OP((event_id, int32 *, int32, int32));
error_code _os9_ev_waitr _OP((event_id, int32 *, int32 *, int32 *));
error_code _os9_ev_info _OP((u_int32 *, ev_infostr *));

#if defined(_OPT_PROTOS)
int _ev_creat _OP((int, int, int, _CONST char *));
int _ev_delete _OP((_CONST char *));
int _ev_info _OP((int, ev_infostr *));
int _ev_link _OP((_CONST char *));
int _ev_pulse _OP((int, int, int));
int _ev_read _OP((int));
int _ev_set _OP((int, int, int));
int _ev_setr _OP((int, int, int));
int _ev_signal _OP((int, int));
int _ev_unlink _OP((int));
int _ev_wait _OP((int, int, int));
int _ev_waitr _OP((int, int, int));
#endif	/* _OPT_PROTOS */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _EVENTS_H */
