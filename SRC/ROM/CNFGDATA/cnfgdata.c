/*
 * $Header:   /h0/MWOS/SRC/ROM/CNFGDATA/VCS/cnfgdata.c_v   1.12   24 Oct 1996 09:28:22   allan  $
 */
 
/*--------------------------------------------------------------------------,
|                                                                           |
|            Copyright 1995-1996 by Microware Systems Corporation           |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| cnfgdata.c - rom configuration module data structure                      |
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 95/07/24 created                                                 Nib   | 
| 02 95/08/14 First design feedback update.                           ats   |
| 03 95/08/21 Design walkthrough update.                              ats   |
| 04 95/08/22 Fix notification identification.                        ats   |
| 05 95/09/22 Missed reserved field in cnfg_protoman.                 ats   |
| 06 95/10/06 Force edition number update for added if fields.        ats   |
|             ---- OS-9/68K V3.0.2 System-state Debug Support Release ---   |
| 07 96/01/05 Added initialization of llpm_datasize, boot_datasize,   ats   |
|             and boot_cmdsize fields.                                      |
|             ---- OS9000/PPC V2.0 ----                                     |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   |
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/

#define EDITION		7

_asm("_sysedit: equ %0", __obj_constant(EDITION));

#define		NEWINFO
#include	<rom.h>

#define		_CNFGDATA_C_
#include	<cnfgdata.h>		/* get configuration data structure */
#include	<systype.h>			/* get port dependant data */

_asm("_m_exec:	equ	config_data");


#if defined	(_STDC_) || defined(_ANSI_EXT)
	#define	CONST	const
#else
	#define CONST
#endif

extern CONST	cnfg_console	config_console;
extern CONST	cnfg_comm		config_comm;
extern CONST	cnfg_debug		config_debugger;
extern CONST	cnfg_protoman	cnfg_llpm;
extern CONST	cnfg_bootsys	config_boot;
extern CONST	cnfg_ntfysvc	config_ntfysvc;

CONST u_int32	config_data[] =
{
#if	defined(CONS_NAME)
	/* configuration entry for console driver.			*/
	(u_int32)&config_console,
#endif
#if	defined(COMM_NAME)
	/* configuration entry for communication driver.	*/
	(u_int32)&config_comm,
#endif
#if	defined(DEBUG_NAME)
	/* configuration entry for debug module				*/
	(u_int32)&config_debugger,
#endif	
#if	defined(LLPM_CNFG_ARRAY)
	/* configuration entry for llpm module				*/
	(u_int32)&cnfg_llpm,
#endif
#if defined(BOOT_ARRAY)
	/* configuration entry for boot module				*/
	(u_int32)&config_boot,
#endif
#if	defined(NTFY_CNFG_NOTIFIERS)
	/* configuration entry for notification services	*/
	(u_int32)&config_ntfysvc,
#endif
	0L									/* terminater	*/
};



/* -------------------------------------------------------------,
| Default console constants definition area                   	|
| For this definition, these macros have to be defined in     	|
| systype.h file.                                             	|
|  CONS_NAME 		--	console name						   	|
|                                                             	|
| <communication parameters>                                  	|
|	CONS_VECTOR     --  interrupt vector number					|
|	CONS_PRIORITY	--	interrupt priority						|
|	CONS_LEVEL		--	interrupt level							|
|   CONS_TIMEOUT	--	polling timeout							|
|	CONS_PARITY		--  parity size                            	|
|	CONS_BAUDRATE   --  communication speed                    	|
|	CONS_WORDSIZE	--	character size							|
|	CONS_STOPBITS	--  size of stop bit						|
|	CONS_FLOW		--	communication flow control flag			|
`--------------------------------------------------------------*/



#if	defined(CONS_NAME)

CONST	u_char	console_name[] = CONS_NAME;

CONST	cnfg_console	config_console =
{
	{
		CNFG_ID, 			/* configuration entry	 */
		CNFG_TYPE_CONS,		/* console configuration */
		CONS_REVISION_MAX	/* configuration revision number	*/
	},
	(u_int32) console_name,	/* console name	*/
	CONS_VECTOR,			/* interrupt vecter		*/
	CONS_PRIORITY,			/* interrupt priority	*/
	CONS_LEVEL,				/* interrupt level		*/
	CONS_TIMEOUT,			/* polling timeout		*/
	CONS_PARITY,			/* parity size	*/
	CONS_BAUDRATE,			/* communication speed	*/
	CONS_WORDSIZE,			/* character size		*/
	CONS_STOPBITS,			/* stop bit size		*/
	CONS_FLOW				/* flow control			*/
};
#endif


/* -------------------------------------------------------------,
| Default communication port constants definition area         	|
| For this definition, these macros have to be defined in     	|
| systype.h file.                                             	|
|  COMM_NAME 		--	commole name						   	|
|                                                             	|
| <communication parameters>                                  	|
|	COMM_VECTOR     --  interrupt vector number					|
|	COMM_PRIORITY	--	interrupt priority						|
|	COMM_LEVEL		--	interrupt level							|
|	COMM_TIMEOUT	--	polling timeout							|
|	COMM_PARITY		--  parity size   	                        |
|	COMM_BAUDRATE   --  communication speed                    	|
|	COMM_WORDSIZE	--	character size							|
|	COMM_STOPBITS   --  size of stop bit						|
|	COMM_FLOW		--	communication flow control flag			|
`--------------------------------------------------------------*/

#if	defined(COMM_NAME)

CONST	u_char	comm_name[] = COMM_NAME;

CONST	cnfg_comm	config_comm = 
{
	{
		CNFG_ID, 				/* configuration entry	 */
		CNFG_TYPE_COMM,			/* comm port configuration */
		CONS_REVISION_MAX		/* configuration revision number	*/
	},
	(u_int32) comm_name,	/* comm port name	*/
	COMM_VECTOR,			/* interrupt vecter		*/
	COMM_PRIORITY,			/* interrupt priority	*/
	COMM_LEVEL,				/* interrupt level		*/
	COMM_TIMEOUT,			/* polling timeout		*/
	COMM_PARITY,			/* parity size	*/
	COMM_BAUDRATE,			/* communication speed	*/
	COMM_WORDSIZE,			/* character size		*/
	COMM_STOPBITS,			/* stop bit size		*/
	COMM_FLOW				/* flow control			*/
};


#endif



/* -------------------------------------------------------------,
| Default debug module constants definition area				|
| For this definition, these macros have to be defined in     	|
| systype.h file.                                             	|
|  DEBUG_NAME 		--	default debug clients name			   	|
|                                                             	|
| <debug module parameters>                                  	|
|	DEBUG_COLD_FLAG	--  flag for calling the client at cold		| 
`--------------------------------------------------------------*/


#if	defined(DEBUG_NAME)
CONST	u_char	debug_name[] = DEBUG_NAME;

CONST	cnfg_debug	config_debugger = 
{
	{
		CNFG_ID,			/* configuration ID 			*/
		CNFG_TYPE_DEBG,		/* debugger configuration data	*/
		DBG_REVISION_MAX	/* configuration revision number	*/			
	},
	(u_int32)debug_name,	/* defaults client name			*/
	DEBUG_COLD_FLAG			/* call client at cold or not	*/
};

#endif


/* -------------------------------------------------------------,
| Default llpm module constants definition area					|
| For this definition, these macros have to be defined in     	|
| systype.h file.                                             	|
| <protoman module parameters>                                 	|
|	IF_STRINGS			-- Opt. interface string definitions.	|
|	LLPM_MAXLLPMPROTOS 	-- Max. number of protocals on the		|
|							protocol stack.						|
|   LLPM_MAXRCVMBUFS  	-- Number of maximum recive mbuffs		|
|	LLPM_MAXLLPMCONNS	-- Max. number of low-level protoman	|
|							connections.						|
|	LLPM_CNFG_ARRAY		-- See llpm_config_data structure.		|	
|						  										|
`--------------------------------------------------------------*/



#if	defined(LLPM_CNFG_ARRAY)

#if	defined(IF_STRINGS)
IF_STRINGS
#endif

CONST llpm_interface_data	llpm_cnfg_array[] = LLPM_CNFG_ARRAY;

CONST cnfg_protoman	cnfg_llpm =
{
	{
		CNFG_ID, 				/* configuration entry	 */	
		CNFG_TYPE_LLPM,			/* llpm configuration	 */
		LLPM_REVISION_MAX		/* configuration revision number	*/
	},
	LLPM_MAXLLPMPROTOS,
	LLPM_MAXRCVMBUFS,
	LLPM_MAXLLPMCONNS,
	0,	/* reserved field */
	sizeof(llpm_cnfg_array)/sizeof(llpm_interface_data),
	(u_int32)&llpm_cnfg_array[0],
	sizeof(llpm_interface_data)
};

#endif


/* -------------------------------------------------------------,
| Default boot module constants definition area					|
| For this definition, these macros have to be defined in     	|
| systype.h file.                                             	|
| <debug module parameters>                                  	|
|	BOOT_ARRAY		--	See boot_data structure.				|
|	BOOT_STRINGS	--	Optional boot string definitions.		|
|   BOOT_MAX_CMD_SZ	--	Optional max user input string size.    |
|                       (default value applied below).          |
`--------------------------------------------------------------*/


#if defined(BOOT_ARRAY)

#if !defined(BOOT_MAX_CMD_SZ)
#define	BOOT_MAX_CMD_SZ		32		/* 32-byte input default */
#endif

#if	defined(BOOT_STRINGS)
BOOT_STRINGS
#endif

CONST boot_data	boot_cnfg_array[] = BOOT_ARRAY;

CONST cnfg_bootsys config_boot =
{
	{
		CNFG_ID,
		CNFG_TYPE_BOOT,
		BOOT_REVISION_MAX
	},
	sizeof(boot_cnfg_array)/sizeof(boot_data),
	(u_int32)&boot_cnfg_array[0],
	sizeof(boot_data),
	BOOT_MAX_CMD_SZ
};

#endif

/* -------------------------------------------------------------,
| Default notification services module constants definition		|
| area.															|
|																|
| For this definition, these macros have to be defined in     	|
| systype.h file.                                             	|
| <debug module parameters>                                  	|
|	NTFY_CNFG_NOTIFIERS	--	Max. number of registered notifiers	|
|						  										|
`--------------------------------------------------------------*/


#if defined(NTFY_CNFG_NOTIFIERS)

CONST cnfg_ntfysvc config_ntfysvc =
{
	{
		CNFG_ID,
		CNFG_TYPE_NTFY,
		NTFY_REVISION_MAX
	},
	NTFY_CNFG_NOTIFIERS
};

#endif
