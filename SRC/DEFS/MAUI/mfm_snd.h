/*******************************************************************************
 * FILENAME : mfm_snd.h
 *
 * DESCRIPTION :
 *
 *  This file contains definitions for the MAUI Sound Driver API
 *
 * WARNING:
 *
 *  When the MAUI sound API is finished, this interface will be hidden from
 *  applications. Directly using this interface in you applications is only
 *  recommended until the official API is available.
 *
 * COPYRIGHT:
 *
 *  This source code is the proprietary confidential property of Microware
 *  Systems Corporation, and is provided to licensee solely for documentation
 *  and educational purposes. Reproduction, publication, or distribution in
 *  any form to any party other than the licensee is strictly prohibited.
 *
 * EDITION  DATE      WHO  DETAIL
 *       1  01/12/96  EBM  First version
 *       2  01/15/96  TJE  Made consistent with other MAUI driver interfaces
 *       3  05/15/96  SRM  Brought up to date with Draft 2.4 of SDS
 */
#ifndef _MFM_SND_H
#define _MFM_SND_H

#include <MAUI/maui_snd.h>

/*******************************************************************************
* GETSTAT PARAMETER BLOCKS
*******************************************************************************/

typedef struct {
    u_int16 func_code;          /* Must be FC_SND_COMPAT */
    u_int32 *ret_sdv_compat;    /* Driver compatibility level */
    u_int32 api_compat;         /* API compatibility level */
} gs_snd_compat_pb, *Gs_snd_compat_pb;

typedef struct {
  u_int16 func_code;            /* Must be FC_SND_DEVCAP */
  SND_DEV_CAP **ret_dev_cap;    /* Device capabilities */
} gs_snd_devcap_pb, *Gs_snd_devcap_pb;

typedef struct {
  u_int16 func_code;            /* Must be FC_SND_DEV_STATUS */
  SND_DEV_STATUS **ret_status;  /* Status of device*/
} gs_snd_status_pb, *Gs_snd_status_pb;

/*******************************************************************************
* SETSTAT PARAMETER BLOCKS
*******************************************************************************/

/* _os_ss_relea() uses the standard pb  from os_lib.l */
/* _os_ss_sendsig() uses the standard pb from os_lib.l */


typedef struct {
  u_int16 func_code;            /* Must be FC_SND_ABORT */
} ss_snd_abort_pb, *Ss_snd_abort_pb;

typedef struct {
  u_int16 func_code;            /* Must be FC_SND_CONT */
} ss_snd_cont_pb, *Ss_snd_cont_pb;

typedef struct {
  u_int16 func_code;            /* Must be FC_SND_VOLUME */
  SND_GAIN *gain;               /* Gain setting */
} ss_snd_gain_pb, *Ss_snd_gain_pb;

typedef struct {
  u_int16 func_code;            /* Must be FC_SND_PAUSE */
} ss_snd_pause_pb, *Ss_snd_pause_pb;

typedef struct {
  u_int16 func_code;            /* Must be FC_SND_PLAY */
  SND_SMAP *smap;               /* Sound map */
  SND_BLOCK_TYPE block_type;    /* Type of blocking to perform */
} ss_snd_play_pb, *Ss_snd_play_pb;

typedef struct {
  u_int16 func_code;            /* Must be FC_SND_RECORD */
  SND_SMAP *smap;               /* Sound map */
  SND_BLOCK_TYPE block_type;    /* Type of blocking to perform */
} ss_snd_record_pb, *Ss_snd_record_pb;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code _os_gs_snd_compat(path_id path, u_int32 *ret_sdv_compat, u_int32 api_compat);
error_code _os_gs_snd_devcap(path_id path, SND_DEV_CAP **ret_dev_cap);
error_code _os_gs_snd_status(path_id path, SND_DEV_STATUS **ret_status);
error_code _os_ss_snd_abort(path_id path);
error_code _os_ss_snd_cont(path_id path);
error_code _os_ss_snd_gain(path_id path, SND_GAIN *gain);
error_code _os_ss_snd_pause(path_id path);
error_code _os_ss_snd_play(path_id path, SND_SMAP *smap, SND_BLOCK_TYPE block_type);
error_code _os_ss_snd_record(path_id path, SND_SMAP *smap, SND_BLOCK_TYPE block_type);

#if 0 /* The following calls are supported, but use the standard
         bindings from os_lib.l */
error_code _os_attach(const char *name, u_int32 mode, dev_list **dev_tbl);
error_code _os_close(path_id path);
error_code _os_detach(dev_list *dev_tbl);
error_code _os_open(const char *name, u_int32 mode, path_id *path);
error_code _os_ss_relea(path_id path); 
error_code _os_ss_sendsig(path_id path, signal_code signal);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MFM_SND_H */
