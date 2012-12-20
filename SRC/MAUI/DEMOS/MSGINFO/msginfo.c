/*******************************************************************************
 *
 * DESCRIPTION:	Mailbox info DEMO
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 *
 */

#define MAIN

#include <stdio.h>
#include <ctype.h>
#include <MAUI/wp.h> /* At the moment, this must be included before
                        mauidemo.h and maui_win.h. This to be
                        corrected. */
#include <MAUI/mauidemo.h>
#include <MAUI/maui_msg.h>
#include <MAUI/maui_inp.h>
#include <MAUI/mp.h>
#include <MAUI/maui_win.h>

/*******************************************************************************
 * Print helper functions
 *******************************************************************************/
#define strprint(expr) printf(#expr " = '%s'\n", expr)
#define dprint(expr)   printf(#expr " = %d\n", expr)
#define xprint(expr)   printf(#expr " = 0x%x\n", expr)
#define xnprint(expr)  printf(#expr " = 0x%x ", expr)
#define STR(ARG) case ARG: str = "" #ARG ","; break
#define CASE(ARG) case ARG: printf(" 0x%x ", ARG); break

typedef union _MESSAGE {
  MSG_COMMON any;             /* Common header on all messages */
  INP_MSG inp;
  WIN_MSG win;
  MP_DEV_MSG mpdev;
  WP_CMD_MSG wp_cmd;
  WP_REP_MSG wp_rep;
} MESSAGE;

u_int16 dump = 0;           /* Option flag used to dump more data */

static u_int16 item;        /* Counter for mailbox entries */

/*******************************************************************************
 * Usage information
 *******************************************************************************/
static void print_usage(char* modname)
{
  fprintf(stderr, "Usage: %s <opts> <mboxnames ...> \n", modname);
  fprintf(stderr, "Description: Print information about mailboxes\n");
  fprintf(stderr, "<opts>: -h or -?  This help\n");
  fprintf(stderr, "        -d        Dump contents\n");
  fprintf(stderr, "        -dd       Attempt to decode messages\n");
  exit(0);
}

/*******************************************************************************
 * Option parsing
 *******************************************************************************/
static void parse_options(int argc, char **argv)
{
  char *modname = *argv;
  char *ptr;
  
  /* parse the command line parameters */
  while (--argc) {
    if (*(ptr = *++argv) == '-') {
      while(*++ptr) {
        switch(*ptr|0x20) { /* to lower */
          
        case 'd':
          dump++;
          break;
          
        case 'h':
        case '?':   /* help message */
          print_usage(modname);
          exit(0);
          
        default:    /* illegal option */
          fprintf(stderr, "\nunknown option: '%c'\n",*ptr);
          print_usage(modname);
          exit(1);
          
        }   /* end of switch */
      }   /* end of while */
    } /* end of if */
  } /* end of while */
}

/*******************************************************************************
 * Function to print window process message function types
 *******************************************************************************/
static void wp_func_print(WP_FUNC func)
{
  switch (func) {
    CASE(WP_FUNC_FORK);
    
    CASE(WP_FUNC_CREATEDEV);
    CASE(WP_FUNC_DESTROYDEV);
    CASE(WP_FUNC_OPENDEV);
    CASE(WP_FUNC_CLOSEDEV);
    CASE(WP_FUNC_OPENINPDEV);
    CASE(WP_FUNC_CLOSEINPDEV);
    CASE(WP_FUNC_SETFOCUS);
    
    CASE(WP_FUNC_CREATEWIN);
    CASE(WP_FUNC_DESTROYWIN);
    CASE(WP_FUNC_GETWINSTATUS);
    CASE(WP_FUNC_UNLOCKREGION);
    
    CASE(WP_FUNC_SETSTATE);
    CASE(WP_FUNC_MOVEWIN);
    CASE(WP_FUNC_RESIZEWIN);
    CASE(WP_FUNC_RESTACKWIN);
    CASE(WP_FUNC_REPARENTWIN);
    
    CASE(WP_FUNC_SETCALLBACK);
    CASE(WP_FUNC_SETMSGMASK);
    CASE(WP_FUNC_SETCMAP);
    CASE(WP_FUNC_SETCURSOR);
    
    CASE(WP_FUNC_SETINKMETHOD);
    CASE(WP_FUNC_SETINKPIX);
    CASE(WP_FUNC_ERASEINK);
    
    CASE(WP_FUNC_CREATECMAP);
    CASE(WP_FUNC_DESTROYCMAP);
    CASE(WP_FUNC_ALLOCCMAPCOLOR);
    CASE(WP_FUNC_ALLOCCMAPCELLS);
    CASE(WP_FUNC_FREECMAPCELLS);
    CASE(WP_FUNC_SETCMAPCELLS);
    CASE(WP_FUNC_GETCMAPCELLS);
    CASE(WP_FUNC_GETCMAPFREE);
    
    CASE(WP_FUNC_CREATECURSOR);
    CASE(WP_FUNC_DESTROYCURSOR);
    
    CASE(WP_FUNC_GRABPTR);
    CASE(WP_FUNC_UNGRABPTR);
    
    CASE(WP_FUNC_COPYBLOCK);
    CASE(WP_FUNC_RESTACKDEV);
    
    CASE(WP_FUNC_BADACK_REPLY);
  }
}


/*******************************************************************************
 * Filter function to print message description
 *******************************************************************************/
static error_code filter(BOOLEAN *ret_use_msg, const void *_msg, void *filter_data)
{
  MESSAGE *msg = (MESSAGE*)_msg;
  char *str;
  const char *title = "    %5s %-16s %8s %6s  %8s\n";
  const char *line  = "    %5d %-16s %8d %6d 0x%08x\n";

  if (!item++)
    printf(title, "Count", "Type", "Queued", "Pid", "Callback");

  switch (msg->any.type) {
    STR(MSG_TYPE_NONE);
    STR(MSG_TYPE_PTR);
    STR(MSG_TYPE_KEY);
    STR(MSG_TYPE_WIN);
    STR(MSG_TYPE_MPCMD);
    STR(WP_MSG_TYPE_CMD);
    STR(WP_MSG_TYPE_REP);
  default:
    str = "is unknown\n"; break;
  }
  printf(line, str, msg->any.time_queued, msg->any.pid, msg->any.callback);
  
  if (dump> 1) {
    
    switch (msg->any.type) {
    case MSG_TYPE_PTR:
      printf("      subtype : 0x%08x ", msg->inp.ptr.subtype); /* Type of pointer message */
#define IFOR(ARG) if (msg->inp.ptr.subtype & ARG) printf(#ARG " ")
      IFOR(INP_PTR_DOWN);
      IFOR(INP_PTR_UP);
      IFOR(INP_PTR_MOVE);
      printf("\n");
#undef IFOR
      printf("      device_id : 0x%08x\n", msg->inp.ptr.device_id);
      printf("      button : %d\n", msg->inp.ptr.button);
      printf("      button_state : 0x%08x\n", msg->inp.ptr.button_state);
      printf("      Position x,y : %d,%d\n", msg->inp.ptr.position.x, 
             msg->inp.ptr.position.y);
      printf("      keysym : 0x%08x '%c'\n", msg->inp.ptr.keysym,
             msg->inp.ptr.keysym <= 0xff && isprint(msg->inp.ptr.keysym) ? 
             msg->inp.ptr.keysym : '.');
      break;
      
    case MSG_TYPE_KEY:
      printf("      subtype : 0x%08x ", msg->inp.key.subtype); /* Type of key message */
#define IFOR(ARG) if (msg->inp.key.subtype & ARG) printf(#ARG " ")
      IFOR(INP_KEY_DOWN);
      IFOR(INP_KEY_UP);
      IFOR(INP_KEYMOD_DOWN);
      IFOR(INP_KEYMOD_UP);
      printf("\n");
#undef IFOR
      printf("      device_id : 0x%08x\n", msg->inp.key.device_id);
      printf("      keysym : 0x%08x '%c'\n", msg->inp.key.keysym,
             msg->inp.key.keysym <= 0xff && isprint(msg->inp.key.keysym) ? 
             msg->inp.key.keysym : '.');
      printf("      key mods : 0x%08x ", msg->inp.key.key_modifiers);
      if (msg->inp.key.key_modifiers == INP_KEY_NOMOD) {
        printf(" == INP_KEY_NOMOD\n");
      } else {
        printf("\n");
#define TEST_MOD(def) (msg->inp.key.key_modifiers & (def) ? 'x' : ' ')
        printf( "        Shft CapL Ctrl Alt  Meta Num  Scrl\n");
        printf( "        L R  L R  L R  L R  L R  Lock Lock\n");
        printf( "        %1c %1c  %1c %1c  %1c %1c  %1c %1c  %1c %1c   %1c    %1c\n",
                TEST_MOD(INP_KEY_SHIFT_L),
                TEST_MOD(INP_KEY_SHIFT_R),
                TEST_MOD(INP_KEY_CAPLCK_L),
                TEST_MOD(INP_KEY_CAPLCK_R),
                TEST_MOD(INP_KEY_CNTL_L),
                TEST_MOD(INP_KEY_CNTL_R),
                TEST_MOD(INP_KEY_ALT_L),
                TEST_MOD(INP_KEY_ALT_R),
                TEST_MOD(INP_KEY_META_L),
                TEST_MOD(INP_KEY_META_R),
                TEST_MOD(INP_KEY_NUMLCK),
                TEST_MOD(INP_KEY_SCRLCK));
#undef TEST_MOD
        break;
        
      case MSG_TYPE_WIN:
        switch(msg->win.any_win.wtype) {
          STR(WIN_MSG_BUTTON_DOWN);
          STR(WIN_MSG_BUTTON_UP);
          STR(WIN_MSG_BORDER_ENTER);
          STR(WIN_MSG_BORDER_LEAVE);
          STR(WIN_MSG_CREATE);
          STR(WIN_MSG_DESTROY);
          STR(WIN_MSG_EXPOSE);
          STR(WIN_MSG_FOCUS_IN);
          STR(WIN_MSG_FOCUS_OUT);
          STR(WIN_MSG_KEY_DOWN);
          STR(WIN_MSG_KEY_UP);
          STR(WIN_MSG_MOVE);
          STR(WIN_MSG_MOVE_REQ);
          STR(WIN_MSG_PTR);
          STR(WIN_MSG_REPARENT);
          STR(WIN_MSG_RESIZE);
          STR(WIN_MSG_RESIZE_REQ);
          STR(WIN_MSG_RESTACK);
          STR(WIN_MSG_RESTACK_REQ);
          STR(WIN_MSG_STATE);
          STR(WIN_MSG_STATE_REQ);
          STR(WIN_MSG_INK_OFF);
          STR(WIN_MSG_KEYMOD_DOWN);
          STR(WIN_MSG_KEYMOD_UP);
        default:
          str = "is unknown\n"; break;
        }
        printf("      wtype : 0x%08x %s\n", msg->win.any_win.wtype, str);
        printf("      windev : 0x%08x\n", msg->win.any_win.windev);
        printf("      win : 0x%08x\n", msg->win.any_win.win);
        break;
        
      case MSG_TYPE_MPCMD:
        xnprint(msg->mpdev.any.cmd);
        switch(msg->mpdev.any.cmd) {
          STR(CMD_INP_INIT);
          STR(CMD_INP_INIT_REPLY);
          STR(CMD_INP_TERM);
          STR(CMD_INP_TERM_REPLY);
          STR(CMD_OPEN_DEV);
          STR(CMD_OPEN_DEV_REPLY);
          STR(CMD_CLOSE_DEV);
          STR(CMD_CLOSE_DEV_REPLY);
          STR(CMD_SET_MSG_MASK);
          STR(CMD_SET_MSG_MASK_REPLY);
          STR(CMD_CHECK_KEYS);
          STR(CMD_CHECK_KEYS_REPLY);
          STR(CMD_GET_DEV_CAP);
          STR(CMD_GET_DEV_CAP_REPLY);
          STR(CMD_GET_DEV_STATUS);
          STR(CMD_GET_DEV_STATUS_REPLY);
          STR(CMD_RELEASE_KEY);
          STR(CMD_RELEASE_KEY_REPLY);
          STR(CMD_RESERVE_KEY);
          STR(CMD_RESERVE_KEY_REPLY);
          STR(CMD_SET_MSG_CALLBACK);
          STR(CMD_SET_MSG_CALLBACK_REPLY);
          STR(CMD_SET_SIM_METH);
          STR(CMD_SET_SIM_METH_REPLY);
          STR(CMD_SET_PTR_LIMIT);
          STR(CMD_SET_PTR_LIMIT_REPLY);
          STR(CMD_SET_PTR_POS);
          STR(CMD_SET_PTR_POS_REPLY);
          STR(CMD_BADACK_REPLY);
          STR(CMD_RESTACK_DEV);
          STR(CMD_RESTACK_DEV_REPLY);
        default:
          str = "is unknown\n"; break;
        }
        xprint(msg->mpdev.any.dev_id);
        break;
        
      case WP_MSG_TYPE_CMD:
        xnprint(msg->wp_cmd.any.func);
        wp_func_print(msg->wp_cmd.any.func);
        xprint(msg->wp_cmd.any.proc);
        break;
        
      case WP_MSG_TYPE_REP:
        xnprint(msg->wp_rep.any.func);
        wp_func_print(msg->wp_rep.any.func);
        xprint(msg->wp_rep.any.err);
        break;
        
      default:
        break;
      }
    }
  }
  printf("+------------------------------------------------------+\n");
  
  *ret_use_msg = FALSE;
  return SUCCESS;
}


/*******************************************************************************
 * MAINLINE
*******************************************************************************/
void main(int argc, char **argv)
{
	size_t          size;
	MSG_MBOX_ID     mbox;
	MESSAGE         msg;
	error_code      err;
	MSG_MBOX_STATUS status;

	parse_options(argc, argv);

	/* initialize the messaging API */
	msg_init();

	/* for each command line argument */
	while(*++argv)
	{
		/* skip the options arguments */
		if(**argv == '-')
			continue;

		/* open the mailbox */
		size= sizeof(msg);
		err= msg_open_mbox( & mbox, *argv, & size);
		if(err != SUCCESS)
			demo_error(err, "Cannot open mailbox %s.\n", *argv);
		else
		{
			err= msg_get_mbox_status( & status, mbox);
			if(err != SUCCESS)
				demo_error(err, "Failed to get status for mailbox %s.\n", *argv);
			else
			{
				printf("Mailbox status for '%s'\n", status.name);
				printf("  number of entries : %d\n", status.num_entries);
				printf("  free entries : %d\n", status.free_entries);
				printf("  message size : %d\n", status.entry_size);
				printf("  link count: %d\n", status.link_count);

				if(dump)
				{
					/* set a filter function to examine contents of mailbox */
					err= msg_set_filter(mbox, filter, 0);
					if(err != SUCCESS)
					{
						if(err == EOS_MAUI_NOTIMPLEMENTED)
							demo_error(err, "This version of \"mauidrvr\" does not support filtering, use \"mauidrvr_filter\".\n");
						else
							demo_error(err, "Failed to install filter function for dump.\n");
					}
				}
				else
				{
					item= 0;

					/*
					 * Trigger the filter callback. The ANY mask causes all
					 * message types to be passed to the filter function. The
					 * filter function passes back FALSE so we don't change
					 * the contents of the mailbox.
					 */
					msg_flush(mbox, MSG_TYPE_ANY);
				}
			}
		}

		err= msg_close_mbox(mbox);
		if(err != SUCCESS)
			demo_error(err, "Failed to to close mailbox.\n");

		printf("\n"); /* double space */
		fflush(stdout);
	}

	/* terminate the messaging API */
	msg_term();

	exit(0);
}
