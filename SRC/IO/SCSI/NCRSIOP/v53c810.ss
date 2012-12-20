;  
;   v53c810.ss - low level driver script for scsi810 driver.
;
;**********************************************************************  
;*  Copyright 1991-1997 by Microware Systems Corporation              *
;*  Reproduced Under License                                          *
;*                                                                    * 
;*  This source code is the proprietary confidential property of      *
;*  Microware Systems Corporation, and is provided to licensee        *
;*  solely for documentation and educational purposes. Reproduction,  *
;*  publication, or distribution in any form to any party other than  *
;*  the licensee is strictly prohibited.                              *
;**********************************************************************  
;  
;  Edition History
;
;  ed#   date    by   changes
;  --- --------  ---  -----------------------------------------
;  01  95/06/14  GbG  Adapted from v53c710.ss Edition 17
;  02  96/04/02  GbG  Entended message fixes.
;*                    ---- OS-9000/x86 V2.1 Released ----            *
;*                    ---- OS-9000/PPC V2.1.1 Released ----          *
;*                    ---- OS-9000/ARMv3 V2.2 Released ----          *
;* 03  97/04/10  GbG  Added Wide Support.
;* 04  97/04/26  GbG  Dmode removed. IO mode done by driver.
;*                    ---- OS-9000/PPC V2.1.2 Released ----          *
;*                    ---- OS-9000/SH3 V2.2.6 Released ----          *
;  70  01/01/15 KeiT  Changed input_data & output_data from "if MSG_IN" to "when MSG_IN"
;*                    $$                    <RELEASE_INFO>        $$ *
;  
ARCH 825
pass(#define NCR_REVISION 4)
;
pass( )
pass(/*)
pass(*   Compile instructions:)
pass(*   1. Delete the line below 'typedef unsigned long ULONG;')
pass(*   2. Delete *External_Names array) 
pass(*   3. Delete *Absolute_Names array)
pass(*   4. Compile using 'nasm v53c810.ss -u -o v53c810.c')
pass(*   5. Copy v53c810.c to SCSI53c810 directory and re-make)
pass(*/)
pass( )
pass(typedef unsigned long const ULONG;)
;
;
;		Definition area
;  These definitions provide the offset in bytes from the DSA register
;  to the values to be used.  If there is a change to the static storage
;  definition of logical thread (lthread - see scsi710.h) these defs
;  would also change.
;
ABSOLUTE datacount    = 0x0000	; Data phase size and pointer
ABSOLUTE cdblength    = 0x0008	; Command size and pointer
ABSOLUTE statucount   = 0x0010	; Status size and pointer
ABSOLUTE msgoptr      = 0x0018	; Message Out size and pointer
ABSOLUTE msgiptr      = 0x0020	; Message In size and pointer 
ABSOLUTE devfield     = 0x0028	; Target ID, Synchronous/Parity register
;
;  This is the offset to the extended message frame which is created when
;  an extended message in is detected.
;
ABSOLUTE extmsgcount  = 0x0000
ABSOLUTE  extmsgptr   = 0x0004
;
;   Message definitions
;
ABSOLUTE M_COMPLETE    =  0x0	; Command complete message
ABSOLUTE M_EXTENDED    =  0x1	; message identifier
ABSOLUTE M_SAVEPOINT   =  0x2	; Save data pointers
ABSOLUTE M_RESTPOINT   =  0x3	; Restore data pointers
ABSOLUTE M_DISCONNECT  =  0x4	; Disconect message (interum discon.)
ABSOLUTE M_INITERROR   =  0x5	; Initiator detected an error
ABSOLUTE M_ABORT       =  0x6	; Abort Operation Message
ABSOLUTE M_REJECT      =  0x7	; Message Rejected
ABSOLUTE M_NOOP        =  0x8	; No-Op -- no other message to send
ABSOLUTE M_PARITYER    =  0x9	; Message parity error
ABSOLUTE M_DEVRESET    =  0xC	; Tell the device to reset
;
ABSOLUTE SYNC_REQEST       = 0x1	; Synchronus Transfer Negotiations
ABSOLUTE SYNCREQ_SIZE      = 0x3	; Size of additional bytes in Sync Req.
;
ABSOLUTE IDENTIFY_NODISCON = 0x80	; Identify message, no disconnect
ABSOLUTE IDENTIFY_DISCON   = 0xC0	; Identify message, disconnect allowed
;
;   We use the scratch register to determine flag certain phase information
;   to the CPU side of the driver.  These values appear in scratch0 of the
;   SIOP at certain times.
;
ABSOLUTE PH_MASK   = 0x0F  ; only least significant bits count 
ABSOLUTE PH_SELECT = 0x01  ; Attempting to select a device
ABSOLUTE PH_MESS   = 0x02  ; message phase (at connection)
ABSOLUTE PH_MESSD  = 0x12  ; message phase (which disconnect)
ABSOLUTE PH_MESSC  = 0x22  ; message phase (command complete)
ABSOLUTE PH_MESSA  = 0x03  ; message out -- abort
ABSOLUTE PH_CMD    = 0x04  ; command phase
ABSOLUTE PH_IDATA  = 0x05  ; input data phase in progress
ABSOLUTE PH_ODATA  = 0x15  ; output data phase in progress
ABSOLUTE PH_STATUS = 0x06  ; status phase
ABSOLUTE PH_DONE   = 0x00  ; not a special phase
ABSOLUTE PH_RES4   = 0x30  ; Reserved Out
ABSOLUTE PH_RES5   = 0x31  ; Reserved In
ABSOLUTE PH_WHICH  = 0xFF  ; Which Phase
;
; Absolute values are stored in DSPS Register for
; purposes of interrupt processing.  These values and their
; meaning must aggree between the CPU driver and SIOP script.
;
;     DSPS messages -- not of the error or problem variety 
;
ABSOLUTE SC_DONE   = 0x0080	;   command complete, target disconnected
ABSOLUTE SC_SIGP   = 0x0090	;   SIGP creates this when wait for reselect 
ABSOLUTE SC_RESEL  = 0x00A0	;   reselected 
ABSOLUTE SC_IDISC  = 0x00B0	;   intermediate disconnect received 
ABSOLUTE SC_EXMSG  = 0x00C0	;   extended message received.
ABSOLUTE SC_MSGREJ = 0x00D0	;	message reject received 
ABSOLUTE SC_SWIDEB = 0x00E0 ;	move wide byte residual
;
;    DSPS messages of the error variety (usually protocol violations) 
;
ABSOLUTE NOTMSGO = 0x0FF01	;   not message out after selection 
ABSOLUTE NOTCMD  = 0x0FF02	;   unexpected phase before command 
ABSOLUTE NOTSTAT = 0x0FF03	;   not status phase after data transfer 
ABSOLUTE NOTMSGI = 0x0FF04	;   not message in after status phase 
ABSOLUTE UNKNPH  = 0x0FF05	;   unidentified next phase 
ABSOLUTE BADMSG  = 0x0FF06	;   unidentified message in recieved 

ABSOLUTE SEL_DONE     = 0x0000	; value written to scratch(memory) to indicate
                                ;  selection phase complete.
;
; Other register defs
;
ABSOLUTE B_CON   = 0x10
ABSOLUTE B_RSIGP = 0x40
ABSOLUTE B_DIOM_ON = 0x10
ABSOLUTE B_DIOM_OFF = 0xEF
ABSOLUTE B_SIOM_ON = 0x20
ABSOLUTE B_SIOM_OFF = 0xDF

; move dmode &  B_DIOM_OFF to dmode 
; move dmode | B_DIOM_ON to dmode 

;
;
;   Entry points for the SIOP.  These are the various starting points
;   at which the CPU starts the SIOP.
;
ENTRY WAITFORRESELECT      ; No commands ready, wait for reselect (Idle)
ENTRY SELECTNOATN          ; Select Target without asserting attention
ENTRY SELECTATN            ; Select Target with attention asserted
ENTRY WHICHPHASE           ; Determime current phase and route accordingly
ENTRY GETSTATUS            ; Get the status in message
ENTRY RESELECTED           ; check for more messages to come on reselection
ENTRY ABORTMESSAGE         ; Send an abort message to the Target and disconnect
ENTRY REJECTMESSAGE        ; Send a message reject
ENTRY PEMESSAGE	           ; Send a message parrity error message
ENTRY SENDMESSAGE          ; Send an arbitrary message out
ENTRY FREEIT               ; Free bus on fatal error
ENTRY ACKANYMSG			   ; Ack any message
ENTRY TEST				   ; Test Entry
ENTRY MOVE_SWIDE_BYTE      ; Move SWIDE byte
;
extern statadr, phythread, lcrcid_tid, lcrcid_idmsg,  extcntptr
extern  extcntptr1,  extmframeptr,  armsg_abort,  armsg_reject
extern  armsg_mpe,  bitbucket, RW_Last_di_byte_buf
;
;  NCR 53C810 register addresses used for the memory move commands
;
extern ncrscratch3  	; address of scratch 3 register
extern ncrscratch1 	; address of scratch 1 register
extern ncrscratch0 	; address of scratch 0 register
extern ncrdsa 		; data storage address
extern ncrlcrc		; for id of reselector
;
;
; This is the SCSI SCRIPT itself.
;
;  Idle entry point,  no commands ready so hang out in case reselected.
;
WAITFORRESELECT:
move PH_DONE to scratcha0
clear target
wait reselect REL(alt_sig_p1)
jump REL(resel_adr)
;
alt_sig_p1:
move scntl1 & B_CON to sfbr ; connected?
jump REL(alt_sig_p2) if B_CON 
;
move ctest2 & B_RSIGP to sfbr
jump REL(sig_p_set) if B_RSIGP 
jump REL(WAITFORRESELECT)
;
sig_p_set:
int SC_SIGP
jump REL(resel_adr)
;
;
alt_sig_p2:
move ctest2 to sfbr
jump REL(WAITFORRESELECT)
;
; already selected of re-selected
;
already_selected:
move scntl0 & 0xdf to scntl0
jump REL(resel_adr)
;
; select without attention
;  This routine will not assert attention at selection.  When the
;  path options do not have SCSI_ATN set, this is the entry point
;  that is used to start off the command.  Devices should not disconnect
;  as there is no message phase after selection with which to tell the
;  device that it is legal to disconnect.
;
SELECTNOATN:
move PH_SELECT to scratcha0  ; indicate entering attempt to reach target
clear target
select FROM devfield, REL(already_selected)
jump REL(snatn1) when CMD ; wait for phase change... must be command phase.
jump REL(WHICHPHASE)
;
;  If selection does not timeout, we will arrive here.
;
snatn1:
move SEL_DONE to scratcha3 ; status for the driver
move memory 1, ncrscratch3, statadr ; to the flag location
move memory 4, phythread, ncrdsa ; now restore the dsa
 
jump REL( send_cmd ) when CMD
jump REL(WHICHPHASE)
;
; select with attention
;	This entry point will assert attention with select.  This entry point
; is used when SCSI_ATN is set in the path options.  We also advertise that
; interrmediate disconnect is allowed during selection.
;
SELECTATN:
move PH_SELECT to scratcha0  ; indicate entering attempt to reach target
clear target
select atn FROM devfield, REL(already_selected)
jump REL(satn1) when MSG_OUT ; wait for selection ... must be message out
jump REL(WHICHPHASE)
;
satn1:
move SEL_DONE to scratcha3 ; status for the driver
move memory 1, ncrscratch3, statadr ; to the flag location
move memory 4, phythread, ncrdsa ; now restore the dsa

;
; Have entered message out phase...
;
move PH_MESS to scratcha0 ; set phase flag 
move FROM msgoptr, when MSG_OUT ; send the i.d. message out to the target
;
;  Look for a message in phase.  Some devices will try to go synchronus
;  at the first time they see select with attention.  Under normal
;  circumstances however, we will have sent the negotiation request and
;  are about to receive his answer.  It is not an error if we don't do
;  this phase at this time.
;
jump REL(messgin) when MSG_IN ; message in requested
jump REL(WHICHPHASE) when not CMD
;
; Command phase
;
send_cmd:
clear ATN
move PH_CMD to scratcha0 ; entered command phase when 
move memory 4, phythread, ncrdsa ; now restore the dsa
move FROM cdblength, when CMD ; send the command to the target
;
; At this point, there are several options as to how to proceed.  The next
; phase will be selected by the target dependent on the command and what
; needs to be done.  We depart for a slice of code that will route on the
; phase.
;
jump REL(WHICHPHASE) 
;
; Data_In phase operation.
;
input_data:
move PH_IDATA to scratcha0  ; indicate start of data phase
move memory 4, phythread, ncrdsa ; now restore the dsa
move FROM datacount, when DATA_IN ; transfer data
jump REL(messgin) when MSG_IN
jump REL(get_status) if STATUS
jump REL(WHICHPHASE)
;
; Data_Out phase operation.
;
output_data:
move PH_ODATA to scratcha0  ; indicate start of data phase
move memory 4, phythread, ncrdsa ; now restore the dsa
move FROM datacount, when DATA_OUT ; transfer data
jump REL(messgin) when MSG_IN
jump REL(get_status) if STATUS
;
; Route operation based on next detected phase.
;
WHICHPHASE:
move PH_WHICH to scratcha0			; indicate start of data phase
jump REL(messgin) when MSG_IN		; message in
jump REL(send_cmd) when CMD        	; command phase
jump REL(input_data) when DATA_IN	; data input
jump REL(output_data) when DATA_OUT	; data output
jump REL(get_status) when STATUS	; status phase
jump REL(messgout) when MSG_OUT		; message out
jump REL(undefout) when RES4		; reserved out
jump REL(undefin) when RES5			; reserved in
int UNKNPH                        	; unexpected next phase 
;
; do message out phase
; 
messgout:
move PH_MESSA to scratcha0 ; have entered Message Out
move memory 4, phythread, ncrdsa ; now restore the dsa
move FROM msgoptr, when MSG_OUT
jump REL(WHICHPHASE)
;
; do undefined out phase
; this phase should not occur but if it does we will
; allow it.
;
undefout:
move PH_RES4 to scratcha0 ; have entered Reserved Out
move memory 4, phythread, ncrdsa ; now restore the dsa
move 1, bitbucket, when RES4
jump REL(WHICHPHASE)
;
; do undefined in phase
; this phase should not occur but if it does we will
; allow it.
;
undefin:
move PH_RES5 to scratcha0 ; have entered Reserved In
move memory 4, phythread, ncrdsa ; now restore the dsa
move 1, bitbucket, when RES5
jump REL(WHICHPHASE)
;
; Status Phase
;
GETSTATUS:
get_status:
move PH_STATUS to scratcha0 ; have entered status phase
move memory 4, phythread, ncrdsa ; now restore the dsa
move FROM statucount, when STATUS ;get status byte into memory
jump REL(messgin) when MSG_IN ; message in requested
jump REL(WHICHPHASE) 
;
; Resume point after message in, also logical conclusion to the
; normal flow of command.  When the SIOP reads data during message_in
; it will leave ACK asserted,  which effectivly locks the SCSI Bus until
; we have a chance to parse the message.
ackanddisc:
move 0x00 to SCNTL2 ;clear the SCSI disconnect
clear ack
;
; wait for a physical disconnect from the bus and inform driver when it
; occurs.
;
wait DISCONNECT
int SC_DONE
;
ACKANYMSG:
ackanymsg:
clear ack
jump REL(messgin) when MSG_IN		; message in
jump REL(WHICHPHASE)
;
;  Reselected:  the target has awakened and is now awakening us.
;
;  Note:  The message buffer that is being written may be one of:
;     The idle ID (selfid) message in buffer.
;     A newly started commands message in buffer.
;	If the driver is concerned with the contents of the message in it
;   must look in the message buffer of the "current physical thread"
;   for the data.  This should not be a problem in any known case, as
;   a pointer to the block is present at interrupt time.  Note that only
;   the entire message phase IS NOT preserved.
;
;   Note:  DSA is destroyed...
;
resel_adr:
move memory 4, phythread, ncrdsa ; now restore the dsa
move ssid & 0xf to sfbr
move sfbr to scratcha1
move memory 1,  ncrscratch1, lcrcid_tid ; save reselector's ID.
move 1, lcrcid_idmsg, when MSG_IN ; get message from target
move sfbr & IDENTIFY_NODISCON to sfbr
jump rel(idmsgin) if not 0 
int NOTMSGI
;
idmsgin: 
int SC_RESEL
RESELECTED:
jump REL(ackanymsg)
;
;  If we suspect an intermediate disconnect is occuring, we will
;  come here an process the message in phase.  The SIOP will route
;  based on the message byte received.
messginloop:
clear ACK ; acknowledge the last byte, fall through to get next...
jump REL(messgin) when MSG_IN
clear ACK ; one more try
jump REL(messgin) when MSG_IN
jump REL(WHICHPHASE)
;
;
messgin:
move PH_MESSD to scratcha0
move memory 4, phythread, ncrdsa ; now restore the dsa
move FROM msgiptr, when MSG_IN
jump REL(parsemessgin)
;
parsemessgin:
jump REL(ackanddisc) if M_COMPLETE		; Command Complete
jump REL(messginloop) if M_SAVEPOINT	; expect interum disconn.
jump REL(idiscn) if M_DISCONNECT		; interum disconnect 
jump REL(rejected) if M_REJECT			; message reject received
jump REL(extended) if M_EXTENDED		; extended message request
jump REL(irpmsg) if M_RESTPOINT			; restore pointers message
int BADMSG								; wasn't disconnect message!
;
irpmsg:
clear ack ; accecpt restore pointers message
jump REL(output_data) when DATA_OUT	; data output
jump REL(input_data) when DATA_IN ; data input
jump REL(WHICHPHASE) 
;
; intermediate disconnect requested, inform CPU as soon as target clears
;
idiscn:
move 0x00 to scntl2 ; this will turn off unexpected disconnect bit
clear ACK
wait DISCONNECT
int SC_IDISC
;
; rejected message received
;
rejected:
int SC_MSGREJ ; inform driver of problem
clear ACK ; accept the rejection gracefully
jump REL(WHICHPHASE) 
;
; extended message arriving:
;	Here we must set up to receive a message of indeterminant length.
;   The way we do this is to get the size byte and save it into
;      a data frame for the SIOP.  We then switch context via the DSA
;      register and take in whatever the target is sending.  
;      When the message is in, we pass it along to the driver to decide
;      how to proceed.
;
;	NOTE:  ack is left asserted and as such we must clear it either at the
;          resumption point, or at the outset of our rejection of the message.
;	NOTE ALSO:  DSA register is destroyed prior to the interrupt to the driver,
;          and thus must be retored prior to reentry into the scripts.
;
extended:
clear ACK ; accept the extended message flag
move FROM msgiptr, when MSG_IN ; read in the size bytes
move sfbr to scratcha0 ; save in the scratcha0 register
move memory 1, ncrscratch0, extcntptr ; save in extended frame
jump REL(acksize) if not 0
move 1 to scratcha1
move memory 1, ncrscratch1,  extcntptr1 ; 0 := 0x100 bytes comming

acksize:
move memory 4, extmframeptr, ncrdsa ; extended message frame

clear ACK
move FROM extmsgcount, when MSG_IN ; read in the extended message
int SC_EXMSG ; wake the driver
clear ACK
jump REL(messgin) when MSG_IN ; in case sending another message in.
jump REL(WHICHPHASE) ; the message is understood, just proceed (resume point)
;
; send abort message and signal bus free at disconnect time.
;
ABORTMESSAGE:
abortmessage:
set ATN ; inform target of message about to be sent
clear ACK
move PH_MESSA to scratcha0 ; set flag for ourselves
move 1,  armsg_abort, when MSG_OUT ; send out the message
jump REL(WHICHPHASE) ; let the target decide what to do
;
; send a message reject to the target
;
REJECTMESSAGE:
set ATN  ; tell him we are about to transmit
clear ACK ; now tell him we have received the message
move 1,  armsg_reject, when MSG_OUT ; send the rejected message
jump REL(WHICHPHASE) ; let the target decide what to do
;
; send a message indicating message parity error 
PEMESSAGE:
set ATN  ; tell him we are about to transmit
clear ACK ; now tell him we have received the message
move 1, armsg_mpe, when MSG_OUT ; send the rejected message
jump REL(WHICHPHASE) ; let the target decide what to do

;
; send an arbitrary message
SENDMESSAGE:
set ATN
clear ACK
move FROM msgoptr, when MSG_OUT
jump REL(WHICHPHASE)

; some form of fatal error has been detected and as such we must free
;  the SCSI bus.  The target is in some undetermined state and as
;  such we try to fullfil the target's requirements.
;
erabortmsg:
set ATN ; inform target of message about to be sent
clear ACK
move PH_MESSA to scratcha0 ; set flag for ourselves
move 1,  armsg_abort, when MSG_OUT ; send out the message
jump REL(freeit_loop) 
;
; pad data out until target quits...
;    NOTE:  dsa destroyed!
erdout:
move 1, bitbucket, when DATA_OUT
jump REL(freeit_loop) when not DATA_OUT
jump REL(erdout)
;
; sink data from target until it gives up
; NOTE:  dsa destroyed!!
;
erdin:
move 1, bitbucket, when DATA_IN
jump REL(freeit_loop) when not DATA_IN
jump REL(erdin)
;
;  loop to free the bus
;
freeit_loop:
move memory 4, phythread, ncrdsa ; now restore the dsa

;
;  enter here for bus freedom movement.
;
FREEIT:
clear ack
set ATN
jump REL(erdout) when DATA_OUT
jump REL(erdin) when DATA_IN
jump REL(erabortmsg) when MSG_OUT
jump REL(messgin) when MSG_IN
jump REL(get_status) when STATUS
jump REL(WAITFORRESELECT)

TEST:
; ncrscratch3 ; 0x37
move memory 1, ncrscratch3,  extcntptr1 ; 0 := 0x100 bytes comming

; ncrscratch1 ; 0x35
move memory 1, ncrscratch1,  extcntptr1 ; 0 := 0x100 bytes comming

; ncrscratch0 ; 0x34
move memory 1, ncrscratch0,  extcntptr1 ; 0 := 0x100 bytes comming

; ncrdsa  ; 0x10
move memory 4, extmframeptr, ncrdsa ; extended message frame

; ncrlcrc ; 0x44
move memory 1, ncrlcrc, lcrcid_tid ; save reselector's ID.

; *** Script move SWIDE byte ENTRY
MOVE_SWIDE_BYTE:
CHMOV 1, RW_Last_di_byte_buf, WHEN DATA_IN
INT SC_SWIDEB



