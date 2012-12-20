/* dexec() - debug modes */
#if !defined(_DEXEC_H)
#define _DEXEC_H

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

typedef enum {
	DBG_M_INACTV,				/* inactive mode (used by kernel) */
	DBG_M_HARD,					/* hard break points / full speed execution	*/
	DBG_M_SOFT,					/* soft break points / continuous execution */
	DBG_M_COUNT,				/* execute count instructions				*/
	DBG_M_CONTROL,				/* execute until change of control (future)	*/
} dexec_mode;

/* dexec() return status */
typedef enum {
	DBG_S_FINISH,				/* command finished successfully			*/
	DBG_S_BRKPNT,				/* process hit break point					*/
	DBG_S_CHILDSIG,				/* process received signal (no intercept)	*/
	DBG_S_PARENTSIG,			/* debugger received signal					*/	
	DBG_S_WATCH,				/* process hit watch point (future)			*/
	DBG_S_CHAIN,				/* process has made an F$Chain call			*/
	DBG_S_EXCEPT,				/* execption occurred during execution 		*/
	DBG_S_EXIT		= 0x80,		/* process has made an F$Exit call			*/
	DBG_S_CONTROL	= 0x24,		/* process executed a jmp or bra (future)	*/
	DBG_S_FORK		= 15,	  	/* process has made an F$Fork call			*/
} dexec_status;

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
error_code _os_dexec(process_id, dexec_mode, void *, u_int32 *, u_int32, void **, u_int32 *, dexec_status *, u_int32 *, void **, error_code *);
error_code _os_dexit(process_id);
error_code _os9_dfork(u_int32, u_int32, const char *, const void *, u_int32, u_int32, process_id *, u_int32, void *);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
error_code _os_dexec();
error_code _os_dexit();
error_code _os9_dfork();
#endif

#endif /* _DEXEC_H */
