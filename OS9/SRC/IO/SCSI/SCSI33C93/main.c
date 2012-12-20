/*
 * scsi33c93 - low-level scsi subroutine module for WD33C93 SBIC
 *
 * Copyright 1988, 1989, 1990, 1991, 1992, 1993, 1994, 1995 by
 * Microware Systems Corporation
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
 *  1  88/01/14  djl  created
 *  2  88/08/03  srw  reworked for wd33c93
 *  3  88/08/05  wwb  name changing exercise:  high-level drvr is "rb5400",
 *                    low-level is "scsi33c93".
 *  4  88/08/16  lac  revisions to syscalls.a to reduce stack usage.
 *  5  88/09/01  lac  Revised to use SCSI controller interrupts instead of DMA
 *                    interrupts to signal IRQ event.  Added a flag for the
 *                    high-level driver to tell the low-level driver which
 *                    direction to transfer data.
 *  6  88/09/02  lac  Revised static storage usage; now requires the high-level
 *                    driver to supply the RBF static storage ptr in a6.
 *  7  88/09/23  wwb  revised for high-level driver passing 12-byte packets.
 *  8  88/10/23  wwb  added STATS information collection.
 *  9  88/10/30  wwb  re-write starts
 * 10  88/11/01  wwb  re-write continues.
 * 11  88/11/03  wwb  updated for new high-low interface.
 *                    (added 'atn, llcmd')
 * 12  88/11/04  wwb  fixed enable reselection.
 * 13  88/11/09  wwb  reworked scsi phase handling.
 * 14  88/11/14  wwb  added VME147 dma irq processing.
 * 15  88/11/29  wwb  added irq masking while dma init.
 *     88/11/29  wwb  added no dma enable if no data transfer.
 * 16  88/11/30  wwb  fixed problem with dma enable when not using dma.
 * 17  88/11/30  wwb  once again into the bowels of dma.
 * 18  88/12/02  wwb  added (VME147 only) caching control in irqsrc.
 * 19  88/12/20  wwb  added VME620 code, added DMAIRQ conditional for
 *                    systems which have separate dma interrupts.
 * 20  89/03/06  jal  fixed and debugged VME620 version - fully operational
 * 21  89/05/15  wwb  added _f_trans usage, added scsi flags support,
 *                    added link/unlk protocol to #asm functions.
 * 22  89/05/25  wwb  fixed problem with valid scsi flag detection.
 * 23  89/07/05  wwb  slowed down irqsvc for 25MHz and faster mpus.
 *                    fixed E_HARDWARE to use errno.h label.
 * 24  89/08/04  wwb  fixed time-out (no device present) and other
 *                    hardware-type errors.
 * 25  89/11/22  Rwb  Began the major rewrite, turns the IQ into a
 *                    'pseudo-state machine'.  Requires overhaul of 
 *                    descriptors to allow maximum usage.  
 * 26  90/03/02  Rwb  Removed retries within exec() to decrease 
 *                    Bus Hogging.
 * 27  90/03/26  Rwb  Now strips the port address so that multiple
 *                    descriptors on rbsccs should work correctly.
 * 28  90/03/27  Rwb  _f_trans only called if block size other than 0.
 * 29  90/03/28  Rwb  all register file sets to zero done with a constant
 *                      ( see notes below )
 * 30  90/04/18  wwb  added link/unlk to flush_dcache.
 * 31  90/04/30  Rwb  repaired string termination problem in exec()
 * 32  90/05/15  Rwb  repaired spurious lockup problems.  Message phase
 *                    code rewritten.
 * 33  90/05/30  Rwb  <Skipped revision to insure that proper version
 *                     is distributed.>
 * 34  90/06/05  Rwb  Changes to term(), no longer performs unnessesary 
 *                    unlinks.  init(),  properly claims command block
 *                    on first entry.  exec() will now properly translate
 *                    addresses for the vme620.
 * 35  90/06/13  Rwb  ignores disconnect flag when synchronous flag set
 *                    and data-transfer being performed (suspect chip
 *                    problem).
 * 36  90/06/15  Rwb  Now allows disconnect for Synchronous drives.  The
 *                    resume point after disconnect is now within the
 *                    reselection operation ( see srq.c; group4() ).
 * 37  90/06/18  Rwb  Now will do an SAT resume after negotiations for
 *                    synchronous operation.  This removes the majority
 *                    of low level operations performed by the driver.  It
 *                    also allows the negotiations to be performed on 
 *                    any arbitrary SCSI command.
 * 38  90/06/18  Rwb  term() performs Bus Device Reset on any device
 *                    running synchronus at terminate time.
 * 39  90/06/22  Rwb  repaired problems with message reject.
 * 40  90/06/28  Rwb  made exec() maintain do a flush_dcache if DMA was
 *                    performed.  Old versions only used this code in
 *                    the case of VME147.  The vme620 could be used in
 *                    a system where this is nessesary, hence the change.
 * 41  90/07/10  Rwb  A serious problem was located and solved with the 
 *                    VME147 version of the program, wherein the PCC was
 *                    left with residual data durring intermediate disconnects.
 * 42  90/08/30  wwb  added testing to exec() to ensure that dma-counts
 *                    passed by high-level driver can be handled by this
 *                    module, improved cache flushing code.
 * 43  90/09/04  Rwb  Made this driver correctly cope with devices that
 *                    release BUSY without waiting for ACK to be negated
 *                    durring the message in phase.
 * 44  90/09/05  Rwb  Located and removed a chip timing violation in 
 *                    message_rejected() that caused occational lock-ups.
 *                    ---- OS-9/68K V2.4 released ----
 * 45  91/01/07  Rwb  Made changes to non-dma code to repair problems noted
 *                    by various OEM's in the field.  
 *                    Added code to handle SCSI reset interrupts on the 
 *                    mvme147 card.  (temporary, for inhouse testing)
 * 46  91/02/07  Rwb  Made trap to sysdbug in case of SCSI reset a conditional
 *                    feature. (vme147 SCSI reset interrupt phantom) The case
 *                    of SCSI reset (being real reset rather than a phantom)
 *                    needs to be resolved)
 * 47  91/03/19  Rwb  Added support for the wd33c93A version part.
 * 48  91/03/28  Rwb  Added easily identified words to the 'D' registers if
 *                    debug trap is left in place.
 * 49  91/04/23  Rwb  Repaired irq.c for NODMA case of mVme147.  No code
 *                    differences for DMA version drivers
 * 50  91/06/13  Rwb  Module access permissions changed.
 * 51  91/06/14  Rwb  Moved definition to conform with conventions.
 *                    ---- OS-9/68K V2.4.3 released ----
 * 52  91/10/29  Rwb  Modification to irqsup().  When a synchronous 
 *                    negotiation is returned requesting a faster period
 *                    than we advertise,  we set the chip to run at our
 *                    speed.  This should accomodate drives that violate
 *                    the SCSI standard (Maxtor Panther)
 * 53  91/11/15  Rwb  Fixed conditional mistake in exec().
 * 54  92/04/07  wwb  Removed hardwired value for dma vector in 147 case
 *                    (use the descriptor value + 1)
 * 55  93/02/10  wwb  Repaired flush_dcache for VME620 case.
 * 56  93/02/16  wwb  Reworked mask_irq for MSP support.
 * 57  93/05/03  ats  Modified for UCC -mode=compat. Should be backwards
 *                    compatible with old tools. Large chunks of assembly
 *                    code were extracted to .a files. 
 *                    ---- OS-9/68K V3.0 released ----
 * 58  94/07/20  ats  Moved SCSI ID sanity check into exec() as preparation
 *                    for large ID support in the high-level drivers on
 *                    "wide" busses.
 * 59  94/08/02  wwb  Added MAXSCSI definition for scsi id checks.
 * 60  95/02/10  rws  Made code ANSI conformant and added compiler optimizations.
 *                    ---- OS-9/68K V3.0.2 Release ----
 *                    ---- OS-9/68K V3.1 Release ----
 * 61 00/07/27   gdw  add address check for 147.
 * 62 01/01/31	 mgh  change v_port mask of bottom nibble to mask of bottom
 * 						byte for use with partition utility
 */

#ifdef _UCC
_asm("_sysedit: equ 62");
#else
@_sysedit: equ 62 current edition number
#endif

/* 
 *  Notes on the WD33c93A part and porting the driver.
 *
 *  The files wd33c93.h and scsi33c93.h describe the configuration of the
 *  driver/hardware platform.  It is essential that these files be understood
 *  when porting the driver to another platform.  This driver is specially
 *  configured for either the MVME147 (Motorola) or the VME620 (VME 
 *  specialists) SCSI cards.
 *
 *  This driver attempts to dynamically determine the version of the chip
 *  it is dealing with and then adapts to what it finds.
 *
 *  Of particular import to the "A" version of the part is the neccessity
 *  of describing the clock input to the chip.  This will select the
 *  correct divisor for the chip so that the SCSI bus cycles have the 
 *  correct timing.  See WD33c93.h and the Western Digital hardware manual
 *  for details.
 *
 *  The "A" version part seems to be much more efficient than the older
 *  part.  The "A" version part supresses the terminating interrupt
 *  (0x85) that previously followed on the heels of the command complete
 *  interrupt.  The SAT resume that previously did not work on reconnect
 *  now seems to function correctly with the added advantage that the
 *  target's IDENTIFY message has already been placed in a register, thus
 *  eliminating the manual message phase that was needed for the old part.
 */

/*
 *       Western Digital has noted that rapid polling of the auxilliary
 *   status register for interrupt pending followed by a rapid reading
 *   of the SCSI status register can cause a false status to be latched.
 *
 *    This possible problem has been addressed in message_in() where 
 *    there is a likelyhood of occurance.  There is sufficient time
 *    in irq() and others to prevent this occurance.  
 */

/*
 *    IMPORTANT PROGRAMMERS NOTE:
 *
 *    Most of the wd33c93's register file will auto-increment on read or
 *    write.  This is certainly true of the count registers.  This allows
 *    rapid setting of the values in these registers.  This creates a
 *    problem for 68000 processers as ---
 *
 *    when the 'C' compiler sees:     HW(reg_file) = 0;
 *    it generates:                   clr reg_file(aX)
 *
 *     while this is fine for 68010 and higher incarnations of the chip
 *     in the 68000 machine this instruction will:
 *
 *             move reg_file(aX) ->  to the bit bucket    (causes rf inc )
 *             move #0 -> reg_file(aX)  to clear the location (causes rf inc )
 *
 *    All sets (including defined constants that are or might be 0) are
 *    sent to the register file via a constant.  This will cause a single
 *    increment of the register file and the proper results are achieved.
 *    While this is a compatability issue, it must be adheared to.
 */

/* 
 * The following #include is only needed for correct compilation in the ucc strict 
 * ANSI mode, otherwise, without this #include, will get "empty declaration" and 
 * "; expected" error messages.
 */
#include <scsi33c93.h> 
