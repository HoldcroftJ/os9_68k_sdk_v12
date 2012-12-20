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
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  18 12/15/97 Fixed to stop compiler warnings                        tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#include "chat.h"

/*******************************************************/
/*******************************************************/


void do_open( char* name, u_int32 mode, path_id* pid )
{
	if( verbose ) printf( "Opening device %s \n", name );

	status = _os_open( name, mode, pid );
	show_error( "_os_open", status );
}

Device_options sv_devopt( path_id pd )
{
	Device_options opts;

	opts = (Device_options) malloc( sizeof(device_options) );

	if( opts )
	{
		opts->next = NULL;
		opts->prev = NULL;
		opts->dopts.pd = pd;

		save_devopt( &(opts->dopts) );
	}

	return( opts );
}

void sv_devopts( path_id pd )
{
	Device_options	opts = sv_devopt( pd );

	if( opts )
	{
		opts->prev	= devopt_last;

		if( devopt_last )
		{
			devopt_last->next = opts;
		}

		devopt_last = opts;

		if( ! devopt_list )
		{
			devopt_list = opts;
		}
	}
}

void rst_devopt( Device_options opts )
{
	restore_devopt( &(opts->dopts) );
	free( opts );
}

void rst_devopts( void )
{
	Device_options	cur, opts = devopt_last;

	while( opts )
	{
		cur = opts;
		opts = opts->prev;

		rst_devopt( cur );
	}
}

void port_noecho( path_id pd )
{
	Device_options	opts = sv_devopt( pd );

	if( ! opts )
	{
		return;
	}

	/* Set port for noecho mode */

#if defined(_OSK)
	opts->dopts.path_opts.pd_eko = 0;
#elif defined(_OS9000)
	opts->dopts.path_opts.pd_echo = 0;
#endif

	restore_devopt( &(opts->dopts) );
}

void pt_raw( path_id pd, Ppp_modem_params mp )
{
	device_opts	opts;

	opts.pd			= pd;

	opts.baud_rate	= mp->val.baud_rate;
	opts.parity		= mp->val.parity;
	opts.stop_bits	= mp->val.stop_bits;
	opts.word_size	= mp->val.word_size;
	opts.rts_enable	= mp->val.rts_enable;

	port_raw( &opts );
}

char rd_modem_char( void )
{
	unsigned char	buf[2];
	error_code		status;

	status = read_char( com_rd, buf );

	if( status == SUCCESS )
	{
		if( show_data_chars || echo || debug )
		{
			if( buf[0] != 0x0a && last_rx_char == 0x0d )
			{
				fputc( '\n', logfp );
			}

			printqc( logfp, buf[0] );
			last_rx_char = buf[0];

			if( buf[0] == 0x0a )
			{
				fputc( '\n', logfp );
			}
		}

		return( buf[0] );
	}

	return( 0 );
}

void wr_modem_char( char c )
{
	char	buf[2];
	u_int32	len = 1;

	buf[0] = c;

	status = _os_write( com_wr, buf, &len );
	show_error( "_os_write", status );

	if( verbose || debug )
	{
		printqc( logfp, c );
	}
}

void wr_modem_string( char* s )
{
	u_int32			len = strlen( s );

	status = _os_write( com_wr, s, &len );
	show_error( "_os_write", status );

	if( verbose || debug )
	{
		while( len-- )
		{
			printqc( logfp, *s++ );
		}
	}
}

error_code send_break( path_id path )
{
	error_code		status = SUCCESS; /* non-fatal??? */

#ifndef _OSK
	status = _os_ss_break( path );
#else
	printf( "chat: Warning: Break currently not supported for OS9. \n" );
#endif

	return( status );
}

error_code read_char( path_id path, unsigned char* buf )
{
	u_int32		len;
	error_code	status;

	status = _os_gs_ready( path, &len );

	if( ! status && len > 0 )
	{
		len = 1;
		status = _os_read( path, buf, &len );

		if( status )
		{
			printf( "\n _os_read returned %x for %s \n", status, comport_name );
		}
		else
		{
			if( mask_high_bit )
			{
				*buf = (*buf & 0x7f);
			}
		}
	}

	return( status );
}
