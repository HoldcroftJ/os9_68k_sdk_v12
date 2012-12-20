* Definitions for Zilog 8536 CIO chip
 opt -l

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

* Chip (8536) external register offsets
 org 0 
PortC do.w 1 
PortB do.w 1
PortA do.w 1
Control do.w 1

*internal CIO addresses
mic equ 0 master irq control
mcc equ 1<<0 master config conrtol
pai equ 1<<1  port a irq vector
pbi equ 3     port b irq vector
cti equ 1<<2 counter timer irq vector
pac equ 1<<3 port a command and status reg
pbc equ 9
cdpp  equ 5
cdd  equ 6 port c data dir reg
csio  equ 7 port c special io
ct1cs  equ $0a counter/timer 1 command /status
ct2cs  equ $0b counter/timer 2 command /status
ct3cs  equ $0c counter/timer 3 command /status
ct3ms  equ $1e counter/timer mode specification
civ  equ $1f current irq vector

ct3tch equ $1a ct 3 current count msb
ct3tcl equ $1b ct 3 current count lsb
ct3cch equ $14 ct 3 current count msb
ct3ccl equ $15 ct 3 current count lsb


*mic defs
mie equ 1<<7 master irq enable
dlc equ 1<<6 disable lower chain
nv  equ 1<<5 no vector
pavis equ 1<<4 port A vector encludes status
pbvis equ 1<<3 port B vector encludes status
ctvis equ 1<<2 counter timer etc
rja equ 1<<1  right adjst addr
reset equ 1<<0 reset
micStd equ mie!rja
*micStd equ $9e

*mcc defs
pbe equ 1<<7 port B enable
ct1e equ 1<<6 counter timer 1 enable
ct2e equ 1<<5 counter timer 2 enable
ct3e equ 1<<4 counter timer 3 and port c enable
portlk equ 1<<3
pae equ 1<<2 port A enable
mccStd equ pae

* port A offsets
pam equ $20 port a mode reg
pah equ $21 port a handshake reg
papp equ $22 port a data path polarity reg
padd equ $23 port a data direction reg
pasio equ $24 port a special io reg
pap equ $25 port a pattern polarity
pat equ $26 port a patterrn transition reg
patm equ $27 port a pattern mask reg
 
 
 
* port A internal register definitions for ...
*pac 
* status bits...
* first three are universal
pie equ 1<<6 port irq enable
ius  equ 1<<7 irq under service
ip   equ 1<<5 irq pending
pie_b equ 6
ius_b equ 7
ip_b equ 5

ore equ 1<<3 output reg empty (read only)
ire equ 1<<2 input reg empty (read only)
pmf equ 1<<1 pattern match flag (readonly)
* command codes..
null equ 0
cipius equ 1<<5 clr irq pending and irq under service
irqoe equ 1<<0 irq on error 
sius equ 1<<6 set irq under service
cius equ cipius!sius clr ius
sip equ 1<<7 set ip
cip equ sip!cipius clr ip
sie equ sip!sius set irq enable
cie equ pie!ius!ip irq enable

pacStd equ sie set irq enable
 
*pam
bitport equ 0
inport equ 1<<6 
outport equ 1<<7
biport equ inport!outport
itb equ 1<<5 irq on 2 bytes
imo equ 1<<3 irq on match only
andmode equ 1<<1
ormode equ 1<<2
orprioty equ andmode!ormode
lpm equ 1<<0 latch bits on match
pamStd equ bitport!orprioty

*pah port A handshake
pahStd equ 0 not used much here

*papp port A data  path polarity
sbxp7i0 equ 1<<7    1= invert the assertion 
sbxp8i0 equ 1<<6   etc
qicrdy equ 1<<5
qicexc equ 1<<4
sbxp7i1 equ 1<<3
sbxp8i1 equ 1<<2
sbxp7m  equ 1<<1
sbxp8m  equ 1<<0
* ifeq DMA-ME
*pappStd equ ^sbxp8i0 me board
* else 
pappStd equ ^sbxp7i0 m10 board
* endc

*padd port A data direction reg
paddStd equ $FF  1= input 0= output

*pasio port A special io reg
*pasioStd equ $80 ones catching 
pasioStd equ $00  disable ones catching 

* pap port A pattern polarity reg
* ifeq DMA-ME
*papStd equ $40  for me
* else 
papStd equ $80  for M10
* endc

*pat port A pattern transistion reg
patStd equ $00 all transistions

*patm port A pattern mask reg
* ifeq DMA-ME
*patmStd equ $40 mask off all but sbx int1 p8  
* else 
patmStd equ $80 mask off all but sbx int1 p7  
* endc

* port B offsets
pbm equ $28 port b mode reg
pbh equ $29 port b handshake reg
pbpp equ $2A port b data path polarity reg
pbdd equ $2B port b data direction reg
pbsio equ $2C port b special io reg
pbp equ $2D port b pattern polarity
pbt equ $2E port b patterrn transition reg
pbtm equ $2F port b pattern mask reg


* ct3 mode specification register

csc  equ 1<<7 continuous or single cycle
eoe  equ 1<<6 ext output enable
ece  equ 1<<5 ext count enable
ete  equ 1<<4 ext trigger enable
ege  equ 1<<3 ext gate enable
reb  equ 1<<2 retrigger enable
po  equ 0 pulse output
oso  equ 1<<0 one shot output
swo  equ 1<<1 square wave out
dns  equ 3 do not specify
opdc equ swo

* ct 3 command status register

* the 4 msb of all command status reg are same
cntip  equ 1<<0 count in progress
trigc  equ 1<<1 trigger command
gcb  equ 1<<2 gate command bit
rcc  equ 1<<3 read counter control

 opt l
