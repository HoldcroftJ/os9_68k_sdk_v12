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
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     09/17/97 In auth_del_entry, decrement the entry_cnt instead     sr
 *              of incrementing it.
 *   2 11/28/97 Added auth_get_chap & auth_get_pap functions           tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#include <SPF/ppp_auth.h>


error_code auth_get_entryp( char* isp, u_int16 type,
							Auth_entry* eh, Auth_handle hndl );
error_code auth_cur_info( char* id, char* secret, u_int16 type,
							Auth_handle hndl, char *isp );
error_code auth_new_entry( char* isp, char *id, char *secret,
							u_int16 type, Auth_handle hndl );
error_code auth_add_entry( char* isp, char *id, char *secret,
							u_int16 type, Auth_handle hndl );
error_code auth_del_entry( char* isp, u_int16 type, Auth_handle hndl );



/*******************************************************************************
** FUNC: auth_create_mod()
** DESC:
*******************************************************************************/

error_code auth_create_mod( u_int16 max_entries, Auth_handle hndl )
{
	u_int16		ar, tl;
	u_int32		perms, size;
	error_code	err;

	/* Setup the parameters to make the data module. */

	ar = mkattrevs( MA_REENT, 0x09 );
	tl = MT_DATA;
	perms=(MP_OWNER_READ | MP_OWNER_WRITE);
	size = sizeof(auth_data) + ((max_entries - 1) * sizeof(auth_entry));

	/*
	** Make the data module with the data area big enough to hold the
	** auth header and enough entries.
	*/

	err = _os_datmod( AUTH_MOD, size, &ar, &tl, perms,
					(void*) &(hndl->data), &(hndl->mod_hdr) );

	if( err != SUCCESS )
	{
		hndl->mod_hdr = NULL;
		hndl->data = NULL;
		return( err );
	}

	memset( hndl->data, 0, size );

	/* Fill in the auth header appropriately. */

	hndl->data->hdr.max_entries	= max_entries;

	_os_setcrc( (mh_com*) hndl->mod_hdr );

	return( SUCCESS );
}
/* auth_create_mod() */

/*******************************************************************************
** FUNC: auth_link_mod()
** DESC:
*******************************************************************************/

error_code auth_link_mod( Auth_handle hndl )
{
	u_int16		tl, ar;
	char*		mod_name = AUTH_MOD;
	error_code	err;

	tl = MT_ANY;

	err = _os_link( &mod_name, (mh_com**) &(hndl->mod_hdr),
						(void**) &(hndl->data), &tl, &ar );

	if( err != SUCCESS )
	{
		hndl->mod_hdr = NULL;
		hndl->data = NULL;
	}

	return( err );
}
/* auth_link_mod() */

/*******************************************************************************
** FUNC: auth_unlink_mod()
** DESC:
*******************************************************************************/

error_code auth_unlink_mod( Auth_handle hndl )
{
	error_code	err;

	err = _os_unlink( (mh_com*) hndl->mod_hdr );

	return( err );
}
/* auth_unlink_mod() */

/*******************************************************************************
** FUNC: auth_clr_entries()
** DESC:
*******************************************************************************/

error_code auth_clr_entries( u_int32 num_entries, Auth_handle hndl )
{
	Auth_entry	entry;
	u_int32		i = 0, num_free, num_to_free;

	/* Limit number of free entries to max_entries. */

	if( num_entries > hndl->data->hdr.max_entries )
	{
		num_entries = hndl->data->hdr.max_entries;
	}

	/* Figure out how many entries we need to delete. */

	num_free = (hndl->data->hdr.max_entries - hndl->data->hdr.entry_cnt);
	num_to_free = (num_entries - num_free);

	while( num_to_free && i < hndl->data->hdr.max_entries )
	{
		entry = &(hndl->data->entry[i]);

		if( entry->flags.in_use )
		{
			memset( entry, 0, sizeof(auth_entry) );
			hndl->data->hdr.entry_cnt--;
			num_to_free--;
		}

		i++;
	}

	_os_setcrc( (mh_com*) hndl->mod_hdr );

	return( SUCCESS );
}

/*******************************************************************************
** FUNC: auth_get_cur_chap()
** DESC:
*******************************************************************************/

error_code auth_get_cur_chap( char* id, char* secret, Auth_handle hndl )
{
	error_code	err;

	err = auth_cur_info( id, secret, AUTH_TYPE_CHAP, hndl, 0 );

	return( err );
}

/*******************************************************************************
** FUNC: auth_get_cur_pap()
** DESC:
*******************************************************************************/

error_code auth_get_cur_pap( char* id, char* secret, Auth_handle hndl )
{
	error_code	err;

	err = auth_cur_info( id, secret, AUTH_TYPE_PAP, hndl, 0 );

	return( err );
}

/*******************************************************************************
** FUNC: auth_get_chap()
** DESC:
*******************************************************************************/

error_code auth_get_chap( char *isp, char* id, char* secret, Auth_handle hndl )
{
	error_code	err;

	err = auth_cur_info( id, secret, AUTH_TYPE_CHAP, hndl, isp );

	return( err );
}

/*******************************************************************************
** FUNC: auth_get_pap()
** DESC:
*******************************************************************************/

error_code auth_get_pap( char *isp, char* id, char* secret, Auth_handle hndl )
{
	error_code	err;

	err = auth_cur_info( id, secret, AUTH_TYPE_PAP, hndl, isp );

	return( err );
}

/*******************************************************************************
** FUNC: auth_get_isp_name()
** DESC:
*******************************************************************************/

error_code auth_get_isp_name( char* isp_name, Auth_handle hndl )
{
	strcpy( isp_name, hndl->data->hdr.isp_name );
	return( SUCCESS );
}

/*******************************************************************************
** FUNC: auth_set_isp_name()
** DESC:
*******************************************************************************/

error_code auth_set_isp_name( char* isp_name, Auth_handle hndl )
{
	strcpy( hndl->data->hdr.isp_name, isp_name );
	return( SUCCESS );
}

/*******************************************************************************
** FUNC: auth_add_chap()
** DESC:
*******************************************************************************/

error_code auth_add_chap( char* isp, char *id, char *secret, Auth_handle hndl )
{
	error_code	err;

	err = auth_add_entry( isp, id, secret, AUTH_TYPE_CHAP, hndl );

	return( err );
}

/*******************************************************************************
** FUNC: auth_del_chap()
** DESC:
*******************************************************************************/

error_code auth_del_chap( char* isp, Auth_handle hndl )
{
	error_code	err;

	err = auth_del_entry( isp, AUTH_TYPE_CHAP, hndl );

	return( err );
}

/*******************************************************************************
** FUNC: auth_add_pap()
** DESC:
*******************************************************************************/

error_code auth_add_pap( char* isp, char *id, char *secret, Auth_handle hndl )
{
	error_code	err;

	err = auth_add_entry( isp, id, secret, AUTH_TYPE_PAP, hndl );

	return( err );
}

/*******************************************************************************
** FUNC: auth_del_pap()
** DESC:
*******************************************************************************/

error_code auth_del_pap( char* isp, Auth_handle hndl )
{
	error_code	err;

	err = auth_del_entry( isp, AUTH_TYPE_PAP, hndl );

	return( err );
}

/*******************************************************************************
** FUNC: auth_get_entry()
** DESC:
*******************************************************************************/

error_code auth_get_entry( char* isp, u_int16 type,
							Auth_entry buf, Auth_handle hndl )
{
	Auth_entry	entry;
	error_code	err;

	if( type != AUTH_TYPE_CHAP && type != AUTH_TYPE_PAP )
	{
		return( EOS_PARAM );
	}

	err = auth_get_entryp( isp, type, &entry, hndl );

	if( err )
	{
		return( err );
	}

	memcpy( buf, entry, sizeof(auth_entry) );

	return( SUCCESS );
}

/*******************************************************************************
** FUNC: auth_copy_entry()
** DESC:
*******************************************************************************/

error_code auth_copy_entry( char* isp, u_int16 type, u_int16 new_type,
							Auth_handle hndl )
{
	Auth_entry	ent;
	error_code	err;

	err = auth_get_entryp( isp, type, &ent, hndl );

	if( err )
	{
		return( err );
	}

	err = auth_add_entry( isp, ent->id, ent->secret, new_type, hndl );

	return( err );
}

/*******************************************************************************
** FUNC: auth_set_type()
** DESC:
*******************************************************************************/

error_code auth_set_type( char* isp, u_int16 type,
							u_int16 new_type, Auth_handle hndl )
{
	Auth_entry	ent;
	error_code	err;

	err = auth_get_entryp( isp, type, &ent, hndl );

	if( err )
	{
		return( err );
	}

	ent->type = new_type;
	_os_setcrc( (mh_com*) hndl->mod_hdr );

	return( SUCCESS );
}

/*******************************************************************************
** FUNC: auth_set_id()
** DESC:
*******************************************************************************/

error_code auth_set_id( char* isp, u_int16 type, char* id, Auth_handle hndl )
{
	Auth_entry	ent;
	error_code	err;

	err = auth_get_entryp( isp, type, &ent, hndl );

	if( err )
	{
		return( err );
	}

	strcpy( ent->id, id );
	_os_setcrc( (mh_com*) hndl->mod_hdr );

	return( SUCCESS );
}

/*******************************************************************************
** FUNC: auth_set_secret()
** DESC:
*******************************************************************************/

error_code auth_set_secret( char* isp, u_int16 type, char* secret,
							Auth_handle hndl )
{
	Auth_entry	ent;
	error_code	err;

	err = auth_get_entryp( isp, type, &ent, hndl );

	if( err )
	{
		return( err );
	}

	strcpy( ent->secret, secret );
	_os_setcrc( (mh_com*) hndl->mod_hdr );

	return( SUCCESS );
}

/*
 ***********************
 * Internal functions. *
 ***********************
 */

/*******************************************************************************
** FUNC: auth_get_entryp()
** DESC: get a pointer to an entry in the database.
*******************************************************************************/

error_code auth_get_entryp( char* isp, u_int16 type,
							Auth_entry* eh, Auth_handle hndl )
{
	Auth_entry	entry;
	u_int16		i = 0;

	/* Search for the specified entry. */

	while( i < hndl->data->hdr.max_entries )
	{
		entry = &(hndl->data->entry[i]);

		if( entry->flags.in_use )
		{
			if( type == entry->type )
			{
				if( strcmp( entry->isp_name, isp ) == 0 )
				{
					*eh = entry;
					return( SUCCESS );
				}
			}
		}
		else
		{
			if( type == AUTH_TYPE_FREE )
			{
				*eh = entry;
				return( SUCCESS );
			}
		}

		i++;
	}

	*eh = NULL;

	if( type == AUTH_TYPE_FREE )
	{
		return( EOS_FULL );
	}

	return( EOS_PNNF );
}

/*******************************************************************************
** FUNC: auth_cur_info()
** DESC: get id and secret of given type for current or specified ISP
*******************************************************************************/

error_code auth_cur_info( char* id, char* secret, u_int16 type,
							Auth_handle hndl, char *isp )
{
	auth_entry	entry;
	error_code	err;

	if (isp)
		err = auth_get_entry( isp, type, &entry, hndl );
	else
		err = auth_get_entry( hndl->data->hdr.isp_name, type, &entry, hndl );

	if( err )
	{
		*id = '\0';
		*secret = '\0';

		return( err );
	}

	strcpy( id, entry.id );
	strcpy( secret, entry.secret );

	return( SUCCESS );
}

/*******************************************************************************
** FUNC: auth_new_entry()
** DESC: get first free entry and fill it in.
*******************************************************************************/

error_code auth_new_entry( char* isp, char *id, char *secret,
							u_int16 type, Auth_handle hndl )
{
	Auth_entry	entry;
	error_code	err;

	/* Must create a new entry.
	 * Verify we have room left and we have a valid secret type..
	 */

	if( hndl->data->hdr.entry_cnt >= hndl->data->hdr.max_entries )
	{
		return( EOS_FULL );
	}

	if( (type != AUTH_TYPE_CHAP) && (type != AUTH_TYPE_PAP) )
	{
		return( EOS_ILLPRM );
	}

	/* Find and fill the next entry. */

	err = auth_get_entryp( isp, AUTH_TYPE_FREE, &entry, hndl );

	if( err )
	{
		/* Something went wrong. Change entry_cnt to max_entries. */

		hndl->data->hdr.entry_cnt = hndl->data->hdr.max_entries;
		_os_setcrc( (mh_com*) hndl->mod_hdr );

		return( err );
	}

	memset( entry, 0, sizeof(auth_entry) );

	entry->flags.in_use = 1;
	entry->type = type;

	strcpy( entry->isp_name, isp );
	strcpy( entry->id, id );
	strcpy( entry->secret, secret );

	(hndl->data->hdr.entry_cnt)++;
	_os_setcrc( (mh_com*) hndl->mod_hdr );

	return( SUCCESS );
}

/*******************************************************************************
** FUNC: auth_add_entry()
** DESC: Add a new entry with the information supplied.
*******************************************************************************/

error_code auth_add_entry( char* isp, char *id, char *secret,
							u_int16 type, Auth_handle hndl )
{
	auth_entry	buf;
	error_code	err, err2 = SUCCESS;

	/* Note: This function does NOT need to update CRC
	 * because any function it calls to do the actual work
	 * will update the CRC itself.
	 */

	/* See if this entry is already in database. */

	err = auth_get_entry( isp, type, &buf, hndl );

	if( err )
	{
		err = auth_new_entry( isp, id, secret, type, hndl );
		return( err );
	}

	/* Update the fields in the existing entry. */

	if( strcmp( buf.id, id ) != 0 )
	{
		err = auth_set_id( isp, type, id, hndl );
	}

	if( strcmp( buf.secret, secret ) != 0 )
	{
		err2 = auth_set_secret( isp, type, secret, hndl );
	}

	if( err )
	{
		return( err );
	}

	return( err2 );
}

/*******************************************************************************
** FUNC: auth_del_entry()
** DESC: Clear the information from the appropriate entry in database.
*******************************************************************************/

error_code auth_del_entry( char* isp, u_int16 type, Auth_handle hndl )
{
	Auth_entry	ptr;
	error_code	err;

	/* See if this entry is in database. */

	err = auth_get_entryp( isp, type, &ptr, hndl );

	if( err )
	{
		return( err );
	}

	memset( ptr, 0, sizeof(auth_entry) );

	(hndl->data->hdr.entry_cnt)--;
	_os_setcrc( (mh_com*) hndl->mod_hdr );

	return( SUCCESS );
}
