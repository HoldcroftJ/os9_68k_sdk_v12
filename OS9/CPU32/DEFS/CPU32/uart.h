/*********************************************************************** 
* NOTICE
* All files contained on this disk are subject to the licensing conditions
* issued by MOTOROLA Inc.
*
* All files are copyright 1993 by MOTOROLA Inc. 
************************************************************************/

/*
 * uart.h
 * definitions for the UART controllers
 */

/*
 * transmit BD's
 */
#define	T_R	0x8000		/* ready bit */
#define	T_W	0x2000		/* wrap bit */
#define	T_I	0x1000		/* interrupt on completion */
#define	T_CR	0x0800		/* clear to send report */
#define	T_A	0x0400		/* address characters in buffer */
#define	T_CM	0x0200		/* continuous mode */
#define	T_P	0x0100		/* send preamble before data */
#define	T_NS	0x0080		/* no stop bit in transmit */

#define	UART_T_ERROR	0x000f
#define	T_CT	0x0001		/* error: CTS lost */

/*
 * receive BD's
 */
#define	R_E	0x8000		/* buffer empty */
#define	R_W	0x2000		/* wrap bit */
#define	R_I	0x1000		/* interrupt on reception */
#define	R_C	0x0800		/* buffer contains control character */
#define	R_A	0x0400		/* buffer contains address character */
#define	R_K	0x0200		/* kind of address match */
#define	R_ID	0x0100		/* buffer closed due to idles */
#define	R_AM	0x0080		/* address match */

#define	R_ERROR	0x00ff
#define	R_BRK	0x0020		/* break character received */
#define	R_FR	0x0010		/* framing error */
#define	R_PR	0x0008		/* parity error */
#define	R_OV	0x0002		/* receive overrun */
#define	R_CD	0x0001		/* carrier detect lost */

/*
 * uart interrupts
 */
#define	UART_GLr	0x1000	/* Glitch on rx */
#define	UART_GLt	0x0800	/* Glitch on tx */
#define	UART_AB		0x0200	/* Auto band */
#define	UART_IDLE	0x0100	/* idle sequence status changed */
#define	UART_GRA	0x0080	/* Graceful stop complete */
#define	UART_BRKe	0x0040	/* break end */
#define	UART_BRKs	0x0020	/* break start */
#define	UART_CCR	0x0008	/* control character received */
#define	UART_BUSY	0x0004	/* busy */
#define	UART_TX		0x0002	/* transmit buffer */
#define	UART_RX		0x0001	/* receive buffer */
