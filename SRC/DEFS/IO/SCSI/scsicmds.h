/*--------------------------------------------------------------------------,
|                                                                           |
|          Copyright 1996, 1998 by Microware Systems Corporation            |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
|  SCSI command/status definitions                                          |
|                                                                           |
| history:                                                                  |
|                                                                           |
| ed#   date    changes                                                by   |
| --- -------- ------------------------------------------------------- ---  |
| 00 90/02/13 Merger of all previous files that served the                  |
|                 same exact purpose.                                  Rwb  |
|             ---- OS-9/68K V3.0 Release ----                               |
| 01 96/01/04 Moved to common area for OS9/OS9000 usage                GbG  |
| 02 96/01/08 Added SC_VERIFYEXT						               GbG  |
|             ---- OS9000/PPC V2.0 Release ----                             |
|             ---- OS-9/68K V3.0.3 Release ----                             |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 03 98/01/28 Fixed nested comment warnings from history header.       GbG  |
|             ---- OS-9000/SH3 V2.2.6 Released ----             *           |
|             ---- SCSI Common Sub-Component v1.0 Released ---- *           |
|             $$                <RELEASE_INFO>               $$ *           |
|--------------------------------------------------------------------------*/

/*
 * definitions of scsi status
 */
#define		CHKCOND			0x02		/* check condition */
#define		CONDMET			0x04		/* condition met */
#define		BUSY			0x08		/* device is busy */
#define		INTERMEDIATE	0x10		/* intermediate status */
#define		RESERV_CONFLCT	0x18		/* reservation conflict */
 
/*
 * definitions of scsi command bytes
 */

	/*  These command apply to all devices */
#define		SC_TESTREADY	0x00		/* test unit ready */
#define		SC_REQSENSE		0x03		/* request sense */
#define		SC_INQUIRY		0x12		/* find out about device */
#define		SC_MODESELECT	0x15		/* mode select (6 byte version )*/
#define		SC_RSRVUNIT		0x16		/* reserve unit */
#define		SC_RELEASE		0x17		/* release unit */
#define		SC_COPY			0x18		/* copy data */
#define		SC_MODESENSE	0x1a		/* mode sense  (6 byte version )*/
#define		SC_RECDIAG		0x1c		/* receive diagnostic */
#define		SC_SENDDIAG		0x1d		/* send diagnostic */
#define		SC_READDB		0x3c		/* read data buffer */

	/* These commands are primarily but not limited to direct access */
#define		SC_RESTORE		0x01		/* restore unit */
#define		SC_FORMAT		0x04		/* format the drive */
#define		SC_REASSIGN		0x07		/* reassign blocks */
#define		SC_READ			0x08		/* read data from device */
#define		SC_WRITE		0x0a		/* write */
#define		SC_SEEK			0x0b		/* seek */
#define		SC_REMOVAL		0x1e		/* prevent/allow removal */
#define		SC_READCAP		0x25		/* read capacity */
#define		SC_READXTD		0x28		/* read extended */
#define		SC_WRITEXTD		0x2a		/* write extended */
#define		SC_SEEKEXTD		0x2b		/* seek extended */
#define		SC_WRITEVFY		0x2e		/* write and verify */
#define		SC_DAVERIFY		0x2f		/* verify for Direct Access devices */
#define		SC_RDDEFECT		0x37		/* read defect data */
#define		SC_WRITEDB		0x3b		/* write data buffer */

	/* these commands are primarily sequential-access oriented */
#define		SC_REWIND		0x01		/* rewind the unit */
#define		SC_RETENTION	0x02		/* retention the tape */
#define		SC_READLIMITS	0x05		/* read block limits */
#define		SC_SEEKBLOCK	0x0c		/* seek block */
#define		SC_WRITEMARK	0x10		/* write file mark */
#define		SC_SPACE		0x11		/* space the tape */
#define		SC_VERIFY		0x13		/* verify */
#define		SC_VERIFYEXT	0x2f		/* verify extended */
#define		SC_READBUFDAT	0x14		/* recover buffered data */
#define		SC_RECOVER		0x14		/* recover buffered data */
#define		SC_ERASE		0x19		/* erase tape */
#define		SC_LOAD			0x1b		/* load/unload tape */


  /* vender specific commands for the omti/dtc 5400 controller */
#define		SC_FORMATRK		0x06		/* format one track */
#define		SC_DEFINE		0xc0		/* define floppy disk format */
#define		SC_ASSIGN		0xc2		/* assign disk parameters */
#define		SC_WRITEECC		0xe1		/* ????? */
#define		SC_READID		0xe2		/* ????? read drive id */
#define		SC_READREV		0xc1		/* read revision level */

