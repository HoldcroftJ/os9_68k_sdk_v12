/*******************************************************************************
* FILENAME : maui.h
*
* DESCRIPTION :
*
*	This file is provided as a convenience. It includes the header file for
*	each MAUI API and also defines prototypes for the MAUI System API.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  06/21/95  TJE  First version
*/
#ifndef _MAUI_H
#define _MAUI_H

/*******************************************************************************
* INCLUDE THE HEADER FILE FOR EACH MAUI API
*******************************************************************************/

#include <MAUI/maui_com.h>
#include <MAUI/maui_mem.h>
#include <MAUI/maui_cdb.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_blt.h>
#include <MAUI/maui_txt.h>
#include <MAUI/maui_drw.h>
#include <MAUI/maui_anm.h>
#include <MAUI/maui_msg.h>
#include <MAUI/maui_inp.h>
#include <MAUI/maui_win.h>

/*******************************************************************************
* DEFINE A UNION OF ALL MAUI MESSAGE STRUCTURES
*******************************************************************************/

typedef union _MAUI_MSG {
	MSG_PTR ptr;				/* Pointer message */
	MSG_KEY key;				/* Key symbol message */
	MSG_WIN_BORDER border;		/* Border enter/leave message */
	MSG_WIN_BUTTON button;		/* Button down/up message */
	MSG_WIN_CREATE create;		/* Create child window message */
	MSG_WIN_DESTROY destroy;	/* Destroy window message */
	MSG_WIN_EXPOSE expose;		/* Expose message */
	MSG_WIN_FOCUS focus;		/* Focus in/out message */
	MSG_WIN_KEY winkey;			/* Key down/up message */
	MSG_WIN_MOVE move;			/* Window move message */
	MSG_WIN_PTR winptr;			/* Pointer move message */
	MSG_WIN_REPARENT reparent;	/* Re-parent window message */
	MSG_WIN_RESIZE resize;		/* Window re-sized message */
	MSG_WIN_RESTACK restack;	/* Window Re-stacked message */
	MSG_WIN_STATE state;		/* Window state change message */
	MSG_WIN_INK_OFF inkoff;		/* Inking turned off message */
	MSG_WIN_COMMON any_win;		/* Any window message */
	MSG_COMMON any;				/* Common for all messages */
} MAUI_MSG;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code maui_init(void);
error_code maui_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL
    passback_level, MAUI_ERR_LEVEL exit_level);
error_code maui_term(void);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_H */
