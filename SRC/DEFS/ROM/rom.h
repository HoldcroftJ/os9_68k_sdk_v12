/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1989-2001 by Microware Systems Corporation         |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| rom.h - data structures used for kernel/rom interface                     |
|                                                                           |
| edition history                                                           |
|                                                                           |
| ed#   date    changes                                                by   |
| --- -------- ------------------------------------------------------- ---  |
|  0  88/11/02 created                                                 djl  |
|  1  90/04/30 added entry in rom info structure for consumed          djl  |
|  2  93/09/09 Added "rom_delay" field to the ROM structure.           afh  |
|  3  93/11/02 Added "rom_dmabuff" field to the ROM structure.  Also,       |
|              made "rom_delay" and "rom_dmabuff" conditional for 386. afh  |
|  4  93/12/07 Added "rom_ramlimit" variable.                          afh  |
|  5  94/04/19 Reworked for rom restructuring.                         ats  |
|     94/05/02 Changed proto_status parameters, added Consdev field         |
|              All changes to the proto_srvr structure                 cas  |
|  6  94/05/07 modified proto_man structure                            gkm  |
|     94/05/18 modified llpm_conn, proto_srvr. Added proto type IDs.   cas  |
|     94/06/01 Added proto_addr to proto_srvr structure.  To be inited gkm  |
|              by init routine of LL driver.  Null for other                |
|              protocol server entities like UDP etc.                       |
|  7  94/09/07 Update consdev structure for compatibility mode builds. ats  |
|  8  94/09/09 Update protocol structs for compatibility mode builds.  ats  |
|  9  94/09/29 Added kernel and debug extension fields.                ats  |
| 10  94/10/05 Added rom extension field.                              ats  |
| 11  94/10/24 Added aliases for old.kernel_extnd and old.rom_newinfo. ats  |
|              Renamed old.kernel_extnd to old.rom_kernel_extnd.            |
| 12  94/10/24 Added cc_fputype to hw_config for PPC.                  ats  |
| 13  94/10/25 Added use_debug routine ptr to rom_svcs structure.      ats  |
| 14  94/11/02 Added rom_hellomsg ptr to rom_svcs structure.           ats  |
| 15  94/11/04 Added bctrl_statics to boot_svcs.                       ats  |
| 16  94/12/13 Added pointers for log. & phys. consumed list end.      ats  |
| 17  95/01/26 Added dbg_except_entry and dbg_watch to dbg_server.     ats  |
|              Added rom_dbginit to dbg_svcs structure.                ats  |
| 18  95/02/03 Added ctrl_flags to boot_svcs structure.                ats  |
| 19  95/03/08 Added flush_cache routine to hw_config structure.       ats  |
| 20  95/04/10 Revised console device structure.                       ats  |
|              Revised timer services structure.                            |
| 21  95/04/19 Replace poll_delay and poll_count fields in consdev     ats  |
|              with poll_timeout.                                           |
| 22  95/04/25 Update dbg_server structure for split debugger.     djl/ats  |
| 23  95/04/28 Update dbg_svcs structure for split debugger.           djl  |
| 24  95/04/28 Create oldcons structure for linked dubugger.           ats  |
| 25  95/05/08 Updated dbg_monitor function prototype.                 djl  |
| 26  95/05/15 Strip boot functions down for embedded pkg release.     ats  |
| 27  95/05/16 Add rom_Bootp into oldinfo table for 1.4 bootp support. ats  |
| 28  95/05/18 proto_srvr, llpm_conn, proto_man consts updated for     gkm  |
|              OS/Rombug mode combo I/O.                                    |
| 29  95/05/22 Refined os_svcs structure & values for 1st release.     ats  |
| 30  95/06/08 Eliminate periods out of constant definitions.          ats  |
| 31  95/07/06 Added CONS_NOECHO, and baudrate mask and max defs.      ats  |
| 32  95/07/26 Added get_config_data() service and configuration data  ats  |
|              module pointer to rom_svcs.                                  |
| 33  95/08/15 Added cnfg_entry_index field to rom_svcs.               ats  |
| 34  95/08/17 Add reserved field to newinfo typedef.                  ats  |
| 35  95/08/18 Moved configuration services into dedicated record.     ats  |
|              Added notification services record.                          |
|              Added cons_level to consdev record.                          |
|              Timer init service now returns error_code.                   |
| 36  95/09/13 Add parameter to flush_cache to specify data or inst.   ats  |
| 37  95/09/14 Added configuration id for use_alt if element.          ats  |
| 38  95/09/21 Added CST_BLKD_READ definition and updated llpm_static  ats  |
|              and llpm_conn structures.                                    |
|              Renamed LLPM_COUNT to LLPM_IFCOUNT.                          |
| 39  95/09/26 Added dbg_entry() entry point for debug glue.           ats  |
| 40  95/10/03 Added empty_list to notify_svcs.                        ats  |
| 41  95/10/06 Added LLPM_IF_SUBNET_MASK, LLPM_IF_BRDCST_ADDRESS,      ats  |
|              and LLPM_IF_GW_ADDRESS configuration definitions.            |
| 42  95/10/13 Added last_direction to notify_svcs.                    gdb  |
|              ---- OS-9/68K V3.0.2 System-state Debug Support Release ---- |
| 43  96/01/02 Complete function prototypes for bootdev record.        ats  |
| 44  96/01/04 Added structure IDs for fdman and scsiman defined       ats  |
|              structures.                                                  |
| 45  96/01/04 Updated module_svcs to version 2:                       ats  |
|              - Added kernel name pointer.                                 |
|              - Added goodmodule() service.                                |
| 46  96/01/05 Updated proto_srvr (V2) for booter and  ethernet        ats  |
|                support and future large IP addresses.                     |
|              Added IPPROTO_LL21040 protocol type for first                |
|                driver.                                                    |
|              Added more values for proto_flags.                           |
| 47  96/01/08 Updated bootdev structure to version 2:                 ats  |
|              - Added user_params, config_params, config_string and        |
|                autoboot_delay fields for booter processing.               |
|              Added BOOTER_PARAMS, BOOTER_AUTODELAY, and BOOT_CMDSIZE      |
|              configuration definitions.                                   |
| 48  96/01/10 Update parameter list for dbg_refresh_vect() service.   ats  |
| 49  96/01/16 Added various protoman definitions.                     ats  |
| 50  96/01/17 Corrected previous addition. Added yet another.         ats  |
| 51  96/03/21 Added additional protoman definitions.                  rak  |
|              ---- OS-9000/PPC V2.0 Release ----                           |
| 52  96/04/08 Added defs for 56000, 57600, 64000, & 115200 baud.      gdb  |
| 53  96/05/13 Added state1 field to llpm_conn (version 2).            ats  |
|              Defined CANTRCVMORE bit within state1 field.                 |
| 54  96/06/27 Added oem_globals field in os_svcs for OS-9/68K.        ats  |
| 55  96/08/13 Replaced IPPROTO* defines with ipproto.h.               ats  |
| 56  96/08/21 Changed path to ipproto.h to ROM/ipproto.h              rak  |
|              ---- OS-9000/PPC V2.0.1 Released ----                        |
|              ---- OS-9000/x86 V2.1 Released ----                          |
| 57  96/09/23 Added ARM hw_config record.                             gdb  |
| 58  96/10/10 Added MIPS rom_kernel_extnd record.                     cdg  |
|              ---- OS-9000/PPC V2.1.1 Released ----                        |
|              ---- OS-9/68K V3.0.3 System-state Debug Support Release ---  |
| 59  96/11/06 Added excpt_svcs and multi-client debug records.        gdb  |
| 60  96/11/22 Added MIPS hw_config record.                            sep  |
| 61  96/12/16 Added rom_excptrtn field to debug services.             gdb  |
| 62  96/12/20 Removed last comma in config_element_id per comp group  cdh  |
| 63  97/01/07 Fixed syntax error in dbg_except_entry prototype.       gdb  |
| 64  97/02/10 Fixed nested comment around CST_CONN_RCVD               dwj  |
| 65  97/03/09 Fixed prototype for dbg_watch.                          gdb  |
| 66  97/03/18 Added DEBUG_BYPASS and DEBUG_CALL defines.              ljs  |
| 67  97/04/03 Added SH hw_config record.                              gdb  |
|              ---- OS-9000/ARMv3 V2.2 Released ----                        |
| 68  97/05/02 Changed name of new in rominfo to new_style for C++     cdh  |
|              compatibility; either name works for C now                   |
| 69  97/05/14 Added SPARC hw_config record. Removed tabs from header. gdb  |
|              ---- OS-9000/ARMv3 V2.2.1 Released ----                      |
| 70  97/10/01 Added excpt_glbls field to excpt_svcs record.           ats  |
|              ---- OS-9000/ARM V2.2.3 OS Component Released ----           |
|              ---- OS-9000/SH3 V2.2.4 OS Component Released ----           |
|              ---- Modular Rom Sub-system v1.0 Released ----               |
|              ---- MBX_BSP_Beta Released ----                              |
| 71  98/01/22 Don't include ipproto.h when running Editmod.           ats  |
|              ---- MBX_BSP Released ----                                   |
|              ---- OS-9000/SH3 V2.2.6 Released ----                        |
|              ---- OS-9000/SPARC V2.2.7 Released ----                      |
| 72  98/03/24 Added BT_INFO define.                                   gdb  |
| 73  98/05/04 Added "fastboot" enhancements, which consisted of	    |
|	       adding the "boot_config" field to the "os_svcs" 		    |
|	       structure along with the functional bit definitions.	    |
|	       Also, bumped the "os" structure's version number.       afh  |
|              ---- Modular Rom v1.1 Sub-component Released ----            |
| 74  98/06/19 Conditionalized fastboot definitions to avoid           ats  |
|              duplicate definitions with OS9/SRC/DEFS/sysglob.h.           |
|              ---- Modular ROM v1.2 Sub-component Released ----            |
! 75  99/04/08 Changed defines for NEWINFO to use new_style instead    sr   !
!              of new to work with C++.  Both new and new_style             !
!              should work for C.                                           !
!              Resolve macro definition conflict with item.h                !
! 76  99/09/01 Filling out hw_config union with cc_hwflags flags field      !
!              to allow extra cpu specific flag section per processor. rkw  ! 
! 77  00/01/13 Changed INET/mbuf.h to SPF/mbuf.h                       rds  !
! 78  00/05/04 Added comment on Baud Rate table and more rates         mgh  !
! 79  00/05/16 Added HW_CACHETYPE_DATA_INV type for ARM.               rkw  !
| 80  00/12/08 Add oem_sub_svcs structure for OEM command extension    and  |
|              for Rombug.                                                  |
| 81  01/02/05 Added data cache invalidation support for IBM 40x       rry  |
| 82  01/05/23 Added LLPM_IF_MTU.  SLIP driver modified to allow            |
|              changing of MTU/MRU during configuration.  Added             |
|              macro LLIF_SLIP_WIN2K.                                  jhd  |
| 83  01/06/01 Added proto_bootp to the proto_srvr structure.It points  vd  |
|              to the bootp server's response packet.                   vd  |
| 84  01/06/13 Added compression services.                             gdw  |
| 85  01/06/15 Changed PVR_VER_MAX back to 2 (see note in code)        gdw  |
| 86  01/06/22 Added subcodes for ll ethernet driver setstats.         sr   |
| 87  01/07/05 Added support for compiling in back-compat mode         rry  |
| 88  01/10/01 Added support for sh5m                                  gjb  |
`--------------------------------------------------------------------------*/

#if !defined(_ROM_H)
#define	_ROM_H

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
#define CONST_ROM_H const
#else
#define CONST_ROM_H
#endif

/*
 * compatibility flags
 */
 
#if !defined(NEWINFO)
#define OLDINFO
#endif

/* Incomplete Rominfo delaration */

typedef union rominfo rominfo, *Rominfo;

/*
 * device status flags
 */
 
#define		STAT_PRESENT	0x01		/* hardware is present */
#define		STAT_INIZED		0x02		/* device initialized */
#define		STAT_ENABLED	0x04		/* device enabled */

#define		MAXROMBLKS		64			/* max number of rom blocks */

#define		NOABORT			-1l			/* no abort switch available */

/*
 * identification/version structure
 */
 
typedef struct idver {
	u_int16			struct_id,			/* structure identifier */
					struct_ver;			/* structure version */
	u_int32			struct_size;		/* allocated structure size */
} idver, *Idver;

#define		struct_id		infoid.struct_id
#define		struct_ver		infoid.struct_ver
#define		struct_size		infoid.struct_size

/*
 * memory list data structure 
 */
 
typedef struct dumbmem {
	struct dumbmem	*nxt;				/* where the next one is */
	u_int32			size;				/* how much there is */
} dumb_mem, *Dumb_mem;

/*
 * rom list data structure
 */
 
typedef struct rom_list {
	u_char			*addr;				/* where the rom is */
	u_int32			siz;				/* size of the block */
} rom_list, *Rom_list;

/*
 * console serial i/o low level driver interface structure 
 */

#define CONSDEVID	0xA861

#define CDV_VER_MIN	1
#define CDV_VER_MAX 1

typedef struct consdev consdev, *Consdev;

#ifdef	NEWINFO
struct oldcons {
#else
struct consdev {
#endif
	void		*cons_addr;				/* port address */
#if	defined(__STDC__)||defined(_ANSI_EXT)
	u_int32		(*cons_probe)(Rominfo, Consdev), /* see if device is present */
				(*cons_init)(Rominfo, Consdev), /* initialize function */
				(*cons_term)(Rominfo, Consdev); /* deiniz function */
	u_char		(*cons_read)(Rominfo, Consdev); /* read one character */
	u_int32		(*cons_write)(char, Rominfo, Consdev), /* write one character */
				(*cons_check)(Rominfo, Consdev), /* check for character available */
#else
	u_int32		(*cons_probe)(),		/* see if device is present */
				(*cons_init)(),			/* initialize function */
				(*cons_term)();			/* deiniz function */
	u_char		(*cons_read)();			/* read one character */
	u_int32		(*cons_write)(),		/* write one character */
				(*cons_check)(),		/* check for character available */
#endif
				cons_flags;				/* device status flags */
	u_char		cons_csave,				/* read ahead stash */
				cons_unused[3];			/* unused characters */
	u_char		*cons_abname,			/* abreviated name */
				*cons_name;				/* full name and description */
	void		*cons_data;				/* device specific data */
#ifdef	NEWINFO
	struct oldcons
#else
	struct consdev
#endif
				*cons_next;				/* next device in list */
#ifdef	NEWINFO
	struct consdev
				*cons_real;				/* pointer to real consdev entry */
#endif
};

#ifdef	NEWINFO
typedef struct oldcons oldcons, *Oldcons;

struct consdev {
	idver		infoid;
	void		*cons_addr;				/* port address */
	u_int32		(*cons_probe)(Rominfo, Consdev), /* see if device is present */
				(*cons_init)(Rominfo, Consdev), /* initialize function */
				(*cons_term)(Rominfo, Consdev); /* deiniz function */
	u_char		(*cons_read)(Rominfo, Consdev); /* read one character */
	u_int32		(*cons_write)(char, Rominfo, Consdev), /* write one character */
				(*cons_check)(Rominfo, Consdev); /* check for character available */
	u_int32		(*cons_stat)(Rominfo, Consdev, u_int32), /* get/set stat functions */
				(*cons_irq)(Rominfo, Consdev), /* interrupt service routine */
				(*proto_upcall)(Rominfo, void*, char*);
	u_int32		cons_flags;				/* device status flags */
	u_char		cons_csave,				/* read ahead stash */
				cons_baudrate,
				cons_parsize,			/* parity, data bits, stop bits */
				cons_flow;				/* flow control */
	u_int32		cons_vector,			/* interrupt vector */
				cons_priority,			/* interrupt priority */
				poll_timeout;			/* polling timeout in cons_irq() */
	u_char		*cons_abname,			/* abreviated name */
				*cons_name;				/* full name and description */
	void		*cons_data;				/* device specific data */
	void		*upcall_data;			
	Consdev		cons_next;				/* next device in list */
	u_int32		cons_level;				/* interrupt level */
	int			reserved;
};
#endif

#define	CONS_SETSTAT_POLINT_OFF		0x8000
#define CONS_SETSTAT_POLINT_ON		0x8001
#define CONS_SETSTAT_RXFLOW_OFF		0x8002
#define CONS_SETSTAT_RXFLOW_ON		0x8003
#define CONS_SETSTAT_ROMBUG_ON		0x8004
#define CONS_SETSTAT_ROMBUG_OFF		0x8005

#define CONS_NOECHO					0x80

/*
 * BAUD Rate values.  Keep In sync with OS9000 values in scf.h
 */
#define CONS_BAUDRATE_MASK			0x3F
#define CONS_BAUDRATE_HARDWIRE		0x00
#define CONS_BAUDRATE_50			0x01
#define CONS_BAUDRATE_75			0x02
#define CONS_BAUDRATE_110			0x03
#define CONS_BAUDRATE_134P5			0x04
#define CONS_BAUDRATE_150			0x05
#define CONS_BAUDRATE_300			0x06
#define CONS_BAUDRATE_600			0x07
#define CONS_BAUDRATE_1200			0x08
#define CONS_BAUDRATE_1800			0x09
#define CONS_BAUDRATE_2000			0x0A
#define CONS_BAUDRATE_2400			0x0B
#define CONS_BAUDRATE_3600			0x0C
#define CONS_BAUDRATE_4800			0x0D
#define CONS_BAUDRATE_7200			0x0E
#define CONS_BAUDRATE_9600			0x0F
#define CONS_BAUDRATE_19200			0x10
#define CONS_BAUDRATE_31250			0x11
#define CONS_BAUDRATE_38400			0x12
#define CONS_BAUDRATE_56000			0x13
#define CONS_BAUDRATE_57600			0x14
#define CONS_BAUDRATE_64000			0x15
#define CONS_BAUDRATE_115200		0x16
#define CONS_BAUDRATE_MAX			CONS_BAUDRATE_115200
/* Fast rates defined for future use but not yet implemented */
#define CONS_BAUDRATE_230400		0x17
#define CONS_BAUDRATE_460800		0x18
#define CONS_BAUDRATE_921600		0x19
#define CONS_BAUDRATE_76800   		0x1a
#define CONS_BAUDRATE_153600  		0x1b
#define CONS_BAUDRATE_307200  		0x1c
#define CONS_BAUDRATE_614400  		0x1d
#define CONS_BAUDRATE_1228800 		0x1e


#define CONS_PARITY_MASK			0x0F
#define CONS_PARITY_SHIFT			0
#define CONS_NOPARITY				0x00
#define CONS_ODDPARITY				0x01
#define CONS_EVENPARITY				0x02
#define CONS_MARKPARITY				0x03
#define CONS_SPACEPARITY			0x04

#define CONS_STOP_MASK				0x30
#define CONS_STOP_SHIFT				4
#define CONS_1STOP					0x00
#define CONS_1P5STOP				0x10
#define CONS_2STOP					0x20

#define CONS_DBITS_MASK				0xC0
#define CONS_DBITS_SHIFT			6
#define CONS_8BITS					0x00
#define CONS_7BITS					0x40
#define CONS_6BITS					0x80
#define CONS_5BITS					0xC0

#define CONS_NOSHAKE				0x00
#define CONS_SWSHAKE				0x01	/* XOFF, any char on */
#define CONS_HWSHAKE				0x02
#define CONS_SWSTRICT				0x03	/* strictly XOFF-XON */

/*
 * debugger interface structure 
 */

#define DEBUG_BYPASS    0x0     /*  Bypass calling debugger during boot sequence  */
#define DEBUG_CALL      0x1     /*  Call debugger during boot sequence  */

/*
 * boot device interface structure 
 *
 * version changes:
 * 1: original
 * 2: added user_params and config_params string pointers
 */

#define	BOOTDEVID		0xA891

#define	BDV_VER_MIN		1
#define	BDV_VER_MAX		2

#define BDV_PARAM_STRINGS	2	/* version id for parameter strings */
#define BDV_AUTOBOOT_DELAY	2	/* version id for autoboot delay */


typedef struct bootdev bootdev, *Bootdev;

struct bootdev {

#ifdef	NEWINFO
	idver		infoid;
#endif

	void		*bt_addr;			/* the port address */

#if defined(__STDC__)||defined(_ANSI_EXT)

				/* check for device existence */
	u_int32		(*bt_probe) (Bootdev bdev, Rominfo rinf),

				/* initialize boot device */
				(*bt_init) (Bootdev bdev, Rominfo rinf),

				/* read data from boot device */
				(*bt_read) (u_int32 blks, u_int32 blkaddr, u_char *buff, Bootdev bdev, Rominfo rinf),

				/* write data from boot device */
				(*bt_write) (u_int32 blks, u_int32 blkaddr, u_char *buff, Bootdev bdev, Rominfo rinf),

				/* terminate the boot device */
				(*bt_term)(Bootdev bdev, Rominfo rinf),

				/* bring boot in from device */
				(*bt_boot) (Bootdev bdev, Rominfo rinf);

#else
	u_int32		(*bt_probe)(),		/* check for device existence */
				(*bt_init)(),		/* initialize boot device */
				(*bt_read)(),		/* read data from boot device */
				(*bt_write)(),		/* write data to boot device */
				(*bt_term)(),		/* terminate the boot device */
				(*bt_boot)();		/* bring boot in from device */
#endif

	u_int32		bt_flags;			/* misc. flags */
	u_char		*bt_abname,			/* abreviated name */
				*bt_name;			/* full name and description */

#ifndef	NEWINFO
	u_int32		bt_blksize;			/* disk block size */
#endif

	void		*bt_data;			/* special data for boot device */
	Bootdev		bt_next;			/* next device in the list */

#ifdef	NEWINFO
	Bootdev		bt_subdev;			/* sub-device record */

	u_char		**user_params;		/* user parameter array */
	u_char		**config_params;	/* configuration parameter array */
	u_char		*config_string;		/* configuration parameter string */
	u_int32     autoboot_delay;		/* autoboot delay time */

	u_int32		bt_reserved[4];		/* reserved for emergency expansion */
#endif
};

/*
 * the boot rom/kernel interface structure (ORIGINAL)
 */
 
typedef struct oldinfo {
	u_int32		rom_osflag;				/* non-zero when system is running */
	void		*rom_glbldata,			/* global data pointer */
				*rom_initsp;			/* initial stack pointer */
	u_int32		*rom_vectors;			/* the vector table */
	void		(*rom_start)(),			/* reset pc */
				(*rom_debug)(),			/* debugger warm entry point */
				(*rom_set_dbg_vects)(),	/* let debugger reset its vectors */
				(*rom_ostart)();		/* kernel or equivalent entry point */
	Dumb_mem	rom_memlist;			/* the limited memory list */
	Rom_list	rom_romlist;			/* rom blocks found */
	u_int32		rom_totram,				/* total ram found */
				rom_calldebug,			/* flag kernel break in coldstart*/
				rom_cputype;			/* cpu type */
	u_int32		(*rom_malloc)(),		/* get memory */
				(*rom_free)();			/* free memory */
	char		(*rom_getchar)(),		/* get a character from console */
				(*rom_getc)();			/* get character from some console */
	void		(*rom_putchar)(),		/* put a character to console */
				(*rom_putc)();			/* put a character to some console */
	char		*(*rom_gets)();			/* use rom_getchar for a line */
	void		(*rom_puts)();			/* use rom_putchar for a line */
	Consdev		rom_conslist,			/* list of console type devices */
				rom_incons,				/* input console device */
				rom_outcons,			/* output console device */
				rom_in2cons,			/* second input console device */
				rom_out2cons;			/* second output console device */
	Bootdev		rom_bdevlist;			/* boot device list */
	u_int32		rom_fputype;			/* floating point type */
	Rom_list	rom_consumed;			/* memory consumed by roms */
	u_char		*rom_ramlimit;			/* RAM limit (highest address) */
#if		defined(_MPF386)
	u_int32		rom_delay;				/* delay loop counter */
	u_char		*rom_dmabuff;			/* 64k DMA buffer for > 16meg systems */
	void		*rom_BootP;				/* BootP booter/driver comm area */
	u_int32		rom_unused[3];			/* unused storage */
#elif	defined(_MPFPOWERPC) || defined(_MPFMIPS)
	u_char		*rom_kernel_extnd;		/* extension of vector table */
	u_int32		rom_unused[5];			/* unused storage */
#else
	u_int32		rom_unused[6];			/* unused storage */
#endif
	u_int32		rom_newinfo;			/* pointer to newinfo table */
} oldinfo, *Oldinfo;

/*
 * hardware configuration structure
 *
 * version changes:
 * 1: original
 */

#define	HWCFGID		0xA810

#define	HW_VER_MIN	1
#define HW_VER_MAX	1
 
typedef struct {
#if defined(NEWINFO)
	union hw_config {
		struct cpu68k_config {
			idver	infoid;					/* id/version for hw_config */
			u_int32	cc_cputype,				/* specific cpu type */
					cc_fputype,				/* specific fpu type */
					cc_mmutype,				/* specific mmu type */
					cc_intctrltype;			/* interrupt controller type */
		} cpu68k;
		struct cpu386_config {
			idver	infoid;					/* id/version for hw_config */
			u_int32	cc_cputype,				/* specific cpu type */
					cc_fputype,				/* specific fpu type */
					cc_intctrltype,			/* interrupt controller type */
					cc_hwflags;             /* hardware specific flags */
		} cpu386;
		struct cpuppc_config {
			idver	infoid;					/* id/version for hw_config */
			u_int32	cc_cputype,				/* specific cpu type */
					cc_fputype,				/* specific fpu type */
					cc_intctrltype,			/* interrupt controller type */
					cc_hwflags;             /* hardware specific flags */
		} cpuppc;
		struct cpuarm_config {
			idver	infoid;					/* id/version for hw_config */
			u_int32	cc_cputype,				/* specific cpu type */
					cc_fputype,				/* specific fpu type */
					cc_intctrltype,			/* interrupt controller type */
					cc_hwflags;             /* hardware specific flags */
		} cpuarm;
		struct cpumips_config {
			idver	infoid;					/* id/version for hw_config */
			u_int32	cc_cputype,				/* specific cpu type */
					cc_fputype,				/* specific fpu type */
					cc_intctrltype,			/* interrupt controller type */
					cc_hwflags;             /* hardware specific flags */
		} cpumips;
		struct cpush_config {
			idver	infoid;					/* id/version for hw_config */
			u_int32	cc_cputype,				/* specific cpu type */
					cc_fputype,				/* specific fpu type */
					cc_intctrltype,			/* interrupt controller type */
					cc_hwflags;             /* hardware specific flags */
		} cpush;
		struct cpusparc_config {
			idver	infoid;					/* id/version for hw_config */
			u_int32	cc_cputype,				/* specific cpu type */
					cc_fputype,				/* specific fpu type */
					cc_intctrltype,			/* interrupt controller type */
					cc_hwflags;             /* hardware specific flags */
		} cpusparc;
		struct cpush5m_config {
			idver	infoid;					/* id/version for hw_config */
			u_int32	cc_cputype,				/* specific cpu type */
					cc_fputype,				/* specific fpu type */
					cc_intctrltype,			/* interrupt controller type */
					cc_hwflags;             /* hardware specific flags */
		} cpush5m;
	}			cpu;
	void		(*flush_cache)(u_int32 *addr, u_int32 size, u_int8 type,
								Rominfo rinf); /* cache flushing routine */
#endif
	int			reserved;				/* reserved for emergency expansion */
} hw_config, *Hw_config;

#define	cpu68k	cpu.cpu68k
#define cpu386	cpu.cpu386
#define cpuppc	cpu.cpuppc
#define cpuarm	cpu.cpuarm
#define cpumips	cpu.cpumips
#define cpush	cpu.cpush
#define cpusparc	cpu.cpusparc
#define cpush5m cpu.sh5m

/* cc_hwflags provides a rinf entry for hardware specific defines. Unless stated all 
 * bit field entries are currently reserved for all non-68k processors. This flag will
 * be got/set in a similar fashon to the fpu and cpu fields (btfuncs.a)
 */

/* cpuarm: cc_hwflags
 *
 * bit 0: 1=CP15 is accessable via mcr/mrc instructions.
 * bit 1-30: reserved
 */

/* cache type values for flush_cache() service */
#define HW_CACHETYPE_INST	   0x00000001
#define HW_CACHETYPE_DATA	   0x00000002
#if defined(_MPFARM) || defined(_MPFPPC401) || defined(_MPFPPC403) || defined(_MPF405)
/* Currrently only ARM and IBM 40x flush cache supports this invalidate operation.
 * It is a must for cached buffer descriptors and the low level system.
 */
 #define HW_CACHETYPE_DATA_INV 0x00000004
#else
 #define HW_CACHETYPE_DATA_INV	HW_CACHETYPE_DATA
#endif

/*
 * memory services structure
 *
 * version changes:
 * 1: original
 */
 
#define MEMSVCID	0xA820

#define	MEM_VER_MIN	1
#define MEM_VER_MAX 1

typedef struct mem_svcs {
	idver		infoid;					/* id/version for mem_svcs */
#if defined(NEWINFO)
	Dumb_mem	rom_memlist;			/* the limited memory list */
	Rom_list	rom_romlist;			/* rom memory list */
	Rom_list	rom_bootlist;			/* boot memory list */
	Rom_list	rom_consumed;			/* memory consumed by roms */
	Rom_list    consumed_next;			/* next free consumed list entry */
	Rom_list    consumed_end;			/* last entry in consumed list */
	u_char		*rom_ramlimit;			/* RAM limit (highest address */
	u_int32		rom_totram;				/* total ram found */
	u_int32		(*rom_malloc)(u_int32 *, char **, Rominfo), /* get memory */
				(*rom_free)(u_int32, char *, Rominfo); /* free memory */
	void		(*mem_clear)(u_int32, char *); /* clear memory */
	u_char		*rom_dmabuff;			/* 64k DMA buffer for >16MB systems */
#endif
	int			reserved;				/* reserved for emergency expansion */
} mem_svcs, *Mem_svcs;

/*
 * the rom services structure
 *
 * version changes:
 * 1: original
 */
 
#define ROMSVCID	0xA830

#define	ROM_VER_MIN	1
#define	ROM_VER_MAX	1

typedef struct rom_svcs {
	idver		infoid;					/* id/version for rom_svcs */
#if defined(NEWINFO)
	void		*rom_glbldata,			/* global data pointer */
				*rom_excptjt,			/* exception jump table */
				*rom_initsp;			/* initial stack pointer */
	u_int32		*rom_vectors;			/* the vector table */
	void		(*rom_start)();			/* reset pc	*/
	u_char		*kernel_extnd;			/* the kernel extension */
	u_char		*debug_extnd;			/* the debugger extension */
	u_char		*rom_extnd;				/* the ROM extension */
	u_int32		(*use_debug)();			/* debugger enable routine */
	char		*rom_hellomsg;			/* hello message pointer */
#endif
	int			reserved;				/* reserved for emergency expansion */
} rom_svcs, *Rom_svcs;

/*
 * the low-level module services structures
 *
 * module list version changes:
 * 1: original
 *
 * module services version changes:
 * 1: original
 * 2: added kernel_name field and goodmodule() service
 */

#define MODLISTID	0xA841

#define ML_VER_MIN	1
#define ML_VER_MAX	1

typedef struct mod_list {
	idver		infoid;					/* id/version fro mod_list */
#if defined(NEWINFO)
	struct mod_list *mod_next;			/* pointer to next module */
	u_char		*modptr,				/* pointer to the module */
				*dataptr;				/* pointer to module's static data */
	u_int32		datasize;				/* size of module's static data */
	void		(*mod_deinit)();		/* module's deinitialize routine */
#endif
	int			reserved;				/* reserved for emergency expansion */
} mod_list, *Mod_list;

#define	MODSVCID	0xA840

#define MS_VER_MIN	1
#define MS_VER_MAX	2

#define MSV_KERNEL_NAME	2
#define MSV_GOODMODULE	2
				 
typedef struct mod_svcs{
	idver		infoid;					/* id/version for mod_svcs */
#if defined(NEWINFO)
				/* init module as P2 */
	u_int32		(*rom_modinit)(u_char *modptr, Rominfo rinf),

				/* deinit module */
				(*rom_moddeinit)(),

				/* insert into list */
				(*rom_modins)(u_char *modptr, Mod_list *mleptr, Rominfo rinf),

				/* delete module from list */
				(*rom_moddel)(u_char *modptr, Rominfo rinf);

				/* find module start ptr */
	void		(*rom_findmod)(u_char *codeptr, u_char **modptr);

				/* find module list entry */
	u_int32		(*rom_findmle)(u_char *modptr, Mod_list *mleptr, Rominfo rinf);

				/* scan for modules */
	void		(*rom_modscan)(u_char *modptr, u_int32 hdrchk, Rominfo rinf);

	Mod_list	rom_modlist;			/* low-level module list */
	char		*kernel_name;			/* pointer to kernel name string */

				/* validate module */
	u_int32		(*goodmodule)(u_char *modptr, u_int32 bootsize, u_int32 *modsize,
								u_int32 kerchk, Rominfo rinf);
#endif
	int			reserved[4];			/* reserved for emergency expansion */
} mod_svcs, *Mod_svcs;

/*
 * the timer services structures
 *
 * timer services version changes:
 * 1: original
 */

#define TIMSVCID	0xA850

#define	TVC_VER_MIN	1
#define TVC_VER_MAX 1

typedef struct tim_svcs {
	idver		infoid;					/* id/version for tim_svcs */
#if defined(NEWINFO)
	error_code	(*timer_init)(Rominfo);	/* initialize the timer */
	void		(*timer_set)(Rominfo, u_int32);	/* set timeout value & start */
	u_int32		(*timer_get)(Rominfo);		/* get time left, zero = expired */
	void		(*timer_deinit)(Rominfo);	/* de-initialize timer */
	void		*timer_data;			/* local data structure */
	u_int32		rom_delay;				/* delay loop counter, 1us delay */
#endif
	int			reserved;				/* reserved for emergency expansion */
} tim_svcs, *Tim_svcs;

/*
 * the console services structure
 *
 * version changes:
 * 1: original
 */
 
#define	CONSVCID	0xA860

#define CON_VER_MIN	1
#define CON_VER_MAX	1

typedef struct cons_svcs {
	idver		infoid;					/* id_version for cons_svcs */
#if defined(NEWINFO)
	Consdev		rom_conslist,			/* list of console type devices */
				rom_incons,				/* input console device */
				rom_outcons,			/* output console device */
				rom_in2cons,			/* second input console device */
				rom_out2cons;			/* second output console device */
	char		(*rom_getchar)(Rominfo), /* get a character from console */
				(*rom_getc)(Rominfo, Consdev); /* get a character from some console */
	void		(*rom_putchar)(char, Rominfo), /* put a character to console */
				(*rom_putc)(char, Rominfo, Consdev); /* put a character to some console */
	char		*(*rom_gets)(char *, u_int32, Rominfo); /* use rom_getchar for a line */
	void		(*rom_puts)(char *, Rominfo), /* use rom_putchar for a line */
				(*rom_puterr)(error_code, Rominfo); /* output code in xxx:xxx */
	int32		(*rom_fprintf)();		/* printf function for rom code */
#endif
	int			reserved;				/* reserved for emergency expansion */
} cons_svcs, *Cons_svcs;

/*
 * the protocol manager structures
 *
 * protocol server version changes:
 * - see below
 *
 * protocol manager version changes:
 * 1: original
 *
 * manager statics version changes:
 * 1: original
 *
 * connections version changes:
 * 1: original
 * 2: Added state1 field (previously rsv1).
 *    Defined CANTRCVMORE bit in state1 field.
 *
 */

#ifndef PROTO
typedef char *LlMbuf;
#else
#include <SPF/mbuf.h>
#define LlMbuf Mbuf
#endif

typedef struct proto_srvr proto_srvr, *Proto_srvr;

#define NO_MTU		0xFFFF		/* Protocols with no MTU use this macro		*/

#define PRTCONNID	0xA873

#define PCI_VER_MIN 1
#define PCI_VER_MAX 2

#define PCV_STATE1_FIELD	2
#define PCV_CANTRCVMORE		2

#define MAXLLPMPROTOS	7

typedef struct llpm_conn {	/* low-level protocol manager connection */
	idver		infoid;					/* ID/version information			*/
#if	defined(NEWINFO)
	u_int32		stack_depth;	/* Number of protocols on the protocol stack */
	Proto_srvr	*stack;			/* Ponter to protocol stack for the conn	*/
	u_int16		min_mtu,		/* Min MTU of conn stack (ID by protoman)	*/
				hdr_sum,		/* sum of headers for all protocols in conn */
				tlr_sum;		/* sum of trailers for all protocls in conn */
	u_char		state,			/* connection state see defs below			*/
				state1,			/* version 2 field, for connection state.   */
				src_addr[16],	/* Source address field						*/
				dst_addr[16];	/* Destination address field				*/
	LlMbuf		rxmb_ptr,		/* Receive buffer pointer					*/
				txmb_ptr;		/* Transmit buffer pointer					*/
	event_id	rcv_evid,		/* Receive event id (for hlProto only)		*/
				snd_evid;		/* Transmit event id ( - do - )				*/
#endif
} llpm_conn, *Llpm_conn;

#define CST_CONNECTED	0x01	/* connection state: connected */
#define CST_LISTENING	0x02	/* connection state: listening */
#define CST_ACKRCVD		0x04	/* acknowledgement received */
#define CST_OS_MODE		0x08	/* OS Mode if set, else Rombug Mode */
#define CST_DATA_RCVD   0x10	/* data has been received (OS Mode only) */
#define CST_DATA_USED	0x20	/* data has been consumed. (OS mode only) */
#if 0
#define CST_CONN_RCVD	0x20	/* connect request has been received */
#endif
#define CST_BOUND		0x40	/* llpath is bound to an abbress */
#define CST_BLKD_READ	0x80	/* TCP indicates if it is a blocked read */

/*  macros to be used by state1 field, version 2 of llpm_conn. */
#define CST_CANTRCVMORE	0x01	/* LLPMConn cannot be used anymore. */

#define PRMDATAID	0xA872

#define PMD_VER_MIN 1
#define PMD_VER_MAX 1

#define MAXLLPMCONNS	16

typedef struct llpm_static { /* low-level protocol manager data area */
	idver		infoid;
#if	defined(NEWINFO)
	u_int32		num_llconns;	/* Number of llpmconns */
	Llpm_conn	*llconn;		/* Pointer to llpmconn ptr array. */
#endif
} llpm_static, *Llpm_static;

#define PRTSVRID	0xA871

#define PVR_VER_MIN 1
#define PVR_VER_MAX 2

/* struct proto_srvr version changes:
 *  1: Original version.
 *  2: Added (a) proto_drv_name for booter linkage,
 *           (b) proto_hwaddr array and proto_irqlevel for mac hw support,
 *           (c) proto_ipaddr array which supercedes proto_addr from V1.
 *     Added proto_bootp to the proto_srvr structure. 
 *           PVR_VER_MAX was not changed because almost all low level
 *           protocol modules had an incorrect version number check and
 *           they all broke. They have now been fixed and in the future
 *           we may be able to use the PVR_VER_MAX field. For now, because
 *           the proto struct is initialized to all zeros we can detect
 *           the presence of bootp information by the field being non-zero.
 */
 
#define PVV_BOOTER_LINKAGE		2
#define PVV_MAC_HW				2
#define PVV_EXTENDED_IPADDR		2

/* Protocol IDs for proto_type_id field reside in ipproto.h.
 *
 * **NOTE** Protocol IDs for internet protocols with IPPROTO_xx
 *          fields defined in "in.h" should use those.
 */
#ifndef	_EDITMOD	/* include for protocol modules/drivers only */
#include <ROM/ipproto.h>
#endif

/**** NOTE: use proto_type_id of IPPROTO_RAW for IP.  Not IPPROTO_IP.
 ****       Got bit by that one a few times
 ****/

struct proto_srvr {
	idver		infoid;						/* id/version for proto_srvr	*/
#if	defined(NEWINFO)
	Proto_srvr	next;						/* next protocol stack in list	*/
	u_int32		proto_type_id;							/* protocol id	*/
	error_code	(*proto_install)(Rominfo, u_char *),	/* Installation	*/
				(*proto_iconn)(Llpm_conn, u_int32),		/* initiate conn */
				(*proto_read)(Llpm_conn, u_int32, LlMbuf *),	/* read conn */
				(*proto_write)(Llpm_conn, u_int32),				/* write conn */
				(*proto_status)(Llpm_conn, u_int32, void *),	/* get status */
				(*proto_tconn)(Llpm_conn, u_int32),		/* terminate conn */
				(*proto_deinstall)(Rominfo),			/* De-installation */
				(*proto_timeout)(Rominfo, Proto_srvr),	/* timeout processing */
				(*proto_upcall)(Rominfo, Proto_srvr, void*); /* LL ISR upcall */
	void		*proto_data;			/* server local data structure ptr */
	u_int32		proto_data_size,		/* protocol's data area size */
				proto_conn_cnt;			/* number of active connections */
	Consdev		proto_cons_drvr;		/* llvl serial comm console (slip) */
	u_int16		proto_mtu,				/* Max Xmission Unit for protocol */
				proto_hdr_len,			/* Space requirements for header */
				proto_tlr_len;			/* Space requirements for trailer */
	u_char		proto_flags;			/* Protocol status & type flags */
	u_char		proto_rsv1;				/* align on longword boundary */
	u_int32		proto_addr;	/* V1 only -   IP address, null except drivers */
	u_char		*proto_globs;			/* Pointer to protocol srvr globals */
	u_int32		proto_vector,			/* vector for lldrivers */
				proto_priority;			/* llisr priority */
	void		*proto_port_addr;		/* lldriver port address */
	/* fields added at V2 */
	u_char		proto_ipaddr[16];		/* Extended IP address */
	u_char		proto_hwaddr[16];		/* Physical (MAC) address */
	u_int32		proto_irqlevel;			/* IRQ level for low-level drivers */
	char		*proto_drv_name;		/* name identifier of protocol/driver */
	void        *proto_bootp;           /* stores bootp server's response */
    u_int32		proto_rsv2[5];			/* reserved for emergency expansion */
#else
	int			reserved;
#endif
};

/* values for proto_flags
 */
#define PVR_RELIABLE		0x01		/* reliable protocol */
#define PVR_LLISR_REG_REQ	0x02		/* request LLISR registration */
#define PVR_LLISR_REG_ERR	0x04		/* the LLISR reg req failed */
/* The following flag is to be used to indicate which driver to use, if
 * the interface IP address does not match that specified during the bind. */
#define PVR_DRV_USEME		0x08
#define	PVR_BOOTDEV			0x10	/* To indicate interface booted from */
#define	PVR_MWRSV0			0x20
/* We might use these for distinguishing protocols/drivers at some point */
#define PVR_DRIVER			0x40
#define PVR_PROTOCOL		0x80

/* Reserved Flags for Microware for proto_rsv1 field of proto_srvr */
#define PVR_MWRSV1			0x01
#define PVR_MWRSV2			0x02
#define PVR_MWRSV3			0x04
#define PVR_MWRSV4			0x08

/* For OEM User use */
#define PVR_OEM1			0x10
#define PVR_OEM2			0x20
#define PVR_OEM3			0x40
#define PVR_OEM4			0x80

/* subcodes for implementation by ethernet driver proto_status()
 */
#define SS_IntEnable		0x01
#define SS_IntDisable		0x02
#define SS_RombugOn			0x03
#define SS_RombugOff		0x04
#define SS_Setstatics		0x05
/* For defines from here on, the driver may assume that the caller
 * has passed the parameter block as the first parameter to proto_status
 * that contains the code and also data if any.
 */
#define SS_SetRtEntry		 0x06
#define SS_SetSubnetMask 0x07

#define PRTMANID	0xA870

#define PRM_VER_MIN 1
#define PRM_VER_MAX 1

typedef struct proto_man {
	idver		infoid;					/* id/version for proto_man */
#if	defined(NEWINFO)
	Proto_srvr	rom_pm_list;			/* list of protocols available */
	Llpm_static	rom_pm_static;			/* ptr to protoman's static */
	u_int32		rom_pm_done;			/* track done things, to be undone */
	error_code	(*rom_pm_open)(void *, Rominfo), /* ll open */
				(*rom_pm_read)(void *, Rominfo), /* ll socket read */
				(*rom_pm_write)(void *, Rominfo), /* ll socket write*/
				(*rom_pm_getstat)(void *, Rominfo), /* ll socket getstat */
				(*rom_pm_setstat)(void *, Rominfo),
				(*rom_pm_close)(void *, Rominfo),	/* ll socket close */
				(*rom_pm_term)(Rominfo);			/* terminate protoman */
	u_int32		reserved[3];			/* reserved for emergency expansion */
#else
	int			reserved;
#endif
} proto_man, *Proto_man;

/* definitions for the rom_pm_done field in proto_man */
#define PRM_PM_ALLOCATED	0x00000001	/* proto_man allocated */
#define PRM_PS_ALLOCATED	0x00000002	/* protoman static allocated */
#define PRM_PC_ALLOCATED	0x00000004	/* llpm_conn's allocated */
#define PRM_LLISR_REG_REQ	0x00000008	/* a LLISR is requesting ISR install */
#define PRM_MALLOC_ERR		0x00000010	/* malloc err during hlProto Thread */
#define PRM_LLISR_REG_ERR	0x00000020	/* a LLISR reg req failed */


/*
 * the debug services structures
 *
 * debug client version changes:
 * 1: original
 *
 * debug server version changes:
 * 1: original
 *
 * debug services version changes:
 * 1: original
 * 2: Added Multi-Client support
 */

#define	DBGCLNTID	0xA882

#define	DCV_VER_MIN	1
#define DCV_VER_MAX 1
 
typedef struct dbg_client dbg_client, *Dbg_client;
struct dbg_client {
	idver		infoid;					/* id/version for dbg_client */
#if defined(NEWINFO)
	char		*client_name;			/* name used to refer to client */
	void		*rom_globals,			/* client's global storage */
				(*call_back)(void *);	/* client's callback function */
	int			(*rom_qual_brk)();		/* lets ext code qualify breakpoint */
	Dbg_client	next_client;			/* next client in the list */
#endif
	int			reserved;				/* reserved for emergency expansion */
};

#define	DBGSVRID	0xA881

#define	DSV_VER_MIN	1
#define DSV_VER_MAX 1
 
typedef struct dbg_server {
	idver		infoid;					/* id/version for dbg_server */
#if defined(NEWINFO)
	error_code	/* attach a new debugger */
					(*dbg_init)(Rominfo, int, void *, void *),
				/* set/clear a breakpoint */
					(*dbg_bkpt)(Rominfo, void *, void(*)()),
				/* monitor exception */
					(*dbg_monitor)(Rominfo, u_int32, void(*)(), 
						int super, int user),
				/* set/clear a watchpoint */
					(*dbg_watch)(Rominfo, void *, void(*)()),
				/* return to given context */
					(*dbg_exec)(Rominfo, void *, int, int32 *, int *, u_int32 *);
	void		/* return debugger's globals */
					*(*dbg_get_globals)(Rominfo, int),
				/* refresh vectors */
					(*dbg_refresh_vect)(void),
				/* entry point for exceptions caught by ROM */
					(*dbg_except_entry)(Rominfo, void*),
				/* global storage for server */
					*dbg_globals;
#endif
	int			reserved;				/* reserved for emergency expansion */
} dbg_server, *Dbg_server;

#define DBGSVCID	0xA880

#define DBG_VER_MIN	1
#define DBG_VER_MAX 3

#define DBV_MULTI_CLIENT	2
#define DBV_EXCPTRTN		3

typedef struct dbg_svcs {
	idver		infoid;					/* id/version for dbg_server */
#if defined(NEWINFO)
	u_int32		rom_calldebug;			/* flag kernel break in coldstart */
	void		*rom_globals;			/* client's global storage */
	error_code	(*rom_dbginit)(void);	/* debugger initialization */
	void		(*rom_except)(),		/* server calls debugger through here */
				(*rom_debug)(),			/* debugger warm entry point */
				(*rom_dbgpreempt)();	/* gives debugger control from isr */
	int			(*rom_qual_brk)();		/* lets ext code qualify breakpoint */
	void		(*rom_int_disbl)(),		/* lets ext code mask interrupts */
				(*rom_int_enabl)();		/* lets ext code unmask interrupts */
	Dbg_server	rom_dbg_srvr;			/* debug server structure */
	void		(*dbg_entry)();			/* debug glue entry point */
	Dbg_client	client_list,			/* list of debugger clients */
				default_client;			/* currently active client */
	void		(*rom_excptrtn)();		/* exception return code */
#endif
	int			reserved;				/* reserved for emergency expansion */
} dbg_svcs, *Dbg_svcs;

/*
 * the boot services structure
 *
 * version changes:
 */

#define BOOTSVCID	0xA890

#define BTS_VER_MIN	1
#define	BTS_VER_MAX 1
 
typedef struct boot_svcs {
	idver		infoid;					/* id/version for boot_svcs */
#if defined(NEWINFO)
	Bootdev		rom_bdevlist;			/* boot device list */
	error_code	(*rom_bootctrl)(Rominfo), /* boot control service */
				(*rom_regbdev)(u_int32, Bootdev, Rominfo); /* booter registration service */
	Bootdev		*autolist,				/* array of Bootdevs registered */
				*menulist;
	u_int32		autosz,					/* size of registration array (in entries) */
				menusz;
	u_int32		ctrlr_id,				/* controller id */
				ctrl_flags;				/* boot control flags */
	void		(*rom_portmenu)(Rominfo); /* port dependent registration routine */
	u_char		*bctrl_statics;			/* boot control module statics */
#endif
	int			reserved;				/* reserved for emergency expansion */
} boot_svcs, *Boot_svcs;

/* Booter types for registration */
#define BT_INFO	0
#define BT_AUTO	1
#define BT_MENU	2

/* Boot Control flags */
#define BT_NOTRAPS	0x80000000

/*
 * fdman and scsiman structure identifiers.
 *
 * NOTE: the version identifiers and the structures 
 *       are defined in fdman.h and scsiman.h.
 */
 
#define FDMSVCID	0xA892			/* id reserved for fdman */
#define SMSVCID		0xA893			/* id reserved for scsiman */
#define LLSSVCID	0xA894			/* id reserved for ll scsi services*/

/*
 * the os services structure
 *
 * version changes:
 * 1: original
 * 2: Added oem_globals field for OSK
 */

#define OSSVCID		0xA8A0

#define OS_VER_MIN	1
#define OS_VER_MAX	3

#define OSV_OEM_GLOBALS		2
#define OSV_BOOT_CONFIG		3

typedef struct os_svcs {
	idver		infoid;					/* id/version for os_svcs */
	u_int32		rom_osflag;				/* non-zero when system is running */
	u_int32		oem_globals;			/* pointer to OEM globals for OSK */
	u_int32		boot_config;			/* boot configuration (see flags below)*/
	int			reserved;				/* reserved for emergency expansion */
} os_svcs, *Os_svcs;

  /* Structure for OEM's to add external commands to Rombug */
typedef struct oem_sub_svcs {
	u_int16		infoid;		      /* id/version for os_svcs */
	char *		(*oem_command)();     /* pointer to OEM command for OSK */
        u_char *        oem_static;           /* pointer to OEM static */
	int		reserved;             /* reserved for emergency expansion */
} oem_sub_svcs, *Oem_sub_svcs;

#define OEMSVCID		0xA8A1


/* "Fastboot" configuration flags for the above "boot_config" field */

/* Note: The definitions are conditionalized to avoid conflict with the
         original definitions in OS9/SRC/DEFS/sysglob.h for OSK targets.
         For now, since fastboot is not to be initially implemented in
         OSK modular roms, we will simply diable the definitions for any
         OSK target. However, we will add a check for the OSK_FASTBOOT_DEFS
         definition, which will be added to OSK's sysglob.h independently.
         Then, after both are done, the !defined(_OSK) check below can
         be removed, but it doesn't have to be done until fastboot is
         implemented in OSK modular roms, or some code common to both
         OS-9000 modular roms and the OS-9 System State debug support. */

#if !defined(_OSK) && !defined(OSK_FASTBOOT_DEFS)

#define B_QUICKVAL	0x1			/* do not validate module CRCs */
#define B_OKRAM		0x2			/* accept RAM definition without verification */
#define B_OKROM		0x4			/* accept ROM definition without verification */
#define B_1STINIT	0x8			/* accept the 1st init module found */
#define B_NOIRQMASK	0x10		/* do not mask IRQs during the bootstrap */
#define B_NOPARITY	0x20		/* do not parity initialize memory */
#define B_QUIET		0x40		/* do not output status messages */

#if	defined(_OSK)
#define OSK_FASTBOOT_DEFS
#endif	/* defined(_OSK) */

#endif /* !defined(OSK_FASTBOOT_DEFS) */

/*
 * the configuration services structure
 *
 * version changes:
 * 1: original
 */

#define CNFGSVCID		0xA8B0

#define CNFG_VER_MIN	1
#define CNFG_VER_MAX	1

/* configuration element ids */

enum	config_element_id
{
	CONS_START			=	0x10000,
	CONS_REVS			=	CONS_START,
	CONS_NAME,			/* console name				*/
	CONS_VECTOR,		/* interrupt vector number	*/
	CONS_PRIORITY,		/* interrupt priority	  	*/
	CONS_LEVEL,			/* interrupt level			*/
	CONS_TIMEOUT,		/* polling timeout			*/
	CONS_PARITY,		/* parity size				*/
	CONS_BAUDRATE,		/* baud rate 				*/
	CONS_WORDSIZE,		/* character size			*/
	CONS_STOPBITS,		/* stop bit					*/
	CONS_FLOW,			/* flow control 			*/
	CONS_END,

	COMM_START			=	0x20000,
	COMM_REVS			=	COMM_START,
	COMM_NAME,			/* console name				*/
	COMM_VECTOR,		/* interrupt vector number	*/
	COMM_PRIORITY,		/* interrupt priority	  	*/
	COMM_LEVEL,			/* interrupt level			*/
	COMM_TIMEOUT,		/* polling timeout			*/
	COMM_PARITY,		/* parity size				*/
	COMM_BAUDRATE,		/* baud rate 				*/
	COMM_WORDSIZE,		/* character size			*/
	COMM_STOPBITS,		/* stop bit					*/
	COMM_FLOW,			/* flow control 			*/
	COMM_END,


	DEBUG_START			=	0x30000,
	DEBUG_REVS  		=   DEBUG_START,
	DEBUG_NAME,			/* default debugger client name	*/
	DEBUG_COLD_FLAG,	/* flag	the client should be called at  */
						/* cold start, or not					*/
	DEBUG_END,
	
	LLPM_START			=	0x40000,
    LLPM_REVS			=	LLPM_START,
    LLPM_MAXLLPMPROTOS,			/* Max. number of protocols on the 	*/
								/* protocol stack. 					*/
    LLPM_MAXRCVMBUFS, 			/* Number of maximum receive nbuffs	*/
    LLPM_MAXLLPMCONNS,			/* Max. number of low level protoman*/
								/* connections						*/
    LLPM_IFCOUNT,			/* Number of hardware configuration entries */
    LLPM_END,

	LLPM_IF_START		=	0x50000,
    LLPM_IF_IP_ADDRESS	=	LLPM_IF_START,	/* IP Address			*/
    LLPM_IF_SUBNET_MASK,		/* Subnet mask						*/
    LLPM_IF_BRDCST_ADDRESS,		/* Broadcast address				*/
    LLPM_IF_GW_ADDRESS,			/* Gateway address					*/
    LLPM_IF_MAC_ADDRESS,		/* MAC (Ethernet) address			*/
	LLPM_IF_TYPE,				/* Type of hardware interface		*/
	LLPM_IF_ALT_PARITY,			/* Alternate serial port parity		*/
	LLPM_IF_ALT_BAUDRATE,		/* Alternate serial port baudrate	*/
	LLPM_IF_ALT_WORDSIZE,		/* Alternate serial port word size	*/
	LLPM_IF_ALT_STOPBITS,		/* Alternate serial port stop bits	*/
	LLPM_IF_ALT_FLOW,		/* Alternate serial port flow control	*/
	LLPM_IF_FLAGS,				/* Interface flags	                */
	LLPM_IF_NAME,				/* Name of hardware interface		*/
    LLPM_IF_PORT_ADDRESS,		/* Replacement HW interface address	*/
	LLPM_IF_VECTOR,				/* Interrupt vector number			*/
	LLPM_IF_PRIORITY,			/* Interrupt priority				*/
	LLPM_IF_LEVEL,				/* Interrupt level					*/
	LLPM_IF_ALT_TIMEOUT,		/* Alternate serial port timeout	*/
	LLPM_IF_USE_ALT,			/* Alternate usage flags			*/
	LLPM_IF_MTU,				/* Alternate MTU/MRU flag			*/
	LLPM_IF_END,

	BOOT_START			=	0x60000,	
    BOOT_REVS			=	BOOT_START,
    BOOT_COUNT,			/* Number of boot system configuration entries */
    BOOT_CMDSIZE,			/* Maximum size of user input string		*/
	BOOT_END,
	
	BOOTER_START		=	0x70000,
	BOOTER_ABNAME		=	BOOTER_START,	/* Abbreviated booter name	*/
	BOOTER_NEWAB,			/* Replacement abbreviated name				*/
	BOOTER_NEWNAME,			/* Replacement full name					*/
	BOOTER_AUTOMENU,		/* Auto/Menu registration flag				*/
	BOOTER_PARAMS,			/* Parameter string 						*/
	BOOTER_AUTODELAY,		/* Autoboot delay time (in microseconds)	*/
	BOOTER_END,
	
	NTFY_START			=	0x80000,
	NTFY_REVS			=	NTFY_START,
	NTFY_MAX_NOTIFIERS,		/* Maximum number of registered notifiers	*/
	NTFY_END
};

typedef struct cnfg_svcs {
	idver		infoid;					/* id/version for cnfg_svcs */
#if defined(NEWINFO)
	/* configuration service */
	error_code	(*get_config_data)(enum config_element_id id, u_int32 index,
					Rominfo rinf, void *buf);
	/* pointer to configuration data module */
	void		*config_data;
#endif
	int			reserved;				/* reserved for emergency expansion */
} cnfg_svcs, *Cnfg_svcs;

/*
 * Element id - specific function protypes: (void *)buf parameter type
 *                varies with the function called.
 *
 * Console port configuration elements:
 * get_config_data(CONS_REVS, 0, Rominfo, u_int16 *);
 * get_config_data(CONS_NAME, 0, Rominfo, char **);
 * get_config_data(CONS_VECTOR, 0, Rominfo, u_int32 *);
 * get_config_data(CONS_PRIORITY, 0, Rominfo, u_int32 *);
 * get_config_data(CONS_LEVEL, 0, Rominfo, u_int32 *);
 * get_config_data(CONS_TIMEOUT, 0, Rominfo, u_int32 *);
 * get_config_data(CONS_PARITY, 0, Rominfo, u_int8 *);
 * get_config_data(CONS_BAUDRATE, 0, Rominfo, u_int8 *);
 * get_config_data(CONS_WORDSIZE, 0, Rominfo, u_int8 *);
 * get_config_data(CONS_STOPBITS, 0, Rominfo, u_int8 *);
 * get_config_data(CONS_FLOW, 0, Rominfo, u_int8 *);
 * 
 * Communications port configuration elements:
 * get_config_data(COMM_REVS, 0, Rominfo, u_int16 *);
 * get_config_data(COMM_NAME, 0, Rominfo, char **);
 * get_config_data(CONS_VECTOR, 0, Rominfo, u_int32 *);
 * get_config_data(CONS_PRIORITY, 0, Rominfo, u_int32 *);
 * get_config_data(CONS_LEVEL, 0, Rominfo, u_int32 *);
 * get_config_data(CONS_TIMEOUT, 0, Rominfo, u_int32 *);
 * get_config_data(COMM_PARITY, 0, Rominfo, u_int8 *);
 * get_config_data(COMM_BAUDRATE, 0, Rominfo, u_int8 *);
 * get_config_data(COMM_WORDSIZE, 0, Rominfo, u_int8 *);
 * get_config_data(COMM_STOPBITS, 0, Rominfo, u_int8 *);
 * get_config_data(COMM_FLOW, 0, Rominfo, u_int8 *);
 * 
 * Debugger configuration elements:
 * get_config_data(DEBUG_REVS, 0, Rominfo, u_int16 *);
 * get_config_data(DEBUG_NAME, 0, Rominfo, char **);
 * get_config_data(DEBUG_COLD_FLAG, 0, Rominfo, u_int32 *);
 *
 * LLProtoman configuration elements:
 * get_config_data(LLPM_REVS, 0, Rominfo, u_int16 *);
 * get_config_data(LLPM_MAXLLPMPROTOS, 0, Rominfo, u_int16 *);
 * get_config_data(LLPM_MAXRCVMBUFS, 0, Rominfo, u_int16 *);
 * get_config_data(LLPM_MAXLLPMCONNS, 0, Rominfo, u_int16 *);
 * get_config_data(LLPM_IFCOUNT, 0, Rominfo, u_int32 *);
 *
 * For all LLPM interface configuration records (per entry):
 *
 * get_config_data(LLPM_IF_IP_ADDRESS, index, Rominfo, u_int8 **);
 * get_config_data(LLPM_IF_SUBNET_MASK, index, Rominfo, u_int8 **);
 * get_config_data(LLPM_IF_BRDCST_ADDRESS, index, Rominfo, u_int8 **);
 * get_config_data(LLPM_IF_GW_ADDRESS, index, Rominfo, u_int8 **);
 * get_config_data(LLPM_IF_TYPE, index, Rominfo, u_int8 *);
 * get_config_data(LLPM_IF_FLAGS, index, Rominfo, u_int16 *);
 * get_config_data(LLPM_IF_NAME, index, Rominfo, char **);
 * get_config_data(LLPM_IF_VECTOR, index, Rominfo, u_int32 *);
 * get_config_data(LLPM_IF_PRIORITY, index, Rominfo, u_int32 *);
 * get_config_data(LLPM_IF_LEVEL, index, Rominfo, u_int32 *);
 * get_config_data(LLPM_IF_MTU, index, Rominfo, u_int32 *);
 *
 * For all Ethernet LLPM interface configuration records (per entry):
 *
 * get_config_data(LLPM_IF_MAC_ADDRESS, index, Rominfo, u_int8 **);
 * get_config_data(LLPM_IF_PORT_ADDRESS, index, Rominfo, u_int32 *);
 *
 * For all SLIP LLPM interface configuration records (per entry);
 *
 * get_config_data(LLPM_IF_ALT_TIMEOUT, index, Rominfo, u_int32 *);
 * get_config_data(LLPM_IF_ALT_PARITY, index, Rominfo, u_int8 *);
 * get_config_data(LLPM_IF_ALT_BAUDRATE, index, Rominfo, u_int8 *);
 * get_config_data(LLPM_IF_ALT_WORDSIZE, index, Rominfo, u_int8 *);
 * get_config_data(LLPM_IF_ALT_STOPBITS, index, Rominfo, u_int8 *);
 * get_config_data(LLPM_IF_ALT_FLOW, index, Rominfo, u_int8 *);
 *
 * Boot system configuration elements:
 * get_config_data(BOOT_REVS, 0, Rominfo, u_int16 *);
 * get_config_data(BOOT_COUNT, 0, Rominfo, u_int32 *);
 *
 * (added at version 2):
 * get_config_data(BOOT_CMDSIZE, 0, Rominfo, u_int32 *);
 *
 * Booter configuration elements (per entry):
 * get_config_data(BOOTER_ABNAME, index, Rominfo, char **);
 * get_config_data(BOOTER_NEWAB, index, Rominfo, char **);
 * get_config_data(BOOTER_NEWNAME, index, Rominfo, char **);
 * get_config_data(BOOTER_AUTOMENU, index, Rominfo, u_int8 *);
 *
 * (added at version 2):
 * get_config_data(BOOTER_PARAMS, index, Rominfo, char **);
 * get_config_data(BOOTER_AUTODELAY, index, Rominfo, u_int32 *);
 *
 * Nofitication configuration elements:
 * get_config_data(NTFY_REVS, 0, Rominfo, u_int16 *);
 * get_config_data(NTFY_MAX_NOTIFIERS, 0, Rominfo, u_int32 *);
 *
 */

/* Interface type definitions */

#define LLPM_NOHW		0
#define LLPM_SLIP		1
#define LLPM_ETHER		2

/* SLIP Interface flag definitions */

#define LLIF_CSLIP_ON	0x8000
#define LLIF_CSLIP_OFF	0x0000
#define LLIF_SLIP_WIN2K 0x4000

/*
 * the notification services structure
 *
 * version changes:
 * 1: original
 */

#define NTFYSVCID		0xA8C0

#define NTSV_VER_MIN	1
#define NTSV_VER_MAX	1

typedef struct notify_hndlr *Notify_hndlr;
 
typedef struct notify_svcs {
	idver		infoid;					/* id/version for notify_svcs */
#if defined(NEWINFO)
	/* handler registration service */
	error_code		(*reg_hndlr)(Rominfo rinf, u_int32 priority,
					void (*handler)(u_int32 direction, void *parameter),
					void *parameter, u_int32 *hndlr_id);
	/* handler deregistration service */
	error_code		(*dereg_hndlr)(Rominfo rinf, u_int32 hndlr_id);
	/* notification service */
	error_code		(*rom_notify)(Rominfo rinf, u_int32 direction);
	Notify_hndlr	torom_list,		/* ordered lists of handlers */
					tosys_list,
					empty_list;		/* empty list of available records */
	u_int32			last_direction;	/* direction of last notification call */
#endif
	int			reserved;				/* reserved for emergency expansion */
} notify_svcs, *Notify_svcs;

/* Notification direction constants */

#define NTFY_DIR_TOROM		1
#define NTFY_DIR_TOSYS		2


/*
 * the exception services structure
 *
 * version changes:
 * 1: original
 * 2: added excpt_glbls field
 */

#define EXCPSVCID		0xA8D0

#define EXCP_VER_MIN	1
#define EXCP_VER_MAX	2

#define EXCV_GLOBALS	2

typedef struct excpt_svcs {
	idver		infoid;					/* id/version for excpt_svcs */
	/* register/deregister an exception handler */
	error_code	(*reg_excpt)(u_int32 excpt_number, u_int32 priority,
			error_code (*handler)(), void *statics, void *port, Rominfo rinf);
	/* poll through registered exception handlers */
	void		(*dispatch_excpt)(Rominfo rinf, u_int8 *stack_frame,
					u_int32 excpt_number);
	/* check if a hardware port is present */
	error_code	(*hw_probe)(void *addr, u_int32 ptype, Rominfo rinf);
	void		*excpt_list;			/* list of exception handlers */
	u_char		*excpt_glbls;			/* exception module globals */
	u_int32		reserved;				/* reserved for emergency expansion */
} excpt_svcs, *Excpt_svcs;


/*
 * the compression services structure
 *
 * version changes:
 * 1: original
 */

#define COMPRSVCID		0xA8E0

#define COMPR_VER_MIN	1
#define COMPR_VER_MAX	1

  /* defined service types are */
#define COMPR_INFLATE 1 /* zip/gzip/zlib inflate method */
#define COMPR_DEFLATE 2 /* zip/gzip/zlib deflate method */

typedef struct compr_svcs {
	idver		infoid;					/* id/version for excpt_svcs */
	/* compress/uncompress call */
	error_code	(*work)(u_int32 type, u_int8 *dest, u_int32 *dest_len,
                     CONST_ROM_H u_int8 *source, u_int32 source_len, Rominfo rinf);
	u_int32		*compr_ll;  			/* compression modules linked list*/
	u_int32		reserved1;				/* reserved for emergency expansion */
	u_int32		reserved2;				/* reserved for emergency expansion */
} compr_svcs, *Compr_svcs;


/*
 * the new rominfo structure
 *
 * version changes:
 * 1: original
 * 2: added excpt_svcs pointer field
 */

#define INFOID		0xA800

#define	INF_VER_MIN	1
#define INF_VER_MAX 3

#define INV_EXCPTS	2
#define INV_COMPRESS 3
 
typedef struct newinfo {
	idver		infoid;					/* id_version for rominfo */
	Hw_config	hardware;				/* hardware config struct ptr */
	Mem_svcs	memory;					/* memory services struct ptr */
	Rom_svcs	rom;					/* rom services struct ptr */
	Mod_svcs	modules;				/* module services struct ptr */
	Tim_svcs	timer;					/* timer services struct ptr */
	Cons_svcs	cons;					/* console services struct ptr */
	Proto_man	protoman;				/* protocol manager struct ptr */
	Dbg_svcs	dbg;					/* debugger services struct ptr */
	Boot_svcs	boots;					/* boot services struct ptr */
	Os_svcs		os;						/* OS services struct ptr */
	Cnfg_svcs	config;					/* configuration services struct ptr */
	Notify_svcs	notify;					/* notification services struct ptr */
	Excpt_svcs	excpts;					/* exception services struct ptr */
    Compr_svcs  comprs;                 /* compression services struct ptr */
	u_int32		reserved;
	u_int32		reserved2;
	u_int32		reserved3;
} newinfo, *Newinfo;

/* now complete the rominfo definition */

#if !defined(__cplusplus)
#define new_style new
#endif

union rominfo {
	oldinfo		old;
	newinfo		new_style;
};

#if defined(OLDINFO)
#define 	rom_osflag			old.rom_osflag
#define 	rom_glbldata		old.rom_glbldata
#define 	rom_initsp			old.rom_initsp
#define 	rom_vectors			old.rom_vectors
#define 	rom_start			old.rom_start
#define 	rom_debug			old.rom_debug
#define 	rom_set_dbg_vects	old.rom_set_dbg_vects
#define 	rom_ostart			old.rom_ostart
#define 	rom_memlist			old.rom_memlist
#define 	rom_romlist			old.rom_romlist
#define 	rom_totram			old.rom_totram
#define 	rom_calldebug		old.rom_calldebug
#define 	rom_cputype			old.rom_cputype
#define 	rom_malloc			old.rom_malloc
#define 	rom_free			old.rom_free
#define 	rom_getchar			old.rom_getchar
#define 	rom_getc			old.rom_getc
#define 	rom_putchar			old.rom_putchar
#define 	rom_putc			old.rom_putc
#define 	rom_gets			old.rom_gets
#define 	rom_puts			old.rom_puts
#define 	rom_conslist		old.rom_conslist
#define 	rom_incons			old.rom_incons
#define 	rom_outcons			old.rom_outcons
#define 	rom_in2cons			old.rom_in2cons
#define 	rom_out2cons		old.rom_out2cons
#define 	rom_bdevlist		old.rom_bdevlist
#define 	rom_fputype			old.rom_fputype
#define 	rom_consumed		old.rom_consumed
#define 	rom_ramlimit		old.rom_ramlimit
#define 	rom_delay			old.rom_delay
#define 	rom_dmabuff			old.rom_dmabuff
#define		rom_kernel_extnd	old.rom_kernel_extnd
#define		rom_newinfo			old.rom_newinfo
#endif

#if defined(NEWINFO)
#define		hardware			new_style.hardware
#define		memory				new_style.memory
#define		rom					new_style.rom
#define		modules				new_style.modules
#define		timer				new_style.timer
#define		cons				new_style.cons
#define		protoman			new_style.protoman
#define		dbg					new_style.dbg
#define		boots				new_style.boots
#define		os					new_style.os
#define		config				new_style.config
	/* If the _ITEM header file has been included, call this notify_rom */
#ifdef _ITEM 
#define		notify_rom		new_style.notify
#else
#define		notify				new_style.notify
#endif
#define		excpts				new_style.excpts
#define		comprs  			new_style.comprs
#endif

#undef CONST_ROM_H
	
#endif

