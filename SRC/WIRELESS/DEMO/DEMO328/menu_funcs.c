/*****************************************************************************
** Functions handling Main Menu's sub-menus
******************************************************************************
** Copyright 1996 by Microware Systems Corporation                          **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**   1 03/23/96 creation                                          rws       **
*****************************************************************************/

#include <MAUI/maui_mem.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_msg.h>
#include <MAUI/maui_inp.h>
#include "load.h"

#define GO_HOME   0
#define STAY_HERE 1

typedef union _MESSAGE {	/* Union of message types we care about */
	MSG_PTR ptr;
	MSG_KEY key;
} MESSAGE;

/* Source Drawmap Pointer in demo.c */
extern GFX_DMAP *dmap;	        /* for each  screen */
extern MSG_MBOX_ID mbox;		/* Mailbox ID */

/* Pointers to OS9 data module containing image -- used by load_image() */
extern char *image_name_a_f;	/* alarms fire image module */
extern char *image_name_a_w;	/* alarms warning image module */
extern char *image_name_a_w;	/* alarms warning image module */
extern char *image_name_s_1;	/* security main  image module */
extern char *image_name_s_e1;	/* employee 1 image module */
extern char *image_name_s_e2;	/* employee 2 image module */
extern char *image_name_s_e3;	/* employee 3 image module */
extern char *image_name_s_e4;	/* employee 4 image module */
extern char *image_name_m_1;	/* message1 image module */
extern char *image_name_m_2;	/* message2 image module */

/*
** Function Prototypes
*/

#if defined (DEBUG)
error_code ec;
extern int _errmsg (int nerr, char *msg, ...);
extern void terminate (error_code err);
 #endif

void security_menu (void)
{
	MESSAGE message;
	u_int32 security_pic (u_int32 num);
	
	while(1)
	{
#if defined (DEBUG)
        if ((ec = load_image (image_name_s_1, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        load_image (image_name_s_1, FALSE);
#endif

#if defined (DEBUG)
        while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
            if (message.ptr.com.type == MSG_TYPE_PTR &&
                message.ptr.subtype == INP_PTR_UP)
                break;

         if (ec != SUCCESS)
         terminate(_errmsg(ec, "msg_read() error during msg_read()\n"));
#else

		while (1)
		{
			msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK);
			if (message.ptr.com.type == MSG_TYPE_PTR &&
				message.ptr.subtype == INP_PTR_UP)
				break;
		}
#endif

		if (message.ptr.position.y < 50)
			return;

		else if (message.ptr.position.x < 62)
		{
			if (security_pic(2) == GO_HOME)
				return;
		}

		else if (message.ptr.position.x < 117)
		{
			if (security_pic(3) == GO_HOME)
				return;
		}

		else if (message.ptr.position.x < 174)
		{
			if (security_pic(4) == GO_HOME)
				return;
		}

		else
		{
			if (security_pic(5) == GO_HOME)
				return;
		}
	}
}

u_int32 security_pic (u_int32 num)
{
	MESSAGE message;

	switch (num)
	{
		case(2):
#if defined (DEBUG)
    	    	if ((ec = load_image (image_name_s_e1, FALSE)) != SUCCESS)
        	    	terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
    	    	load_image (image_name_s_e1, FALSE);
#endif
			break;

		case(3):
#if defined (DEBUG)
		        if ((ec = load_image (image_name_s_e2, FALSE)) != SUCCESS)
        		    terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
		        load_image (image_name_s_e2, FALSE);
#endif
			break;

		case(4):
#if defined (DEBUG)
        	if ((ec = load_image (image_name_s_e3, FALSE)) != SUCCESS)
            	terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        	load_image (image_name_s_e3, FALSE);
#endif		
			break;

		default:
#if defined (DEBUG)
        if ((ec = load_image (image_name_s_e4, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        load_image (image_name_s_e4, FALSE);
#endif

	}
	

#if defined (DEBUG)
        while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
           if (message.ptr.com.type == MSG_TYPE_PTR &&
               message.ptr.subtype == INP_PTR_UP)
               break;

         if (ec != SUCCESS)
             terminate(_errmsg(ec, "msg_read() error during msg_read()\n"));
#else
	while (1)
	{
		msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK);
		if (message.ptr.com.type == MSG_TYPE_PTR &&
			message.ptr.subtype == INP_PTR_UP)
			break;
	}
#endif

	if (message.ptr.position.y < 75)
		return (GO_HOME);

	else
		return (STAY_HERE);
}

void alarms_menu (void)
{
	MESSAGE message;

start:

#if defined (DEBUG)
        if ((ec = load_image (image_name_a_f, FALSE)) != SUCCESS)
			terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        load_image (image_name_a_f, FALSE);
#endif


#if defined (DEBUG)
        while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
            if (message.ptr.com.type == MSG_TYPE_PTR &&
                message.ptr.subtype == INP_PTR_UP)
                break;

        if (ec != SUCCESS)
            terminate(_errmsg(ec, "msg_read() error during msg_read()\n"));
#else

	while (1)
	{
		msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK);
		if (message.ptr.com.type == MSG_TYPE_PTR &&
			message.ptr.subtype == INP_PTR_UP)
			break;
	}
#endif

	if (message.ptr.position.y < 75)
		return;

	else
	{
#if defined (DEBUG)
        if ((ec = load_image (image_name_a_w, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        load_image (image_name_a_w, FALSE);
#endif

		
#if defined (DEBUG)
        while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
            if (message.ptr.com.type == MSG_TYPE_PTR &&
                message.ptr.subtype == INP_PTR_UP)
                break;

        if (ec != SUCCESS)
            terminate(_errmsg(ec, "msg_read() error during msg_read()\n"));
#else
		while (1)
		{
			msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK);
			if (message.ptr.com.type == MSG_TYPE_PTR &&
				message.ptr.subtype == INP_PTR_UP)
				break;
		}
#endif

		if (message.ptr.position.y < 75)
			return;
		else
			goto start;
	}
}

void messages_menu (void)
{
	MESSAGE message;

start:

#if defined (DEBUG)
        if ((ec = load_image (image_name_m_1, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        load_image (image_name_m_1, FALSE);
#endif

#if defined (DEBUG)
        while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
            if (message.ptr.com.type == MSG_TYPE_PTR &&
                message.ptr.subtype == INP_PTR_UP)
                break;

         if (ec != SUCCESS)
             terminate(_errmsg(ec, "msg_read() error during msg_read()\n"));
#else

	while (1)
	{
		msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK);
		if (message.ptr.com.type == MSG_TYPE_PTR &&
			message.ptr.subtype == INP_PTR_UP)
			break;
	}
#endif

	if (message.ptr.position.y < 75)
		return;

	else
	{
#if defined (DEBUG)
        if ((ec = load_image (image_name_m_2, FALSE)) != SUCCESS)
            terminate(_errmsg(ec, "Can't load image into image drawmap\n"));
#else
        load_image (image_name_m_2, FALSE);
#endif

#if defined (DEBUG)
        while ((ec = msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK)) == SUCCESS)
            if (message.ptr.com.type == MSG_TYPE_PTR &&
                message.ptr.subtype == INP_PTR_UP)
                break;

        if (ec != SUCCESS)
            terminate(_errmsg(ec, "msg_read() error during msg_read()\n"));
#else

		while (1)
		{
			msg_read (mbox, &message, MSG_TYPE_ANY, MSG_BLOCK);
			if (message.ptr.com.type == MSG_TYPE_PTR &&
				message.ptr.subtype == INP_PTR_UP)
				break;
		}
#endif

		if (message.ptr.position.y < 75)
			return;
		else
			goto start;
	}
}


