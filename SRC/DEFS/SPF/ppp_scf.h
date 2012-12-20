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
 *  01 02/22/96 Initial                                                billw
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#ifndef _ppp_scf_h
#define _ppp_scf_h

	#include <const.h>
	#include <errno.h>
	#include <memory.h>
	#include <modes.h>
	#include <scf.h>
	#include <sg_codes.h>
	#include <string.h>
	#include <types.h>

	#if defined(_OSK)
    	typedef struct scf_opt  scf_opt;
	#endif

	/************************************/
	/* Structure for SCF device options */
	/************************************/

	typedef struct device_opts	device_opts, *Device_opts;

	struct device_opts
	{
		path_id			pd;
		u_char			dev_type;

		u_int8			baud_rate;		/* (1) Serial port baud rate	*/
		u_int8			parity;			/* (2) Serial port parity		*/
		u_int8			word_size;		/* (3) Serial port word size	*/
		u_int8			stop_bits;		/* (4) Serial port stop bits	*/
		u_int8			rts_enable;		/* (5) Serial port RTS control	*/

	#if defined(_OSK)
		scf_opt			path_opts;
	#elif defined(_OS9000)
		scf_path_opts	path_opts;
		scf_lu_opts		lu_opts;
	#endif
	};


	#ifdef __cplusplus
		extern "C" {
	#endif

	error_code open_dev( char* name, u_int32 mode, path_id* path );
	error_code save_devopt( Device_opts opts );
	error_code restore_devopt( Device_opts opts );
	error_code port_raw( Device_opts opts );
	error_code attach_dev( char* name );
	error_code clear_device( path_id pid );
	void set_accm_bit( u_int32* accmptr, u_int8 opt );
	error_code update_accm( Device_opts opts, u_int32* accmptr );

	#ifdef __cplusplus
		}
	#endif


#endif	/* _ppp_scf_h */
