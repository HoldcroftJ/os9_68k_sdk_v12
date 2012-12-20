/****************************************************************************
 *  $Workfile:   maui_fm.h  $
 *  $Revision:   1.10  $
 *  $Modtime:   15 Mar 1994 13:52:00  $
 *  $Date:   15 Mar 1994 13:53:06  $
 ****************************************************************************
 *  General FM application definitions.                                 *
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
 *   10 03/04/97    Changed DT_MFM from 20 to 17 for OS9000 and 16          *
 *                  for OSK to match io.h, added pd_version             srm *
 *   14 01/02/01    CF2394 Creating MAUIdevice for MSG and CDB              *
 *                  enhancements.                                       srm *
 ****************************************************************************/

#ifndef _MAUI_FM_H
#define _MAUI_FM_H

/*
 *  Header Files
 */
#include <types.h>

/* This is here because not all versions of io.h define DT_MFM and it
   is defined differently in _OSK from _OS9000 */
#ifndef DT_MFM
  #if defined(_OSK)
    #define DT_MFM 16
  #elif defined(_OS9000)
    #define DT_MFM 17
  #else
    #error "_OSK or _OS9000 must be defined"
  #endif
#endif

/*
 *  Type Definitions
 */

#if !defined(GS_MOD_PB)
typedef struct gs_mod_pb {
  u_int32     mod_code;           /* Setstat/getstat module  code         */
  u_int32     mod_size;           /* Size of mod_param (if needed)        */
  void        *mod_param;         /* Module parameter block               */
  void        *mod_pd;            /* Mfm_path_desc */
} gs_mod_pb, *Gs_mod_pb;
#define GS_MOD_PB
#endif

#if !defined(SS_MOD_PB)
typedef struct ss_mod_pb {
  u_int32     mod_code;           /* Setstat/getstat module  code         */
  u_int32     mod_size;           /* Size of mod_param (if needed)        */
  void        *mod_param;         /* Module parameter block               */
  void        *mod_pd;            /* Mfm_path_desc */
} ss_mod_pb, *Ss_mod_pb;
#define SS_MOD_PB
#endif

/******* LOGICAL UNIT OPTIONS STORAGE *******/
/******* LOGICAL UNIT OPTIONS STORAGE *******/
/******* LOGICAL UNIT OPTIONS STORAGE *******/
typedef struct mfm_lu_opt {     
  u_int16   
    lu_optsize;                 /* Size of logical unit options */
  u_char
    lu_class,                   /* Device type (DT_MFM= ) */
    lu_errs;                    /* Accumulated I/O errors */
  
  u_char  lu_rsv1[27];              /* RESERVED FOR FUTURE USE!             */
  
} mfm_lu_opt, *Mfm_lu_opt;


/***** PATH DESCRIPTOR OPTIONS STORAGE ******/
/***** PATH DESCRIPTOR OPTIONS STORAGE ******/
/***** PATH DESCRIPTOR OPTIONS STORAGE ******/
typedef struct mfm_path_opt {
#if defined(_OSK)
  u_int8        pd_devtype;         /* device type (DT_MFM) */
  u_int8        pd_rsv1;            /* RESERVED FOR FUTURE USE! */
#elif defined(_OS9000)
  u_int16       pd_devtype;         /* device type (DT_MFM) */
#else
  #error "_OSK or _OS9000 must be defined"
#endif
  u_int16
    pd_optsize;                 /* Path options size (128) */
  u_int16
    pd_version;                 /* current MFM revision (MFM_VERSION) */
  u_int8
    pd_rsv2[128-6];             /* RESERVED FOR FUTURE USE! */
} mfm_path_opt, *Mfm_path_opt;  /* size = 128 bytes */


#endif /* _MAUI_FM_H */
