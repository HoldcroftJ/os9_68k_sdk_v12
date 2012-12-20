/****************************************************************************
 *  Low Level Ethernet Driver for QUICC pin setup and  control.             *
 ****************************************************************************
 *  Copyright 1997-1998 by Microware Systems Corporation                    *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 *  01 97/06/26 Created.                                           ahms/ats *
 *  09 08/11/97 Added SCC channel dependent code                       taka *
 *  11 10/13/97 Added 68360 support                                    taka *
 *     10/22/97 <***** OS-9000 MBX BSP Release v1.0 **********>             *
 *  14 01/21/98 Removed compiler and linker warnings                     jk *
 *     04/02/98 <***** SPF LAN Comm Pack v3.2 Release*********>             *
 *     07/23/98 Fixed compile error when RTS2/3/4 configured on PORTC.  ats *
 *  19 02/10/00 Added NO_CONFIG_TENA and NO_CONFIG_PINS ifdefs          rds *
 *  20 02/11/00 removed MC68360 conditional                            kimk *
 *              qedvr.h is including systype.h now                          *
 ****************************************************************************/

#include <SPQUICC/defs.h>
#include <options.h>

/*	set up RxD, TxD, CD, CTS pins according to 'options.h' */

#ifndef NO_CONFIG_PINS
void config_pins(QUICC *qp, u_int32 channel)
{
	u_int16	cts_mask = 0x10 << (channel * 2);
	u_int16 rxd_mask = 0x1 << (channel * 2);
	u_int16 txd_mask = 0x2 << (channel * 2);
	u_int16 cd_mask  = 0x20 << (channel * 2);
	u_int32 rck_mask = 0x38 << (channel * 8);
	u_int32 tck_mask = 0x7 << (channel * 8);


	/* configure CTS & CD pin for channel */

	outw((void *) &qp->pio_pcdir,
		(inw((void *) &qp->pio_pcdir) & ~(cts_mask | cd_mask)));	/* clear pio_pcdir bit */

	outw((void *) &qp->pio_pcso,
		(inw((void *) &qp->pio_pcso) | cts_mask | cd_mask));		/* set pio_pcso bit */
	outw((void *) &qp->pio_pcpar,
		(inw((void *) &qp->pio_pcpar) & ~(cts_mask | cd_mask)));	/* clear pio_pcpar bit */

	/* configure TXD/RXD for channel */

	switch(channel) {

#ifdef	TXD1
		case 0:
#if	TXD1 == PORTA
			outw((void *)&qp->pio_papar, (inw((void *)&qp->pio_papar) | txd_mask));
			outw((void *)&qp->pio_padir, (inw((void *)&qp->pio_padir) & ~txd_mask));
#endif
#if	RXD1 == PORTA
			outw((void *)&qp->pio_papar, (inw((void *)&qp->pio_papar) | rxd_mask));
			outw((void *)&qp->pio_padir, (inw((void *)&qp->pio_padir) & ~rxd_mask));
#endif

	outl((void *)&qp->si_sicr, (inl((void *)&qp->si_sicr) & ~tck_mask) | TCK1 );
	outl((void *)&qp->si_sicr, (inl((void *)&qp->si_sicr) & ~rck_mask) | RCK1<<3 );

	outw((void *)&qp->pio_padir, inw((void *)&qp->pio_padir) & ~((0x0010<<TCK1) | (0x0010<<RCK1)));
	outw((void *)&qp->pio_papar, inw((void *)&qp->pio_papar) | ((0x0010<<TCK1) | (0x0010<<RCK1)));

		break;
#endif	/* TXD1 */

#ifdef	TXD2
		case 1:
#if	TXD2 == PORTA
			outw((void *)&qp->pio_papar, (inw((void *)&qp->pio_papar) | txd_mask));
			outw((void *)&qp->pio_padir, (inw((void *)&qp->pio_padir) & ~txd_mask));
#endif
#if	RXD2 == PORTA
			outw((void *)&qp->pio_papar, (inw((void *)&qp->pio_papar) | rxd_mask));
			outw((void *)&qp->pio_padir, (inw((void *)&qp->pio_padir) & ~rxd_mask));
#endif

	outl((void *)&qp->si_sicr, (inl((void *)&qp->si_sicr) & ~tck_mask) | TCK2<<(channel*8) );
	outl((void *)&qp->si_sicr, (inl((void *)&qp->si_sicr) & ~rck_mask) | RCK2<<(3+(channel*8)) );

	outw((void *)&qp->pio_padir, inw((void *)&qp->pio_padir) & ~((0x0010<<TCK2) | (0x0010<<RCK2)));
	outw((void *)&qp->pio_papar, inw((void *)&qp->pio_papar) | ((0x0010<<TCK2) | (0x0010<<RCK2)));

			break;
#endif	/* TXD2 */

#ifdef	TXD3
		case 2:
#if	TXD3 == PORTA
			outw((void *)&qp->pio_papar, (inw((void *)&qp->pio_papar) | txd_mask));
			outw((void *)&qp->pio_padir, (inw((void *)&qp->pio_padir) & ~txd_mask));
#endif
#if	TXD3 == PORTD
			outw((void *)&qp->pio_pdpar, (inw((void *)&qp->pio_pdpar) | txd_mask));
#endif
#if	RXD3 == PORTA
			outw((void *)&qp->pio_papar, (inw((void *)&qp->pio_papar) | rxd_mask));
			outw((void *)&qp->pio_padir, (inw((void *)&qp->pio_padir) & ~rxd_mask));
#endif
#if	RXD3 == PORTD
			outw((void *)&qp->pio_pdpar, (inw((void *)&qp->pio_pdpar) | rxd_mask));
#endif

	outl((void *)&qp->si_sicr, (inl((void *)&qp->si_sicr) & ~tck_mask) | (TCK3 & CLK_Msk)<<(channel*8) );
	outl((void *)&qp->si_sicr, (inl((void *)&qp->si_sicr) & ~rck_mask) | ((RCK3 & CLK_Msk)<<(3+(channel*8) );

	outw((void *)&qp->pio_padir, inw((void *)&qp->pio_padir) & ~((0x0001<<TCK3) | (0x0001<<RCK3)));
	outw((void *)&qp->pio_papar, inw((void *)&qp->pio_papar) | ((0x0001<<TCK3) | (0x0001<<RCK3)));

			break;
#endif	/* TXD3 */

#ifdef	TXD4
		case 3:
#if	TXD4 == PORTA
			outw((void *)&qp->pio_papar, (inw((void *)&qp->pio_papar) | txd_mask));
			outw((void *)&qp->pio_padir, (inw((void *)&qp->pio_padir) & ~txd_mask));
#endif
#if	TXD4 == PORTD
			outw((void *)&qp->pio_pdpar, (inw((void *)&qp->pio_pdpar) | txd_mask));
#endif
#if	RXD4 == PORTA
			outw((void *)&qp->pio_papar, (inw((void *)&qp->pio_papar) | rxd_mask));
			outw((void *)&qp->pio_padir, (inw((void *)&qp->pio_padir) & ~rxd_mask));
#endif
#if	RXD4 == PORTD
			outw((void *)&qp->pio_pdpar, (inw((void *)&qp->pio_pdpar) | rxd_mask));
#endif

	outl((void *)&qp->si_sicr, (inl((void *)&qp->si_sicr) & ~tck_mask) | (TCK4 & CLK_Msk)<<(channel*8) );
	outl((void *)&qp->si_sicr, (inl((void *)&qp->si_sicr) & ~rck_mask) | (RCK4 & CLK_Msk)<<(3+(channel*8) );

	outw((void *)&qp->pio_padir, inw((void *)&qp->pio_padir) & ~((0x0001<<TCK4) | (0x0001<<RCK4)));
	outw((void *)&qp->pio_papar, inw((void *)&qp->pio_papar) | ((0x0001<<TCK4) | (0x0001<<RCK4)));

			break;
#endif	/* TXD4 */

	}
}
#endif /* NO_CONFIG_PINS */


#ifndef NO_CONFIG_TENA
/*	set up RTS pins selected by to 'options.h'  	*/
/*  use pins as transmitter RTS.			*/
/*	RTS becomes TENA for use with E-Net 		*/
/*							*/
/*	This is done later than the other pins		*/
/*	because RTS can enable the transceiver		*/
/*  if set up before the channel is programmed  */

void	config_TENA(QUICC *qp, u_int32 channel)
{
#if (RTS1==PORTB || RTS2==PORTB || RTS3==PORTB || RTS4==PORTB )
	u_int32 pb_mask = 0x1000 << channel;
#endif
#if (RTS1==PORTC || RTS2==PORTC || RTS3==PORTC || RTS4==PORTC)
	u_int16 pc_mask = 0x1 << channel;
#endif
#if RTS3==PORTD || RTS4==PORTD
	u_int16 pd_mask = 0x40 << channel;
#endif

	/* configure RTS pin as RTS for channel */

	switch (channel) {

#ifdef	RTS1
		case 0:
#if		RTS1 == PORTB
			/* set pio_pbdir bit */
			outl((void *)&qp->pip_pbdir,
				(inl((void *)&qp->pip_pbdir) | pb_mask));
			/* set pip_pbpar bit */
			outl((void *)&qp->pip_pbpar,
				(inl((void *)&qp->pip_pbpar) | pb_mask));
#elif	RTS1 == PORTC
			/* clear pio_pcdir bit */
			outw((void *)&qp->pio_pcdir,
				(inw((void *)&qp->pio_pcdir) & ~pc_mask));
			/* set pio_pcpar bit */
			outw((void *)&qp->pio_pcpar,
				(inw((void *)&qp->pio_pcpar) | pc_mask));
#endif
			break;
#endif	/* RTS1 */

#ifdef	RTS2
		case 1:
#if		RTS2 == PORTB
			/* set pip_pbdir bit */
			outl((void *)&qp->pip_pbdir,
				(inl((void *)&qp->pip_pbdir) | pb_mask));
			/* set pip_pbpar bit */
			outl((void *)&qp->pip_pbpar,
				(inl((void *)&qp->pip_pbpar) | pb_mask));
#elif	RTS2 == PORTC
			/* clear pio_pcdir bit */
			outw((void *)&qp->pio_pcdir,
				(inw((void *)&qp->pio_pcdir) & ~pc_mask));
			/* set pio_pcpar bit */
			outw((void *)&qp->pio_pcpar,
				(inw((void *)&qp->pio_pcpar) | pc_mask));
#endif
			break;
#endif	/* RTS2 */

#ifdef	RTS3
		case 2:
#if		RTS3 == PORTB
			/* set pip_pbdir bit */
			outl((void *)&qp->pip_pbdir,
				(inl((void *)&qp->pip_pbdir) | pb_mask));
			/* set pip_pbpar bit */
			outl((void *)&qp->pip_pbpar,
				(inl((void *)&qp->pip_pbpar) | pb_mask));
#elif	RTS3 == PORTC
			/* clear pio_pcdir bit */
			outw((void *)&qp->pio_pcdir,
				(inw((void *)&qp->pio_pcdir) & ~pc_mask));
			/* set pio_pcpar bit */
			outw((void *)&qp->pio_pcpar,
				(inw((void *)&qp->pio_pcpar) | pc_mask));
#elif	RTS3 == PORTD
			/* set pio_pdpar bit */
			outw((void *)&qp->pio_pdpar,
				(inw((void *)&qp->pio_pdpar) | pd_mask));
#endif
			break;
#endif	/* RTS3 */

#ifdef	RTS4
		case 3:
#if		RTS4 == PORTB
			/* set pip_pbdir bit */
			outl((void *)&qp->pip_pbdir,
				(inl((void *)&qp->pip_pbdir) | pb_mask));
			/* set pip_pbpar bit */
			outl((void *)&qp->pip_pbpar,
				(inl((void *)&qp->pip_pbpar) | pb_mask));
#elif	RTS4 == PORTC
			/* clear pio_pcdir bit */
			outw((void *)&qp->pio_pcdir,
				(inw((void *)&qp->pio_pcdir) & ~pc_mask));
			/* set pio_pcpar bit */
			outw((void *)&qp->pio_pcpar,
				(inw((void *)&qp->pio_pcpar) | pc_mask));
#elif	RTS4 == PORTD
			/* set pio_pdpar bit */
			outw((void *)&qp->pio_pdpar,
				(inw((void *)&qp->pio_pdpar) | pd_mask));
#endif
			break;
#endif	/* RTS4 */

	}
}
#endif /* NO_CONFIG_TENA */
