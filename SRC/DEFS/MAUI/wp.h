/**
 ** wp.h - declarations shared between WIN API and maui_win process
 **
 ** This source code is the proprietary confidential property of Microware
 ** Systems Corporation, and is provided to licensee solely for documentation
 ** and educational purposes. Reproduction, publication, or distribution in
 ** form to any party other than the licensee is strictly prohibited.
 **
 ** 12JUL86 - Mike Schoonover
 **/

#ifndef _WP_H
#define _WP_H

#define _WIN_ID_STRUCTS_EXPOSED
#include <MAUI/maui_win.h>
#include <MAUI/sema4.h>
#include <alarm.h>


/****************************************************************************
 * WP PUBLIC CONSTANTS
 ****************************************************************************/
/* the prefix used to build the API's reply mailbox name: */
#define WP_REP_MBOX_PFX "wp"
/* error_code divisor constant: */
#ifdef _OS9000
#define WP_ERR_DIV      0x10000
#else
#define WP_ERR_DIV      0x100
#endif
/* maximum number of cursor colors & max cursor width & height */
#define WP_CURSOR_MAX_COLORS    2
#define WP_CURSOR_MAX_WIDTH     32
#define WP_CURSOR_MAX_HEIGHT    32
/* signal to send maui_win when cursor might need refreshing: */
#define WP_SIG_CURSOR_REFRESH   0x200
/* ink hit point & width/height: */
#define WP_INK_WIDTH    3
#define WP_INK_HEIGHT   3
#define WP_INK_HIT_X    1
#define WP_INK_HIT_Y    1
#define WP_INK_BUTTON (1<<0)    /* button 1 triggers inking */


/****************************************************************************
 * WP INTERNAL ID TYPES
 ****************************************************************************/
#ifdef _WP_ID_STRUCTS_EXPOSED
  typedef struct _WP_PROC WP_PROC;
  #define WP_PROC_ID WP_PROC *
#else
  typedef void * WP_PROC_ID;
#endif



/****************************************************************************
 * GENERAL MACROS
 ****************************************************************************/
/* make a sync_code from four characters: */
#ifndef WP_SYNC
#define WP_SYNC(a,b,c,d) (((a&0xff)<<24)|((b&0xff)<<16)|((c&0xff)<<8)|(d&0xff))
#endif
#define WP_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))
/* return maximum of two values: */
#define WP_MAX(a,b) ((a) > (b) ? (a) : (b))


/****************************************************************************
 * RECTANGLES
 ****************************************************************************/
typedef struct _WP_REC WP_REC;
struct _WP_REC {
  GFX_POS x0, y0;               /* upper-left of rectangle */
  GFX_POS x1, y1;               /* lower-right corner of rectangle */
};

typedef struct _WP_REC_NODE WP_REC_NODE;
struct _WP_REC_NODE {
  WP_REC        rec;
  WP_REC_NODE * next;
  WP_REC_NODE * prev;
};

typedef struct _WP_REC_LIST WP_REC_LIST;
struct _WP_REC_LIST {
  u_int32       len;
  WP_REC_NODE * head;
  WP_REC_NODE * tail;
};

/****************************************************************************
 * MESSAGE QUEUE
 ****************************************************************************/

/*
   MSGQ is a queue of WIN_MSG messages.  The routine for manipulating this
   queue are found in msgq.c.
   */
typedef struct _MSGQ_NODE MSGQ_NODE;
typedef struct _MSGQ {
  u_int32       len;            /* number of entries in queue */
  MSGQ_NODE *   head;           /* remove entries from this end */
  MSGQ_NODE *   tail;           /* add entries at this end */
} MSGQ;

/****************************************************************************
 * CURSOR LIST NODES
 ****************************************************************************/

typedef struct _CUR_NODE CUR_NODE;

/****************************************************************************
 * INPDEV LIST NODES
 ****************************************************************************/

typedef struct _INP_NODE INP_NODE;

/****************************************************************************
 * WINDOW DEVICE
 ****************************************************************************/

/*
   Structure shared between maui_win and WIN API that defines information
   that is useful to both.  Currently, there is only one instance of this
   structure per maui_win/windev.  It gets allocated in maui_win when
   executing the function WP_FUNC_CREATEDEV.  Its memory gets permitted to
   the API by WP_FUNC_CREATEDEV and WP_FUNC_OPENDEV.
   */
struct _WIN_DEV {
  u_int32               sync_code;      /* must be _WIN_DEV_SYNC */
  sema4                 lock;           /* to control access to windev */
  WP_PROC_ID            owner_proc;     /* id of proc that created dev */
  process_id            owner_pid;      /* process id of device owner */
  WIN *                 root_win;       /* root window of this device */
  u_int32               gfxshade;       /* shade used for dmap allocation */
  GFX_DEV_ID            gfxdev;         /* graphics dev created by maui_win */
  GFX_VPORT_ID          vport;          /* viewport in which dmap displayed */
  GFX_DMAP              dmap;           /* drawmap created by maui_win */
  alarm_id              cur_refresh_alrm; /* cyclic alarm for cursor refresh */

  WIN *                 focus_win;      /* window that currently has focus */
  WIN *                 ptr_win;        /* window that has pointer focus */
  GFX_POINT             ptr_pos;        /* latest position of cursor on root */
  WIN_CMAP *            cmap;           /* currently installed colormap */
  WIN_CURSOR *          cursor;         /* cursor currently being used */
  BOOLEAN               cursor_on;      /* cursor is currently on if TRUE */
  u_int8                grab_buttons;   /* which button is causing grab */

  GFX_POINT             ink_last_pos;   /* last position ptr at for ink */
  GFX_POINT             ink_new_pos;    /* new position ptr at for ink */
  BLT_CONTEXT_ID        ink_bltctx;     /* blit context for inking, and also
					** win_copy_block operations */
  WP_REC                ink_rec;        /* rectangle in which inking occurs */

  BLT_CONTEXT_ID        cur_show_bltctx; /* for showing cursor */
  BLT_CONTEXT_ID        cur_save_bltctx; /* for save-under */
  BLT_CONTEXT_ID        cur_rest_bltctx; /* for restore */
  GFX_DMAP              cur_save_dmap;  /* drawmap used to store save-under */
  GFX_POS               curx;           /* cursor upper-left corner X */
  GFX_POS               cury;           /* cursor upper-left corner Y */
  GFX_DIMEN             curw;           /* cursor width */
  GFX_DIMEN             curh;           /* cursor height */
  GFX_POS               curx_offset;    /* cursor blit offset X */
  GFX_POS               cury_offset;    /* cursor blit offset Y */

  WIN *                 lock_win_head;  /* first in lock region window list */
  WIN *                 lock_win_tail;  /* last in lock region window list */

  WIN_CMAP *            cmap_head;      /* head of colormap list */
  WIN_CMAP *            cmap_tail;      /* tail of colormap list */

  CUR_NODE *            cur_head;       /* head of cursor node list */
  CUR_NODE *            cur_tail;       /* tail of cursor node list */

  INP_NODE *            inp_head;       /* head of input list */
  INP_NODE *            inp_tail;       /* tail of input list */

  MSGQ			msgq;		/* queue of undelivered messages */

  GFX_DMAP work_dmap;         /* Offscreen drawmap--not used after 2.1.3,
							  ** but left in for compatibility (used in
							  ** WIN API 2.1.3) */
  BOOLEAN cursor_state;       /* TRUE if cursor is on */
  u_int32		flags;		/* general purpose flags:
					** 0x1 - Hardware Cursor
					** 0x2 - Hardware Cursor Mask is 1 bit
					*/
};

#define _WP_HW_CURSOR             0x01
#define _WP_HW_CURSOR_MASK_1BIT   0x02


#define _WIN_DEV_SYNC   WP_SYNC('W','D','E','V')
#define WP_GOOD_DEV_ID(p) ((p) != NULL && (p)->sync_code == _WIN_DEV_SYNC)
#define WP_BAD_DEV_ID(p)  ((p) == NULL || (p)->sync_code != _WIN_DEV_SYNC)


/****************************************************************************
 * WINDOWS
 ****************************************************************************/

/*
   Structure that maui_win uses to keep track of a window associated with a
   windowing device.
   */
struct _WIN {

  /* The following fields are do not currently have any use outside of the
     windowing API and maui_win: */

  u_int32        sync_code;             /* must be _WIN_SYNC */
  WP_PROC_ID     owner_proc;            /* maui_win info on window's owner */
  process_id     owner_pid;             /* process_id of creator of window */
  u_int32        notused1;              /* Reserved */

  /* The following fields mirror those provided in the WIN_STATUS structure. */

  WIN_DEV *      windev;                /* windowing device ID */
  WIN *          parent;                /* parent window */
  WIN *          child_front;           /* child in front of all others */
  WIN *          child_back;            /* child in back of all others */
  WIN *          sib_front_of;          /* sibling in front of window */
  WIN *          sib_back_of;           /* sibling in back of window */
  BOOLEAN        state;                 /* window state */
  GFX_POINT      root_pos;              /* window position in root */
  GFX_POINT      position;              /* window position in parent */
  GFX_DIMEN      width;                 /* window width */
  GFX_DIMEN      height;                /* window height */
  u_int32        cursor_id;             /* cursor id */
  WIN_INK_METHOD ink_method;            /* inking method */
  GFX_PIXEL      ink_pixel;             /* inking pixel value */
  DRW_CONTEXT_ID drwctx;                /* drawing context */
  TXT_CONTEXT_ID txtctx;                /* text context */
  WIN_CMAP_ID    cmap;                  /* colormap ID (NULL-> use parent's) */
  WIN_MSG_MASK   msg_mask;              /* message mask */
  WIN_CALLBACK   callback;              /* user callback */
  void *         user_data;             /* user data for callback */

  /* This field represents area of window that is clipped to parent. */

  GFX_RECT       clip_rect;             /* area that is clipped to parent */

  /* The following fields are used to set the drawing area. */

  GFX_POINT      drw_area_pos;          /* upper-left corner of draw area */
  GFX_DIMEN      drw_area_width;        /* width of draw area */
  GFX_DIMEN      drw_area_height;       /* height of draw area */

  /* The following fields are used to optimize the lookup of fields
     that default to an ancestor's value when set to NULL. */

  WIN_CMAP_ID    abs_cmap;              /* == cmap or first ancestor's set */
  WIN_CURSOR *   abs_cursor;            /* == cursor or first ancestor's set */

  /* The following fields are useful for cursor drawing within mui_win and
     by win_lock_region() and win_unlock_region(): */

  BOOLEAN        is_visible;            /* TRUE if any portion is visible */
  WP_REC_LIST    vis_areas;             /* list of exposed areas in window */

  WIN *          vis_child_front;       /* frontmost visible child */
  WIN *          vis_child_back;        /* backmost visible child */
  WIN *          vis_sib_front_of;      /* visible sibling in front */
  WIN *          vis_sib_back_of;       /* visible sibling in back */
  BOOLEAN        clip_changed;          /* clipping changed since last lock */
  BOOLEAN        pos_changed;           /* position changed since last lock */

  BOOLEAN        new_is_visible;        /* TRUE -> new visibility flag */
  WP_REC_LIST    new_vis_areas;         /* newly computed visible areas */

  BOOLEAN        reg_locked;            /* lock_region is in place */
  WP_REC         lock_region;           /* region locked from cursor drawing */
  WIN *          lock_win_next;         /* next window in lock region list */
  WIN *          lock_win_prev;         /* prev window in lock region list */

  BOOLEAN        do_unlock;             /* TRUE=>API calls unlock cmd,
                                           to release exposures */
  BOOLEAN        destroyed;             /* indicates that window has been
                                           destroyed, so its WIN structure
                                           can be disposed after sending
                                           expose messages from the window */
  MSGQ           delayed_msgs;          /* queue of msgs delayed til unlock */

  WIN *          nextFree;              /* WIN free list management */

  WP_REC         base_clip_rec;         /* base clip rectangle */
};


#define _WIN_SYNC         WP_SYNC('M','W','I','N')
#define WP_GOOD_WIN_ID(p) ((p) != NULL && (p)->sync_code == _WIN_SYNC)
#define WP_BAD_WIN_ID(p)  ((p) == NULL || (p)->sync_code != _WIN_SYNC)


/****************************************************************************
 * COLORMAP
 ****************************************************************************/

/*
   For each cell in the colormap, a WIN_CELL structure defines whether
   the cell is allocated, who allocated it, and whether it is shareable.
   */
typedef struct _WIN_CELL {
  BOOLEAN       allocated;              /* TRUE => cell has been allocated  */
  BOOLEAN       shareable;              /* TRUE=>read-only; FALSE=>read/write*/
  WP_PROC_ID    owner;                  /* proc that allocated private cell */
  u_int32		link_count;				/* Number of users of this cell */
} WIN_CELL;
/*
   WIN_CMAP is the structure that maui_win uses to keep track of colormap
   information.  The data contained in the colormap depends on the coding
   method of the device for which the colormap is created, as well as the
   color type.
   */
struct _WIN_CMAP {
  u_int32               sync_code;      /* _WIN_CMAP_SYNC */
  WP_PROC_ID            owner_proc;     /* who created the cmap */
  process_id            owner_pid;      /* pid of owner of device */
  WIN_DEV *             windev;         /* window device associated with */
  GFX_COLOR_TYPE        ctype;          /* type of colors specified */
  u_int16               num_colors;     /* number of cells in colormap */
  GFX_PALETTE *         palette;        /* for CLUT-based coding methods */
  WIN_CELL *            cells;          /* info about each cell */

  WIN_CMAP *            next;           /* next cmap in list toward tail */
  WIN_CMAP *            prev;           /* next cmap in list toward head */

  WIN_CMATCH     colormatch_method; /* Current colormatchig method*/
};


#define _WIN_CMAP_SYNC          WP_SYNC('C','M','A','P')
#define WP_GOOD_CMAP_ID(p)  ((p) != NULL && (p)->sync_code == _WIN_CMAP_SYNC)
#define WP_BAD_CMAP_ID(p)   ((p) == NULL || (p)->sync_code != _WIN_CMAP_SYNC)


/****************************************************************************
 * RPC MESSAGE INFRASTRUCTURE
 ****************************************************************************/

/*
   The constant WP_MSG_TYPE_CMD defines the message type of WP_CMD_* messages
   written to the maui_win command mailbox.  This value is placed in the
   wcom.com.type field of all WP_CMD_* messages.  Similarly, the
   WP_MSG_TYPE_REP constant defines the message type of WP_REP_* messages that
   are written to the API's reply mailbox.
   */
#define WP_MSG_TYPE_CMD  (1<<24)
#define WP_MSG_TYPE_REP  (1<<25)


/*
   The WP_FUNC type defines all valid values that can be assigned to the
   .func field of RPC messages (WP_CMD_* & WP_REP_*).  For each WIN_FUNC enum
   value, there is an associated WP_FUNC_<type> and WP_REP_<type> message
   structure that defines the command and reply parameters.  For instance,
   for WP_FUNC_CREATEWIN, there are WP_CMD_CREATEWIN & WP_REP_CREATEWIN
   structures.  Once exceptiont is WP_FUNC_BADACK_REPLY, which is sent from
   the maui_win process as a reply message .func if the requested function
   is not defined by maui_win.
   */
typedef enum {
  WP_FUNC_FORK,

  WP_FUNC_CREATEDEV,
  WP_FUNC_DESTROYDEV,
  WP_FUNC_OPENDEV,
  WP_FUNC_CLOSEDEV,
  WP_FUNC_OPENINPDEV,
  WP_FUNC_CLOSEINPDEV,
  WP_FUNC_SETFOCUS,

  WP_FUNC_CREATEWIN,
  WP_FUNC_DESTROYWIN,
  WP_FUNC_GETWINSTATUS,
  WP_FUNC_UNLOCKREGION,

  WP_FUNC_SETSTATE,
  WP_FUNC_MOVEWIN,
  WP_FUNC_RESIZEWIN,
  WP_FUNC_RESTACKWIN,
  WP_FUNC_REPARENTWIN,

  WP_FUNC_SETCALLBACK,
  WP_FUNC_SETMSGMASK,
  WP_FUNC_SETCMAP,
  WP_FUNC_SETCURSOR,

  WP_FUNC_SETINKMETHOD,
  WP_FUNC_SETINKPIX,
  WP_FUNC_ERASEINK,

  WP_FUNC_CREATECMAP,
  WP_FUNC_DESTROYCMAP,
  WP_FUNC_ALLOCCMAPCOLOR,
  WP_FUNC_ALLOCCMAPCELLS,
  WP_FUNC_FREECMAPCELLS,
  WP_FUNC_SETCMAPCELLS,
  WP_FUNC_GETCMAPCELLS,
  WP_FUNC_GETCMAPFREE,

  WP_FUNC_CREATECURSOR,
  WP_FUNC_DESTROYCURSOR,

  WP_FUNC_GRABPTR,
  WP_FUNC_UNGRABPTR,

  WP_FUNC_COPYBLOCK,
  WP_FUNC_RESTACKDEV,

  WP_FUNC_SETCURPOS,
  WP_FUNC_SETCURSTATE,

  WP_FUNC_ALLOCCMAPCOLORS,

  WP_FUNC_BADACK_REPLY	= 9999
} WP_FUNC;


/*
   The WP_CMD_COMMON type defines a structure that is the common header of
   all command messages.  The basic structure of a maui_win command message is:

   typedef struct _WP_MSG_<function> {
     MSG_COMMON         com;            // common MAUI message part
     WP_FUNC            func;           // function to be performed
     WP_PROC_ID         proc;           // WP process identification
     <command-specific parameters>
   } WP_CMD_<function>;
   */

typedef struct _WP_CMD_COMMON {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* function to perform */
  WP_PROC_ID            proc;           /* WP process identification */
} WP_CMD_COMMON;

/*
   The WP_REP_COMMON type defines a structure that is the common header of
   all reply messages.  The basic structure of a maui_win reply message is:

   typedef struct _WP_REP_<function> {
     MSG_COMMON         com;            // common MAUI message part
     WP_FUNC            func;           // function that was performed
     error_code         err;            // error code (if any) that resulted
     <command-specific reply fields>
   } WP_REP_<function>;
   */

typedef struct _WP_REP_COMMON {
  MSG_COMMON          com;      /* common MAUI message part */
  WP_FUNC             func;     /* function that reply is being sent for */
  error_code          err;      /* error code returned for the function */
} WP_REP_COMMON;



/****************************************************************************
 * RPC COMMAND and REPLY TYPES
 ****************************************************************************/

/*
   WP_REP_BADACK_REPLY is sent as a reply from maui_win if it does not
   recognize the WP_CMD_MSG message's command code wcom.func =
   WP_FUNC_BADACK_REPLY.
   */
typedef struct _WP_REP_BADACK {
  MSG_COMMON          com;      /* common MAUI message part */
  WP_FUNC             func;     /* function that reply is being sent for */
  error_code          err;      /* error code returned for the function */
} WP_REP_BADACK;


/*
   The WP_REP_FORK message is unique in that it has no WP_CMD_ counterpart.
   This is because this message is actually sent in response to maui_win
   being forked.  The return <err> indicates the status initialization that
   occurred in maui_win as a result of being forked.
   */
typedef struct _WP_CMD_FORK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_FORK */
  WP_PROC_ID            proc;           /* should be NULL for this func */
} WP_CMD_FORK;

typedef struct _WP_REP_FORK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_FORK */
  error_code            err;            /* error code returned by operation */
} WP_REP_FORK;


/*
   All of the remaining message types have both a command and reply message.
   */
typedef struct _WP_CMD_CREATEDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CREATEDEV */
  WP_PROC_ID            proc;           /* should be NULL for this func */

  char                  gfxdev_name[GFX_MAX_DEV_NAME]; /* gfx device name */
  u_int8                res_idx;        /* resolution index */
  u_int8                cm_idx;         /* coding method index */
  char                  app_mbox_name[MSG_MAX_MBOX_NAME];  /* app mbox name */
  u_int32               compat_level;   /* API's compatibility level */
  void (*api_callback)(const void *msg); /* ptr to API's internal callback */
} WP_CMD_CREATEDEV;

typedef struct _WP_REP_CREATEDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CREATEDEV */
  error_code            err;            /* error code returned by function */

  WP_PROC_ID            proc;           /* WP process identification info */
  WIN_DEV_ID            windev;         /* id of created device */
  u_int32               compat_level;   /* maui_win's compatibility level */
} WP_REP_CREATEDEV;


typedef struct _WP_CMD_DESTROYDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYDEV */
  WP_PROC_ID            proc;           /* calling process */
} WP_CMD_DESTROYDEV;

typedef struct _WP_REP_DESTROYDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYDEV */
  error_code            err;            /* error code returned */
} WP_REP_DESTROYDEV;


typedef struct _WP_CMD_OPENDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_OPENDEV */
  WP_PROC_ID            proc;           /* should be NULL for this func */

  char                  app_mbox_name[MSG_MAX_MBOX_NAME]; /* app mbox name*/
  u_int32               compat_level;   /* API's compatibility level */
  void (*api_callback)(const void *msg); /* ptr to API's internal callback */
} WP_CMD_OPENDEV;

typedef struct _WP_REP_OPENDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_OPENDEV */
  error_code            err;            /* error code returned by function */

  WP_PROC_ID            proc;           /* WP process identification info */
  WIN_DEV_ID            windev;         /* WIN_DEV_ID returned if successful */
  u_int32               compat_level;   /* maui_win's compatibility level */
} WP_REP_OPENDEV;


typedef struct _WP_CMD_CLOSEDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CLOSEDEV */
  WP_PROC_ID            proc;           /* WP process identifier */
} WP_CMD_CLOSEDEV;

typedef struct _WP_REP_CLOSEDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CLOSEDEV */
  error_code            err;            /* error code returned by function */
} WP_REP_CLOSEDEV;


typedef struct _WP_CMD_OPENINPDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_OPENINPDEV */
  WP_PROC_ID            proc;           /* WP process identifier */

  char                  inpdev_name[INP_MAX_DEV_NAME];  /* name of inp dev */
} WP_CMD_OPENINPDEV;

typedef struct _WP_REP_OPENINPDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_OPENINPDEV */
  error_code            err;            /* error code returned by function */

  INP_DEV_ID            inpdev;         /* actual inp dev ID */
} WP_REP_OPENINPDEV;


typedef struct _WP_CMD_CLOSEINPDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CLOSEINPDEV */
  WP_PROC_ID            proc;           /* WP process identifier */

  INP_DEV_ID            inpdev;         /* ID of input device to close */
} WP_CMD_CLOSEINPDEV;

typedef struct _WP_REP_CLOSEINPDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CLOSEINPDEV */
  error_code            err;            /* error code returned by function */

} WP_REP_CLOSEINPDEV;


typedef struct _WP_CMD_RESTACKDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_RESTACKDEV */
  WP_PROC_ID            proc;           /* calling process */

  WIN_DEV_ID            windev;         /* win device to be restacked */
  WIN_DEV_PLACEMENT     placement;      /* Must be only WIN_DEV_FRONT or WIN_DEV_BACK */
  
} WP_CMD_RESTACKDEV;

typedef struct _WP_REP_RESTACKDEV {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_RESTACKDEV */
  error_code            err;            /* error code returned by function */
} WP_REP_RESTACKDEV;



typedef struct _WP_CMD_SETCURSTATE {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCURSTATE */
  WP_PROC_ID            proc;           /* calling process */
  WIN_DEV *             windev;         /* win device to be restacked */
  BOOLEAN               active;         /* TRUE if cursor to be visible */
  
} WP_CMD_SETCURSTATE;

typedef struct _WP_REP_SETCURSTATE {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCURSTATE */
  error_code            err;            /* error code returned by function */
} WP_REP_SETCURSTATE;





typedef struct _WP_CMD_SETFOCUS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETFOCUS */
  WP_PROC_ID            proc;           /* WP process identifier */

  WIN_ID                win;            /* new focus window */
} WP_CMD_SETFOCUS;

typedef struct _WP_REP_SETFOCUS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETFOCUS */
  error_code            err;            /* error code returned by function */
} WP_REP_SETFOCUS;


typedef struct _WP_CMD_GRABPTR {
  MSG_COMMON		com;		/* common MAUI message part */
  WP_FUNC		func;		/* WP_FUNC_GRABPTR */
  WP_PROC_ID		proc;		/* WP process identifier */

  WIN_ID		win;		/* window to grab ptr focus */
} WP_CMD_GRABPTR;

typedef struct _WP_REP_GRABPTR {
  MSG_COMMON		com;		/* common MAUI message part */
  WP_FUNC		func;		/* WP_FUNC_GRABPTR */
  error_code		err;		/* error code returned by function */
} WP_REP_GRABPTR;


typedef struct _WP_CMD_UNGRABPTR {
  MSG_COMMON		com;		/* common MAUI message part */
  WP_FUNC		func;		/* WP_FUNC_UNGRABPTR */
  WP_PROC_ID		proc;		/* WP process identifier */

  WIN_ID		win;		/* window to ungrab ptr focus */
} WP_CMD_UNGRABPTR;

typedef struct _WP_REP_UNGRABPTR {
  MSG_COMMON		com;		/* common MAUI message part */
  WP_FUNC		func;		/* WP_FUNC_UNGRABPTR */
  error_code		err;		/* error code returned by function */
} WP_REP_UNGRABPTR;


typedef struct _WP_CMD_CREATEWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CREATEWIN */
  WP_PROC_ID            proc;           /* WP process identifier */

  WIN_ID                parent;         /* parent window id */
  GFX_POS               x;              /* window X position */
  GFX_POS               y;              /* window Y position */
  GFX_DIMEN             width;          /* window width */
  GFX_DIMEN             height;         /* window height */
  u_int32               msg_mask;       /* initial message mask */
  WIN_PLACEMENT         place;          /* window stack placement */
  WIN_ID                ref_win;        /* id of stack reference window */
} WP_CMD_CREATEWIN;

typedef struct _WP_REP_CREATEWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CREATEWIN */
  error_code            err;            /* error code returned by function */

  WIN_ID                win;            /* id of newly created window */
} WP_REP_CREATEWIN;


typedef struct _WP_CMD_DESTROYWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYWIN */
  WP_PROC_ID            proc;           /* WP process identifier */

  WIN_ID                win;            /* id of window to destroy */
} WP_CMD_DESTROYWIN;

typedef struct _WP_REP_DESTROYWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYWIN */
  error_code            err;            /* error code returned by function */

} WP_REP_DESTROYWIN;


typedef struct _WP_CMD_GETWINSTATUS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_GETWINSTATUS */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to get info about */
} WP_CMD_GETWINSTATUS;

typedef struct _WP_REP_GETWINSTATUS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_GETWINSTATUS */
  error_code            err;            /* error code returned */

  WIN_STATUS            stat;           /* returned status info about window */
} WP_REP_GETWINSTATUS;


typedef struct _WP_CMD_UNLOCKREGION {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_UNLOCKREGION */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to perform unlock on */
} WP_CMD_UNLOCKREGION;

typedef struct _WP_REP_UNLOCKREGION {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_UNLOCKREGION */
  error_code            err;            /* error code returned */
} WP_REP_UNLOCKREGION;


typedef struct _WP_CMD_MOVEWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_MOVEWIN */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to move */
  GFX_POS               x;              /* new X position relative to parent */
  GFX_POS               y;              /* new Y position relative to parent */
} WP_CMD_MOVEWIN;

typedef struct _WP_REP_MOVEWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_MOVEWIN */
  error_code            err;            /* error code returned */
} WP_REP_MOVEWIN;


typedef struct _WP_CMD_RESIZEWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_RESIZEWIN */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to resize */
  GFX_DIMEN             width;          /* new width */
  GFX_DIMEN             height;         /* new height */
} WP_CMD_RESIZEWIN;

typedef struct _WP_REP_RESIZEWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_RESIZEWIN */
  error_code            err;            /* error code returned */
} WP_REP_RESIZEWIN;


typedef struct _WP_CMD_RESTACKWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_RESTACKWIN */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to restack */
  WIN_PLACEMENT         place;          /* window placement in stack */
  WIN_ID                ref_win;        /* reference for WIN_(BACK|FRONT)_OF */
} WP_CMD_RESTACKWIN;

typedef struct _WP_REP_RESTACKWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_RESTACKWIN */
  error_code            err;            /* error code returned */
} WP_REP_RESTACKWIN;


typedef struct _WP_CMD_REPARENTWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_REPARENTWIN */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to reparent */
  WIN_ID                parent;         /* new parent for window */
  GFX_POS               x;              /* X position within new parent */
  GFX_POS               y;              /* Y position within new parent */
  WIN_PLACEMENT         place;          /* placement w/in new sibling stack */
  WIN_ID                ref_win;        /* for WIN_(BACK|FRONT)_OF */
} WP_CMD_REPARENTWIN;

typedef struct _WP_REP_REPARENTWIN {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_REPARENTWIN */
  error_code            err;            /* error code returned */
} WP_REP_REPARENTWIN;


typedef struct _WP_CMD_SETCALLBACK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCALLBACK */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to set callback for */
  WIN_CALLBACK          callback;       /* callback for window */
  void *                user_data;      /* user data passed to callback */
} WP_CMD_SETCALLBACK;

typedef struct _WP_REP_SETCALLBACK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCALLBACK */
  error_code            err;            /* error code returned */
} WP_REP_SETCALLBACK;


typedef struct _WP_CMD_SETCMAP {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCMAP */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to set colormap for */
  WIN_CMAP_ID           cmap;           /* colormap to use in this window */
} WP_CMD_SETCMAP;

typedef struct _WP_REP_SETCMAP {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCMAP */
  error_code            err;            /* error code returned */
} WP_REP_SETCMAP;
  

typedef struct _WP_CMD_SETCURSOR {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCURSOR */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to set cursor for */
  u_int32               cursor_id;      /* cursor to use in window */
} WP_CMD_SETCURSOR;

typedef struct _WP_REP_SETCURSOR {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCURSOR */
  error_code            err;            /* error code returned */
} WP_REP_SETCURSOR;


typedef struct _WP_CMD_SETINKMETHOD {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETINKMETHOD */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to set inking method for */
  WIN_INK_METHOD        ink_method;     /* inking method for this window */
} WP_CMD_SETINKMETHOD;

typedef struct _WP_REP_SETINKMETHOD {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETINKMETHOD */
  error_code            err;            /* error code returned */
} WP_REP_SETINKMETHOD;


typedef struct _WP_CMD_SETINKPIX {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETINKPIX */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to set inking pixel for */
  GFX_PIXEL             pix;            /* pixel to use for inking */
} WP_CMD_SETINKPIX;

typedef struct _WP_REP_SETINKPIX {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETINKPIX */
  error_code            err;            /* error code returned */
} WP_REP_SETINKPIX;


typedef struct _WP_CMD_ERASEINK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_ERASEINK */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window in which to erase ink */
} WP_CMD_ERASEINK;

typedef struct _WP_REP_ERASEINK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_ERASEINK */
  error_code            err;            /* error code returned */
} WP_REP_ERASEINK;


typedef struct _WP_CMD_SETMSGMASK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETMSGMASK */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to set message mask for */
  WIN_MSG_MASK          mask;           /* message mask for the window */
} WP_CMD_SETMSGMASK;

typedef struct _WP_REP_SETMSGMASK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETMSGMASK */
  error_code            err;            /* error code returned */
} WP_REP_SETMSGMASK;


typedef struct _WP_CMD_SETSTATE {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETSTATE */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                win;            /* window to change state for */
  BOOLEAN               active;         /* window's new active state */
} WP_CMD_SETSTATE;

typedef struct _WP_REP_SETSTATE {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETSTATE */
  error_code            err;            /* error code returned */
} WP_REP_SETSTATE;




typedef struct _WP_CMD_COPYBLOCK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_COPYBLOCK */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_ID                dst_win;        /* window to copy to */
  WIN_ID                src_win;        /* window to copy from */
  GFX_POS               dx;             /* destination X  relative to dst window */
  GFX_POS               dy;             /* destination Y  relative to dst window */
  GFX_POS               sx;             /* source X  relative to src window */
  GFX_POS               sy;             /* source Y  relative to src window */
  GFX_DIMEN             width;          /* block width */
  GFX_DIMEN             height;         /* block height*/
} WP_CMD_COPYBLOCK;

typedef struct _WP_REP_COPYBLOCK {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_COPYBLOCK */
  error_code            err;            /* error code returned */
} WP_REP_COPYBLOCK;


typedef struct _WP_CMD_CREATECMAP {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CREATECMAP */
  WP_PROC_ID            proc;           /* process requesting info */

  GFX_COLOR_TYPE        ctype;          /* type of colors to allocate */
} WP_CMD_CREATECMAP;

typedef struct _WP_REP_CREATECMAP {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CREATECMAP */
  error_code            err;            /* error code returned */

  WIN_CMAP_ID           cmap;           /* returned colormap ID */
} WP_REP_CREATECMAP;


typedef struct _WP_CMD_GETCMAPCELLS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_GETCMAPCELLS */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_CMAP_ID           cmap;           /* cmap to get cell color values for */
  GFX_COLOR_VALUE *     ret_colors;     /* array to fill with color values */
  u_int16               num_colors;     /* number of colors to get */
  GFX_PIXEL *           pixels;         /* pixels to get color values for */
} WP_CMD_GETCMAPCELLS;

typedef struct _WP_REP_GETCMAPCELLS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_GETCMAPCELLS */
  error_code            err;            /* error code returned */
} WP_REP_GETCMAPCELLS;


typedef struct _WP_CMD_GETCMAPFREE {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_GETCMAPFREE */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_CMAP_ID           cmap;           /* cmap to get free list info for */
} WP_CMD_GETCMAPFREE;

typedef struct _WP_REP_GETCMAPFREE {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_GETCMAPFREE */
  error_code            err;            /* error code returned */

  u_int16               ret_free_cells; /* total number of free cells */
  u_int16               ret_largest;    /* largest free block  */
  u_int16               ret_num_blocks; /* number of free contig blocks */
} WP_REP_GETCMAPFREE;


typedef struct _WP_CMD_DESTROYCMAP {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYCMAP */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_CMAP_ID           cmap;           /* colormap to destroy */
} WP_CMD_DESTROYCMAP;

typedef struct _WP_REP_DESTROYCMAP {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYCMAP */
  error_code            err;            /* error code returned */
} WP_REP_DESTROYCMAP;


typedef struct _WP_CMD_ALLOCCMAPCOLOR {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_ALLOCCMAPCOLOR */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_CMAP_ID           cmap;           /* colormap to allocate color from */
  GFX_COLOR_VALUE       color;          /* color to allocate */
} WP_CMD_ALLOCCMAPCOLOR;

typedef struct _WP_REP_ALLOCCMAPCOLOR {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_ALLOCCMAPCOLOR */
  error_code            err;            /* error code returned */

  GFX_PIXEL             pix;            /* pixel value for allocated color */
} WP_REP_ALLOCCMAPCOLOR;

typedef struct _WP_CMD_ALLOCCMAPCOLORS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_ALLOCCMAPCOLOR */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_CMAP_ID           cmap;           /* colormap to allocate color from */
  u_int32               num_cells;      /* number of colors/cells to alloc */
  const GFX_COLOR_VALUE       * color;          /* colors to allocate */
  GFX_PIXEL *           pixels;         /* pixel array that gets set */
} WP_CMD_ALLOCCMAPCOLORS;

typedef struct _WP_REP_ALLOCCMAPCOLORS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_ALLOCCMAPCOLOR */
  error_code            err;            /* error code returned */

} WP_REP_ALLOCCMAPCOLORS;


typedef struct _WP_CMD_ALLOCCMAPCELLS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_ALLOCCMAPCELLS */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_CMAP_ID           cmap;           /* colormap to allocate cells from */
  u_int32               num_cells;      /* number of colors/cells to alloc */
  BOOLEAN               contig;         /* TRUE => cells must be contiguous */
  const GFX_COLOR_VALUE * colors;       /* color values to put in cells */
  GFX_PIXEL *           pixels;         /* pixel array that gets set */
} WP_CMD_ALLOCCMAPCELLS;

typedef struct _WP_REP_ALLOCCMAPCELLS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_ALLOCCMAPCELLS */
  error_code            err;            /* error code returned */
} WP_REP_ALLOCCMAPCELLS;


typedef struct _WP_CMD_FREECMAPCELLS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_FREECMAPCELLS */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_CMAP_ID           cmap;           /* colormap to free cells in */
  GFX_PIXEL             start_pix;      /* starting pixel (cell idx) to free */
  u_int16               num_colors;     /* number of cells to free */
} WP_CMD_FREECMAPCELLS;

typedef struct _WP_REP_FREECMAPCELLS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_FREECMAPCELLS */
  error_code            err;            /* error code returned */
} WP_REP_FREECMAPCELLS;


typedef struct _WP_CMD_SETCMAPCELLS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCMAPCELLS */
  WP_PROC_ID            proc;           /* process requesting info */

  WIN_CMAP_ID           cmap;           /* cmap to set colors */
  const GFX_PIXEL *     pixels;         /* pixels to set colors for */
  u_int16               num_colors;     /* number of colors to set */
  const GFX_COLOR_VALUE * colors;       /* color values to set to */
} WP_CMD_SETCMAPCELLS;

typedef struct _WP_REP_SETCMAPCELLS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_SETCMAPCELLS */
  error_code            err;            /* error code returned */
} WP_REP_SETCMAPCELLS;


typedef struct _WP_CMD_CREATECURSOR {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CREATECURSOR */
  WP_PROC_ID            proc;           /* process requesting info */

  u_int32               cursor_id;      /* id of cursor to create */
  WIN_CURSOR *          cursor;         /* cursor definition */
} WP_CMD_CREATECURSOR;

typedef struct _WP_REP_CREATECURSOR {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_CREATECURSOR */
  error_code            err;            /* error code returned */
} WP_REP_CREATECURSOR;


typedef struct _WP_CMD_DESTROYCURSOR {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYCURSOR */
  WP_PROC_ID            proc;           /* process requesting info */

  u_int32               cursor_id;      /* id of cursor to create */
} WP_CMD_DESTROYCURSOR;

typedef struct _WP_REP_DESTROYCURSOR {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYCURSOR */
  error_code            err;            /* error code returned */
} WP_REP_DESTROYCURSOR;


typedef struct _WP_CMD_SETCURPOS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYCURSOR */
  WP_PROC_ID            proc;           /* process requesting info */
  WIN_ID                win;
  GFX_POS               x;
  GFX_POS               y;              /* where to put cursor */
} WP_CMD_SETCURPOS;

typedef struct _WP_REP_SETCURPOS {
  MSG_COMMON            com;            /* common MAUI message part */
  WP_FUNC               func;           /* WP_FUNC_DESTROYCURSOR */
  error_code            err;            /* error code returned */
} WP_REP_SETCURPOS;



/*
   WP_CMD_MSG is the union of all possible command messages.
   */
typedef union _WP_CMD_MSG {
  MSG_COMMON            com;            /* to access common MSG part */
  WP_CMD_COMMON         any;            /* to access common command part */

  WP_CMD_FORK           fork;

  WP_CMD_CREATEDEV      createdev;
  WP_CMD_DESTROYDEV     destroydev;
  WP_CMD_OPENDEV        opendev;
  WP_CMD_CLOSEDEV       closedev;
  WP_CMD_OPENINPDEV     openinpdev;
  WP_CMD_CLOSEINPDEV    closeinpdev;
  WP_CMD_SETFOCUS       setfocus;
  WP_CMD_GRABPTR	grabptr;
  WP_CMD_UNGRABPTR	ungrabptr;

  WP_CMD_CREATEWIN      createwin;
  WP_CMD_DESTROYWIN     destroywin;
  WP_CMD_GETWINSTATUS   getwinstatus;
  WP_CMD_UNLOCKREGION   unlockregion;

  WP_CMD_SETSTATE       setstate;
  WP_CMD_MOVEWIN        movewin;
  WP_CMD_RESIZEWIN      resizewin;
  WP_CMD_RESTACKWIN     restackwin;
  WP_CMD_REPARENTWIN    reparentwin;

  WP_CMD_SETCALLBACK    setcallback;
  WP_CMD_SETMSGMASK     setmsgmask;
  WP_CMD_SETCMAP        setcmap;
  WP_CMD_SETCURSOR      setcursor;

  WP_CMD_SETINKMETHOD   setinkmethod;
  WP_CMD_SETINKPIX      setinkpix;
  WP_CMD_ERASEINK       eraseink;

  WP_CMD_CREATECMAP     createcmap;
  WP_CMD_DESTROYCMAP    destroycmap;
  WP_CMD_ALLOCCMAPCOLOR alloccmapcolor;
  WP_CMD_ALLOCCMAPCELLS alloccmapcells;
  WP_CMD_FREECMAPCELLS  freecmapcells;
  WP_CMD_SETCMAPCELLS   setcmapcells;
  WP_CMD_GETCMAPCELLS   getcmapcells;
  WP_CMD_GETCMAPFREE    getcmapfree;

  WP_CMD_CREATECURSOR   createcursor;
  WP_CMD_DESTROYCURSOR  destroycursor;

  WP_CMD_COPYBLOCK      copyblock;
  WP_CMD_RESTACKDEV     restackdev;
  WP_CMD_SETCURPOS      setcurpos;
  WP_CMD_SETCURSTATE    setcurstate;

  WP_CMD_ALLOCCMAPCOLORS alloccmapcolors;

} WP_CMD_MSG;

/*
   WP_REP_MSG is the union of all possible reply messages.
   */
typedef union _WP_REP_MSG {
  MSG_COMMON            com;            /* to access common MSG part */
  WP_REP_COMMON         any;            /* to access common reply part */

  WP_REP_FORK           fork;

  WP_REP_CREATEDEV      createdev;
  WP_REP_DESTROYDEV     destroydev;
  WP_REP_OPENDEV        opendev;
  WP_REP_CLOSEDEV       closedev;
  WP_REP_OPENINPDEV     openinpdev;
  WP_REP_CLOSEINPDEV    closeinpdev;
  WP_REP_SETFOCUS       setfocus;
  WP_REP_GRABPTR	grabptr;
  WP_REP_UNGRABPTR	ungrabptr;

  WP_REP_CREATEWIN      createwin;
  WP_REP_DESTROYWIN     destroywin;
  WP_REP_GETWINSTATUS   getwinstatus;
  WP_REP_UNLOCKREGION   unlockregion;

  WP_REP_SETSTATE       setstate;
  WP_REP_MOVEWIN        movewin;
  WP_REP_RESIZEWIN      resizewin;
  WP_REP_RESTACKWIN     restackwin;
  WP_REP_REPARENTWIN    reparentwin;

  WP_REP_SETCALLBACK    setcallback;
  WP_REP_SETMSGMASK     setmsgmask;
  WP_REP_SETCMAP        setcmap;
  WP_REP_SETCURSOR      setcursor;

  WP_REP_SETINKMETHOD   setinkmethod;
  WP_REP_SETINKPIX      setinkpix;
  WP_REP_ERASEINK       eraseink;

  WP_REP_CREATECMAP     createcmap;
  WP_REP_DESTROYCMAP    destroycmap;
  WP_REP_ALLOCCMAPCOLOR alloccmapcolor;
  WP_REP_ALLOCCMAPCELLS alloccmapcells;
  WP_REP_FREECMAPCELLS  freecmapcells;
  WP_REP_SETCMAPCELLS   setcmapcells;
  WP_REP_GETCMAPCELLS   getcmapcells;
  WP_REP_GETCMAPFREE    getcmapfree;

  WP_REP_CREATECURSOR   createcursor;
  WP_REP_DESTROYCURSOR  destroycursor;

  WP_REP_COPYBLOCK      copyblock;
  WP_REP_RESTACKDEV     restackdev;
  WP_REP_SETCURPOS      setcurpos;
  WP_REP_SETCURSTATE    setcurstate;

  WP_REP_ALLOCCMAPCOLORS alloccmapcolors;

  WP_REP_BADACK         badack;         /* error reply */
} WP_REP_MSG;

#endif /* _WP_H */

