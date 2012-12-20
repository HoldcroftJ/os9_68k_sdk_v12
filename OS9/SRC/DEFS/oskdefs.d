 opt -l
* This file is used because of various restrictions on external names

********************************
* Module Type/Language field

* Type values
*
Prgrm		equ 1 Program Module
Sbrtn		equ 2 Subroutine Module
Multi		equ 3 Multi-Module
Data		equ 4 Data Module
CSDData		equ 5 Configuration Status Descriptor
CDBData		equ CSDData Configuration Description Block (same value as CSDData)
TrapLib		equ 11 Trap handler library
Systm		equ 12 System
FlMgr		equ 13 File Manager
Drivr		equ 14 Device Driver
Devic		equ 15 Device Descriptor

* Language values
*
Objct		equ 1 Object Code Module
ICode		equ 2 Basic I-code
PCode		equ 3 Pascal P-code
CCode		equ 4 C I-code
CblCode		equ 5 Cobol I-code
FrtnCode	equ 6 Fortran I-code
JavaCode	equ 7 Java byte code


********************************
* Module and Attr/Revision field

* Attribute bits
ReEnt		equ $80 module is re-entrant
Ghost		equ $40 module remains in memory when not in use
SupStat		equ $20 module must execute in supervisor state

ReEntBit	equ 7 re-entrant module bit number
GhostBit	equ 6 Ghost module bit number
SupStBit	equ 5 supervisor state bit number

********************************
* Module/File Access Permissions and Mode values

*   If bit is set, access is permitted.

Read_	equ %00000001
Write_	equ %00000010
Exec_	equ %00000100
Updat_	equ Read_+Write_
PRead_	equ %00001000
PWrit_	equ %00010000
PExec_	equ %00100000
PUpdat_	equ PRead_+PWrit_
Append_ equ %00010000 append all writes to EOF
ISize_	equ %00100000 (create Mode value only)
Share_	equ %01000000 non-Sharable file if set
Dir_	equ %10000000 Directory file if set
 opt l
