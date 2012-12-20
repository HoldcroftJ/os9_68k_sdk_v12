




/* OS-9/68k module header definitions */
#if !defined(_MODULE_H)
#define _MODULE_H

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
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
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 98/07/29 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|  02 01/03/02 Add const where appropriate                             mgh  |
|  03 01/09/28 Added subroutine and trap module number definitons	   mrg  |
|  04 01/10/03 Added SLIB_NETDB                                        rds  |
|              Converted file from unix to dos format                  rds  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/


#define VHPCNT  (sizeof(struct modhcom)-2)  /* sizeof common header */
#define MODSYNC 0x4afc      /* module header sync code */
#define CRCCON	0x800fe3	/* crc polynomial constant */

/* Module access permission values */
#define MP_OWNER_READ	0x0001
#define MP_OWNER_WRITE	0x0002
#define MP_OWNER_EXEC	0x0004
#define MP_GROUP_READ	0x0010
#define MP_GROUP_WRITE	0x0020
#define MP_GROUP_EXEC	0x0040
#define MP_WORLD_READ	0x0100
#define MP_WORLD_WRITE	0x0200
#define MP_WORLD_EXEC	0x0400
#define MP_OWNER_MASK	0x000f
#define MP_GROUP_MASK	0x00f0
#define MP_WORLD_MASK	0x0f00
#define MP_SYSTM_MASK	0xf000

/* Module Type/Language values */
#define MT_ANY		0
#define MT_PROGRAM	1
#define MT_SUBROUT	2
#define MT_MULTI	3
#define MT_DATA		4
#define MT_CSDDATA	5
#define	MT_CDBDATA	MT_CSDDATA		/* same module type as MT_CSDDATA */
#define MT_TRAPLIB	11
#define MT_SYSTEM	12
#define MT_FILEMAN	13
#define MT_DEVDRVR	14
#define MT_DEVDESC	15

#define ML_ANY		0
#define ML_OBJECT	1
#define ML_ICODE	2
#define	ML_PCODE	3
#define	ML_CCODE	4
#define	ML_CBLCODE	5
#define	ML_FRTNCODE	6
#define	ML_JAVACODE	7

#define mktypelang(type,lang)	(((type)<<8)|(lang))

/* Module Attribute values */
#define MA_REENT	0x80
#define MA_GHOST	0x40
#define MA_SUPER	0x20

#define mkattrevs(attr, revs)	(((attr)<<8)|(revs))

/* configuration module:  version smoothing definitions (_mcompat field) */

#define	CP_NOMOVES		(1<<0)	/* no CPU-Space MOVES during coldstart (kerc32) */
#define	CP_NOSTOP		(1<<1)	/* don't use STOP instruction */
#define	CP_NOGHOST		(1<<2)	/* don't retain ghost/sticky modules */
#define	CP_NOBURST		(1<<3)	/* don't enable 68030 burst-fill */
#define	CP_ZAPMEM		(1<<4)	/* patternize memory when allocated & freed */
#define	CP_NOCLOCK		(1<<5)	/* don't start system clock during coldstart */
#define	CP_SPURIRQ		(1<<6)	/* ignore spurious IRQ's */

/* configuration module:  cache control definitions (_mcompat2 field)	*/
/* NOTE: a snoop cache is a cache that maintains coherancy through		*/
/*       hardware means, or is non-existant.							*/
#define	CP2_SEI			(1<<0)	/* external instr. caches are snoopy */
#define	CP2_SED			(1<<1)	/* external data caches are snoopy */
#define	CP2_SOI			(1<<2)	/* on-chip instr. cache is snoopy */
#define	CP2_SOD			(1<<3)	/* on-chip data cache is snoopy */
#define	CP2_CBank0		(1<<4)	/* 68349: enable cache/sram bank 0 as cache */
#define	CP2_CBank1		(1<<5)	/* 68349: enable cache/sram bank 1 as cache */
#define	CP2_CBank2		(1<<6)	/* 68349: enable cache/sram bank 2 as cache */
#define	CP2_CBank3		(1<<7)	/* 68349: enable cache/sram bank 3 as cache */
/* CP2_DDIO is obsolete for V3.0 */
/*#define	CP2_DDIO		(1<<7)*/	/* don't disable data caching when in I/O */

/* configuration module:  system configuration flags (_msysconf field) */

#define	SC_NOTBLEXP		(1<<0)	/* system tables are not expanded automatically */
#define	SC_CRCDIS		(1<<2)	/* CRC checking disabled (Atomic only) */
#define	SC_SYSTSDIS		(1<<3)	/* System-state time-slicing disabled */
/* NOTE: SC_SSM_NOPROT is only applicable to the Development kernel, */
/*       Atomic kernel always uses this mode */
#define	SC_SSM_NOPROT	(1<<4)	/* SSM builds 1 table for user-state */
/* NOTE: SC_SSM_SYSPT is not implemented as yet */
#define	SC_SSM_SYSPT	(1<<5)	/* SSM uses sys-state page tables */

typedef unsigned short ushort;

/* macro definitions for accessing module header fields */
#define MODNAME(mod) ((u_char*)((u_char*)mod + ((Mh_com)mod)->_mname))
#define MODSIZE(mod) ((u_int32)((Mh_com)mod)->_msize)

struct modhcom {
    short       _msync,     /* sync bytes ($4afc) */
				_msysrev;	/* system revision check value */
    long        _msize,     /* module size */
                _mowner,    /* owner id */
				_mname;		/* offset to module name */
	short		_maccess,   /* access permission */
                _mtylan,    /* type/lang */
                _mattrev,   /* rev/attr */
                _medit;     /* edition */
	long		_musage,	/* comment string offset */
				_msymbol;	/* symbol table offset */
	short		_mident;	/* ident code */
	char		_mspare[6];	/* reserved bytes */
	long		_mhdext;	/* module header extension offset */
	short		_mhdextsz;	/* module header extension size */
	short		_mparity;   /* header parity */
};

/* OS-9000 compatible type definition */
typedef struct modhcom mh_com, *Mh_com;

/* Executable memory module */
typedef struct {
    struct modhcom _mh;     /* common header info */
    long        _mexec,     /* offset to execution entry point */
				_mexcpt,	/* offset to exception entry point */
                _mdata,     /* data storage requirement */
                _mstack,    /* stack size */
                _midata,    /* offset to initialized data */
                _midref;    /* offset to data reference lists */
} mod_exec;

/* OS-9000 compatible type definition */
typedef mod_exec mh_exec, *Mh_exec;

/* device driver module */
typedef struct {
    struct modhcom _mh;     /* common header info */
    long        _mexec,     /* offset to execution entry point */
				_mexcpt,	/* offset to exception entry point */
                _mdata;     /* data storage requirement */
    short		_mdinit,	/* offset to init routine */
    			_mdread,	/* offset to read routine */
    			_mdwrite,	/* offset to write routine */
    			_mdgetstat,	/* offset to getstat routine */
    			_mdsetstt,	/* offset to setstat routine */
    			_mdterm,	/* offset to terminate routine */
    			_mderror;	/* offset to exception error routine */
} mod_driver;

/* OS-9000 compatible type definition */
typedef mod_driver mh_drvr, *Mh_drvr;

/* Device descriptor module */
typedef struct {
    struct modhcom	_mh;     /* common header info */
	char *			_mport;		/* device port address */
	unsigned char	_mvector;	/* trap vector number */
	unsigned char	_mirqlvl;	/* irq interrupt level */
	unsigned char	_mpriority;	/* irq polling priority */
	unsigned char	_mmode;		/* device mode capabilities */
	short			_mfmgr;		/* file manager name offset */
	short			_mpdev;		/* device driver name offset */
	short			_mdevcon;	/* device configuration offset */
	unsigned short	_mdscres[1];/* (reserved) */
	unsigned long	_mdevflags;	/* reserved for future usage */
	unsigned short	_mdscres2[1];	/* reserved */
	unsigned short	_mopt;		/* option table size */
	unsigned char	_mdtype;	/* device type code */
} mod_dev;

/* OS-9000 compatible type definition */
typedef mod_dev mh_dev, *Mh_dev;

/* Configuration module */
typedef struct mod_config {
    struct modhcom _mh;     	/* common header info */
	long		_mmaxmem;		/* top limit of free ram (unused) */
	unsigned short
				_mpollsz,		/* number of IRQ polling tbl entries */
				_mdevcnt,		/* number of device table entries */
				_mprocs,		/* number of process table entries */
				_mpaths,		/* number of path table entries */
				_msysparam,		/* offset to parameter string for _msysgo */
				_msysgo,		/* offset to initial module name */
				_msysdrive,		/* offset to system device name */
				_mconsol,		/* offset to system consol terminal name */
				_mextens,		/* offset to customization module name list */
				_mclock,		/* offset to clock module name */
				_mslice,		/* number of clock ticks per time slice */
				_mip_id;		/* interprocessor identification */
	long		_msite;			/* installation site code */
	unsigned short
				_minstal;		/* installation name offset */
	long		_mcputyp;		/* cpu class (68000/010/020/030/3XX/040/070) */
	char		_mos9lvl[4];	/* operating system level/version/edition */
	unsigned short
				_mos9rev,		/* offset to OS-9 level/revision string */
				_msyspri,		/* initial system priority */
				_mminpty,		/* initial minimum executable priority */
				_maxage,		/* initial maximum natural process age */
				_mmdirsz,		/* initial maximum module count */
				_mreserv3;		/* reserved */
	unsigned short
				_mevents;		/* number of system event table entries */
	char		_mcompat,		/* version change smooth byte #1 */
				_mcompat2;		/* version change smoothing byte #2*/
	unsigned short
				_mmemlist,		/* offset to (colored) memory list */
				_mstacksz,		/* IRQ stack size (in longwords) */
				_mcoldretrys,	/* coldstart's "chd" retry counter */
				_mreserv2[2],	/* reserved */
				_mcachelist,	/* offset to SSM(MMU) cache modes list */
				_mioman,		/* offset to IOMan module name */
				_mpreio,		/* offset to "pre-IO" P2 extension module list*/
				_msysconf,		/* system configuration control flags */
	/* NOTE: _mnumsigs is currently unimplemented */
				_mnumsigs,		/* max number of queued signals per process */
				_mprcdescstack, /* process descriptor stack size */
				_mreserved[32];	/* reserved space */
} mod_config;

typedef struct mod_dir {
	struct modhcom *md_mptr;	/* module ptr */
	struct modhcom *md_group;	/* module group ptr */
	unsigned long md_static;	/* module group memory size */
	unsigned short md_link;		/* module link count */
	unsigned short md_mchk;		/* module header checksum */
} mod_dir, *Mod_dir;

/* OS-9000 compatible type definition */
#if !defined(_MH_CONFIG)
#define _MH_CONFIG
typedef mod_config mh_config, *Mh_config;
#endif /* _MH_CONFIG */

/* Data memory module */
typedef mod_exec mh_data, *Mh_data;

/* File manager memory module */
typedef struct {
    struct modhcom _mh;     /* common header info */
    long        _mexec,     /* offset to execution entry point */
				_mexcpt;	/* offset to exception entry point */
} mh_fman, *Mh_fman;

/* Trap handler module */
typedef mh_exec mh_trap, *Mh_trap;

/* These are definitions of subroutine and trap module numbers
 0 - 8 and 13 - 15   reserved for microware use only
 9 - 12     not reserved
 10, 11, and 12 are in conflict with non 68k version, in non 68k 0 - 9
are microware use only, and 10 - 15 are not reserved */

#define SLIB_CSL 0
#define SLIB_MATH 1
#define SLIB_PCI 2
#define SLIB_MAUI 8
#define SLIB_NETDB 9

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#define _CONST const
#else
#define _OP(x) ()
#define _CONST
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

error_code _os_crc _OP((void *, u_int32, int32 *));
error_code _os_datmod _OP((_CONST char *, u_int32, u_int16 *, u_int16 *, u_int32, void **, mh_data **));
error_code _os_get_moddir _OP((void *, u_int32 *));
error_code _os_link _OP((char **, mh_com **, void **, u_int16 *, u_int16 *));
error_code _os_load _OP((_CONST char *, mh_com **, void **, u_int32, u_int16 *, u_int16 *, u_int32));
error_code _os_loadp _OP((_CONST char *, u_int32, char *, mh_com **));
error_code _os_mkmodule _OP((_CONST char *, u_int32, u_int16 *, u_int16 *, u_int32, void **, mh_com **, u_int32));
error_code _os_setcrc _OP((mh_com *));
error_code _os_unlink _OP((mh_com *));
error_code _os_unload _OP((_CONST char *, u_int32));
error_code _os_tlink _OP((u_int32, _CONST char *, void **, mh_trap **, void *, u_int32 ));
error_code _os9_vmodul _OP((u_int32, u_int32, mh_com *, mod_dir **));

#if defined(_OPT_PROTOS)
int crc _OP((void *, unsigned int, int *));
int _get_module_dir _OP((void *, int));
mh_com *make_module _OP((_CONST char *, int, int, int, int, int));
void *_mkdata_module _OP((_CONST char *, unsigned int, int, int));
mh_com *modcload _OP((_CONST char *, int, int));
mh_com *modlink _OP((_CONST char *, int));
mh_com *modload _OP((_CONST char *, int));
mh_com *modloadp _OP((_CONST char *, int, char *));
int munlink _OP((mh_com *));
int munload _OP((_CONST char *, int));
char *_prgname _OP((void));
int _setcrc _OP((mh_com *));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _MODULE_H */
