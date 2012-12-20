/****************************************************************************
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
 *   1 04/04/97 Creation ... Code mix ISP/I82596 and SP21040           mil
 *     ??/??/97 Ported to SPF                                          ???
 *   2 05/22/97 Changed to not include any INET header files           tmd
 *              Followed SPLANCE's lead by putting stuff in defs.h     tmd
 *   3 07/15/97 Ported to OS9/68k                                      jpd
 *   4 08/27/97 Bug fix                                                taka
 *   5 09/08/97 fixed os_irq()                                         taka
 *   6 09/29/97 Added MVME172 support                                  pjw
 *     11/05/97 Added RFD_ERRS to rirq.c and i82596.h                  pjw
 *   7 12/05/97 Added code to support netstat utility                  jk
 *   8 01/21/98 Removed all compiler and linker warnings               jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *   9 04/06/98 Fixed a few MAKE_LOCAL_ADDR_SWAP casts to u_int32      tmd
 *                            (DDTS MHQco03924)                        tmd
 *   9 06/26/98 Changed Cache Flushing, so that the off / on counts    KeiT
 *              are correct. Modified a lot of high-load issues.
 *  10 06/11/99 Set stk_int_flags in SPF_GS_UPDATE (entry.c)           rds
 *  11 06/22/99 Integrated Kei Thomsen's changes, fixed mbuf leak.     jk
 *  12 08/29/99 Fix cache problem in some error conditions (chip.c)    rds
 *              Use lu_item_addr instead of lu_ether_addr (chip/entry) rds
 *              Enable hardware to pad short packets (entry.c)         rds
 *              Fixed lu_item_addr initialization (init.c)             rds
 *              Lots of general cleanup                                rds
 *  13 06/10/00 Fixed bus error problem under high load (rirq.c)       rds
 *              Fixed receive hangs when running low on mubfs (rirq.c) rds
 *  14 11/11/00 Modified to use PVTRAM. This is much more stable than      
 *              the normal (fast) version. Removed a lot _os_cache()       
 *              for the PVTRAM version. PVTRAM may be slower, because      
 *              it is using memcpy to move the data from the buffer to     
 *              the mbufs. It is still fast and not longer crashing!   KeiT
 *  15 08/17/01 Corrected supply_mbuf for PVTRAM mode.                 KeiT
 * 115 08/17/01 Edition 115 is PVTRAM mode and edition 15 is normal mode. KeiT
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/

/* Driver edition number information */
#ifdef SYSEDIT

#ifdef PVTRAM
_asm("_sysedit: equ 115");
#else
_asm("_sysedit: equ 15");
#endif

_asm("_sysattr: equ 0xA000");

#endif

/* Descriptor edition number information */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 7");
_asm("_sysattr: equ 0x8000");

#endif
