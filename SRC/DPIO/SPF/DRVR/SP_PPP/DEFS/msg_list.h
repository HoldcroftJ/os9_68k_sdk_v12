/****************************************************************************
 *  ID:		%A%
 *  Date:	%G%
 ****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *  01 01/15/97 Initial Version                                        billw
 *  02 10/20/97 Adopted Bill W. Changes to Beta Code                   tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#ifndef _msg_list_h
	#define _msg_list_h

	#include <const.h>
	#include <errno.h>
	#include <memory.h>
	#include <string.h>
	#include <types.h>
	#include <SPF/mbuf.h>

	#define MBUF_LIST_SIZE		10

	typedef struct {
		Mbuf		mb[MBUF_LIST_SIZE];

	} mbufList;


	#define MSG_TYPE_ANY	0x8000
	#define MSG_ID_ANY		0x8000


	#ifdef __cplusplus
		"C" {
	#endif

		void ins_msg( Mbuf mb, mbufList* list );
		void del_msg( u_int16 type, u_int16 id, mbufList* list );
		void del_msg_ff( u_int16 type, u_int16 id, mbufList* list );
		Mbuf find_msg( u_int16 type, u_int16 id, mbufList* list );
		Mbuf extract_msg( u_int16 type, u_int16 id, mbufList* list );
		Mbuf last_msg( mbufList* list );
		void free_msg_list( mbufList* list );

	#ifdef __cplusplus
		}	/*  End C++ */
	#endif


#endif	/* _msg_list_h */
