/*
 * sbviper - scsi device driver for Archive 2150S/2060S Viper tape drive
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
 * ed#   date     changes                                        by
 * --- -------- --------------------------------------           ---
 *  1  88/10/19  today we write a tape driver...                 wwb
 *  2  88/10/21  fixed terminate when init has problems.         wwb
 *  3  88/10/23  added reinit tape when device resets.           wwb
 *  4  88/11/03  added scsistat function.                        wwb
 *  5  88/11/03  updated for new high-low interface.             wwb
 *               (added 'atn, llcmd')
 *  6  88/11/04  removed file-mark/block counters.               wwb
 *  7  88/11/07  added residual length valid detection, added    wwb
 *               eof handling.
 *  8  88/11/29  renamed from sb2150 to sbviper for release.     wwb
 *  9  88/11/30  made write not truncate partial blocks.         wwb
 * 10  88/11/30  added delay after starting.                     wwb
 * 11  88/12/03  added early eom warning handling.               wwb
 * 12  89/03/20  recompiled for new sbglue (ss_skip fix).        wwb
 * 13  89/05/12  added optflags for pd_scsiopt.                  wwb
 * 14  89/06/26  added conditionals to allow SBGIGA version.     wwb
 * 15  89/06/27  fixed typo in stat.c for ed.14.                 wwb
 * 16  89/08/10  added sleeping when DEVBUSY.                    wwb
 * 17  89/10/13  fixed early eom warning for multi-volume fsave  srw
 * 18  90/03/27  Now refuses partial block operations            Rwb
 * 19  90/04/06  made all static storage structures be long-word wwb
 *               aligned. This is an issue for dma-driven low-
 *               level modules:  some systems can not handle
 *               non-aligned transfers, so the high-level module
 *               needs to ensure that its internal structures
 *               are safely aligned (JIC: just in case!).
 *               Removed enquiry structure storage (not used).   wwb
 * 20  90/04/10  Added direct command interface                  Rwb
 * 21  90/04/20  changed "can't get sense info" error from E_IBA wwb
 *               to E_HARDWARE.
 * 22  90/04/07  Now attempts to set media to QIC24 if mode      Rwb
 *               select fails (Archive 2060S support).
 * 23  90/06/15  added CONTR_SUPPORT qualification to pd_scsiopt.wwb
 *               ---- OS-9/68K V2.4 released ----
 * 24  91/01/07  Now attempts to set media to QIC24 if mode      Rwb
 *               select fails (Archive 2060S support).  Code from
 *               edition 22 disappeared prior to the release.
 * 25  91/02/05  execute() modified to stop EXABYTE from 
 *               performing disconnects durring read and write.  Rwb
 * 26  91/03/04  Repaired uninited variables in direct_command() Rwb
 *               removed ed.25 changes.
 *               ---- OS-9/68K V2.4.3 released ----
 * 27  92/10/06  fixed missing close comment in misc.c (RESTORE  wwb
 *                check on SENSE_RECOVER) - thanks kkk.
 * 28  92/12/15  Patch from edition #26 given to cust. support   ats
 *				 to allow 147/167 devpaks to boot from 2525S
 *               drives with version 8.0 firmware. The patch
 *               issues SC_LOAD as a non-immediate command.
 * 29  92/12/15  Includes functionality of patch edition #28 in  ats
 *               source against edition #27.
 * 30  93/05/19  Modified for Ultra C compatibility mode.        ats
 *				 ---- OS-9/68K V3.0 released ----
 * 31  94/07/18  Removed CONTR_SUPPORT checks (ed24) - checks    wwb
 *               are now normally performed by low-level.
 * 32  94/07/29  Removed SCSI id validation against MAX_SCSI -   ats
 *               check is now normally performed by low-level
 *               driver.
 *				 ---- OS-9/68K V3.0.1 released ----
 * 33  96/03/08  SONY SDT-5000 fixed. ASC:4-ASCQ:1 returns busy. GbG
 * 34  98/07/08  reworked static storage naming for UCC2.X       wwb
 * 35  98/09/08  Added new copyright info.                       wwb
 *				 ---- OS-9/68K V3.1 released ----
 * 36  99/10/12  Fixed EOF clearing in SS_Feed in status.c       rkw
 * 37  99/11/20  Bump edition number to identify edition made    mgh
 *               with defs that restore compatiblity with versions
 *               before 3.1 release.
 *     $$                    <RELEASE_INFO>                     $$  |
 */

#ifdef _UCC
_asm("_sysedit: equ 37");
#else
@_sysedit: equ 37
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
 * write/getstat/putstat) means that the onlt way to pick up the scsi
 * controller id and lun is during the 'init' routine.  This implies
 * that only one logical drive can be supported per driver incarnation.
 */
 
#ifdef _UCC
#include	<stddef.h>
#define		NO_STATICS_SHORTCUTS
#endif
 
#define		driver_static

#include	"sbviper.h"

#ifdef _UCC
_asm("sys_stat: equ %0", offsetof(viperstatics, ds_sys_stat));
_asm("stat: equ %0", offsetof(viperstatics, ds_stat));
_asm("pathdesc: equ %0", offsetof(viperstatics, ds_pathdesc));
_asm("procdesc: equ %0", offsetof(viperstatics, ds_procdesc));
#endif

