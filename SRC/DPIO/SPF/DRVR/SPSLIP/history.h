/****************************************************************************
 *           Copyright 1989-2000 by Microware Systems Corporation           *
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
 * 203 ??/??/?? Created from ISP SLIP.                                 sr
 * 204 12/23/96 Changed thread_exit and kill_thread.  Added call       sr
 *              to terminate_thread. (spslip.c)
 *              In dr_downdata wait on xmit event to be set to 0
 *              to avoid race condition with spsliptx. (entry.c)
 *              Modified and added prototypes. (proto.h)
 * 205 12/26/96 In dr_iniz, allocate input and output buffers,based    sr
 *              on the sizes specified in desc.  In dr_term
 *              deallocate accordingly. (entry.c)
 *              Now the receive and xmit buffer sizes are
 *              configurable from the desc. (spslip.c)
 *              Added more fields to lustat, for input and output
 *              buffer pointers. (defs.h)
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 * 206 11/05/97 Changed to use MM/DD/YY in history                     tmd
 *              Added BSD statistics                                   tmd
 *              Removed INET directory references                      tmd
 *              Fixed to set pathdesc and deventry ptr on dr_updata    tmd
 *              Reserve some space in mbuf to save pullup in SPIP      tmd
 * 207 11/18/97 Use COMPRESS_FLAG to set lu_compress field. (defs.h)   sr
 * 208 12/11/97 Added check for PD_SYSTATE to skip check_access        tmd
 * 209 02/04/98 Stole PPP's compression, removed compiler warnings     rds
 *              Replaced cslip.h with compress.h                       rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 * 211 06/10/97 Added STACK_ALLOC for SPARC                            tmd
 * 212 09/09/98 Verify packet is TCP before calling compress_tcp       rds
 *     09/09/98 Set each pathopts element to 0 (ARM compiler bug)      rds
 * 213 10/15/98 Added multicast support (entry.c)                      rds
 * 214 01/19/99 Set stk_int_flags in dr_getstat (entry.c)              rds
 * 215 02/08/00 Adding STACK_ALLOC for MIPS.						   dwj
 * 216 08/16/00 Updated because of memory leak in thread.c             zw
 * 217 12/12/00 Compiled with new thread library to work with threaded sr    
 *              kernel for OS9000.     
 * 218 08/02/01	Set the event value of TERMINATE smaller       	       pengf 
 *              Check the death flag using zero                                     
 * 219 10/02/01 CF9782: Fixed irq_* prototypes							rry
 ****************************************************************************/

/*
**	Edition/Revision Numbers
*/

/* driver edition number */
#ifdef SYSEDIT

_asm("_sysedit: equ 219");
_asm("_sysattr: equ 0xA000");

#endif /* SYSEDIT */


/* descriptor edition number */
#ifdef DESC_SYSEDIT

_asm("_sysedit: equ 207");
_asm("_sysattr: equ 0x8000");

#endif /* DESC_SYSEDIT */
