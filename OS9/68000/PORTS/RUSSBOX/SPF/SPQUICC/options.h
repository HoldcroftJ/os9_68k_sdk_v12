/****************************************************************************
 *  Low Level Ethernet Port customization options definitions.              *
 ****************************************************************************
 *  Copyright 1997 by Microware Systems Corporation                         *
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
 *  01 07/15/97 Created for MPC821ADS port.                             ats *
 *     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
 ****************************************************************************/

/* This file controls all customization options by which this driver
 * may be built. These options are provided to maximize the driver's
 * efficiency or tailor the driver operations to a particular hardware
 * implementation by allowing the porting person to specify which options
 * are actually required for a given target port.
 */

/* Debugging Option:
 *
 * DEBUG - This symbol is defined when debugging the driver. When it
 *         is defined, the driver gets built differently in two ways.
 *
 *         1. Debugging information is collected in a data module.
 *         2. Functions declared with the STATDBG macro, which would
 *            normally be declared as "static" when DEBUG is not defined,
 *            are now visible. Beware that this _does_not_ prevent them
 *            from also being in-lined during compiler optimization, unless
 *            those functions are defined in separate compilation units
 *            from their invocation and I-code linking is _not_ being used.
 */

/*#define		DEBUG	*/	/* build with DEBUG option */

/* Cacheing Considerations:
 *
 * The normal DMA operation of the SCC requires some consideration
 * of the data cacheing environment.
 *
 * If the mbuf pool resides in an area of memory that is cache inhibited,
 * then this driver need not perform any cache-related functions. However,
 * in a pipelining architecture where writes to memory may be pending,
 * this driver needs to ensure that data transmitted by the application
 * (or any upper protocols) is written to memory before initiating
 * transmission. This is accomplished with the SYNC macro. There are no
 * special considerations necessary for received data, as the SCC ensures
 * that data has been written to memory when closing the buffer.
 * 
 * If the mbuf pool resides in an area of memory that is cache enabled
 * then the driver must perform some cache-related functions. If a
 * write-through cache policy is being used, then the transmit data
 * side requirements are identical to the above. If a copy-back cache
 * policy is being used, then the driver must ensure that (for transmit
 * data) the write to cache is complete (if there is any chance of it
 * not being), and that the contents of the cache is written to memory.
 *
 * On the MPC821 and MPC860 processors, caches are written immediately
 * from the pipeline, so the cache only needs to be stored. However,
 * since storing alone is not yet supported, flushing (which includes
 * invalidation) of the data areas must be invoked before starting
 * transmission. Currently the default for this driver is to expect
 * that mbufs (used for data buffering) will reside in an area of memory
 * that is cache enabled (with a write copy-back policy) for data access.
 *
 * On the 68360 processor, there is no cache.
 *
 * For received data, anytime data cacheing is enabled, any cached areas
 * of the data buffers must be invalidated before the data can be read by
 * the processor. However, since invalidation alone is not implemented on
 * the PowerPC(tm) architecture, the data caches must be flushed prior to
 * reception.
 *
 * There is also the assumption that the mbuf routines allocate mbufs
 * such that their boundaries coincide with cache line boundaries.
 *
 * Cacheing options:
 *
 * CACHE_INHIBITED_BUFS - This symbol should be defined when it is known
 *                        that the mbuf pool will reside in a cache inhibited
 *                        area of memory (or when cacheing does not exist or
 *                        is disabled).
 *
 * SYNC_TRANSMIT_DATA   - This symbol should be defined if it is known that
 *                        synchronization is necessary to ensure that data
 *                        is written to memory (or to internal caches).
 *
 * CACHE_WRITETHROUGH   - This symbol should be defined when it is known
 *                        that the mbuf pool will reside in a cache enabled
 *                        area of memory with a write-through policy.
 *
 * CACHE_CANSTORE       - This symbol should be defined when it is known
 *                        that a data cache store function is supported in
 *                        the system cache module.
 *
 * CACHE_CANINVALIDATE  - This symbol should be defined when it is known
 *                        that a data cache invalidate function is supported
 *                        in the system cache module.
 */

/* Overrides for defaults */

/*#define	CACHE_INHIBITED_BUFS */	/* no data cacheing for mbufs */

/*#define	SYNC_TRANSMIT_BUFS */	/* transmit data sync necessary */

/*#define	CACHE_WRITETHROUGH */	/* mbufs are cache writethrough */

/*#define	CACHE_CANSTORE	 */	/* data cache storing supported */

/*#define	CACHE_CANINVALIDATE */ 	/* data cache invalidation supported */

#if	defined(_MPFPOWERPC)
/* defaults to nearly worst case */

#elif	defined(_OSK)
/* defaults to no cacheing, no synchronization necessary */
#define	CACHE_INHIBITED_BUFS

#else

#error	!! Unsupported processor/architecture !!
#endif


/*	The Ethernet controller function can run on any SCC (1-4)	
 *																
 *	Which channel is determined in the driver from examining 	
 *	the port address.											
 *																
 *	The buffer descriptor area is selected from the Ethernet	
 *	area assigned to the channel in CPM.H						
 *																
 * 	Pin Assignment Considerations
 *
 * 	The Ethernet interface requires the assignment of ALL the 
 *	following signals to a channel:
 *
 *	RxD	TxD	CD CTS RTS RCLK TCLK	
 *
 *	Ethernet protocols can be supported on any SCC (1-4)
 *
 *	Many of these pin functions can be multiplexed onto different pins
 *  on the chip. The module pins,c together with this module 
 *	attempts to support this in a generic manner.
 *
 *				SCC1		SCC2		SCC3		SCC4  
 *
 *	RxD			PA15		PA13		PA11		PA9		
 *									   *PD11	   *PD9
 *												   *PA15
 *
 *	TxD			PA14		PA12		PA10		PA8
 *									   *PD10	   *PD8
 *												   *PA14
 *
 *	CD			PC10		PC8			PC6			PC4
 *			
 *	CTS			PC11		PC9			PC7			PC5
 *
 *	RTS			PB19		PB18		PB17		PB16
 *				PC15		PC14		PC13		PC12
 *									   *PD7		   *PD6
 *
 *
 *	The MPC860 supports the full list above.
 *	The MPC821 supports SCC 1 & 2 only.
 *	The 68360  supports everything except '*"
 *			   note that the 68360 bit ordering is reversed!
 *
 *	RxD, TxD, RTS for each channel are assigned by 
 * 	defining which PORT (A-D) they are on
 *
 *  CD and CTS are fixed, so no action is required
 */
/* Fixed PORTx definitions - do not change! */

#define PORTA	1
#define PORTB	2
#define PORTC	3
#define PORTD	4

/* Fixed configuration - uncomment if used */

				/* SCC1 & 2 pins on 860/821/68360 */				

#define TXD1	PORTA	/* TXD1 configured on PORTA */
#define RXD1	PORTA	/* RXD1 configured on PORTA */
/*#define RTS1	PORTB */	/* RTS1 configured on PORTB */
#define RTS1	PORTC	/* RTS1 configured on PORTC */

/*#define TXD2	PORTA */	/* TXD2 configured on PORTA */
/*#define RXD2	PORTA */ 	/* RXD2 configured on PORTA */
/*#define RTS2	PORTC */	/* RTS2 configured on PORTC */

				/* SCC3 & 4 pins on 860/68360 only */

/*#define TXD3	PORTA */	/* TXD3 configured on PORTA */
/*#define RXD3	PORTA */	/* RXD3 configured on PORTA */
/*#define RTS3	PORTC */	/* RTS3 configured on PORTC */

/*#define TXD4	PORTA */	/* TXD4 configured on PORTA */
/*#define RXD4	PORTA */	/* RXD4 configured on PORTA */
/*#define RTS4	PORTC */	/* RTS4 configured on PORTC */


/*
 *	The clock signals are allocated separately:
 *
 *				SCC1		SCC2		SCC3		SCC4  
 *				
 *	RCLK		CLK1		CLK1		CLK5		CLK5
 *				CLK2		CLK2		CLK6		CLK6
 *				CLK3		CLK3		CLK7		CLK7
 *				CLK4		CLK4		CLK8		CLK8
 *
 *	TCLK		CLK1		CLK1		CLK5		CLK5
 *				CLK2		CLK2		CLK6		CLK6
 *				CLK3		CLK3		CLK7		CLK7
 *				CLK4		CLK4		CLK8		CLK8
 *
 *	Note that SCC3 & 4 and CLK5-8 do not exist on MPC821
 *															*/

#define	CLK1	0x4
#define	CLK2	0x5
#define	CLK3	0x6
#define	CLK4	0x7

/*	Note that CLK5-8 must be masked with 0x7 before using */

#define	CLK5	0xC
#define	CLK6	0xD
#define	CLK7	0xE
#define	CLK8	0xF

#define	CLK_Msk	0x7

#if		defined(RXD1) && defined(TXD1) && defined(RTS1)
#define	RCK1	CLK2 			/* CLK1/CLK2/CLK3/CLK4	*/
#define	TCK1	CLK1 			/* CLK1/CLK2/CLK3/CLK4	*/
#endif

#if		defined(RXD2) && defined(TXD2) && defined(RTS2)
/*#define	RCK2	CLK3 << 11 */	/* CLK1/CLK2/CLK3/CLK4	*/
/*#define	TCK2	CLK1 <<  8 */	/* CLK1/CLK2/CLK3/CLK4	*/
#endif

#if		defined(RXD3) && defined(TXD3) && defined(RTS3)
/*#define	RCK3	CLK5 << 19 */	/* CLK5/CLK6/CLK7/CLK8	*/
/*#define	TCK3	CLK6 << 16 */	/* CLK5/CLK6/CLK7/CLK8	*/
#endif

#if		defined(RXD4) && defined(TXD4) && defined(RTS4)
/*#define	RCK4	CLK7 << 27 */		/* CLK5/CLK6/CLK7/CLK8	*/
/*#define	TCK4	CLK8 << 24 */		/* CLK5/CLK6/CLK7/CLK8	*/
#endif


/*	In addition to the pin functions above, a board may also have 
 *	additional Ethernet control functions on GPIO pins and/or through
 *	an external register etc.
 *
 *	For example:
 *
 *	MPC821ADS board
 *
 *	Register	Address		Bit 	Name	Function	
 *
 *	BSCR1		0x02100000	 2		ETHEN	0= Enable EEST
 *  PCDAT		QUICC		 4		ETHLOOP	1= Enable diagnostic loopback
 *	PCDAT		QUICC		 5		TPFLDL	0= Enable TP full duplex mode
 *	PCDAT		QUICC		 6		TPSQEL	0= Enable TP SQET Enable
 *
 *
 *	MBX821/860 board
 *
 *	Register	Address		Bit 	Name	Function	
 *
 *	CR1		0xFA100000	 0		ETHEN	0= Enable EEST
 *  CR1		0xFA100000	 1		ETHLOOP	1= Enable diagnostic loopback
 *	CR1		0xFA100000	 2		TPAUTO	1= Enable Auto TP/AUI Select
 *	CR1		0xFA100000	 3		TPSEL	1= Select TP port
 *	CR1		0xFA100000	 4		TPFLDL	0= Enable TP full duplex mode
 *	CR1		0xFA100000	 5		TPSQEL	0= Enable TP SQET Enable
 *																			*/
 




/* Sanity check/reminder */

/* Must define a full set of pins for Ethernet operation */

#if !defined(TXD1)&&!defined(TXD2)&&!defined(TXD3)&&!defined(TXD4)
#error !! No TXD configuration has been specified !!
#endif
#if !defined(RXD1)&&!defined(RXD2)&&!defined(RXD3)&&!defined(RXD4)
#error !! No RXD configuration has been specified !!
#endif
#if !defined(RTS1)&&!defined(RTS2)&&!defined(RTS3)&&!defined(RTS4)
#error !! No RTS configuration has been specified !!
#endif
#if !defined(RCK1)&&!defined(RCK2)&&!defined(RCK3)&&!defined(RCK4)
#error !! No RCK configuration has been specified !!
#endif
#if !defined(TCK1)&&!defined(TCK2)&&!defined(TCK3)&&!defined(TCK4)
#error !! No TCK configuration has been specified !!
#endif

#if defined(TXD1) || defined(RXD1) || defined(RTS1) || defined(RCK1) || defined(TCK1)
#if !defined(TXD1) || !defined(RXD1) || !defined(RTS1) || !defined(RCK1) || !defined(TCK1)
#error !! Incomplete pin definition for SCC1 !!
#endif
#endif
#if defined(TXD2) || defined(RXD2) || defined(RTS2) || defined(RCK2) || defined(TCK2)
#if !defined(TXD2) || !defined(RXD2) || !defined(RTS2) || !defined(RCK2) || !defined(TCK2)
#error !! Incomplete pin definition for SCC2 !!
#endif
#endif
#if defined(TXD3) || defined(RXD3) || defined(RTS3) || defined(RCK3) || defined(TCK3)
#if !defined(TXD3) || !defined(RXD3) || !defined(RTS3) || !defined(RCK3) || !defined(TCK3)
#error !! Incomplete pin definition for SCC3 !!
#endif
#endif
#if defined(TXD4) || defined(RXD4) || defined(RTS4) || defined(RCK4) || defined(TCK4)
#if !defined(TXD4) || !defined(RXD4) || !defined(RTS4) || !defined(RCK4) || !defined(TCK4)
#error !! Incomplete pin definition for SCC4 !!
#endif
#endif


