;  
;   v167ll.ss - low level driver script for scsi710 driver.
;  

;**********************************************************************  
;*  Copyright 1991 by Microware Systems Corporation                   *
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
;   1  91/08/20  Rwb  Phase I script initial edits
;   2  91/08/30  Rwb  Added definitions of the Table Indirect
;                     Structure,  No patching nessesary at run time.
;                     ERR6 removed (not nessesary).
;   3  91/08/30  Rwb  Reincarnated ERR6 (phase mismatch) and added
;                     ERR8,  unknown phase after mismatch.
;   4  91/09/03  Rwb  Added reselect code to cover those devices that
;                     refuse to obey the standard and disconnect without
;                     being allowed the option (via Identify w/ disconnect).
;   5  91/09/04  Rwb  revised attempt at un-solicicted disconnect. (err9)
;   -  91/09/05  Rwb  <<< Phase I script frozen >>>
;   6  91/09/04  Rwb  first work on select with ATN (for disconnect)
;   7  91/09/10  Rwb  phase identification code in place.
;   8  91/09/12  Rwb  <<< Phase II script frozen >>>>
;	9  91/09/16  Rwb  Added select timeout flag clear routine.
;  10  91/09/18  Rwb  Learned some things about the internal workings
;                     of the DSP.  We now have a different flow to all
;                     operations.
;  11  91/09/19  Rwb  First cut of extended message handling
;  12  91/09/24  Rwb  Removed bug in reselect that could allow data to arrive
;                     before chip setup (synchronus)
;  13  91/09/30  Rwb  began bus lock recovery testing
;  14  91/11/03  Rwb  Added test for message phase after data transfer to
;                     to support devices that insist on disconnecting after
;                     data phase with no further data to transfer....
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


;   Message definitions
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

ABSOLUTE SYNC_REQEST       = 0x1	; Synchronus Transfer Negotiations
ABSOLUTE SYNCREQ_SIZE      = 0x3	; Size of additional bytes in Sync Req.

ABSOLUTE IDENTIFY_NODISCON = 0x80	; Identify message, no disconnect
ABSOLUTE IDENTIFY_DISCON   = 0xC0	; Identify message, disconnect allowed

;   We use the scratch register to determine flag certain phase information
;   to the CPU side of the driver.  These values appear in scratch0 of the
;   SIOP at certain times.
ABSOLUTE PH_MASK   = 0x0F  ; only least significant bits count 
ABSOLUTE PH_SELECT = 0x01  ; Attempting to select a device
ABSOLUTE PH_MESS   = 0x02  ; message phase (at connection)
ABSOLUTE PH_MESSD  = 0x12  ; message phase (which disconnect)
ABSOLUTE PH_MESSC  = 0x22  ; message phase (command complete)
ABSOLUTE PH_MESSA  = 0x03  ; message out -- abort
ABSOLUTE PH_CMD    = 0x04  ; command phase
ABSOLUTE PH_DATA   = 0x05  ; data phase in progress
ABSOLUTE PH_STATUS = 0x06  ; status phase
ABSOLUTE PH_DONE   = 0x00  ; not a special phase

;
; Absolute values are stored in DSPS Register for
; purposes of interrupt processing.  These values and their
; meaning must aggree between the CPU driver and SIOP script.

;     DSPS messages -- not of the error or problem variety 
ABSOLUTE SC_DONE   = 0x0080	;   command complete, target disconnected
ABSOLUTE SC_SIGP   = 0x0090	;   SIGP creates this when wait for reselect 
ABSOLUTE SC_RESEL  = 0x00A0	;   reselected 
ABSOLUTE SC_IDISC  = 0x00B0	;   intermediate disconnect received 
ABSOLUTE SC_EXMSG  = 0x00C0 ;   extended message received.
ABSOLUTE SC_MSGREJ = 0x00D0 ;	message reject received 

;    DSPS messages of the error variety (usually protocol violations) 
ABSOLUTE NOTMSGO = 0x0FF01		;   not message out after selection 
ABSOLUTE NOTCMD  = 0x0FF02		;   unexpected phase before command 
ABSOLUTE NOTSTAT = 0x0FF03		;   not status phase after data transfer 
ABSOLUTE MOTMSGI = 0x0FF04		;   not message in after status phase 
ABSOLUTE UNKNPH  = 0x0FF05		;   unidentified next phase 
ABSOLUTE BADMSG  = 0x0FF06		;   unidentified message in recieved 

ABSOLUTE SEL_DONE     = 0x0000	; value written to scratch(memory) to indicate
                                ;  selection phase complete.

;  NCR 53C710 register addresses used for the memory move commands
ABSOLUTE SIOPBASE    =  0xFFF47000    ; base address of SIOP in mVme167 map
ABSOLUTE NCRSCRATCH  = SIOPBASE+0x34  ; address of scratch register
ABSOLUTE NCRSCRATCH1 = SIOPBASE+0x36  ; address of scratch 1 register
ABSOLUTE NCRSCRATCH0 = SIOPBASE+0x37  ; address of scratch 0 register
ABSOLUTE NCRDSA      = SIOPBASE+0x10  ; data storage address
ABSOLUTE NCRLCRC     = SIOPBASE+0x20  ; for ID of reselector

;  These contents are used as masks for CTEST7 to enable and
;  disable the SELECT TIMOUT.
ABSOLUTE TimeOn = 0xEF
ABSOLUTE NoTime = 0x10

;   Entry points for the SIOP.  These are the various starting points
;   at which the CPU starts the SIOP.
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

;
; This is the SCSI SCRIPT itself.
;

;
;  Idle entry point,  no commands ready so hang out in case reselected.
;
WAITFORRESELECT:
move PH_DONE to scratch0
wait reselect REL(busfree)
jump REL(resel_adr)

;
;  Setting the SIGP will cause the wait above to fall into this 
;  peice of code.  The interrupt thus generated will cause us to
;  attempt activatation of a waiting command.
busfree:
int SC_SIGP

;
; select without attention
;  This routine will not assert attention at selection.  When the
;  path options do not have SCSI_ATN set, this is the entry point
;  that is used to start off the command.  Devices should not disconnect
;  as there is no message phase after selection with which to tell the
;  device that it is legal to disconnect.
;
SELECTNOATN:
move PH_SELECT to scratch0  ; indicate entering attempt to reach target
move ctest7 & TimeOn to ctest7 ; turn on the select timer
select FROM devfield, REL(resel_adr)
jump REL(snatn1) when CMD ; wait for phase change... must be command phase.
int NOTCMD

;  If selection does not timeout, we will arrive here.
snatn1:
move SEL_DONE to scratch3 ; status for the driver
move memory 1, NCRSCRATCH, PASS(extern statadr) ; to the flag location
move memory 4, PASS(extern phythread), NCRDSA ; now restore the dsa
move ctest7 | NoTime to ctest7
jump REL( send_cmd )

;
; select with attention
;	This entry point will assert attention with select.  This entry point
; is used when SCSI_ATN is set in the path options.  We also advertise that
; interrmediate disconnect is allowed during selection.
;
SELECTATN:
move PH_SELECT to scratch0  ; indicate entering attempt to reach target
move ctest7 & TimeOn to ctest7
select atn FROM devfield, REL(resel_adr)
jump REL(satn1) when MSG_OUT ; wait for selection ... must be message out
int NOTMSGO ; wake the driver with an error!

satn1:
move SEL_DONE to scratch3 ; status for the driver
move memory 1, NCRSCRATCH, PASS(extern statadr1) ; to the flag location
move memory 4, PASS(extern phythread1), NCRDSA ; now restore the dsa
move ctest7 | NoTime to ctest7

;
; Have entered message out phase...
;
move PH_MESS to scratch0 ; set phase flag 
move FROM msgoptr, when MSG_OUT ; send the i.d. message out to the target

;
;  Look for a message in phase.  Some devices will try to go synchronus
;  at the first time they see select with attention.  Under normal
;  circumstances however, we will have sent the negotiation request and
;  are about to receive his answer.  It is not an error if we don't do
;  this phase at this time.
jump REL(messgin) when MSG_IN ; message in requested

;
; Command phase
;
send_cmd:
move PH_CMD to scratch0 ; entered command phase when 
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
move PH_DATA to scratch0  ; indicate start of data phase
move FROM datacount, when DATA_IN ; transfer data
jump REL(messgin) if MSG_IN
jump REL(get_status)

;
; Data_Out phase operation.
;
output_data:
move PH_DATA to scratch0  ; indicate start of data phase
move FROM datacount, when DATA_OUT ; transfer data
jump REL(messgin) if MSG_IN
jump REL(get_status)

;
; Route operation based on next detected phase.
;
WHICHPHASE:
jump REL(get_status) when STATUS	; status phase
jump REL(messgin) when MSG_IN		; message in
jump REL(output_data) when DATA_OUT	; data output
jump REL(input_data) when DATA_IN	; data input
jump REL(send_cmd) when CMD         ; command phase
int UNKNPH                          ; unexpected next phase 

;
; Status Phase
;
GETSTATUS:
get_status:
move PH_STATUS to scratch0 ; have entered status phase
int NOTSTAT when not STATUS
move FROM statucount, when STATUS ;get status byte into memory

;
; Message In after status.
;
move PH_MESSC to scratch0 ; have entered the message in for command complete
int MOTMSGI when not MSG_IN ; complain if not message phase 
move FROM msgiptr, when MSG_IN ; read message byte.

;
; Resume point after message in, also logical conclusion to the
; normal flow of command.  When the SIOP reads data during message_in
; it will leave ACK asserted,  which effectivly locks the SCSI Bus until
; we have a chance to parse the message.
ackanddisc:
clear ack

;
; wait for a physical disconnect from the bus and inform driver when it
; occurs.
;
wait DISCONNECT
int SC_DONE

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
resel_adr:
move memory 1, NCRLCRC, PASS(extern lcrcid_tid) ; save reselector's ID.
move 1, PASS(extern lcrcid_idmsg), when MSG_IN ; get message from target
int SC_RESEL
RESELECTED:
clear ACK                       ; ack the byte in.
jump REL(WHICHPHASE)			; find out what's next

;
;  If we suspect an intermediate disconnect is occuring, we will
;  come here an process the message in phase.  The SIOP will route
;  based on the message byte received.
messginloop:
clear ACK ; acknowledge the last byte, fall through to get next...

messgin:
move PH_MESSD to scratch0
move FROM msgiptr, when MSG_IN
jump REL(ackanddisc) if M_COMPLETE    ; Command Complete
jump REL(messginloop) if M_SAVEPOINT  ; expect interum disconn.
jump REL(idiscn) if M_DISCONNECT      ; interum disconnect 
jump REL(rejected) if M_REJECT        ; message reject received
jump REL(extended) if M_EXTENDED      ; extended message request
jump REL(WHICHPHASE) if M_RESTPOINT	  ; this has already been done
int BADMSG                            ; wasn't disconnect message!

;
; intermediate disconnect requested, inform CPU as soon as target clears
;
idiscn:
clear ACK
wait DISCONNECT
int SC_IDISC

;
; rejected message received
;
rejected:
int SC_MSGREJ ; inform driver of problem
clear ACK ; accept the rejection gracefully
jump REL(WHICHPHASE) ; go on to next phase

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
move sfbr to scratch0 ; save in the scratch0 register
move memory 1, NCRSCRATCH0, PASS(extern extcntptr) ; save in extended frame
jump REL(acksize) if not 0
move 1 to scratch1
move memory 1, NCRSCRATCH1, PASS(extern extcntptr1) ; 0 := 0x100 bytes comming
acksize:
move memory 4, PASS(extern extmframeptr), NCRDSA ; extended message frame
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
move PH_MESSA to scratch0 ; set flag for ourselves
move 1, PASS(extern armsg_abort), when MSG_OUT ; send out the message
wait DISCONNECT ; should be disconnecting now
int SC_DONE ; inform the driver

;
; send a message reject to the target
REJECTMESSAGE:
set ATN  ; tell him we are about to transmit
clear ACK ; now tell him we have received the message
move 1, PASS(extern armsg_reject), when MSG_OUT ; send the rejected message
jump REL(WHICHPHASE) ; let the target decide what to do

;
; send a message indicating message parity error 
PEMESSAGE:
set ATN  ; tell him we are about to transmit
clear ACK ; now tell him we have received the message
move 1, PASS(extern armsg_mpe), when MSG_OUT ; send the rejected message
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

; pad data out until target quits...
;    NOTE:  dsa destroyed!
erdout:
move 1, PASS(extern bitbucket), when DATA_OUT
jump REL(freeit_loop) when not DATA_OUT
jump REL(erdout)

; sink data from target until it gives up
; NOTE:  dsa destroyed!!
erdin:
move 1, PASS(extern bitbucket1), when DATA_IN
jump REL(freeit_loop) when not DATA_IN
jump REL(erdin)

;  loop to free the bus
freeit_loop:
move memory 4, PASS(extern phythread2), NCRDSA ; now restore the dsa

;  enter here for bus freedom movement.
FREEIT:
set ATN
jump REL(erdout) when DATA_OUT
jump REL(erdin) when DATA_IN
jump REL(abortmessage) when MSG_OUT
jump REL(messgin) when MSG_IN
jump REL(get_status) when STATUS
int UNKNPH


