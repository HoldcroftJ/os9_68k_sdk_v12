/*
 *  main.c - Edition history and Port notes for the NCR SIOP
 *           driver.  (Note:  Motorola mVme162/167/177 SBC specific)
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1993-2001 by Microware Systems Corporation         |
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
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
|  1  91/07/29  Initial edits.										  Rwb   |
|  2  91/08/30  Initial testing of the driver for proof				  Rwb   |
|                    of concept.										    |
|  -  91/09/05  <<< Phase I freeze >>>								  Rwb   |
|  3  91/09/10  Inital work on disconnect/reselect					  Rwb   |
|  4  91/09/11  First crack at ID semaphore (interleaved access)      Rwb   |
|  5  91/09/12  <<< Phase II freeze >>>								  Rwb   |
|  6  91/09/18  Added Select timeout code and faced reality....		  Rwb   |
|  7  91/09/19  Revised script and flow to properly handle reality	  Rwb   |
|  8  91/09/20  Added the first attempt at extended messages.         Rwb   |
|  9  91/09/24  irq() now services interrupts until none pending.     Rwb   |
| 10  91/09/30  select timeout irq now does chipreset, began testing  Rwb   |
|                 of bus lock recovery code.  (involves low-level retries)  |
| 11  91/10/04  First validation attempt							  Rwb   |
| 12  91/10/10  Boot driver in place, same code, same script at boot, Rwb   |
|                    Ready for first external consumption.					|
| 13  91/10/14  Ready for validation:  Synchronus removed due to	  Rwb   |
|                    reliablility problems.									|
| 14  91/10/23  reworked mask_irq to preserve trace flags.			  wwb   |
| 15  91/10/25  added NOSYNC flag to exec to disable scsi_synchr flag wwb   |
| 16  91/10/28  Fixed typo in defs statement  (boot code only 		  Rwb   |
|				-- freesto()										        |
| 17  91/11/03  Modified script such that message in phase  after	  Rwb   |
|               data in/out is not detected as an error.  This supports     |
|               devices that insist on disconnecting after data with no     |
|               further data to be transfered.  (Archive viper)             |
| 18  91/11/04  reschedule() now transitions the list such that the   Rwb   |
|               lower SCSI ID's are scheduled in favor of the higher        |
|               ID's														|
| 19  91/11/04  Abandon ed.18 change,  grabsem() sleep changed.		  Rwb   |
| 20  93/03/01  Eliminated need for math library. 					  ats   |
|				Added vme162 support.										|
| 21  93/03/23  Wwb reworked coherent() so that all writes complete	  ats   |
|               before unmasking interrupts. Also provided alternate        |
|               scsi id of 3 for in-house 162s.								|
| 22  93/02/10  Reworked irq masking for MSP support.				  wwb   |
| 23  93/04/30  Modified for UCC -mode=compat. Should be backwards	  ats   |
|               compatible with old tools. Large chunks of assembly         |
|               code were extracted to .a files.							|
|						---- OS-9/68K V3.0 Release ----						|
| 24  94/04/19  Started MVME177 support.							  wwb   |
| 25  94/07/20  Moved SCSI ID sanity check into exec() as preparation ats   |
|               for large ID support in the high-level drivers on           |
|               "wide" busses.												|
| 26  94/08/02  Added MAXSCSI definition for scsi id checks.		  wwb   |
| 27  94/11/29  Added missing second parameter on some calls to       ats   |
|                    chipreset();                                           |
| 28  95/01/18  made coherent() keep stack long-aligned.			  wwb   |
| 29  95/02/10  updated snoop control values for mvme177 (dev177.h)   wwb   |
| 30  95/02/15  Changes made to fix problems with selfid and          GbG   |
|					  phase mis-match. Files affected "dev162.c				|
|					  dev167.c dev177.c irq.c irqsup.c misc.c 				|
|					  script.c.												|
|																			|
|					  Fixed sync message protocal in 'irqsup.c' 			|
|					  check_synchmsg. tpg and tos as well as the			|
|					  SCSI state are save in stats area. Code was			|
|					  added in 'init.c' to place the string 'STAT'			|
|					  in memory at the start of the stats area. This		|
|					  will allow the STATS fields to be located more		|
|					  quickly.												|
|																			|
|					  STATS field added:									|
|					  stats_msg[4] =  'STAT'								|
|					  scsi_timing = 0 (ASYNC) 1 = (SCSI I) 2 = (SCSI II)	|
|					  syn_period = tgp;	 record target period 				|
|					  syn_offset = tos;	 record target offset 				|
|																			|
|					  Fixed bug in dev162.c, dev167.c, and dev177.c			|
|					  'start_710' did the calculation for siop->dcntl		|
|					  via clk but never stored the result.					|
|																			|
|					  Support for slow drives added. Add -dSLOW_SCSI		|
|					  to force support.										|
|																			|
|					  Changed SCLK define in 'scsi710.h'. SCLK should		|
|					  now be when get_siop_clock() is not used.				|
|					  To define a SIOP clock of 66 MHZ use "-dSCLK=66".		|
| 31  95/04/05  Fix reject message act script entry (GregG found).	  ats   |
| 31b 95/02/21  ANSIfied/optimized edition.							  GbG   |
| 32b 95/06/06  NCR53c710 specific routines moved to hw53c710 		  GbG   |
|				and devxxx.c												|
| 33b 95/07/08  Added case of descriptor has SYNC and drive does not  GbG 	|
|					support Driver will now recover and drop down to 		|
|					ASYNC.  Changed code to allow OS9 code not to 			|
|					require CPU.L at this time. See mask_irq().				|
|                   Copyright notices were also updated as needed.			|
| 34  95/07/11  Prepare ANSIOpt_Branch code for trunk merge.		  GbG   |
| 35  95/07/14  Split sources. Common code now located in			  GbG   |
|                    "MWOS/SRC/IO/SCSI/NCRSIOP"								|
| 36b 95/07/18  MultiSiop_Branch: Added support for 810/825			  GbG   |
|                    HW53C710.C was also updated to allow reselect			|
|					  attempt on unknown phase error.						|
|					  This error will should only be seen on drives			|
|					  that have disconect with no warning at all!			|
| 36c 95/10/15 Fixed Problem with term.								  GbG   |
| 36d 96/02/24 Improved recovery on major SCSI errors added.		  GbG	|
| 37  96/03/05 Prepare MultiSiop_Branch code for trunk merge.		  GbG   |
| 38  96/03/11 SAVE_STATE now checks opcode to qualify phase-mismatch GbG   |
| 39  96/03/13 Removed case for qualify on phase-mistach.			  GbG   |
| 40  96/03/18 'scripti_msgi'unsolicited synchronus negotiation now   GbG   |
|				allowed 'irq' send signal now qualified.					|
| 41  96/03/22 Changed access mode for data module.					  GbG   |
| 42  96/04/02 SCSI error recover fixes and script fixes for          GbG   |
|              fast SCSI.													|
| 43  96/04/05 'hw53c8xx' check_synchmsg changed to allow sync 		  GbG   |
|               on Sony DAT.												|
| 44  96/05/15 Added Cache Flush support for OS9000.				  GbG   |
| 45  96/05/21 Allow 810 and 825 in same system.					  GbG   |
| 46  96/05/23 'hw53c8xx' 386 PIC ISR clearing added.				  GbG   |
| 47  96/05/24 Fixed error recovery on failed init.					  GbG   |
| 48  96/05/24 Check for lack of low-level statics in term.			  GbG   |
|                     ---- OS-9/68K V3.0.3 Released ----					|
| 49  96/08/26 Added burst opcode fetch and read line enable for 	  GbG   |
|			   810/825.														|
|             ---- OS-9000/x86 V2.1 Released ----                           |
| 50  96/09/02 Read SCSI status as word to prevent 12 CLK delay 	  GbG   |
|			  in hw53c8xx for 810/825.			    						|
| 51  96/09/20 Added OS9000 Cache Flush in hw53c710.                  GbG   |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
| 52 96/11/27 Setup PCI configuration command register for 810/825.   GbG   |
| 53 97/01/23 Added support for Symbios 875.                          GbG   |
| 54 97/03/20 Added support for 710 support for MVME172.              GbG   |
| 55 97/04/10 Fixed script problem on 710 during reselect.            GbG   |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
| 56 97/04/28 Symbios 8xx series update.                                    |
|             Fixed save_state for Symbios 8xx series, this is              |
|             required for Symbios 875 series.                              |
|             Burst for 825/875 changed from 16 to 128                      |
|             Fifo size for 825/875 changed from 88 to 536                  |
|             Added Diamond Fireport20/40                                   |
|             Support for 810A, 810AP, 815, 820, 860, 885, 895, 896         |
|             Added SLOW_SCSI option to force Sync at 5MBS.                 | 
|             Added SCSI_ULTRA FAST20/40 and Wide Support.                  |
|			  The following compile options were added:                     |
|                   -dSCRIPTS_RAM                                           |
|                   -dPCI_IO_MAPPED                                         |
|                   -dSYMBIOS_DEVICE_ID=xx                                  |
|             Added debugging code. The -dDEBUG flag will invoke.     GbG   |
| 57 97/04/10 systype.h in ncrproto only included for NCR53C8xx.      GbG   |
| 58 97/06/01 Fixed problem in save_state for 53c8xx series,                |
|             datacount not adjusted.                                 GbG   |  
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 59 97/07/18 Changed 710 save_state to not use dnad register.        GbG   |
| 60 97/11/17 Added address translation for 53c710.                   gdb   |
| 61 98/01/29 Fixed compiler warnings.                                GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
| 62 99/04/12 Fixed bug in hw53c710.c where control bits were         djl   |
|             overwritten during reset.  Customer First bug #3783.          |
|             ---- Enhanced OS-9/68k Release V1.0 - OS 3.1 ----             |
| 63 99/11/22 Bump edition number to identify Editions remade with    mgh   |
|             defs once again compatible with earlier 68k releases.         |
|             The 3.1 Release broke compatibility. 
| 64 00/03/06 Fixed problem with initial chdir from system process.         |
|             System process ignores signals. See misc.c and irq.c.   GbG   |
| 65 00/05/31 Added 895/896 Ultra2 Wide 160Mhz support (dev8xx.c).          |
|             Modified relocate() in hw53c8xx.c (see info).           KeiT  |
| 66 00/07/27 added hwprobe for 710.                                  gdw   |
| 67 00/08/17 Fixed OSK issue with chdir as well.                     GbG   |
| 68 00/09/27 Fixed OSK CBOOT sleep issue in enqueue.                 GbG   |
| 69 01/01/15 Corrected Script (input_data & output_data) for fast          |
|             Harddisks                                               KeiT  |
| 70 01/02/26 Added C_ADDR to cache call (exec.c)		      sbh   |
| 71 01/08/09 change debug code to use dbglog.                        gdw   |
| 72 09/20/01 Improve the fariness of access to disk (CF10781)        peng	|
`--------------------------------------------------------------------------*/
#define EDITION "72"


/*
 *  Notes:
 *     This driver is specific to the NCR 53C710.  While every 
 *     attempt has been made to make it generic across the entire
 *     NCR 53C7xx family (at least in terms of the scripts and 
 *     the code to manipulate the chip) no guarantees are made.
 *
 *     This driver is also specific to the Motorola mVme167 and mVme162 SBC.
 *     The code is constructed so that device specificity is limited
 *     to "dev167.c" and "dev162.c".  It is hoped that a port could be done
 *     by modifying this module alone.  Of particular note are the
 *     cache control routines and interrupt management.
 *
 *     	Suitability for any purpose beyond functioning example is
 *      not guaranteed.  See readme.167 for further details.
 */
 
/* return chip type */

#if defined (__STDC__) || defined (_ANSI_EXT)
int ncrchip ( void )
#else
int ncrchip ()
#endif
{
	return(710);
}

_asm("_sysedit: equ " EDITION);	/* current edition number */
	 
