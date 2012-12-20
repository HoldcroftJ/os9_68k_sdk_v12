/****************************************************************************
 *  Copyright 1994 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *   0 06/28/96 Birth                                                  ???  *
 *   1 07/10/96 Renamed to regs360.h from registers.h to avoid         sr   *
 *              potential conflicts in future.                              *
 *              ---- OS-9000/PPCV2.0.1 Released ----                        *
 *     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->       *
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->       *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>             *
 *     04/02/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->       *
 *  18 12/28/99 Added GSMR_ENT and GSMR_ENR                            rds  *
 ****************************************************************************/

/***********************************************************************
* NOTICE
* All files contained on this disk are subject to the licensing conditions
* issued by MOTOROLA Inc.
*
* All files are copyright 1993 by MOTOROLA Inc.
************************************************************************/

/*
 * registers.h
 * Definitions of the QUICC registers
 */


/*****************************************************************
    Base Address Register (address 0x2ff00)
*****************************************************************/

#define	WRITE_BAR(x)	(*((long *)0x2ff00) = x)


/*****************************************************************
	Command Register
*****************************************************************/

/* bit fields within command register */
#define	SOFTWARE_RESET	0x8000
#define	CMD_OPCODE	0x0f00
#define	CMD_CHANNEL	0x00f0
#define	CMD_FLAG	0x0001

/* general command opcodes */
#define	INIT_RXTX_PARAMS	0x0000
#define	INIT_RX_PARAMS		0x0100
#define	INIT_TX_PARAMS		0x0200
#define	ENTER_HUNT_MODE		0x0300
#define	STOP_TX			0x0400
#define	GR_STOP_TX		0x0500
#define	RESTART_TX		0x0600
#define	CLOSE_RX_BD		0x0700
#define	SET_ENET_GROUP		0x0800
#define	RESET_ENET_GROUP	0x0900

/* SMC (GCI) commands */
#define	GCI_ABORT		0x0400
#define	GCI_TIMEOUT		0x0500

/* Timer commands */
#define	SET_TIMER		0x0800




/*****************************************************************
	General SCC mode register (GSMR)
*****************************************************************/
/* GSMRA */
/* SCC modes */
#define	HDLC_PORT		0x0
#define	HDLC_BUS		0x1
#define	APPLE_TALK		0x2
#define	SS_NO7			0x3
#define	UART			0x4
#define	PROFI_BUS		0x5
#define	ASYNC_HDLC		0x6
#define	V14			0x7
#define	BISYNC_PORT		0x8
#define	DDCMP_PORT		0x9
#define	ETHERNET_PORT		0xc

/*
** Enable transmit and receive
*/
#define GSMR_ENT 0x10
#define GSMR_ENR 0x20

/*
 * diagnostics bits
 */
#define	NORMAL		(0<<6)	/* normal operation with CTS and CD */
				/* automatically controlled by SCC */
#define	LOOPBACK	(1<<6)	/* connect tx to rx */
#define	SERIAL_ECHO	(2<<6)	/* connect rx to tx */
#define	LOOPBACK_ECHO	(3<<6)	/* loopback and echo */

/* common mode register bits */
#define	MODE		0x0000000f

/* GSMRB */
#define	TCRC		0x00008000
#define	TRX		0x00001000
#define	TTX		0x00000800

/* hdlc mode register */
#define	CRC32		0x0800


/* uart mode register */

/* ethernet mode register */
#define	IAM		0x1000	/* individual address mode */
#define	PRO		0x0200	/* promiscuous */
#define	BRO		0x0100	/* broadcast address */

/*
 * tx parity (SCC asynchronous)
 */
#define	ODD_PARITY	(0<<14)
#define	EVEN_PARITY	(1<<14)
#define	LOW_PARITY	(2<<14)
#define	HIGH_PARITY	(3<<14)

/*
 * UART options
 */
#define	UOPT		0x0c00
#define	OPT_NORMAL	(0<<10)
#define	OPT_MULTD	(1<<10)	/* multi-drop, no auto */
#define	OPT_DDCMP	(2<<10)	/* ddcmp */
#define	OPT_AUTO	(3<<10)	/* mutli-drop with automatic */
				/* address recognition */
#define	UART_FRZ	0x0200


/* bisync mode register */
#define	BI_CRC		0x0800	/* BI_CRC set than LRC is used */
#define	BI_BCS		0x0200
#define	BI_TRANSP	0x0100


/*****************************************************************
	TODR (Transmit on demand) Register
*****************************************************************/
#define	TODR_TOD	0x8000	/* Transmit on demand */

/*****************************************************************
	SMC mode register
*****************************************************************/

/*
 * defines for SMC modes
 */
#define	SMC_MODE	0x0030	/* SMC MODE */
#define	SMC_GCI_MODE	0x0000	/* SMC MODE: GCI or SCIT support */
#define	SMC_UART	0x0020	/* SMC MODE: UART */
#define	SMC_TRANSPARENT	0x0030	/* SMC MODE: Transparent */
/*****************************************************************
	SPI registers
*****************************************************************/

#define MULTI_MASTER_ERR        ABORT /* Multi master error event */


/*****************************************************************
	interrupt registers
*****************************************************************/

#define	INTR_PIO_PC0	0x80000000	/* parallel i/o C bit 0 */
#define	INTR_SCC1	0x40000000	/* SCC port 1 */
#define	INTR_SCC2	0x20000000	/* SCC port 2 */
#define	INTR_SCC3	0x10000000	/* SCC port 3 */
#define	INTR_SCC4	0x08000000	/* SCC port 4 */
#define	INTR_PIO_PC1	0x04000000	/* parallel i/o C bit 1 */
#define	INTR_TIMER1	0x02000000	/* timer 1 */
#define	INTR_PIO_PC2	0x01000000	/* parallel i/o C bit 2 */
#define	INTR_PIO_PC3	0x00800000	/* parallel i/o C bit 3 */
#define	INTR_SDMA_BERR	0x00400000	/* SDMA channel bus error */
#define	INTR_DMA1	0x00200000	/* idma 1 */
#define	INTR_DMA2	0x00100000	/* idma 2 */
#define	INTR_TIMER2	0x00040000	/* timer 2 */
#define	INTR_CP_TIMER	0x00020000	/* CP timer */
#define	INTR_PIP_STATUS	0x00010000	/* PIP status */
#define	INTR_PIO_PC4	0x00008000	/* parallel i/o C bit 4 */
#define	INTR_PIO_PC5	0x00004000	/* parallel i/o C bit 5 */
#define	INTR_TIMER3	0x00001000	/* timer 3 */
#define	INTR_PIO_PC6	0x00000800	/* parallel i/o C bit 6 */
#define	INTR_PIO_PC7	0x00000400	/* parallel i/o C bit 7 */
#define	INTR_PIO_PC8	0x00000200	/* parallel i/o C bit 8 */
#define	INTR_TIMER4	0x00000080	/* timer 4 */
#define	INTR_PIO_PC9	0x00000040	/* parallel i/o C bit 9 */
#define	INTR_SPI	0x00000020	/* SPI */
#define	INTR_SMC1	0x00000010	/* SMC 1 */
#define	INTR_SMC2	0x00000008	/* SMC 2 */
#define	INTR_PIO_PC10	0x00000004	/* parallel i/o C bit 10 */
#define	INTR_PIO_PC11	0x00000002	/* parallel i/o C bit 11 */
#define	INTR_ERR	0x00000001	/* error */



/*****************************************************************
        Spi register setting
*****************************************************************/

#define NORMAL_MODE     0x18
#define SPI_START       0x80 /* STR bit in SPCOM, start transmit */
#define CLEAR_SPIE      0xff
#define SPI_MODE	0x50 /* Cp spi mode for init Rx Tx */

/* event bits in SPIE */
#define MME             0x20
#define TXE             0x10
#define BSY             0x04
#define TXB             0x02
#define RXB             0x01

/*
 * transmit BD's status (For Spi)
 */
#define T_L             0x0800 /* Last bit of frame */

/*****************************************************************
	chip select option register
*****************************************************************/
#define	DTACK		0xe000
#define	ADR_MASK	0x1ffc
#define	RW_MASK		0x0002
#define	FC_MASK		0x0001

/*****************************************************************
	tbase and rbase registers
*****************************************************************/

#define	TBD_ADDR(w)	((struct quicc_bd *) \
			(w->quicc->udata_bd_ucode +\
					w->pram->pscc.h.tbase))
#define	RBD_ADDR(w)	((struct quicc_bd *) \
			(w->quicc->udata_bd_ucode +\
					w->pram->pscc.h.rbase))
/*****************************************************************
	SMC tbase and rbase registers
*****************************************************************/

#define	SMC_TBD_ADDR(w)	((struct quicc_bd *) \
			(w->quicc->udata_bd_ucode +\
			w->pram->pothers.idma_smc.psmc.u.tbase))
#define	SMC_RBD_ADDR(w)	((struct quicc_bd *) \
			(w->quicc->udata_bd_ucode +\
			w->pram->pothers.idma_smc.psmc.u.rbase))

/*****************************************************************
         Port B Pin Assignment
*****************************************************************/
#define PB10TO17        0x0003FC00
#define PB1TO3          0x0000000E
#define PB0             0x00000001

/*****************************************************************/

