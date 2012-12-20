/*****************************************************************************
 *  SPF Device Driver history.                                               *
 *****************************************************************************
 *            Copyright 1989-1996 by Microware Systems Corporation           *
 *                            All Rights Reserved                            *
 *                          Reproduced Under License                         *
 *                                                                           *
 *  This software is confidential property of Microware Systems Corporation, *
 *  and is provided under license for internal development purposes only.    *
 *  Reproduction, publication, distribution, or creation of derivative works *
 *  in any form to any party other than the licensee is strictly prohibited, *
 *  unless expressly authorized in writing by Microware Systems Corporation. *
 *****************************************************************************
 *  Edition History:                                                         *
 *   #   Date       Comments                                           By    *
 *  --- --------    ----------------------------------------------- -------- *
 *  01 10/07/96 Creation                                                     *
 *  02 02/02/97 In dr_getstat, call get_eaddr to get hardware address.  sr   *
 *              (entry.c)                                                    *
 *              Save space in our mbuf for passing dev_list ptr up.          *
 *              (init.c)                                                     *
 *  03 02/05/97 Free up all the recv desc mbufs. (term.c)               sr   *
 *              In dr_term, disable pic. (entry.c)                           *
 *  04 02/06/97 In dr_downdata, if qe_xmit does not return SUCCESS      sr   *
 *              enqueue mbuf for retry. (entry.c)                            *
 *              In qe_xmit, don't free mbuf if we do not have desc.          *
 *              In xmit_irq, before exiting check to see if there is         *
 *              enqueued data that can be sent out. (isr.c)                  *
 *  05 02/17/97 isr.c: Updated to not sub, but add offset for dev_entry pjw  *
 *     02/19/97 <***** SPF LAN Comm Pack v3.0 Release*********>              *
 *  06 04/02/97 Modified BD_BASE to avoid conflict with Microcode area. sr   *
 *              (qedvr.h)                                                    *
 *  07 05/09.97 isr.c: casted dev_entry to (u_int32)                    pjw  *
 *  08 05/19/97 Removed C_INVDATA define from here. It is in cache.h    sr   *
 *              Fixed Microcode confict issue.  Change has to be to          *
 *              BD_OFFSET and not BD_BASE. (qedvr.h)                         *
 *     05/19/97 <***** SPF LAN Comm Pack v3.1 Release*********>              *
 *  09 08/11/97 Added SCC channel dependent code                        taka *
 *  10 10/13/97 Mbuf leak fix: set T_I bit at qe_xmit in isr.c          taka *
 *  11 10/15/97 Added 68360 support                                     taka *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>              *
 *  12 12/02/97 Added code to support netstat                           jk   *
 *  13 12/05/97 reset chip before do anything.  (init.c)                taka *
 *  14 01/21/98 Removed compiler and linker warnings                    jk   *
 *  15 03/05/98 Invalidate mbuf data area, even if CACHE_INHIBITED_BUFS jk/sr*
 *              is defined, since this is not in the inhibited area.         *
 *              (init.c)                                                     *
 *              In dr_downdata, in case of "scoop", get a new mbuf,          *
 *              set the offset, then call m_move. Other clean up. (entry.c)  *
 *              Incorporated Uday's fix for mbuf leaks.                      *
 *              In isr.c clear the scce event register, after processing     *
 *              the interrupt.  Call xmit_irq, if transmit error occurs.     *
 *              Call recv_irq in case of incomplete frames. (RXB status)     *
 *              Other code cleanup. (isr.c)                                  *
 *              Added more prototypes, conditionalized for DEBUG. (defs.h)   *
 *              if 0ed SCCE defines (T_TXE etc), use the real ones           *
 *              defined in enet360.h. (qedvr.h)                              *
 *     04/02/98 <***** SPF LAN Comm Pack v3.2 Release*********>              *
 *  16 02/12/99 Set stk_int_flags in dr_getstat (entry.c)               rds  *
 *  17 11/15/99 CF6721: fix transmit mbuf leak, improve performance          *
 *              in xmit_irq().                                          gjb  *
 *  18 12/28/99 Added multicast support (entry.c)                       rds  *
 *  19 02/10/00 Added transmit queue limits (entry.c, isr.c, defs.h)    rds  *
 *              Enable ETHERNET_BSY, fix write_long/write_word (init.c) rds  *
 *              Added NO_CONFIG_PINS, NO_CONFIG_ETRX, NO_CONFIG_TENA    rds  *
 *              Flush mbufs in dr_downdata instead of qe_xmit           rds  *
 *  20 02/11/00 Removed MC68360 conditional, 3rd arg to get_eaddr()     kimk *
 *              the port will get enetaddr in a more flexible manner.        *
 *              Reworked initialization parameters to match modern setup.    *
 *              Include <systype.h> in qedvr.h  Added CHECK_LOCK option to   *
 *              allow SCC shared iniz locks.                                 *
 *  21 05/11/00 Change logical or of bits in transmit irq to bit or     mgh  *
 *                 CF8303                                                    *
 *  22 08/02/00 CF8554 Fix: Multicast problem                           mdu  *
 *  23 08/17/00 CF2733: Add the user access check before doing memory        *
 *              copy in GS_HWSTAT function in entry.c. Also, remove the      *
 *              unused DEV_PB structure in the same file                mdu  *
 *  24 08/29/00 Remove junk code that didn't compile in Ed 23           mgh  *
 *  25 12/07/00 CF9967 - Added KeiT's changes in entry.c and isr.c      mdu  *
 *  26 02/19/01 Added C_ADDR to cache_cctl() (isr.c, init.c, entry.c)	sbh  *
 *  27 08/15/01 CF7420: add multicast support                           taka *
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/

/* driver edition number */
#ifdef SYSEDIT

_asm("_sysedit: equ 27");
_asm("_sysattr: equ 0xA000");

#endif

/* descriptor edition number */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 14");
_asm("_sysattr: equ 0x8000");

#endif

