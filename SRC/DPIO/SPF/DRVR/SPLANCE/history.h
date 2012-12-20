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
 *   1 06/17/96 Creation                                               bw
 *   2 06/20/96 Added 4 bytes header to store mbuf address
 *   3 08/??/96 made descriptor to set up txsize
 *   4 09/03/96 pop and push bug fix
 *   5 09/24/96 Update get_eaddr (init.c) to get eaddr from BBRAM      pjw
 *   6 01/22/97 Pass our dev_entry in 4 bytes just before the data  of sr
 *              of the upgoing packet. (isr.c)
 *   7 01/27/97 While doing get_mbuf, get 4 extra bytes, that way our  sr
 *              data space is not reduced because of the dev_list ptr.
 *              (isr.c)
 *   8 03/05/97 In free_rmds, account for the Dev_list ptrs to get to  sr
 *              the start of the mbuf. (term.c)
 *   9 02/17/97 isr.c: Updated to add, not sub offset for dev_entry    pjw
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  10 10/23/97 Added/Uncommented BSD statistics keeping               tmd
 *              Fixed TMD3 flags: made to check high byte              tmd
 *              Set broadcast/multicast bit in mbuf                    tmd
 *              Added ITE_GETADDR & SPF_GS_SYMBOLS                     tmd
 *  11 01/08/98 Removed causes of compiler warnings except pointer
 *               conversion in init.c                                  jk
 *  12 01/21/98 Removed compiler and linker warnings                   jk
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  13 06/25/99 Set stk_int_flags in SPF_GS_UPDATE (entry.c)           rds
 *  14 10/02/01 CF9782: Fixed irq_* prototypes						   rry
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/

/* driver edition number */
#ifdef SYSEDIT

_asm("_sysedit: equ 14");
_asm("_sysattr: equ 0xA000");

#endif

/* descriptor edition number */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 12");
_asm("_sysattr: equ 0x8000");

#endif

