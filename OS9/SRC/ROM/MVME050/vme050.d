****************************************************
* Board Specific Definitions for the MVME050 card
*
 opt -l
 ifndef SYSTYPE_VME050

 pag
********************************
* Edition History
*   date    comments                                             by
* -------- ----------------------------------------------------  ---
* 89/05/03 Hacked from devpaks in general (3 new kittens today!) jal
* 89/05/05 Added RTC and TODC conditionals                       Rwb
* 89/07/06 Updated for 2.3 release.                              wwb
* 89/07/11 Updated for unified vectors.                          wwb
*          ---- OS-9/68K V2.3 Release ----
*          ---- OS-9/68K V2.4 Release ----
*          ---- OS-9/68K V3.0 Release ----
*          ---- OS-9/68K V3.1 Release ----
*

********************************
* Interrupt Levels
*
Port1Level equ 1 user terminal
Port2Level equ Port1Level user terminal
PPrinterLevel equ Port1Level parallel printer
SPrinter1Level equ Port1Level serial printer on port 1
SPrinter2Level equ Port2Level serial printer on port 2
RTCLevel equ 6 MC146818 TOD Clock chip

********************************
* Interrupt Vector assignments
*
Port1Vect equ 190 auxiliary i/o
Port2Vect equ 191 user terminal
PPrinterVect equ 192 parallel printer
SPrinter1Vect equ Port1Vect serial printer
SPrinter2Vect equ Port2Vect serial printer
RTCVect equ 193 VME050 time of day clock

*********************************
* System I/O addressing
*
 ifndef ShortIOBase
ShortIOBase equ $FFFF0000
 endc
VME50Base equ ShortIOBase+$1000 VME050 base address
Port1Base equ VME50Base port 1, VME050 module
Port2Base equ VME50Base+$40 port 2, VME050 module
PPrinterBase equ VME50Base+$80 parallel port, VME050 module
SPrinter1Base equ Port1Base serial printer
SPrinter2Base equ Port2Base serial printer
RTCBase equ VME50Base+$100 MC146818 base address

************************************************
* SCF device descriptor definitions
* (used only by SCF device descriptor modules)
*
* SCFDesc: Port,Vector,IRQlevel,Priority,Parity,BaudRate,DriverName
*
T10 macro
 SCFDesc Port1Base+1,Port1Vect,Port1Level,5,$00,$0e,sc68560
DevCon set 0
 endm

T11 macro
 SCFDesc Port2Base+1,Port2Vect,Port2Level,5,$00,$0e,sc68560
DevCon set 0
 endm

P10 macro
 SCFDesc SPrinter1Base+1,SPrinter1Vect,SPrinter1Level,5,$00,$0e,sc68560
DevCon set 0
* Define access permissions here:
Mode set Share_+ISize_+Write_
 endm

P11 macro
 SCFDesc SPrinter2Base+1,SPrinter2Vect,SPrinter2Level,5,$00,$0e,sc68560
DevCon set 0
* Define access permissions here:
Mode set Share_+ISize_+Write_
 endm

P12 macro
 SCFDesc PPrinterBase+1,PPrinterVect,PPrinterLevel,5,$00,$00,scpio050
DevCon set 0
* Define access permissions here:
Mode set Share_+ISize_+Write_
 endm

SYSTYPE_VME050 set 0
 endc
 opt l
