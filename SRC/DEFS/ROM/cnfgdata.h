
/*
 * $Header:   /h0/MWOS/SRC/DEFS/ROM/VCS/cnfgdata.h_v   1.22   09 Jul 1998 11:32:54   alexs  $
 */
 
/*--------------------------------------------------------------------------,
!                                                                           !
!           Copyright 1995, 1996 by Microware Systems Corporation           !
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
|																			|
| cnfgdata.h - rom configuration module data structure						|
|																			|
| edition history															|
|																			|
|   date    changes                                        		        by	|
| -------- ------------------------------------------------------------ ---	|
| 95/07/24 created                                        		 		Nib	|
| 95/07/28 added the definition of function get_config_data.	  		Nib	|
| 95/08/14 First design feedback update.                                ats |
| 95/08/18 Design walkthrough updates. Moved public info to rom.h.      ats |
| 95/08/22 Update structure alignment/filler.                           ats |
| 95/09/14 Added use_alt flags to if structure.                         ats |
| 95/10/06 Added subnet_mask, brdcst_address & gw_address               ats |
|          to if structure.                                                 |
|          ---- OS-9/68K V3.0.2 System-state Debug Support Release ----     |
| 96/01/05 Updated booter & protoman structures to V2:                  ats |
|          - Added llpm_datasize to cnfg_protoman.                          |
|          - Added boot_datasize & boot_cmdsize to cnfg_bootsys.            |
|          - Added boot_params to boot_data entry.                          |
|          - Extended llpm address fields to 16 bytes providing             |
|            previous version definitions for backwards compatibility.      |
|          - Added autoboot_delay to boot_data entry.                       |
|          ---- OS-9000/PPC V2.0 Release ----                               | 
|          ---- OS-9/68K V3.0.3 System-state Debug Support Release ----     |
|          ---- OS-9000/x86 V2.1 Released ----                              |
|          ---- OS-9000/PPC V2.1.1 Released ----                            |
|          ---- OS-9000/ARMv3 V2.2 Released ----                            |
|          ---- OS-9000/ARMv3 V2.2.1 Released ----                          |
|          ---- Modular Rom Sub-system v1.0 Released ----                   |
|          ---- OS-9000/SH3 V2.2.6 Released ----                            |
|          ---- OS-9000/SPARC V2.2.7 Released ----                          |
|          ---- Modular Rom v1.1 Sub-component Released ----                |
|          ---- Modular ROM v1.2 Sub-component Released ----                |
| 01/05/23 Added if_mtu to support ability to change SLIP MRU/MTU           |
|          during configuration.                                        jhd |
|          $$                      <RELEASE_INFO>                        $$ |
`--------------------------------------------------------------------------*/

/* ****************************************************************************

   Configuration Module structure.

   +--------------------+
   | Module Header      |
   +--------------------+
            :
   +--------------------+ + entry offset.
   | pointer to oft tbl.| <-- Pointer to offset table
   +--------------------+            |
            :                        |
   +--------------------+ <----------+
   | offset to 1st data | <-- Offset to 1st configuration data.
   +--------------------+
   | offset to 2nd data | <-- Offset to 2nd configuration data.
   +--------------------+
            :
   +--------------------+
   | offset to n th data| <-- Offset to last configuration data.
   +--------------------+
   |        0L          | <-- Terminated by 0
   +--------------------+
            :
   +--------------------+
   | Configuration data | <-- Configuration data entry.
   |        :           |
   |        :           |
   |        :           |
   +--------------------+
   | End of module      |
   +--------------------+

   Configuration data entry structure.

   +--------------------+
   | "CNFG" ID Field    | <-- Configuration data identifier
   +--------------------+
   | "????" TYPE Field  | <-- Configuration data type identifier
   +--------------------+
   | revision number    | <-- revision number.
   +--------------------+
   | Pointer to NAME	| <-- offset to name field.
   +--------------------+
   | Configuration data | <-- configuration data fields.
   |         :          |     ( these fields depend on module type.)
   |         :          |
   +--------------------+

 **************************************************************************** */

#if !defined(_CNFGDATA_H_)

#define	_CNFGDATA_H_
#include <types.h>

#define	CNFG_ID			{'C','N','F','G'}	/* Configuration entry ID STRING*/
#define	CNFG_S_ID		"CNFG"				/* Configuration entry ID STRING*/

#define	CNFG_TYPE_CONS	{'C','O','N','S'}	/* ENTRY TYPE ID STRING CONSOLE	*/
#define CNFG_TYPE_COMM	{'C','O','M','M'}	/* 			     COMM. PORT 	*/
#define CNFG_TYPE_DEBG	{'D','E','B','G'}	/*			  	 DEBUG MODULE	*/
#define CNFG_TYPE_LLPM	{'L','L','P','M'}	/*			     LLPM  MODULE	*/
#define CNFG_TYPE_BOOT	{'B','O','O','T'}	/*               BOOT  MODULE	*/
#define CNFG_TYPE_NTFY	{'N','T','F','Y'}	/* Notification Services Module	*/
#define CNFG_TYPE_NULL	{'N','U','L','L'}	/*			     NULL ENTRY		*/

#define	CNFG_TYPE_S_CONS "CONS"	/* ENTRY TYPE ID STRING CONSOLE	*/
#define CNFG_TYPE_S_COMM "COMM"	/* 			     COMM. PORT 	*/
#define CNFG_TYPE_S_DEBG "DEBG"	/*			  	 DEBUG MODULE	*/
#define CNFG_TYPE_S_LLPM "LLPM" /*			     LLPM  MODULE	*/
#define CNFG_TYPE_S_BOOT "BOOT"	/*               BOOT  MODULE	*/
#define CNFG_TYPE_S_NTFY "NTFY"	/* Notification Services Module	*/

typedef	struct cnfg_entry_header
{
	u_char	cnfg_id[4];		/* "CNFG" configuration id field. 			*/
	u_char	cnfg_hw_type[4];/* entry type id field						*/
	u_int16	cnfg_rev;		/* Revision field 							*/
	u_int16 reserved1;
	u_int32	reserved2;
} cnfg_entry_header, *CNFG_entry_header;

/*
 * Configuration data structure of console module.
 */

#define CONS_REVISION_MIN	1
#define CONS_REVISION_MAX	1

typedef	struct cnfg_console
{
	cnfg_entry_header ch;	/* configuration entry header */

	u_int32	cons_name;		/* console name				*/
	u_int32	cons_vector;	/* interrupt vector number	*/
	u_int32	cons_priority;	/* interrupt priority	  	*/
	u_int32 cons_level;		/* interrupt level			*/
	u_int32	cons_timeout;	/* polling timeout			*/
	u_int8	cons_parity;	/* parity size				*/
	u_int8	cons_baudrate;	/* baud rate 				*/
	u_int8	cons_wordsize;	/* character size			*/
	u_int8	cons_stopbits;	/* stop bit					*/
	u_int8	cons_flow;		/* flow control 			*/
	u_int8	reserved1;
	u_int16 reserved2;
	u_int32	reserved3;
		
} cnfg_console, *CNFG_console, cnfg_comm, *CNFG_comm;


/*
 * Configuration data structure of debugger module.
 */

#define DBG_REVISION_MIN	1
#define DBG_REVISION_MAX	1

typedef struct cnfg_debug
{
	cnfg_entry_header ch;
	
	u_int32	debug_default_client;	/* offset to the name string of default */
									/* debug client.						*/
	u_int32	debug_call_at_cold;		/* flag	the client should be called at  */
									/* cold start, or not					*/
	u_int32	reserved1;
	u_int32 reserved2;
	
} cnfg_debug, *CNFG_debug;


/*
 * Low Level protoman interface data entry.
 */

typedef	struct	llpm_interface_data
{
	u_int8		ip_address[16];		/* IP Address						*/
	u_int8		subnet_mask[16];	/* Subnet Mask						*/
	u_int8		brdcst_address[16];	/* Broadcast address				*/
	u_int8		gw_address[16];		/* Gateway Address					*/
	u_int8		mac_address[16];	/* MAC (Ethernet) address			*/
	u_int8		hwtype;				/* Driver type						*/
	u_int8		alt_parity;			/* Alt serial port parity			*/
	u_int8		alt_baudrate;		/* Alt serial port baudrate			*/
	u_int8		alt_wordsize;		/* Alt serial port wordsize			*/
	u_int8		alt_stopbits;		/* Alt serial port stopbits			*/
	u_int8		alt_flow;			/* Alt serial port flow control		*/
	u_int16		if_flags;			/* Interface-specific flag(s)		*/
	u_int32		if_name;			/* offset to interface name			*/
	u_int32		port_address;		/* Interface port address			*/
	u_int32		if_vector;			/* Interrupt vector					*/
	u_int32		if_priority;		/* Interrupt priority				*/
	u_int32		if_level;			/* Interrupt level					*/
	u_int32		alt_timeout;		/* Alt serial port timeout			*/
	u_int32		use_alt;			/* Alt usage flags					*/
	u_int16		if_mtu;				/* Maximum Transmit/Receive Unit    */
	u_int16		reserved1;
	u_int32		reserved2;
	u_int32		reserved3;
}	llpm_interface_data, *LLPM_interface_data;

typedef	struct	v1_llpm_interface_data
{
	u_int8		ip_address[8];		/* IP Address						*/
	u_int8		subnet_mask[8];		/* Subnet Mask						*/
	u_int8		brdcst_address[8];	/* Broadcast address				*/
	u_int8		gw_address[8];		/* Gateway Address					*/
	u_int8		mac_address[8];		/* MAC (Ethernet) address			*/
	u_int8		hwtype;				/* Driver type						*/
	u_int8		alt_parity;			/* Alt serial port parity			*/
	u_int8		alt_baudrate;		/* Alt serial port baudrate			*/
	u_int8		alt_wordsize;		/* Alt serial port wordsize			*/
	u_int8		alt_stopbits;		/* Alt serial port stopbits			*/
	u_int8		alt_flow;			/* Alt serial port flow control		*/
	u_int16		if_flags;			/* Interface-specific flag(s)		*/
	u_int32		if_name;			/* offset to interface name			*/
	u_int32		port_address;		/* Interface port address			*/
	u_int32		if_vector;			/* Interrupt vector					*/
	u_int32		if_priority;		/* Interrupt priority				*/
	u_int32		if_level;			/* Interrupt level					*/
	u_int32		alt_timeout;		/* Alt serial port timeout			*/
	u_int32		use_alt;			/* Alt usage flags					*/
	u_int32		reserved1;
	u_int32		reserved2;
	u_int32		reserved3;
}	v1_llpm_interface_data, *V1_LLPM_interface_data;

#define IF_USEALT_PARITY	0x80000000
#define IF_USEALT_BAUDRATE	0x40000000
#define IF_USEALT_WORDSIZE	0x20000000
#define IF_USEALT_STOPBITS	0x10000000
#define IF_USEALT_FLOW		0x08000000
#define IF_USEALT_VECTOR	0x04000000
#define IF_USEALT_PRIORITY	0x02000000
#define IF_USEALT_LEVEL		0x01000000
#define IF_USEALT_TIMEOUT	0x00800000

/*
 * Configuration data structure of protoman module.
 */

/*
 * version changes:
 * 1: original
 * 2: added llpm_datasize field
 *    extended size of address fields
 */
	
#define LLPM_REVISION_MIN	1
#define LLPM_REVISION_MAX	2

#define LLPM_V1_DATASIZE	sizeof(v1_llpm_interface_data)

#define	LLPM_VER_NODATASIZE	1		/* version without passed datasize */

typedef struct cnfg_protoman
{
	cnfg_entry_header ch;

	u_int16		maxllpmprotos;		/* Max. number of protocols on the 	*/
									/* protocol stack. 					*/
	u_int16		maxrcvmbufs;		/* Number of maximum receive nbuffs	*/
	u_int16		maxllpmconns;		/* Max. number of low level protoman*/
									/* connections						*/
	u_int16		reserved1;
	u_int32		llpm_count;			/* number of data. entries			*/
	u_int32		llpm_config;		/* configuration data entries.  	*/
	u_int32		llpm_datasize;		/* size of individual data entry	*/
	u_int32		reserved3;
	u_int32		reserved4;
	u_int32		reserved5;
} cnfg_protoman, *CNFG_protoman;

/*
 * Configuration data structure of boot system module.
 */

/*
 * version changes:
 * 1: original
 * 2: added boot_datasize, boot_cmdsize, boot_params,
 *    and autoboot_delay fields.
 */
     
#define BOOT_REVISION_MIN	1
#define BOOT_REVISION_MAX	2

#define BOOT_V1_DATASIZE	16			/* size of V1 boot_data */

#define BOOT_VER_NODATASIZE	1			/* version without passed datasize */
#define BOOT_VER_PARAMS		2			/* version with parameter string */
#define BOOT_VER_CMDSIZE	2			/* version with user input string size */
#define BOOT_VER_AUTODELAY	2			/* version with autoboot delay */

typedef struct boot_data
{
	u_int32				boot_abname;	/* offset to abbreviated booter name */
	u_int32				boot_newab;		/* optional replacement abname		*/
	u_int32				boot_newname;	/* optional replacement full name	*/
	u_int8				boot_automenu;	/* auto/menu flag					*/
	u_int8				reserved1;
	u_int16				reserved2;
	u_int32				boot_params;	/* optional parameter string		*/
	u_int32				autoboot_delay;	/* delay value in microseconds		*/
	u_int32				reserved3;
	u_int32				reserved4;
} boot_data, *BOOT_data;

typedef struct cnfg_bootsys
{
	cnfg_entry_header ch;

	u_int32				boot_count;		/* number of boot system entries	*/
	u_int32				boot_config;	/* configuration data entries		*/
	u_int32				boot_datasize;	/* size of individual data entry	*/
	u_int32				boot_cmdsize;	/* max size of user input string	*/
	u_int32				reserved1;
	u_int32				reserved2;
	u_int32				reserved3;
	u_int32				reserved4;
} cnfg_bootsys, *CNFG_bootsys;


/*
 * Configuration data structure of notification services module.
 */
 
#define NTFY_REVISION_MIN	1
#define NTFY_REVISION_MAX	1

typedef struct cnfg_ntfysvc
{
	cnfg_entry_header	ch;
	
	u_int32			max_notifiers;	/* maximum number of registered notifiers */
	u_int32			reserved1;
	u_int32			reserved2;
	u_int32			reserved3;
} cnfg_ntfysvc, *CNFG_ntfysvc;


#endif

/* ************************************************************************** */
