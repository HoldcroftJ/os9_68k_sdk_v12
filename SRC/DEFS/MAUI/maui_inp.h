/*******************************************************************************
* FILENAME : maui_inp.h
*
* DESCRIPTION :
*
*   This file contains definitions for applications that use the MAUI input
*   API.
*
* COPYRIGHT:
*
*   This source code is the proprietary confidential property of Microware
*   Systems Corporation, and is provided to licensee solely for documentation
*   and educational purposes. Reproduction, publication, or distribution in
*   form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  01/13/95  TJE  First version
*/
#ifndef _MAUI_INP_H
#define _MAUI_INP_H

#include <stddef.h>
#include <types.h>
#include <const.h>
#include <MAUI/maui_com.h>
#include <MAUI/maui_mem.h>
#include <MAUI/maui_msg.h>
#include <MAUI/maui_gfx.h>

/*******************************************************************************
* DEFINE ID TYPES
*******************************************************************************/

#ifdef _INP_ID_STRUCTS_EXPOSED
typedef struct _INP_DEV INP_DEV;
#define INP_DEV_ID INP_DEV *
#else
typedef void * INP_DEV_ID;
#endif

/*******************************************************************************
* DEFINITIONS FOR DEVICE PLACEMENT (STACKING ORDER)
*******************************************************************************/

typedef enum {
    INP_DEV_FRONT,              /* In front */
    INP_DEV_BACK,               /* In back */
    INP_DEV_FRONT_OF,           /* In front of the specified device */
    INP_DEV_BACK_OF             /* In back of the specified device */
} INP_DEV_PLACEMENT;

/*******************************************************************************
* DEFINITIONS FOR SIMULATION
*******************************************************************************/

typedef enum {
    INP_SIM_NATIVE,             /* Use native mode */
    INP_SIM_PTR,                /* Simulate pointer input */
    INP_SIM_KEY                 /* Simulate key symbol input */
} INP_SIM_METH;

/*******************************************************************************
* TYPE DEFINITIONS
*******************************************************************************/

#define INP_MAX_DEV_NAME (80+1) /* Maximum length of device name */
#define INP_MAX_BUTTONS 8       /* Maximum number of buttons on a ptr device */

/*******************************************************************************
* POINTER MESSAGES
*******************************************************************************/

typedef int INP_PTR_SUBTYPE;
#define INP_PTR_DOWN    1        /* Pointer button down */
#define INP_PTR_UP      2        /* Pointer button up */
#define INP_PTR_MOVE    4        /* Pointer move */

typedef struct _MSG_PTR {
    MSG_COMMON  com;            /* Common section */
    INP_PTR_SUBTYPE subtype;    /* Type of pointer message */
    INP_DEV_ID  device_id;      /* Device ID */
    u_int8      button;         /* Button that went up or down (1-8) */
    u_int8      button_state;   /* State of all pointer buttons (1 bit each) */
    GFX_POINT   position;       /* New position */
    wchar_t     keysym;         /* Keysym if simulation caused the message */
} MSG_PTR;

/*******************************************************************************
* KEY SYMBOL DEFINITIONS
*******************************************************************************/

#define INP_KEY_NONE        0x00
#define INP_KEY_NULL        0x00

#define INP_KEY_BS          0x08
#define INP_KEY_HT          0x09

#define INP_KEY_LF          0x0a

#define INP_KEY_CR          0x0d
#define INP_KEY_ENTER       0x0d

#define INP_KEY_ESC         0x1b

#define INP_KEY_DEL         0x7f

#define INP_KEY_PLAY        0xfe80
#define INP_KEY_STOP        0xfe81
#define INP_KEY_PAUSE       0xfe82
#define INP_KEY_NEXT        0xfe83
#define INP_KEY_PREV        0xfe84
#define INP_KEY_REWIND      0xfe85
#define INP_KEY_FASTFWD     0xfe86
#define INP_KEY_RECORD      0xfe87
#define INP_KEY_SKIP_BACK   0xfe88
#define INP_KEY_SKIP_FWD    0xfe89
#define INP_KEY_EJECT       0xfe8a

#define INP_KEY_CUR_UL      0xfe8c
#define INP_KEY_CUR_UR      0xfe8d
#define INP_KEY_CUR_DL      0xfe8e
#define INP_KEY_CUR_DR      0xfe8f

#define INP_KEY_GOTO        0xfe91
#define INP_KEY_EXIT        0xfe92
#define INP_KEY_DISPLAY     0xfe93
#define INP_KEY_STORE       0xfe94
#define INP_KEY_RECALL      0xfe95

#define INP_KEY_CHAN_U      0xfe99
#define INP_KEY_CHAN_D      0xfe9a
#define INP_KEY_LASTCHAN    0xfe9b
#define INP_KEY_VOD         0xfe9c
#define INP_KEY_VIP         0xfe9d
#define INP_KEY_VDT         0xfe9e
#define INP_KEY_POWER       0xfe9f
#define INP_KEY_POWER_ON    0xfea0
#define INP_KEY_POWER_OFF   0xfea1
#define INP_KEY_BYPASS      0xfea2
#define INP_KEY_BYPASS_ON   0xfea3
#define INP_KEY_BYPASS_OFF  0xfea4
#define INP_KEY_GUIDE       0xfea5
#define INP_KEY_TUNE        0xfea6
#define INP_KEY_THEME       0xfea7
#define INP_KEY_LIST        0xfea8
#define INP_KEY_MOVE        0xfea9
#define INP_KEY_PAGE_U      0xfeaa
#define INP_KEY_PAGE_D      0xfeab
#define INP_KEY_VOL_U       0xfeac
#define INP_KEY_VOL_D       0xfead
#define INP_KEY_MUTE        0xfeae
#define INP_KEY_SAP         0xfeaf
#define INP_KEY_PROG        0xfeb0
#define INP_KEY_PPV         0xfeb1
#define INP_KEY_FAV         0xfeb2
#define INP_KEY_DAY_U       0xfeb3
#define INP_KEY_DAY_D       0xfeb4
#define INP_KEY_INFO        0xfeb5
#define INP_KEY_OPTIONS     0xfeb6
#define INP_KEY_DEGAUSS     0xfeb7
#define INP_KEY_ZOOM        0xfeb8
#define INP_KEY_ZOOM_IN     0xfeb9
#define INP_KEY_ZOOM_OUT    0xfeba
#define INP_KEY_FORWARD     0xfebb
#define INP_KEY_BACK        0xfebc
#define INP_KEY_RELOAD      0xfebd
#define INP_KEY_PIP         0xfebe
#define INP_KEY_PIP_EXCHNG  0xfebf
#define INP_KEY_PIP_CHAN_U  0xfec0
#define INP_KEY_PIP_CHAN_D  0xfec1
#define INP_KEY_PIP_LASTCH  0xfec2

#define INP_KEY_TEL_0       0xfef0
#define INP_KEY_TEL_1       0xfef1
#define INP_KEY_TEL_2       0xfef2
#define INP_KEY_TEL_3       0xfef3
#define INP_KEY_TEL_4       0xfef4
#define INP_KEY_TEL_5       0xfef5
#define INP_KEY_TEL_6       0xfef6
#define INP_KEY_TEL_7       0xfef7
#define INP_KEY_TEL_8       0xfef8
#define INP_KEY_TEL_9       0xfef9
#define INP_KEY_TEL_STAR    0xfefa
#define INP_KEY_TEL_POUND   0xfefb
#define INP_KEY_SPEAKER     0xfefc
#define INP_KEY_REDIAL      0xfefd
#define INP_KEY_FLASH       0xfefe
#define INP_KEY_HANGUP      0xfeff

#define INP_KEY_CLEAR       0xff0b

#define INP_KEY_HOLD        0xff13

#define INP_KEY_HOME        0xff50
#define INP_KEY_CUR_L       0xff51
#define INP_KEY_CUR_U       0xff52
#define INP_KEY_CUR_R       0xff53
#define INP_KEY_CUR_D       0xff54

#define INP_KEY_END         0xff57
#define INP_KEY_BEGIN       0xff58

#define INP_KEY_SELECT      0xff60
#define INP_KEY_PRINT       0xff61
#define INP_KEY_EXECUTE     0xff62
#define INP_KEY_INSERT      0xff63

#define INP_KEY_UNDO        0xff65
#define INP_KEY_REDO        0xff66
#define INP_KEY_MENU        0xff67
#define INP_KEY_FIND        0xff68
#define INP_KEY_CANCEL      0xff69
#define INP_KEY_HELP        0xff6a
#define INP_KEY_BREAK       0xff6b
#define INP_KEY_MAIL        0xff6c
#define INP_KEY_GAMES       0xff6d
#define INP_KEY_WWW         0xff6e
#define INP_KEY_AGAIN       0xff6f
#define INP_KEY_CUT         0xff70
#define INP_KEY_COPY        0xff71
#define INP_KEY_PASTE       0xff72

#define INP_KEY_F1          0xffbe
#define INP_KEY_F2          0xffbf
#define INP_KEY_F3          0xffc0
#define INP_KEY_F4          0xffc1
#define INP_KEY_F5          0xffc2
#define INP_KEY_F6          0xffc3
#define INP_KEY_F7          0xffc4
#define INP_KEY_F8          0xffc5
#define INP_KEY_F9          0xffc6
#define INP_KEY_F10         0xffc7
#define INP_KEY_F11         0xffc8
#define INP_KEY_F12         0xffc9
#define INP_KEY_F13         0xffca
#define INP_KEY_F14         0xffcb
#define INP_KEY_F15         0xffcc
#define INP_KEY_F16         0xffcd
#define INP_KEY_F17         0xffce
#define INP_KEY_F18         0xffcf
#define INP_KEY_F19         0xffd0
#define INP_KEY_F20         0xffd1
#define INP_KEY_F21         0xffd2
#define INP_KEY_F22         0xffd3
#define INP_KEY_F23         0xffd4
#define INP_KEY_F24         0xffd5
#define INP_KEY_F25         0xffd6
#define INP_KEY_F26         0xffd7
#define INP_KEY_F27         0xffd8
#define INP_KEY_F28         0xffd9
#define INP_KEY_F29         0xffda
#define INP_KEY_F30         0xffdb
#define INP_KEY_F31         0xffdc
#define INP_KEY_F32         0xffdd
#define INP_KEY_F33         0xffde
#define INP_KEY_F34         0xffdf
#define INP_KEY_F35         0xffe0

#define INP_KEY_L1          0xffc8
#define INP_KEY_L2          0xffc9
#define INP_KEY_L3          0xffca
#define INP_KEY_L4          0xffcb
#define INP_KEY_L5          0xffcc
#define INP_KEY_L6          0xffcd
#define INP_KEY_L7          0xffce
#define INP_KEY_L8          0xffcf
#define INP_KEY_L9          0xffd0
#define INP_KEY_L10         0xffd1
#define INP_KEY_R1          0xffd2
#define INP_KEY_R2          0xffd3
#define INP_KEY_R3          0xffd4
#define INP_KEY_R4          0xffd5
#define INP_KEY_R5          0xffd6
#define INP_KEY_R6          0xffd7
#define INP_KEY_R7          0xffd8
#define INP_KEY_R8          0xffd9
#define INP_KEY_R9          0xffda
#define INP_KEY_R10         0xffdb
#define INP_KEY_R11         0xffdc
#define INP_KEY_R12         0xffdd
#define INP_KEY_R13         0xffde
#define INP_KEY_R14         0xffdf
#define INP_KEY_R15         0xffe0

/*******************************************************************************
* KEYSYM MESSAGES
*******************************************************************************/

typedef int INP_KEY_SUBTYPE;
#define INP_KEY_DOWN    1        /* Key down (pressed) */
#define INP_KEY_UP      2        /* Key up (released) */
#define INP_KEYMOD_DOWN 4        /* Key modifier down (pressed) */
#define INP_KEYMOD_UP   8        /* Key modifier up (released) */

typedef int INP_KEYMOD;
#define INP_KEY_NOMOD       0x00000000 /* No modifier keys */
#define INP_KEY_SHIFT_L     0x00000001 /* Left shift key */
#define INP_KEY_SHIFT_R     0x00000002 /* Right shift key */
#define INP_KEY_CAPLCK_L    0x00000004 /* Left caps-lock key */
#define INP_KEY_CAPLCK_R    0x00000008 /* Right caps-lock key */
#define INP_KEY_CNTL_L      0x00000010 /* Left control key */
#define INP_KEY_CNTL_R      0x00000020 /* Right control key */
#define INP_KEY_ALT_L       0x00000040 /* Left alt key */
#define INP_KEY_ALT_R       0x00000080 /* Right alt key */
#define INP_KEY_META_L      0x00000100 /* Left meta key */
#define INP_KEY_META_R      0x00000200 /* Right meta key */
#define INP_KEY_NUMLCK      0x00000400 /* Num-lock key */
#define INP_KEY_SCRLCK      0x00000800 /* Scroll-lock key */
#define INP_NUMPAD          0x00001000 /* Key is on the NUMPAD */

typedef struct _MSG_KEY {
    MSG_COMMON  com;            /* Common section */
    INP_KEY_SUBTYPE subtype;    /* Type of key symbol message */
    INP_DEV_ID  device_id;      /* Device ID */
    wchar_t     keysym;         /* Key symbol */
    INP_KEYMOD  key_modifiers;  /* Key modifiers */
} MSG_KEY;

/*******************************************************************************
* DEVICE CAPABILITIES
*******************************************************************************/

typedef int INP_DEV_CLASS;
#define INP_CLASS_MOUSE     0x00000001 /* Mouse */
#define INP_CLASS_TRACKBALL 0x00000002 /* Trackball */
#define INP_CLASS_TOUCHSCR  0x00000004 /* Touch-screen */
#define INP_CLASS_TABLET    0x00000008 /* Tablet */
#define INP_CLASS_JOYSTICK  0x00000010 /* Joystick */
#define INP_CLASS_GAMEPAD   0x00000020 /* Game pad */
#define INP_CLASS_KEYBOARD  0x00000040 /* Alphanumeric keyboard */
#define INP_CLASS_REMOTE    0x00000080 /* Remote control */

typedef int INP_KEYS;
#define INP_KEYS_NONE   0x00000000    /* No keys */
#define INP_KEYS_ASCII  0x00000001    /* ASCII character set */
#define INP_KEYS_UCASE  0x00000002    /* Upper case alphabetic keys (A-Z) */
#define INP_KEYS_LCASE  0x00000004    /* Lower case alphabetic keys (a-z) */
#define INP_KEYS_NUM    0x00000008    /* Numeric keys (0-9) */
#define INP_KEYS_4WAY   0x00000010    /* Four way cursor movement */
#define INP_KEYS_8WAY   0x00000020    /* Eight way cursor movement */
#define INP_KEYS_PLAYER 0x00000040    /* Play, stop, pause, fastfwd, rewind */
#define INP_KEYS_PHONE  0x00000080     /* Telephone keypad (12 keys) */

typedef struct _INP_DEV_CAP {
    INP_DEV_CLASS ptr_type;     /* Device classification */
    u_int8      ptr_buttons;    /* Number of pointer buttons */
    u_int8      func_keys;      /* Number of function keys (1st is F1) */
    INP_KEYMOD  key_modifiers;  /* Key modifiers supported */
    INP_KEYS    key_cap;        /* Keysym capabilities */
} INP_DEV_CAP;

/*******************************************************************************
* DEVICE STATUS
*******************************************************************************/

typedef struct _INP_DEV_STATUS {
    GFX_POINT   ptr_min;        /* Minimum position for the pointer */
    GFX_POINT   ptr_max;        /* Maximum position for the pointer */
    GFX_POINT   ptr_cur;        /* Current position of the pointer */
    u_int8      button_state;   /* Current button state */
    INP_KEYMOD  key_modifiers;  /* Current key modifiers */
    INP_SIM_METH sim_meth;      /* Simulation method */
    GFX_DELTA   speed;          /* X/Y speed for simulation */
    wchar_t button_map[INP_MAX_BUTTONS];
                                /* Button to/from keysym associations */
    u_int32     write_mask;     /* Write mask for messages */
    void (*callback)(const void *msg);
                                /* Callback for messages */
} INP_DEV_STATUS;

/*******************************************************************************
* DEFINE A UNION OF ALL INPUT MESSAGE TYPES
*******************************************************************************/

typedef union _INP_MSG {
    MSG_PTR ptr;                /* Pointer message */
    MSG_KEY key;                /* Key symbol message */
    MSG_COMMON any;             /* Common header on all messages */
} INP_MSG;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code inp_check_keys(BOOLEAN *ret_all_present, INP_DEV_ID inpdev, wchar_t
    min_key, wchar_t max_key);
error_code inp_close_dev(INP_DEV_ID inpdev);
error_code inp_get_dev_cap(INP_DEV_CAP *ret_dev_cap, INP_DEV_ID inpdev);
error_code inp_get_dev_status(INP_DEV_STATUS *ret_dev_status, INP_DEV_ID
    inpdev);
error_code inp_init(void);
error_code inp_open_dev(INP_DEV_ID *ret_inpdev, MSG_MBOX_ID mbox, const char
    *device_name);
error_code inp_release_key(INP_DEV_ID inpdev, wchar_t key);
error_code inp_reserve_key(INP_DEV_ID inpdev, wchar_t key);
error_code inp_restack_dev(INP_DEV_ID inpdev, INP_DEV_PLACEMENT placement, ...);
error_code inp_set_callback(INP_DEV_ID inpdev, void (*callback)(const void
    *msg));
error_code inp_set_sim_meth(INP_DEV_ID inpdev, u_int8 num_buttons, const
    wchar_t keysyms[], INP_SIM_METH sim_meth, ...);
error_code inp_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
    passback_level, MAUI_ERR_LEVEL exit_level);
error_code inp_set_msg_mask(INP_DEV_ID inpdev, u_int32 mask);
error_code inp_set_ptr_limit(INP_DEV_ID inpdev, GFX_POS minx, GFX_POS miny,
    GFX_POS maxx, GFX_POS maxy);
error_code inp_set_ptr_pos(INP_DEV_ID inpdev, GFX_POS x, GFX_POS y);
error_code inp_term(void);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_INP_H */
