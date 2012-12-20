/****************************************************************************
 *  $Workfile:   pram360.h  $
 *  $Revision:   1.1  $
 *  $Modtime:   19 Feb 1997 21:36:00  $
 *  $Date:   19 Feb 1997 21:47:34  $
 ****************************************************************************
 *	Copyright 1994 by Microware Systems Corporation							*
 *	Reproduced Under License												*
 *																			*
 *	This source code is the proprietary confidential property of			*
 *	Microware Systems Corporation, and is provided to licensee				*
 *	solely for documentation and educational purposes. Reproduction,		*
 *	publication, or distribution in any form to any party other than		*
 *	the licensee is strictly prohibited.									*
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date    Comments                                           By	    *
 *  --- -------- ----------------------------------------------- -------    *
 *    0 06/28/96 Birth						    rds     *
 *    1 07/10/96 Renamed to pram360.h from pram.h to avoid           sr     *
 *               potential conflicts in future.                             *
 *               ---- OS-9000/PPCV2.0.1 Released ----                       *
 *      02/19/97 <***** SPF LAN Comm Pack v3.0 Release ********>            *
 *      05/05/97 <***** SPF LAN Comm Pack v3.1 Release ********>            *
 *      10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>            * 
 *     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              *
 *              $$                    <RELEASE_INFO>                     $$ *
 ****************************************************************************/

/*********************************************************************** 
* NOTICE
* All files contained on this disk are subject to the licensing conditions
* issued by MOTOROLA Inc.
*
* All files are copyright 1993 by MOTOROLA Inc. 
************************************************************************/

/*
 * pram.h
 * Definitions of the parameter area RAM.
 * Note that different structures are overlaid
 * at the same offsets for the different modes
 * of operation.
 */

#if		!defined(_pram_h)
#	define	_pram_h

/*****************************************************************
	HDLC parameter RAM
*****************************************************************/

struct hdlc_pram {
	/*
	 * SCC parameter RAM
	 */
	unsigned short	rbase;		/* RX BD base address */
	unsigned short	tbase;		/* TX BD base address */
	unsigned char	rfcr;		/* Rx function code */
	unsigned char	tfcr;		/* Tx function code */
	unsigned short	mrblr;		/* Rx buffer length */
	unsigned long	rstate;		/* Rx internal state */
	unsigned long	rptr;		/* Rx internal data pointer */
	unsigned short	rbptr;		/* rb BD Pointer */
	unsigned short	rcount;		/* Rx internal byte count */
	unsigned long	rtemp;		/* Rx temp */
	unsigned long	tstate;		/* Tx internal state */
	unsigned long	tptr;		/* Tx internal data pointer */
	unsigned short	tbptr;		/* Tx BD pointer */
	unsigned short	tcount;		/* Tx byte count */
	unsigned long	ttemp;		/* Tx temp */
	unsigned long	rcrc;		/* temp receive CRC */
	unsigned long	tcrc;		/* temp transmit CRC */

	/*
	 * HDLC specific parameter RAM
	 */
	unsigned char	RESERVED1[4];	/* Reserved area */
	unsigned long	c_mask;		/* CRC constant */
	unsigned long	c_pres;		/* CRC preset */
	unsigned short	disfc;		/* discarded frame counter */
	unsigned short	crcec;		/* CRC error counter */
	unsigned short	abtsc;		/* abort sequence counter */
	unsigned short	nmarc;		/* nonmatching address rx cnt */
	unsigned short	retrc;		/* frame retransmission cnt */
	unsigned short	mflr;		/* maximum frame length reg */
	unsigned short	max_cnt;	/* maximum length counter */
	unsigned short	rfthr;		/* received frames threshold */
	unsigned short	rfcnt;		/* received frames count */
	unsigned short	hmask;		/* user defined frm addr mask */
	unsigned short	haddr1;	/* user defined frm address 1 */
	unsigned short	haddr2;	/* user defined frm address 2 */
	unsigned short	haddr3;	/* user defined frm address 3 */
	unsigned short	haddr4;	/* user defined frm address 4 */
	unsigned short	tmp;	/* temp */
	unsigned short	tmp_mb;	/* temp */
};



/*****************************************************************
	UART parameter RAM
*****************************************************************/

/*
 * bits in uart control characters table
 */
#define	CC_INVALID	0x8000		/* control character is valid */
#define	CC_REJ		0x4000		/* don't store char in buffer */
#define	CC_CHAR		0x00ff		/* control character */

/* UART */
struct uart_pram {
	/*
	 * SCC parameter RAM
	 */
	unsigned short	rbase;		/* RX BD base address */
	unsigned short	tbase;		/* TX BD base address */
	unsigned char	rfcr;		/* Rx function code */
	unsigned char	tfcr;		/* Tx function code */
	unsigned short	mrblr;		/* Rx buffer length */
	unsigned long	rstate;		/* Rx internal state */
	unsigned long	rptr;		/* Rx internal data pointer */
	unsigned short	rbptr;		/* rb BD Pointer */
	unsigned short	rcount;		/* Rx internal byte count */
	unsigned long	rx_temp;	/* Rx temp */
	unsigned long	tstate;		/* Tx internal state */
	unsigned long	tptr;		/* Tx internal data pointer */
	unsigned short	tbptr;		/* Tx BD pointer */
	unsigned short	tcount;		/* Tx byte count */
	unsigned long	ttemp;		/* Tx temp */
	unsigned long	rcrc;		/* temp receive CRC */
	unsigned long	tcrc;		/* temp transmit CRC */

	/*
	 * UART specific parameter RAM
	 */
	unsigned char	RESERVED1[8];	/* Reserved area */
	unsigned short	max_idl;	/* maximum idle characters */
	unsigned short	idlc;		/* rx idle counter (internal) */
	unsigned short	brkcr;		/* break count register */

	unsigned short	parec;		/* Rx parity error counter */
	unsigned short	frmer;		/* Rx framing error counter */
	unsigned short	nosec;		/* Rx noise counter */
	unsigned short	brkec;		/* Rx break character counter */
	unsigned short	brkln;		/* Reaceive break length */

	unsigned short	uaddr1;		/* address character 1 */
	unsigned short	uaddr2;		/* address character 2 */
	unsigned short	rtemp;		/* temp storage */
	unsigned short	toseq;		/* Tx out of sequence char */
	unsigned short	cc[8];		/* Rx control characters */
	unsigned short	rccm;		/* Rx control char mask */
	unsigned short	rccr;		/* Rx control char register */
	unsigned short	rlbc;		/* Receive last break char */
};



/*****************************************************************
	BISYNC parameter RAM
*****************************************************************/

struct bisync_pram {
	/*
	 * SCC parameter RAM
	 */
	unsigned short	rbase;		/* RX BD base address */
	unsigned short	tbase;		/* TX BD base address */
	unsigned char	rfcr;		/* Rx function code */
	unsigned char	tfcr;		/* Tx function code */
	unsigned short	mrblr;		/* Rx buffer length */
	unsigned long	rstate;		/* Rx internal state */
	unsigned long	rptr;		/* Rx internal data pointer */
	unsigned short	rbptr;		/* rb BD Pointer */
	unsigned short	rcount;		/* Rx internal byte count */
	unsigned long	rtemp;		/* Rx temp */
	unsigned long	tstate;		/* Tx internal state */
	unsigned long	tptr;		/* Tx internal data pointer */
	unsigned short	tbptr;		/* Tx BD pointer */
	unsigned short	tcount;		/* Tx byte count */
	unsigned long	ttemp;		/* Tx temp */
	unsigned long	rcrc;		/* temp receive CRC */
	unsigned long	tcrc;		/* temp transmit CRC */

	/*
	 * BISYNC specific parameter RAM
	 */
	unsigned char	RESERVED1[4];	/* Reserved area */
	unsigned long	crcc;		/* CRC Constant Temp Value */
	unsigned short	prcrc;		/* Preset Receiver CRC-16/LRC */
	unsigned short	ptcrc;		/* Preset Transmitter CRC-16/LRC */
	unsigned short	parec;		/* Receive Parity Error Counter */
	unsigned short	bsync;		/* BISYNC SYNC Character */
	unsigned short	bdle;		/* BISYNC DLE Character */
	unsigned short	cc[8];		/* Rx control characters */
	unsigned short	rccm;		/* Receive Control Character Mask */
};

/*****************************************************************
	IOM2 parameter RAM
	(overlaid on tx bd[5] of SCC channel[2])
*****************************************************************/
struct iom2_pram {
	unsigned short	ci_data;	/* ci data */
	unsigned short	monitor_data;	/* monitor data */
	unsigned short	tstate;		/* transmitter state */
	unsigned short	rstate;		/* receiver state */
};

/*****************************************************************
	SPI/SMC parameter RAM
	(overlaid on tx bd[6,7] of SCC channel[2])
*****************************************************************/

#define	SPI_R	0x8000		/* Ready bit in BD */

struct spi_pram {
	unsigned short	rbase;		/* Rx BD Base Address */
	unsigned short	tbase;		/* Tx BD Base Address */
	unsigned char	rfcr;		/* Rx function code */
	unsigned char	tfcr;		/* Tx function code */
	unsigned short	mrblr;		/* Rx buffer length */
	unsigned long	rstate;		/* Rx internal state */
	unsigned long	rptr;		/* Rx internal data pointer */
	unsigned short	rbptr;		/* rb BD Pointer */
	unsigned short	rcount;		/* Rx internal byte count */
	unsigned long	rtemp;		/* Rx temp */
	unsigned long	tstate;		/* Tx internal state */
	unsigned long	tptr;		/* Tx internal data pointer */
	unsigned short	tbptr;		/* Tx BD pointer */
	unsigned short	tcount;		/* Tx byte count */
	unsigned long	ttemp;		/* Tx temp */
};

struct smc_uart_pram {
	unsigned short	rbase;		/* Rx BD Base Address */
	unsigned short	tbase;		/* Tx BD Base Address */
	unsigned char	rfcr;		/* Rx function code */
	unsigned char	tfcr;		/* Tx function code */
	unsigned short	mrblr;		/* Rx buffer length */
	unsigned long	rstate;		/* Rx internal state */
	unsigned long	rptr;		/* Rx internal data pointer */
	unsigned short	rbptr;		/* rb BD Pointer */
	unsigned short	rcount;		/* Rx internal byte count */
	unsigned long	rtemp;		/* Rx temp */
	unsigned long	tstate;		/* Tx internal state */
	unsigned long	tptr;		/* Tx internal data pointer */
	unsigned short	tbptr;		/* Tx BD pointer */
	unsigned short	tcount;		/* Tx byte count */
	unsigned long	ttemp;		/* Tx temp */
	unsigned short	max_idl;	/* Maximum IDLE Characters */
	unsigned short	idlc;		/* Temporary IDLE Counter */
	unsigned short	brkln;		/* Last Rx Break Length */
	unsigned short	brkec;		/* Rx Break Condition Counter */
	unsigned short	brkcr;		/* Break Count Register (Tx) */
	unsigned short	r_mask;		/* Temporary bit mask */
};

struct smc_trnsp_pram {
	unsigned short	rbase;		/* Rx BD Base Address */
	unsigned short	tbase;		/* Tx BD Base Address */
	unsigned char	rfcr;		/* Rx function code */
	unsigned char	tfcr;		/* Tx function code */
	unsigned short	mrblr;		/* Rx buffer length */
	unsigned long	rstate;		/* Rx internal state */
	unsigned long	rptr;		/* Rx internal data pointer */
	unsigned short	rbptr;		/* rb BD Pointer */
	unsigned short	rcount;		/* Rx internal byte count */
	unsigned long	rtemp;		/* Rx temp */
	unsigned long	tstate;		/* Tx internal state */
	unsigned long	tptr;		/* Tx internal data pointer */
	unsigned short	tbptr;		/* Tx BD pointer */
	unsigned short	tcount;		/* Tx byte count */
	unsigned long	ttemp;		/* Tx temp */
	unsigned short  reserved[5];	/* Reserved */
};

struct idma_pram {
	unsigned short	ibase;		/* IDMA BD Base Address */
	unsigned short	ibptr;	/* IDMA buffer descriptor pointer */
	unsigned long	istate;	/* IDMA internal state */
	unsigned long	itemp;	/* IDMA temp */
};

struct ethernet_pram {
	/*
	 * SCC parameter RAM
	 */
	unsigned short	rbase;		/* RX BD base address */
	unsigned short	tbase;		/* TX BD base address */
	unsigned char	rfcr;		/* Rx function code */
	unsigned char	tfcr;		/* Tx function code */
	unsigned short	mrblr;		/* Rx buffer length */
	unsigned long	rstate;		/* Rx internal state */
	unsigned long	rptr;		/* Rx internal data pointer */
	unsigned short	rbptr;		/* rb BD Pointer */
	unsigned short	rcount;		/* Rx internal byte count */
	unsigned long	rtemp;		/* Rx temp */
	unsigned long	tstate;		/* Tx internal state */
	unsigned long	tptr;		/* Tx internal data pointer */
	unsigned short	tbptr;		/* Tx BD pointer */
	unsigned short	tcount;		/* Tx byte count */
	unsigned long	ttemp;		/* Tx temp */
	unsigned long	rcrc;		/* temp receive CRC */
	unsigned long	tcrc;		/* temp transmit CRC */

	/*
	 * ETHERNET specific parameter RAM
	 */
	unsigned long	c_pres;		/* preset CRC */
	unsigned long	c_mask;		/* constant mask for CRC */
	unsigned long	crcec;		/* CRC error counter */
	unsigned long	alec;		/* alighnment error counter */
	unsigned long	disfc;		/* discard frame counter */
	unsigned short	pads;		/* short frame PAD characters */
	unsigned short	ret_lim;	/* retry limit threshold */
	unsigned short	ret_cnt;	/* retry limit counter */
	unsigned short	mflr;		/* maximum frame length reg */
	unsigned short	minflr;		/* minimum frame length reg */
	unsigned short	maxd1;		/* maximum DMA1 length reg */
	unsigned short	maxd2;		/* maximum DMA2 length reg */
	unsigned short	maxd;		/* rx max DMA */
	unsigned short	dma_cnt;	/* rx dma counter */
	unsigned short	max_b;		/* max bd byte count */
	unsigned short	gaddr1;		/* group address filter 1 */
	unsigned short	gaddr2;		/* group address filter 2 */
	unsigned short	gaddr3;		/* group address filter 3 */
	unsigned short	gaddr4;		/* group address filter 4 */
	unsigned long	tbuf0_data0;	/* save area 0 - current frm */
	unsigned long	tbuf0_data1;	/* save area 1 - current frm */
	unsigned long	tbuf0_rba0;
	unsigned long	tbuf0_crc;
	unsigned short	tbuf0_bcnt;
	unsigned short	paddr_h;	/* physical address (MSB) */
	unsigned short	paddr_m;	/* physical address */
	unsigned short	paddr_l;	/* physical address (LSB) */
	unsigned short	p_per;		/* persistence */
	unsigned short	rfbd_ptr;	/* rx first bd pointer */
	unsigned short	tfbd_ptr;	/* tx first bd pointer */
	unsigned short	tlbd_ptr;	/* tx last bd pointer */
	unsigned long	tbuf1_data0;	/* save area 0 - next frame */
	unsigned long	tbuf1_data1;	/* save area 1 - next frame */
	unsigned long	tbuf1_rba0;
	unsigned long	tbuf1_crc;
	unsigned short	tbuf1_bcnt;
	unsigned short	tx_len;		/* tx frame length counter */
	unsigned short	iaddr1;		/* individual address filter 1*/
	unsigned short	iaddr2;		/* individual address filter 2*/
	unsigned short	iaddr3;		/* individual address filter 3*/
	unsigned short	iaddr4;		/* individual address filter 4*/
	unsigned short	boff_cnt;	/* back-off counter */
	unsigned short	taddr_h;	/* temp address (MSB) */
	unsigned short	taddr_m;	/* temp address */
	unsigned short	taddr_l;	/* temp address (LSB) */
};

struct transparent_pram {
	/*
	 * SCC parameter RAM
	 */
	unsigned short	rbase;		/* RX BD base address */
	unsigned short	tbase;		/* TX BD base address */
	unsigned char	rfcr;		/* Rx function code */
	unsigned char	tfcr;		/* Tx function code */
	unsigned short	mrblr;		/* Rx buffer length */
	unsigned long	rstate;		/* Rx internal state */
	unsigned long	rptr;		/* Rx internal data pointer */
	unsigned short	rbptr;		/* rb BD Pointer */
	unsigned short	rcount;		/* Rx internal byte count */
	unsigned long	rtemp;		/* Rx temp */
	unsigned long	tstate;		/* Tx internal state */
	unsigned long	tptr;		/* Tx internal data pointer */
	unsigned short	tbptr;		/* Tx BD pointer */
	unsigned short	tcount;		/* Tx byte count */
	unsigned long	ttemp;		/* Tx temp */
	unsigned long	rcrc;		/* temp receive CRC */
	unsigned long	tcrc;		/* temp transmit CRC */

	/*
	 * TRANSPARENT specific parameter RAM
	 */
	unsigned long	crc_p;		/* CRC Preset */
	unsigned long	crc_c;		/* CRC constant */
};

struct timer_pram {
	
	/*
	 * RISC timers parameter RAM
	 */
	unsigned short	tm_base;	/* RISC timer table base adr */
	unsigned short	tm_ptr;		/* RISC timer table pointer */
	unsigned short	r_tmr;		/* RISC timer mode register */
	unsigned short	r_tmv;		/* RISC timer valid register */
	unsigned long	tm_cmd;		/* RISC timer cmd register */
	unsigned long	tm_cnt;		/* RISC timer internal cnt */
};

#endif
