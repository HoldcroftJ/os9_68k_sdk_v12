/***************************************************************************
*  SPF IrDA Protocol Stack Header File
****************************************************************************
*  Copyright 1995 by Microware Systems Corporation
*  Copyright Counterpoint Systems Foundry, Inc. 1995. 1996
*  Copyright Counterpoint Systems Foundry, Inc. 1995, 1996, 1997
*  Counterpoint Systems Foundry, Inc. Confidential
*  Reproduced Under License
*
*  This source code is the proprietary confidential property of
*  Microware Systems Corporation and Counterpoint Systems Foundry, Inc.,
*  and is provided to licensee solely for documentation and educational
*  purposes. Reproduction, publication, or distribution in any form to any 
*  party other than the licensee is strictly prohibited.
****************************************************************************
* Edition History:                                                         *
* #   Date     Comments                                          By        *
* --- -------- ------------------------------------------------- --------- *
*   1 07/29/96 initial coding                                    pbt       *
*   1 06/26/97 added all of the current code                     adg       *
****************************************************************************/

#ifndef _IRDA_H  /* See if irda definintions have already been defined */
#define _IRDA_H 

/****** HEADER FILES *****************************************************/

#include <process.h>
#include <types.h>
#include <io.h>
#include <errno.h>
#include <SPF/spf.h>
#include <SPF/mbuf.h>

/* Boolean Definitions */

#ifndef TRUE
#define TRUE  (1==1)
#endif
#ifndef FALSE
#define FALSE (0==1)  
#endif

#define IR_DISABLED 0
#define IR_ENABLED 1

/***** General Function Prototypes ****************************************/
/* #if defined(_OSK) */
extern error_code _os_getstat(path_id, u_int32, void *);
extern error_code _os_setstat(path_id, u_int32, void *);
/* #endif */

/* Errors returned by library functions */
#define IR_MIN_ERROR  (EOS_WANBASE + 0xF2)  /* Minimum value of IRDA defined errors. */
#define IR_REQ_FAILED (EOS_WANBASE + 0xF2)  /* The request failed. See library document for reasons. */
#define IR_NO_LAP     (EOS_WANBASE + 0xF3)  /* Request requires Irlap and it is not up. */
#define IR_MEDIA_BUSY (EOS_WANBASE + 0xF4)  /* The media is busy. There is IR in the space. */
#define IR_BAD_INPUT  (EOS_WANBASE + 0xF5)  /* A bad input value was passed to the library call. */
#define IR_LAP_DISCON (EOS_WANBASE + 0xF6)  /* The IrLap link was disconnected during request. */
#define IR_MAX_ERROR  (EOS_WANBASE + 0xF6)  /* Maximum value of IRDA defined errors. */

/* Signal constants */
/* Do not use values 300-304. Currently the stack uses these values
for internal stack signals. Values 310 - 325 are available for signals
that are sent to the application. */
#define IR_SIG_APP_SIGNAL	310   /* Start value for application signals */
#define IR_SIG_DATA_AVAIL	310   /* Data is available on read path */
#define IR_SIG_NO_PROGRESS	311   /* Link is not making progress. IR blocked. */
#define IR_SIG_LINK_OKAY	312   /* Link is now making progress. IR unblocked. */
#define IR_SIG_LAP_DOWN		313   /* IrLap has gone down. */
#define IR_SIG_LAP_CONNECT	314   /* A connection to IrLap has been established. */
#define IR_SIG_LMP_DOWN		315   /* A connection to IrLmp has gone down. */
#define IR_SIG_LMP_CON_REQ	316   /* A connection to IrLmp has been requested. */
/* These signals are only used by the framer tester stack spirtest. */
#define IR_SIG_RX_IND		323   /* A packet has been received and is on the apps rx queue. */
#define IR_SIG_TX_CNF		324   /* A packet has been transmitted over ir. */
#define IR_SIG_MEDIA_CHG	325   /* The media has changed from not busy to busy. */
#define IR_MAX_SIGNAL		325   /* The maximum signal value. */

/*
 * Status of a path data queue query.
 */
#define IR_STATUS_NO_DATA        0  /* No Data is on any per_path send queue */
#define IR_STATUS_PP_DATA        1  /* Data on per_path send queue */
#define IR_STATUS_MULTI_DATA     2  /* Data on per_path and other path send queues */
#define IR_STATUS_OTHER_DATA     3  /* Data on other path send queues */

/* Packet Types */
/*  These constants are used in the type field of the irpacket structure. */
#define IR_TEST_PKT          10  /* 0x0a */
#define IR_TX_PACKET_CNF     11  /* 0x0b */
#define IR_RX_PACKET_IND     12  /* 0x0c */
#define IR_TX_PACKET         13  /* 0x0d */
#define IR_SIGNAL_PKT        14  /* 0x0e */
#define IR_APP_DATA          15  /* 0x0f */

/* Connection types used during an ir_open call. */
#define IR_LMP_CONNECT        0
#define IR_TTP_CONNECT        1

/* The number of extra bofs to be initialized in the framer. */
#define IR_EXTRA_BOFS         10
/*---------------------------------------------------------------------------
 * 
 * Maximum size of the XID info field used in a discovery frame. The XID
 * info field contains the device hints and nickname. Currently the 
 * maximum number of bytes for hints is 2. The maximum size allowed for 
 * the XID info field (according to the IrLMP specification) is 23 bytes.
 */
#define IR_MAX_XID_LEN   23

/* Maximum size of packet that can be sent at connect time (ConnectReq or
 * ConnectRsp) for IrLMP and Tiny TP connections. 
 */
#define IR_MAX_CON_PACKET     60
#define IR_MAX_TTP_CON_PACKET 52
#define IR_MAX_TEST_PACKET    376
#define IR_MAX_DEVICE_INFO    23

 /* Size of the device list used in discovery process 
 */
#define IR_DEVICE_LIST_SIZE 6

/* The following are used to access the hint bits in the first byte
 * of the Device Info field of an XID frame (IrDeviceInfo).
 */
#define IR_HINT_PNP       0x01
#define IR_HINT_PDA       0x02
#define IR_HINT_COMPUTER  0x04
#define IR_HINT_PRINTER   0x08
#define IR_HINT_MODEM     0x10
#define IR_HINT_FAX       0x20
#define IR_HINT_LAN       0x40
#define IR_HINT_EXT       0x80

/* The following are used to access the hint bits in the second byte
 * of the Device Info field of an XID frame (IrDeviceInfo). Note 
 * that LM_HINT_EXT works for all hint bytes.
 */
#define IR_HINT_TELEPHONY 0x01
#define IR_HINT_FILE      0x02
#define IR_HINT_IRCOMM    0x04
#define IR_HINT_MESSAGE   0x08
#define IR_HINT_HTTP      0x10
#define IR_HINT_OBEX      0x20

/****************************************************************************
 *
 * Data Structures.
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 *
 * ListEntry is for creating doubly linked circular lists. These can be used 
 * as either a list head, or as part of another structure to allow that 
 * structure to be a link node. This structure is used only by the stack. The
 * structure is defined here so the irpacket structure may be overlayed on
 * the data portion of the mbuf since the space is allocated in each mbuf by
 * the stack. 
 */
typedef struct  _ListEntry 
{
    struct _ListEntry *Flink;
    struct _ListEntry *Blink;

} ListEntry;

/* Definition of speeds. The speed values below correspond to the bit number 
 * in the IrLAP negotiation parameters.
 */
#define  IR_SPEED_2400      0   /* 2400 bps */
#define  IR_SPEED_9600      1   /* 9600 bps */
#define  IR_SPEED_19200     2   /* 19200 bps */
#define  IR_SPEED_38400     3   /* 38400 bps */
#define  IR_SPEED_57600     4   /* 57600 bps */
#define  IR_SPEED_115200    5   /* 115200 bps */ 
#define  IR_SPEED_576K      6   /* 576000 bps IRDA-2 */
#define  IR_SPEED_1152K     7   /* 1152000 bps IRDA-2 */
#define  IR_SPEED_4M        8   /* 4000000 bps IRDA-2 */

/* The number of negotiation parameters. This allows the negotiation parameters
 * to be accessed by an array instead of naming the parameter. */
#define IR_LAP_NEG_PARMS 6
/*---------------------------------------------------------------------------
 *
 * IrLAP Negotiation parameters is a structure for holding the IrLAP
 * parameters negotiated at connection time. The framer determines the
 * parameters required by IrLAP.
 */
typedef struct {
    u_int16 speed; /* This speed contains an ORed combination of the defined
                    * bit patterns for speeds. See IRLAP SPEED below. */
    
    /* The other parameters */
    union {
        u_int8 ptr[IR_LAP_NEG_PARMS];
        struct _named {
            u_int8 maxTurnAroundTime;
            u_int8 dataSize;
            u_int8 windowSize;
            u_int8 numBOFs;
            u_int8 minTurnAroundTime;
            u_int8 discThreshold;
        } named;
    } o;

} IrLapNegotiationParms;

/*---------------------------------------------------------------------------
 *
 * Framer capabilities structure includes negotiation parameters and
 * and other parameters affecting IrLAP
 *
 */

typedef struct {
    IrLapNegotiationParms p;

    /* Maximum size of data the framer can transmit. A common number is
     * is 2048 which is the maximum IrDA allows but it can be smaller.
     */
    u_int16                   txDataSize;

    /* Maximum windows size for transmit.
     */
    u_int8                    txWindowsSize;

#if IR_VARIABLE_POWER == IR_ENABLED
    /* Maximum power levels */
    u_int8                    powerLevels;
#endif /* IR_VARIABLE_POWER == IR_ENABLED */

} IrFramerCapabilities;

/*---------------------------------------------------------------------------
 *
 * The constants defined below are used to set the capabilities of the
 * framer. They must be set to the appropriate values for the hardware
 * and the framer. The values will be used by IrLAP as the values it
 * negotiates. In the case of Compact some of these values are fixed
 * and the settings here will not affect the parameters negotiated.
 *
 * These constants follow the bit pattern described in the IrLAP
 * specification. All lower bits should be 1's with the most significant
 * bit corresponding to the desired setting (the exception is speed where 
 * only the speeds supported are have a bit set). For example the bits for
 * data size are as follows:
 *
 *  bit 0  -   64 bytes
 *  bit 1  -  128 bytes
 *  bit 2  -  256 bytes
 *  bit 3  -  512 bytes
 *  bit 4  - 1024 bytes
 *  bit 5  - 2048 bytes
 *  bit 6  - reserved should be set to 0
 *  bit 7  - reserved should be set to 0
 *
 * This format is know as "bit pattern form".
 */

/*---------------------------------------------------------------------------
 *
 * IRLAP_SPEED is the speed supported by the framer. Speed is a 16 bit 
 * entity. Set this to the desired speed. To set the speed OR together
 * the desired speeds from the ones given below.
 */
#define IRBPS_2400     0x0001
#define IRBPS_9600     0x0002
#define IRBPS_19200    0x0004
#define IRBPS_38400    0x0008
#define IRBPS_57600    0x0010
#define IRBPS_115k     0x0020
#define IRBPS_576k     0x0040
#define IRBPS_1152K    0x0080
#define IRBPS_4M       0x0100
/* i.e.
#define IRLAP_SPEED  (IRBPS_2400 | IRBPS_9600 | IRBPS_19200 | IRBPS_38400 | \
                      IRBPS_57600 | IRBPS_115k)*/
 
/*---------------------------------------------------------------------------
 *
 * IRLAP_DATA_SIZE is the receive data size supported by the framer. It 
 * is used to negotiate the data size. IR_FRM_DATA_SIZE in entry.c is used 
 * to allocate memory.  The two must represent the same amount or major 
 * problems will occur. It is assumed that all data sizes smaller than the 
 * specified size are supported so to set IRLAP_DATA_SIZE use one of the 
 * IRDS_X constants defined below.
 */
#define IRDS_64     0x01
#define IRDS_128    0x03
#define IRDS_256    0x07
#define IRDS_512    0x0f
#define IRDS_1024   0x1f
#define IRDS_2048   0x3f
/* i.e.  
#define IRLAP_DATA_SIZE      IRDS_2048*/

#define IRFRMDS_64    64
#define IRFRMDS_128   128
#define IRFRMDS_256   256
#define IRFRMDS_512   512
#define IRFRMDS_1024  1024
#define IRFRMDS_2048  2048
/* i.e.  
#define IR_FRM_DATA_SIZE      IRFRMDS_2048*/

/*---------------------------------------------------------------------------
 *
 * IRLAP_WINDOW_SIZE is the window size that the framer can receive. In 
 * Compact and Extended the window size must be 1. In performance it can 
 * be up to 7. It is assumed that all window sizes smaller than the specified 
 * size are supported so to set IRLAP_WINDOW_SIZE use one of the IRWS_X 
 * constants defined below. (Bit pattern form)
 */
#define IRWS_1     0x01
#define IRWS_2     0x03
#define IRWS_3     0x07
#define IRWS_4     0x0f
#define IRWS_5     0x1f
#define IRWS_6     0x3f
#define IRWS_7     0x7f
/* i.e.  
#define IRLAP_WINDOW_SIZE    IRWS_1*/

/*---------------------------------------------------------------------------
 *
 * IRLAP_ADD_BOFS are the additional BOFs needed by the framer. Normally this 
 * is 0. It should not be changed unless more BOFs are actually needed.
 * Only the specific bit for the desired number of addtional BOFs needs to be 
 * set so to set IRLAP_ADD_BOFS use one of the IRAB_X constants defined below. 
 * (Bit pattern form)
 */
#define IRAB_48  0x01
#define IRAB_24  0x02
#define IRAB_12  0x04
#define IRAB_6   0x08
#define IRAB_3   0x10
#define IRAB_2   0x20
#define IRAB_1   0x40
#define IRAB_0   0x80
/* i.e.
#define IRLAP_ADD_BOFS       IRAB_0*/

/*---------------------------------------------------------------------------
 *
 * IRLAP_MIN_TURN_TIME is the minimum turn around time required by 
 * the hardware. To set IRLAP_MIN_TURN TIME use one of the IRMT_X constants 
 * defined below. (Bit pattern form)
 */
#define IRMT_10MS  0x01
#define IRMT_5MS   0x03
#define IRMT_1MS   0x07
#define IRMT_500US 0x0f
#define IRMT_100US 0x1f
#define IRMT_50US  0x3f
#define IRMT_10US  0x7f
#define IRMT_0     0xff
/* i.e.
#define IRLAP_MIN_TURN_TIME  IRMT_5MS*/

/*---------------------------------------------------------------------------
 *
 * These defines are used by the framer when the turn around index is set 
 * for responding to the device that is being communicated with.
 */
#define IR_INDEX_10MS  0
#define IR_INDEX_5MS   1
#define IR_INDEX_1MS   2
#define IR_INDEX_500US 3
#define IR_INDEX_100US 4
#define IR_INDEX_50US  5
#define IR_INDEX_10US  6
#define IR_INDEX_0     15

 /*---------------------------------------------------------------------------
 *
 * IRLAP_DISC_THRESHOLD is the link disconnect threshold used by IrLAP. 
 * In Extended and performance stacks the value can be changed, The most 
 * common setting is 40 seconds. For compact stacks the link disconnect is 
 * fixed to 3 seconds. To set IRLAP_DISC_THRESHOLD use one of the
 * IRDT_X constants defined below. (Bit pattern form)
 */
#define IRDT_3   0x01
#define IRDT_8   0x03
#define IRDT_12  0x07
#define IRDT_16  0x0f
#define IRDT_20  0x1f
#define IRDT_25  0x3f
#define IRDT_30  0x7f
#define IRDT_40  0xff
/* i.e.
#define IRLAP_DISC_THRESHOLD IRDT_40*/

/*---------------------------------------------------------------------------
 *
 * The maximum size data that can be sent. This is the actual number not
 * a bit pattern. The maximum size corresponds to the info field of
 * an IrLAP I-Frame and does not include the 2 bytes of IrLAP header.
 */
#define IRFRM_MAXTXDATASIZE   2048

/*---------------------------------------------------------------------------
 *
 * The maximum window size the framer can send. This is not a bit
 * pattern but the actual window size. Only useful in the performance 
 * version.
 */
#define IRFRM_MAXTXWINDOWSIZE 1

/*---------------------------------------------------------------------------
 *
 * IRLAP_MAX_TURN_TIME is the maximum turn around time required by 
 * IrLite.  This value is set to 500ms which is the bit pattern 0x01.
 */
#define IRLAP_MAX_TURN_TIME  IRMT_10MS

/****************************************************************************
 *
 * Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 *
 * Packet Structure for sending IrDA packets.
 */
typedef struct _IrPacket {
    /*========  The Following Are For Internal Use Only =========
     *
     * node
     * origin
     * header
     * headerLen
     *
     *==================================================================*/

    ListEntry  node;
    u_int8*        buff;        /* Pointer to the buffer of data to send */
    Mbuf           mb;          /* Pointer to the mbuf that this structure is contained in */
    void*          origin;      /* Pointer to connection which owns packet */
    u_int16        len;         /* Number of bytes in the buff */
    u_int8         type;        /* Type of the packet */
    u_int8         headerLen;   /* Number of bytes in the header */
    u_int8         more;		/* Flag for TTP SAR more bit */
    u_int8         bufspace[9]; /* space holders for 16 byte boundary */
    u_int8         header[14];  /* Storage for the header */
} IrPacket;

/*---------------------------------------------------------------------------
 *
 * 32-bit Device Address
 */
typedef u_int32 IrDeviceAddr;

/*---------------------------------------------------------------------------
 *
 * The information returned for each device discovered during discovery.
 * The maximum size of the xid field is 23. This holds the hints and
 * the nickname. For many implementation only hints is needed. Currently
 * only two bytes of hints have been defined. See irconfig.h to set
 * IR_MAX_XID_LEN to the desired value (2 for just hints).
 */
typedef  struct {
    IrDeviceAddr hDevice;            /* 32-bit address of device */
    u_int8           len;                /* Length of xid */
    u_int8           xid[IR_MAX_XID_LEN];/* XID information */
} IrDeviceInfo;

/*---------------------------------------------------------------------------
 *
 * List of Device Discovery info elements.
 */
typedef  struct {
  u_int8		nItems;                   /* Number items in the list */
  IrDeviceInfo	dev[IR_DEVICE_LIST_SIZE]; /* Fixed size in IrDA Lite */
} IrDeviceList;

/* Maximum size of a query that observes the IrDA Lite rules
 */
#define IR_MAX_QUERY_LEN 61

/* Maximum values for IAS fields. IR_MAX_IAS_NAME is the maximum allowable
 * size for IAS Object names and Attribute names.
 */
#define IR_MAX_IAS_NAME            60
#define IR_MAX_ATTRIBUTES         255

/* Maximum size of an IAS attribute that fits within the IrDA Lite rules. 
 * Even though attribute values can be larger IrDA Lite highly recommends 
 * that the total size of an attribute value fit within one 64 byte packet 
 * thus, the allowable size is 56 bytes or less. This size is enforced by the 
 * code.
 */
#define IR_MAX_IAS_ATTR_SIZE       56

/* Type of the IAS entry. This is the value returned for type when parsing 
 * the results buffer after a successful IAS Query.
 */
#define IRIAS_ATTRIB_MISSING      0
#define IRIAS_ATTRIB_INTEGER      1
#define IRIAS_ATTRIB_OCTET_STRING 2
#define IRIAS_ATTRIB_USER_STRING  3
#define IRIAS_ATTRIB_UNDEFINED    0xff

/* Ias Return Codes. One of these values will found in the IAS Query
 * structure in the retCode field after a successful IAS Query.
 */
#define IRIAS_RET_SUCCESS        0    /* Query operation is successful */
#define IRIAS_RET_NO_SUCH_CLASS  1    /* Query failed no such class exists */
#define IRIAS_RET_NO_SUCH_ATTRIB 2    /* Query failed no such attribute exists */
#define IRIAS_RET_UNSUPPORTED    0xff /* Query failed operation is unsupported */

/* IAS Get Value By Class opcode number */
#define IRIAS_GET_VALUE_BY_CLASS 4

/*---------------------------------------------------------------------------
 *
 * Character set for user strings. These are definitions for the character
 * set in Nicknames and in IAS attributes of type User String.
 */
#define IR_CHAR_ASCII       0
#define IR_CHAR_ISO_8859_1  1
#define IR_CHAR_ISO_8859_2  2
#define IR_CHAR_ISO_8859_3  3
#define IR_CHAR_ISO_8859_4  4
#define IR_CHAR_ISO_8859_5  5
#define IR_CHAR_ISO_8859_6  6
#define IR_CHAR_ISO_8859_7  7
#define IR_CHAR_ISO_8859_8  8
#define IR_CHAR_ISO_8859_9  9
#define IR_CHAR_UNICODE     0xff

/* Framer Description:  The following section contains definitions for the
 *                      framer. */
/****************************************************************************
 *
 * Types and Constants
 *
 ****************************************************************************/

/* Definitions used in frmFlags*/
#define IRFRM_FLAG_RX_DONE     0x01 /* Used for signalling when pkt is received.  Should
                                    not be needed in DMA. */
#define IRFRM_FLAG_MEDIA_BUSY  0x02 /* The media busy flag is set when ANY IR is received. */
#define IRFRM_FLAG_TX_DONE     0x04 /* Used to signal when a packet has sent the last char.
                                    Should not be needed in DMA. */
#define IRFRM_FLAG_TX_TRANS    0x08 /* Used for transmitting transparency. Not needed for DMA */
#define IRFRM_FLAG_TRANSMIT    0x10 /* For framer ISR to know if a packet is being transmitted.
                                    The receive fifo will be ignored.  If receive fifo may be 
                                    disabled in the hardware this flag will not be needed. */

#define IRLAP_POLL           0x10
#define IRLAP_BROADCAST      0xfe	 /* frame broadcast address*/

#define IRFCS_PRESET        0xffff  /* Initial FCS value*/
#define IRFCS_MASK          0xf0b8  /* Good final FCS value*/

#define IRLAP_XBOF          0xff
#define IRLAP_BOF           0xc0
#define IRLAP_EOF           0xc1
#define IRLAP_CE            0x7d
#define IRLAP_CE_COMP       0x20

/*---------------------------------------------------------------------------
 *
 * Maximum Turn Around Times
 */
#define IR_MAXTA_DEFAULT 0x00
#define IR_MAXTA_500MS   0x01
#define IR_MAXTA_250MS   0x03
#define IR_MAXTA_100MS   0x07
#define IR_MAXTA_50MS    0x0f

/*---------------------------------------------------------------------------
 *
 * Ultra Protocol ID values as defined in the Ultra specification.
 */
#define ULTRA_PID_OBEX 0x01

/* Maximum size of an Ultra packet */
#define IR_ULTRA_MAX_PACKET_SIZE 60

/*---------------------------------------------------------------------------
 *
 * Power Levels.
 */
#define IR_POWER_AUTO    0
#define IR_POWER_LOW     1
#define IR_POWER_MEDIUM  2
#define IR_POWER_HIGH    3

/****************************************************************************
 *
 * Framer Data Structures
 *  This is the statically allocated buffer for the framer to use to send
 *  signal packets up to the stack. This buffer is reused. The framer doesn't
 *  have to allocate an mbuf every time a signal is sent to the stack.
 ****************************************************************************/
typedef struct _irstatmbuf {
  mbuf mb;
  Dev_list lu_updrvr;
  IrPacket packet;
} irstatmbuf;


/****************************************************************************
 *
 * Stack Data Structures
 *
 ****************************************************************************/

/*---------------------------------------------------------------------------
 *
 * The LmIasAttribute is a strucutre that holds one attribute of an IAS 
 * object.
 */
typedef struct _IrIasAttribute {
    u_int8*            name;    /* Pointer to name of attribute */
    u_int8             len;     /* Length of attribute name */
    u_int8*            value;   /* Hardcode value (see below) */
    u_int8             valLen;  /* Length of the value. */
} IrIasAttribute;

/* The value field of the IrIasAttribute structure is a hard coded string 
 * which represents the actual bytes sent over the IR for the attribute 
 * value. The value field contains all the bytes as which represent an
 * an attribute value based on the transmission format described in section
 * 4.3 of the IrLMP specification. An example of a user string is given
 * below.
 *
 * User String:
 *   1 byte type,  1 byte Char set, 1 byte length, length byte string
 *
 * Example of an user string "Hello World" in ASCII
 *
 * u_int8 helloString[] = {
 *    IRIAS_ATTRIB_USER_STRING,IR_CHAR_ASCII,11,
 *    'H','e','l','l','o',' ','W','o','r','l','d'
 * };
 */

/*---------------------------------------------------------------------------
 *
 ***** Structure for IAS Object used by IrLMP of IrDA Protocol Module *****
 * The LmIasObject is storage for an IAS object managed by the local
 * IAS server.
 */
typedef struct _IrIasObject {
  u_int8*             name;      /* Pointer to name of object */
  u_int8              len;       /* Length of object name */
  u_int8              nAttribs;  /* Number of attributes */
  IrIasAttribute* attribs;   /* A pointer to an array of attributes */

} IrIasObject;

/*---------------------------------------------------------------------------
 *
 * Forward declaration of a structure used for performing IAS Queries so
 * that a callback type can be defined for use in the structure.
 */
typedef u_int8 IrStatus;
typedef void (*IrIasQueryCallBack)(Dev_list, IrStatus);

/*---------------------------------------------------------------------------
 *
 * Actual definition of the IrIasQuery structure.
 */
typedef struct _IrIasQuery
{
    /* Query fields. The query buffer contains the class name and class
     * attribute whose value is being queried it is as follows:
     *
     * 1 byte - Length of class name
     * "Length" bytes - class name
     * 1 byte - length of attribute name
     * "Length" bytes - attribute name
     *
     * queryLen - contains the total number of byte in the query
     */
    u_int8       queryLen;       /* Total length of the query */
    u_int8*      queryBuf;       /* Points to buffer containing the query */

    /* Fields for the query result */
    u_int16      resultBufSize;  /* Size of the result buffer */
    u_int16      resultLen;      /* Actual number of bytes in the result buffer */
    u_int16      listLen;        /* Number of items in the result list. */
    u_int16      offset;         /* Offset into results buffer */
    u_int8       retCode;        /* Return code of operation */
    u_int8       overFlow;       /* Set TRUE if result exceeded result buffer size */
    u_int8*      result;         /* Pointer to buffer containing result; */

    /* Pointer to callback function */
    IrIasQueryCallBack callBack;
} IrIasQuery;

/*---------------------------------------------------------------------------*/
/* API auxiliary defines for parsing the token returned from an ias query    */
/*---------------------------------------------------------------------------*/
#define IasGetu_int16(ptr)	(u_int16)( ((u_int16)(*((u_int8*)ptr) << 8)) | \
						((u_int16) (*((u_int8*)ptr+1))))

#define IasGetu_int32(ptr)	(u_int32)( ((u_int32)(*((u_int8*)ptr) << 24))   | \
						((u_int32)(*((u_int8*)ptr+1) << 16)) | \
						((u_int32)(*((u_int8*)ptr+2) << 8))  | \
						((u_int32)(*((u_int8*)ptr+3))) )

/*---------------------------------------------------------------------------*/
/* API function for parsing the token returned from an ias query             */
/*---------------------------------------------------------------------------
 *
 * Prototype:     u_int16 IRIAS_GetObjectID(IrIasQuery* token)
 *
 * Description:   Return the unique object ID of the current result item. 
 *
 * Parameters:    token - pointer to an IrIasQuery structure
 *
 * Return Values: object ID
 */
#define IRIAS_GetObjectID(t) IasGetu_int16((t)->result + (t)->offset)

/*---------------------------------------------------------------------------
 *
 * Prototype:     u_int8 IRIAS_GetType(IrIasQuery* token)
 *
 * Description:   Return the type of the current result item
 *
 * Parameters:    token - pointer to an IrIasQuery structure
 *
 * Return Values: Type of result item such as IRIAS_ATTRIB_INTEGER, 
 *                IRIAS_ATTRIB_OCTET_STRING or IRIAS_ATTRIB_USER_STRING.
 */
#define IRIAS_GetType(t) ((t)->result[(t)->offset + 2])

/*---------------------------------------------------------------------------
 *
 * Prototype:     u_int32 IRIAS_GetInteger(IrIasQuery* token)
 *
 * Description:   Return an integer value assuming that the current result 
 *                item is of type IRIAS_ATTRIB_INTEGER (call IRIAS_GetType() to 
 *                determine the type of the current result item). This 
 *                function is only supported  if IR_NO_32BIT_SUPPORT is NOT 
 *                defined.
 *
 * Parameters:    token - pointer to an IrIasQuery structure
 *
 * Return Values: Integer value.
 */
#define IRIAS_GetInteger(t) IasGetu_int32((t)->result + (t)->offset + 3)

/*---------------------------------------------------------------------------
 *
 * Prototype:     u_int8 IRIAS_GetIntLsap(IrIasQuery* token)
 *
 * Description:   Return an integer value that represents an LSAP assuming 
 *                that the current result item is of type IRIAS_ATTRIB_INTEGER 
 *                (call IRIAS_GetType() to determine the type of the current 
 *                result item). Usually integer values returned in a query
 *                are LSAP selectors.
 *
 * Parameters:    token - pointer to an IrIasQuery structure
 *
 * Return Values: Integer value.
 */
#define IRIAS_GetIntLsap(t) ((t)->result[(t)->offset + 6])

/****************************************************************************
 *
 * Prototype:     u_int16 IRIAS_GetOctetStringLen(IrIasQuery* token)
 *
 * Description:   Get the length of an octet string assuming that the current
 *                result item is of type IRIAS_ATTRIB_OCTET_STRING (call 
 *                IRIAS_GetType() to determine the type of the current result 
 *                item).
 *
 * Parameters:    token - pointer to an IrIasQuery structure
 *
 * Return Values: Length of octet string
 */
#define IRIAS_GetOctetStringLen(t) IasGetu_int16((t)->result + (t)->offset + 3)

/*---------------------------------------------------------------------------
 *
 * Prototype:     u_int8* IRIAS_GetOctetString(IrIasQuery* token)
 *
 * Description:   Return a pointer to an octet string assuming that the
 *                current result item is of type IRIAS_ATTRIB_OCTET_STRING (call 
 *                IRIAS_GetType() to determine the type of the current result 
 *                item).
 *
 * Parameters:    token - pointer to an IrIasQuery structure
 *
 * Return Values: pointer to octet string
 */
#define IRIAS_GetOctetString(t) ((t)->result + (t)->offset + 5)

/*---------------------------------------------------------------------------
 *
 * Prototype:     u_int8 IRIAS_GetUserStringLen(IrIasQuery* token)
 *
 * Description:   Return the length of a user string assuming that the
 *                current result item is of type IRIAS_ATTRIB_USER_STRING (call
 *                IRIAS_GetType() to determine the type of the current result 
 *                item).
 *
 * Parameters:    token - pointer to an IrIasQuery structure
 *
 * Return Values: Length of user string
 */
#define IRIAS_GetUserStringLen(t) ((t)->result[(t)->offset + 4])

/****************************************************************************
 *
 * Prototype:     IrCharSet IRIAS_GetUserStringCharSet(IrIasQuery* token)
 *
 * Description:   Return the character set of the user string assuming that
 *                the current result item is of type IRIAS_ATTRIB_USER_STRING 
 *                (call IRIAS_GetType() to determine the type of the current 
 *                result item).
 *
 * Parameters:    token - pointer to an IrIasQuery structure
 *
 * Return Values: Character set
 */
#define IRIAS_GetUserStringCharSet(t) ((t)->result[(t)->offset + 3])

/*---------------------------------------------------------------------------
 *
 * Prototype:     u_int8* IRIAS_GetUserString(IrIasQuery* token)
 *
 * Description:   Return a pointer to a user string assuming that the
 *                current result item is of type IRIAS_ATTRIB_USER_STRING (call
 *                IRIAS_GetType() to determine the type of the current result 
 *                item).
 *
 * Parameters:    token - pointer to an IrIasQuery structure
 *
 * Return Values: Pointer to result string
 */
#define IRIAS_GetUserString(t) ((t)->result + (t)->offset + 5)

/***** Structure for parameter block *************************************/
typedef struct ss_irda_pb {
	spf_ss_pb			spb;
	u_int8				speed;			/* Set to bit number for speed */
	u_int8				setmedia;		/* Flag to indicate media busy (1 or 0) */
	u_int8				turnindex;		/* Minimum turn around time index */
	u_int8				numbofs;		/* Number of extra bofs to be sent */
	u_int16				mbufsize;		/* The mbuf size to be supported */
	u_int8				getmedia;		/* The returning media busy flag (1 or 0) */
	u_int8				getstatus;		/* Returning status of the framer */
	IrFramerCapabilities*	capabilities;	/* Pointer to the capabilities structure */
	IrDeviceList*		deviceList;		/* Discovery result structure */
	IrDeviceAddr		condevaddr;		/* Address of the device to connect to */
	u_int8				data_pending;	/* Status of data pending on all paths */
	u_int8				connecttype;	/* IrLMP = 0, Tiny TP = 1 */
	u_int8				rlsap;			/* Remote lsap to connect to */
	u_int8				lsap;			/* Local lsap of the connection opened */
	u_int8*				connectdata;	/* Pointer to connection data */
	u_int16				datalen;		/* Length of the connection data */
	u_int16				lis_time;		/* Time in 1/100ths of second to listen. 0 is infinite */
	process_id			appid;			/* Process id of application */
	IrIasQuery*			token;			/* Ias query request and results */
	IrIasObject*		iasobject;		/* Object to add or remove in ias */
	u_int8*				deviceInfo;		/* Device info to be sent on discovery */
	u_int8				deviceInfoLen;	/* Length of the deviceInfo string */
	u_int16				regsig;			/* Signal to be registered in this path */
} ss_irda_pb, *Ss_irda_pb;

/************* SETSTAT & GETSTAT CODES  ***********************************/
/**************************************************************************/

/****** Framer Codes ******************************************************/

#define SS_BAUD_SET 	((SPF_PR_IRDA<<16) + 1) /* Set the Baud rate */
#define SS_SET_MEDIA	((SPF_PR_IRDA<<16) + 2) /* Set media busy flag */
#define SS_TURN_INDEX	((SPF_PR_IRDA<<16) + 3)	/* Turn around time index */
#define SS_NUMBOFS 	  ((SPF_PR_IRDA<<16) + 4)	/* Set number of bofs. */
#define SS_MBUF_SIZE	((SPF_PR_IRDA<<16) + 5)	/* Negotiated mbuf size. */
													   
#define GS_MEDIA_BUSY	((SPF_PR_IRDA<<16) + 51) /* Get media busy flag */
#define GS_FRM_STATUS	((SPF_PR_IRDA<<16) + 52) /* Get framer status */
#define GS_CAPABILITIES ((SPF_PR_IRDA<<16) + 53) /* Get capabilities */

/****** Stack Codes *******************************************************/
#define SS_DISCOVER 	((SPF_PR_IRDA<<16) + 100) /* Do a discovery. */
#define SS_MEDIA_CHANGE ((SPF_PR_IRDA<<16) + 101) /* Notify stack of media
                                                       change. */
#define SS_IRLAP_CONNECT ((SPF_PR_IRDA<<16) + 102) /* Connect to irlap */
#define SS_IRLAP_DISCONNECT ((SPF_PR_IRDA<<16) + 103) /* Disconnect from irlap */
#define SS_IR_CONNECT ((SPF_PR_IRDA<<16) + 104) /* irlmp or tinytp connection */
#define SS_CONNECT_TYPE ((SPF_PR_IRDA<<16) + 105) /* irlmp or tinytp connection */
#define SS_LISTEN ((SPF_PR_IRDA<<16) + 106) /* process has started to listen for connection */
#define SS_IAS_QUERY ((SPF_PR_IRDA<<16) + 107) /* Start ias query */
#define SS_IAS_ADD ((SPF_PR_IRDA<<16) + 108) /* Add entry to ias */
#define SS_IAS_REMOVE ((SPF_PR_IRDA<<16) + 109) /* Remove entry from ias */
#define SS_DEV_INFO ((SPF_PR_IRDA<<16) + 110) /* Set the device info */
#define SS_SIGREG ((SPF_PR_IRDA<<16) + 111) /* Register the signal for this path */
#define SS_SIGUNREG ((SPF_PR_IRDA<<16) + 112) /* Unregister the signal for this path */

#define GS_IR_TXDATA ((SPF_PR_IRDA<<16) + 150) /* Is tx data pending */

/****** Test Codes *******************************************************/
#define GS_RX_PKT_LEN ((SPF_PR_IRDA<<16) + 160) /* How much rx data last put on app rx queue */
/******* FUNCTION PROTOTYPES **********************************************/
/**************************************************************************/

/****** IrDA Framer API function prototypes *******************************/

error_code irfrmr_ss_hwen(path_id path);
error_code irfrmr_ss_hwdis(path_id path);
error_code irfrmr_ss_baud(path_id path, u_int8 speed);
error_code irfrmr_ss_media_busy(path_id path, u_int8 flag);
error_code irfrmr_ss_turnindex(path_id path, u_int16 flag);
error_code irfrmr_ss_bofs(path_id path, u_int16 flag);
error_code irfrmr_ss_mbufsize(path_id path, u_int16 flag);

error_code irfrmr_gs_media_busy(path_id path, u_int8* media);
error_code irfrmr_gs_frm_status(path_id path, u_int8* status);
error_code irfrmr_gs_frm_caps(path_id path, IrFramerCapabilities* capabilities);

/****** IrDA Stack API function prototypes *********************************/

error_code ir_discover(path_id path, IrDeviceList* deviceListPT);
error_code ir_ias_query(path_id path, IrIasQuery* token);
error_code ir_ias_add(path_id path, IrIasObject* iasobject);
error_code ir_ias_remove(path_id path, IrIasObject* iasobject);
error_code ir_connect(path_id path, u_int8 rlsap, u_int8* data, u_int16* len);
error_code ir_listen(path_id path, u_int8* data, u_int16* len, u_int16 lis_time);
error_code ir_deviceinfo(path_id path, u_int8* deviceInfo, u_int8 deviceInfoLen);
error_code ir_open(path_id* path, char* driver, u_int8 contype, u_int8* lsap);
error_code ir_connectlap(path_id path, IrDeviceAddr addr);
error_code ir_disconnectlap(path_id path);
error_code ir_read(path_id path, u_char* buf, u_int32* readamount);
error_code ir_write(path_id path, u_char* buf, u_int32* sendamount);
error_code ir_close(path_id path);
error_code ir_data_pending(path_id path, u_int8* data_pending);
error_code ir_status_regsig(path_id path, u_int16 signalcode);
error_code ir_status_unregsig(path_id path, u_int16 signalcode);

#endif /*_IRDA_H */
