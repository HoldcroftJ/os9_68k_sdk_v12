/*
	@(#)types.h	1.1.6	8/29/91
*/
#if !defined(_TYPES_H) && !defined(_types)
#define _TYPES_H
#define _types 1 /* Do NOT use this wrapper identifier in new software! */

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

typedef unsigned char   u_char;
typedef unsigned char	u_int8;
typedef unsigned short u_int16;
typedef unsigned int   u_int32;
typedef unsigned short  u_short;
typedef unsigned long   u_long;
typedef unsigned int u_int;

typedef char int8;
typedef short int16;
typedef int int32;

typedef u_int32 process_id;
typedef u_int32 signal_code;
typedef u_int32 path_id;
typedef u_int32 error_code;
typedef u_int32 event_id;
typedef u_int32 status_code;
typedef u_int32 alarm_id;
typedef u_char	hardware_vector;
typedef u_int16 prior_level;
typedef union {
	u_int32 group_user; /* group/user numbers							*/
	struct {
		u_int16 grp; /* group number									*/
		u_int16 usr; /* user number										*/
	} grp_usr;
} owner_id;

#endif /* _TYPES_H */
