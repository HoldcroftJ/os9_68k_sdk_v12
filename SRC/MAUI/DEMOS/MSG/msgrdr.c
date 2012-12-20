/*******************************************************************************
*
* DESCRIPTION:	MSG Reader Demo program
*
* COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
* VERSION:		@(#)msgrdr.c	1.4 10/13/95
* */
#define MAIN

#include "_msg.h"
#include <errno.h>

error_code my_filter(BOOLEAN *ret_use_msg, const void *msg, void *filter_data)
{
	*ret_use_msg = 1;
	if (filter_data == (void*)0xabbababa)
		printf("filter!\n");
	else
		printf("Bad filter entry!\n");
	return SUCCESS;
}
/*******************************************************************************
* MAINLINE
*******************************************************************************/
void main(int argc, char **argv)
{
  MESSAGE msg;
  MSG_MBOX_ID mbox;
  error_code err;
  u_int32 cnt = 0;
  size_t size = sizeof(MESSAGE);

  /* initialize the message API */
  msg_init();

  /* open the mailbox */
  err = msg_open_mbox(&mbox, MBOX_NAME, &size);
  if (err == EOS_MNF) {
    fprintf (stderr, "msgrdr could not find the mailbox.\n");
    fprintf (stderr, "Run msgwrtr and it will create the mailbox and start msgrdr.\n");
	fflush(stderr);
	exit(err);
  }
  if (getenv("FILTER"))
	  msg_set_filter(mbox, my_filter, (void*)0xabbababa);

  /* read the messages */
  do {
	err = msg_read(mbox, &msg, MSG_TYPE_ANY, MSG_BLOCK);
	if (err != SUCCESS)
	  fprintf (stderr, "***Read error %d%c\n", err, 7);
	if (msg.cnt != cnt) {
	  fprintf (stderr, "***Count error on read %d (msg.cnt=%d)%c\n", cnt, msg.cnt, 7);
	}
	cnt++;
  } while (msg.com.type != DONEMSG);

  fprintf (stderr, "Read %d messages\n", cnt);

  /* close the mailbox */
  msg_close_mbox(mbox);

  /* terminate the shaded memory API */
  msg_term();

  exit(0);
}

