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

#ifndef _chat_h_
#define _chat_h_

#include <alarm.h>
#include <cglob.h>
#include <const.h>
#include <ctype.h>
#include <errno.h>
#include <io.h>
#include <memory.h>
#include <modes.h>
#include <stdlib.h>
#include <types.h>

#if defined(_OSK)
	#include <os9def.h>
#endif

#include <scf.h>

#include <sg_codes.h>
#if defined(_OSK)
#include <sgstat.h>
#endif
#include <stdio.h>
#include <string.h>
#include <types.h>
#include <scf.h>

#include <SPF/ppp.h>
#include <SPF/ppp_scf.h>

enum { ALARM_SIG = 10000, DATA_READY, TERM_READY };

#define SECONDS(s)		(0x80000000 | (s * 256))

#define TMP_BUF_SIZE	32
#define MAX_LINE_LEN	256
#define	COM_BUF_LEN		1024
#define	STR_LEN			1024
#define MAX_ABORTS		6

#define LBL_S		1
#define LBL_E		12
#define DE_S		14
#define DE_E		24
#define RX_S		26
#define RX_E		36
#define TX_S		38
#define TX_E		48

#define MAXLOGSTR	80

#define J_LEFT		1
#define J_RIGHT		2
#define J_CENTER	3

#ifndef False
	#define False		0
#endif

#ifndef True
	#define True		1
#endif

#define VPRT	if( verbose ) printf
#define DPRT	if( debug ) printf

#define isodigit(c)	( c >= '0' && c <= '7' ? 1 : 0 )


typedef struct cmd_pair		cmd_pair, *Cmd_pair;
typedef struct chat_string	chat_str, *Chat_str;


typedef struct device_options   device_options, *Device_options;

struct device_options
{
    Device_options  next;
    Device_options  prev;
    device_opts     dopts;
};

struct cmd_pair
{
	char*	token;
	void	(*fn)(void);
};

struct chat_string
{	
	Chat_str	next;
	char*		cursor;
	char		str[1];
	char        pad[3];		/* Add padding to make 4 byte aligned */
};

#define parse_arg_name( arg, ptr, msg ) \
	if( *(arg+1) == '=' ) \
	{ \
		if( ptr ) \
		{ \
			printf( "Cannot specify multiple %s. \n", msg ); \
			show_help( E_PARAM ); \
		} \
		else \
		{ \
			ptr = arg+2; \
			arg = NULL; \
		} \
	} \
	else \
	{ \
		printf( "Unrecognized option '-%c' \n", *arg ); \
		show_help( E_PARAM ); \
	}


#if defined(_OSK)
	#define BAUD(p)			p->dopts.path_opts.pd_bau
	#define PARITY(p)		p->dopts.path_opts.pd_par
	#define STOPS(p)		-1
	#define WORDS(p)		-1
	#define RTS(p)			( ! p->dopts.path_opts.pd_xon )
	/* Flags */
	#define CASE(p)			p->dopts.path_opts.pd_upc
	#define BSP(p)			p->dopts.path_opts.pd_bso
	#define DELETE(p)		p->dopts.path_opts.pd_dlo
	#define ECHO(p)			p->dopts.path_opts.pd_eko
	#define ALF(p)			p->dopts.path_opts.pd_alf
	#define PAUSE(p)		p->dopts.path_opts.pd_pau
	#define INSM(p)			-1
	/* Characters */
	#define BSPC(p)			p->dopts.path_opts.pd_bsp
	#define DELC(p)			p->dopts.path_opts.pd_del
	#define EORC(p)			p->dopts.path_opts.pd_eor
	#define EOFC(p)			p->dopts.path_opts.pd_eof
	#define RPRC(p)			p->dopts.path_opts.pd_rpr
	#define DUPC(p)			p->dopts.path_opts.pd_dup
	#define PSCH(p)			p->dopts.path_opts.pd_psc
	#define INTR(p)			p->dopts.path_opts.pd_int
	#define QUIT(p)			p->dopts.path_opts.pd_qut
	#define BSEC(p)			p->dopts.path_opts.pd_bse
	#define BELL(p)			p->dopts.path_opts.pd_ovf
	#define TABC(p)			p->dopts.path_opts.pd_Tab
#elif defined(_OS9000)
	#define BAUD(p)			p->dopts.lu_opts.v_baud
	#define PARITY(p)		p->dopts.lu_opts.v_parity
	#define STOPS(p)		p->dopts.lu_opts.v_stopbits
	#define WORDS(p)		p->dopts.lu_opts.v_wordsize
	#define RTS(p)			p->dopts.lu_opts.v_rtsstate
	/* Flags */
	#define CASE(p)			p->dopts.path_opts.pd_case
	#define BSP(p)			p->dopts.path_opts.pd_backsp
	#define DELETE(p)		p->dopts.path_opts.pd_delete
	#define ECHO(p)			p->dopts.path_opts.pd_echo
	#define ALF(p)			p->dopts.path_opts.pd_alf
	#define PAUSE(p)		p->dopts.path_opts.pd_pause
	#define INSM(p)			p->dopts.path_opts.pd_insm
	/* Characters */
	#define BSPC(p)			-1
	#define DELC(p)			-1
	#define EORC(p)			p->dopts.path_opts.pd_eorch
	#define EOFC(p)			p->dopts.path_opts.pd_eofch
	#define RPRC(p)			-1
	#define DUPC(p)			-1
	#define PSCH(p)			p->dopts.lu_opts.v_psch
	#define INTR(p)			p->dopts.lu_opts.v_intr
	#define QUIT(p)			p->dopts.lu_opts.v_quit
	#define BSEC(p)			p->dopts.path_opts.pd_bspch
	#define BELL(p)			p->dopts.path_opts.pd_bellch
	#define TABC(p)			p->dopts.path_opts.pd_tabch
#endif

/*********************************************
 **** Function prototypes for file chat.c ****
 *********************************************/
void main( int argc, char* argv[] );
void initialize( void );
void parse_args( int argc, char* argv[] );
void run_script( void );
void parse_datamod( char* name );
void parse_file( char* name );
void parse_stream( FILE* fp );
void parse_line( char* line );
void do_abort_cmd( void );
void do_expect_cmd( void );
void do_exp_data_cmd( void );
void do_if_abort_cmd( void );
void do_port_rdy_cmd( void );
void do_retry_cmd( void );
void do_query_cmd( void );
void do_quiet_cmd( void );
void do_send_cmd( void );
void do_show_ascii( void );
void do_show_data( void );
void do_show_hex( void );
void do_timeout_cmd( void );
void do_wait_cmd( void );
void signalHandler( signal_code sig );
void show_help( status_code err );
void show_error( char* fn, error_code status );
void invalid_cmd( void );
void show_cmds( char* s );
void term_prog( status_code err );
void clean_line( void );
void show_port( Ppp_modem_params mp );
void print_vals( char* lbl, char* fmt, int v1, int v2, int v3 );
void print_val( int start, int stop, int v, char* fmt, int nl );
void print_txt( int start, int end, int justify, char* str, int nl );
void print_int( int st, int end, int just, u_int32 val, char* fmt, int nl );


/*************************************************
 **** Function prototypes for file chat_dev.c ****
 *************************************************/

void do_open( char* name, u_int32 mode, path_id* pid );
Device_options sv_devopt( path_id pd );
void sv_devopts( path_id pd );
void rst_devopt( Device_options opts );
void rst_devopts( void );
void port_noecho( path_id pd );
void pt_raw( path_id pd, Ppp_modem_params mp );
char rd_modem_char( void );
void wr_modem_char( char c );
void wr_modem_string( char* s );
error_code send_break( path_id path );
error_code read_char( path_id path, unsigned char* buf );

/*************************************************
 **** Function prototypes for file chat_utl.c ****
 *************************************************/
int match_string( Chat_str p, char c );
char* next_token( char* src, char* delim );
void get_next_word( char* buf, int maxlen );
int get_boolean( void );
int get_integer( void );
int check_integer( char* word, int* valid );
void check_cmds( void );
int check_aborts( char c );
char* skipwhite( char* s );
int strnccmp( char* s, char* t );
int strnnccmp( char* s, char* t );
void parse_string( char* s, char* buf, int sending );
void printqc( FILE* fp, char c );
void printchar( FILE* fp, char c );
void printstring( FILE* fp, char* s );
void set_alarm( u_int32 delay );
void reset_alarm( u_int32 delay );
void clear_alarm( void );
int check_alarm( char* msg );
Chat_str new_string( char* word );
u_int32 atox( char* str, u_int8 numdigits, char** cursor );


int tsleep( unsigned svalue );
int intercept( void (*f)(int) );

#if defined(_OSK)

error_code _os_getstat( path_id, u_int32, void* );
error_code _os_setstat( path_id, u_int32, void* );
error_code _os_intercept( void (*f)(), void* );
error_code _os9_sleep( u_int32* );
error_code _os_rte( void );
error_code _os_exit( status_code );


#endif

#include "chat_glb.h"



#endif /* _chat_h_ */
