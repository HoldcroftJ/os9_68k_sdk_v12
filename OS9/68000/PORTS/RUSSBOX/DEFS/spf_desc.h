/****************************************************************************
 *	$Workfile:   spf_desc.h  $
 *	$Revision:   1.0  $
 *	$Modtime:   18 Apr 1994 17:14:00  $
 *	$Date:   12 Jun 1994 17:23:22  $
 ****************************************************************************
 *	Definitions file for making SPF device descriptors for QUICC driver.
 ****************************************************************************
 *  Copyright 1994 by Microware Systems Corporation              
 *  Reproduced Under License                         
 *                                                                          
 *  This source code is the proprietary confidential property of Microware   
 *  Systems Corporation, and is provided to licensee for documentation and   
 *  educational purposes only. Reproduction, publication, or distribution    
 *  in any form to any party other than the licensee is strictly prohibited. 
 ****************************************************************************/


#if defined(spraw0) || defined(spraw1) || defined(spraw2) || defined(spraw3)

/****************************************************************************
 *	/spraw0																	*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- RAW PMod -- /term port -- DTE	*
 ****************************************************************************
 *	/spraw1																	*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- RAW PMod -- /t1 port -- DCE	*
 ****************************************************************************
 *	/spraw2																	*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- RAW PMod -- /t2 port -- DTE	*
 ****************************************************************************
 *	/spraw3																	*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- RAW PMod -- /t3 port -- DCE	*
 ****************************************************************************/

	#define PM_NAME		"raw"				/* protocol module name			*/
	#define SHARE		0					/* path sharing flag			*/

	#if defined(spraw0)
		#define PORT0						/* port number (0/1/2/3)		*/
	#endif

	#ifdef spraw1
		#define PORT1						/* port number (0/1/2/3)		*/
	#endif

	#ifdef spraw2
		#define PORT2						/* port number (0/1/2/3)		*/
	#endif

	#ifdef spraw3
		#define PORT3						/* port number (0/1/2/3)		*/
	#endif

#endif /* spraw0/spraw1/spraw2/spraw3 ***************************************/


#if defined(splapb0) || defined(splapb1) || defined(splapb2) || defined(splapb3)

/****************************************************************************
 *	/splapb0																*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- LAPB PMod -- /term port -- DTE*
 ****************************************************************************
 *	/splapb1																*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- LAPB PMod -- /t1 port -- DCE	*
 ****************************************************************************
 *	/splapb2																*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- LAPB PMod -- /t2 port -- DTE	*
 ****************************************************************************
 *	/splapb3																*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- LAPB PMod -- /t3 port -- DCE	*
 ****************************************************************************/

	#define PM_NAME		"lapb"				/* protocol module name			*/
	#define SHARE		0					/* path sharing flag			*/
	#define _MYDESC_LAPB					/* get LAPB PMod constants		*/

	#if defined(splapb0)
		#define PORT0						/* port number (0/1/2/3)		*/
	#endif

	#ifdef splapb1
		#define PORT1						/* port number (0/1/2/3)		*/
	#endif

	#ifdef splapb2
		#define PORT2						/* port number (0/1/2/3)		*/
	#endif

	#ifdef splapb3
		#define PORT3						/* port number (0/1/2/3)		*/
	#endif

#endif /* splapb0/splapb1/splapb2/splapb3 ***********************************/


#if defined(spx250) || defined(spx251) || defined(spx252) || defined(spx253)

/****************************************************************************
 *	/spx250																	*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- X25 PMod -- /term port -- DTE	*
 ****************************************************************************
 *	/spx251																	*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- X25 PMod -- /t1 port -- DCE	*
 ****************************************************************************
 *	/spx252																	*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- X25 PMod -- /t2 port -- DTE	*
 ****************************************************************************
 *	/spx253																	*
 *																			*
 *	Device Descriptor for SPF QUICC driver -- X25 PMod -- /t3 port -- DCE	*
 ****************************************************************************/

	#define PM_NAME		"x25"				/* protocol module name			*/
	#define SHARE		1					/* path sharing flag			*/
	#define _MYDESC_X25						/* get X25 PMod constants		*/

	#if defined(spx250)
		#define PORT0						/* port number (0/1/2/3)		*/
	#endif

	#ifdef spx251
		#define PORT1						/* port number (0/1/2/3)		*/
	#endif

	#ifdef spx252
		#define PORT2						/* port number (0/1/2/3)		*/
	#endif

	#ifdef spx253
		#define PORT3						/* port number (0/1/2/3)		*/
	#endif

#endif /* spx250/spx251/spx252/spx253 ***************************************/


#if defined(_MYDESC_LAPB)
/****************************************************************************
 * LAPB Protocol Module Definitions											*
 *																			*/

#include <INET/lapb.h>

/* X.25/LAP-B Protocol Module constants section */
#define PM_CONST_TYPE       \
    struct {                                                                \
        lapb_const l2const;                                                 \
    }
#define PM_CONST            \
    {\
        {\
            0,                      /* l2_synch: Synchronous operation      */\
            1,                      /* l2_copyinfo: Make local copy of info */\
            LAPB_NONE,              /* l2_endpt: Endpoint type              */\
            LAPB_SLINK,             /* l2_oper: Single/Multi Link operation */\
            LAPB_DTDC_SLC,          /* l2_cmd_addr: Command address byte    */\
            LAPB_DTDC_SLR,          /* l2_rsp_addr: Response address byte   */\
            7,                      /* l2_k: Max number outstanding I-frames*/\
            {0},                    /* l2_rsv1[2]: RESERVED FOR FUTURE USE! */\
            2,                      /* l2_t1: Acknowledgement timer         */\
            1,                      /* l2_t2: Acknowledge response timer    */\
            30,                     /* l2_t3: Idle timer                    */\
            4096,                   /* l2_n1: Max number of bits in I-frame */\
            10,                     /* l2_n2: Max number of retransmissions */\
            {0}                     /* l2_rsv2[20]: RESERVED FOR FUTURE USE!*/\
        } \
    }

#endif /* _MYDESC_LAPB ******************************************************/


#if defined(_MYDESC_X25)
/****************************************************************************
 * X25 Protocol Module Definitions											*
 *																			*/

#include <INET/lapb.h>
#include <INET/x25.h>

/* X.25/LAP-B Protocol Module constants section */
#define PM_CONST_TYPE       \
    struct {                                                                \
        lapb_const l2const;                                                 \
        x25_const l3const;                                                  \
    }
#define PM_CONST            \
    {\
        {\
            0,                      /* l2_synch: Synchronous operation      */\
            1,                      /* l2_copyinfo: Make local copy of info */\
            LAPB_NONE,              /* l2_endpt: Endpoint type              */\
            LAPB_SLINK,             /* l2_oper: Single/Multi Link operation */\
            LAPB_DTDC_SLC,          /* l2_cmd_addr: Command address byte    */\
            LAPB_DTDC_SLR,          /* l2_rsp_addr: Response address byte   */\
            7,                      /* l2_k: Max number outstanding I-frames*/\
            {0},                    /* l2_rsv1[2]: RESERVED FOR FUTURE USE! */\
            2,                      /* l2_t1: Acknowledgement timer         */\
            1,                      /* l2_t2: Acknowledge response timer    */\
            30,                     /* l2_t3: Idle timer                    */\
            4096,                   /* l2_n1: Max number of bits in I-frame */\
            10,                     /* l2_n2: Max number of retransmissions */\
            {0}                     /* l2_rsv2[20]: RESERVED FOR FUTURE USE!*/\
        },\
        {\
            NL_GFI_8,               /* l3_gfi: General Format Indicator     */\
            7,                      /* l3_wndsz: Default Window Size        */\
            0,                      /* l3_thrghpt: Default Throughput Class */\
            0,                      /* l3_inumlen: Len ISDN GWay Phone Num  */\
            7,                      /* l3_addrlen: Len Local DTE addr       */\
            0,                      /* l3_userlen: Len User Data            */\
            0,                      /* l3_d: Delivery Confirmation Bit      */\
            0,                      /* l3_q: Qualifier Bit (used w/UserData)*/\
            {"1730"},               /* l3_inum[NL_INUM_LEN]: ISDN GWay PNum */\
            {"2221731"},            /* l3_addr[NL_XNUM_LEN]: Local DTE Addr */\
            {0},                    /* l3_user[NL_USER_LEN]: User Data [CR] */\
            0,						/* l3_nonegoc: No Negotiation Flag		*/\
            {0},                    /* l3_rsv1[7]: RESERVED FOR FUTURE USE! */\
            1024,                   /* l3_pktsz: Default Max Data Packet Sz */\
            {0},                    /* l3_rsv2[1]: RESERVED FOR FUTURE USE! */\
            180,                    /* l3_t20: Timeout for t20              */\
            200,                    /* l3_t21: Timeout for t21              */\
            180,                    /* l3_t22: Timeout for t22              */\
            180,                    /* l3_t23: Timeout for t23              */\
            300,                    /* l3_t28: Timeout for t28              */\
            /* nl_nl_lcgn[16]: logical group bitmap */\
            {\
                /* lc_type/lc_avail/lc_rsv[2]/lc_bitmap[8] */\
                /* default: LG0/LC1 open for 2Way VC calls */\
                { LG_VC2WAY,3,{0},{0x70000000,0,0,0,0,0,0,0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} },\
                { LG_NOTUSED,0,{0},{0} }\
            },\
            {0}                     /* l3_rsv3[16]: RESERVED FOR FUTURE USE!*/\
        }\
    }

#endif /* _MYDESC_LAPB ******************************************************/


/****************************************************************************
 * quicc Device Driver Definitions											*
 *																			*/

#include <INET/68360.h>				/* device-specific hardware definitions	*/

#undef  IRQLEVEL
#undef  MODE

#define IRQLEVEL	5				/* Port IRQ Level 						*/
#define PRIORITY	5				/* IRQ polling priority					*/
#define MODE 		S_ISIZE | S_IREAD | S_IWRITE	/* device mode			*/
#define DRV_NAME	"spquicc"		/* device driver name					*/
#define DATARATE	16000			/* baud rate: 16000						*/
#define PARITY		SPF_PRTY_NONE	/* parity (not used)					*/
#define STOPBITS	SPF_STBT_NONE	/* stopbits (not used)					*/
#define WORDSIZE	8				/* number of bits per character			*/
#define ENCODING	SPF_ENC_NRZI	/* NRZI encoding						*/
#define RXPKTSZ		512				/* receive mbuf allocation (in bytes)	*/
#define RX_PRIORITY	500				/* Receive thread process priority		*/
#define RX_STACKSZ	0				/* Additional receive thread stack		*/
#define IRQMASK		(IRQLEVEL << 8)	/* CPU interrupt mask 					*/
#define CLOCKRATE   4096000         /* clock rate                           */

#define CLOCKMULT   1               /* clock multiplier                     */
#define SYNCCHAR    SPF_SYNC_HDLC   /* sync character                       */
#define SYNCMODE    SPF_MODE_HDLC   /* sync mode                            */
#define CRCTYPE     SPF_CRC_16_1    /* crc type                             */
#define LCLLOOP     0               /* local loopback flag                  */
#define IDLETYPE    SPF_IDLE_FLAG   /* idle type                            */
#define MULTIPT     0               /* multipoint flag                      */

#if defined(PORT0)
	#define LUN			0				/* logical unit number 				*/
	#define DIRECTION	SPF_DIR_OUT		/* com dircetion:DCE=in/DTE=out		*/
	#define PORTADDR	0x00020c00		/* Base address of hardware 		*/
	#define VECTOR		254			/* Port vector 						*/
	#define DPADDR		(PORTADDR&0xffffe000)	/* dp memory address */
#elif defined(PORT1)
	#define LUN			0				/* logical unit number 				*/
	#define DIRECTION	SPF_DIR_IN		/* com dircetion:DCE=in/DTE=out		*/
	#define PORTADDR	0x00020d00		/* Base address of hardware 		*/
	#define VECTOR		253			/* Port vector 						*/
	#define DPADDR		(PORTADDR&0xffffe000)	/* dp memory address */
#elif defined(PORT2)
	#define LUN			0				/* logical unit number 				*/
	#define DIRECTION	SPF_DIR_OUT		/* com dircetion:DCE=in/DTE=out		*/
	#define PORTADDR	0x00020e00		/* Base address of hardware 		*/
	#define VECTOR		252			/* Port vector 						*/
	#define DPADDR		(PORTADDR&0xffffe000)	/* dp memory address */
#elif defined(PORT3)
	#define LUN			0				/* logical unit number 				*/
	#define DIRECTION	SPF_DIR_OUT		/* com dircetion:DCE=in/DTE=out		*/
	#define PORTADDR	0x00020f00		/* Base address of hardware 		*/
	#define VECTOR		251			/* Port vector 						*/
	#define DPADDR		(PORTADDR&0xffffe000)	/* dp memory address */
#endif

/* logical unit specific initialized static storage */
#define LU_SPECIFICS_INIT  \
	{\
		(void*)DPADDR, 				/* channel: address of control channel	*/\
	}

