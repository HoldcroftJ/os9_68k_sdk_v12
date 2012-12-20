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
 *  01 ??/??/?? Created                                                ???
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  02 11/28/97 Changed proto for auth_get_cur_[chap|pap]              tmd
 *              Added proto for auth_get_[chap|pap]                    tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/
#ifndef _AUTH_MOD__
#define _AUTH_MOD__

#include <const.h>
#include <errno.h>
#include <module.h>
#include <modes.h>
#include <string.h>
#include <types.h>


#define MAX_PEER_NAME		(32)
#define MAX_SECRET			(32)
#define AUTH_MOD			"ppp_auth"


#define AUTH_TYPE_CHAP		100
#define AUTH_TYPE_PAP		101
#define AUTH_TYPE_FREE		999



/*******************************************************************************
** STRUCTURES
*******************************************************************************/

typedef struct _auth_header
{
	u_int16		max_entries;
	u_int16		entry_cnt;
	char		isp_name[MAX_PEER_NAME];

} auth_header;

typedef struct
{
	u_int32		in_use		: 1;
	u_int32		rsvd_1		: 15;

} auth_flags, *Auth_flags;

typedef struct _auth_entry
{
	auth_flags	flags;
	u_int16		type;
	char		isp_name[MAX_PEER_NAME];
	char		id[MAX_PEER_NAME];
	char		secret[MAX_SECRET];

} auth_entry, *Auth_entry;

typedef struct
{
	auth_header	hdr;
	auth_entry	entry[1];

} auth_data, *Auth_data;

typedef struct _auth_handle
{
	mh_data*	mod_hdr;
	Auth_data	data;

} auth_handle, *Auth_handle;

/*******************************************************************************
** PROTOTYPES
******************************************************************************/

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */

	error_code auth_create_mod( u_int16 max_entries, Auth_handle hndl );
	error_code auth_link_mod( Auth_handle hndl );
	error_code auth_unlink_mod( Auth_handle hndl );

	error_code auth_clr_entries( u_int32 num_entries, Auth_handle hndl );

	error_code auth_get_cur_chap( char* name, char* secret, Auth_handle hndl);
	error_code auth_get_cur_pap( char* name, char* secret, Auth_handle hndl);

	error_code auth_get_chap( char *isp, char* name, char* secret,
	                              Auth_handle hndl);
	error_code auth_get_pap( char *isp, char* name, char* secret,
	                             Auth_handle hndl);

	error_code auth_get_isp_name( char* name, Auth_handle hndl );
	error_code auth_set_isp_name( char* name, Auth_handle hndl );

	error_code auth_add_chap( char* isp_name, char* id, char* secret,
								Auth_handle hndl );

	error_code auth_del_chap( char* isp_name, Auth_handle hndl );

	error_code auth_add_pap( char* isp_name, char* id, char* secret,
								Auth_handle hndl );

	error_code auth_del_pap( char* isp_name, Auth_handle hndl );

	error_code auth_get_entry( char* isp_name, u_int16 type,
								Auth_entry buf, Auth_handle hndl );

	error_code auth_copy_entry( char* isp_name, u_int16 type,
								u_int16 new_type, Auth_handle hndl );

	error_code auth_set_type( char* isp_name, u_int16 type,
							u_int16 new_type, Auth_handle hndl );

	error_code auth_set_id( char* isp_name, u_int16 type,
							char* id, Auth_handle hndl );

	error_code auth_set_secret( char* isp_name, u_int16 type,
								char* secret, Auth_handle hndl );

#ifdef __cplusplus
	}
#endif /* __cplusplus */

#endif /* _AUTH_MOD__ */
