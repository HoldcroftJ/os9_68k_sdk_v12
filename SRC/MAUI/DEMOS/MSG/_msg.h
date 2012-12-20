
/*******************************************************************************
*
* DESCRIPTION:	MSG Demo program common header
*
* AUTHOR:		Steve McClellan
*
* CREATED ON:	05/22/95
*
* COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
* VERSION:		@(#)_msg.h	1.2 10/13/95
* */
#ifndef _MSG_H
#define _MSG_H

#include <MAUI/maui_mem.h>
#include <MAUI/maui_msg.h>
#include <stdio.h>
#include <stdlib.h>

#define MBOX_NAME "app_mbox"
#define MBOX_CNT  50
#define TESTMSG   24
#define DONEMSG   25

typedef struct _MESSAGE {
  MSG_COMMON com;
  u_int32    cnt;
} MESSAGE;

#endif
