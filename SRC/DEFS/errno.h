/*---------------------------------------------------------------------------,
|                                                                            |
|              Copyright 2001 by Microware Systems Corporation.              |
|              All Rights Reserved.   Reproduced Under License.              |
|                                                                            |
|  This software is confidential property of Microware Systems Corporation,  |
|  and is provided under license for internal development purposes only.     |
|  Reproduction, publication, modification, distribution, or creation of     |
|  derivative works in any form to any party other than the Licensee is      |
|  strictly prohibited, unless expressly provided for under the terms of a   |
|  mutually executed written license agreement for this software between     |
|  the end-user and Microware Systems Corporation, or otherwise expressly    |
|  authorized in writing by Microware Systems Corporation.  Licensee shall   |
|  reproduce this copyright notice exactly as provided herein on any copies  |
|  of the software that Licensee is authorized to distribute.                |
|                                                                            |
|----------------------------------------------------------------------------|
|                                                                            |
| Edition History:                                                           |
| #   Date      Comments                                                By   |
| --- --------- ------------------------------------------------------- ---- |
|   1 17-Mar-92 Created.                                                rcb  |
|   2 24-Jun-92 Lots of changes for _ANSI_EXT pre-processor symbol      rry  |
|   3 29-Jul-92 New error #'s for ANSI, POSIX, and exceptions           rry  |
|   4 01-Sep-92 New wide area network errors                            rcb  |
|   5 29-Sep-93 Added errors that were in sys.l, but not errno.h        rcb  |
|   6 13-Feb-95 New conditionals for C++                                rcb  |
|   7 03-Jul-96 Fixed octal error messages.                             rry  |
|   8 20-Feb-98 Added this header.                                      nan  |
|   9 08-Jan-00 Merged spf.h errors and changed WANBASE to SPFBASE.     rds  |
|  10 11-Jan-00 Merged MAUI error codes from MAUI/maui_com.h.           srm  |
|  11 11-Feb-00 Added Reserved Value comment for errors below $20.      mgh  |
|  12 21-Feb-00 Thread support added                                rry/ajk  |
|  13 09-Mar-00 Added IEEE1394 error codes, updated copyright.          cdh  |
|     31-Mar-00 Revised MIPS predefined name usage to fit new MIPS      jej  |
|               nomenclature.                                                |
|  14 25-May-00 Mapped EAGAIN to EWOULDBLOCK                            rds  |
|  15 13-Feb-01 Adding additional reserved group numbers				rry  |
|  16 05-Mar-01 Adding x86 specific										jhd  |
|  17 05-Jun-01 Changed CMX to SH-5M									rry  |
`---------------------------------------------------------------------------*/

/* System service error codes which may be returned from a call
 * or may be found in 'errno' after an error has occurred.
 *
 * Group allocation:
 *
 * 000  -  OS-9 / OS-9000 operating system
 * 001  -  C compiler
 * 006  -  RAVE
 * 007  -  IFF / Ethernet
 * 008  -  SPF
 * 009  -  ROM
 * 010  -  MAUI
 * 011	-  OS Thread
 * 012  -  Application FrameWork library
 * 020  -  POSIX
 * 100  -  PowerPC processor specific codes
 * 101  -  SH-5M   processor specific codes
 * 102  -  MIPS    processor specific codes
 * 103  -  ARM     processor specific codes
 * 104  -  SH      processor specific codes
 * 106  -  x86     processor specific codes
 * 107  -  SPARC   processor specific codes
 */

#if !defined(_ERRNO_H)
#define _ERRNO_H

/*
 * NOTE: Error numbers 0x0 through 0x1F in Group 0 (zero)
 *       are reserved by Microware.
 */

#if !defined(__STDC__) && !defined(__cplusplus)
#define E_SIGABRT   0x20    /* abort signal                         */
#define E_SIGFPE    0x21    /* erroneous math operation signal      */
#define E_SIGILL    0x22    /* illegal function image signal        */
#define E_SIGSEGV   0x23    /* segment violation (bus error) signal */
#define E_SIGTERM   0x24    /* termination request signal           */
#define E_SIGALRM   0x25    /* alarm time elapsed signal            */
#define E_SIGPIPE   0x26    /* write to pipe with no readers signal */
#define E_SIGUSR1   0x27    /* user signal #1                       */
#define E_SIGUSR2   0x28    /* user signal #2                       */

#if defined(_MPF68K)
#define E_SIGADDR   0x29    /* address error signal                 */
#define E_SIGCHK    0x2a    /* chk instruction signal               */
#define E_SIGTRAPV  0x2b    /* trapv instruction signal             */
#define E_SIGPRIV   0x2c    /* priveledge violation signal          */
#define E_SIGTRACE  0x2d    /* trace exception signal               */
#define E_SIG1010   0x2e    /* 1010 line-A exception signal         */
#define E_SIG1111   0x2f    /* 1111 line-F exception signal         */

#if defined(_OS9000)
#define E_SIGCOPRCV 0x30    /* coprocessor protocol violation signal*/
#define E_SIGFMTERR 0x31    /* format error signal                  */
#define E_SIGUNIRQ  0x32    /* uninitialized interrupt signal       */
#endif  /* OS-9000/68020 */

#elif defined(_MPF386)
#define E_SIGGPROT  0x29    /* general protection signal            */
#define E_SIGSTACK  0x2a    /* stack exception signal               */
#define E_SIGSEGNP  0x2b    /* segment not present signal           */
#define E_SIGINVTSS 0x2c    /* invalid TSS signal                   */
#define E_SIGDBLFLT 0x2d    /* double fault signal                  */
#define E_SIGBNDCHK 0x2e    /* boundry check signal                 */
#define E_SIGBRKPT  0x2f    /* breakpoint signal                    */
#define E_SIGNMI    0x30    /* non-maskable interrupt signal        */
#define E_SIGDBG    0x31    /* debug exception signal               */

#elif defined(_MPFPOWERPC)
#define E_SIGCHECK  0x29    /* machine check signal                 */
#define E_SIGALIGN  0x2a    /* alignment violation signal           */
#define E_SIGINST   0x2b    /* instruction access signal            */
#define E_SIGPRIV   0x2c    /* privilege violation signal           */

#elif defined(_MPFARM)
#define E_SIGALIGN  0x29    /* alignment violation signal           */

#elif defined(_MPFSH3)
#define E_SIGALIGN  0x29    /* alignment violation signal           */

#elif defined(_MPFSPARC)
#define E_SIGALIGN      0x29    /* alignment violation signale      */
#define E_SIGWINDOWOV   0x30    /* window overflow signal           */
#define E_SIGWINDOWUF   0x31    /* window underflow signal          */
#define E_SIGTAGOV      0x32    /* tagged add/sub overflow signal   */
#define E_SIGCPE        0x33    /* coprocessor exception signal     */
#endif

#define E_ILLFNC    0x40    /* illegal function code                */
#define E_FMTERR    0x41    /* format error                         */
#define E_NOTNUM    0x42    /* number not found/not a number        */
#define E_ILLARG    0x43    /* illegal argument                     */

#if defined(_OS9000)
#define E_MEMINUSE  0x50    /* memory already in use */
#define E_UNKADDR   0x51    /* don't know how to translate this */
#endif /* _OS9000 */

#define E_BUSERR    0x66    /* bus error TRAP 2 occurred            */
#define E_ADRERR    0x67    /* address error TRAP 3 occurred        */
#define E_ILLINS    0x68    /* illegal instruction TRAP 4 occurred  */
#define E_ZERDIV    0x69    /* zero divide TRAP 5 occurred          */
#define E_CHK       0x6a    /* CHK instruction TRAP 6 occurred      */
#define E_TRAPV     0x6b    /* TrapV instruction TRAP 7 occurred    */
#define E_VIOLAT    0x6c    /* privelage violation TRAP 8 occurred  */
#define E_TRACE     0x6d    /* Uninitialized Trace TRAP 9 occurred  */
#define E_1010      0x6e    /* Uninitialized 1010 TRAP 10 occurred  */
#define E_1111      0x6f    /* Uninitialized 1111 TRAP 11 occurred  */
#define E_RESRVD    0x70    /* (Motorola reserved)                  */
#define E_CPROTO    0x71    /* Coprocessor protocol error           */
#define E_STKFMT    0x72    /* Stack frame format error             */
#define E_UNIRQ     0x73    /* Uninitialized interrupt              */
#define E_TRAP      0x85    /* user traps 1-15 E_TRAP+n             */
#define E_FPUNORDC  0x94    /* FP unordered condition error         */
#define E_FPINXACT  0x95    /* FP inexact result                    */
#define E_FPDIVZER  0x96    /* FP zero divisor                      */
#define E_FPUNDRFL  0x97    /* FP underflow                         */
#define E_FPOPRERR  0x98    /* FP operand error                     */
#define E_FPOVERFL  0x99    /* FP overflow                          */
#define E_FPNOTNUM  0x9a    /* FP not a number                      */
#define E_FPUNDATA  0x9b    /* FP Unsupported data type             */
#define E_MMUCONF   0x9c    /* MMU configuration error              */
#define E_MMUILLEG  0x9d    /* MMU illegal operation                */
#define E_MMUACCES  0x9e    /* MMU access level violation           */

#define E_PERMIT    0xa4    /* You must be super user to do that    */
#define E_DIFFER    0xa5    /* Arguments to F$ChkNam are different  */
#define E_STKOVF    0xa6    /* Stack overflow (ChkNam)              */
#define E_EVNTID    0xa7    /* Invalid or Illegal event ID number   */
#define E_EVNF      0xa8    /* Event (name) not found               */
#define E_EVBUSY    0xa9    /* The event is busy (can't be deleted) */
#define E_EVPARM    0xaa    /* Impossible Event parameters supplied */
#define E_DAMAGE    0xab    /* System data structures are damaged   */
#define E_BADREV    0xac    /* Incompatible revision                */
#define E_PTHLOST   0xad    /* Path Lost (net node was down)        */
#define E_BADPART   0xae    /* Bad partition data or no active      */
                            /* partition                            */
#define E_HARDWARE  0xaf    /* Hardware is damaged                  */

#if defined(_OSK)
#define E_SECTSIZE  0xb0    /* Invalid sector size                  */
#define E_BSIG      0xb1    /* process recieved signal fatal signal or no intercept */
#elif defined(_OS9000)
#define E_NOTME     0xb0    /* not responsible for servicing exception */
#define E_BSIG      0xb1    /* process recieved signal fatal signal or no intercept */
#define E_BUF2SMALL 0xb2    /* buffer is too small                  */
#define E_ISUB      0xb3    /* illegal/used subroutine module number*/
#define E_EVTFUL    0xb4    /* event descriptor table full          */

#define E_SYMLINK   0xc4    /* symbolic link found in path list     */
#define E_EOLIST    0xc5    /* end of alias list                    */
#define E_LOCKID    0xc6    /* illegal I/O lock identifier          */
#define E_NOLOCK    0xc7    /* lock not obtained                    */
#endif /* _OS9000 */

#define E_PTHFUL    0xc8    /* Path Table full                      */
#define E_BPNUM     0xc9    /* Bad Path Number                      */
#define E_POLL      0xca    /* Polling Table Full                   */
#define E_BMODE     0xcb    /* Bad Mode                             */
#define E_DEVOVF    0xcc    /* Device Table Overflow                */
#define E_BMID      0xcd    /* Bad Module ID                        */
#define E_DIRFUL    0xce    /* Module Directory Full                */
#define E_MEMFUL    0xcf    /* Process Memory Full                  */
#define E_UNKSVC    0xd0    /* Unknown Service Code                 */
#define E_MODBSY    0xd1    /* Module Busy                          */
#define E_BPADDR    0xd2    /* Bad Page Address                     */
#define E_EOF       0xd3    /* End of File                          */
#define E_VCTBSY    0xd4    /* IRQ Vector is already used           */
#define E_NES       0xd5    /* Non-Existing Segment                 */
#define E_FNA       0xd6    /* File Not Accesible                   */
#define E_BPNAM     0xd7    /* Bad Path Name                        */
#define E_PNNF      0xd8    /* Path Name Not Found                  */
#define E_SLF       0xd9    /* Segment List Full                    */
#define E_CEF       0xda    /* Creating Existing File               */
#define E_IBA       0xdb    /* Illegal Block Address                */
#define E_HANGUP    0xdc    /* Phone hangup occurred (modem)        */
#define E_MNF       0xdd    /* Module Not Found                     */
#define E_NOCLK     0xde    /* System has no clock running          */
#define E_DELSP     0xdf    /* Deleting Stack Pointer memory        */
#define E_IPRCID    0xe0    /* Illegal Process ID                   */
#define E_PARAM     0xe1    /* Impossible polling parameter specified*/
#define E_NOCHLD    0xe2    /* No Children                          */
#define E_ITRAP     0xe3    /* Invalid or unavailable Trap code     */
#define E_PRCABT    0xe4    /* Process Aborted                      */
#define E_PRCFUL    0xe5    /* Process Table Full                   */
#define E_IFORKP    0xe6    /* Illegal Fork Parameter               */
#define E_KWNMOD    0xe7    /* Known Module                         */
#define E_BMCRC     0xe8    /* Bad Module CRC                       */

#if defined(_OSK)
#define E_USIGP     0xe9    /* Unprocessed Signal Pending (obsolescent)*/
#endif /* _OSK */

#define E_SIGNAL    0xe9    /* Signal Error (replaces E_USIGP)      */
#define E_NEMOD     0xea    /* Not Executable Module                */
#define E_BNAM      0xeb    /* Bad Name                             */
#define E_BMHP      0xec    /* bad module header parity             */
#define E_NORAM     0xed    /* No Ram Available                     */
#define E_DNE       0xee    /* Directory not empty                  */
#define E_NOTASK    0xef    /* No available Task number             */
#define E_UNIT      0xf0    /* Illegal Unit (drive)                 */
#define E_SECT      0xf1    /* Bad SECTor number                    */
#define E_WP        0xf2    /* Write Protect                        */
#define E_CRC       0xf3    /* Bad Check Sum                        */
#define E_READ      0xf4    /* Read Error                           */
#define E_WRITE     0xf5    /* Write Error                          */
#define E_NOTRDY    0xf6    /* Device Not Ready                     */
#define E_SEEK      0xf7    /* Seek Error                           */
#define E_FULL      0xf8    /* Media Full                           */
#define E_BTYP      0xf9    /* Bad Type (incompatible) media        */
#define E_DEVBSY    0xfa    /* Device Busy                          */
#define E_DIDC      0xfb    /* Disk ID Change                       */
#define E_LOCK      0xfc    /* Record is busy (locked out)          */
#define E_SHARE     0xfd    /* Non-sharable file busy               */
#define E_DEADLK    0xfe    /* I/O Deadlock error                   */
#define E_FORMAT    0xff    /* Device is format protected           */

/* 0x600 through 0x6ff RAVE */
#if defined(_OSK)
#define E_GFM_BASE  0x600               /* GFM error code base */
#elif defined(_OS9000)
#define E_GFM_BASE  0x60000             /* GFM error code base */
#endif /* _OS9000 */
#define E_ILLPRM    (E_GFM_BASE+0)      /* illegal parameter */
#define E_IDFULL    (E_GFM_BASE+1)      /* identifier table full */
#define E_BADSIZ    (E_GFM_BASE+2)      /* bad size error */
#define E_RGFULL    (E_GFM_BASE+3)      /* region definition full */
#define E_UNID      (E_GFM_BASE+4)      /* unallocated identifier number */
#define E_NULLRG    (E_GFM_BASE+5)      /* null region */
#define E_BADMOD    (E_GFM_BASE+6)      /* bad drawmap / pattern mode */
#define E_NOFONT    (E_GFM_BASE+7)      /* no active font */
#define E_NODM      (E_GFM_BASE+8)      /* no drawmap */
#define E_NOPLAY    (E_GFM_BASE+9)      /* no play/record is in progress */
#define E_ABORT     (E_GFM_BASE+10)     /* marks a play/record as aborted */
#define E_QFULL     (E_GFM_BASE+11)     /* audio queue is full */
#define E_BUSY      (E_GFM_BASE+12)     /* audio device is busy */

#if defined(_OSK)
#define E_GSL_BASE      0x664           /* GSL error code base */
#elif defined(_OS9000)
#define E_GSL_BASE      0x60064         /* GSL error code base */
#endif /* _OS9000 */
#define E_RES_BASE      (E_GSL_BASE+0)  /* errors for resource manager   */
#define E_RES_NOSLOT    (E_RES_BASE+0)  /* No free slot in the table     */
#define E_RES_BADSLOT   (E_RES_BASE+1)  /* The slot is bad               */
#define E_RES_NOSHARE   (E_RES_BASE+2)  /* The resource is not sharable  */
#define E_RES_NOTYPE    (E_RES_BASE+3)  /* The type is bad               */
#define E_RES_NORES     (E_RES_BASE+4)  /* The ID of the resource is bad */

#define E_REQ_BASE      (E_GSL_BASE+10) /* errors for request manager    */
#define E_REQ_NOITEMS   (E_REQ_BASE+0)  /* request has no items          */
#define E_REQ_BADITEM   (E_REQ_BASE+1)  /* item number out of range      */
#define E_REQ_BADCOLS   (E_REQ_BASE+2)  /* request has bad num. columns  */
#define E_REQ_BADPTR    (E_REQ_BASE+3)  /* bad pointer for item array    */
#define E_REQ_NOCREATE  (E_REQ_BASE+4)  /* request hasn't been created   */
#define E_REQ_TIMEOUT   (E_REQ_BASE+5)  /* request has timed out         */
#define E_REQ_NOSEL     (E_REQ_BASE+6)  /* no selection has been made    */
#define E_REQ_DEFID     (E_REQ_BASE+7)  /* bad definition id             */
#define E_REQ_DEFACT    (E_REQ_BASE+8)  /* bad definition action code    */
#define E_REQ_STATE     (E_REQ_BASE+9)  /* illegal item state            */
#define E_REQ_BADRECT   (E_REQ_BASE+10) /* illegal request rectangle     */

#define E_CNT_BASE      (E_GSL_BASE+30) /* errors for control manager */
#define E_CNT_BHVID     (E_CNT_BASE+0)  /* bad standard behavior id */
#define E_CNT_DEFID     (E_CNT_BASE+1)  /* bad standard definition id */
#define E_CNT_DEFACT    (E_CNT_BASE+2)  /* bad action for definition function */
#define E_CNT_BHVACT    (E_CNT_BASE+3)  /* bad action for behavior function */
#define E_CNT_STATE     (E_CNT_BASE+4)  /* bad control state */
#define E_CNT_PART      (E_CNT_BASE+5)  /* bad control part code */
#define E_CNT_FLAGS     (E_CNT_BASE+6)  /* bad flags */
#define E_CNT_MINMAX    (E_CNT_BASE+7)  /* bad min, bad max or value out of range */
#define E_CNT_TYPE      (E_CNT_BASE+8)  /* bad type of control */

#define E_CLIP_BASE (E_GSL_BASE+40) /* errors for clipboard manager */
#define E_CLIP_DEV  (E_CLIP_BASE+0) /* can't find clipboard default device from preferences */
#define E_CLIP_FULL (E_CLIP_BASE+1) /* clipboard is full (attempt to write too many different types in clipboard) */
#define E_CLIP_TYPE (E_CLIP_BASE+2) /* type not currently represented in clipboard */
#define E_CLIP_ACC  (E_CLIP_BASE+3) /* clipboard not opened for the requested access */
#define E_CLIP_CNT  (E_CLIP_BASE+4) /* type offset greater than type count */
#define E_CLIP_OPEN (E_CLIP_BASE+5) /* clipboard not currently opened */
#define E_CLIP_INIT (E_CLIP_BASE+6) /* clipboard not initialized */
#define E_CLIP_CLOSE    (E_CLIP_BASE+7) /* clipboard is not currently closed */
#define E_CLIP_RW   (E_CLIP_BASE+8) /* can't rewrite - type not represented in clipboard */

#define E_HNDLR_BASE    (E_GSL_BASE+50) /* errors for dispatch manager */
#define E_HNDLR_UNKNOWN (E_HNDLR_BASE+0)

#define E_ATBL_BASE     (E_GSL_BASE+55) /* errors for association table routines */
#define E_ATBL_NOENTRY  (E_ATBL_BASE+0) /* no entry found */

#define E_BOX_BASE      (E_GSL_BASE+60)     /* errors for ed_box function  */
#define E_BOX_TABLE     (E_BOX_BASE+0)      /* line table overflow */
#define E_BOX_COUNT     (E_BOX_BASE+1)      /* text too long (max = 65535 characters) */
#define E_BOX_TYPE      (E_BOX_BASE+2)      /* bad type or type not implemented */
#define E_BOX_MAXL      (E_BOX_BASE+3)      /* attempt to draw too long line (max is 1024 chars) */
#define E_BOX_NOTAB     (E_BOX_BASE+4)      /* need a line table (if the BOX_F_USETAB flag is set in the "type" parameter) */
#define E_BOX_NOFONT    (E_BOX_BASE+5)      /* font not set in the drawmap */
#define E_BOX_RECT      (E_BOX_BASE+6)      /* bad rectangle */

#define E_INIT_BASE     (E_GSL_BASE+80)     /* errors for init function */
#define E_INIT_VARERR   (E_INIT_BASE+0)

#define E_INTER_BASE    (E_GSL_BASE+85)
#define E_INTER_NOMOD   (E_INTER_BASE+0)    /* no preference module */
#define E_INTER_ILLARG  (E_INTER_BASE+1)    /* illegal arguement */

#define E_OVL_BASE      (E_GSL_BASE+90)     /* overlay window errors */
#define E_OVL_BADRECT   (E_OVL_BASE+0)      /* bad rectangle for overlay */
#define E_OVL_NOTTOP    (E_OVL_BASE+1)      /* not top of overlay stack */
#define E_OVL_UNKNOWN   (E_OVL_BASE+2)      /* unknown overlay */

#define E_IND_BASE      (E_GSL_BASE+100)    /* indicator manager errors */
#define E_IND_DEFID     (E_IND_BASE+0)      /* bad definition id */
#define E_IND_DEFACT    (E_IND_BASE+1)      /* bad action for def. function */
#define E_IND_MINMAX    (E_IND_BASE+2)      /* bad min, max or value out of range */
#define E_IND_BADCORDS  (E_IND_BASE+3)      /* bad coordinates for indicator */
#define E_IND_NOCREATE  (E_IND_BASE+4)      /* attempt op. on non-created ind. */
#define E_IND_BADFLAGS  (E_IND_BASE+5)      /* bad flag combination */
#define E_IND_BADPTR    (E_IND_BASE+6)      /* bad pointer in structure */
#define E_IND_BADDISP   (E_IND_BASE+7)      /* bad displacement */

#if !defined(E_IFF_BASE)
#if defined(_OSK)
/* 0x700 through 0x7ff IFF */
#define E_IFF_BASE          ((7<<8)+000)    /* base error code for IFF */
#elif defined(_OS9000)
/* 0x70000 through 0x700ff IFF */
#define E_IFF_BASE          ((7<<16)+000)   /* base error code for IFF */
#endif /* _OS9000 */
#define E_IFF_RONLY         (E_IFF_BASE+1)  /* this path is read-only */
#define E_IFF_WONLY         (E_IFF_BASE+2)  /* this path is write-only */
#define E_IFF_ACTFORM       (E_IFF_BASE+3)  /* there is no FORM active */
#define E_IFF_READER        (E_IFF_BASE+4)  /* wrong reader for this type of FORM */
#define E_IFF_NOTIFF        (E_IFF_BASE+5)  /* not an IFF file */
#define E_IFF_BADPARM       (E_IFF_BASE+6)  /* bad parameters */
#define E_IFF_BADCAT        (E_IFF_BASE+7)  /* bad CAT id (for iff_open) */
#define E_IFF_SIZE_UNKNOWN  (E_IFF_BASE+8)  /* cannot skip, size is unknown */
#define E_IFF_NOT_DATA      (E_IFF_BASE+9)  /* not to the data yet */
#define E_IFF_PIPE_SEEK     (E_IFF_BASE+10) /* attempt to seek back in a pipe */
#define E_IFF_BADCHUNKSIZE  (E_IFF_BASE+11) /* fixed size chunk was not the 
                                                correct size.  Could indicate 
                                                wrong version of reader */
#define E_IFF_FPNOTNUM      (E_IFF_BASE+12) /* can't make float conversion */
#endif /* E_IFF_BASE */

#endif /* __STDC__ */

#define EOS_SIGABRT 0x20    /* abort signal                         */
#define EOS_SIGFPE  0x21    /* erroneous math operation signal      */
#define EOS_SIGILL  0x22    /* illegal function image signal        */
#define EOS_SIGSEGV 0x23    /* segment violation (bus error) signal */
#define EOS_SIGTERM 0x24    /* termination request signal           */
#define EOS_SIGALRM 0x25    /* alarm time elapsed signal            */
#define EOS_SIGPIPE 0x26    /* write to pipe with no readers signal */
#define EOS_SIGUSR1 0x27    /* user signal #1                       */
#define EOS_SIGUSR2 0x28    /* user signal #2                       */

#if defined(_MPF68K)
#define EOS_SIGADDR     0x29    /* address error signal                 */
#define EOS_SIGCHK      0x2a    /* chk instruction signal               */
#define EOS_SIGTRAPV    0x2b    /* trapv instruction signal             */
#define EOS_SIGPRIV     0x2c    /* priveledge violation signal          */
#define EOS_SIGTRACE    0x2d    /* trace exception signal               */
#define EOS_SIG1010     0x2e    /* 1010 line-A exception signal         */
#define EOS_SIG1111     0x2f    /* 1111 line-F exception signal         */

#if defined(_OS9000)
#define EOS_SIGCOPRCV   0x30    /* coprocessor protocol violation signal*/
#define EOS_SIGFMTERR   0x31    /* format error signal                  */
#define EOS_SIGUNIRQ    0x32    /* uninitialized interrupt signal       */
#endif  /* OS-9000/68020 */

#elif defined(_MPF386)
#define EOS_SIGGPROT    0x29    /* general protection signal            */
#define EOS_SIGSTACK    0x2a    /* stack exception signal               */
#define EOS_SIGSEGNP    0x2b    /* segment not present signal           */
#define EOS_SIGINVTSS   0x2c    /* invalid TSS signal                   */
#define EOS_SIGDBLFLT   0x2d    /* double fault signal                  */
#define EOS_SIGBNDCHK   0x2e    /* boundry check signal                 */
#define EOS_SIGBRKPT    0x2f    /* breakpoint signal                    */
#define EOS_SIGNMI      0x30    /* non-maskable interrupt signal        */
#define EOS_SIGDBG      0x31    /* debug exception signal               */

#elif defined(_MPFPOWERPC)
#define EOS_SIGCHECK    0x29    /* machine check signal                 */
#define EOS_SIGALIGN    0x2a    /* alignment violation signal           */
#define EOS_SIGINST     0x2b    /* instruction access signal            */
#define EOS_SIGPRIV     0x2c    /* privilege violation signal           */
#endif

#define EOS_ILLFNC  0x40    /* illegal function code                */
#define EOS_FMTERR  0x41    /* format error                         */
#define EOS_NOTNUM  0x42    /* number not found/not a number        */
#define EOS_ILLARG  0x43    /* illegal argument                     */

#if defined(_OS9000)
#define EOS_MEMINUSE 0x50   /* memory already in use */
#define EOS_UNKADDR 0x51    /* don't know how to translate this */
#endif /* _OS9000 */

#define EOS_BUSERR  0x66    /* bus error TRAP 2 occurred            */
#define EOS_ADRERR  0x67    /* address error TRAP 3 occurred        */
#define EOS_ILLINS  0x68    /* illegal instruction TRAP 4 occurred  */
#define EOS_ZERDIV  0x69    /* zero divide TRAP 5 occurred          */
#define EOS_CHK     0x6a    /* CHK instruction TRAP 6 occurred      */
#define EOS_TRAPV   0x6b    /* TrapV instruction TRAP 7 occurred    */
#define EOS_VIOLAT  0x6c    /* privelage violation TRAP 8 occurred  */
#define EOS_TRACE   0x6d    /* Uninitialized Trace TRAP 9 occurred  */
#define EOS_1010    0x6e    /* Uninitialized 1010 TRAP 10 occurred  */
#define EOS_1111    0x6f    /* Uninitialized 1111 TRAP 11 occurred  */
#define EOS_RESRVD  0x70    /* (Motorola reserved)                  */
#define EOS_CPROTO  0x71    /* Coprocessor protocol error           */
#define EOS_STKFMT  0x72    /* Stack frame format error             */
#define EOS_UNIRQ   0x73    /* Uninitialized interrupt              */
#define EOS_TRAP    0x85    /* user traps 1-15 EOS_TRAP+n               */
#define EOS_FPUNORDC    0x94    /* FP unordered condition error         */
#define EOS_FPINXACT    0x95    /* FP inexact result                    */
#define EOS_FPDIVZER    0x96    /* FP zero divisor                      */
#define EOS_FPUNDRFL    0x97    /* FP underflow                         */
#define EOS_FPOPRERR    0x98    /* FP operand error                     */
#define EOS_FPOVERFL    0x99    /* FP overflow                          */
#define EOS_FPNOTNUM    0x9a    /* FP not a number                      */
#define EOS_FPUNDATA    0x9b    /* FP Unsupported data type             */
#define EOS_MMUCONF     0x9c    /* MMU configuration error              */
#define EOS_MMUILLEG    0x9d    /* MMU illegal operation                */
#define EOS_MMUACCES    0x9e    /* MMU access level violation           */
#define EOS_UEA         0xa0    /* unimplemented effective address (68060) */
#define EOS_UII         0xa1    /* unimplemented integer instruction (68060) */

#define EOS_PERMIT  0xa4    /* You must be super user to do that    */
#define EOS_DIFFER  0xa5    /* Arguments to F$ChkNam are different  */
#define EOS_STKOVF  0xa6    /* Stack overflow (ChkNam)              */
#define EOS_EVNTID  0xa7    /* Invalid or Illegal event ID number   */
#define EOS_EVNF    0xa8    /* Event (name) not found               */
#define EOS_EVBUSY  0xa9    /* The event is busy (can't be deleted) */
#define EOS_EVPARM  0xaa    /* Impossible Event parameters supplied */
#define EOS_DAMAGE  0xab    /* System data structures are damaged   */
#define EOS_BADREV  0xac    /* Incompatible revision                */
#define EOS_PTHLOST 0xad    /* Path Lost (net node was down)        */
#define EOS_BADPART 0xae    /* Bad partition data or no active      */
                            /* partition                            */
#define EOS_HARDWARE    0xaf    /* Hardware is damaged                  */

#if defined(_OSK)
#define EOS_SECTSIZE    0xb0    /* Invalid sector size                  */
#define EOS_BSIG        0xb1    /* process recieved signal fatal signal or no intercept */
#elif defined(_OS9000)
#define EOS_NOTME       0xb0    /* not responsible for servicing exception */
#define EOS_BSIG        0xb1    /* process recieved signal fatal signal or no intercept */
#define EOS_BUF2SMALL   0xb2    /* buffer is too small                  */
#define EOS_ISUB        0xb3    /* illegal/used subroutine module number*/
#define EOS_EVTFUL      0xb4    /* event descriptor table full          */

#define EOS_SYMLINK 0xc4    /* symbolic link found in path list     */
#define EOS_EOLIST  0xc5    /* end of alias list                    */
#define EOS_LOCKID  0xc6    /* illegal I/O lock identifier          */
#define EOS_NOLOCK  0xc7    /* lock not obtained                    */
#endif /* _OS9000 */

#define EOS_PTHFUL  0xc8    /* Path Table full                      */
#define EOS_BPNUM   0xc9    /* Bad Path Number                      */
#define EOS_POLL    0xca    /* Polling Table Full                   */
#define EOS_BMODE   0xcb    /* Bad Mode                             */
#define EOS_DEVOVF  0xcc    /* Device Table Overflow                */
#define EOS_BMID    0xcd    /* Bad Module ID                        */
#define EOS_DIRFUL  0xce    /* Module Directory Full                */
#define EOS_MEMFUL  0xcf    /* Process Memory Full                  */
#define EOS_UNKSVC  0xd0    /* Unknown Service Code                 */
#define EOS_MODBSY  0xd1    /* Module Busy                          */
#define EOS_BPADDR  0xd2    /* Bad Page Address                     */
#define EOS_EOF     0xd3    /* End of File                          */
#define EOS_VCTBSY  0xd4    /* IRQ Vector is already used           */
#define EOS_NES     0xd5    /* Non-Existing Segment                 */
#define EOS_FNA     0xd6    /* File Not Accesible                   */
#define EOS_BPNAM   0xd7    /* Bad Path Name                        */
#define EOS_PNNF    0xd8    /* Path Name Not Found                  */
#define EOS_SLF     0xd9    /* Segment List Full                    */
#define EOS_CEF     0xda    /* Creating Existing File               */
#define EOS_IBA     0xdb    /* Illegal Block Address                */
#define EOS_HANGUP  0xdc    /* Phone hangup occurred (modem)        */
#define EOS_MNF     0xdd    /* Module Not Found                     */
#define EOS_NOCLK   0xde    /* System has no clock running          */
#define EOS_DELSP   0xdf    /* Deleting Stack Pointer memory        */
#define EOS_IPRCID  0xe0    /* Illegal Process ID                   */
#define EOS_PARAM   0xe1    /* Impossible polling parameter specified*/
#define EOS_NOCHLD  0xe2    /* No Children                          */
#define EOS_ITRAP   0xe3    /* Invalid or unavailable Trap code     */
#define EOS_PRCABT  0xe4    /* Process Aborted                      */
#define EOS_PRCFUL  0xe5    /* Process Table Full                   */
#define EOS_IFORKP  0xe6    /* Illegal Fork Parameter               */
#define EOS_KWNMOD  0xe7    /* Known Module                         */
#define EOS_BMCRC   0xe8    /* Bad Module CRC                       */

#if defined(_OSK)
#define EOS_USIGP   0xe9    /* Unprocessed Signal Pending (obsolescent)*/
#endif /* _OSK */

#define EOS_SIGNAL  0xe9    /* Signal Error (replaces EOS_USIGP)    */
#define EOS_NEMOD   0xea    /* Not Executable Module                */
#define EOS_BNAM    0xeb    /* Bad Name                             */
#define EOS_BMHP    0xec    /* bad module header parity             */
#define EOS_NORAM   0xed    /* No Ram Available                     */
#define EOS_DNE     0xee    /* Directory not empty                  */
#define EOS_NOTASK  0xef    /* No available Task number             */
#define EOS_UNIT    0xf0    /* Illegal Unit (drive)                 */
#define EOS_SECT    0xf1    /* Bad SECTor number                    */
#define EOS_WP      0xf2    /* Write Protect                        */
#define EOS_CRC     0xf3    /* Bad Check Sum                        */
#define EOS_READ    0xf4    /* Read Error                           */
#define EOS_WRITE   0xf5    /* Write Error                          */
#define EOS_NOTRDY  0xf6    /* Device Not Ready                     */
#define EOS_SEEK    0xf7    /* Seek Error                           */
#define EOS_FULL    0xf8    /* Media Full                           */
#define EOS_BTYP    0xf9    /* Bad Type (incompatible) media        */
#define EOS_DEVBSY  0xfa    /* Device Busy                          */
#define EOS_DIDC    0xfb    /* Disk ID Change                       */
#define EOS_LOCK    0xfc    /* Record is busy (locked out)          */
#define EOS_SHARE   0xfd    /* Non-sharable file busy               */
#define EOS_DEADLK  0xfe    /* I/O Deadlock error                   */
#define EOS_FORMAT  0xff    /* Device is format protected           */

#if defined(_OSK)
/* 0x100 is for C compiler */
#define ERANGE      0x100   /* ANSI C number out of range error     */
#define EDOM        0x101   /* ANSI C number not in domain          */
#endif /* _OSK */

#if defined(_OS9000)
#define ERANGE      0x10000 /* ANSI C number out of range error     */
#define EDOM        0x10001 /* ANSI C number not in domain          */
#endif /* _OS9000 */

/* ----------------------- RAVE Specific Section ----------------------------*/
/* 0x600 through 0x6ff RAVE */
#if defined(_OSK)
#define EOS_GFM_BASE    0x600               /* GFM error code base */
#elif defined(_OS9000)
#define EOS_GFM_BASE    0x60000             /* GFM error code base */
#endif /* _OS9000 */
#define EOS_ILLPRM      (EOS_GFM_BASE+0)    /* illegal parameter              */
#define EOS_IDFULL      (EOS_GFM_BASE+1)    /* identifier table full          */
#define EOS_BADSIZ      (EOS_GFM_BASE+2)    /* bad size error                 */
#define EOS_RGFULL      (EOS_GFM_BASE+3)    /* region definition full         */
#define EOS_UNID        (EOS_GFM_BASE+4)    /* unallocated identifier number  */
#define EOS_NULLRG      (EOS_GFM_BASE+5)    /* null region                    */
#define EOS_BADMOD      (EOS_GFM_BASE+6)    /* bad drawmap / pattern mode     */
#define EOS_NOFONT      (EOS_GFM_BASE+7)    /* no active font                 */
#define EOS_NODM        (EOS_GFM_BASE+8)    /* no drawmap                     */
#define EOS_NOPLAY      (EOS_GFM_BASE+9)    /* no play/record is in progress  */
#define EOS_ABORT       (EOS_GFM_BASE+10)   /* marks a play/record as aborted */
#define EOS_QFULL       (EOS_GFM_BASE+11)   /* audio queue is full            */
#define EOS_BUSY        (EOS_GFM_BASE+12)   /* audio device is busy           */

#if defined(_OSK)
#define EOS_GSL_BASE        0x664           /* GSL error code base */
#elif defined(_OS9000)
#define EOS_GSL_BASE        0x60064         /* GSL error code base */
#endif /* _OS9000 */
#define EOS_RES_BASE      (EOS_GSL_BASE+0)  /* errors for resource manager    */
#define EOS_RES_NOSLOT    (EOS_RES_BASE+0)  /* No free slot in the table      */
#define EOS_RES_BADSLOT   (EOS_RES_BASE+1)  /* The slot is bad                */
#define EOS_RES_NOSHARE   (EOS_RES_BASE+2)  /* The resource is not sharable   */
#define EOS_RES_NOTYPE    (EOS_RES_BASE+3)  /* The type is bad                */
#define EOS_RES_NORES     (EOS_RES_BASE+4)  /* The ID of the resource is bad  */
 
#define EOS_REQ_BASE      (EOS_GSL_BASE+10) /* errors for request manager     */
#define EOS_REQ_NOITEMS   (EOS_REQ_BASE+0)  /* request has no items           */
#define EOS_REQ_BADITEM   (EOS_REQ_BASE+1)  /* item number out of range       */
#define EOS_REQ_BADCOLS   (EOS_REQ_BASE+2)  /* request has bad num. columns   */
#define EOS_REQ_BADPTR    (EOS_REQ_BASE+3)  /* bad pointer for item array     */
#define EOS_REQ_NOCREATE  (EOS_REQ_BASE+4)  /* request hasn't been created    */
#define EOS_REQ_TIMEOUT   (EOS_REQ_BASE+5)  /* request has timed out          */
#define EOS_REQ_NOSEL     (EOS_REQ_BASE+6)  /* no selection has been made     */
#define EOS_REQ_DEFID     (EOS_REQ_BASE+7)  /* bad definition id              */
#define EOS_REQ_DEFACT    (EOS_REQ_BASE+8)  /* bad definition action code     */
#define EOS_REQ_STATE     (EOS_REQ_BASE+9)  /* illegal item state             */
#define EOS_REQ_BADRECT   (EOS_REQ_BASE+10) /* illegal request rectangle      */

#define EOS_CNT_BASE      (EOS_GSL_BASE+30) /* errors for control manager     */
#define EOS_CNT_BHVID     (EOS_CNT_BASE+0)  /* bad standard behavior id       */
#define EOS_CNT_DEFID     (EOS_CNT_BASE+1)  /* bad standard definition id     */
#define EOS_CNT_DEFACT    (EOS_CNT_BASE+2)  /* bad action for definition func */
#define EOS_CNT_BHVACT    (EOS_CNT_BASE+3)  /* bad action for behavior func   */
#define EOS_CNT_STATE     (EOS_CNT_BASE+4)  /* bad control state              */
#define EOS_CNT_PART      (EOS_CNT_BASE+5)  /* bad control part code          */
#define EOS_CNT_FLAGS     (EOS_CNT_BASE+6)  /* bad flags                      */
#define EOS_CNT_MINMAX    (EOS_CNT_BASE+7)  /* bad min, max or out of range   */
#define EOS_CNT_TYPE      (EOS_CNT_BASE+8)  /* bad type of control            */

#define EOS_CLIP_BASE   (EOS_GSL_BASE+40)   /* errors for clipboard manager   */
#define EOS_CLIP_DEV    (EOS_CLIP_BASE+0)   /* can't find clipboard default   *
                                             *  device from preferences       */
#define EOS_CLIP_FULL   (EOS_CLIP_BASE+1)   /* clipboard is full (too many    *
                                             *  different types in clipboard) */
#define EOS_CLIP_TYPE   (EOS_CLIP_BASE+2)   /* type not currently represented *
                                             *  in clipboard                  */
#define EOS_CLIP_ACC    (EOS_CLIP_BASE+3)   /* clipboard not opened for the   *
                                             *  requested access              */
#define EOS_CLIP_CNT    (EOS_CLIP_BASE+4)   /* type offset greater than type  *
                                             *  count                         */
#define EOS_CLIP_OPEN   (EOS_CLIP_BASE+5)   /* clipboard not currently opened */
#define EOS_CLIP_INIT   (EOS_CLIP_BASE+6)   /* clipboard not initialized      */
#define EOS_CLIP_CLOSE  (EOS_CLIP_BASE+7)   /* clipboard not currently closed */
#define EOS_CLIP_RW     (EOS_CLIP_BASE+8)   /* can't rewrite - type not       *
                                             *  represented in clipboard      */

#define EOS_HNDLR_BASE      (EOS_GSL_BASE+50) /* errors for dispatch manager */
#define EOS_HNDLR_UNKNOWN   (EOS_HNDLR_BASE+0)

#define EOS_ATBL_BASE       (EOS_GSL_BASE+55) /* errors for association table routines */
#define EOS_ATBL_NOENTRY    (EOS_ATBL_BASE+0)   /* no entry found */

#define EOS_BOX_BASE        (EOS_GSL_BASE+60)       /* errors for ed_box function  */
#define EOS_BOX_TABLE       (EOS_BOX_BASE+0)        /* line table overflow */
#define EOS_BOX_COUNT       (EOS_BOX_BASE+1)        /* text too long (max = 65535 characters) */
#define EOS_BOX_TYPE        (EOS_BOX_BASE+2)        /* bad type or type not implemented */
#define EOS_BOX_MAXL        (EOS_BOX_BASE+3)        /* attempt to draw too long line (max is 1024 chars) */
#define EOS_BOX_NOTAB       (EOS_BOX_BASE+4)        /* need a line table (if the BOX_F_USETAB flag is set in the "type" parameter) */
#define EOS_BOX_NOFONT      (EOS_BOX_BASE+5)        /* font not set in the drawmap */
#define EOS_BOX_RECT        (EOS_BOX_BASE+6)        /* bad rectangle */

#define EOS_INIT_BASE       (EOS_GSL_BASE+80)       /* errors for init function */
#define EOS_INIT_VARERR     (EOS_INIT_BASE+0)

#define EOS_INTER_BASE      (EOS_GSL_BASE+85)
#define EOS_INTER_NOMOD     (EOS_INTER_BASE+0)  /* no preference module */
#define EOS_INTER_ILLARG    (EOS_INTER_BASE+1)  /* illegal arguement */

#define EOS_OVL_BASE        (EOS_GSL_BASE+90)       /* overlay window errors */
#define EOS_OVL_BADRECT     (EOS_OVL_BASE+0)        /* bad rectangle for overlay */
#define EOS_OVL_NOTTOP      (EOS_OVL_BASE+1)        /* not top of overlay stack */
#define EOS_OVL_UNKNOWN     (EOS_OVL_BASE+2)        /* unknown overlay */

#define EOS_IND_BASE        (EOS_GSL_BASE+100)      /* indicator manager errors */
#define EOS_IND_DEFID       (EOS_IND_BASE+0)        /* bad definition id */
#define EOS_IND_DEFACT      (EOS_IND_BASE+1)        /* bad action for def. function */
#define EOS_IND_MINMAX      (EOS_IND_BASE+2)        /* bad min, max or value out of range */
#define EOS_IND_BADCORDS    (EOS_IND_BASE+3)        /* bad coordinates for indicator */
#define EOS_IND_NOCREATE    (EOS_IND_BASE+4)        /* attempt op. on non-created ind. */
#define EOS_IND_BADFLAGS    (EOS_IND_BASE+5)        /* bad flag combination */
#define EOS_IND_BADPTR      (EOS_IND_BASE+6)        /* bad pointer in structure */
#define EOS_IND_BADDISP     (EOS_IND_BASE+7)        /* bad displacement */

/* ----------------------- IFF  Specific Section ----------------------------*/
#if !defined(EOS_IFF_BASE)
#if defined(_OSK)
/* 0x700 through 0x7ff IFF */
#define EOS_IFF_BASE            ((7<<8)+000)        /* base error code for IFF */
#elif defined(_OS9000)
/* 0x70000 through 0x700ff IFF */
#define EOS_IFF_BASE            ((7<<16)+000)       /* base error code for IFF */
#endif /* _OS9000 */
#define EOS_IFF_RONLY           (EOS_IFF_BASE+1)    /* this path is read-only */
#define EOS_IFF_WONLY           (EOS_IFF_BASE+2)    /* this path is write-only */
#define EOS_IFF_ACTFORM         (EOS_IFF_BASE+3)    /* there is no FORM active */
#define EOS_IFF_READER          (EOS_IFF_BASE+4)    /* wrong reader for this type of FORM */
#define EOS_IFF_NOTIFF          (EOS_IFF_BASE+5)    /* not an IFF file */
#define EOS_IFF_BADPARM         (EOS_IFF_BASE+6)    /* bad parameters */
#define EOS_IFF_BADCAT          (EOS_IFF_BASE+7)    /* bad CAT id (for iff_open) */
#define EOS_IFF_SIZEOS_UNKNOWN  (EOS_IFF_BASE+8)    /* cannot skip, size is unknown */
#define EOS_IFF_NOT_DATA        (EOS_IFF_BASE+9)    /* not to the data yet */
#define EOS_IFF_PIPEOS_SEEK     (EOS_IFF_BASE+10)   /* attempt to seek back in a pipe */
#define EOS_IFF_BADCHUNKSIZE    (EOS_IFF_BASE+11)   /* fixed size chunk was not the 
                                                       correct size.  Could indicate 
                                                       wrong version of reader */
#define EOS_IFF_FPNOTNUM        (EOS_IFF_BASE+12)   /* can't make float conversion */
#endif /* EOS_IFF_BASE */

/* ----------------------- ENET Specific Section ----------------------------*/
#if defined(_OSK)
/* 0x700 is for Ethernet */
#define EOS_ENETBASE    0x700
#elif defined(_OS9000)
/* 0x70000 is for Etherenet */
#define EOS_ENETBASE    0x70000
#endif /* _OS9000 */
#define EWOULDBLOCK     EOS_ENETBASE+0x01  /* I/O operation would block */
#define EINPROGRESS     EOS_ENETBASE+0x02  /* I/O operation now in progress */
#define EALREADY        EOS_ENETBASE+0x03  /* operation already in progress */
#define EDESTADDRREQ    EOS_ENETBASE+0x04  /* destination address required */
#define EMSGSIZE        EOS_ENETBASE+0x05  /* message too long */
#define EPROTOTYPE      EOS_ENETBASE+0x06  /* protocol wrong type for socket */
#define ENOPROTOOPT     EOS_ENETBASE+0x07  /* bad protocol option */
#define EPROTONOSUPPORT EOS_ENETBASE+0x08  /* protocol not supported */
#define ESOCKNOSUPPORT  EOS_ENETBASE+0x09  /* socket type not supported */
#define EOPNOTSUPP      EOS_ENETBASE+0x0a  /* operation not supported on socket */
#define EPFNOSUPPORT    EOS_ENETBASE+0x0b  /* protocol family not supported */
#define EAFNOSUPPORT    EOS_ENETBASE+0x0c  /* addr fam. not supp by protocol */
#define EADDRINUSE      EOS_ENETBASE+0x0d  /* address already in use */
#define EADDRNOTAVAIL   EOS_ENETBASE+0x0e  /* can't assign requested address */
#define ENETDOWN        EOS_ENETBASE+0x0f  /* network is down */
#define ENETUNREACH     EOS_ENETBASE+0x10  /* network is unreachable */
#define ENETRESET       EOS_ENETBASE+0x11  /* network dropped conn on reset */
#define ECONNABORTED    EOS_ENETBASE+0x12  /* software caused connection abort */
#define ECONNRESET      EOS_ENETBASE+0x13  /* connection reset by peer */
#define ENOBUFS         EOS_ENETBASE+0x14  /* no buffer space available */
#define EISCONN         EOS_ENETBASE+0x15  /* socket is already connected */
#define ENOTCONN        EOS_ENETBASE+0x16  /* socket is not connected */
#define ESHUTDOWN       EOS_ENETBASE+0x17  /* can't send after socket shutdown */
#define ETOOMANYREFS    EOS_ENETBASE+0x18  /* too many references */
#define ETIMEDOUT       EOS_ENETBASE+0x19  /* connection timed out */
#define ECONNREFUSED    EOS_ENETBASE+0x1a  /* connection refused by target */
#define EBUFTOOSMALL    EOS_ENETBASE+0x1b  /* mbuf too small for operation    */
#define ESMODEXISTS     EOS_ENETBASE+0x1c  /* socket module already attached  */
#define ENOTSOCK        EOS_ENETBASE+0x1d  /* path is not a socket            */
#define EHOSTUNREACH    EOS_ENETBASE+0x1e  /* No route to host */
#define EHOSTDOWN       EOS_ENETBASE+0x1f  /* host is down */


/* 008:000 reserved for Wide Area Networking */
#if defined(_OS9000)

#define EOS_SPFBASE     0x80000             /* SPF error code base          */
#define EOS_WANBASE EOS_SPFBASE             /* For backwards compatability  */
#endif /* _OS9000 */

#if defined(_OSK)
#define EOS_SPFBASE     0x800               /* SPF error code base          */
#define EOS_WANBASE EOS_SPFBASE             /* For backwards compatability  */
#endif /* _OSK */

#define EOS_LNKDWN      (EOS_SPFBASE+1)     /* Layer 1 link down [Driver]   */
#define EOS_CONN        (EOS_SPFBASE+2)     /* Connection error [Driver]    */
#define EOS_RXTHREAD    (EOS_SPFBASE+3)     /* Error in receive thread      */
#define EOS_ME          (EOS_SPFBASE+4)     /* Management entity error      */
#define EOS_SAPI        (EOS_SPFBASE+5)     /* Unrecognized SAPI            */
#define EOS_TEI         (EOS_SPFBASE+6)     /* TEI not found                */
#define EOS_MAX_TEI     (EOS_SPFBASE+7)     /* Maximum # of TEI             */
#define EOS_TSTATE      (EOS_SPFBASE+8)     /* Illegal TEI state            */
#define EOS_TEI_DENIED  (EOS_SPFBASE+9)     /* TEI request denied by network*/
#define EOS_PRIMITIVE   (EOS_SPFBASE+10)    /* Unrecognized primitive       */
#define EOS_L2IN        (EOS_SPFBASE+11)    /* Layer 2 error                */
#define EOS_PEER_BUSY   (EOS_SPFBASE+12)    /* Peer receiver busy condition */
#define EOS_K           (EOS_SPFBASE+13)    /* Max # outstanding I frames   */
#define EOS_MAXCREF     (EOS_SPFBASE+14)    /* Max # of call references     */
#define EOS_CREF        (EOS_SPFBASE+15)    /* Call reference doesn't exist */
#define EOS_CALLPROG    (EOS_SPFBASE+16)    /* Error on call progress state */
#define EOS_RCVR        (EOS_SPFBASE+17)    /* Receiver previously assigned */
#define EOS_REQDENIED   (EOS_SPFBASE+18)    /* Request denied by far end    */
#define EOS_RXSTART     (EOS_SPFBASE+19)    /* Receive thread didn't start  */
#define EOS_NOSTACK     (EOS_SPFBASE+20)    /* Last driver on path's stack  */
#define EOS_BTMSTK      (EOS_SPFBASE+21)    /* Attemp to pop last driver    */
#define EOS_NPBNULL     (EOS_SPFBASE+22)    /* Notify param block is NULL   */
#define EOS_PPS_NOTFND  (EOS_SPFBASE+23)    /* Per path storage not found   */
#define EOS_STKFULL     (EOS_SPFBASE+24)    /* Path's stack array is full   */
#define EOS_MBNOTINST   (EOS_SPFBASE+25)    /* Sysmbuf not installed        */
#define EOS_TMRNTFND    (EOS_SPFBASE+26)    /* Timer not found              */
#define EOS_GETIME      (EOS_SPFBASE+27)    /* Get time error?              */
#define EOS_TIMERINT    (EOS_SPFBASE+28)    /* Timer interrupt?             */
#define EOS_RXMB_NODEVENTRY (EOS_SPFBASE+29)/* No device entry in mbuf      */
#define EOS_PGM_TBLBSY  (EOS_SPFBASE+30)    /* PSI/SI Table is in use       */
#define EOS_PGM_TBLOVF  (EOS_SPFBASE+31)    /* Too many tables being read   */
#define EOS_PGM_TBLNFND (EOS_SPFBASE+32)    /* Table not found              */
#define EOS_PGM_NFND    (EOS_SPFBASE+33)    /* Program not found            */
#define EOS_PGM_NOPLAY  (EOS_SPFBASE+34)    /* No pgm currently playing     */
#define EOS_NODNDRVR    (EOS_SPFBASE+35)    /* No down driver               */

#define EOS_RXMB_ERR    (EOS_SPFBASE+40)    /* Receive data error base      */

#define EOS_WANAPI_BASE (EOS_SPFBASE+192)   /* Error base for WAN API's     */

#define EOS_GENDRVR_BASE (EOS_SPFBASE+224)  /* Custom 3rd party drvr codes  */

#define EOS_IEEE1394_BASE (EOS_SPFBASE+256) /* IEEE 1394 error codes        */

/* IEEE1394 is 008:256 (0x008:100) to 008:304 (0x008:130) */
#define EOS_MW1394_BUSRESET           (EOS_IEEE1394_BASE+0x01)
#define EOS_MW1394_NOIRM              (EOS_IEEE1394_BASE+0x02)
#define EOS_MW1394_NNF                (EOS_IEEE1394_BASE+0x03)
#define EOS_MW1394_TIMEDOUT           (EOS_IEEE1394_BASE+0x04) /* 260 */
#define EOS_MW1394_FAILED             (EOS_IEEE1394_BASE+0x05)
#define EOS_MW1394_BADSIZE            (EOS_IEEE1394_BASE+0x06)
#define EOS_MW1394_ADDRINUSE          (EOS_IEEE1394_BASE+0x07)
#define EOS_MW1394_ADDRNOTFND         (EOS_IEEE1394_BASE+0x08)
#define EOS_MW1394_ADDRNOTAVAIL       (EOS_IEEE1394_BASE+0x09)
#define EOS_MW1394_INVCHANNEL         (EOS_IEEE1394_BASE+0x0a)
#define EOS_MW1394_CHNLINUSE          (EOS_IEEE1394_BASE+0x0b)
#define EOS_MW1394_NOCFGREG           (EOS_IEEE1394_BASE+0x0c)
#define EOS_MW1394_CHNLNOTFND         (EOS_IEEE1394_BASE+0x0d)
#define EOS_MW1394_CHNL_STOPPED       (EOS_IEEE1394_BASE+0x0e) /* 270 */
#define EOS_MW1394_NO_FREECHNL        (EOS_IEEE1394_BASE+0x0f)
#define EOS_MW1394_NOXMIT             (EOS_IEEE1394_BASE+0x10)
#define EOS_MW1394_INVTOPOMAP         (EOS_IEEE1394_BASE+0x11)
#define EOS_MW1394_INVSPEEDMAP        (EOS_IEEE1394_BASE+0x12)
#define EOS_MW1394_BUSMGR_EXISTS      (EOS_IEEE1394_BASE+0x13)
#define EOS_MW1394_BEBUSMGRFAIL       (EOS_IEEE1394_BASE+0x14)
#define EOS_MW1394_BUSMGR_ALRDY       (EOS_IEEE1394_BASE+0x15)
#define EOS_MW1394_ALLOCCHNLFAIL      (EOS_IEEE1394_BASE+0x16)
#define EOS_MW1394_FREECHNLFAIL       (EOS_IEEE1394_BASE+0x17)
#define EOS_MW1394_CHNLFREEALRDY      (EOS_IEEE1394_BASE+0x18) /* 280 */
#define EOS_MW1394_ALLOCBWDTHFAIL     (EOS_IEEE1394_BASE+0x19)
#define EOS_MW1394_FREEBWDTHFAIL      (EOS_IEEE1394_BASE+0x1a)
#define EOS_MW1394_BANDWIDTH_NOTAVAIL (EOS_IEEE1394_BASE+0x1b)
#define EOS_MW1394_INV_BANDWIDTH_HNDL (EOS_IEEE1394_BASE+0x1c)
#define EOS_MW1394_NOBUFFERS_ATTCHD   (EOS_IEEE1394_BASE+0x1d)
#define EOS_MW1394_RCODE_ERROR        (EOS_IEEE1394_BASE+0x1e)
#define EOS_MW1394_NOBUSMGR           (EOS_IEEE1394_BASE+0x1f)
#define EOS_MW1394_INVACCESSTYPE      (EOS_IEEE1394_BASE+0x20)

/* ----------------------- MAUI Specific Section -----------------------------*/
#if defined(_OSK)
/* 0xa00 is for MAUI */
#define EOS_MAUI_BASE 0xa00
#elif defined(_OS9000)
/* 0xa0000 is for MAUI */
#define EOS_MAUI_BASE 0xa0000
#endif /* _OS9000 */

#define EOS_MAUI_BADACK         (EOS_MAUI_BASE+1)   /* Bad acknowledgement */
#define EOS_MAUI_BADCODEMETH    (EOS_MAUI_BASE+2)   /* Bad coding method */
#define EOS_MAUI_BADCOLORTYPE   (EOS_MAUI_BASE+3)   /* Bad color type */
#define EOS_MAUI_BADCOMPATLEVEL (EOS_MAUI_BASE+4)   /* Bad compatibility level */
#define EOS_MAUI_BADDEFCHAR     (EOS_MAUI_BASE+5)   /* Bad default character */
#define EOS_MAUI_BADDIMEN       (EOS_MAUI_BASE+6)   /* Bad dimension */
#define EOS_MAUI_BADFRAME       (EOS_MAUI_BASE+7)   /* Bad frame */
#define EOS_MAUI_BADID          (EOS_MAUI_BASE+8)   /* Bad ID */
#define EOS_MAUI_BADLINESIZE    (EOS_MAUI_BASE+9)   /* Bad line size */
#define EOS_MAUI_BADMBC         (EOS_MAUI_BASE+10)  /* Bad multi-byte char */
#define EOS_MAUI_BADPOS         (EOS_MAUI_BASE+11)  /* Bad position */
#define EOS_MAUI_BADPTR         (EOS_MAUI_BASE+12)  /* Bad pointer */
#define EOS_MAUI_BADRANGE       (EOS_MAUI_BASE+13)  /* Bad range */
#define EOS_MAUI_BADSHADE       (EOS_MAUI_BASE+14)  /* Bad shade */
#define EOS_MAUI_BADSIZE        (EOS_MAUI_BASE+15)  /* Bad size */
#define EOS_MAUI_BADVALUE       (EOS_MAUI_BASE+16)  /* Bad value */
#define EOS_MAUI_BUSY           (EOS_MAUI_BASE+17)  /* Busy - try later */
#define EOS_MAUI_CANTDISPLAY    (EOS_MAUI_BASE+18)  /* Can't display pixmem */
#define EOS_MAUI_DAMAGE         (EOS_MAUI_BASE+19)  /* Data structures damaged */
#define EOS_MAUI_DEFINED        (EOS_MAUI_BASE+20)  /* Already defined */
#define EOS_MAUI_DMAPTOOSMALL   (EOS_MAUI_BASE+21)  /* Drawmap is too small */
#define EOS_MAUI_INCOMPATCM     (EOS_MAUI_BASE+22)  /* Incompatible coding method */
#define EOS_MAUI_INTERNAL       (EOS_MAUI_BASE+23)  /* Internal error */
#define EOS_MAUI_INUSE          (EOS_MAUI_BASE+24)  /* Still in use */
#define EOS_MAUI_ISINIT         (EOS_MAUI_BASE+25)  /* Is initialized */
#define EOS_MAUI_ISRESERVED     (EOS_MAUI_BASE+26)  /* Is reserved */
#define EOS_MAUI_MASKED         (EOS_MAUI_BASE+27)  /* Message type is masked */
#define EOS_MAUI_MBOXFULL       (EOS_MAUI_BASE+28)  /* Mailbox is full */
#define EOS_MAUI_MISSINGFEP     (EOS_MAUI_BASE+29)  /* Missing FEP in driver */
#define EOS_MAUI_NOCALLBACK     (EOS_MAUI_BASE+30)  /* No callback function */
#define EOS_MAUI_NODMAP         (EOS_MAUI_BASE+31)  /* No drawmap */
#define EOS_MAUI_NODSTDMAP      (EOS_MAUI_BASE+32)  /* No destination drawmap */
#define EOS_MAUI_NOEXPTABLE     (EOS_MAUI_BASE+33)  /* No expansion table */
#define EOS_MAUI_NOFONT         (EOS_MAUI_BASE+34)  /* No font */
#define EOS_MAUI_NOHWSUPPORT    (EOS_MAUI_BASE+35)  /* No hardware support */
#define EOS_MAUI_NOINIT         (EOS_MAUI_BASE+36)  /* Not initialized */
#define EOS_MAUI_NOMASKDMAP     (EOS_MAUI_BASE+37)  /* No mask drawmap */
#define EOS_MAUI_NOMAUIP        (EOS_MAUI_BASE+38)  /* No MAUI process */
#define EOS_MAUI_NOPIXMEM       (EOS_MAUI_BASE+39)  /* No pixel memory */
#define EOS_MAUI_NOPMOD         (EOS_MAUI_BASE+40)  /* No protocol module */
#define EOS_MAUI_NOSPRITE       (EOS_MAUI_BASE+41)  /* No sprite */
#define EOS_MAUI_NOSRCDMAP      (EOS_MAUI_BASE+42)  /* No source drawmap */
#define EOS_MAUI_NOTALIGNED     (EOS_MAUI_BASE+43)  /* Not aligned */
#define EOS_MAUI_NOTFOUND       (EOS_MAUI_BASE+44)  /* Not found */
#define EOS_MAUI_NOTIMPLEMENTED (EOS_MAUI_BASE+45)  /* Feature not implemented */
#define EOS_MAUI_NOTPENDING     (EOS_MAUI_BASE+46)  /* Not pending */
#define EOS_MAUI_NOTRESERVED    (EOS_MAUI_BASE+47)  /* Is not reserved */
#define EOS_MAUI_SIGNAL         (EOS_MAUI_BASE+48)  /* Wait aborted by signal */
#define EOS_MAUI_TOOCOMPLEX     (EOS_MAUI_BASE+49)  /* Too complex */
#define EOS_MAUI_TOOLONG        (EOS_MAUI_BASE+50)  /* Too long */
#define EOS_MAUI_CANTRESIZE     (EOS_MAUI_BASE+51)  /* Cannot resize */
#define EOS_MAUI_NOPALETTE      (EOS_MAUI_BASE+52)  /* No palette specified */
#define EOS_MAUI_BADNUMCHAN     (EOS_MAUI_BASE+53)  /* Bad number of channels */
#define EOS_MAUI_NOTBUSY        (EOS_MAUI_BASE+54)  /* Not busy */
#define EOS_MAUI_NOTPAUSED      (EOS_MAUI_BASE+55)  /* Not paused */
#define EOS_MAUI_ABORT          (EOS_MAUI_BASE+56)  /* Aborted */
#define EOS_MAUI_TOOOLD         (EOS_MAUI_BASE+57)  /* Too old */
#define EOS_MAUI_INCOMPATVER    (EOS_MAUI_BASE+58)  /* Incompatible versions */
#define EOS_MAUI_NOTALLOWED     (EOS_MAUI_BASE+59)  /* Operation not allowed */
#define EOS_MAUI_NOTOWNER       (EOS_MAUI_BASE+60)  /* Not the owner */
#define EOS_MAUI_DISABLED       (EOS_MAUI_BASE+61)  /* Feature disabled */
#define EOS_MAUI_NOTRAP         (EOS_MAUI_BASE+62)  /* MAUI trap not found */
#define EOS_MAUI_DEVNOTFOUND    (EOS_MAUI_BASE+63)  /* Device not found */
#define EOS_MAUI_CMAPFULL       (EOS_MAUI_BASE+64)  /* Colormap full */
#define EOS_MAUI_PAUSED         (EOS_MAUI_BASE+65)  /* Paused */
#define EOS_MAUI_BADRATE        (EOS_MAUI_BASE+66)  /* Bad rate */
#define EOS_MAUI_NODVSUPPORT    (EOS_MAUI_BASE+67)  /* No driver support */
#define EOS_MAUI_NOTVISIBLE     (EOS_MAUI_BASE+68)  /* Not visible */

/* ----------------------- Thread Specific Section -------------------------*/
#ifdef	_OS9000
#	define	_THREAD_BASE	(11<<16)
#endif	/* _OS9000 */

#define	EOS_THRD_INVLD		(_THREAD_BASE+1)	/* op not valid w/ threads	*/

/* ----------------------- POSIX Specific Section --------------------------*/
#ifdef _OSK
#define _POSIX_BASE (20<<8)
#else
#define _POSIX_BASE (20<<16)
#endif

#define E2BIG			(_POSIX_BASE |  1)	/* argument list too long		*/
#define EACCES			EOS_PERMIT			/* permission denied			*/
#define EAGAIN			EWOULDBLOCK			/* item temporarily unavailable	*/
#define EBADF 			EOS_BPNUM			/* bad file descriptor			*/
#define EBADMSG			(_POSIX_BASE |  3)	/* bad message					*/
#define EBUSY			EOS_DEVBSY			/* resource busy				*/
#define ECANCELED		(_POSIX_BASE |  4)	/* operation canceled			*/
#define ECHILD			EOS_NOCHLD			/* no child process				*/
#define EDEADLK			(_POSIX_BASE |  5)	/* resource deadlock avoided	*/
#define EEXIST			EOS_CEF				/* file exists					*/
#define EFAULT			EOS_BPADDR			/* bad address					*/
#define EFBIG			(_POSIX_BASE |  6)	/* file too big					*/
#define EINTR			EALREADY			/* interrupted function call	*/
#define EINVAL			EOS_ILLARG			/* invalid argument				*/
#define EIO				EOS_READ			/* I/O error					*/
#define EISDIR			(_POSIX_BASE |  7)	/* is a directory				*/
#define EMFILE			(_POSIX_BASE |  8)	/* too many process open files	*/
#define EMLINK			(_POSIX_BASE |  9)	/* too many links				*/
#define ENAMETOOLONG	(_POSIX_BASE | 10)	/* filename too long			*/
#define ENFILE			(_POSIX_BASE | 11)	/* too many system open files	*/
#define ENODEV			EOS_NOTRDY			/* no such device				*/
#define ENOENT			EOS_PNNF			/* no such file or directory	*/
#define ENOEXEC			(_POSIX_BASE | 12)	/* exec format error			*/
#define ENOLCK			(_POSIX_BASE | 13)	/* no locks available			*/
#define ENOMEM			EOS_NORAM			/* insufficient RAM				*/
#define ENOSPC			(_POSIX_BASE | 14)	/* no space left on device		*/
#define ENOSYS			(_POSIX_BASE | 15)	/* function not implemented		*/
#define ENOTDIR			(_POSIX_BASE | 16)	/* not a directory				*/
#define ENOTEMPTY		(_POSIX_BASE | 17)	/* directory not empty			*/
#define ENOTSUP			(_POSIX_BASE | 18)	/* not supported				*/
#define ENOTTY			(_POSIX_BASE | 19)	/* bad I/O control operation	*/
#define ENXIO			EOS_NOTRDY			/* no such device or address	*/
#define EPERM			EOS_FNA				/* operation no permitted		*/
#define EPIPE			EOS_EOF				/* broken pipe					*/
#define EROFS			(_POSIX_BASE | 20)	/* read-only file system		*/
#define ESPIPE			(_POSIX_BASE | 21)	/* invalid seek					*/
#define ESRCH			EOS_IPRCID			/* no such process				*/
#define EXDEV			(_POSIX_BASE | 22)	/* improper link				*/


/* ----------------------- PPC Specific Section -----------------------------*/
#define PPC_BASE (100 << 16)

#define EOS_PPC_MACHCHK (PPC_BASE+2)    /* Machine check                     */
#define EOS_PPC_DATAACC (PPC_BASE+3)    /* Data access                       */
#define EOS_PPC_INSTACC (PPC_BASE+4)    /* Instruction access                */
#define EOS_PPC_EXTINT  (PPC_BASE+5)    /* External interrupt                */
#define EOS_PPC_ALIGN   (PPC_BASE+6)    /* Alignment                         */
#define EOS_PPC_PROGRAM (PPC_BASE+7)    /* Program                           */
#define EOS_PPC_FPUUNAV (PPC_BASE+8)    /* FPU unavailable                   */
#define EOS_PPC_DEC     (PPC_BASE+9)    /* Decrementer                       */
#define EOS_PPC_IOCONT  (PPC_BASE+10)   /* I/O controller                    */
#define EOS_PPC_SYSCALL (PPC_BASE+12)   /* System call                       */
#define EOS_PPC_TRACE   (PPC_BASE+32)   /* Trace                             */

/* ----------------------- SH-5m Specific Section ---------------------------*/
#define SH5M_BASE (101 << 16)

#define EOS_SH5M_ITLB    (SH5M_BASE+0)    /* Instruction TLB                   */
#define EOS_SH5M_DTLB    (SH5M_BASE+1)    /* Data TLB                          */
#define EOS_SH5M_OPCODE  (SH5M_BASE+2)    /* Unimplemented opcode              */
#define EOS_SH5M_PROTECT (SH5M_BASE+3)    /* Protected instruction violation   */
#define EOS_SH5M_READ    (SH5M_BASE+4)    /* No read permission                */
#define EOS_SH5M_WRITE   (SH5M_BASE+5)    /* No write permission               */
#define EOS_SH5M_EXECUTE (SH5M_BASE+6)    /* No execute permission             */
#define EOS_SH5M_UNALIGN (SH5M_BASE+7)    /* Unaligned operation               */
#define EOS_SH5M_OVERFLOW (SH5M_BASE+8) /* Integer overflow                */
#define EOS_SH5M_DIVZERO (SH5M_BASE+9)    /* Integer divide by zero            */
#define EOS_SH5M_SOFT    (SH5M_BASE+10)   /* Software trap (trap instruction)  */
#define EOS_SH5M_MALADDR (SH5M_BASE+11)   /* Bad effective address             */
#define EOS_SH5M_DEBUG   (SH5M_BASE+16)   /* Single step or debug instruction  */
#define EOS_SH5M_IWATCH  (SH5M_BASE+17)   /* Instruction watchpoint            */
#define EOS_SH5M_DWATCH  (SH5M_BASE+18)   /* Data watchpoint                   */
#define EOS_SH5M_FPINV   (SH5M_BASE+24)   /* Floating-point invalid operation  */
#define EOS_SH5M_FPDIVZ  (SH5M_BASE+25)   /* Floating-point divide by zero     */
#define EOS_SH5M_FPOFLOW (SH5M_BASE+26)   /* Floating-point overflow           */
#define EOS_SH5M_FPUFLOW (SH5M_BASE+27)   /* Floating-point underflow          */
#define EOS_SH5M_FPINEXCT (SH5M_BASE+28) /* Floating-point inexact             */
#define EOS_SH5M_FPIEEE  (SH5M_BASE+29)   /* Floating-point IEEE software support */


/* -------------------- MIPS Specific Section -----------------------------*/
#define MIPS_BASE (102 << 16)

#define EOS_MIPS_EXTINT (MIPS_BASE+0)   /* External Interrupt               */
#define EOS_MIPS_MOD    (MIPS_BASE+1)   /* TLB Modification Exception       */
#define EOS_MIPS_TLBL   (MIPS_BASE+2)   /* TLB Miss Exception (load or instruction fetch) */
#define EOS_MIPS_TLBS   (MIPS_BASE+3)   /* TLB Miss Exception (store)       */
#define EOS_MIPS_ADEL   (MIPS_BASE+4)   /* Address Error Exception (load or instruction fetch) */
#define EOS_MIPS_ADES   (MIPS_BASE+5)   /* Address Error Exception (store)  */
#define EOS_MIPS_IBE    (MIPS_BASE+6)   /* Bus Error Exception (instruction fetch) */
#define EOS_MIPS_DBE    (MIPS_BASE+7)   /* Bus Error Exception (load or store) */
#define EOS_MIPS_SYS    (MIPS_BASE+8)   /* SYSCALL Exception                */
#define EOS_MIPS_BP     (MIPS_BASE+9)   /* Breakpoint Exception             */
#define EOS_MIPS_RI     (MIPS_BASE+10)  /* Reserved Instruction Exception   */
#define EOS_MIPS_CPU    (MIPS_BASE+11)  /* CoProcessor Unusable Exception   */
#define EOS_MIPS_OVF    (MIPS_BASE+12)  /* Arithmetic Overflow Exception    */
#define EOS_MIPS_TR     (MIPS_BASE+13)  /* Trap Exception                   */
#if defined(_MPFMIPS64)
#define EOS_MIPS_FPE    (MIPS_BASE+15)  /* Floating Point Exception         */
#endif
#define EOS_MIPS_WATCH  (MIPS_BASE+23)  /* Watch Exception                  */
#define EOS_MIPS_UTLB   (MIPS_BASE+32)  /* User State TLB Miss Exception    */
#if defined(_MPFMIPS3000)
#define EOS_MIPS_FPE    (MIPS_BASE+67)  /* Floating Point Exception         */
#endif

/* -------------------- ARM Specific Section -----------------------------*/
#define ARM_BASE (103 << 16)

#define EOS_ARM_UNDEF   (ARM_BASE+1)    /* Undefined Instruction Exception  */
#define EOS_ARM_PFABORT (ARM_BASE+3)    /* Instruction pre-fetch abort exception    */
#define EOS_ARM_DTABORT (ARM_BASE+4)    /* Data abort Exception             */
#define EOS_ARM_ALIGN   (ARM_BASE+8)    /* Alignment Exception              */

/* -------------------- SH Specific Section -----------------------------*/
#define SH_BASE (104 << 16)

#define EOS_SH_TLBMISSLD    (SH_BASE+2)     /* TLB miss on a load       */
#define EOS_SH_TLBMISSST    (SH_BASE+3)     /* TLB miss on a store      */
#define EOS_SH_INITPG       (SH_BASE+4)     /* Initial page write       */
#define EOS_SH_TLBPROTLD    (SH_BASE+5)     /* TLB protection violation on a load */
#define EOS_SH_TLBPROTST    (SH_BASE+6)     /* TLB protection violation on a store */
#define EOS_SH_ADDRLD       (SH_BASE+7)     /* Address error on a load  */
#define EOS_SH_ADDRST       (SH_BASE+8)     /* Address error on a store */
#define EOS_SH_TRAPA        (SH_BASE+11)    /* TRAPA instruction        */
#define EOS_SH_RSRVINSTR    (SH_BASE+12)    /* Reserved instruction     */
#define EOS_SH_ILLSLOT      (SH_BASE+13)    /* Illegal slot instruction */
#define EOS_SH_BRKPT        (SH_BASE+15)    /* User break point         */

/* --------------------- x86 Specific Section ----------------------------*/
#define X86_BASE (106 << 16)

#define EOS_X86_DIVIDE		(X86_BASE)		/* Divide Error				*/
#define EOS_X86_DEBUG		(X86_BASE+1		/* Debug Error				*/
#define EOS_X86_NMI			(X86_BASE+2)	/* NMI						*/
#define EOS_X86_BREAKPT		(X86_BASE+3)	/* Breakpoint				*/
#define EOS_X86_OVF			(X86_BASE+4)	/* Overflow 				*/
#define EOS_X86_BNDSCHK		(X86_BASE+5)	/* Bounds Check				*/
#define EOS_X86_INVLDOP		(X86_BASE+6)	/* Invalid Opcode			*/
#define EOS_X86_NODEVICE	(X86_BASE+7)	/* Device Not Available		*/
#define EOS_X86_DBLFLT		(X86_BASE+8)	/* Double Fault				*/
#define EOS_X86_INVALTSS	(X86_BASE+10)	/* Invalid TSS				*/
#define EOS_X86_NOSEG		(X86_BASE+11)	/* Segment Not Present		*/
#define EOS_X86_STACK		(X86_BASE+12)	/* Stack Fault				*/
#define EOS_X86_GPF			(X86_BASE+13)	/* General Protection Fault	*/
#define EOS_X86_PAGEFLT		(X86_BASE+14) 	/* Page Fault				*/
#define EOS_X86_FPE			(X86_BASE+16)  	/* Floating Point Error		*/
#define EOS_X86_ALIGN		(X86_BASE+17)  	/* Alignment Error			*/
#define EOS_X86_MACHCHK		(X86_BASE+18)	/* Machine Check			*/
 
/* -------------------- SPARC Specific Section ---------------------------*/
#define SPARC_BASE (107 << 16)

#define EOS_SPARC_INSTRBLK  (SPARC_BASE+0x01)   /* Instruction access exception */
#define EOS_SPARC_ILLINSTR  (SPARC_BASE+0x02)   /* Illegal instruction      */
#define EOS_SPARC_PRIVINSTR (SPARC_BASE+0x03)   /* Privileged instruction   */
#define EOS_SPARC_FPDISABLE (SPARC_BASE+0x04)   /* Floating point disabled  */
#define EOS_SPARC_WINDOWOV  (SPARC_BASE+0x05)   /* Window overflow          */
#define EOS_SPARC_WINDOWUF  (SPARC_BASE+0x06)   /* Window underflow         */
#define EOS_SPARC_UNALIGNED (SPARC_BASE+0x07)   /* Memory address unaligned */
#define EOS_SPARC_FP        (SPARC_BASE+0x08)   /* Floating point exception */
#define EOS_SPARC_DATABLK   (SPARC_BASE+0x09)   /* Data access exception    */
#define EOS_SPARC_TAGOV     (SPARC_BASE+0x0A)   /* Tagged arith. overflow   */
#define EOS_SPARC_WATCHPNT  (SPARC_BASE+0x0B)   /* Watchpoint detected      */
#define EOS_SPARC_RREGACC   (SPARC_BASE+0x20)   /* Register access error    */
#define EOS_SPARC_INSTRPER  (SPARC_BASE+0x21)   /* Instruction access error */
#define EOS_SPARC_CPDISABLE (SPARC_BASE+0x24)   /* Coprocessor disabled     */
#define EOS_SPARC_NOFLUSH   (SPARC_BASE+0x25)   /* Unimplemented FLUSH      */
#define EOS_SPARC_CP        (SPARC_BASE+0x28)   /* Coprocessor exception    */
#define EOS_SPARC_DATAPER   (SPARC_BASE+0x29)   /* Data access error        */
#define EOS_SPARC_ZERODIV   (SPARC_BASE+0x2A)   /* Divide by Zero           */
#define EOS_SPARC_DATAST    (SPARC_BASE+0x2B)   /* Data store error         */
#define EOS_SPARC_DATAMMU   (SPARC_BASE+0x2C)   /* MMU Data miss            */
#define EOS_SPARC_INSTRMMU  (SPARC_BASE+0x3C)   /* MMU Instruction miss     */

#if defined(_OS9THREAD)

#ifndef _PTHREAD_H
#include <pthread.h>
#endif

#define errno (*(int *)_pthread_local_slot(_PTHREAD_ERRNO))

#else	/* defined(_OS9THREAD) */

extern int errno;

#endif	/* !defined(_OS9THREAD) */

#if defined(_ANSI_EXT) && defined(_OSK)

#if !defined(_TYPES_H)
#include <types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
error_code _os_perr(path_id, u_int32);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif

#endif /* _ERRNO_H */
