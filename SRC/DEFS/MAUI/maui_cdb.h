
/*******************************************************************************
 *                                                                             *
 *              Copyright 1994 by Microware Systems Corporation                *
 *                         Reproduced Under License.                           *
 *                                                                             *
 *  This source  code is the  proprietary confidential  property of Microware  *
 *  Systems Corporation and is provided to  licensee solely for documentation  *
 *  and  educational purposes.   Reproduction,  publication, or  distribution  *
 *  in any form to any  party other than the licensee is strictly prohibited.  *
 *                                                                             *
 *******************************************************************************/

#ifndef _MAUI_CDB_H
#define _MAUI_CDB_H

#include <types.h>
#include <MAUI/maui_com.h>
#include <MAUI/maui_mem.h>

/*******************************************************************************
* DEFINITIONS FOR DDR FUNCTIONS
*******************************************************************************/

/* CDB Device Type Codes */
typedef enum {
  CDB_TYPE_SYSTEM     = 0,   /* System */
  CDB_TYPE_CDC        = 1,   /* CD-Control Unit */
  CDB_TYPE_SOUND      = 2,   /* Sound Processor */
  CDB_TYPE_GRAPHIC    = 3,   /* Video Output Processor */
  CDB_TYPE_NVRAM      = 4,   /* Non-volatile Random Access memory */
  CDB_TYPE_REMOTE     = 5,   /* Remote Pointing and Key Devices */
  CDB_TYPE_IROUT      = 6,   /* IR Output Blaster */
  CDB_TYPE_PIPEDEV    = 9,   /* Pipe Device */
  CDB_TYPE_SER        = 20,  /* SCF Serial Device */
  CDB_TYPE_PRNT       = 21,  /* SCF Parallel Printer Device */
  CDB_TYPE_MIDI       = 25,  /* SCF MIDI device */
  CDB_TYPE_LED        = 26,  /* LED device */
  CDB_TYPE_RAM        = 30,  /* RAM extensions */
  CDB_TYPE_FLASH      = 31,  /* FLASH RAM */
  CDB_TYPE_FD         = 40,  /* RBF (Universal Format) Floppy Disk */
  CDB_TYPE_HD         = 50,  /* RBF Hard Disk */
  CDB_TYPE_PCFD       = 60,  /* PCF (MS-DOS Format) Floppy Disk */
  CDB_TYPE_PCHD       = 70,  /* PCF (MS-DOS Format) Hard Disk */
  CDB_TYPE_TAPE       = 80,  /* SBF Format Magnetic Tape */
  CDB_TYPE_MPV        = 90,  /* MPEG Video */
  CDB_TYPE_MPA        = 91,  /* MPEG Audio */
  CDB_TYPE_ANET       = 100, /* NFM (Arcnet) Local Area Network */   
  CDB_TYPE_ENET       = 101, /* IFMAN (Ethernet) Local Area Network */
  CDB_TYPE_ISDN       = 110, /* ISM (ISDN) Wide Area Network */
  CDB_TYPE_RTNFM      = 111, /* RTNFM Real-Time Wide Area Network */
  CDB_TYPE_SPF        = 112, /* SPF Device */
  CDB_TYPE_CTRLCHAN   = 113, /* Control Channel device */
  CDB_TYPE_DATACHAN   = 114, /* Data Channel device */
  CDB_TYPE_MACFD      = 120, /* MACFM floppy disk */
  CDB_TYPE_MACHD      = 130, /* MACFM hard disk */
  CDB_TYPE_WIN        = 1000 /* MAUI Win logical device */
} CDB_TYPE;

/* useful constants */
#define CDB_MAX_DNAME   80	/* maximum length of a device name */
#define CDB_MAX_PARAM   128 /* maximum length of a parameter string */

/* CDB ANSI Function Prototypes */

#ifdef __cplusplus
extern "C" {
#endif

error_code cdb_get_copy (char **buf, u_int32 shade_id);
error_code cdb_get_ddr(CDB_TYPE type, u_int32 num, char *name, char *param);
error_code cdb_get_ncopy (char *buf, size_t *size);
error_code cdb_get_size (size_t *size);
error_code cdb_init(void);
error_code cdb_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
    passback_level, MAUI_ERR_LEVEL exit_level);
error_code cdb_term(void);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_CDB_H */
