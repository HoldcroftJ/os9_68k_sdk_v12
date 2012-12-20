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
 *  15 02/03/97 Released to John Miller for Uniden                     udayn
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  17 09/16/97 Don't know what ed 16 was! Print com_portname          sr
 *              (not t1) when closing a path.
 *  18 12/15/97 Fixed to stop compiler warnings                        tmd
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
/*
** Set the edition number.
*/
_asm("_sysedit: equ 18");

#define MAIN
#include "chat.h"


/*******************************************************/
/*******************************************************/

void main( int argc, char* argv[] )
{
	ppp_port_pb			port_params;
	ppp_modem_params	modem_params;

	initialize();				/* Initialize program */
	parse_args( argc, argv );	/* Parse the command line */

	if( not_valid )
	{
		term_prog( SUCCESS );
	}

	if( ! driver_name && ! comport_name )
	{
		printf( "Error - no device name specified.\n" );
		show_help( E_BNAM );
	}

	if( log_file )
	{
		logfp = fopen( log_file, "w" );

		if( ! logfp )
		{
			printf( "WARNING: Unable to open logfile '%s' \n", log_file );
			logfp = stderr;
		}
	}

	memset( &modem_params, 0, sizeof(modem_params) );

	if( ! comport_name )
	{
		/* Open the driver so we can do getstats */

		do_open( driver_name, S_IREAD | S_IWRITE, &driver_path );

		/* Get name of TX device */

		VPRT( "Reading user parameters for device %s \n", driver_name );

		/* Try newer version getstat code first. */

		modem_params.spb.code = PPP_GS_MODEM_PARAMS;
		modem_params.spb.updir = SPB_GOINGDWN;

		status = _os_getstat( driver_path, SS_SPF, &modem_params );

		if( status == SUCCESS )
		{
			VPRT( "Info (chat): _os_getstat(%s) returned %s. \n",
					"PPP_GS_MODEM_PARAMS", "SUCCESS" );
			comport_name = modem_params.val.tx_dev_name;
		} 
		else if( status == EOS_UNKSVC )
		{
			ppp_sysparams_pb	sys_params;

			/* Try the older style getstat code. */

			VPRT( "Info (chat): _os_getstat(%s) returned %s. \n",
					"PPP_GS_MODEM_PARAMS", "EOS_UNKSVC" );
			memset( &sys_params, 0, sizeof(sys_params) );
			sys_params.spb.code = PPP_GS_USRPARAMS;
			sys_params.spb.updir = SPB_GOINGDWN;
			sys_params.flg.tx_dev_name = 1;
	
			status = _os_getstat( driver_path, SS_SPF, &sys_params );
			show_error( "_os_getstat", status );

			comport_name = sys_params.tx_dev_name;
		}
		else
		{
			VPRT( "Info (chat): _os_getstat(%s) returned %d (%x).\n",
					"PPP_GS_MODEM_PARAMS", status, status );
			show_error( "_os_getstat", status );
		}

		/* Set up device to accept modem commands */

		VPRT( "Configuring device for modem commands.\n" );

		memset( &port_params, 0, sizeof(port_params) );
		port_params.spb.code = PPP_SS_PORT_SETUP;
		port_params.spb.updir = SPB_GOINGDWN;

		status = _os_setstat( driver_path, SS_SPF, &port_params );
		show_error( "_os_setstat", status );
	}

	/* Open TX device */

	do_open( comport_name, S_IREAD, &com_rd );
	sv_devopts( com_rd );
	pt_raw( com_rd, &modem_params );

	do_open( comport_name, S_IWRITE, &com_wr );
	sv_devopts( com_wr );
	pt_raw( com_wr, &modem_params );

	VPRT( "Device %s open; read path = %u, write path = %u.\n",
				comport_name, com_rd, com_wr );

	if( sh_port )
	{
		show_port( &modem_params );
	}

	/* Run the chat script */

	run_script();

	term_prog( SUCCESS );
}

void initialize( void )
{
	/* Set up the signal handler. */

	status = _os_intercept( signalHandler, _glob_data );
	show_error( "_os_intercept", status );

	/* Initialize program variables */

	driver_name		=					/* String variables */
	comport_name	=
	log_file		=
	cmd_file		= NULL;

	cur_line[0]		=					/* String buffers */
	last_expect[0]	=
	last_send[0]	=
	last_rx_char	= '\0';

	memset( com_buf, 0, COM_BUF_LEN );	/* Com port Rx buffer */

	com_buf_end		=
	com_buf_inp		=
	com_buf_outp	= com_buf;

	com_buf_end += COM_BUF_LEN;			/* End of com port Rx buffer */

	driver_path		=					/* Path IDs */
	com_wr			=
	com_rd			= 0;

	term			= fileno( stdin );

	devopt_list		=
	devopt_last		= NULL;				/* Device options list */

	logfp			= stderr;			/* File pointers */

	cmd_path		=
	src_fp			= NULL;

	mod_hdr			= NULL;				/* Data module header */
	
	alarmed			=					/* Flags and counters */
	com_buf_full	=
	data_mod		= 
	debug			= 
	echo			=
	line_dirty		=
	line_num		=
	n_retries		=
	n_hexchars		=
	not_valid		=
	port_ready		=
	port_spec		=
	quiet			=
	quit_sig		=
	retries			=
	retry			=
	sh_port			=
	show_ctrl_chars	=
	show_data_chars	=
	show_hex_vals	=
	timeout			=
	verbose			= 0;

	com_buf_empty	= 1;
	cur_col			= 1;

	alrm_id			= 0;				/* Alarm ID for timeout */
	delim			= 0;
	
	aborts			=					/* Abort strings */
	if_abort		=
	last_abort		=
	last_if_abort	= NULL;

	mask_high_bit	= 1;

	status			= SUCCESS;			/* Error code */
}

void parse_args( int argc, char* argv[] )
{
	char*	arg;

	while( --argc )
	{
		if( *(arg = *++argv) == '-' )
		{
			while( arg && *++arg != '\0' )
			{
				switch( tolower(*arg) )
				{
				case '?':
					show_help( SUCCESS );
					break;
				case 'c':
					show_ctrl_chars = 1;
					break;
				case 'd':
					data_mod = 1;
					break;
				case 'e':
					echo = 1;
					break;
				case 'g':
					debug = 1;
					break;
				case 'h':
					show_cmds( arg );
					break;
				case 'l':
					parse_arg_name( arg, log_file, "logfiles" );
					break;
				case 'p':
					DPRT( "Port Name: %s \n", arg );
					parse_arg_name( arg, comport_name, "ports" );
					DPRT( "Port Name: %s \n", comport_name );
					port_spec = 1;
					break;
				case 's':
					sh_port = 1;
					break;
				case 'v':
					verbose = 1;
					break;
				case 'z':
					if( *(arg+1) == '=' )
					{
						check_cmds();
						cmd_file = arg+2;
						arg = NULL;
					}
					else
					{
						check_cmds();
						cmd_path = stdin;
					}
					break;
				default:
					printf( "Unrecognized option '-%c' \n", *arg );
					show_help( E_PARAM );
					break;
				}
			}
		}
		else	/* name of protocol stack */
		{
			if( driver_name )
			{
				printf( "Cannot specify multiple drivers \n" );
				show_help( E_PARAM );
			}

			driver_name = arg;
		}
	}
}

void run_script( void )
{
	/* Parse the command file */

	if( cmd_path )
	{
		VPRT( "Reading from stdin \n" );
		parse_stream( cmd_path );
	}

	else if( cmd_file )
	{
		if( data_mod )
		{
			parse_datamod( cmd_file );
		}
		else
		{
			parse_file( cmd_file );
		}
	}
}

void parse_datamod( char* name )
{
	u_int16	ar = 0,
			tl = 0;
	char	c,
			*s,
			*t,
			*mod_data,
			module_name[MAX_NAME_LEN],
			buf[MAX_LINE_LEN];

	if( ! name )
	{
		return;
	}

	/* Fix up the module name */

	s = skipwhite( name );	/* Strip leading spaces */
	t = module_name;

	while( *s > ' ' )
	{
		*t++ = *s++;
	}

	*t = '\0';
	t = module_name;

	VPRT( "Reading from data module %s \n", module_name );

	status = _os_link( &t, (mh_com**) &mod_hdr, (void**) &mod_data, &tl, &ar );

	if( status != SUCCESS )
	{
		printf( "Can't link to module %s \n", module_name );
		show_error( "_os_link", status );
	}

	s = (char*) mod_data;

	while( *s != '\0' )
	{
		t = buf;

		do
		{
			c = *s++;

			if( c != '\n' && c != 0x0a && c != '\0' )
			{
				*t++ = c;
			}

		} while( c != '\n' && c != 0x0a && c != '\0' );

		*t++ = '\n';
		*t = '\0';

		parse_line( buf );

		while( retry )
		{
			retry = 0;
			parse_line( last_send );
			parse_line( last_expect );
		}
	}

	_os_unlink( mod_hdr );
}

void parse_file( char* name )
{
	/*char	*s, buf[MAX_LINE_LEN];*/

	VPRT( "Reading from file %s \n", name );

	src_fp = fopen( name, "r" );

	if( ! src_fp )
	{
		printf( "Can't open file %s \n", name );
		show_error( "fopen", E_FNA );
	}

	parse_stream( src_fp );
	fclose( src_fp );
}

void parse_stream( FILE* fp )
{
	char	*s, buf[MAX_LINE_LEN];

	while( (s = fgets( buf, MAX_LINE_LEN, fp )) )
	{
		parse_line( s );

		while( retry )
		{
			retry = 0;
			parse_line( last_send );
			parse_line( last_expect );
		}
	}
}

void parse_line( char* line )
{
	char*	word;
	int		i = 0;

	if( ! line || line[0] == '\0' )
	{
		return;
	}

	clean_line();
	DPRT( "Line = <%s>\n", line );

	strcpy( cur_line, line );
	line_num++;

	if( strlen(line) < 1 || *line == '#' )
	{
		return;
	}

	word = next_token( line, " \t.,=\n\0" );

	if( ! word )
	{
		return;
	}

	while( cmds[i].token )
	{
		if( strnccmp( word, cmds[i].token ) == 0 )
		{
			cmds[i].fn();
			return;
		}

		i++;
	}

	invalid_cmd();
}

void do_abort_cmd( void )
{
	Chat_str	this;

	this = new_string( next_token( NULL, "'\n\0" ) );

	if( last_abort )
	{
		last_abort->next = this;
	}

	else
	{
		aborts = this;
	}

	last_abort = this;

	if( debug )
	{
		int	num = 0;

		this = aborts;

		while( this )
		{
			printf( "ABORT[%u] = %s \n", num++, this->str );
			this = this->next;
		}
	}
}

void do_expect_cmd( void )
{
	u_int8		got_str = 0;
	/* Make u_int32 to avoid alignment issues. */
	u_int32     buf[(MAX_LINE_LEN + sizeof(chat_str))/sizeof(u_int32)];
	char		c;
	int			sig;
	Chat_str	expect;

	expect = (Chat_str) buf;

	parse_string( next_token( NULL, "'\n\0" ), expect->str, 0 );
	expect->cursor = expect->str;

	if( verbose )
	{
		clean_line();
		printf( "Expect( %s ) \n", expect->str );
		line_dirty = 1;
	}

	strcpy( last_expect, cur_line );

	if( *(expect->cursor) == '\0' )
	{
		got_str = 1;
	}

    set_alarm( timeout );

	while( ! alarmed && ! got_str )
	{
		c = rd_modem_char();

		if( c != 0 )
		{
			sig = check_aborts( c );

			if( sig )
			{
				term_prog( sig );
			}

			if( match_string( expect, c ) )
			{
				got_str = 1;
			}
		}

		if( quit_sig )
		{
			VPRT( "chat: Terminate signal received. \n" );
			term_prog( E_SIGABRT );
		}
	}

	sig = check_alarm( "Read timed out" );

	if( sig == E_SIGALRM )
	{
		return;
	}

	else if( sig )
	{
		term_prog( sig );
	}

	if( got_str )
	{
		if( verbose )
		{
			clean_line();
			printf( "Got expected string \n" );
		}
	}

	else
	{
		printf( "expect loop ended prematurely \n" );
	}
}

void do_exp_data_cmd( void )
{
	char	*word, *oword, *cursor;
	u_int8	c, got_str = 0, val;
	int		sig;

	word = next_token( NULL, "' \t\n\0" );

	if( ! word )
	{
		return;
	}

	if( verbose )
	{
		clean_line();
		printf( "Expect data( %s ) \n", word );
		line_dirty = 1;
	}

	strcpy( last_expect, cur_line );

	if( *word == '\0' )
	{
		got_str = 1;
	}

    set_alarm( timeout );
	oword = word;

	while( ! alarmed && ! got_str )
	{
		mask_high_bit=0;
		c = (u_int8) rd_modem_char();
		mask_high_bit=1;

		if( c != 0 )
		{
			sig = check_aborts( c );

			if( sig )
			{
				term_prog( sig );
			}

			if( isxdigit(word[0]) && isxdigit(word[1]) )
			{
				/* Get next two hex digits */

				val = (u_int8) atox( word, 2, &cursor );

				if( c == val )	/* Advance pointer */
				{
					word = cursor;
				}
				else			/* Reset pointer */
				{
					word = oword;
				}
			}
			else	/* Ran out of hex digits */
			{
				got_str = 1;
			}

		}	/* End if char from modem is valid */

		if( quit_sig )
		{
			VPRT( "chat: Terminate signal received. \n" );
			term_prog( E_SIGABRT );
		}
	}

	sig = check_alarm( "Read timed out" );

	if( sig == E_SIGALRM )
	{
		return;
	}

	else if( sig )
	{
		term_prog( sig );
	}

	if( got_str )
	{
		if( verbose )
		{
			clean_line();
			printf( "Got expected string \n" );
		}
	}
	else
	{
		printf( "expect loop ended prematurely \n" );
	}
}

void do_if_abort_cmd( void )
{
	Chat_str	this;

	this = new_string( next_token( NULL, "'\n\0" ) );

	if( last_if_abort )
	{
		last_if_abort->next = this;
	}
	else
	{
		if_abort = this;
	}

	last_if_abort = this;

	if( debug )
	{
		int	num = 0;

		this = aborts;

		while( this )
		{
			printf( "ABORT[%u] = %s \n", num++, this->str );
			this = this->next;
		}
	}
}

void do_port_rdy_cmd( void )
{
	port_ready = get_boolean();

	if( debug )
	{
		clean_line();
		printf( "PORT_READY %u \n", port_ready );
	}
}

void do_retry_cmd( void )
{
	retries = get_integer();
	n_retries = 0;

	if( debug )
	{
		clean_line();
		printf( "RETRY %u \n", retries );
	}
}

void do_query_cmd( void )
{
	char*			s, buf[MAX_LINE_LEN];
	int				c, len = 0, sig;
	Device_options	opts;

	s = next_token( NULL, "'\n\0" );

	if( s )
	{
		fprintf( stdout, "%s", s );
	}

	term = fileno( stdin );
	opts = sv_devopt( term );

	port_noecho( term );

	s = buf;

	do
	{
		c = getc( stdin );

		*s++ = (char) c;
		len++;

	} while( c != 0x0d && len < MAX_LINE_LEN );

	*s++ = '\0';

	rst_devopt( opts );

	s = buf;

    set_alarm( timeout );

	if( verbose )
	{
		clean_line();
		fprintf( logfp, "send( " );
		fflush( logfp );
	}

	if( s )
	{
		wr_modem_string( s );
	}

	if( verbose )
	{
		fprintf( logfp, " ) \n" );
	}

	sig = check_alarm( "Write timed out" );

	if( sig == E_SIGALRM )
	{
		return;
	}
	else if( sig )
	{
		term_prog( sig );
	}
}

void do_quiet_cmd( void )
{
	quiet = get_boolean();

	if( debug )
	{
		clean_line();
		printf( "QUIET_MODE %u \n", quiet );
	}
}

void do_send_cmd( void )
{
	char	cur_chr, *s;
	int		sig, add_return = 1;

	strcpy( last_send, cur_line );
	s = next_token( NULL, "'\n\0" );

    set_alarm( timeout );

	if( verbose )
	{
		clean_line();
		fprintf( logfp, "send( " );
		fflush( logfp );
	}

	/* Clear the modem command buffer. */

	/* Parse the string and transmit chars to modem */

	while( ! alarmed && s && *s != '\0' )
	{
		if( (cur_chr = *s++) == '^' )
		{
			if( (cur_chr = *s++) == '\0' )
			{
				wr_modem_char( '^' );
				break;
			}

			cur_chr &= 0x1F;

			if( (cur_chr &= 0x1F) != 0 )
			{
				wr_modem_char( cur_chr );
			}

			continue;
		}

		if( cur_chr != '\\' )
		{
			wr_modem_char( cur_chr );
			continue;
		}

		/*** Handle escape sequence '\?' ***/

		if( (cur_chr = *s++) == '\0' )
		{
			wr_modem_char( '\\' );
			wr_modem_char( '\\' );
			break;
		}

		switch( cur_chr )
		{
		case 'b':	/* Send break character */
			status = send_break( com_wr );
			break;
		case 'p':	/* Pause 0.3 second */
			tsleep( 77 );
			break;
		case 'c':	/* Don't add carriage return. Valid only at end of line. */
			if( *s == '\0' )
			{
				add_return = 0;
			}
			else
			{
				wr_modem_char( cur_chr );
			}
			break;
		case 'n':	/* Insert newline */
			wr_modem_char( '\n' );
			break;
		case 'N':
			wr_modem_char( '\0' );
			break;
		case 'q':	/* Toggle quiet mode */
			quiet = ! quiet;
			break;
		case 'r':	/* Insert carriage return */
			wr_modem_char( '\r' );
			break;
		case 's':	/* Insert space */
			wr_modem_char( ' ' );
			break;
		case 't':	/* Insert tab */
			wr_modem_char( '\t' );
			break;

		default:
			if( isodigit(cur_chr) )
			{
				int	n = 0, val = 0;

				while( (n++ < 3) && isodigit(cur_chr) )
				{
					val <<= 3;
					val |= (cur_chr - '0');

					cur_chr = *s++;
				}

				if( val )
				{
					wr_modem_char( val );
				}

				break;
			}

			wr_modem_char( cur_chr );
			break;
		}
	}

	if( add_return )
	{
		wr_modem_char( 0x0d );
	}

	if( verbose )
	{
		fprintf( logfp, " ) \n" );
	}

	sig = check_alarm( "Write timed out" );

	if( sig == E_SIGALRM )
	{
		return;
	}
	else if( sig )
	{
		term_prog( sig );
	}
}

void do_show_ascii( void )
{
	show_hex_vals = 0;
	delim = 0;

	if( debug )
	{
		clean_line();
		printf( "SHOW_ASCII ON \n" );
	}
}

void do_show_data( void )
{
	show_data_chars = get_boolean();

	if( debug )
	{
		clean_line();
		printf( "SHOW_DATA %u \n", show_data_chars );
	}
}

void do_show_hex( void )
{
	char	*word, *cursor;

	show_hex_vals = 1;
	n_hexchars = 0;
	delim = 0;

	word = next_token( NULL, " \t,\n" );

	if( word )
	{
		if( isxdigit(word[0]) && isxdigit(word[1]) )
		{
			/* Get next two hex digits */

			delim = (u_int8) atox( word, 2, &cursor );
		}
	}

	if( debug )
	{
		clean_line();
		printf( "SHOW_HEX ON, delimiter = %x \n", delim );
	}
}

void do_timeout_cmd( void )
{
	timeout = get_integer();

	if( debug )
	{
		clean_line();
		printf( "TIMEOUT %u \n", timeout );
	}
}

void do_wait_cmd( void )
{
	u_int32		ticks, delay = get_integer();

	if( debug )
	{
		clean_line();
		printf( "WAIT %u \n", delay );
	}

	else if( verbose )
	{
		clean_line();
		printf( "wait( %u ) \n", delay );
	}

	ticks = SECONDS( delay );

	do
	{
#ifdef _OSK
		_os9_sleep( &ticks );
#else
		{
			signal_code	sig;
			_os_sleep( &ticks, &sig );
		}
#endif
	
	} while( ticks );

	DPRT( "WAIT done \n" );
}

void signalHandler( signal_code sig )
{
	if( verbose )
	{
		clean_line();
		printf( "chat: Received signal %u.\n", sig );
	}

	switch( sig )
	{
	case 2:
	case 3:
		quit_sig = 1;
    	break;

	case ALARM_SIG:
		alarmed = 1;
		break;

	default:
		break;
    }

    _os_rte();
}

void show_help( status_code err )
{
	printf( _help );
	not_valid = 1;

	if( err != SUCCESS )
	{
		term_prog( err );
	}
}

void show_error( char* fn, error_code status )
{
	if( status != SUCCESS )
	{
		printf( "Error - %s returned %x.\n", fn, status );
		term_prog( status );
	}
}

void invalid_cmd( void )
{
	clean_line();
	printf( "Invalid command at line %u \n", line_num );
	printf( "    %s \n\n", cur_line );

	term_prog( E_PARAM );
}

void show_cmds( char* s )
{
	printf( _chat_cmds );
	not_valid = 1;
}

void term_prog( status_code err )
{
	intercept( 0 );
	clean_line();

	if( if_abort )
	{
		Chat_str	p, p1 = if_abort;

		printf( "Handling if_abort strings \n" );

		while( p1 )
		{
			if( err == E_SIGABRT && p1->str )
			{
				if( 0 ) printf( "SEND: <%s> \n", p1->str );
				wr_modem_string( p1->str );
				wr_modem_char( 0x0d );
			}

			p = p1;
			p1 = p1->next;
			free( p );
		}
	}

	clean_line();

	if( aborts )
	{
		Chat_str	p, p1 = aborts;

		printf( "Clearing abort strings \n" );

		while( p1 )
		{
			p = p1;
			p1 = p1->next;
			free( p );
		}
	}

	rst_devopts();

	if( port_ready && verbose ) printf( "Port is ready. \n" );

	if( com_wr )
	{
		VPRT( "Closing write path for device %s. \n", comport_name);
		_os_close( com_wr );
	}

	if( com_rd )
	{
		VPRT( "Closing read path for device %s. \n", comport_name);
		_os_close( com_rd );
	}

	if( driver_path )
	{
		ppp_port_pb	port_params;

		VPRT( "Configuring device for normal operation. \n" );

		memset( &port_params, 0, sizeof(port_params) );
		port_params.spb.code = PPP_SS_PORT_DONE;
		port_params.spb.updir = SPB_GOINGDWN;
		port_params.port_ready = port_ready;

		status = _os_setstat( driver_path, SS_SPF, &port_params );

		DPRT( "_os_setstat() returned %x \n", status );
		VPRT( "Closing device %s. \n", driver_name );

		_os_close( driver_path );
	}

	if( src_fp )	
	{
		fclose( src_fp );
	}

	if( mod_hdr )
	{
		_os_unlink( mod_hdr );
	}

	if( logfp != stderr )
	{
		fclose( logfp );
	}

	VPRT( "chat done. Status code = %x \n", err );

	_os_exit( err );
}

void clean_line( void )
{
	if( line_dirty )
	{
		printf( "\n" );
	}

	line_dirty = 0;
}

void show_port( Ppp_modem_params mp )
{
	Device_options	rx, tx;
	char			tmp[MAXLOGSTR];

	rx = sv_devopt( com_rd );
	tx = sv_devopt( com_wr );

	printf( "  Serial Port Settings for %s \n", comport_name );

	if( driver_name )
	{
		print_txt( DE_S, DE_E, J_CENTER, driver_name, False );
	}
	else
	{
		print_txt( DE_S, DE_E, J_CENTER, "-", False );
	}

	sprintf( tmp, "%s (Rx)", comport_name );
	print_txt( RX_S, RX_E, J_CENTER, tmp, False );

	sprintf( tmp, "%s (Tx)", comport_name );
	print_txt( TX_S, TX_E, J_CENTER, tmp, True );

	memset( tmp, '-', 60 );
	tmp[60] = '\0';
	puts( tmp );

	print_vals( "Baud Rate:", "%x", mp->val.baud_rate, BAUD(rx), BAUD(tx) );
	print_vals( "Parity:", "%u", mp->val.parity, PARITY(rx), PARITY(tx) );
	print_vals( "Word Size:", "%u", mp->val.word_size, WORDS(rx), WORDS(tx) );
	print_vals( "Stop Bits:", "%u", mp->val.stop_bits, STOPS(rx), STOPS(tx) );
	print_vals( "RTS Enable:", "%u", mp->val.rts_enable, RTS(rx), RTS(tx) );

	/* Flags */

	print_vals( "Case:", "%u", -1, CASE(rx), CASE(tx) );
	print_vals( "BackSpace:", "%u", -1, BSP(rx), BSP(tx) );
	print_vals( "Delete:", "%u", -1, DELETE(rx), DELETE(tx) );
	print_vals( "Echo:", "%u", -1, ECHO(rx), ECHO(tx) );
	print_vals( "AutoLF:", "%u", -1, ALF(rx), ALF(tx) );
	print_vals( "Pause:", "%u", -1, PAUSE(rx), PAUSE(tx) );
	print_vals( "InsertMode:", "%u", -1, INSM(rx), INSM(tx) );

	/* Characters */

	print_vals( "BSP Char:", "%u", -1, BSPC(rx), BSPC(tx) );
	print_vals( "DEL Char:", "%u", -1, DELC(rx), DELC(tx) );
	print_vals( "EOR Char:", "%u", -1, EORC(rx), EORC(tx) );
	print_vals( "EOF Char:", "%u", -1, EOFC(rx), EOFC(tx) );
	print_vals( "RPR Char:", "%u", -1, RPRC(rx), RPRC(tx) );
	print_vals( "DUP Char:", "%u", -1, DUPC(rx), DUPC(tx) );
	print_vals( "PAUSE Char:", "%u", -1, PSCH(rx), PSCH(tx) );
	print_vals( "INTR Char:", "%u", -1, INTR(rx), INTR(tx) );
	print_vals( "QUIT Char:", "%u", -1, QUIT(rx), QUIT(tx) );
	print_vals( "BSE Char:", "%u", -1, BSEC(rx), BSEC(tx) );
	print_vals( "TAB Char:", "%u", -1, TABC(rx), TABC(tx) );

	free( rx );
	free( tx );
}

void print_vals( char* lbl, char* fmt, int v1, int v2, int v3 )
{
	print_txt( LBL_S, LBL_E, J_RIGHT, lbl, False );
	print_val( DE_S, DE_E, v1, fmt, False );
	print_val( RX_S, RX_E, v2, fmt, False );
	print_val( TX_S, TX_E, v3, fmt, True );
}

void print_val( int start, int stop, int v, char* fmt, int nl )
{
	if( v == -1 )
	{
		print_txt( start, stop, J_CENTER, "-", nl );
	}
	else
	{
		print_int( start, stop, J_CENTER, v, fmt, nl );
	}
}

void print_txt( int start, int end, int justify, char* str, int nl )
{
	int	fw, sw, tpad, lpad, rpad, no_nl = 1;

	fw = (end - start + 1);
	sw = MIN( fw, strlen(str) );
	tpad = (fw - sw);

	switch( justify )
	{
	case J_LEFT:
		lpad = 0;
		rpad = tpad;
		break;
	case J_RIGHT:
		lpad = tpad;
		rpad = 0;
		break;
	case J_CENTER:
	default:
		lpad = (tpad / 2);
		rpad = (tpad - lpad);
		break;
	}

	/* Get cursor over to start column. */

	while( cur_col < start )
	{
		putchar( ' ' );
		cur_col++;
	}

	/* Add left padding, if any. */

	while( lpad-- )
	{
		putchar( ' ' );
		cur_col++;
	}

	/* Add string */

	while( sw-- )
	{
		putchar( *str );
		cur_col++;

		if( *str++ == '\n' )
		{
			cur_col = 1;
			no_nl = 0;
		}
	}

	/* Add right padding, if any. */

	while( rpad-- )
	{
		putchar( ' ' );
		cur_col++;
	}

	if( nl && no_nl )
	{
		putchar( '\n' );
		cur_col = 1;
	}
}

void print_int( int st, int end, int just, u_int32 val, char* fmt, int nl )
{
	char	tmp[MAXLOGSTR];

	sprintf( tmp, fmt, val );
	print_txt( st, end, just, tmp, nl );
}
