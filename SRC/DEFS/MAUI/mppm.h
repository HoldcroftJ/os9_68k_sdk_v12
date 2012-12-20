
/*******************************************************************************
* FILENAME : mppm.h
*
* DESCRIPTION :
*
*	This file contains public definitions for the Maui Process
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  01/06/95  srm  create
*       2  03/13/95  srm  added support for real input arbitration
*/
#ifndef _MPPM_H
#define _MPPM_H

#include <MAUI/maui_inp.h>
#include <module.h>

#define EOS_UNFINISHED 1

/* create on inp_init and destroy on inp_term */
#define _MP_PROC_SYNC 0x40505043      /* "@PPC" */
typedef struct _MP_PROC {
  u_int32         sync_code;	      /* Sync code - must be _MP_PROC_SYNC */
  u_int32         api_compat_level;   /* compat level of api */
  process_id      pid;                /* process ID of the caller */
  MSG_MBOX_ID     reply_mbox_id;      /* reply mailbox ID */
  struct _MP_PROC *next, *prev;       /* link list ptrs */
  struct _MP_DEV  *mp_dev_head;       /* first device of a registered process */
} MP_PROC; /* seen by the INP API as MP_PROC_ID */

/* create on any inp_open and destroy on inp_close */
#define _MP_DEV_SYNC 0x4d504456       /* "MPDV" */
typedef struct _MP_DEV {
  u_int32         sync_code;	      /* Sync code - must be _MP_DEV_SYNC */
  struct _MP_PROC *proc;              /* ptr to owner MP_PROC */
  struct _MP_DEV  *proc_next_dev;     /* Next MP_DEV with the same MP_PROC */
  struct _MP_MPPM    *mppm;              /* ptr to device and pmod */
  struct _MP_DEV  *mppm_next_dev;     /* Next MP_DEV with the same MP_MPPM */
  MSG_MBOX_ID     app_mbox_id;        /* mailbox ID to send event messages */
  INP_DEV_ID      device_id;          /* ID to return in PTR and KEY messages */
} MP_DEV;

/* create on first inp_open to a device and protocol module and
   destroy on last inp_close */
typedef struct _MP_MPPM {
  u_int32         maui_inp_compat_level; /* compat level of mauip */
  /* owner info */
  struct _MP_DEV  *mp_dev_head;       /* ptr to current dev entry */
  struct _MP_MPPM *next, *prev;       /* MPPM link list ptrs */
  /*raw  device info */
  path_id         dev_path;           /* path id of input device */
  char            dev_type;           /* type of device DT_SCF, DT_UCM, etc. */
  char            dev_name[INP_MAX_DEV_NAME];
  /* protocol module info */
  u_int32         pmod_compat_level;  /* compat level of pmod */
  mh_com          *pmod_head;         /* ptr to head of the MAUI protocol module */
  char            pmod_name[INP_MAX_DEV_NAME];
  void            *pmod_mem;          /* ptr to static memory for the MAUI protocol module */
  void            *pmod_functable;    /* ptr to function offset table of the MAUI protocol module */
} MP_MPPM;

#define _MP_ID_STRUCTS_EXPOSED
#define MP_PROC_ID MP_PROC *
#define MP_DEV_ID MP_DEV *
#include <MAUI/mp.h>


/*******************************************************************************
 * Protocol Module PROTOTYPES
 *******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code mppm_initsize(MP_MPPM *mppm, size_t *mem_size);
error_code mppm_init(MP_MPPM *mppm, void *mem_buf, size_t mem_size);
error_code mppm_attach(MP_DEV *mp_dev);
error_code mppm_process_msg(MP_MPPM *mppm, MP_DEV_MSG *cmd_msg, MP_DEV_MSG **reply_msg);
error_code mppm_process_data(MP_MPPM *mppm, u_char **buf, size_t *buf_size, MSG_MBOX_ID *mbox_id, INP_MSG **reply_msg);
error_code mppm_detach(MP_DEV *mp_dev);
error_code mppm_term(MP_MPPM *mppm);

#ifdef __cplusplus
}
#endif

#endif /* _MPPM_H */
