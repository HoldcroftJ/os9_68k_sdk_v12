/*
 * $Header:   /h0/MWOS/SRC/DEFS/HW/VCS/68681.h_v   1.15   22 Jun 1998 11:06:38   alexs  $
 * $Revision:   1.15  $
 */
/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1996 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
| Definitions for the 68681 serial chip.									|
|																			|
| Edition History:															|
|																			|
| Ed#	Date	Changes													By	|
| --- --------	------------------------------------------------------- --- |
|  01 96/05/07  Created                                                 sep |
|  02 96/07/03  Added conditional register mappings.                    ats |
|  03 96/08/07  Added RxRTS definition.                                 ats |
|               -- OS-9/68K V3.0.3 System-state Debug Support Release --    |
|               ---- OS-9000/x86 V2.1 Released ----                         |
|               ---- OS-9000/PPC V2.1.1 Released ----                       |
|               ---- OS-9000/ARMv3 V2.2 Released ----                       |
|               ---- OS-9000/ARMv3 V2.2.1 Released ----                     |
|  04 98/02/16	Changed NOSUP from -1 to max u_char value				rkw |
|               ---- OS-9000 SCF v1.1 Released ----                         |
|               ---- Chipset v1.1 Sub-component Released ----               |
|               ---- OS-9000/505EVB V1.0 Released ----                      |
|               ---- Chipset v1.2 Sub-component Released ----               |
|               $$                    <RELEASE_INFO>                     $$ |
`--------------------------------------------------------------------------*/

/* Default BUS_WIDTH and BYTE_OFFSET definitions */

#ifndef	BUS_WIDTH
#define	BUS_WIDTH		2	/* Conventional layout example definition */
#endif

#ifndef	BYTE_OFFSET
#define	BYTE_OFFSET		0	/* Conventional layout example definition */
#endif

/*  Port-specific Register Definitions  */

#define	PORTA	(0*BUS_WIDTH)+BYTE_OFFSET	/* A-side of chip offset		*/
#define PORTB	(8*BUS_WIDTH)+BYTE_OFFSET	/* B-side of chip offset		*/

#define MR1		(0*BUS_WIDTH)+BYTE_OFFSET	/* Mode Register One			*/
#define MR2		(0*BUS_WIDTH)+BYTE_OFFSET	/* Mode Register Two			*/

#define STATR	(1*BUS_WIDTH)+BYTE_OFFSET	/* Status Register				*/
#define CSR		(1*BUS_WIDTH)+BYTE_OFFSET	/* Clock Select Register		*/

#define CMDR	(2*BUS_WIDTH)+BYTE_OFFSET	/* Command Register				*/

#define RBUF	(3*BUS_WIDTH)+BYTE_OFFSET	/* Receive Buffer				*/
#define TBUF	(3*BUS_WIDTH)+BYTE_OFFSET	/* Transmit Buffer				*/

/*  Part-global Register Definitions    */

#define IPCR	(4*BUS_WIDTH)+BYTE_OFFSET	/* Input Port Change Register	*/
#define ACR		(4*BUS_WIDTH)+BYTE_OFFSET	/* Auxiliary Control Register	*/

#define ISR		(5*BUS_WIDTH)+BYTE_OFFSET	/* Interrupt Status Register	*/
#define IMR		(5*BUS_WIDTH)+BYTE_OFFSET	/* Interrupt Mask Register		*/

#define CTUR	(6*BUS_WIDTH)+BYTE_OFFSET	/*  C/T Upper Preload Reg.		*/
#define CUR		(6*BUS_WIDTH)+BYTE_OFFSET	/*  C/T Upper Register			*/

#define CTLR	(7*BUS_WIDTH)+BYTE_OFFSET	/*  C/T Lower Preload Reg.		*/
#define CLR		(7*BUS_WIDTH)+BYTE_OFFSET	/*  C/T Lower Register			*/

#define IVR		(12*BUS_WIDTH)+BYTE_OFFSET	/*  Interrupt Vector Register	*/

#define IPR		(13*BUS_WIDTH)+BYTE_OFFSET	/*  Input Port Register			*/
#define OPCR	(13*BUS_WIDTH)+BYTE_OFFSET	/*  Output Port Config Register */

#define SOPR	(14*BUS_WIDTH)+BYTE_OFFSET	/*  Set Output Port Register	*/

#define ROPR	(15*BUS_WIDTH)+BYTE_OFFSET	/*  Reset Output Port Register	*/


/*  Register Field Values              */

/*  Mode Register One                  */
#define RxRTS			7			  /* Rx RTS Control						 */

#define PARITY          2             /*  Parity field shift value           */
#define EVEN_PARITY     0             /*  Even Parity                        */
#define ODD_PARITY      1             /*  Odd Parity                         */
#define NO_PARITY       4             /*  No Parity                          */

#define BITS_CHAR       0             /*  Bits per character                 */
#define BIT5            0             /*  Five bits                          */
#define BIT6            1             /*  Six bits                           */
#define BIT7            2             /*  Seven bits                         */
#define BIT8            3             /*  Eight bits                         */

/*  Mode Register Two                  */
#define TxRTS           5             /* Tx RTS Control                      */
#define OUTPORT         0             /* RTS controlled by output port reg.  */
#define TRANSPORT       1             /* RTS controlled by trans. busy/ready */

#define TxCTS           4             /* Tx CTS Control                      */
#define NO_EFFECT       0             /* No effect of CTS on transmitter     */
#define CTSCNTRL        1             /* Transmitter controlled by CTS status*/

#define STOP_BIT        0             /* Stop Bits                           */
#define SB1             7             /* One stop bit                        */
#define SB1P5           8             /* 1.5 stop bits                       */
#define SB2             0x0F          /* Two stop bits                       */

/*  Clock Select Register              */
#define TXCLK           0             /* Transmitter-Clock Select            */
#define RXCLK           4             /* Receiver-Clock Select               */

/*  Baud Rate Set 1 - ACR Bit 7 = 0    */
#define NOSUP           255           /* Baud Rate Not Supported             */
#define BAUD1_50		0             /* 50    Baud                          */
#define BAUD1_110       1             /* 110   Baud                          */
#define BAUD1_134P5     2             /* 134.5 Baud                          */
#define BAUD1_200       3             /* 200   Baud                          */
#define BAUD1_300       4             /* 300   Baud                          */
#define BAUD1_600       5             /* 600   Baud                          */
#define BAUD1_1050      6             /* 1050  Baud                          */
#define BAUD1_1200      7             /* 1200  Baud                          */
#define BAUD1_2400      8             /* 2400  Baud                          */
#define BAUD1_4800      9             /* 4800  Baud                          */
#define BAUD1_7200     10             /* 7200  Baud                          */
#define BAUD1_9600     11             /* 9600  Baud                          */
#define BAUD1_38400    12             /* 38400 Baud                          */

/*  Baud Rate Set 2 - ACR Bit 7 = 1    */
#define BAUD2_75		0             /* 75    Baud                          */
#define BAUD2_110       1             /* 110   Baud                          */
#define BAUD2_134P5     2             /* 134.5 Baud                          */
#define BAUD2_200       3             /* 200   Baud                          */
#define BAUD2_300       4             /* 300   Baud                          */
#define BAUD2_600       5             /* 600   Baud                          */
#define BAUD2_1200      6             /* 1200  Baud                          */
#define BAUD2_1800      7             /* 1800  Baud                          */
#define BAUD2_2000      8             /* 2000  Baud                          */
#define BAUD2_2400      9             /* 2400  Baud                          */
#define BAUD2_4800     10             /* 4800  Baud                          */
#define BAUD2_9600     11             /* 9600  Baud                          */
#define BAUD2_19200    12             /* 19200 Baud                          */

/*  Auxiliary Control Register         */
#define BAUDRATE        7             /* Baud Rate                           */
#define BAUDSET1        0             /* Baud Rate Set 1                     */
#define BAUDSET2        1             /* Baud Rate Set 2                     */

/*  Command Register                   */

#define RXCMDS          0             /* Receiver Commands                   */
#define TXCMDS          2             /* Transmitter Commands                */
#define ENABLE          1             /* Enable Receiver OR Transmitter      */
#define DISABLE         2             /* Disable Reciever OR Transmitter     */

#define MISCMDS         4             /* Miscellaneous Commands              */
#define RSTMODE         1             /* Reset mode register pointer to reg 1*/
#define RSTRX           2             /* Reset receiver, flush FIFO          */
#define RSTTX           3             /* Reset transmitter                   */
#define RSTERROR        4             /* Reset error status bits             */

/*  Status Register                    */
#define RXRDY           0             /* Receiver Ready                      */
#define RXFIFOFULL      1             /* Receiver FIFO Full                  */
#define TXRDY           2             /* Transmitter Ready                   */
#define TXEMPTY         3             /* Transmitter Empty                   */
#define OVRERR          4             /* Overrun Error                       */
#define PARITYERR       5             /* Parity Error                        */
#define FRAMERR         6             /* Framing Error                       */
#define RCVBREAK        7             /* Received Break                      */

/*  Set Output Port Register           */
/*  Reset Output Port Register         */
#define CHARTS          0             /* Channal A Request To Send           */
#define CHBRTS          1             /* Channal B Request To Send           */
#define CHADTR          2             /* Channal A Data Terminal Ready       */
#define CHBDTR          3             /* Channal B Data Terminal Ready       */

#define ACTIVATE        1             /* Activate hardware signal            */
#define DEACTIVATE      1             /* DeActivate hardware signal          */

/*  Interrupt Status Register          */
/*  Interrupt Mask Register            */
#define CHATXRDY        0             /* Channel A Transmitter Ready         */
#define CHARXRDY        1             /* Channel A Receiver Ready            */
#define CHABRK          2             /* Change of Channel A Break State     */
#define TIMRDY          3             /* Timer Ready                         */
#define CHBTXRDY        4             /* Channel B Transmitter Ready         */
#define CHBRXRDY        5             /* Channel B Receiver Ready            */
#define CHBBRK          6             /* Change of Channel B Break State     */
#define IPCHG           7             /* Change of Input Port State          */

#define INT_ENABLE      1             /* Enable interrupt                    */
#define INT_MASK        0             /* Mask interrupt                      */

/*  Interrupt Vector Register          */
