/*******************************************************************************
* FILENAME : maui_msg.h
*
* DESCRIPTION :
*
*	This file contains definitions for applications that use the MAUI messaging
*	API.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  09/27/94  TJE  First version
*       2  01/09/01  srm  CF2144 Add Notification Extensions to MSG API
*/
#ifndef _MAUI_MSG_H
#define _MAUI_MSG_H

#include <stddef.h>
#include <types.h>
#include <time.h>
#include <const.h>
#include <process.h>
#include <signal.h>
#include <MAUI/maui_com.h>
#include <MAUI/maui_mem.h>

/*******************************************************************************
* DEFINE ID TYPES
*******************************************************************************/

#ifdef _MSG_ID_STRUCTS_EXPOSED
typedef struct _MSG_MBOX MSG_MBOX;
#define MSG_MBOX_ID MSG_MBOX *
#else
typedef void * MSG_MBOX_ID;
#endif

/*******************************************************************************
* BLOCKING TYPES
*******************************************************************************/

typedef enum {
	MSG_BLOCK,						/* Block until a message is ready */
	MSG_NOBLOCK,					/* Do not block, return an error instead */
	MSG_SIGBLOCK					/* Block even if we get a signal */
} MSG_BLOCK_TYPE;

/*******************************************************************************
* MAILBOX DEFINITIONS
*******************************************************************************/

#define MSG_MAX_MBOX_NAME (11+1)/* Maximum length of mailbox name is max */
								/* event name length plus 1 (NULL byte) */

typedef enum {
	MSG_AT_HEAD,				/* Place at head of the queue */
	MSG_AT_TAIL					/* Place at tail of the queue */
} MSG_PLACEMENT;

typedef struct _MSG_MBOX_STATUS {
	char name[MSG_MAX_MBOX_NAME]; /* Mailbox name */
	u_int32 num_entries;		/* Max number of entries allowed in mailbox */
	u_int32 free_entries;		/* Number of free entries in the mailbox */
	size_t entry_size;			/* Size of each entry in bytes */
	u_int16 link_count;			/* Number of links to the mailbox */
	u_int32 write_mask;			/* Mask for writing to the queue */
	error_code (*filter)(BOOLEAN *ret_use_msg, const void *msg, void
		*filter_data);			/* Filter function */
	void *filter_data;			/* Application data for filter() */
} MSG_MBOX_STATUS;

/*******************************************************************************
* MESSAGE TYPES
*
* Range of Bits				Defined By				Used By
* 0 - 19					MAUI APIs				Applications
* 20 - 23					Reserved by Microware	Microware (internal)
* 24 - 31					Applications			Applications
*******************************************************************************/

#define MSG_TYPE_NONE 0			/* No message */
#define MSG_TYPE_PTR 1			/* Pointer messages */
#define MSG_TYPE_KEY 2			/* Keysym messages */
#define MSG_TYPE_WIN 4			/* Windowing messages */
#define MSG_TYPE_ANY 0xffffffff	/* Any message */

/*******************************************************************************
* MESSAGE STRUCTURES
*******************************************************************************/

typedef struct _MSG_COMMON {
	u_int32 type;				/* Message type */
	u_int32 time_queued;		/* Time that the message was queued */
	process_id	pid;			/* Process ID of writer */
	void (*callback)(const void *msg);
								/* Message callback */
} MSG_COMMON;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code msg_close_mbox(MSG_MBOX_ID mbox);
error_code msg_create_mbox(MSG_MBOX_ID *ret_mbox, const char *mbox_name, u_int32 num_entries, size_t entry_size, u_int32 color);
error_code msg_dispatch(const void *msg);
error_code msg_flush(MSG_MBOX_ID mbox, u_int32 mask);
error_code msg_get_error_action(MAUI_ERR_LEVEL *ret_debug_level, MAUI_ERR_LEVEL *ret_passback_level, MAUI_ERR_LEVEL *ret_exit_level);
error_code msg_get_mbox_status(MSG_MBOX_STATUS *ret_mbox_status, MSG_MBOX_ID mbox);
error_code msg_init(void);
error_code msg_open_mbox(MSG_MBOX_ID *ret_mbox, const char *mbox_name, size_t *entry_size);
error_code msg_peek(MSG_MBOX_ID mbox, void *msg, u_int32 mask, MSG_BLOCK_TYPE block_type);
error_code msg_peekn(MSG_MBOX_ID mbox, void *msg, size_t *size, u_int32 mask, MSG_BLOCK_TYPE block_type);
error_code msg_read(MSG_MBOX_ID mbox, void *msg, u_int32 mask, MSG_BLOCK_TYPE block_type);
error_code msg_readn(MSG_MBOX_ID mbox, void *msg, size_t *size, u_int32 mask, MSG_BLOCK_TYPE block_type);
error_code msg_release_sig(MSG_MBOX_ID mbox);
error_code msg_send_sig(MSG_MBOX_ID mbox, u_int32 mask, signal_code signal);
error_code msg_set_error_action(MAUI_ERR_LEVEL debug_level, MAUI_ERR_LEVEL passback_level, MAUI_ERR_LEVEL exit_level);
error_code msg_set_filter(MSG_MBOX_ID mbox, error_code (*filter)(BOOLEAN *ret_use_msg, const void *msg, void *filter_data), void *filter_data);
error_code msg_set_mask(MSG_MBOX_ID mbox, u_int32 mask);
error_code msg_term(void);
error_code msg_unread(MSG_MBOX_ID mbox, const void *msg, MSG_BLOCK_TYPE block_type, MSG_PLACEMENT placement);
error_code msg_unreadn(MSG_MBOX_ID mbox, const void *msg, size_t *size, MSG_BLOCK_TYPE block_type, MSG_PLACEMENT placement);
error_code msg_write(MSG_MBOX_ID mbox, const void *msg, MSG_BLOCK_TYPE block_type, MSG_PLACEMENT placement);
error_code msg_writen(MSG_MBOX_ID mbox, const void *msg, size_t *size, MSG_BLOCK_TYPE block_type, MSG_PLACEMENT placement);

/* MAUI 3.1 extensions */
error_code msg_send_watch(process_id pid, signal_code signal);
error_code msg_release_watch(process_id pid);

#ifdef __cplusplus
}
#endif

#endif /* _MAUI_MSG_H */
