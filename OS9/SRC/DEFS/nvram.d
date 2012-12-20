*****************************************************************************
*                                                                           *
*              Copyright 1998 by Microware Systems Corporation              *
*                            All Rights Reserved                            *
*                          Reproduced Under License                         *
*                                                                           *
*  This software is confidential property of Microware Systems Corporation, *
*  and is provided under license for internal development purposes only.    *
*  Reproduction, publication, modification, distribution, or creation of    *
*  derivative works in any form to any party other than the Licensee is     *
*  strictly prohibited, unless expressly provided for under the terms of a  *
*  mutually executed written license agreement for this software between    *
*  the end-user and Microware Systems Corporation, or otherwise expressly   *
*  authorized in writing by Microware Systems Corporation.  Licensee shall  *
*  reproduce this copyright notice exactly as provided herein on any copies *
*  of the software that Licensee is authorized to distribute.               *
*                                                                           *
*****************************************************************************
*                                                                           *
* Edition History:                                                          *
* #   Date     Comments                                                By   *
* --- -------- ------------------------------------------------------- ---- *
*  01 00/01/14 Added edition history.                                  mgh  *
*  02 00/01/14 Add console baud, partity, Networking fields etc.       mgh  *
*  03 00/04/01 Change order of DST Bits to match nvram.h               mgh  *
*              $$                    <RELEASE_INFO>                     $$  *
*****************************************************************************

*
* NVRAM fields
*
* These field offsets must match the definitions in nvram.h.
*
NVR_BOOTER_CNT	set 	16
NVR_SIZE		set		256

 org 0
NVR_NEWSYNC		do.w	1	$00
NVR_VERSION		do.w	1	$02

NVR_SPCRST		do.b	1	$04
NVR_SWITCHES	do.b	1	$05
NVR_BRDID		do.b 	1	$06
NVR_GRPID		do.b	1	$07

NVR_VMEINT		do.b	1	$08
NVR_SLVEN		do.b 	1	$09
NVR_SCSIRST		do.b	1	$0A
* reserved      do.b	1	$0B

*Booter array (4 bytes/entry * 16 entries ) $0c->$4B
NVR_BOOTER		do.l	NVR_BOOTER_CNT 	$0C
NVR_MEMSIZE		do.l	1	$4c

NVR_SLVADDR		do.l	1	$50
NVR_filler1		do.l	1	$54 Padd out to even 8 byte alignment
NVR_CONS_CNTL	do.w	1	$58
NVR_CONS_BAUD	do.b	1	$5a
NVR_CONS_PRTY	do.b	1	$5b
NVR_COMM_CNTL	do.w	1	$5c
NVR_COMM_BAUD	do.b	1	$5e
NVR_COMM_PRTY	do.b	1	$5f
NVR_IP_ADDR		do.l	1	$60
NVR_IP_MASK		do.l	1	$64
NVR_IP_GATE		do.l	1	$68
NVR_TFTPHOST	do.l	1	$6c
NVR_DSTTime		do.l	1	$70 Time of Next DST Change (sec after Midnight)
NVR_DSTDate		do.l	1	$74 Date of Next DST Change (Julian)
NVR_DSTOffset	do.l	1	$78 Seconds to offset(+=DST=>STD, -=STD=>DST)
NVR_DSTStat		do.l	1	$7C Flag/Status bits
NVR_filler2		do.l	((NVR_SIZE-.)>>2)-1
* reserved
NVR_LRSVD		do.w	1	$fc	NVR_SIZE-4
NVR_CKSUM		do.w	1	$fe	NVR_SIZE-2

*
* NVRAM values
*

NVRNEWSYNC		equ		$AB54

 ifndef NVR_VER_MIN
* default for migration to systype.d
NVR_VER_MIN		equ		1
 endc

 ifndef NVR_VER_MAX
* default for migration to systype.d
NVR_VER_MAX		equ		2
 endc

NVV_VINT_MASK	equ		2		version for VME interrupt mask change
NVV_SRST_FLAG	equ		2		version for SCSI reset flag change

NVR_RSTABT		equ		1		bit field for RESET+ABORT switch
NVR_RSTSoft		equ		2		bit field for software forced reset

*************************************************************************
* NV_RAM or VME050 Switch definitions.                                  *
*************************************************************************
* SWITCH 1 / BIT 1: UNUSED                                              *
*       0 = closed: UNUSED                                              *
*       1 = open:   UNUSED                                              *
*************************************************************************
* SWITCH 2 / BIT 2: ROMSWITCH                                           *
*   controls whether to boot from menu choice or auto*sequence.         *
*       0 = closed: menu selected device                                *
*       1 = open:   auto*select sequence                                *
*************************************************************************
* SWITCH 3 / BIT 3:  MENUSWITCH                                         *
*   controls whether to boot from menu choice or auto*sequence.         *
*       0 = closed: menu selected device                                *
*       1 = open:   auto*select sequence                                *
*************************************************************************
* SWITCH 4 / Bit 4: same as 3 on the VME147                             *
*   if switch3 or switch4 = 1, do autoboot]                             *
*       0 = closed: UNUSED                                              *
*       1 = open:   147 Autoboot                                        *
*************************************************************************
* SWITCH 5 / Bit 5: controls the enabling of the debugger               *
*       0 = closed: debugger enabled (default)                          *
*       1 = open:   debugger disabled                                   *
*************************************************************************
* SWITCH 6 / BIT 6: UNUSED                                              *
*       0 = closed: UNUSED                                              *
*       1 = open:   UNUSED                                              *
*************************************************************************
* SWITCH 7 / BIT 7: UNUSED                                              *
*       0 = closed: UNUSED                                              *
*       1 = open:   UNUSED                                              *
*************************************************************************
* SWITCH 8 / BIT 8: UNUSED                                              *
*       0 = closed: UNUSED                                              *
*       1 = open:   UNUSED                                              *
*************************************************************************
SWITCH1		equ	$01		%00000001 = UNUSED
SWITCH2 	equ $02		%00000010 = ROMSWITCH
SWITCH3 	equ $04		%00000100 = MENUSWITCH
SWITCH4 	equ $08		%00001000 = BOOTSWITCH
SWITCH5 	equ $10		%00010000 = DEBUGSWITCH
SWITCH6 	equ $20		%00100000 = UNUSED
SWITCH7 	equ $40		%01000000 = UNUSED
SWITCH8 	equ $80		%10000000 = UNUSED

ROMSWITCH   equ SWITCH2	boot from rom (autoselect)
MENUSWITCH  equ SWITCH3	menu selection switch
BOOTSWITCH  equ SWITCH4	boot selection switch
DEBUGSWITCH equ	SWITCH5	debugger enable switch

*
* Auto Daylight Savings Time Bit definitions (NVR_DSTStat)
* NOTE HIGH WORD IS RESERVED for Driver specific cookie
*
ADSTEn_B    equ $01     Auto DST Enable 0=Off, 1=On 
InDST_B     equ $02     0=Off/STD TIme, 1=On/DST Time
RTCChg_B	equ $04		Change RTC Time also
RTCDST_B	equ $08		RTC Time, 0=STD Time, 1=DST
