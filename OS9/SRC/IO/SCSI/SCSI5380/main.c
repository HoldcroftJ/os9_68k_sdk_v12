/*
 * scsi5380: low-level scsi subroutine module for NCR5380
 */
 
/*
 * Copyright 1988, 1989, 1990, 1991, 1993, 1994 by
 * Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee solely
 * for documentation and educational purposes. Reproduction, publication,
 * or distribution in any form to any party other than the licensee is
 * strictly prohibited.
 */
 
/*******
* edition history
*
* ed# date		changes													who
* --- --------	------------------------------------------------------	---
* 1   88/12/06	hacked into existence from various sources				jal
* 2   89/01/17  adapted to work with both JNMS player and VME117		jal
* 3   89/09/14  adapted to work with Philips ISBX board & fixed bugs	srw
* 4   89/10/31  changed to 2 events for dma useage & fixed dma68450.h   srw
* 5   89/11/06  added counter in non-DMA "data_io" write loop to keep	jal
*				from running off the end of the buffer
*				>>>>>> IMPORTANT NOTICE: <<<<<<
*					This change is necessary because the NCR5380 expects
*					one byte more than the transfer size specified on a
*					write before it will trigger a phase change.
*               --- OS-9/68K V2.4 released ----
* 6   90/08/20  1) Fixed JNMS DMA vector problem						jal
*     90/08/29	2) Added new arbitration/select algorithm in "exec" 	jal
*     90/08/31	3) Added SCSI Self ID selection via data module			jal
* 7   91/05/28  1) SCSI Chip address now striped of ID bits for         Rwb
*                     rbvccs support.
*     91/05/29  2) Optimage changed default settings for the SCSI Card  Rwb
*                  and never bothered telling us, as a result, the
*                  scsi5380.h file needed to change.
* 40  91/05/30  >>> Editions 7-40 never released, Revision is bumped up
*                   to prevent renagade versions from being confused with
*                   supported versions.
*               Edition 40 changes:
*                 Major for the JNMS Player:  
*                   DMA Interrupt is no longer used, SCSI Reset is captured
*                   and returned to the drivers as a Disk ID change (E_DIDC).
*               At this edition, the exec.c file has been changed to include
*               a device specific io_xxxx.c file.  This file will allow the 
*               isolation of implimentation differences.
* 41 91/06/07  Default ID for the JNMS now set to ID 5.                 Rwb
* 42 91/06/13  A timing window existed in sendcmd() where-in the phase  Rwb
*              change could occur between the time that the condition
*              that enabled the interrupt and the chip was conditioned 
*              to report the change.  This affects all versions of the
*              driver except polled mode.
* 43 91/06/13  Change module access permissions.
* 44 91/06/14  Changed DMA_MAX_TIME to 63.5 sec for JNMS player         Rwb
*               due to EXABYTE operational characteristics.
*              <<<<  Optimage Media Mogul Release >>>>
* 45 91/06/17  Moved definition to conform with conventions.            Rwb
*               --- OS-9/68K V2.4.3 released ----
* 46 93/02/10  reworked v_mask_level and irq_mask() for MSP support.	wwb
* 47 93/05/26  Modified for UCC compatibility mode.                     ats
*               --- OS-9/68K V3.0 released ----
* 48 94/07/21  Moved SCSI ID sanity check into exec() as preparation    ats
*              for large ID support in the high-level drivers on
*              "wide" busses.
* 49 94/08/02  Added MAXSCSI definition for scsi id checks.				wwb
*				--- OS-9/68K V3.0.1 released ----
*				--- OS-9/68K V3.1 released ----
*/

#ifdef _UCC
_asm("_sysedit: equ 49");
#else
@_sysedit: equ 49 current edition number
#endif

/*
 *  NOTES CONCERNING THIS DRIVER
 *
 *   While it is technically possible to run any of the four devices
 *   in a polled mode, the JNMS case should always be run in the DMA
 *   mode.  This is due to the code that is in place to care-take the
 *   operations through SCSI BUS Resets.
 *
 *   JNMS Version:
 *      SCSI Bus Reset (SBR) is a completely asynchrounous and unpredictable
 *   event in a multi-initiator environment.  The following compromises
 *   have been made in order to attempt to support multi-initiator operation.
 *
 *      1)  The IRQ event waits on a non-zero value rather than a discrete
 *          value of 1.  It is possible to have a SBR occur immediatly 
 *          after a phase change or EOP interrupt but prior to the driver
 *          being awakend.  The event value would then be > 1 when the driver
 *          does awaken.
 *      2)  A flag is set at entry to the driver indicating that a process
 *          is present to receive notice of a SBR.  This prevents unnessesary
 *          event operations.  Along these lines however, the IRQ event
 *          is cleared at entry to the driver [exec()].  This is done due
 *          to the window between last access to the chip and the clearing
 *          of the flag indicating a process is present in the driver.
 *          If the last command executed to completion, it will thus return
 *          to the caller without error even though a SBR has occured .
 *
 *      3)  While the JNMS player has a DMA chip, and we use it, we do no
 *          use the DMA interrupt.  Instead there is a cyclic alarm 
 *          activated by init and deactivated by term.
 */
