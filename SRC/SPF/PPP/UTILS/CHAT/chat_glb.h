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
 *     97/05/05 <--------- SPF LAN Comm Pack v3.1 Release ---------->
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

/*******************************************************/
/*******************************************************/

#ifndef _chat_glb_h
#define _chat_glb_h

	#ifdef MAIN

		#define EXTERN

		cmd_pair cmds[] =
		{
			{ "abort",			do_abort_cmd	},
			{ "expect",			do_expect_cmd	},
			{ "exp_data",		do_exp_data_cmd	},
			{ "if_abort",		do_if_abort_cmd	},
			{ "port_ready",		do_port_rdy_cmd	},
			{ "retry",			do_retry_cmd	},
			{ "query",			do_query_cmd	},
			{ "quiet",			do_quiet_cmd	},
			{ "send",			do_send_cmd		},
			{ "show_ascii",		do_show_ascii	},
			{ "show_data",		do_show_data	},
			{ "show_hex",		do_show_hex		},
			{ "timeout",		do_timeout_cmd	},
			{ "wait",			do_wait_cmd		},
			{ NULL,				NULL			}
		};

		char* _help = \
		"Syntax: chat <opts> <device> <opts> \n"\
		"Purpose: Initialize serial connection for point-to-point. \n"\
		"Options: \n"\
  		"  -?         You already know this one! \n"\
  		"  -c         Display control chars when displaying data \n"\
  		"  -d         Read commands from data module \n"\
  		"  -e         Echo characters received from modem \n"\
  		"  -h         Show chat script commands \n"\
  		"  -l=<name>  Specify logfile name \n"\
  		"  -p=<port>  Specify serial port \n"\
		"  -s         Show serial port settings \n"\
  		"  -v         Show progress info \n"\
  		"  -z         Read commands from stdin \n"\
  		"  -z=<name>  Read commands from file or data module \n";

		char* _chat_cmds = \
		"The following commands may be used in a chat script. \n"\
		"They are not case sensitive: \n\n"\
   		"  abort <string>        - Specify fatal error string \n"\
   		"  expect <string>       - Wait for string from modem \n"\
   		"  exp_data <hex data>   - Wait for data from modem \n"\
   		"  if_abort <string>     - Specify modem command to send on abort \n"\
   		"  port_ready <ON | OFF> - Specify ready status of port \n"\
   		"  retry <val>           - Set retry counter \n"\
   		"  query <prompt>        - Get input from console and send to modem \n"\
   		"  quiet <ON | OFF>      - Enable or disable quiet mode \n"\
   		"  send <string>         - Send string to modem \n"\
		"  show_ascii            - Display data in ascii (default) \n"\
   		"  show_data <ON | OFF>  - Enable/disable display of received data \n"\
		"  show_hex [delimiter]  - Display data in hex \n"\
   		"  timeout <seconds>     - Set a timer \n"\
   		"  wait <seconds>        - Pause specified number of seconds \n";

		char*	esc_codes[] =
		{
			"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", 
			"BS", "HT", "LF", "VT", "FF", "CR", "SO", "SI", "DLE",
			"DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN",
			"EM", "SUB", "ESC", "FS", "GS", "RS", "US"
		};

	#else	/* ! MAIN */

		#define EXTERN	extern

		EXTERN cmd_pair cmds[];
		EXTERN char 	_help[];
		EXTERN char 	_chat_cmds[];
		EXTERN char*	esc_codes[];

	#endif	/* ! MAIN */

	EXTERN char			*driver_name,
						*comport_name,
						*log_file,
						*cmd_file,
						*cmd_mod;

	EXTERN char			cur_line[MAX_LINE_LEN],
						last_expect[MAX_LINE_LEN],
						last_send[MAX_LINE_LEN],
						last_rx_char;

	EXTERN u_int8		com_buf[COM_BUF_LEN],
						delim,
						*com_buf_end,
						*com_buf_inp,
						*com_buf_outp;

	EXTERN path_id		driver_path,
						com_wr,
						com_rd,
						term;

	EXTERN Dev_list		driver_tbl,
						com_tbl;

	EXTERN Device_options	devopt_list,
							devopt_last;

	EXTERN FILE			*logfp,		/* Log file pointer */
						*cmd_path,
						*src_fp;

	EXTERN mh_com		*mod_hdr;

	EXTERN int			alarmed,
						com_buf_empty,
						com_buf_full,
						cur_col,
						data_mod,
						debug,
						echo,
						line_dirty,
						line_num,
						n_retries,
						n_hexchars,
						not_valid,
						port_ready,
						port_spec,
						quiet,
						quit_sig,
						retries,
						retry,
						sh_port,
						show_ctrl_chars,
						show_data_chars,
						show_hex_vals,
						timeout,
						verbose,
						mask_high_bit;

	EXTERN alarm_id		alrm_id;

	EXTERN Chat_str		aborts,
						if_abort,
						last_abort,
						last_if_abort;

	EXTERN error_code	status;


#endif	/* _chat_glb_h */
