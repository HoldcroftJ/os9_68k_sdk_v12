/*
 * main - edition history and declaration of static storage 
 *
 * Copyright 1992, 1993, 1994, 1995, 1996 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * edition history
 *
 * ed#   date    by   changes
 * --- --------  ---  -----------------------------------------
 *  1  90/01/08  srw  created
 *  2  90/02/15  Rwb  began validation of driver and testing of
 *                      various formats
 *  3  90/04/06  wwb  made all static storage structures be long-word
 *                    aligned. This is an issue for dma-driven low-
 *                    level modules:  some systems can not handle
 *                    non-aligned transfers, so the high-level module
 *                    needs to ensure that its internal structures
 *                    are safely aligned (JIC: just in case!).
 *                    Removed enquiry structure storage (not used).
 *  4  90/04/12  Rwb  Added direct command interface to stat.c
 *  5  90/04/19  wwb  fixed test of pd_drv against v_ndrv in initdrv.
 *  6  90/05/03  Rwb  added code and defs to create a booter.
 *  7  90/06/05  Rwb  Repaired problem in term which resulted in a bad
 *                       pointer to the drive table.
 *     90/06/15  Rwb  Changed comments for varsect in stat().  No code
 *                    changes made.
 *  8  90/06/15  wwb  added CONTR_SUPPORT qualification to pd_scsiopts.
 *  9  90/06/18  Rwb  modified misc.c handlerr() to allow debugging of
 *                    several previously undefined error codes.  These
 *                    code will still return <main sense>:<extended sense>
 * 10  90/07/13  Rwb  Requires new descriptors.......
 * 11  90/07/16  Rwb  modified to detect between HS and JS drives and
 *                    adapt accordingly
 * 12  90/07/19  Rwb  Now can format and read single sided disks
 * 13  90/07/23  Rwb  Now uses extended read and write commands as opposed
 *                    to read and write.  (thanks warren!)
 * 14  90/07/28  Rwb  Finalized driver for release by repairing problems
 *                    in format.
 * 15  90/08/28  Rwb  Repaired the problems noted with E$DIDC occuring on
 *                    inized devices.  Now checks and uses interleave
 *                    as defined on path rather than default case (1).
 *                    ---- OS-9/68K V2.4 Released ----- 
 * 16  91/03/01  Rwb  changed to not send block descriptors and use the
 *                    vendor specific media codes.
 * 17  91/04/08  Rwb  validate_descriptor now insures that sector offset
 *                    is set to 1 as the drive will only work with
 *                    a sector offset of 1.
 *                    ---- OS-9/68K V2.4.3 Released ----- 
 * 18  92/04/06  wwb  more updates to attempt to cover the continually
 *                    shifting target (aka upgrades [sic]).
 *               wwb  repaired NOTRDY case in handlerr(): not-ready
 *                    when no index (remove controller-error).
 *               wwb  repaired calls to scsistat(): should explicitly
 *                    pass "error" (you can be lucky, tho').
 * 19  93/03/26  ats  changed handling of recursion flag to properly
 *                    handle disk id change.
 * 20  93/04/14  ats  fixed problems introduced by edition #18 in
 *                    drive/controller/firmware specific mode select
 *                    generation.
 * 21  93/04/14  ats  Modified for Ultra C.
 *                    ---- OS-9/68K V3.0 Released ----- 
 * 22  94/06/20  ats  Corrected block counts for write verify (it's not
 *                    slow anymore).
 * 23  94/07/18  wwb  Removed CONTR_SUPPORT checks (ed8) - checks are now
 *                    normally performed by low-level.
 * 24  94/07/29  ats  Removed SCSI id validation against MAX_SCSI - check
 *                    is now normally performed by low-level driver.
 *                    ---- OS-9/68K V3.0.1 Released ----- 
 * 25  94/08/19  ats  Added FD55 support.
 * 26  95/01/05  GbG  Added support for FC-5 type drives
 *                    ---- OS-9/68K V3.0.2 Release ----
 * 27  95/07/11  GbG  ANSI Version.
 * 28  95/08/01  GbG  Allow for sector numbering to begin at 0 if requested
 * 29  96/03/07  GbG  RBTEAC now uses tables.
 *                    ---- OS-9/68K V3.0.3 Release ----
 * 30  96/07/29  GbG  Added case for MAIN_ABORT:SENSE_SCSIPAR in misc.c
 *                    ---- OS-9/68K V3.1 Release ----
 * 31  99/11/20  mgh  Bumped Edition number so 68k version made with
 *                    imcompatible defs for V3.1 could be identified.
 *                    NOTE: There may be 2 edition 30s one that works and
 *                      one that doesn't.   
 */

/* Edition 18 comments on the notes below:
 * These notes may no longer be valid - TEAC has continually updated
 * the model/version numbers available since the product was introduced,
 * and probably will continue to do so in the future.  Coupled with this
 * fact is the critical difference between drive firmware released in
 * the U.S.A. and firmware released everywhere else.  Anyone attempting
 * to track these moving targets should refer to TEAC documentation
 * when comments in this code seem at variance with the actual model(s)
 * being tested.
 */

/********* Important programmers note. *********************************
*		The TEAC FC-1 controller needs to manipulate control pins in
*  order to change density on the various drives.  In the case of the
*  1M unformated (300rpm/250kb/s) there is no difference between the
*  the pin 2 definition.   In the case of the JS drive the pin 2 
*  definition is different from the HS drive with respect to the 1.6M
*  unformated (300rpm/500kb/s) disks.  This driver performs an inquiry
*  command to determine the drive type that is present and will configure
*  for the appropriate type.  (see misc.c  get_drive_kind() ).  This
*  also leaves open the case of some other drive from TEAC modeled on the
*  FC-1 controller that is not tested or offically supported.  The user
*  might patch the strings defined...........
***********************************************************************/

/*  
 *  NOTE:  This driver will determine if it should retry a command on
 *    E$DIDC by a flag that is passed to execute() and execnoxfer().
 *    There is probably a better way to do this.  The problem is masked
 *    rather than delt with.  There are some situations where disk ID
 *    change is a valid error.  The problem is how to detect when this
 *    information would be important.......
 */
 
/****************************
 *     There are two versions of the TEAC FC-1 controller that are
 *  distributed by TEAC.  One version is USA/Motorola only, the other
 *  is For world-wide distribution (outside the USA) which is refered 
 *  to as the Eurpoean/Japanese version.  The driver should detect the
 *  difference and adapt on the fly.  The versions are as follows:
 *
 *  FC-1-00 Japanese Standard
 *  FC-1-01 USA Standard
 *
 *  FC235HF-301  USA   FC235JHF-401
 *
 *   if the postfix numbers do not include x01 then  the drive is Japanese
 *      standard
 *****************************/

#ifdef _UCC
_asm("_sysedit: equ 31");
#else
@_sysedit: equ 31
#endif

#ifdef _UCC
#include	<stddef.h>
#define		NO_STATICS_SHORTCUTS
#endif

#define     driver_static
 
#include    "rbteac.h"
#include	"rbproto.h"

#ifdef _UCC
_asm("sys_stat: equ %0", offsetof(teacstatics, sys_stat));
_asm("stat: equ %0", offsetof(teacstatics, stat));
_asm("pathdesc: equ %0", offsetof(teacstatics, pathdesc));
_asm("procdesc: equ %0", offsetof(teacstatics, procdesc));
#endif

