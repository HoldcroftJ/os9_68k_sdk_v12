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
 *   1 05/29/96 Initial version                                        billw
 *  11 01/26/97 RELEASED TO John FOR UNIDEN                            udayn
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  12 01/15/98 Fixed to avoid compiler warnings                       tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

/****************************************************************************
 * This design is based on RFC 1144 and the contents therein, so to be safe,
 * The copyright notice is included:
 *
 * Copyright (c)1989 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that the above copyright notice and this paragraph are duplicates in all
 * such forms and that any documentation, advertising materials, and other
 * materials relating to such distribution and use acknowledge the software
 * was developed by the University of California, Berkely. The name of the
 * University may not be used to endorse or promote products derived from
 * this software without specific prior permission.
 ****************************************************************************/

#include <SPF/ppp_auth.h>
#include <ctype.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

_asm("_sysedit: equ 12");

#define NUM_ENTRIES			(16)

#define NO_MODE				0
#define ADD_MODE			1
#define DELETE_MODE			2
#define FREE_MODE			3
#define MODIFY_MODE			4
#define SETISP_MODE			5
#define NEW_MODE			6

#define AUTH_TYPE_NONE		0


#if 0
#define MSG(s)				{fprintf(stderr,s);}
#define MSG1(s,x1)			{fprintf(stderr,s,x1);}
#define MSG2(s,x1,x2)		{fprintf(stderr,s,x1,x2);}
#define MSG3(s,x1,x2,x3)	{fprintf(stderr,s,x1,x2,x3);}
#endif

#define MSG					printf
#define VMSG				if( verbose ) printf
#define DMSG				if( debug ) printf

#define ERR(s,e)			MSG( "ERROR: %s() returned %x (%d). \n", s, e, e );

#define HELP_MSG(e) \
	{ \
		MSG( help_msg ); \
		_os_exit( e ); \
	}


#define SHOW_STATUS(s,e) 	VMSG( "%s() returned %x (%d). \n", s, e, e );

auth_handle		hndl;

u_int32			num_to_free = 0;

int				mode = NO_MODE;
int				type = AUTH_TYPE_NONE;
int				new_type = AUTH_TYPE_NONE;

int				aidflg = 0,
				freeflg = 0,
				ispflg = 0,
				ntflg = 0,
				secretflg = 0;

int				debug = 0,
				did_something = 0,
				listflg = 0,
				verbose = 0;

char			*isp_name = NULL,
				*auth_id = NULL,
				*secret = NULL;

char			*new_auth_id,
				*new_secret;

char			*help_msg = "\
Syntax: pppauth <option> \n\
Function: Creates/updates the secrets database used by PPP. \n\
Options: \n\
	-a                    Add mode: Add specified entry. \n\
                             -c or -p MUST be specified, \n\
                             along with <ISP name>, <Auth ID> and <Secret>. \n\
	-c                    CHAP specifier: Operate on CHAP entry(ies). \n\
	-d                    Delete mode: Delete specified entry. \n\
                             -c or -p MUST be specified, \n\
                             along with <ISP name>. \n\
	-f <num entries>      Free entries so <num entries> are available. \n\
	-h <ISP name>         Set current ISP name. \n\
    -i <Auth ID>          Used in Modify mode to specify new Auth ID. \n\
	-l                    List mode: List specified entries. \n\
                             -c or -p may be specified. \n\
	-m                    Modify mode: Modify specified entry. \n\
                             -c or -p MUST be specified, \n\
                             along with <ISP name> and params to change. \n\
    -n                    New mode: Copy existing entry with new type. \n\
                             -c or -p[ap] MUST be specified, \n\
                             along with -t[ype]. \n\
	-p                    PAP specifier: Operate on PAP entry(ies). \n\
    -s <Secret>           Used in Modify mode to specify new Secret. \n\
    -t [CHAP | PAP]       Type specifier: Change type to CHAP or PAP. \n\
                             CHAP or PAP may be abbreviated C or P. \n\
	-v                    Verbose mode. Show progress info. \n\n";




void parse_args( int argc, char* argv[] );
void chk_mode_flgs( void );
void chk_type_flgs( void );
void add_entry( void );
void delete_entry( void );
void free_entries( void );
void list_entries( void );
void modify_entry( void );
void new_entry( void );
void set_isp_name( void );

int main( int argc, char* argv[] )
{
	error_code	err;

	/* See if authentication module exists. If not, create one. */

	err = auth_link_mod( &hndl );

	if( err != SUCCESS )
	{
		err = auth_create_mod( NUM_ENTRIES, &hndl );

		if( err != SUCCESS )
		{
			ERR( "auth_create_mod", err );
			_os_exit(err);
		}
	}

	/* Read command line parameters. */

	parse_args( argc, argv );

	switch( mode )
	{
	case ADD_MODE:
		add_entry();
		break;
	case DELETE_MODE:
		delete_entry();
		break;
	case FREE_MODE:
		free_entries();
		break;
	case MODIFY_MODE:
		modify_entry();
		break;
	case NEW_MODE:
		new_entry();
		break;
	case SETISP_MODE:
		set_isp_name();
		break;
	case NO_MODE:
	default:
		break;
	}

	if( listflg )
	{
		list_entries();
	}

	if( did_something == 0 )
	{
		HELP_MSG( 0 );
	}

	_os_exit( 0 );
}

void parse_args( int argc, char* argv[] )
{
	char*	arg;

	while( --argc )
	{
		arg = *++argv;

		DMSG( "ARG = <%s> \n", arg );

		if( *arg == '-' )	/* Option/flag */
		{
			DMSG( "==OPTION \n" );
			arg++;

			while( arg && *arg != '\0' )
			{
				DMSG( "OPTION = <%c> \n", *arg );

				switch( tolower( *arg++ ) )
				{
				case 'a':	/* ADD entry */
					DMSG( "==ADD_MODE \n" );
					chk_mode_flgs();
					mode = ADD_MODE;
					break;
				case 'c':	/* CHAP entry */
					DMSG( "==CHAP \n" );
					chk_type_flgs();
					type = AUTH_TYPE_CHAP;
					break;
				case 'd':	/* DELETE entry */
					DMSG( "==DELETE_ENTRY \n" );
					chk_mode_flgs();
					mode = DELETE_MODE;
					break;
				case 'f':	/* FREE entry */
					DMSG( "==FREE_ENTRY \n" );
					chk_mode_flgs();
					mode = FREE_MODE;
					freeflg = 1;
					break;
				case 'g':	/* Debug mode */
					printf( "==DEBUG \n" );
					debug = 1;
					break;
				case 'h':	/* Set ISP mode */
					DMSG( "==SET_ISP_NAME \n" );
					mode = SETISP_MODE;
					ispflg = 1;
					break;
				case 'i':	/* New auth ID */
					DMSG( "==NEW_AUTH_ID \n" );
					aidflg = 1;
					break;
				case 'l':	/* List entries */
					DMSG( "==LIST_ENTRIES \n" );
					listflg = 1;
					break;
				case 'm':	/* MODIFY entry */
					DMSG( "==MODIFY_ENTRY \n" );
					chk_mode_flgs();
					mode = MODIFY_MODE;
					break;
				case 'n':	/* NEW entry */
					DMSG( "==NEW_ENTRY \n" );
					chk_mode_flgs();
					mode = NEW_MODE;
					break;
				case 'p':	/* PAP entry */
					DMSG( "==PAP \n" );
					chk_type_flgs();
					type = AUTH_TYPE_PAP;
					break;
				case 's':	/* New Secret */
					DMSG( "==NEW_SECRET \n" );
					secretflg = 1;
					break;
				case 't':	/* TYPE specifier */
					DMSG( "==TYPE_SPECIFIER \n" );
					ntflg = 1;
					break;
				case 'v':	/* Verbose mode */
					DMSG( "==VERBOSE \n" );
					verbose = 1;
					break;
				case '?':	/* Help mode */
					DMSG( "==HELP \n" );
				default:
					HELP_MSG( 0 );
					break;
				}	/* End of switch */
			}	/* End while not end of option string. */
		}	/* End if arg started with (-) sign. */

		else if( aidflg )
		{
			new_auth_id = arg;
			aidflg = 0;
		}
		else if( secretflg )
		{
			new_secret = arg;
			secretflg = 0;
		}
		else if( ntflg )	/* New type: PAP or CHAP */
		{
			char	c = tolower(*arg);

			if( c == 'c' )
			{
				new_type = AUTH_TYPE_CHAP;
			}
			else if( c == 'p' )
			{
				new_type = AUTH_TYPE_PAP;
			}

			ntflg = 0;
		}
		else if( ispflg )	/* Should be remote host name. */
		{
			isp_name = arg;
			ispflg = 0;
		}
		else if( freeflg )	/* Should be number to free. */
		{
			num_to_free = atoi( arg );
			freeflg = 0;
		}
		else if( isp_name == NULL )		/* Set ISP name */
		{
			isp_name = arg;
		}
		else if( auth_id == NULL )		/* Set Auth ID */
		{
			auth_id = arg;
		}
		else if( secret == NULL )		/* Set secret */
		{
			secret = arg;
		}
		else
		{
			MSG( "WARNING: Extra parameter specified (%s). \n", arg );
		}
	}	/* End while arguments remain. */
}

void chk_mode_flgs( void )
{
	if( mode )
	{
		MSG( "ERROR: Add, Delete, Free, Modify and Set \n" );
		MSG( "are mutually exclusive options. \n" );
		HELP_MSG( E_PARAM );
	}
}

void chk_type_flgs( void )
{
	if( type )
	{
		MSG( "ERROR: CHAP and PAP are mutually exclusive types. \n" );
		HELP_MSG( E_PARAM );
	}
}

void add_entry( void )
{
	error_code	err;

	if( isp_name == NULL )
	{
		MSG( "ERROR: No ISP name specified. \n" );
		HELP_MSG( E_PARAM );
	}

	if( auth_id == NULL )
	{
		MSG( "ERROR: No Authentication ID specified. \n" );
		HELP_MSG( E_PARAM );
	}

	if( secret == NULL )
	{
		MSG( "ERROR: No secret specified. \n" );
		HELP_MSG( E_PARAM );
	}

	if( type == AUTH_TYPE_CHAP )
	{
		VMSG( "Adding CHAP entry:" );

		did_something = 1;
		err = auth_add_chap( isp_name, auth_id, secret, &hndl );
		SHOW_STATUS( "auth_add_chap", err );
	}
	else if( type == AUTH_TYPE_PAP )
	{
		VMSG( "Adding PAP entry:" );

		did_something = 1;
		err = auth_add_pap( isp_name, auth_id, secret, &hndl );
		SHOW_STATUS( "auth_add_pap", err );
	}
	else
	{
		MSG( "ERROR: no type (CHAP or PAP) specified. \n" );
		HELP_MSG( E_PARAM );
	}
}

void delete_entry( void )
{
	error_code	err;

	if( isp_name == NULL )
	{
		MSG( "ERROR: No ISP name specified. \n" );
		HELP_MSG( E_PARAM );
	}

	if( type == AUTH_TYPE_CHAP )
	{
		VMSG( "Deleting CHAP entry:" );

		did_something = 1;
		err = auth_del_chap( isp_name, &hndl );
		SHOW_STATUS( "auth_del_chap", err );
	}
	else if( type == AUTH_TYPE_PAP )
	{
		VMSG( "Deleting PAP entry:" );

		did_something = 1;
		err = auth_del_pap( isp_name, &hndl );
		SHOW_STATUS( "auth_del_pap", err );
	}
	else
	{
		MSG( "ERROR: no type (CHAP or PAP) specified. \n" );
		HELP_MSG( E_PARAM );
	}
}

void free_entries( void )
{
	error_code	err;

	if( num_to_free == 0 )
	{
		MSG( "WARNING: free_entries() called with zero value. \n" );
		return;
	}

	VMSG( "free_entries(); num = %u. \n", num_to_free );
	did_something = 1;

	err = auth_clr_entries( num_to_free, &hndl );
	SHOW_STATUS( "auth_clr_entries", err );
}

#define LINE_WIDTH		(4 + 2 + 34 + 2 + 34)	

char* curispmsg0 = "\nCurrent ISP name is '%s'.\n\n";
char* curispmsg1 = "\nNo ISP currently specified.\n\n";
char* linefmt = "%-4s  %-34s  %s";

void list_entries( void )
{
	Auth_entry	entry;
	u_int32		i = 0;
	char		buf[LINE_WIDTH + 2];
	char		isp[MAX_PEER_NAME + 3];
	char		id[MAX_PEER_NAME + 3];
	char		secret[MAX_SECRET + 3];

	did_something = 1;

	if( *(hndl.data->hdr.isp_name) == '\0' )
	{
		MSG( curispmsg1 );
	}
	else
	{
		MSG( curispmsg0, hndl.data->hdr.isp_name );
	}

	memset( buf, '-', LINE_WIDTH );
	buf[LINE_WIDTH] = '\n';
	buf[LINE_WIDTH+1] = '\0';

	MSG( linefmt, "Type", "ISP Name", "Authentication ID / Secret" );
	MSG( "\n" );
	MSG( buf );

	while( i < hndl.data->hdr.entry_cnt )
	{
		entry = &(hndl.data->entry[i++]);

		sprintf( isp, "'%s'", entry->isp_name );
		sprintf( id, "'%s'", entry->id );
		sprintf( secret, "'%s'", entry->secret );

		if( entry->flags.in_use )
		{
			if( entry->type == AUTH_TYPE_CHAP )
			{
				MSG( linefmt, "CHAP", isp, id );
			}
			else if( entry->type == AUTH_TYPE_PAP )
			{
				MSG( linefmt, "PAP", isp, id );
			}
			else
			{
				MSG( linefmt, "????", isp, id );
			}

			if( (strlen(id) + strlen(secret)) <= 34 )
			{
				MSG( "  %s", secret );
			}
			else
			{
				MSG( "\n" );
				MSG( linefmt, "", "", secret );
			}

			MSG( "\n" );
		}
	}

	MSG( "\n" );
}

void modify_entry( void )
{
	error_code	err;

	if( isp_name == NULL )
	{
		MSG( "ERROR: No ISP name specified. \n" );
		HELP_MSG( E_PARAM );
	}

	if( type == AUTH_TYPE_CHAP )
	{
		VMSG( "Modifying CHAP entry:" );
	}
	else if( type == AUTH_TYPE_PAP )
	{
		VMSG( "Modifying PAP entry:" );
	}
	else
	{
		MSG( "ERROR: no type (CHAP or PAP) specified. \n" );
		HELP_MSG( E_PARAM );
	}

	did_something = 1;

	if( new_type )
	{
		VMSG( "Modifying Authentication type. \n" );

		err = auth_set_type( isp_name, type, new_type, &hndl );
		SHOW_STATUS( "auth_set_type", err );
	}

	if( new_auth_id )
	{
		VMSG( "Modifying Authentication ID. \n" );

		err = auth_set_id( isp_name, type, new_auth_id, &hndl );
		SHOW_STATUS( "auth_set_id", err );
	}

	if( new_secret )
	{
		VMSG( "Modifying Secret. \n" );

		err = auth_set_secret( isp_name, type, new_secret, &hndl );
		SHOW_STATUS( "auth_set_secret", err );
	}
}

void new_entry( void )
{
	auth_entry	entry;
	error_code	err;

	if( isp_name == NULL )
	{
		MSG( "ERROR: No ISP name specified. \n" );
		HELP_MSG( E_PARAM );
	}

	if( new_type == AUTH_TYPE_NONE || new_type == type )
	{
		MSG( "ERROR: Invalid new type specified. \n" );
		HELP_MSG( E_PARAM );
	}

	if( type == AUTH_TYPE_CHAP )
	{
		VMSG( "Copying CHAP entry:" );
	}
	else if( type == AUTH_TYPE_PAP )
	{
		VMSG( "Copying PAP entry:" );
	}
	else
	{
		MSG( "ERROR: no type (CHAP or PAP) specified. \n" );
		HELP_MSG( E_PARAM );
	}

	did_something = 1;
	err = auth_get_entry( isp_name, type, &entry, &hndl );

	if( err )
	{
		MSG( "ERROR: Specified entry does not exist. \n" );
		return;
	}

	if( new_type == AUTH_TYPE_CHAP )
	{
		err = auth_add_chap( isp_name, entry.id, entry.secret, &hndl );
		SHOW_STATUS( "auth_add_chap", err );
	}
	else if( new_type == AUTH_TYPE_PAP )
	{
		err = auth_add_pap( isp_name, entry.id, entry.secret, &hndl );
		SHOW_STATUS( "auth_add_pap", err );
	}
}

void set_isp_name( void )
{
	error_code	err;

	if( isp_name == NULL )
	{
		MSG( "ERROR: No ISP name specified. \n" );
		HELP_MSG( E_PARAM );
	}

	if( verbose )
	{
		MSG( "Setting current ISP name to %s. \n", isp_name );
	}

	did_something = 1;
	err = auth_set_isp_name( isp_name, &hndl );
	SHOW_STATUS( "auth_set_isp_name", err );
}
