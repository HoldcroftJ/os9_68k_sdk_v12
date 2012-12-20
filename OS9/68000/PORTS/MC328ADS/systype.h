/*****************************************************************************
******************************************************************************
* SysType.h  - system specific definitions for the MC68328DAS.
*
*      These definitions must agree with the definitions that are
*   put forth in systype.d.  If you change systype.d and not
*   systype.h, you could be in for some surprises.
*
******************************************************************************
** Copyright 1995, 1996 by Microware Systems Corporation                    **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
*   0  95/11/03  Adapted from delta's OS9/68020/PORTS/DBG162        rws     **
*   1  96/02/07  Made serial driver priority higher, modified       rws     **
*                LLPM_CNFG_ARRAY                                            **
*   2  96/05/13  Updated Copyright date.                            ats     **
*   3  96/05/21  Added conditionals for low-level serial driver     ats     **
*                compilation and adjust configuration.                      **
*   4  96/05/21  Eliminated unnecessary definitions.                ats     **
*   5  96/06/05  Changed default baudrates to standard (9600).      ats     **
*                ---- OS-9/68K MC68328 Support Beta1 Release ----           **
*   6  96/07/24  Updated configuration for new cnfgdata format.     ats     **
*   7  96/07/24  Switched serial ports to match board labels.       ats     **
*   8  96/08/02  General cleanup.                                   ats     **
*                ---- OS-9/68K MC68328 Support Beta2 Release ----           **
*   9  97/02/01  added ifdef for LLNETROM or LLSLIP                 ecm     **
*  10  99/10/24  Set low-level COMM to 19200 for LLSLIP useability. rkw     **
*****************************************************************************/

#include <types.h>
#include <const.h>

#if defined(CONS68681)||defined(_IO68681_C_)

#define DUARTADDR   0xF00001      /* MC68681 DUART Address */
#define ChABase   (DUARTADDR+0x00)  /* DUART channel A */
#define ChBBase   (DUARTADDR+0x10)  /* DUART channel B */
#define DUARTVect   25        /* MC68681 DUART: Level 1 Autovector */
#define DUARTLevel  1       /* MC68681 DUART: IRQ 1 */

#define CONSADDR    ChABase
#define COMMADDR    ChBBase

/* NOTE: making interrupt priorities higher than high-level serial driver's */

#define CONSVECTOR    DUARTVect
#define CONSPRIORITY  4
#define CONSLEVEL   DUARTLevel

#define COMMVECTOR    DUARTVect
#define COMMPRIORITY  4
#define COMMLEVEL   DUARTLevel

#if defined(_IO68681_C_)

#define CONSNAME    "328:t1"
#define CONSABNAME    "t1"

#define COMMNAME    "328:term"
#define COMMABNAME    "term"

#endif /* defined(_IO68681_C_) */

#if defined(CONS68681)
#if defined(ALTCONS)

#define CONSNAME    "328:t1"
#define CONSABNAME    "t1"

#define COMMNAME    "328:term"
#define COMMABNAME    "term"

#else

#define CONSNAME    "328:term"
#define CONSABNAME    "term"

#define COMMNAME    "328:t1"
#define COMMABNAME    "t1"

#endif /* defined(ALTCONS) */
#endif /* defined(CONS68681) */
#endif /* defined(CONS68681) || defined(_IO68681_C_) */

/* 
 * cnfgdata ONLY definitions 
 */

#if defined(_CNFGDATA_C_)

#define CONS_NAME       CONSNAME
#define CONS_VECTOR     CONSVECTOR
#define CONS_PRIORITY   CONSPRIORITY
#define CONS_LEVEL      CONSLEVEL
#define CONS_TIMEOUT    2000
#define CONS_PARITY     CONS_NOPARITY

#define CONS_BAUDRATE   CONS_BAUDRATE_9600

#define CONS_WORDSIZE   CONS_8BITS
#define CONS_STOPBITS   CONS_1STOP

#if defined (HW_FLOW_CTRL)
#define CONS_FLOW     CONS_HWSHAKE
#else
#define CONS_FLOW     CONS_SWSHAKE
#endif

#define COMM_NAME       COMMNAME
#define COMM_VECTOR     COMMVECTOR
#define COMM_PRIORITY   COMMPRIORITY
#define COMM_LEVEL      COMMLEVEL
#define COMM_TIMEOUT    2000
#define COMM_PARITY     CONS_NOPARITY

#define COMM_BAUDRATE   CONS_BAUDRATE_19200

#define COMM_WORDSIZE   CONS_8BITS
#define COMM_STOPBITS   CONS_1STOP

#if defined (HW_FLOW_CTRL)
#define COMM_FLOW       CONS_HWSHAKE
#else
#define COMM_FLOW     CONS_NOSHAKE
/*#define COMM_FLOW     CONS_SWSHAKE*/
#endif

#if defined LLSLIP
#define IF_STRINGS \
CONST u_char \
 llslip_name[] = "llslip";
#endif

#if defined LLNETROM
#define IF_STRINGS \
CONST u_char \
 llnetrom_name[] = "llnetrom";
#endif
 
#define LLPM_MAXLLPMPROTOS  4
#define LLPM_MAXRCVMBUFS  40
/*#define LLPM_MAXRCVMBUFS  10*/
#define LLPM_MAXLLPMCONNS 10

#if defined LLSLIP
#define LLPM_CNFG_ARRAY \
{\
	{{206,72,8,90}, {255,255,255,0}, {255,255,255,255}, {255,255,255,255}, \
		{0,0,0,0,0,0}, LLPM_SLIP, \
		0, 0, 0, 0x80, 0, LLIF_CSLIP_ON, (u_int32)llslip_name, 0, 0, 0, 0, 0, 0} \
}
#endif

#if defined LLNETROM
#define LLPM_CNFG_ARRAY \
{\
	{{206,72,9,108}, {255,255,255,0}, {206,72,9,255}, {0,0,0,0}, \
		{0x0,0x0,0xf6,0x0,0x6b,0x28}, LLPM_ETHER, \
		0, 0, 0, 0, 0, 0, (u_int32)llnetrom_name, \
		0xffa00000, 0x44, 0x5, 0x5, 0, 0} \
}
#endif

														
#define BOOT_STRINGS \
CONST u_char \
 b1[] = "lr",\
 b2[] = "ro"; 
	
#define NTFY_CNFG_NOTIFIERS   4

#endif /* _CNFGDATA_C_ */
