/****************************************************************************
 * ID:     %A%
 * Date:   %G%
 ****************************************************************************
 * These are the crude SPF device descriptor definitions file.
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
 *   1 03/09/95 Creation (robbed from SPF)                      	bat	    *
 *   2 10/17/95 Added additional value for pd_txtrailer				dwj	    *
 *   3 6/14/96  Added DESC_SYSEDIT definition 					udayn       *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>             *
 * 231 04/17/97 <---------------- SPF v2.2 (beta)-----------------> taka    *
 *     04/21/97 reduced txsize from u_int32 to u_int16              taka    *
 * 235 06/26/97 <---------------- SPF V2.2 --------------------->   taka    *
 ****************************************************************************/


/* Header Files *************************************************************/

#include <types.h>
#include <const.h>
#include <module.h>
#include <sg_codes.h>
#include <io.h>
#include <modes.h>
#include <stddef.h>

#if defined(_OSK)
#include <reg68k.h>
#endif

#define DESC_SYSEDIT
#include <spf_desc.h>

#define SPF_DRVR
#include <SPF/spf.h>


#define FM_NAME_SIZE  16
#define DRV_NAME_SIZE 16


/* driver and filemanager name must be computed this way, since there
** is no way to pre-calculate the size of the module header.   (ioman
** expects these offsets to be from the beginning of the module, not
** relative to myconst.) */
#define FM_NAME_OFFSET  ((u_int32)myconst.fm_name)
#define DRV_NAME_OFFSET ((u_int32)myconst.drv_name)

/* Device descriptor constant data structure ********************************/

struct myconst {

	spf_desc				desc;
	char					fm_name[FM_NAME_SIZE];
	char					drv_name[DRV_NAME_SIZE];
	#if defined(PROTSTAK)
		char				pstak[64];
	#endif

} myconst = {

	/* desc */
	{
		/* Device Descriptor Common Fields */
		(void*)PORTADDR,		/* dd_port: hardware base address 			*/
		LUN,					/* dd_lu_num: logical unit number 			*/
		sizeof(spf_pdstat),		/* dd_pd_size: path descriptor size			*/
		DT_SPF,					/* dd_type: device type 					*/
		MODE,					/* dd_mode: device mode capabilities 		*/
		FM_NAME_OFFSET,         /* dd_fmgr: file mngr name offset		*/
		DRV_NAME_OFFSET,        /* dd_drvr: driver name offset			*/
		DC_SEQ,					/* dd_class: device class 					*/
		0,						/* dd_dscres: reserved for future IOMAN 	*/
	
		/* SPF-specific fields */

		{						/* dd_popts: path descriptor options		*/
		#if defined(_OSK)
			DT_SPF,					/* pd_devtype */
			0,						/* pd_buf1 */
		#elif defined(_OS9000)
			DT_SPF,					/* pd_devtype */
		#endif
			DC_SEQ,					/* pd_devclass */
			SPF_VERSION,			/* pd_version */
			0,						/* pd_ioenabled */
			ASYNCFLAG,				/* pd_ioasync */
			PKTFLAG,				/* pd_iopacket */
			sizeof(spf_popts),		/* pd_optsize */
			BLOCKTIME,				/* pd_iotime */
			READSZ,					/* pd_readsz */
			WRITESZ,				/* pd_writesz */
			0,						/* pd_rsvd */
			0xFFFF,					/* pd_txsize */
			0,						/* pd_txoffset */
			0,						/* pd_txtrailer */
			CTL_FLAGS,				/* pd_txmsgtype */
			0,						/* pd_reliable */
			{ 0 },					/* pd_rsv2[] */
		},

		{					/* dd_item: Initializes structure for ITEM		*/
			MODE,					/* dev_mode: Device mode	*/
			ITE_NETYPE_IN,			/* network type (incoming)	*/
			ITE_NETYPE_OUT,			/* Network type (outgoing)	*/
			ITE_CALL_STATE,			/* Initial call state		*/
			ITE_ASGN_NONE,			/* Initial receiver state		*/
			{ 0 },					/* dev_rsv1[]: Long word bndry	*/
			{ 0 },					/* dev_rsv2[]: Long word bndry	*/

			{				/* dev_ournum: Addr_type structure (ours)		*/
				OURADDR_CLASS,		/* OUr address class			*/
				OURADDR_SUBCLASS,	/* Our address subclass			*/
				0,					/* addr_rsv1:Long word boundary */
				OURADDR_SIZE,		/* Size of our address string	*/
				OURADDR				/* Our address string			*/
			},
			
			{				/* dev_theirnum: Addr_type structure (theirs)	*/
				THEIRADDR_CLASS,	/* Their address class			*/
				THEIRADDR_SUBCLASS,	/* Their address subclass		*/
				0,					/* addr_rsv1:Long word boundary */
				THEIRADDR_SIZE,		/* Size of their address string	*/
				THEIRADDR			/* Their address string			*/
			},
			
			{ 0 }					/* Display screen for telephony	*/
		},
		
		#if defined(PROTSTAK)	/* dd_pmstak */
			(u_int32)myconst.pstak,
		#else
			0,
		#endif

		{ 0 },					/* dd_rsv1[] */

	},

	/* fm_name[]: file manager name */
	{
		SPF_FMNAME
	},

	/* drv_name[]: device driver name */
	{
		DRV_NAME
	},

	#if defined(PROTSTAK)
	/* pstak[]: protocol module stak string */
	{
		PROTSTAK
	},
	#endif
};
