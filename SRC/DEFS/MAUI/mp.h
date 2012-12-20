/*******************************************************************************
 * FILENAME : mp.h
 *
 * DESCRIPTION :
 *
 *  This file contains public definitions for the Maui Process
 *
 * COPYRIGHT:
 *
 *  This source code is the proprietary confidential property of Microware
 *  Systems Corporation, and is provided to licensee solely for documentation
 *  and educational purposes. Reproduction, publication, or distribution in
 *  form to any party other than the licensee is strictly prohibited.
 *
 * EDITION  DATE      WHO  DETAIL
 *       1  01/06/95  srm  create
 */
#ifndef _MP_H
#define _MP_H

#include <MAUI/maui_msg.h>
#include <MAUI/maui_inp.h>
#include <types.h>
#include <const.h>

/* we hope these will be defined in maui_inp.h */
#define MSG_TYPE_MPCMD (1<<20)

#ifndef _MP_ID_STRUCTS_EXPOSED
typedef void* MP_PROC_ID;         /* Input process ID */
typedef void* MP_DEV_ID;          /* Input device ID */
#endif

typedef enum {
  /* mauip commands */
  CMD_INP_INIT,
  CMD_INP_INIT_REPLY,
  CMD_INP_TERM,
  CMD_INP_TERM_REPLY,
  CMD_OPEN_DEV,
  CMD_OPEN_DEV_REPLY,
  CMD_CLOSE_DEV,
  CMD_CLOSE_DEV_REPLY,
  CMD_SET_MSG_MASK,
  CMD_SET_MSG_MASK_REPLY,
  /* protocol module commands */
  CMD_CHECK_KEYS,
  CMD_CHECK_KEYS_REPLY,
  CMD_GET_DEV_CAP,
  CMD_GET_DEV_CAP_REPLY,
  CMD_GET_DEV_STATUS,
  CMD_GET_DEV_STATUS_REPLY,
  CMD_RELEASE_KEY,
  CMD_RELEASE_KEY_REPLY,
  CMD_RESERVE_KEY,
  CMD_RESERVE_KEY_REPLY,
  CMD_SET_MSG_CALLBACK,
  CMD_SET_MSG_CALLBACK_REPLY,
  CMD_SET_SIM_METH,
  CMD_SET_SIM_METH_REPLY,
  CMD_SET_PTR_LIMIT,
  CMD_SET_PTR_LIMIT_REPLY,
  CMD_SET_PTR_POS,
  CMD_SET_PTR_POS_REPLY,
  /* error reply */
  CMD_BADACK_REPLY,
  CMD_RESTACK_DEV,
  CMD_RESTACK_DEV_REPLY
} MP_DEV_CMD;


/* common section of all device messages */
typedef struct _MSG_COMMON_MPCMD {
  MSG_COMMON com;    /* common section of all messages */
  MP_DEV_CMD cmd;    /* command code from MP_DEV_CMD enum */
  MP_DEV_ID  dev_id; /* ID of device as returned by MAUI Process */
} MSG_COMMON_MPCMD;



/* REQUEST PKT */
typedef struct _MSG_INP_INIT {
  MSG_COMMON_MPCMD dcom;         /* device common section */
  u_int32          compat_level; /* compatibility level of Input API */
  process_id       pid;          /* process id of caller process */
} MSG_INP_INIT;
/* REPLY PKT */
typedef struct _MSG_INP_INIT_REPLY {
  MSG_COMMON_MPCMD dcom;         /* device common section */
  u_int32          compat_level; /* compatibility level of MAUI Process */
  MP_PROC_ID       mp_proc_id;   /* caller id as returned by MAUI Process */
  error_code       error;        /* return error code */
} MSG_INP_INIT_REPLY;


/* REQUEST PKT */
typedef struct _MSG_INP_TERM {
  MSG_COMMON_MPCMD dcom;       /* device common section */
  MP_PROC_ID       mp_proc_id; /* caller id as returned by MAUI Process */
} MSG_INP_TERM;
/* REPLY PKT */
typedef struct _MSG_INP_TERM_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_INP_TERM_REPLY;



/* REQUEST PKT */
typedef struct _MSG_OPEN_DEV {
  MSG_COMMON_MPCMD dcom;         /* device common section */
  u_int32          compat_level; /* compatibility level of Input API */
  MP_PROC_ID       mp_proc_id;   /* caller ID as returned by MAUI Process */
  char             mbox_name[MSG_MAX_MBOX_NAME];  /* name of mb to send devices messages to */
  char             device_name[INP_MAX_DEV_NAME]; /* name of device to open */
  INP_DEV_ID       device_id;    /* ID to return in PTR and KEY messages */
} MSG_OPEN_DEV;
/* REPLY PKT */
typedef struct _MSG_OPEN_DEV_REPLY {
  MSG_COMMON_MPCMD dcom;         /* device common section */
  u_int32          compat_level; /* compatibility level of MAUI Protocol Module */
  error_code       error;        /* return error code */
} MSG_OPEN_DEV_REPLY;


typedef struct _MSG_CLOSE_DEV {
  MSG_COMMON_MPCMD dcom; /* device common section */
} MSG_CLOSE_DEV;
/* REPLY PKT */
typedef struct _MSG_CLOSE_DEV_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_CLOSE_DEV_REPLY;



typedef struct _MSG_SET_MSG_MASK {
  MSG_COMMON_MPCMD dcom;       /* device common section */
  u_int32          write_mask; /* MSG write mask */
} MSG_SET_MSG_MASK;
typedef struct _MSG_SET_MSG_MASK_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_SET_MSG_MASK_REPLY;



typedef struct _MSG_GET_DEV_CAP {
  MSG_COMMON_MPCMD dcom; /* device common section */
} MSG_GET_DEV_CAP;
/* REPLY PKT */
typedef struct _MSG_GET_DEV_CAP_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  INP_DEV_CAP      cap;   /* device information */
  error_code       error; /* return error code */
} MSG_GET_DEV_CAP_REPLY;



typedef struct _MSG_GET_DEV_STATUS {
  MSG_COMMON_MPCMD dcom; /* device common section */
} MSG_GET_DEV_STATUS;
/* REPLY PKT */
typedef struct _MSG_GET_DEV_STATUS_REPLY {
  MSG_COMMON_MPCMD dcom;   /* device common section */
  INP_DEV_STATUS   status; /* device information */
  error_code       error;  /* return error code */
} MSG_GET_DEV_STATUS_REPLY;



typedef struct _MSG_SET_PTR_POS {
  MSG_COMMON_MPCMD dcom;     /* device common section */
  GFX_POINT        position; /* New position */
} MSG_SET_PTR_POS;
typedef struct _MSG_SET_PTR_POS_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_SET_PTR_POS_REPLY;


typedef struct _MSG_SET_SIM_METH {
  MSG_COMMON_MPCMD dcom;     /* device common section */
  INP_SIM_METH     sim_meth; /* simulation mode */
  GFX_DELTA        speed;    /* X/Y speed for simulation */
  wchar_t          button_map[INP_MAX_BUTTONS]; /* button to key mapping */
} MSG_SET_SIM_METH;
typedef struct _MSG_SET_SIM_METH_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_SET_SIM_METH_REPLY;


typedef struct _MSG_SET_PTR_LIMIT {
  MSG_COMMON_MPCMD dcom;    /* device common section */
  GFX_POINT        ptr_min; /* minimum position for the pointer */
  GFX_POINT        ptr_max; /* maximum position for the pointer */
} MSG_SET_PTR_LIMIT;
typedef struct _MSG_SET_PTR_LIMIT_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_SET_PTR_LIMIT_REPLY;


typedef struct _MSG_RELEASE_KEY {
  MSG_COMMON_MPCMD dcom;    /* device common section */
  wchar_t          key;     /* key symbol to reserve */
} MSG_RELEASE_KEY;
typedef struct _MSG_RELEASE_KEY_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_RELEASE_KEY_REPLY;

typedef struct _MSG_RESERVE_KEY {
  MSG_COMMON_MPCMD dcom;    /* device common section */
  wchar_t          key;     /* key symbol to reserve */
} MSG_RESERVE_KEY;
typedef struct _MSG_RESERVE_KEY_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_RESERVE_KEY_REPLY;


typedef struct _MSG_RESTACK_DEV {
  MSG_COMMON_MPCMD dcom;    /* device common section */
  INP_DEV_PLACEMENT placement; /* placement in stack of devices */
  MP_DEV_ID ref_dev_id;     /* reference device */
} MSG_RESTACK_DEV;
typedef struct _MSG_RESTACK_DEV_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_RESTACK_DEV_REPLY;


typedef struct _MSG_CHECK_KEYS {
  MSG_COMMON_MPCMD dcom;    /* device common section */
  wchar_t          min_key; /* first key symbol to reserve */
  wchar_t          max_key; /* last key symbol to reserve */
} MSG_CHECK_KEYS;
typedef struct _MSG_CHECK_KEYS_REPLY {
  MSG_COMMON_MPCMD dcom;    /* device common section */
  BOOLEAN          present; /* return TRUE if all present */
  error_code       error;   /* return error code */
} MSG_CHECK_KEYS_REPLY;



typedef struct _MSG_SET_MSG_CALLBACK {
  MSG_COMMON_MPCMD dcom;      /* device common section */
  void           (*callback)(const void *msg);
                              /* callback function pointer */
} MSG_SET_MSG_CALLBACK;
typedef struct _MSG_SET_MSG_CALLBACK_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_SET_MSG_CALLBACK_REPLY;



/* The Protocol Module returns this message if it does not know the
   command code */
typedef struct _MSG_BADACK_REPLY {
  MSG_COMMON_MPCMD dcom;  /* device common section */
  error_code       error; /* return error code */
} MSG_BADACK_REPLY;



typedef union _MP_DEV_MSG {
  /* mauip messages */
  MSG_INP_INIT               inp_init;
  MSG_INP_INIT_REPLY         inp_init_reply;
  MSG_INP_TERM               inp_term;
  MSG_INP_TERM_REPLY         inp_term_reply;
  MSG_OPEN_DEV               open_dev;
  MSG_OPEN_DEV_REPLY         open_dev_reply;
  MSG_CLOSE_DEV              close_dev;
  MSG_CLOSE_DEV_REPLY        close_dev_reply;
  MSG_SET_MSG_MASK           set_msg_mask;
  MSG_SET_MSG_MASK_REPLY     set_msg_mask_reply;
  /* protocol module messages */
  MSG_GET_DEV_CAP            get_dev_cap;
  MSG_GET_DEV_CAP_REPLY      get_dev_cap_reply;
  MSG_GET_DEV_STATUS         get_dev_status;
  MSG_GET_DEV_STATUS_REPLY   get_dev_status_reply;
  MSG_SET_PTR_POS            set_ptr_pos;
  MSG_SET_PTR_POS_REPLY      set_ptr_pos_reply;
  MSG_SET_SIM_METH           set_sim_meth;
  MSG_SET_SIM_METH_REPLY     set_sim_meth_reply;
  MSG_SET_PTR_LIMIT          set_ptr_limit;
  MSG_SET_PTR_LIMIT_REPLY    set_ptr_limit_reply;
  MSG_RESERVE_KEY            reserve_key;
  MSG_RESERVE_KEY_REPLY      reserve_key_reply;
  MSG_RELEASE_KEY            release_key;
  MSG_RELEASE_KEY_REPLY      release_key_reply;
  MSG_CHECK_KEYS             check_keys;
  MSG_CHECK_KEYS_REPLY       check_keys_reply;
  MSG_SET_MSG_CALLBACK       set_msg_callback;
  MSG_SET_MSG_CALLBACK_REPLY set_msg_callback_reply;
  /* error reply message */
  MSG_BADACK_REPLY           badack_reply;
  /* generic message */
  MSG_COMMON_MPCMD           any;
  MSG_RESTACK_DEV            restack_dev;
  MSG_RESTACK_DEV_REPLY      restack_dev_reply;
} MP_DEV_MSG;

#define MP_MBOX_NAME       "mp_mbox"

#endif /* _MP_H */
