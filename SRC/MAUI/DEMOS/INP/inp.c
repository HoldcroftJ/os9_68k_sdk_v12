/*******************************************************************************
 *
 * DESCRIPTION: MAUI INP API Example
 *
 * COPYRIGHT:   Copyright 1995 Microware Systems Corporation. All Rights
 *              Reserved. Use and/or distribution subject to terms and
 *              conditions of applicable Microware license agreement(s).
 *
 */


#include <MAUI/maui.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

/* Function Prototypes */
int intercept(void (*icpthand)(u_int32));

/* Treat signal as request to quit */
volatile BOOLEAN done = FALSE;
void signal_handler(u_int32 signum)
{
  done = TRUE;
}

/* returns a name for the key symbol */
char *get_name(wchar_t keysym)
{
  switch (keysym) {
  case INP_KEY_NULL:
    return "INP_KEY_NULL";
  case INP_KEY_BS:
    return "INP_KEY_BS";
  case INP_KEY_HT:
    return "INP_KEY_HT";
  case INP_KEY_LF:
    return "INP_KEY_LF";
  case INP_KEY_ENTER: /* and INP_KEY_CR */
    return "INP_KEY_ENTER/INP_KEY_CR";
  case INP_KEY_ESC:
    return "INP_KEY_ESC";
  case INP_KEY_DEL:
    return "INP_KEY_DEL";
  case INP_KEY_PLAY:
    return "INP_KEY_PLAY";
  case INP_KEY_STOP:
    return "INP_KEY_STOP";
  case INP_KEY_PAUSE:
    return "INP_KEY_PAUSE";
  case INP_KEY_NEXT:
    return "INP_KEY_NEXT";
  case INP_KEY_PREV:
    return "INP_KEY_PREV";
  case INP_KEY_REWIND:
    return "INP_KEY_REWIND";
  case INP_KEY_FASTFWD:
    return "INP_KEY_FASTFWD";
  case INP_KEY_RECORD:
    return "INP_KEY_RECORD";
  case INP_KEY_CUR_UL:
    return "INP_KEY_CUR_UL";
  case INP_KEY_CUR_UR:
    return "INP_KEY_CUR_UR";
  case INP_KEY_CUR_DL:
    return "INP_KEY_CUR_DL";
  case INP_KEY_CUR_DR:
    return "INP_KEY_CUR_DR";
  case INP_KEY_GOTO:
    return "INP_KEY_GOTO";
  case INP_KEY_EXIT:
    return "INP_KEY_EXIT";
  case INP_KEY_DISPLAY:
    return "INP_KEY_DISPLAY";
  case INP_KEY_STORE:
    return "INP_KEY_STORE";
  case INP_KEY_RECALL:
    return "INP_KEY_RECALL";
  case INP_KEY_CHAN_U:
    return "INP_KEY_CHAN_U";
  case INP_KEY_CHAN_D:
    return "INP_KEY_CHAN_D";
  case INP_KEY_LASTCHAN:
    return "INP_KEY_LASTCHAN";
  case INP_KEY_VIP:
    return "INP_KEY_VIP";
  case INP_KEY_VDT:
    return "INP_KEY_VDT";
  case INP_KEY_POWER:
    return "INP_KEY_POWER";
  case INP_KEY_POWER_ON:
    return "INP_KEY_POWER_ON";
  case INP_KEY_POWER_OFF:
    return "INP_KEY_POWER_OFF";
  case INP_KEY_BYPASS:
    return "INP_KEY_BYPASS";
  case INP_KEY_BYPASS_ON:
    return "INP_KEY_BYPASS_ON";
  case INP_KEY_BYPASS_OFF:
    return "INP_KEY_BYPASS_OFF";
  case INP_KEY_GUIDE:
    return "INP_KEY_GUIDE";
  case INP_KEY_TUNE:
    return "INP_KEY_TUNE";
  case INP_KEY_THEME:
    return "INP_KEY_THEME";
  case INP_KEY_LIST:
    return "INP_KEY_LIST";
  case INP_KEY_MOVE:
    return "INP_KEY_MOVE";
  case INP_KEY_PAGE_U:
    return "INP_KEY_PAGE_U";
  case INP_KEY_PAGE_D:
    return "INP_KEY_PAGE_D";
  case INP_KEY_VOL_U:
    return "INP_KEY_VOL_U";
  case INP_KEY_VOL_D:
    return "INP_KEY_VOL_D";
  case INP_KEY_MUTE:
    return "INP_KEY_MUTE";
  case INP_KEY_SAP:
    return "INP_KEY_SAP";
  case INP_KEY_PROG:
    return "INP_KEY_PROG";
  case INP_KEY_PPV:
    return "INP_KEY_PPV";
  case INP_KEY_FAV:
    return "INP_KEY_FAV";
  case INP_KEY_DAY_U:
    return "INP_KEY_DAY_U";
  case INP_KEY_DAY_D:
    return "INP_KEY_DAY_D";
  case INP_KEY_INFO:
    return "INP_KEY_INFO";
  case INP_KEY_OPTIONS:
    return "INP_KEY_OPTIONS";
  case INP_KEY_DEGAUSS:
    return "INP_KEY_DEGAUSS";
  case INP_KEY_ZOOM:
    return "INP_KEY_ZOOM";
  case INP_KEY_ZOOM_IN:
    return "INP_KEY_ZOOM_IN";
  case INP_KEY_ZOOM_OUT:
    return "INP_KEY_ZOOM_OUT";
  case INP_KEY_FORWARD:
    return "INP_KEY_FORWARD";
  case INP_KEY_BACK:
    return "INP_KEY_BACK";
  case INP_KEY_RELOAD:
    return "INP_KEY_RELOAD";
  case INP_KEY_PIP:
    return "INP_KEY_PIP";
  case INP_KEY_PIP_EXCHNG:
    return "INP_KEY_PIP_EXCHNG";
  case INP_KEY_PIP_CHAN_U:
    return "INP_KEY_PIP_CHAN_U";
  case INP_KEY_PIP_CHAN_D:
    return "INP_KEY_PIP_CHAN_D";
  case INP_KEY_PIP_LASTCH:
    return "INP_KEY_PIP_LASTCH";
  case INP_KEY_TEL_0:
    return "INP_KEY_TEL_0";
  case INP_KEY_TEL_1:
    return "INP_KEY_TEL_1";
  case INP_KEY_TEL_2:
    return "INP_KEY_TEL_2";
  case INP_KEY_TEL_3:
    return "INP_KEY_TEL_3";
  case INP_KEY_TEL_4:
    return "INP_KEY_TEL_4";
  case INP_KEY_TEL_5:
    return "INP_KEY_TEL_5";
  case INP_KEY_TEL_6:
    return "INP_KEY_TEL_6";
  case INP_KEY_TEL_7:
    return "INP_KEY_TEL_7";
  case INP_KEY_TEL_8:
    return "INP_KEY_TEL_8";
  case INP_KEY_TEL_9:
    return "INP_KEY_TEL_9";
  case INP_KEY_TEL_STAR:
    return "INP_KEY_TEL_STAR";
  case INP_KEY_TEL_POUND:
    return "INP_KEY_TEL_POUND";
  case INP_KEY_SPEAKER:
    return "INP_KEY_SPEAKER";
  case INP_KEY_REDIAL:
    return "INP_KEY_REDIAL";
  case INP_KEY_FLASH:
    return "INP_KEY_FLASH";
  case INP_KEY_HANGUP:
    return "INP_KEY_HANGUP";
  case INP_KEY_CLEAR:
    return "INP_KEY_CLEAR";
  case INP_KEY_HOLD:
    return "INP_KEY_HOLD";
  case INP_KEY_HOME:
    return "INP_KEY_HOME";
  case INP_KEY_CUR_L:
    return "INP_KEY_CUR_L";
  case INP_KEY_CUR_U:
    return "INP_KEY_CUR_U";
  case INP_KEY_CUR_R:
    return "INP_KEY_CUR_R";
  case INP_KEY_CUR_D:
    return "INP_KEY_CUR_D";
  case INP_KEY_END:
    return "INP_KEY_END";
  case INP_KEY_BEGIN:
    return "INP_KEY_BEGIN";
  case INP_KEY_SELECT:
    return "INP_KEY_SELECT";
  case INP_KEY_PRINT:
    return "INP_KEY_PRINT";
  case INP_KEY_EXECUTE:
    return "INP_KEY_EXECUTE";
  case INP_KEY_INSERT:
    return "INP_KEY_INSERT";
  case INP_KEY_UNDO:
    return "INP_KEY_UNDO";
  case INP_KEY_REDO:
    return "INP_KEY_REDO";
  case INP_KEY_MENU:
    return "INP_KEY_MENU";
  case INP_KEY_FIND:
    return "INP_KEY_FIND";
  case INP_KEY_CANCEL:
    return "INP_KEY_CANCEL";
  case INP_KEY_HELP:
    return "INP_KEY_HELP";
  case INP_KEY_BREAK:
    return "INP_KEY_BREAK";
  case INP_KEY_F1:
    return "INP_KEY_F1";
  case INP_KEY_F2:
    return "INP_KEY_F2";
  case INP_KEY_F3:
    return "INP_KEY_F3";
  case INP_KEY_F4:
    return "INP_KEY_F4";
  case INP_KEY_F5:
    return "INP_KEY_F5";
  case INP_KEY_F6:
    return "INP_KEY_F6";
  case INP_KEY_F7:
    return "INP_KEY_F7";
  case INP_KEY_F8:
    return "INP_KEY_F8";
  case INP_KEY_F9:
    return "INP_KEY_F9";
  case INP_KEY_F10:
    return "INP_KEY_F10";
  case INP_KEY_F11: /* and INP_KEY_L1 */
    return "INP_KEY_F11/INP_KEY_L1";
  case INP_KEY_F12: /* and INP_KEY_L2 */
    return "INP_KEY_F12/INP_KEY_L2";
  case INP_KEY_F13: /* and INP_KEY_L3 */
    return "INP_KEY_F13/INP_KEY_L3";
  case INP_KEY_F14: /* and INP_KEY_L4 */
    return "INP_KEY_F14/INP_KEY_L4";
  case INP_KEY_F15: /* and INP_KEY_L5 */
    return "INP_KEY_F15/INP_KEY_L5";
  case INP_KEY_F16: /* and INP_KEY_L6 */
    return "INP_KEY_F16/INP_KEY_L6";
  case INP_KEY_F17: /* and INP_KEY_L7 */
    return "INP_KEY_F17/INP_KEY_L7";
  case INP_KEY_F18: /* and INP_KEY_L8 */
    return "INP_KEY_F18/INP_KEY_L8";
  case INP_KEY_F19: /* and INP_KEY_L9 */
    return "INP_KEY_F19/INP_KEY_L9";
  case INP_KEY_F20: /* and INP_KEY_L10 */
    return "INP_KEY_F20/INP_KEY_L10";
  case INP_KEY_F21: /* and INP_KEY_R1 */
    return "INP_KEY_F21/INP_KEY_R1";
  case INP_KEY_F22: /* and INP_KEY_R2 */
    return "INP_KEY_F22/INP_KEY_R2";
  case INP_KEY_F23: /* and INP_KEY_R3 */
    return "INP_KEY_F23/INP_KEY_R3";
  case INP_KEY_F24: /* and INP_KEY_R4 */
    return "INP_KEY_F24/INP_KEY_R4";
  case INP_KEY_F25: /* and INP_KEY_R5 */
    return "INP_KEY_F25/INP_KEY_R5";
  case INP_KEY_F26: /* and INP_KEY_R6 */
    return "INP_KEY_F26/INP_KEY_R6";
  case INP_KEY_F27: /* and INP_KEY_R7 */
    return "INP_KEY_F27/INP_KEY_R7";
  case INP_KEY_F28: /* and INP_KEY_R8 */
    return "INP_KEY_F28/INP_KEY_R8";
  case INP_KEY_F29: /* and INP_KEY_R9 */
    return "INP_KEY_F29/INP_KEY_R9";
  case INP_KEY_F30: /* and INP_KEY_R10 */
    return "INP_KEY_F30/INP_KEY_R10";
  case INP_KEY_F31: /* and INP_KEY_R11 */
    return "INP_KEY_F31/INP_KEY_R11";
  case INP_KEY_F32: /* and INP_KEY_R12 */
    return "INP_KEY_F32/INP_KEY_R12";
  case INP_KEY_F33: /* and INP_KEY_R13 */
    return "INP_KEY_F33/INP_KEY_R13";
  case INP_KEY_F34: /* and INP_KEY_R14 */
    return "INP_KEY_F34/INP_KEY_R14";
  case INP_KEY_F35: /* and INP_KEY_R15 */
    return "INP_KEY_F35/INP_KEY_R15";
  default:
    if (keysym <= 0xff && isprint(keysym)) {
      static char str[4];
      sprintf (str, "'%c'", keysym);
      return str;
    } else {
      return "UNKNOWN";
    }
  }
}

void main(int argc, char ** argv)
{
  INP_DEV_ID app_dev;
  MSG_MBOX_ID app_mbox;
  MAUI_MSG mess;
  char *mboxname="inbox";
  error_code ec;
  BOOLEAN unknown = TRUE;
  int i;
  char devname[CDB_MAX_DNAME];

  devname[0] = '\0';

  /* parse command line args */
  i = 1;
  while (i < argc) {
	if (*argv[i] == '-') {
	  char *ptr = argv[i];
	  char *str = "-i=";
	  if (!strncmp(ptr,str,strlen(str)))
		{
		  /* set inpdev name */
		  strcpy(devname,argv[i] + 3);
		}
	  else {
		fprintf(stderr, "Usage: %s [mboxname] -i=<inputdevname>\n",argv[0]);
		exit(SUCCESS);
	  }
	} else
	  mboxname = argv[i];

    i++;
  }
  
  /* Install signal handler */
  intercept (signal_handler);

  maui_init();
  
  if ((ec = msg_create_mbox(&app_mbox, mboxname, 100, sizeof (mess),
                            MEM_ANY)) != SUCCESS) {
    fprintf(stderr, "Could not create mailbox '%s'\n", mboxname);
    maui_term();
    exit(ec);
  }
  
  /* If now name was specified on the command line, get it from the CDB */
  if (devname[0] == '\0') {
    cdb_get_ddr(CDB_TYPE_REMOTE, 1, devname, NULL);
  }
  
  fprintf(stderr,"Opening device '%s'\n",devname);
  if ((ec = inp_open_dev(&app_dev, app_mbox, devname)) !=
      SUCCESS) {
    fprintf(stderr, "Could not open '%s'\n", devname);
    msg_close_mbox(app_mbox);
    maui_term();
    exit(ec);
  }
  
  fprintf(stderr, "Send signal to '%s' to end test\n", argv[0]);
  fprintf(stderr, "Expected device id 0x%x\n", app_dev);
  fprintf(stderr,"+------------------------------------------------------+\n");

  while (!done) {
    ec = msg_read(app_mbox, &mess, MSG_TYPE_ANY, MSG_BLOCK);
    switch (mess.any.type)
      {
      case MSG_TYPE_NONE:
        fprintf(stderr, "\n***No message in maibox***\n");
        break;
      case MSG_TYPE_PTR:
        fprintf(stderr, "Device type:  +++ Pointer +++  Device ID:   0x%x\n",
                mess.ptr.device_id);
        if (mess.ptr.device_id != app_dev)
          fprintf(stderr, "\n**** Invalid Device ID ****\n\n");
        fprintf(stderr, "| Sub-type: 0x%x\n",mess.key.subtype);
	    unknown = TRUE;
		if ( mess.ptr.subtype & INP_PTR_MOVE ) 
		{
          fprintf(stderr, "|                  INP_PTR_MOVE\n");
		  unknown = FALSE;
		}
		if ( mess.ptr.subtype & INP_PTR_DOWN ) 
		{
          fprintf(stderr, "|                  INP_PTR_DOWN\n");
		  unknown = FALSE;
		}
		if ( mess.ptr.subtype & INP_PTR_UP ) 
		{
          fprintf(stderr, "|                  INP_PTR_UP\n");
		  unknown = FALSE;
		}
		if ( unknown == TRUE )
		{
          fprintf(stderr, "|                  !!!Unknown!!!\n");
		}
        fprintf(stderr, "| Button changed: %d\n",mess.ptr.button);
        fprintf(stderr, "| Button status %d (0x%x)\n",
                mess.ptr.button_state, mess.ptr.button_state);
        fprintf(stderr, "| New position (%d,%d)\n",
                mess.ptr.position.x, mess.ptr.position.y);
        fprintf(stderr, "| Simulating keysym: %s (0x%x)\n",
                get_name(mess.ptr.keysym), mess.ptr.keysym);
        /* quit if both buttons are pressed */
        if ((mess.ptr.button_state & 3) == 3)
          done = TRUE;
        break;
      case MSG_TYPE_KEY:
        fprintf(stderr, "Device type: +++ Key +++ Device ID:   0x%x\n",
                mess.key.device_id);
        if (mess.key.device_id != app_dev)
          fprintf(stderr, "\n**** Invalid Device ID ****\n\n");
        fprintf(stderr, "| Sub-type: 0x%x\n",mess.key.subtype);
	    unknown = TRUE;
		if ( mess.key.subtype & INP_KEY_DOWN ) 
		{
          fprintf(stderr, "|                  INP_KEY_DOWN\n");
		  unknown = FALSE;
		}
		if ( mess.key.subtype & INP_KEY_UP ) 
		{
          fprintf(stderr, "|                  INP_KEY_UP\n");
		  unknown = FALSE;
		}
		if ( mess.key.subtype & INP_KEYMOD_DOWN ) 
		{
          fprintf(stderr, "|                  INP_KEYMOD_DOWN\n");
		  unknown = FALSE;
		}
		if ( mess.key.subtype & INP_KEYMOD_UP ) 
		{
          fprintf(stderr, "|                  INP_KEYMOD_UP\n");
		  unknown = FALSE;
		}
		if ( unknown == TRUE )
		{
          fprintf(stderr, "|                  !!!Unknown!!!\n");
		}
        fprintf(stderr, "| Keysym received: %s (0x%x)\n",
                get_name(mess.key.keysym), mess.key.keysym);
        fprintf(stderr, "| Key modifiers: 0x%x\n",mess.key.key_modifiers);
#define TEST_MOD(def) (mess.key.key_modifiers & (def) ? 'x' : ' ')
        fprintf(stderr, "|   Shft CapL Ctrl Alt  Meta Num  Scrl\n");
        fprintf(stderr, "|   L R  L R  L R  L R  L R  Lock Lock\n");
        fprintf(stderr, "|   %1c %1c  %1c %1c  %1c %1c  %1c %1c  %1c %1c   %1c    %1c\n",
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
      default:
        fprintf(stderr, "\nUnknown device message type %d\n",
                mess.any.type);
        break;
      }
    fprintf(stderr,"+------------------------------------------------------+\n");
  }
  
  inp_close_dev(app_dev);
  msg_close_mbox(app_mbox);
  maui_term();
  
  exit(SUCCESS);
}
