
****************************************
* Drive Static Storage Global variable definitions
*   Required in static storage of all SBF device drivers

*  Note: This does not reserve any memory for drive tables.
*        Each driver is responsible for reserving sufficient
*        memory for the appropriate number of tables.
 org V_USER reserve system required
 do.w 1 reserved (remove when V_USER is multiple of 4)
SBF_NDrv: do.w 1 number of drives
SBF_Flag: do.w 1 driver flags
SBF_Drvr: do.l 1 driver module ptr
SBF_DPrc: do.l 1 driver process ptr
SBF_IPrc: do.l 1 interrupt process ptr
 do.l 16 reserved
SBF_DTS: equ . beginning of driver specific storage

* SBF_Flag bit values
 org 0
nowork: do.b 1 driver working flag


* Global storage for tape drive tables.
 org 0
SBF_DFlg: do.w 1 drive flags
SBF_NBuf: do.w 1 buffer count
SBF_IBH: do.l 1 input buffer list head ptr
SBF_IBT: do.l 1 input buffer list tail ptr
SBF_OBH: do.l 1 output buffer list head ptr
SBF_OBT: do.l 1 output buffer list tail ptr
SBF_Wait: do.l 1 waiting process ptr
SBF_SErr: do.l 1 Number of recoverable errors occurred
SBF_HErr: do.l 1 Number of non-recoverable errors occurred
 do.l 8 reserved
SBF_DTES: equ . SBF drive table entry size

* SBF_DFlg bit values
 org 0
readflag: do.b 1 read mode flag
writflag: do.b 1 write mode flag
drivbusy: do.b 1 driver servicing this drive
EOFflag: do.b 1 at EOF flag


* Buffer structure
 org 0
bh_next: do.l 1 next buffer ptr
bh_siz: do.l 1 buffer size
bh_bcnt: do.l 1 byte count in buffer
bh_flag: do.w 1 flag word
bh_err: do.w 1 error code
bh_buff: equ . beginning of buffer

* bh_flag bit values
 org 0
BuffErr: do.b 1 buffer error flag
BuffFull: do.b 1 buffer full flag

