/*
 * rbvccs - scsi device driver for Common Command Set disk drives
 *          Variable Logical and Physical Sector Size (1:1)
 *
 * main - edition history and declaration of static storage 
 */

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
 *
 * edition history
 *
 * ed#   date    by   changes
 * --- --------  ---  -----------------------------------------
 *  1  90/04/02  Rwb  created (from rbsccs, ed.10)
 *  2  90/04/06  wwb  made all static storage structures be long-word
 *                    aligned. This is an issue for dma-driven low-
 *                    level modules:  some systems can not handle
 *                    non-aligned transfers, so the high-level module
 *                    needs to ensure that its internal structures
 *                    are safely aligned (JIC: just in case!).
 *                    Removed enquiry structure storage (not used)
 *                    Removed cache block definition (not used)
 *  3  90/04/13  wwb  changed all files to include rbvccs.h instead
 *                    of rbsccs.h (no code changes).
 *  4  90/04/18  wwb  fixed static storage initialization in init.
 *                    fixed test of pd_drv in initdrv.
 *  5  90/04/20  wwb  changed "can't get sense info" error from E_IBA
 *                    to E_HARDWARE.
 *  6  90/05/14  wwb  fixed problem in initdrv when pd_ssize = 0.
 *  7  90/05/31  Rwb  Changed the test for sector size 1:1 with block size
 *                    to include test for disks formated 1:1 at 256 with
 *                    old format utilities.  Also does test unit ready to
 *                    determine if a load command is nessesary.
 *  8  90/06/05  Rwb  Repaired error in obtaining options pointer in
 *                    term() which resulted in bad drive table pointer.
 *  9  90/06/14  wwb  fixed problem with blkcnt in exec - wasn't being
 *                    initialized when pd->ssize was 0.
 * 10  90/06/15  wwb  added CONTR_SUPPORT qualification to pd_scsiopts.
 * 11  90/08/29  wwb  added testing of TYP_HREMOV (removable hard disk)
 *                    to qualify sector 0 buffering.
 * 12  90 09/04  Rwb  added support for E_DIDC on (removable) media change.
 *                    ---- OS-9/68K V2.4 released ----
 * 13  91/03/04  Rwb  repaired uninitialized variables in direct_command().
 * 14  91/04/04  Rwb  performs read capacity on any _ss_dsize()
 *                    status call.   Now performs sccsmode() only when
 *                    sector size is being changed.
 * 15  91/07/16  Rwb  Changed initdrv() to hold the synchronous bit off
 *                    until such time as the load command has been
 *                    done sucessfully.
 * 16  91/07/25  Rwb  Changed initdrv() to send a second test unit
 *                    prior to read capacity.  This will allow low level
 *                    drivers that transact the synchronous negotiations
 *                    only on non-data transfer commands.
 *                    ---- OS-9/68K V2.4.3 released ----
 * 17  91/08/01  wwb  removed confusion in comments between rbsccs and
 *                    rbvccs:  no code diffs except ed. #.
 * 18  93/05/03  ats  Modified for Ultra C.
 *                    ---- OS-9/68K V3.0 released ----
 * 19  94/06/20  ats  Corrected block counts on write verify (it's not 
 *                    slow anymore).
 * 20  94/07/18  wwb  Removed CONTR_SUPPORT checks (ed 10) - checks are now
 *                    normally performed by low-level.
 * 21  94/07/29  ats  Removed SCSI id validation against MAX_SCSI - check
 *                    is now normally performed by low-level driver.
 *                    ---- OS-9/68K V3.0.1 released ----
 * 22 94/12/12   GbG  Added case for MEDIUM|NOTREADY in misc.c
 *                    ---- OS-9/68K V3.0.2 Release ----
 * 23 96/07/29   GbG  Added case for MAIN_ABORT:SENSE_SCSIPAR in misc.c
 * 24 98/07/08   wwb  reworked static storage naming to allow UCC2.X
 * 25 98/08/27   dwj  Added check for partition descriptor in SS_WTrk.
 * 26 98/09/08   wwb  Added new copyright info.
 *                    ---- OS-9/68K V3.1 Release ----
 * 27 99/11/20   mgh  Bump edition number to identify 68k version remade
 *                    with defs compatible to releases prior to 3.1
 *    $$                    <RELEASE_INFO>                     $$  |
 */

#ifdef _UCC
_asm("_sysedit: equ 27");
#else
@_sysedit: equ 27
#endif

#ifdef _UCC
#include	<stddef.h>
#define		NO_STATICS_SHORTCUTS
#endif

#define		driver_static
 
#include	"rbvccs.h"

#ifdef _UCC
_asm("sys_stat: equ %0", offsetof(vccsstatics, ds_sys_stat));
_asm("stat: equ %0", offsetof(vccsstatics, ds_stat));
_asm("pathdesc: equ %0", offsetof(vccsstatics, ds_pathdesc));
_asm("procdesc: equ %0", offsetof(vccsstatics, ds_procdesc));
#endif

