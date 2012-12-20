/*
 * scsi53c94 - low-level scsi subroutine module for NCR 53C94
 *
 * Copyright 1992, 1993, 1994 by Microware Systems (UK) Limited
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
 *  1  92/01/09  AM   greatly hacked from scsi33c93
 *  2  93/05/37  ats  Modified for UCC compatibility mode.
 *						---- OS-9/68K V3.0 Release ----
 *  3  94/07/21  ats  Moved SCSI ID sanity check into exec() as preparation
 *                    for large ID support in the high-level drivers on
 *                    "wide" busses.
 *  4  94/08/02  wwb  Added MAXSCSI definition for scsi id checks.
 *						---- OS-9/68K V3.0.1 Release ----
 *						---- OS-9/68K V3.1 Release ----
 */

#ifdef _UCC
_asm("_sysedit: equ 4");
#else
@_sysedit: equ 4 current edition number
#endif

