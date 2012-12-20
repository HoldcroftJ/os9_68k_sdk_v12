/*
 * vme327 low level driver
 *
 * Copyright 1990, 1991, 1992, 1993, 1994 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * main - edition history
 *
 * edition history
 *
 * ed#   date    by   changes
 * --- --------  ---  -----------------------------------------
 *  1  90/01/26  Rwb  Genesis of the SCSI only code
 *  2  90/03/28  Rwb  Repaired problems with translated VME addresses
 *  3  90/04/06  Rwb  Bus width and lword xfer controlled by pd_tfm.
 *                    pd_tfm = 1 => 16 bit data only.
 *  4  90/04/25  Rwb  moved buffered pipe initialization into init()
 *  5  90/09/13  Rwb  Added status check against board error codes.
 *                    errstat()
 *                    ---- OS-9/68K V2.4 (VME141) released ----
 *  6  91/01/23  Rwb  Changed docmd() to support the mVme165 card.
 *  7  91/01/28  Rwb  Changed set_script() to return true on test unit
 *                    ready.  This will allow boot drives to become
 *                    synchronous.
 *  8  91/02/11  Rwb  repaired problems with extended addressing in exec()
 *  9  91/03/05  Rwb  transfers now fixed at 16 bit reguardless of the 
 *                    hosts capabilities.  This is done because of devices
 *                    that insist on unusual disconnect boundries and lack
 *                    of a good means of itdentifying same.
 *                    ---- OS-9/68K V2.4 68040 released ----
 * 10  91/06/13  Rwb  Changed access permissions for the data module
 * 11  91/06/17  Rwb  Moved a definition to conform with earlier conventions
 * 12  91/07/24  Rwb  Changes to get_scipt() and exec(), used to tag only the
 *                    load command for synchronous operation.  This needed to
 *                    be changed to tag any given non-data transfer command.
 *                    ---- OS-9/68K V2.4.3 released ----
 * 13  91/08/19  Rwb  Change to exec() so that address modifier is valid
 *                    in the case of 0 length data transfers.
 * 14  92/04/03  wwb  repaired cacheflush() to ensure valid a6.
 * 15  93/02/10  wwb  reworked irq masking for MSP support.
 * 16  93/05/26  ats  Modified for UCC compatibility mode.
 *                    ---- OS-9/68K V3.0 released ----
 * 17  94/04/06  ats  Fixed synchronous mode operation with 2.7 firmware.
 *                    The card negotiates synchronous operation when SYNC 
 *                    is set on the inquiry and remembers the result until
 *                    next SCSI bus reset.
 * 18  94/04/14  ats  ANSIfied for optimization.
 *                    Added serialization for future superscalars.
 * 19  94/07/20  ats  Moved SCSI ID sanity check into exec() as preparation
 *                    for large ID support in the high-level drivers on
 *                    "wide" busses.
 * 20  94/08/02  wwb  Added MAXSCSI label for scsi id checks.
 *                    ---- OS-9/68K V3.0.1 released ----
 *                    ---- OS-9/68K V3.1 released ----
 */

#ifdef _UCC
_asm("_sysedit: equ 20");
#else 
@_sysedit: equ 20 Current edition number.
#endif

#include <scsi327.h>
