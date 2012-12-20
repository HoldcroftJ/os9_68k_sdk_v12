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
 * 31  15/12/00 Snarfed from old pppd.                                  sr   *
 *     01/09/01 Clean up.                                               sr   *
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
#ifndef _pppd_h_
#define _pppd_h_


#include <cglob.h>
#include <const.h>
#include <ctype.h>
#include <errno.h>
#include <io.h>
#include <modes.h>
#include <process.h>
#include <sg_codes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <SPF/item.h>
#include <SPF/ppp.h>
#include <SPF/spf.h>


#define TMP_BUF_SIZE	128
#define MAX_LINE_LEN	256
#define MAX_DEVICES		6
#define MAX_PATHS		20
#define MAX_TOKENS		6
#define MAX_TOKEN_LEN	32
#define	STR_LEN			1024

typedef struct devp		devp, *Devp;

struct devp
{
	char			name[TMP_BUF_SIZE];
	signal_code		up_signal,
					down_signal;
	u_int8			stk_ioenabled;
	u_int32			protocol;
	Devp			next_device,
					prev_device;

};

typedef struct
{
	double		value;
	u_int8		baud;

} baud_pair;

typedef struct
{
	char*	token;
	void	(*fn)(void);
	
} cmd_pair;

typedef struct
{
	u_int32		in_use	: 1;
	u_int32		unused	: 31;

} pppd_stk_flags, *Pppd_stk_flags;

typedef struct
{
	char			stack_name[TMP_BUF_SIZE];
	u_int32			index;
	process_id		proc_id;
	pppd_stk_flags	flags;

} pppd_stack, *Pppd_stack;

typedef struct
{
	u_int32		num_entries;
	pppd_stack	stacks[MAX_PATHS];

} pppd_paths, *Pppd_paths;



#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

	/*void main( int argc, char* argv[] );*/
	void kill_daemon( void );

	void check_drvr_enabled( void );
	void initialize( void );
	void open_drivers( void );
	void update_drivers( path_id );
	void open_stack( void );

	void parse_args( int argc, char* argv[] );
	void parse_datamod( char* name );
	void parse_file( char* name );
	void parse_stream( FILE* fp );
	void parse_line( char* line );

	void do_abort_cmd( void );
	void do_expect_cmd( void );

	void do_send_cmd( void );

	void do_set_cmd( void );
	void do_set_baud( void );
	void do_set_flow( void );
	void do_set_ipcp( void );
	void do_set_mode( void );
	void do_set_parity( void );
	void do_set_rx( void );
	void do_set_scr( void );
	void do_set_stop( void );
	void do_set_str( void );
	void do_set_timeout( void );
	void do_set_tx( void );
	void do_set_word( void );

	void do_timeout_cmd( void );
	void do_wait_cmd( void );

	char* next_token( char* src, char* delim );
	void get_next_word( char* buf, int maxlen );

	int get_boolean( void );
	int get_integer( void );
	int check_integer( char* word, int* valid );

	void print_vars( Ppp_sysparams_pb p );

	void check_cmds( void );

	void cleanup_on_exit( void );
	void term_prog( status_code status );

	void signalHandler( signal_code sig );
	void handle_sig( signal_code sig );

	void show_error( char* fn, error_code status );
	void invalid_cmd( void );

	void show_cmds( char* s );
	void show_set_cmds( char* s );

	char* skipwhite( char* s );
	int strnccmp( char* s, char* t );
	int strnnccmp( char* s, char* t );
	double atof( const char* s );

	double get_decnum_only( char* s );
	char* get_desc_name( char* sname );
	void link_stackmod( char* name );
	void check_stackmod( void );
	Pppd_stack find_stack( char* name, process_id id );
	Pppd_stack get_stack( void );
	void set_index( void );
	void set_stackname( void );
	void add_stackname( void );
	void del_stackname( Pppd_stack stack );
	void del_stackid( process_id proc_id );
	void init_stacks( void );
	void list_stacks( void );

	int check_proc_desc( process_id pid );

	void prt( char* msg );
	void prt_1u( char* msg, u_int32 val );
	void prt_1s( char* msg, char* val );



	error_code read_protocol_id( Devp device );
	error_code send_ite_hangup( void );
	error_code read_modem_params( Ppp_modem_params params );
	error_code write_modem_params( Ppp_modem_params params );
	error_code read_lcp_params( Ppp_lcp_params params );
	error_code write_lcp_params( Ppp_lcp_params params );
	error_code read_ipcp_params( Ppp_ipcp_params params );
	error_code write_ipcp_params( Ppp_ipcp_params params );
	error_code read_sys_params( Ppp_sys_params params );
	error_code write_sys_params( Ppp_sys_params params );
	void ins_device( Devp device );
	error_code add_notify( Devp device, u_int32 code, int clear_flag );
	void print_old_new( void );



#if defined(_OSK)

	error_code _os_sleep(u_int32 *, signal_code *);
	error_code _os_getstat(path_id, u_int32, void *);
	error_code _os_setstat(path_id, u_int32, void *);

#endif

	int kill( int, int );

#ifdef __cplusplus
}
#endif	/* __cplusplus */


/********************* Program Globals *********************/

char				*hdlc_dev_name = NULL,
					*lcp_dev_name = NULL,
					*ipcp_dev_name = NULL,
					*stack_name = NULL,
					*cmd_file = NULL,
					cur_line[MAX_LINE_LEN];

Devp				devices;

ppp_option_block o_ppp_options = {0};
ppp_option_block n_ppp_options = {0};
ppp_modem_p o_asynch_params = {0};
ppp_modem_p n_asynch_params = {0};

FILE*				cmd_path = NULL;
path_id				stack_path = 0;
process_id			proc_id = 0;
mh_com*				stack_mod = NULL;
Pppd_paths			pppd_stacks;
pr_desc				pppd_desc;


int					delay		= 2,	/* Delay in seconds between updates */
					debug		= 0,
					dmn_index	= 0,	/* index of daemon */
					hangup		= 0,	/* Hang up the phone */
					killd		= 0,	/* Kill daemon */
					listd		= 0,	/* List daemons */
					pppd_done	= 0,
					verbose		= 0,
					quiet		= 0,
					use_datamod	= 0,
					not_valid	= 0,
					line_num	= 0;

baud_pair br_lut[] = 
{
	{ 50.0,		BAUDRATE_50		}, { 75.0,		BAUDRATE_75		},
	{ 110.0,	BAUDRATE_110	}, { 134.5,		BAUDRATE_134_5	},
	{ 150.0,	BAUDRATE_150	}, { 300.0,		BAUDRATE_300	},
	{ 600.0,	BAUDRATE_600	}, { 1200.0,	BAUDRATE_1200	},
	{ 1800.0,	BAUDRATE_1800	}, { 2000.0,	BAUDRATE_2000	},
	{ 2400.0,	BAUDRATE_2400	}, { 3600.0,	BAUDRATE_3600	},
	{ 4800.0,	BAUDRATE_4800	}, { 7200.0,	BAUDRATE_7200	},
	{ 9600.0,	BAUDRATE_9600	}, { 19200.0,	BAUDRATE_19200	},
#if defined(_OS9000)
	{ 31250.0,	BAUDRATE_31250	},
#endif
	{ 38400.0,	BAUDRATE_38400	}, { 56000.0,		BAUDRATE_56000 },
	{ 57600.0,	BAUDRATE_57600	}, { 64000.0,		BAUDRATE_64000 },
	{ 115200.0,	BAUDRATE_115200	}, { 0.0,		0				}
};

cmd_pair cmds[] =
{
	{ "set",		do_set_cmd		},
	{ NULL,			NULL			}
};

cmd_pair set_cmds[] =
{
	{ "baud",		do_set_baud		},
	{ "flow",		do_set_flow		},
	{ "ipcp",		do_set_ipcp		},
	{ "mode",		do_set_mode		},
	{ "parity",		do_set_parity	},
	{ "rx",			do_set_rx		},
	{ "scr",		do_set_scr		},
	{ "stop",		do_set_stop		},
	{ "str",		do_set_str		},
	{ "timeout",	do_set_timeout	},
	{ "tx",			do_set_tx		},
	{ "word",		do_set_word		},
	{ NULL,			NULL		}
};

char _set_cmds[] = "\
The following commands may be used to set device parameters. \n\
They are not case sensitive: \n\n\
   set baud[rate] <rate>          -- Set baud rate (See note 1) \n\
   set flow <RTS | XON>           -- Set flow control (RTS/CTS or X-On/X-Off)\n\
   set ipcp cslot <value>         -- Set IP Compression C-slot \n\
   set ipcp mslot <value>         -- Set IP Compression M-slot \n\
   set ipcp proto[col] <value>    -- Set IP Compression Protocol \n\
   set ipcp defaultroute          -- Set peer as IP default route\n\
   set ipcp accept-remote <value> -- Let peer set the remote IP address\n\
   set ipcp accept-local <value>  -- Let peer set the local IP address\n\
   set mode <option,option...>    -- Set mode flags (See note 5)\n\
   set parity <parity>            -- Set parity mode (See note 2) \n\
   set rx accm <value>            -- Set RX Async Control Character Map \n\
   set rx acfc <value>            -- Set RX Address/Control Field Compression\n\
   set rx buffer <value>          -- Set RX buffer size \n\
   set rx device <name>           -- Set Receive port device \n\
   set rx mru <value>             -- Set RX Max Receive Unit \n\
   set rx pfc <value>             -- Set RX Protocol Field Compression \n\
   set scr <value>                -- Set maximum SCR value \n\
   set stop[bits] <bits>          -- Set number of stop bits (See note 3) \n\
   set str <value>                -- Set maximum STR value \n\
   set timeout <value>            -- Set timeout value \n\
   set tx accm <value>            -- Set TX Async Control Character Map \n\
   set tx acfc <value>            -- Set TX Address/Control Field Compression\n\
   set tx block[size] <value>     -- Set maximum block size for Transmit \n\
   set tx device <name>           -- Set Transmit port device \n\
   set tx mru <value>             -- Set TX Max Receive Unit \n\
   set tx pfc <value>             -- Set TX Protocol Field Compression \n\
   set word[size] <word>          -- Set word size (See note 4) \n\n\
   (1) select rate value from following: \n\
                50     75   110  134.5   150 \n\
               300    600  1200   1800  2000 \n\
              2400   3600  4800   7200  9600 \n\
             19200  31250  MIDI  38400  \n\
       MIDI is only available for OS9000 systems. \n\n\
   (2) select parity value from following: \n\
              None  Odd  Even  Mark  Space \n\n\
   (3) select bits value from following: \n\
                 1  1.5  2 \n\n\
   (4) select word value from following: \n\
                 5  6  7  8 \n\n\
   (5) select options from following: \n\
              nowait  passive  notify  updata  modem  loopback \n\
              norxcomp notxcomp nopap nochap nopfc noacfc \n";

#endif /* _pppd_h_ */
