#if !defined(_FUNCS_H)
#define _FUNCS_H

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
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 98/07/29 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/


#define	F_APROC   0x2c	/* F$AProc: Enter Active Process Queue */
#define	F_ALARM   0x56	/* F$Alarm: send alarm signal */
#define	F_ALLBIT  0x13	/* F$AllBit: Allocate in Bit Map */
#define	F_ALLPD   0x30	/* F$AllPD: Allocate Process/Path Descriptor */
#define	F_ALLPRC  0x4b	/* F$AllPrc: Allocate Process Descriptor */
#define	F_ALLRAM  0x39	/* F$AllRAM: Allocate RAM blocks */
#define	F_ALLTSK  0x3f	/* F$AllTsk: Allocate Process Task number */
#define	F_CCTL    0x5a	/* F$CCtl:	cache control */
#define	F_CRC     0x17	/* F$CRC: Generate CRC */
#define	F_CHAIN   0x05	/* F$Chain: Chain Process to New Module */
#define	F_CHKMEM  0x58	/* F$ChkMem: determine if user process may access memory area */
#define	F_CMPNAM  0x11	/* F$CmpNam: Compare Two Names */
#define	F_CPYMEM  0x1b	/* F$CpyMem: Copy External Memory */
#define	F_CRYPT   0x67	/* F$Crypt: Perform Cryptographic Functions */
#define	F_DATTACH 0x64	/* F$DAttach: Debugger attach to running process */
#define	F_DEXEC   0x23	/* F$DExec: Debugging execution call (single step) */
#define	F_DEXIT   0x24	/* F$DExit: Debugging exit call (kill child) */
#define	F_DFORK   0x22	/* F$DFork: Debugging Fork call */
#define	F_DATMOD  0x25	/* F$DatMod: Create data module */
#define	F_DELBIT  0x14	/* F$DelBit: Deallocate in Bit Map */
#define	F_DELPRC  0x4c	/* F$DelPrc: Deallocate Process Descriptor */
#define	F_DELTSK  0x40	/* F$DelTsk: Deallocate Process Task number */
#define	F_EVENT   0x53	/* F$Event: Create/Link to named event */
#define	F_EXIT    0x06	/* F$Exit:	Terminate Process */
#define	F_FINDPD  0x2f	/* F$FindPD: Find Process/Path Descriptor */
#define F_FIRQ    0x61  /* F$FIRQ: Add/Remove Fast IRQ service */
#define	F_FLASH   0x65	/* F$Flash:  Manage FLASH device(s) */
#define	F_FMODUL  0x4e	/* F$FModul: Find Module Directory Entry */
#define	F_FORK    0x03	/* F$Fork:	Start New Process */
#define	F_GBLKMP  0x19	/* F$GBlkMp: get System Block Map copy */
#define	F_GMODDR  0x1a	/* F$GModDr: get Module Directory copy */
#define	F_GPRDBT  0x1f	/* F$GPrDBT: Get system global data copy */
#define	F_GPRDSC  0x18	/* F$GPrDsc: get Process Descriptor copy */
#define	F_GPROCP  0x37	/* F$GProcP: Get Process ptr */
#define	F_GSPUMP  0x5b	/* F$GSPUMp: get SPU map information for a process */
#define	F_GREGOR  0x54	/* F$Gregor: Convert julian date to gregorian date */
#define F_HLPROTO 0x70	/* F$HLProto: High-Level Protocol manager request */
#define	F_ID      0x0c	/* F$ID: Return Process ID */
#define	F_IODEL   0x33	/* F$IODel: Delete I/O Module */
#define	F_IOQU    0x2b	/* F$IOQu:	Enter I/O Queue */
#define	F_IRQ     0x2a	/* F$IRQ: Enter IRQ Polling Table */
#define	F_ICPT    0x09	/* F$Icpt:	Set Signal Intercept */
#define	F_JULIAN  0x20	/* F$Julian: Convert gregorian to Julian date */
#define	F_LINK    0x00	/* F$Link:	Link to Module */
#define	F_LOAD    0x01	/* F$Load:	Load Module from File */
#define	F_MBUF    0x5f	/* F$MBuf:	Memory buffer manager */
#define	F_MEM     0x07	/* F$Mem: Set Memory Size */
#define	F_MOVE    0x38	/* F$Move:	Move Data */
#define	F_NPROC   0x2d	/* F$NProc: Start Next Process */
#define	F_PERR    0x0f	/* F$PErr:	Print Error */
#define	F_POSK    0x5d	/* F$POSK:	execute svc request */
#define	F_PANIC   0x5e	/* F$Panic: Panic warning */
#define	F_PERMIT  0x3a	/* F$Permit: (old F$AllImg) Allocate Image RAM blocks */
#define	F_PROTECT 0x3b	/* F$Protect: (old F$DelImg) Deallocate Image RAM blocks */
#define	F_PRSNAM  0x10	/* F$PrsNam: Parse Pathlist Name */
#define F_PWRMAN  0x66	/* F$PwrMan: Perform Power Management functions */
#define	F_RTE     0x1e	/* F$RTE: Return from Intercept routine */
#define	F_RETPD   0x31	/* F$RetPD: Return Process/Path Descriptor */
#define	F_SPRIOR  0x0d	/* F$SPrior: Set Process Priority */
#define	F_SRQCMEM 0x5c	/* F$SRqCMem: System Colored Memory Request */
#define	F_SRQMEM  0x28	/* F$SRqMem: System Memory Request */
#define	F_SRTMEM  0x29	/* F$SRtMem: System Memory Return */
#define	F_SSPD    0x0b	/* F$SSpd:	Suspend Process */
#define	F_SSVC    0x32	/* F$SSvc:	Service Request Table Initialization */
#define	F_STIME   0x16	/* F$STime: Set Current Time */
#define	F_STRAP   0x0e	/* F$STrap: Set Trap Intercept */
#define	F_SUSER   0x1c	/* F$SUser: Set User ID number */
#define	F_SCHBIT  0x12	/* F$SchBit: Search Bit Map */
#define F_SEMA    0x62  /* F$Sema: Semphore P/V (reserve/release) service */
#define	F_SEND    0x08	/* F$Send:	Send Signal to Process */
#define	F_SETCRC  0x26	/* F$SetCRC: Generate valid header and CRC in module */
#define	F_SETSYS  0x27	/* F$SetSys: Set/examine system global variable */
#define	F_SIGMASK 0x57	/* F$SigMask: set signal mask */
#define	F_SIGREST 0x63	/* F$SigReset:  Reset signal intercept context */
#define	F_SLEEP   0x0a	/* F$Sleep: Suspend Process */
#define	F_SYSDBG  0x52	/* F$SysDbg: Invoke system level debugger */
#define	F_SYSID   0x55	/* F$SysID: return system identification */
#define	F_TLINK   0x21	/* F$TLink: Link trap subroutine package */
#define	F_TIME    0x15	/* F$Time:	Get Current Time */
#define	F_TRANS   0x60	/* F$Trans: Translate memory address to/from external bus */
#define	F_UACCT   0x59	/* F$UAcct: inform user accounting of process status */
#define	F_UNLINK  0x02	/* F$UnLink: Unlink Module */
#define	F_UNLOAD  0x1d	/* F$UnLoad: Unlink Module by name */
#define	F_VMODUL  0x2e	/* F$VModul: Validate Module */
#define	F_WAIT    0x04	/* F$Wait:	Wait for Child Process to Die */
#define	I_ATTACH  0x80	/* I$Attach: Attach I/O Device */
#define	I_CHGDIR  0x86	/* I$ChgDir: Change Default Directory */
#define	I_CLOSE   0x8f	/* I$Close: Close Path */
#define	I_CREATE  0x83	/* I$Create: Create New File */
#define	I_DELETE  0x87	/* I$Delete: Delete File */
#define	I_DETACH  0x81	/* I$Detach: Detach I/O Device */
#define	I_DUP     0x82	/* I$Dup: Duplicate Path */
#define	I_GETSTT  0x8d	/* I$GetStt: Get Path Status */
#define	I_MAKDIR  0x85	/* I$MakDir: Make Directory File */
#define	I_OPEN    0x84	/* I$Open:	Open Existing File */
#define	I_READ    0x89	/* I$Read:	Read Data */
#define	I_READLN  0x8b	/* I$ReadLn: Read Line of ASCII Data */
#define	I_SGETST  0x92	/* I$SGetSt: Getstat using system path number */
#define	I_SEEK    0x88	/* I$Seek:	Change Current Position */
#define	I_SETSTT  0x8e	/* I$SetStt: Set Path Status */
#define	I_WRITLN  0x8c	/* I$WritLn: Write Line of ASCII Data */
#define	I_WRITE   0x8a	/* I$Write: Write Data */
	
#define SYSTRAP	  0x8000 /* System state system call bit */

/* F_PANIC codes */

#define	K_IDLE		0x01	/* last "official" process has exited */
#define	K_PFAIL		0x02	/* power failure occurred */


#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
error_code _os9_setsvc(void *, void *);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#else
error_code _os9_setsvc();
#endif

#endif /* _FUNCS_H */
