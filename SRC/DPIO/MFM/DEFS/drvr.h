/****************************************************************************
 *  $Workfile:   drvr.h  $
 *  $Revision:   $
 *  $Modtime:   $
 *  $Date:    $
 ****************************************************************************
 *	MFM Device Driver interface & data structure definitions.				*
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
 *      04/11/00    Added dr_read and dr_write                          jph * 
 ****************************************************************************/
#ifndef _DRVR_H
#define _DRVR_H

/*
 *	Header Files
 */
#include <types.h>
#include <srvcb.h>
#include <io.h>

#include <maui_fm.h>

/*
 *	Macro Definitions
 */
 
/*
 *	Type Definitions
 */

/******* DEVICE DRIVER STATIC STORAGE *******/
/******* DEVICE DRIVER STATIC STORAGE *******/
/******* DEVICE DRIVER STATIC STORAGE *******/
typedef struct mfm_dr_stat {	
  error_code
	(*dr_iniz)(Dev_list dev_entry),
	/* Deinitialize hardware */
	(*dr_term)(Dev_list dev_entry),
	/* Driver GetStat */
	(*dr_getstat)(Dev_list dev_entry, gs_mod_pb *gs_pb),
	/* Driver SetStat */
	(*dr_setstat)(Dev_list dev_entry, ss_mod_pb *ss_pb);
  u_int32
	dr_att_cnt;					/* Number of attaches using this driver */
  	/* Driver Read */
  error_code
  	(*dr_read)(Dev_list dev_entry, i_read_pb *gs_pb),
	/* Driver Write */
	(*dr_write)(Dev_list dev_entry, i_write_pb *gs_pb);
  u_char
	dr_rsv1[12];				/* RESERVED FOR FUTURE USE! */
  
#ifdef DR_SPECIFICS	
  DR_SPECIFICS					/* Device driver specific static storage */
#endif
	
} mfm_dr_stat, *Mfm_dr_stat;

/******** LOGICAL UNIT STATIC STORAGE *******/
/******** LOGICAL UNIT STATIC STORAGE *******/
/******** LOGICAL UNIT STATIC STORAGE *******/
typedef struct mfm_lu_stat {	
  
  /* General Logical Unit Information Fields */
  u_char
	lu_share,					/* Path sharing flag */
	lu_rsv1[2];					/* RESERVED FOR FUTURE USE! */
  u_int32
	lu_att_cnt,					/* Number of attaches using logical unit */
	lu_use_cnt,					/* Path use count */
	lu_num;						/* Logical unit number */
  void
	*lu_port;					/* Port Address */
  
  /* Device Driver Logical Unit Information Fields */
  u_char
		lu_rsv2[2];					/* RESERVED FOR FUTURE USE! */
  
  /* Unused Logical Unit Fields */
  u_char
	lu_rsv3[40];				/* RESERVED FOR FUTURE USE! */
  
  /* Logical Unit Options Fields */
  mfm_lu_opt
	lu_luopts;					/* Logical unit options section */
  
  /* Optional Logical Unit Extensions Fields */
#ifdef LU_SPECIFICS	
  LU_SPECIFICS					/* Logical Unit specific static storage */
#endif
	
} mfm_lu_stat, *Mfm_lu_stat;

/****** MFM DEVICE DESCRIPTOR STORAGE *******/
/****** MFM DEVICE DESCRIPTOR STORAGE *******/
/****** MFM DEVICE DESCRIPTOR STORAGE *******/
typedef struct mfm_desc {
  dd_com
	dd_desccom;					/* Common OS-9000 descriptor section */
  mfm_path_opt
	dd_popts;					/* Path descriptor section */
  u_char
	dd_rsv2[26];				/* RESERVED FOR FUTURE USE! */
} mfm_desc, *Mfm_desc;			

/********* PATH DESCRIPTOR STORAGE **********/
/********* PATH DESCRIPTOR STORAGE **********/
/********* PATH DESCRIPTOR STORAGE **********/
typedef struct mfm_path_desc {	
  struct pathcom
	pd_common;					/* Common path descriptor section */
  mfm_path_opt
	pd_opts;					/* Path descriptor options section */
  void 
    *pd_drvr_exten;             /* Reseved for use by drivers for
                                   extensions */
  u_char
	pd_rsv1[8];				    /* RESERVED FOR FUTURE USE! */
} mfm_path_desc, *Mfm_path_desc;

#endif /* _DRVR_H */
