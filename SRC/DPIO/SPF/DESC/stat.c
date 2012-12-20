/****************************************************************************
 *  These are the definitions for the SPF logical unit.
 *  Initializes LU static, LU options, path descriptor options sections.
 ****************************************************************************
 * Copyright 1995 by Microware Systems Corporation              
 * Reproduced Under License                         
 *                                                                          
 * This source code is the proprietary confidential property of Microware   
 * Systems Corporation, and is provided to licensee for documentation and   
 * educational purposes only. Reproduction, publication, or distribution    
 * in any form to any party other than the licensee is strictly prohibited. 
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 *   1 03/09/95 Creation (robbed from SPF)                              bat *
 *   2 10/16/95 Added definition for lu_txtrailer                       dwj *
 *   3 10/17/95 Added TXTRAILER constant                                dwj *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>             *
 *     03/19/97 Added per path storage pointer in lustat                cas *
 * 231 04/17/97 <---------------- SPF v2.2 (beta)----------------->     taka*
 *     04/21/97 changed txsize to rsvd and txsize                       taka*
 * 235 06/26/97 <---------------- SPF V2.2 --------------------->       taka*
 * 250 01/09/97 change lu_prottype from u_int8 to u_int16               taka*
 *              <---------------- SPF v2.2.1 --------------------->     taka*
 * 263 06/28/99 Changed lu_rsv2 to lu_flags and initialize with LUFLAGS rds *
 ****************************************************************************/


/* Header Files *************************************************************/

#include <types.h>
#include <const.h>
#include <module.h>
#include <sg_codes.h>
#include <io.h>
#include <modes.h>

#if defined(_OSK)
#include <reg68k.h>
#endif

#include <spf_desc.h>

#ifndef SPF_LUSTAT_TYPE
#define SPF_LUSTAT_TYPE		struct { spf_lustat lucom; }
#endif


/* Logical Unit Static Storage Declarations *********************************/

SPF_LUSTAT_TYPE my_lu = {

	/* common logical unit section */
	{
		0,								/* lu_att_cnt 		*/
		0,								/* lu_use_cnt 		*/
		(void *)PORTADDR,				/* lu_port 			*/
		LUN,							/* lu_num 			*/
		0,								/* lu_ioenabled 	*/
		0,								/* lu_reliable 		*/
		0,								/* lu_rsvd 			*/
		TXSIZE,							/* lu_txsize 		*/
		TXOFFSET,						/* lu_txoffset 		*/
		TXTRAILER,						/* lu_txtrailer 	*/
		NULL,							/* lu_attachptr 	*/
		NULL,							/* lu_updrvr 		*/
		NULL,							/* lu_dndrvr 		*/
		NULL,							/* lu_pathdesc 		*/
		NULL,							/* lu_next 			*/
		PATH_HOLDONCLOSE,				/* lu_hold_on_close */
		LUFLAGS,						/* lu_flags			*/
		0,								/* lu_ppsize 		*/
		NULL,							/* lu_pplist 		*/
		NULL,							/* lu_ppidata 		*/
		{								/* lu_luopts 		*/
#if defined(_OSK)
			DT_SPF,							/* lu_devtype 	*/
			0,								/* lu_buf1 		*/
#elif defined(_OS9000)
			DT_SPF,							/* lu_devtype 	*/
#endif
			DC_SEQ,							/* lu_devclass 	*/
			SPF_VERSION,					/* lu_version 	*/
			0,								/* lu_rsv2 		*/
			sizeof(spf_luopts),				/* lu_optsize 	*/
			PROTTYPE,						/* lu_prottype 	*/
			{ 0 },							/* lu_rsv1[] 	*/
		},

		/* device-specific logical unit section */	
		#ifdef SPF_LUSTAT_INIT
			SPF_LUSTAT_INIT
		#endif
	}
};
