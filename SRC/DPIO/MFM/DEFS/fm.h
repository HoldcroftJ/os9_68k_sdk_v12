/****************************************************************************
 *  $Workfile:   mfm_fm.h  $
 *  $Revision:   1.9  $
 *  $Modtime:   15 Mar 1994 14:49:00  $
 *  $Date:   15 Mar 1994 14:49:58  $
 ****************************************************************************
 *	MFM File Manager interface & data structure definitions.				*
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
 *    0 01/03/94    Creation                                            bat *
 *    1 01/12/94    More initial fixes                                  bat *
 *      03/15/94    <***************** SPF v1.0 ******************>         *
 *      06/01/95    <***************** MFM v1.0 ******************>     cjv *
 ****************************************************************************/

#ifndef _FM_H
#define _FM_H

/*
 *Header Files
 */

#include <types.h>
#include <srvcb.h>
#include <io.h>

#include <maui_fm.h>

/*
 *	Macro Definitions
 */

#define FM_RSV_DEVENT	0			/* Device entry field of fm_rsv[] */

/*
 *	Type Definitions
 */

/***** MFM FILE MANAGER STATIC STORAGE ******/
/***** MFM FILE MANAGER STATIC STORAGE ******/
/***** MFM FILE MANAGER STATIC STORAGE ******/
typedef struct mfm_fm_stat {	
  u_int32
	fm_entrycnt;				/* Number of OS entry points (16) */
#define FM_ENTRYCNT		16
  error_code
	/* Address of "attach" function */
	(*fm_attach)(i_attach_pb *pb, Dev_list dev_entry),
	/* Address of "chgdir" function */
	(*fm_chdir)(i_chdir_pb *pb, Mfm_path_desc pd),
	/* Address of "close" function */
	(*fm_close)(i_close_pb *pb, Mfm_path_desc pd),
	/* Address of "create" function */
	(*fm_create)(i_create_pb *pb, Mfm_path_desc pd),
	/* Address of "delete" function */
	(*fm_delete)(i_delete_pb *pb, Mfm_path_desc pd),
	/* Address of "detach" function */
	(*fm_detach)(i_detach_pb *pb, Dev_list dev_entry),
	/* Address of "dup" function */
	(*fm_dup)(i_dup_pb *pb, Mfm_path_desc pd),
	/* Address of "getstat" function */
	(*fm_getstat)(i_getstat_pb *pb, Mfm_path_desc pd),
	/* Address of "makdir" function */
	(*fm_makdir)(i_makdir_pb *pb, Mfm_path_desc pd),
	/* Address of "open" function */
	(*fm_open)(i_open_pb *pb, Mfm_path_desc pd),
	/* Address of "read" function */
	(*fm_read)(i_read_pb *pb, Mfm_path_desc pd),
	/* Address of "readln" function */
	(*fm_readln)(i_readln_pb *pb, Mfm_path_desc pd),
	/* Address of "seek" function */
	(*fm_seek)(i_seek_pb *pb, Mfm_path_desc pd),
	/* Address of "setstat" function */
	(*fm_setstat)(i_setstat_pb *pb, Mfm_path_desc pd),
	/* Address of "write" function */
	(*fm_write)(i_write_pb *pb, Mfm_path_desc pd),
	/* Address of "writeln" function */
	(*fm_writeln)(i_writeln_pb *pb, Mfm_path_desc pd);
  u_int32
	fm_att_cnt;					/* Number of attaches using file manager */
  u_char
	fm_rsv2[16];				/* RESERVED FOR FUTURE USE! */
} mfm_fm_stat, *Mfm_fm_stat;	/* size = 128 bytes							*/


#endif /* _FM_H */



