 ttl Floppy controller defs

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
*  01 98/07/29 Added edition history.                                  wwb  *
*				---- OS-9/68K V3.1 Release ----								*
*             $$                    <RELEASE_INFO>                     $$   *
*****************************************************************************


****************************************************
*
* Fd179X Status/Command definitions
*

* Cmd
F_WrtTrk equ $F4 Write track command
F_Type1  equ $D0 Force type 1 status
F_WrSect equ $A0 Write sector
F_RdSect equ $80 Read sector
F_Seek   equ $1B Seek cmd
F_Restor equ $0B Restore cmd
EnDelay  equ $4 Enable settling delay
EnMotOn  equ $8 Enable spin-up sequence
StepMask equ $3 Step rate bit mask

* Status
MotIsOn  equ $80 motor on line status
WritProt equ $40 media write protected
MotSpnUp equ $20 motor spin-up delay complete
SeekErro equ $10 seek error / RNF
CRCError equ $8 CRC bad
OnTrack0 equ $4 on track zero
LostData equ $4 lost data on xfer - cpu slow responding
IndexPls equ $2 index hole present
DataRqst equ $2 data requested by FDC
CtlrBusy equ $1 command in progress

ErrBits  equ SeekErro+CRCError+LostData
FErrBits equ ErrBits+WritProt

* Bit definitions
Busy_Bit equ 0
DatR_Bit equ 1
Indx_Bit equ 1
Prec_Bit equ 1          write precomp bit 1 = no-precomp !!!
LDat_Bit equ 2
Tr00_Bit equ 2
CRCE_Bit equ 3
SkEr_Bit equ 4
SpUp_Bit equ 5
WrPr_Bit equ 6
MotO_Bit equ 7


* end of file
