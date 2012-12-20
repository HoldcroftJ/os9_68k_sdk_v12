/*
 * $Header:   /h0/MWOS/SRC/ROM/SERIAL/68681/VCS/io68681.h_v   1.14   09 Jul 1998 11:48:52   alexs  $
 * $Revision:   1.14  $
 */
/*--------------------------------------------------------------------------,
|                                                                           |
|           Copyright 1989, 1996 by Microware Systems Corporation           |
|                          Reproduced Under License                         |
|                                                                           |
|  This source code is the proprietary confidential property of Microware   |
|  Systems Corporation, and is provided to licensee for documentation and   |
|  educational purposes only. Reproduction, publication, or distribution    |
|  in any form to any party other than the licensee is strictly prohibited. |
|                                                                           |
|---------------------------------------------------------------------------|
| Definitions for the 68681 serial chip.									|
|																			|
| Edition History:															|
|																			|
| Ed#	Date	Changes													By	|
| --- --------	------------------------------------------------------- --- |
|  01 88/05/31  Conversion to OS-9000.                                  jal |
|  02 88/11/14  Converted for use in boot code.                         djl |
|  03 95/01/25  Converted to new rom and ppc                            rg  |
|  04 96/01/22  Added sysglob.h and module.h include files              rws |
|               and added rinf, hndlr_id, and INTSM_save to dat68681        |
|  11 96/05/08  Recreated                                               sep |
|  12 96/07/03  Added definitions for shared register images in OSK.    ats |
|               Added fields to driver data structure for correct           |
|               register sharing and baud rate set preferences in           |
|               OS-9000.                                                    |
|  13 96/08/05  Added error and upcall overhead statistics fields.      ats |
|  14 96/09/18  Modified for compatability with sc68681.h               sep |
|               ---- OS-9/68K V3.0.3 System-state Debug Support Release --- |
|  15 96/09/26  Modified  configuration for IDT MIPS board              sep |
|  16 96/09/26  _MIPS3000 constant macro changed to _MPFMIPS3000.       cdg |
|  17 97/02/10  Moved MIPS specific definiton to systype.h              sep |
|               ---- OS-9000/ARMv3 V2.2 Released ----                       |
|               ---- OS-9000/ARMv3 V2.2.1 Released ----                     |
|               ---- Modular Rom v1.1 Sub-component Released ----           |
|               ---- OS-9000/505EVB V1.0 Released ----                      |
|               ---- Modular ROM v1.2 Sub-component Released ----           |
|               $$                    <RELEASE_INFO>                     $$ |
`--------------------------------------------------------------------------*/

#if !defined(_SC68681_H)
#include <68681.h>
#endif

/* OS-9/68K OEM Globals Shared Register Images */

#ifdef  _OSK
typedef struct {
	    u_char      shared_IMR;         /* Interrupt Mask Register image */
        u_char      shared_ACR;         /* Auxiliary Control Register image */
	    } oem_globals, *OEM_globals;
#endif


/*
 *  Miscellaneous Definitions
 */

#ifndef		XOFF
#define		XOFF	        0x13
#endif

#ifndef		XON
#define		XON		        0x11
#endif

#ifndef     FALSE
#define     FALSE           0
#endif

#ifndef     TRUE
#define     TRUE            ~FALSE
#endif

#ifndef     SUCCESS
#define     SUCCESS         0
#endif

#ifndef     NULL
#define     NULL            0
#endif

#define     FOREVER         1

/*
 * 68681 specific flags
 */
   
#define     SWSHAKE     0x80000000      /* software handshaking enabled  */
#define     SWSTRICT    0x40000000      /* strict software handshaking   */
#define     HWSHAKE     0x20000000      /* hardware handshaking enabled  */
#define     TXOFF       0x10000000      /* output flow control is on     */

#define		INSTRICT	(TXOFF|SWSTRICT)

#define     POLINT_ON   0x08000000      /* polled int. mode              */
#define     INT_STAT    0x04000000      /* interupt enable save status   */

/*
 * 68681 specific data
 */

#define		ASIDE		1
#define		BSIDE		2

#ifdef	_OS9000
/* The following definitions must match the OS-9000 sc68681 driver
   definitions for baud rate set preferences. */
#define 	USESET1		0x40
#define		USESET2		0x80
#define		DONTCARE	0
#endif
   
typedef struct dat68681 dat68681, *Dat68681;
struct dat68681 {
	Rominfo		rinf;               /* pointer back to Rominfo structure    */
	u_char		devside;		    /* device side (A/B)                    */
	u_char		resv1;
	u_int16		resv2;
	u_int32		hndlr_id;           /* handler ID for notification services */
	u_char		*uart_base;         /* base address of UART                 */
	Consdev		otherside;			/* pointer back to other side's consdev	*/
#ifdef	_OSK
	u_int32		oem_globals_offset;	/* offset into OEM globals for part		*/
	OEM_globals shared_images_ptr;	/* pointer to shared register images	*/
#endif
#ifdef	_OS9000
	u_char		pref_baudset;		/* Preferred baudrate set for this side */
	/* The following are kept in the a-side data area only ! */
	u_char		acr_image;			/* ACR image to share with scf driver	*/
	u_char		imr_image;			/* IMR image to share with scf driver	*/
	u_char		opcr_image;			/* OPCR image to share with scf driver	*/
#endif
	u_int32		parerrs;
	u_int32		frmerrs;
	u_int32		ovrerrs;
	u_int32		maxoverhead;		/* maximum upcall overhead              */
};
