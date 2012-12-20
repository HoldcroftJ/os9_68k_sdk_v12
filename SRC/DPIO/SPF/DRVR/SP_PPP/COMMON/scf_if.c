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
 *  18 12/15/97 Fixed to stop compiler warnings                        tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#include <SPF/ppp_scf.h>

#if defined(DRIVER)
	#include	"defs.h"
	#include	"lib_func.h"
#else
	#include	<stdio.h>

	#ifdef DBG_SCF
		#define DPRT(s,v)	printf( s, v )
	#endif
#endif

#ifndef DPRT
	#define DPRT(s,v)
#endif


/* Open an SCF device */

error_code open_dev( char* name, u_int32 mode, path_id* path)
{
	error_code	status = SUCCESS;

	status = _os_open( name, mode, path );

#if defined(DRIVER)
	if( status == E_PNNF && string_nccmp( name, "/pipe/", 0 ) == 0 )
	{
		mode |= S_ISIZE;
		status = _os_create( name, mode, path, 0x0333, 20480 );
	}
#endif

	return( status );
}

/* Save device options for an SCF device */

error_code save_devopt( Device_opts opts )
{
	u_int32		size;
	error_code	status;

	DPRT( "save_devopt: \n", 0 );

	/* get path options */

	opts->dev_type = 127;	/* Initialize to unknown device type */

	size = sizeof(opts->path_opts);
	status = _os_gs_popt( opts->pd, &size, &(opts->path_opts) );
	DPRT( "_os_gs_popt returned %x \n", status );

	if( status == E_UNKSVC )
	{
		return( SUCCESS );
	}

	if( status != SUCCESS )
	{
		return( status );
	}

#if defined(_OSK)

	opts->dev_type = opts->path_opts.pd_dtp;

#elif defined(_OS9000)

	/* get logical unit options (OS9000 ONLY!) */

	size = sizeof(opts->lu_opts);
	status = _os_gs_luopt( opts->pd, &size, &(opts->lu_opts) );
	DPRT( "_os_gs_luopt returned %x \n", status );

	if( status == E_UNKSVC ) return( SUCCESS );
	if( status != SUCCESS ) return( status );

	opts->dev_type = opts->lu_opts.v_class;

#endif

	return( SUCCESS );
}

/* Restore original device options for an SCF device */

error_code restore_devopt( Device_opts opts )
{
	u_int32		size;

	DPRT( "restore_devopt: \n", 0 );

	size = sizeof(opts->path_opts);
	_os_ss_popt( opts->pd, size, &(opts->path_opts) );

#if defined(_OS9000)

	size = sizeof(opts->lu_opts);
	_os_ss_luopt( opts->pd, size, &(opts->lu_opts) );

#endif

	return( SUCCESS );
}

/* Set an SCF device to operate in raw mode */

error_code port_raw( Device_opts opts )
{
	device_opts	cur_opts;
	error_code	status;

	DPRT( "port_raw: \n", 0 );

	/* If PPP has baud rate specified as zero, we don't touch port! */

	DPRT( "Specified baud rate = %x \n", opts->baud_rate );

	cur_opts.pd = opts->pd;
	status = save_devopt( &cur_opts );
	DPRT( "save_devopt returned %x \n", status );

	if( status != SUCCESS )
	{
		return( status );
	}

	if( cur_opts.dev_type != DT_SCF )
	{
		DPRT( "Device type = %x, Not SCF \n", cur_opts.dev_type );
		return( SUCCESS );
	}

#if defined(_OS9000)

	/* Set logical unit options */

	if( opts->baud_rate )
	{
		cur_opts.lu_opts.v_baud     = opts->baud_rate;
		cur_opts.lu_opts.v_parity   = opts->parity;
		cur_opts.lu_opts.v_stopbits = opts->stop_bits;
		cur_opts.lu_opts.v_wordsize = opts->word_size;
	}

	cur_opts.lu_opts.v_rtsstate = opts->rts_enable;

	cur_opts.lu_opts.v_intr = 0;        /* keyboard interrupt character */
	cur_opts.lu_opts.v_quit = 0;        /* keyboard quit character      */
	cur_opts.lu_opts.v_psch = 0;        /* keyboard pause character     */

	if( cur_opts.lu_opts.v_rtsstate )   /* Don't need Xon Xoff */
	{
		cur_opts.lu_opts.v_xon  = 0;    /* x-on character               */
		cur_opts.lu_opts.v_xoff = 0;    /* x-off character              */

		status = _os_ss_enrts( opts->pd );
		DPRT( "_os_ss_enrts returned %x \n", status );
	}

	else    /* Do need Xon Xoff */
	{
		cur_opts.lu_opts.v_xon  = 0x11; /* x-on character               */
		cur_opts.lu_opts.v_xoff = 0x13; /* x-off character              */

		status = _os_ss_dsrts( opts->pd );
		DPRT( "_os_ss_dsrts returned %x \n", status );
	}

	/* Set port for RAW mode */

	cur_opts.path_opts.pd_case  = 0;
	cur_opts.path_opts.pd_backsp= 0;
	cur_opts.path_opts.pd_delete= 0;
	cur_opts.path_opts.pd_echo  = 0;
	cur_opts.path_opts.pd_alf   = 0;
	cur_opts.path_opts.pd_pause = 0;
	cur_opts.path_opts.pd_insm  = 0;
	cur_opts.path_opts.pd_bspch = 0;
	cur_opts.path_opts.pd_eofch = 0;
	cur_opts.path_opts.pd_eorch = 0;
	cur_opts.path_opts.pd_tabch = 0;
	cur_opts.path_opts.pd_bellch= 0;

#endif  /* _OS9000 */

#if defined(_OSK)

	cur_opts.path_opts.pd_eko = 0;
	cur_opts.path_opts.pd_alf = 0;
	cur_opts.path_opts.pd_pau = 0;
	cur_opts.path_opts.pd_bsp = 0;
	cur_opts.path_opts.pd_del = 0;
	cur_opts.path_opts.pd_eof = 0;
	cur_opts.path_opts.pd_rpr = 0;
	cur_opts.path_opts.pd_dup = 0;
	cur_opts.path_opts.pd_psc = 0;
	cur_opts.path_opts.pd_int = 0;
	cur_opts.path_opts.pd_qut = 0;

	if( opts->rts_enable )  /* Don't need Xon Xoff */
	{
		cur_opts.path_opts.pd_xon = 0;
		cur_opts.path_opts.pd_xoff = 0;
	}

	else    /* Do need Xon Xoff */
	{
		cur_opts.path_opts.pd_xon = 0x11;   /* x-on character               */
		cur_opts.path_opts.pd_xoff = 0x13;  /* x-off character              */
	}

	if( opts->baud_rate )
	{
		cur_opts.path_opts.pd_bau = opts->baud_rate;
		cur_opts.path_opts.pd_par = opts->parity;
	}

#endif  /* _OSK */

	status = restore_devopt( &cur_opts );
	DPRT( "restore_devopt returned %x \n", status );
	return( status );
}

#if defined(DRIVER)

error_code attach_dev( char* name )
{
	dev_list*	dev_entry;
	error_code	ec;

	ec = _os_attach( name, S_IREAD|S_IWRITE, &dev_entry );
	return( ec );
}

#endif	/* DRIVER */

#define FLUSH_BUF_SIZE	10

error_code clear_device( path_id pid )
{
	u_int32		byts_rdy;
	error_code	ec;

	ec = _os_gs_ready( pid, &byts_rdy );

	if( ec == SUCCESS )
	{
		u_int8	buf[FLUSH_BUF_SIZE];
		u_int32	num;

		while( byts_rdy )
		{
			num = (byts_rdy > FLUSH_BUF_SIZE ? FLUSH_BUF_SIZE : byts_rdy );
			ec = _os_read( pid,(void*) buf, &num );

			if( ec != SUCCESS )
			{
				num = byts_rdy;
			}

			byts_rdy -= num;
		}
	}

	if( ec == EOS_NOTRDY )
	{
		ec = SUCCESS;
	}

	return( ec );
}

void set_accm_bit( u_int32* accmptr, u_int8 opt )
{
	if( opt != 0 ) {
		u_int32 accm = *accmptr,
				mask = (1 << (opt-1));

		accm |= mask;
		*accmptr = accm;
	}
}

error_code update_accm( Device_opts opts, u_int32* accmptr )
{
	device_opts cur_opts;
	error_code  status;

	cur_opts.pd = opts->pd;
	status = save_devopt( &cur_opts );

	if( status != SUCCESS )
	{
		return( status );
	}

	if( cur_opts.dev_type != DT_SCF )
	{
		return( SUCCESS );
	}

 #if defined(_OS9000)

	set_accm_bit( accmptr, cur_opts.lu_opts.v_intr );
	set_accm_bit( accmptr, cur_opts.lu_opts.v_quit );
	set_accm_bit( accmptr, cur_opts.lu_opts.v_psch );
	set_accm_bit( accmptr, cur_opts.lu_opts.v_xon );
	set_accm_bit( accmptr, cur_opts.lu_opts.v_xoff );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_bspch );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_eofch );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_eorch );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_tabch );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_bellch );

#endif  /* _OS9000 */

#if defined(_OSK)

	set_accm_bit( accmptr, cur_opts.path_opts.pd_bsp );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_bse );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_del );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_eof );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_eor );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_rpr );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_dup );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_psc );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_int );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_qut );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_ovf );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_xon );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_xoff );
	set_accm_bit( accmptr, cur_opts.path_opts.pd_Tab );

#endif  /* _OSK */

	return( SUCCESS );
}
