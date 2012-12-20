/****************************************************************************
 *  ID:     %A%
 *  Date:   %G%
 ****************************************************************************
 *  Z8530 generic definitions.												*
 ****************************************************************************
 *  Copyright 1994 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 *  Edition History:                                                        *
 *   #   Date       Comments                                           By   *
 *  --- --------    ----------------------------------------------- ------- *
 *    0 01/11/94    Creation (from RTNFM driver)                        bat *
 *    1 01/11/94    More additions                                      bat *
 *    2 02/08/94    Split out board-specific definitions                bat *
 *      03/15/94    <***************** SPF v1.0 ******************>         *
 *      06/30/95    <--------------- SPF v2.0(pre) --------------->     cas *
 *      06/26/97    <--------------- SPF V2.2 -------------------->     taka*
 *      03/23/98    fixed typo to remove compiler warnings              taka*
 *      03/23/98    <--------------- SPF v2.2.1 ------------------>     taka*
 *      07/20/98    <--------------- SPF v2.2.2 ------------------>     taka*
 *    3 12/12/98	Added FOS, FDA bit definitions for Status FIFO		mgh *
 ****************************************************************************/


#ifndef _Z8530_H
#define _Z8530_H


/**************** Zilog 8530 Register Macro Definitions *********************/


/*** Read Register #0 ***/
/*     Transmit and Receive Buffer Status and External Status */
#define RXCHAR_AVAIL		0x01
#define ZERO_CNT			0x02
#define TXBUF_EMPTY			0x04
#define DCD					0x08
#define SYNC_HUNT			0x10
#define CTS					0x20
#define TX_URUN_EOM			0x40
#define BRK_ABO				0x80

/*** Read Register #1 ***/
/*     Special Receive Condition Status */
#define ALL_SENT			0x01
#define RESIDUE_2			0x02
#define RESIDUE_1			0x04
#define RESIDUE_0			0x08
#define RESIDUE_BITS		0x0E
#define PAR_ERR				0x10
#define RX_ORUN_ERR			0x20
#define CRC_FRM_ERR			0x40
#define EOF_SDLC			0x80
#define REG1_ERR_MASK		0x70

/*** Read/Write Register #2 ***/
/*     Channel A: Unmodified Interrupt Vector */
/*     Channel B: Modified Interrupt Vector */

/*** Read Register #3 ***/
/*     Channel A: Interrupt Pending Bits */
/*     Channel B: Undefined */
#define CHA_STAT_IP			0x08
#define CHA_TX_IP			0x10
#define CHA_RX_IP			0x20
#define CHA_INT				(CHA_STAT_IP|CHA_TX_IP|CHA_RX_IP)
#define CHB_STAT_IP			0x01
#define CHB_TX_IP			0x02
#define CHB_RX_IP			0x04
#define CHB_INT				(CHB_STAT_IP|CHB_TX_IP|CHB_RX_IP)

/*** Read Register #4 ***/
/*     Trasmit and Receive Modes and Parameters */
/*     Contents of Write Register #4 */

/*** Read Register #5 ***/
/*     Transmit Parameters and Control Modes */
/*     Contents of Write Register #5 */

/*** Read Register #6 ***/
/*     NMOS: Undefined */
/*     Others: SDLC FIFO Byte Counter Lower Bytes (Only When Enabled) */

/*** Read Register #7 ***/
/*     NMOS: Undefined */
/*     Others: SDLD FIFO Byte Count and Status (Only When Enabled) */
#define FDA					0x40
#define	FOS					0x80

/*** Read Register #8 ***/
/*     Receive Buffer */

/*** Read Register #9 ***/
/*     Receive Parameters and Control Modes */
/*     Contents of Write Register #3 */

/*** Read Register #10 ***/
/*     Miscellaneous Status Bits */
#define ON_LOOP				0x02
#define LOOP_SENDING		0x10
#define TWO_CLK_MISSING		0x40
#define ONE_CLK_MISSING		0x80

/*** Read Register #11 ***/
/*     Miscellaneous Transmit and Receive Control Bits */
/*     Contents of Write Register #10 */

/*** Read Register #12 ***/
/*     Lower Byte of Baud Rate Generator Time Constant */

/*** Read Register #13 ***/
/*     Upper Byte of Baud Rate Generator Time Constant */

/*** Read Register #14 ***/
/*     ESCC: Extended Feature and FIFO Control */
/*     Others: Undefined */

/*** Read Register #15 ***/
/*     External Status Interrupt Information */
#define ZERO_CNT_IE			0x02
#define DCD_IE				0x08
#define SYNC_HUNT_IE		0x10
#define CTS_IE				0x20
#define TX_URUN_EOM_IE		0x40
#define BRK_ABO_IE			0x80


/*** Write Register #0 ***/
/*     Register Pointers & Various Initialization Commands */
#define REG_CLEAR			0x00
#define	RES_TX_URUN_EOM		0xC0
#define	RES_TXCRC_GEN		0x80
#define RES_RXCRC_CHK		0x40
#define	RES_HIGHEST_IUS		0x38
#define	ERR_RES				0x30
#define	RES_TXINT_PNDG		0x28
#define	RXCHAR_EN			0x20
#define	SEND_SDLC_ABO		0x18
#define	RES_STAT_INT		0x10

/*** Write Register #1 ***/
/*     Transmit and Receive Interrupt Enables & WAIT/DMA Commands */
#define ALL_INT_CLEAR		0x00
#define WT_DMAREQ_EN		0x80
#define WT_DMAREQ_FUNC		0x40
#define WT_DMAREQ_RXTX		0x20
#define RX_INT_DIS			0x00
#define RX_INT_1ST_CHAR		0x08
#define RX_INT_ALL_CHAR		0x10
#define RX_INT_SPEC_COND	0x18
#define PARITY_SPEC_COND	0x04
#define TX_INT_EN			0x02
#define EXT_INT_EN			0x01

/*** Write Register #2 ***/
/*     Interrupt Vector */

/*** Write Register #3 ***/
/*     Receive Parameters and Control Modes */
#define RX5BPC				0x00
#define RX7BPC				0x40
#define RX6BPC				0x80
#define RX8BPC				0xC0
#define AUTO_EN				0x20
#define ENTER_HUNT_MODE		0x10
#define RX_CRC_EN			0x08
#define ADDR_SRCH			0x04
#define SCHARLOAD_INH		0x02
#define RX_EN				0x01

/*** Write Register #4 ***/
/*     Transmit and Receive Modes and Parameters */
#define CLKx1				0x00
#define CLKx16				0x40
#define CLKx32				0x80
#define CLKx64				0xC0
#define MODE_MONOSYNC		0x00
#define MODE_BISYNC			0x10
#define MODE_SDLC			0x20
#define MODE_EXTSYNC		0x30
#define STOPBIT_SYNC		0x00
#define STOPBIT_1			0x04
#define STOPBIT_1HALF		0x08
#define STOPBIT_2			0x0C
#define PARITY_EVEN			0x02
#define PARITY_ODD			0x00
#define PARITY_ON			0x01
#define PARITY_OFF			0x00

/*** Write Register #5 ***/
/*     Transmit Parameters and Control Modes */
#define DTR					0x80
#define TX5BPC				0x00
#define TX7BPC				0x20
#define TX6BPC				0x40
#define TX8BPC				0x60
#define SEND_BRK			0x10
#define TX_EN				0x08
#define SDLC_CRC16			0x04
#define SDLC_CRC			0x00
#define RTS					0x02
#define TX_CRC_EN			0x01

/*** Write Register #6 ***/
/*     Sync Character or SDLC Address */

/*** Write Register #7 ***/
/*     ESCC: Extended Feature and FIFO Control */
/*     Others: Sync Character or SDLC Flag */
#define SDLC_FLAG			0x7E

/*** Write Register #7' ***/
/*     ESCC ONLY: Extended Features */
#define EXT_READ_EN			0x80
#define TX_FIFO_LEVEL		0x20
#define DTR_REG_TIMING		0x10
#define RX_FIFO_LEVEL		0x08
#define AUTO_RTS_DEACT		0x04
#define AUTO_EOM_RST		0x02
#define AUTO_TX_FLAG		0x01

/*** Write Register #8 ***/
/*     Transmit Buffer */

/*** Write Register #9 ***/
/*     Master Interrupt Control and Reset Commands */
#define INTS_OFF			0x00
#define RESET_CHA			0x80
#define RESET_CHB			0x40
#define RESET_8530			(RESET_CHA|RESET_CHB)
#define INTACK_EN			0x20
#define STATUS_HI			0x10
#define STATUS_LO			0x00
#define MSTR_IRQEN			0x08
#define DSABL_LOCHAIN		0x04
#define NOVECT_SEL			0x02
#define INCL_VECTSTAT		0x01

/*** Write Register #10 ***/
/*     Miscellaneous Transmit and Receive Control Bits */
#define CRC_INIT_0			0x00
#define CRC_INIT_1			0x80
#define ENCODE_NRZ			0x00
#define ENCODE_NRZI			0x20
#define ENCODE_FM1			0x40
#define ENCODE_FM0			0x60
#define ACTIVE_POLL			0x10
#define IDLE_MARK			0x08
#define IDLE_FLAG			0x00
#define URUN_ABTFLG			0x04
#define LOOPMODE_ON			0x02
#define LOOPMODE_OFF		0x00
#define SYNC_8BIT			0x00
#define SYNC_6BIT			0x01

/*** Write Register #11 ***/
/*     Clock Mode Controls for Receive and Transmit */
#define RTxC_XTAL			0x80
#define RTxC_NOXTAL			0x00
#define RXCLK_RTxC			0x00
#define RXCLK_TRxC			0x20
#define RXCLK_BR			0x40
#define RXCLK_DPLL			0x60
#define TXCLK_RTxC			0x00
#define TXCLK_TRxC			0x08
#define TXCLK_BR			0x10
#define TXCLK_DPLL			0x18
#define TRxC_OUT			0x04
#define TRxC_IN				0x00
#define TRxC_XTAL			0x00
#define TRxC_TXCLK			0x01
#define TRxC_BR				0x02
#define TRxC_DPLL			0x03

/*** Write Register #12 ***/
/*     Lower Byte of Baud Rate Generator */

/*** Write Register #13 ***/
/*     Upper Byte of Baud Rate Generator */

/*** Write Register #14 ***/
/*     Miscellaneous Control Bits */
#define ENTER_SEARCH		0x20
#define RESET_CLK			0x40
#define DPLL_OFF			0x60
#define SOURCE_BRGEN		0x80
#define SOURCE_RTxC			0xA0
#define FM_MODE				0xC0
#define NRZI_MODE			0xE0
#define LOOPBACK			0x10
#define AUTO_ECHO			0x80
#define DTR_REQ				0x04
#define BR_GEN_SRC			0x02
#define BR_GEN_ON			0x01
#define BR_GEN_OFF			0x00

/*** Write Register #15 ***/
/*     External Status Interrupt Enable Control */
#define EXT_STAT_OFF		0x00
#define BRK_ABT_ENBL		0x80
#define TXUR_EOM_ENBL		0x40
#define CTS_ENBL			0x20
#define SYNC_HUNT_ENBL		0x10
#define DCD_ENBL			0x08
#define STAT_FIFO_ENBL		0x04
#define ZEROCNT_ENBL		0x02
#define WR7PRIME_ENBL		0x01	/* ESCC ONLY!!! */
#define WR7PRIME_DSBL		0x00	/* ESCC ONLY!!! */


/**************** Zilog 8530 Miscellaneous Definitions **********************/

/* Z8530 Time Constant (Baud Rate)											*
 *  "baud" must be the desired baud rate (in bits/second)					*
 *  "clock" must be the incoming clock frequency (in Hertz)					*
 *  "period" must be the baud rate clock period (in seconds) [usually 1]	*/
#define TC_8530(baud,clock,period)  \
	((clock / (2 * baud * period)) - 2)


#endif /* _Z8530_H */
