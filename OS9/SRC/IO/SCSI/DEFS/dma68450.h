/*
 * definitions for 68450 DMA
 */

/*
 * Copyright 1989 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 * history:
 *
 * ed.  date   reason.                                         who.
 * -- -------  ----------------------------------------------  ---
 * 00 89/20/26 history starts.                                 ???
 *				---- OS-9/68K V3.0 Release ----
 *				---- OS-9/68K V3.1 Release ----
 */

/*------------------------------!
! DMA 68450 device definitions	!
!------------------------------*/

/*----------------------------------------------------------------------!
! NOTE: All the register offsets (except general control register) must	!
have the channel number appended.  The channel number times 64 plus		!
the register offset given here will reach the correct register.			!
!----------------------------------------------------------------------*/

typedef struct dmachannel {
	u_char		dh_status,		/* channel status register (byte)	*/
				dh_error,		/* channel error register (byte)	*/
				dh_nu1[2],		/* unused locations (byte)			*/
				dh_devctrl,		/* device control register (byte)	*/
				dh_opsctrl,		/* operation control register (byte)*/
				dh_seqctrl,		/* sequence control register (byte)	*/
				dh_chlctrl,		/* channel control register (byte)	*/
				dh_nu2[2];		/* more unused locations (byte)		*/
	u_int16		dh_memtfrct;	/* memory transfer counter (word)	*/
	u_char		*dh_memaddr;	/* memory address register (long)	*/
	u_int32		dh_nu3;			/* more unused locations (long)		*/
	u_char		*dh_devaddr;	/* device address register (long)	*/
	u_int16		dh_nu4,			/* more unused locations (word)		*/
				dh_bastfrct;	/* base transfer register (word)	*/
	u_int32		dh_basaddr;		/* base address register (long)		*/
	u_char		dh_nu5[5],		/* more unused locations (byte)		*/
				dh_nmintvct,	/* normal interrupt vector (byte)	*/
				dh_nu6,			/* more unused locations (byte)		*/
				dh_erintvct,	/* error interrupt vector (byte)	*/
				dh_nu7,
				dh_memfuncd,	/* memory function codes (byte)		*/
				dh_nu8[3],
				dh_chlpri,		/* channel priority register (byte)	*/
				dh_nu9[3],
				dh_devfuncd,	/* device function codes (byte)		*/
				dh_nu10[7],
				dh_basfuncd;	/* base function codes (byte)		*/
} dmahardware, *Dmahardware;

#define CHNLSIZE	0x40		/* size of one channel register set	*/
#define GENCTLRG	0xFF		/* general control register (byte)	*/


/*------------------------------!
! device control register (R/W)	!
!------------------------------*/
#define BURSTMOD	0x00	/* burst transfer mode				*/
#define STEALMOD	0x80	/* cycle steal mode without hold	*/
#define STEALHLD	0xC0 	/* cycle steal mode with hold		*/

#define DEV68000	0x00 	/* 68K compatible device, explicitly addressed	*/
#define DEVC6800	0x10 	/* 6800 compatible device, explicitly addressed	*/
#define DEVACK  	0x20 	/* device with *ACK, implicitly addressed		*/
#define DEVACKRY	0x30 	/* device with *ACK & *READY, implicitly addressed*/

#define DEV8BIT 	0x00	/* device port 8 bit				*/
#define DEV16BIT	0x08 	/* device port 16 bit				*/
#define DEVSIZ_B	3 		/* bit number of device port size	*/

#define STATINP 	0		/* status input - peripheral ctl line	*/
#define STATINPI	1 		/* status input with interrupt			*/
#define STARTPLS	2		/* start pulse, negative 1/8 clk		*/
#define ABORTINP	3		/* abort input							*/

/*----------------------------------!
! Operation control register (R/W)	!
!----------------------------------*/
#define MEMTODEV	0x00	/* transfer from memory to device	*/
#define DEVTOMEM	0x80	/* transfer from device to memory	*/

#define BYTESIZE	0x00	/* operation size = byte			*/
#define WORDSIZE	0x10	/* operation size = word			*/
#define LONGSIZE	0x20	/* operation size = long			*/

#define CHAINDIS	0x0		/* chain operation disabled			*/
#define CHAINARR	0x8		/* array chaining enabled			*/
#define CHAINLNK	0xC		/* linked chaining enabled			*/

#define AUREQLIM	0		/* auto request at rate set by GCR				*/
#define AUREQMAX	1		/* auto request at maximum rate					*/
#define REQINIT 	2		/* *REQ line intitiates all operand transfers	*/
#define REQINITA	3		/* auto request first xfr, *REQ for all others	*/

/*----------------------------------!
! Sequence Control Register (R/W)	!
!----------------------------------*/
#define MEMNOCNT	0		/* memory address register does not count	*/
#define MEMCNTUP	4		/* memory address register counts up		*/
#define MEMCNTDN	8		/* memory address register counts down		*/

#define DEVNOCNT	0		/* device address register does not count	*/
#define DEVCNTUP	1		/* device address register counts up		*/
#define DEVCNTDN	2		/* device address register counts down		*/

/*----------------------------------!
! Channel Control Register (R/W)	!
!----------------------------------*/
#define NOOPPEND	0x00	/* no operation is pending				*/
#define STARTOP 	0x80	/* start operation						*/
#define START_B 	7		/* bit number of start operation bit	*/

#define NOCONTIN	0x00	/* no continue operation is pending		*/
#define CONTINOP	0x40	/* continue operation					*/
#define CONTIN_B	6		/* bit number of continue op bit		*/

#define OPNOHALT	0x00	/* operation not halted					*/
#define OPHALTED	0x20	/* operation halted						*/
#define HALTED_B	5		/* bit number of halted op bit			*/

#define NOABORT 	0x00	/* operation not aborted				*/
#define OPABORT 	0x10	/* operation aborted					*/
#define ABORT_B 	4		/* bit number of abort op bit			*/

#define INTRPTDI	0		/* interrupts disabled					*/
#define INTRPTEN	8		/* interrupts enabled					*/
#define INTRPT_B	3		/* bit number of interrupt enable		*/

/*------------------------------!
! Channel Status Register (R/W)	!
!---------------------------------------------------!
! writing a one into any bit clears that status		!
! any written zero bits do not affect the status	!
!--------------------------------------------------*/
#define OPNOCOMP	0x00	/* operation incomplete					*/
#define OPERCOMP	0x80	/* operation complete					*/
#define OPCOMP_B	7		/* bit number of operation complete bit	*/

#define BLKNOCMP	0x00	/* block transfer incomplete					*/
#define BLKCOMP 	0x40	/* block transfer complete						*/
#define BLKCMP_B	6		/* bit number of block transfer complete bit	*/

#define DEVTRMAB	0x00	/* device termination abnormal				*/
#define DEVTRMNO	0x20	/* device termination normal				*/
#define DEVTRM_B	5 		/* bit number of device termination status	*/

#define ERRORSET	0x10	/* error occurred and is noted in CER	*/
#define ERROR_B 	4		/* bit number of error flag bit			*/

#define ACTIVECH	8		/* channel considered active				*/
#define ACTIVE_B	3		/* bit number of active channel flag bit	*/

#define PCLTRANS	2		/* transition occurred on *PCL				*/
#define PCLTRN_B	1		/* bit number of PCL transition flag bit	*/

#define PCLLOW  	0		/* *PCL line low					*/
#define PCLHIGH 	1		/* *PCL line high					*/
#define PCLSTS_B	0		/* bit number of *PCL status bit	*/

/*----------------------------------!
! Channel Error Register (R only)	!
!----------------------------------*/
#define ERCONFIG	0x01	/* configuration error			*/
#define EROPTIMG	0x02	/* operation timing error		*/

#define ERADRMEM	0x05	/* memory address error			*/
#define ERADRDEV	0x06	/* device address error			*/
#define ERADRBAS	0x07	/* base address error			*/

#define ERBUSMEM	0x09	/* memory bus error				*/
#define ERBUSDEV	0x0A	/* device bus error				*/
#define ERBUSBAS	0x0B	/* base bus error				*/

#define ERCNTMEM	0x0D	/* memory count error			*/
#define ERCNTDEV	0x0E	/* device count error			*/
#define ERCNTBAS	0x0F	/* base count error				*/

#define EREABORT	0x10	/* external abort				*/
#define ERSABORT	0x11	/* software abort				*/

/*----------------------------------!
! Channel Priority Register (R/W)	!
!----------------------------------*/
#define CHPRIOR0	0		/* channel priority of zero		*/
#define CHPRIOR1	1		/* channel priority of one		*/
#define CHPRIOR2	2		/* channel priority of two		*/
#define CHPRIOR3	3		/* channel priority of three	*/

/*------------------------------!
! Function Code Registers (R/W)	!
!------------------------------*/
#define USERDATA	1		/* user data address access				*/
#define USERPROG	2		/* user program address access			*/
#define SUPVDATA	5		/* supervisor data address access		*/
#define SUPVPROG	6		/* supervisor program address access	*/

/*----------------------------------!
! General Control Register (R/W)	!
!----------------------------------*/
#define BURSTTIM	0x0C	/* mask for burst time			*/
#define BANDWRAT	0x03	/* mask for bandwidth ratio		*/
