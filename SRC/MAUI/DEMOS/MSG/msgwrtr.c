/*******************************************************************************
 *
 * DESCRIPTION:	MSG Writer Demo program
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 * VERSION:		@(#)msgwrtr.c	1.4 10/13/95
 *
 */
#define MAIN

#include "_msg.h"
#include <process.h>
#include <types.h>

extern char **_environ;

/*******************************************************************************
 * Prototypes
 *******************************************************************************/
static void print_status(MSG_MBOX_ID);
static void exec_reader(process_id *pid);
static void term_reader(process_id pid);

/* system functions missing prototypes */
int tsleep(unsigned);

/*******************************************************************************
 * MAINLINE
 *******************************************************************************/
void main(int argc, char **argv)
{
	MESSAGE msg;
	MSG_MBOX_ID mbox;
	error_code err;
	process_id pid;
	
	/* initialize the message API */
	msg_init();
	
	/* open the mailbox */
	msg_create_mbox(&mbox, MBOX_NAME, MBOX_CNT, sizeof(MESSAGE), 0);
	
	/* start the reader */
	exec_reader(&pid);
	
	/* init message header */
	msg.com.type = TESTMSG;
	msg.com.callback = NULL;
	
	/* write the messages */
	for (msg.cnt = 0; msg.cnt < 1000; msg.cnt++)
	{
		while ((err = msg_write(mbox, &msg, MSG_NOBLOCK, MSG_AT_TAIL)) == EOS_MAUI_MBOXFULL)
		{
			fprintf (stderr, "mbox full retrying\n");
			print_status(mbox);
			tsleep(10);
		}
		if (err != SUCCESS)
		{
			fprintf (stderr, "msg_write error %d%c\n", err, 7);
		}
	}
	
	print_status(mbox);
	
	/* send DONE message */
	msg.com.type = DONEMSG;
	while ((err = msg_write(mbox, &msg, MSG_NOBLOCK, MSG_AT_TAIL)) == EOS_MAUI_MBOXFULL)
	{
		fprintf (stderr, "mbox full retrying\n");
		print_status(mbox);
		tsleep(10);
	}
	if (err != SUCCESS)
	{
		fprintf (stderr, "error %d writing DONE message\n", err);
	}
	
	fprintf (stderr, "Wrote %d messages\n", ++msg.cnt);
	
	/* close the mailbox */
	msg_close_mbox(mbox);
	
	/* wait for the reader to exit */
	term_reader(pid);
	
	/* terminate the message API */
	msg_term();
	
	exit(0);
	
}

/*******************************************************************************
 *
 * NAME:		print_status()
 *
 * USAGE:	Print mailbox status
 */
static void print_status(MSG_MBOX_ID mbox)
{
	MSG_MBOX_STATUS status;
	
	if (msg_get_mbox_status(&status, mbox) == SUCCESS)
	{
		fprintf (stderr, "status.name %s\n", status.name);
		fprintf (stderr, "status.num_entries  %d\n", status.num_entries);
		fprintf (stderr, "status.free_entries %d\n", status.free_entries);
		fprintf (stderr, "status.entry_size   %d\n", status.entry_size);
		fprintf (stderr, "status.link_count   %d\n", status.link_count);
		fprintf (stderr, "status.write_mask   0x%x\n", status.write_mask);
		fprintf (stderr, "status.filter       0x%x\n", status.filter);
		fprintf (stderr, "status.filter_data  0x%x\n\n", status.filter_data);
	}
	
	return;
}

/******************************************************************************
 *
 * NAME:		exec_reader()
 *
 * USAGE:	Start reader process
 */
static void exec_reader(process_id *pid)
{
	error_code err;
	char *argv[] =
	{
		"msgrdr",
		0
	};
	
	err = _os_exec (_os_fork, 0, 3, argv[0], argv, _environ, 0, pid, 0, 0);
	if (err != SUCCESS)
		exit(err);
	
	return;
}

/******************************************************************************
 *
 * NAME:		term_reader()
 *
 * USAGE:	Wait for reader process to terminate
 */
static void term_reader(process_id pid)
{
	process_id child_id;
	status_code ret;
	
	fprintf (stderr, "Waiting for child to finish\n");
	
	do {
		_os_wait (&child_id, &ret);
	} while (child_id != pid);
	
	if (ret != SUCCESS)
		fprintf (stderr, "Reader exited with status %d\n", ret);
	
	return;
}

