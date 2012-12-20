/*
 * sbteac - scsi device driver for TEAC MT-2ST/N50 tape drive
 *
 * Copyright 1989, 1990, 1991, 1993, 1994, 1998 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * main - edition history and declaration of static storage 
 *
 * edition history
 *
 * ed#   date     changes                                        by
 * --- -------- --------------------------------------           ---
 *  1  89/09/12 This is the first crack, taken from sbviper.     Rwb
 *  2  89/10/10 Added code for negative skip using rewind &      srw
 *              skip forward, updated sleep strategy when
 *              drive is busy doing off-line tape motion,
 *              replaced E_HDWARE with E_HARDWARE (see errno.h).
 *  3  89/10/31 corrected request sense data structure           srw
 *  4  89/11/07 corrected init.c:  fixed uninitialized "ptr to   wwb
 *              ptr" problem that corrupted system globals.
 *              Made init exit if data_module can't be created   wwb
 *              or linked.
 *              Made terminate routine not call for SC_REMOVAL   wwb
 *              (fatal system crash if running on behalf of an
 *              error exit from Init).
 *              Made terminate check for null ptr when unlinking wwb
 *              status data module.
 *	5  90/3/27  Read and Write now refuse non-integral block ops Rwb
 *  6  90/04/06 made all static storage structures be long-word  wwb
 *              aligned. This is an issue for dma-driven low-
 *              level modules:  some systems can not handle
 *              non-aligned transfers, so the high-level module
 *              needs to ensure that its internal structures
 *              are safely aligned (JIC: just in case!).
 *              Removed enquiry structure storage (not used).    wwb
 *  7  90/04/12 Added direct command interface to stat.c         Rwb
 *  8  90/04/20 changed "can't get sense info" error from E_IBA  wwb
 *              to E_HARDWARE.
 *  9  90/06/05 added timeout to test unit ready to decrease the  
 *              the errors while tape does it's reseek.          Rwb
 * 10  90/06/15 added CONTR_SUPPORT qualification to pd_scsiopt. wwb
 *              ---- OS-9/68K V2.4 released ----
 * 11  91/03/04 Repaired uninited variables in direct_command()  Rwb
 * 12  91/03/05 Made initdrv() more rational in that test unit   Rwb
 *              ready is not forced at each call, looped 
 *              command now sleep rather than pound on the device.
 * 13  91/05/06 Read was not counting the filemark on failed     Rwb
 *              reads w/residual, which causes the position to be
 *              incorrectly restored durring buffered operations
 *              ( see read() ).
 * 14  91/06/13 Changed module access permissions.               Rwb
 * 15  91/06/17 Moved definition to conform with conventions     Rwb
 *              ---- OS-9/68K V2.4.3 released ----
 * 16  93/05/25 Modified for UCC compatibility mode. Collected   ats
 *              driver statics into a single record.
 *              ---- OS-9/68K V3.0 released ----
 * 17  94/07/18 Removed CONTR_SUPPORT checks (ed10) - checks are wwb
 *              now normally performed by low-level.
 * 18  94/07/29 Removed SCSI id validation against MAX_SCSI -    ats
 *              check is now normally performed by low-level
 *              driver.
 *              ---- OS-9/68K V3.0.1 released ----
 * 19  98/07/08 reworked static storage naming for UCC2.X
 *              ---- OS-9/68K V3.1 released ----
 * 20  99/11/20 Bump Edition number to identify version made    mgh
 *              with defs again compatible with earlier versions.
 */

#ifdef _UCC
_asm("_sysedit: equ 20");
#else
@_sysedit: equ 20
#endif

#ifdef _UCC
#include	<stddef.h>
#define		NO_STATICS_SHORTCUTS
#endif

/* known problems and creature-features:
 *
 * 1. there is a major problem with any counters kept.
 * These counters will be erased between opens if the device is
 * not explicitly iniz'd [which we don't want to do because we want
 * the terminate routine to run so that the 'sbf' process can be
 * terminated].  These counters need to be kept in a "status data
 * module".
 *
 * 2. lack of easy access to the path descriptor options (during read/
 * write/getstat/putstat) means that the only way to pick up the scsi
 * controller id and lun is during the 'init' routine.  This implies
 * that only one logical drive can be supported per driver incarnation.
 */
 
 
#define		driver_static

#include	"sbteac.h"

#ifdef _UCC
_asm("sys_stat: equ %0", offsetof(teacstatics, ds_sys_stat));
_asm("stat: equ %0", offsetof(teacstatics, ds_stat));
_asm("pathdesc: equ %0", offsetof(teacstatics, ds_pathdesc));
_asm("procdesc: equ %0", offsetof(teacstatics, ds_procdesc));
#endif

