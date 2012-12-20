
/*******************************************************************************
 * FILENAME : procdata.c
 *
 * DESCRIPTION :
 *
 *  This file contains functions for processing device data
 *
 * COPYRIGHT:
 *
 *  This source code is the proprietary confidential property of Microware
 *  Systems Corporation, and is provided to licensee solely for documentation
 *  and educational purposes. Reproduction, publication, or distribution in
 *  form to any party other than the licensee is strictly prohibited.
 *
 * EDITION  DATE      WHO  DETAIL
 *       1  01/22/95  srm  create
 */

#define _PROCDATA_C

#include "_key.h"
#include <MAUI/maui_inp.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


/*******************************************************************************
 * LOCAL MACROS
 *******************************************************************************/
#define P_LIMIT(target, min, max) {if (target < min) target = min; else if (target > max) target = max;}
#define INC_TO_0(target,offset) if((target+=offset) > -offset) target = 0
#define DEC_TO_0(target,offset) if((target-=offset) < offset) target = 0


/*******************************************************************************
 * LOCAL PROTOTYPES
 *******************************************************************************/
static void build_button_keymsg (MP_MPPM *mppm, MSG_KEY *kmsg, wchar_t keysym);
static error_code build_ptr_keymsg (MP_MPPM *mppm, MSG_KEY *kmsg);


/*******************************************************************************
 * mppm_process_data()
 *******************************************************************************/
/* note that mbox_id is only changed if this key has been
   reserved. Since the mouse has no keys, mbox_id will never be
   changed*/
error_code mppm_process_data(MP_MPPM *mppm, u_char **buf, size_t *buf_size, MSG_MBOX_ID *mbox_id, INP_MSG **inp_msg)
{
  PMEM           *pmem = mppm->pmod_mem;   /* fast pointer to pmem block */
  INP_DEV_STATUS *status = &pmem->status;  /* fast pointer to status block */
  u_int8         new_button_state;         /* temp var of new button state */
  u_int8         button_change;            /* temp var of old to new changes */
  MSG_PTR        *pmsg;                    /* fast pointer to pointer messages */
  GFX_POS        old_x, old_y;             /* save off old X,Y position */
  GFX_OFFSET     new_x_delta, new_y_delta; /* save X,Y change for key messages */
  int16          cnt;                      /* to walk imsg buffs */
  int16          moves;                    /* store number of imsg[] blocks to move */
  u_int16        tmp;
  int            raw_mode = (status->sim_meth == RAW_MODE);
  GFX_POS        new_x, new_y;

/*******************************************************************************
 * process any unfinished messages
 *******************************************************************************/

  /* see if we have any unfinished key move messages */
  if (pmem->offset_x || pmem->offset_y) {
    if (build_ptr_keymsg (mppm,
						  &((*inp_msg) = &(pmem->imsg[pmem->imsgcnt]))->key)
		== EOS_UNFINISHED)
      return EOS_UNFINISHED;

    return ((pmem->imsgcnt > 0) ? EOS_UNFINISHED : SUCCESS);
  }

  /* see if we have any other unfinished messages */
  if (pmem->imsgcnt > 0) {

	--pmem->imsgcnt;
    *inp_msg = &(pmem->imsg[pmem->imsgcnt]);

    return ((pmem->imsgcnt > 0) ? EOS_UNFINISHED : SUCCESS);
  }



/*******************************************************************************
 * build a mouse packet in the packet buffer
 *******************************************************************************/

  /* If on 1st byte of packet, advance 1 byte at a time until we get a
     good start byte (bit 6 set to 1) */
  if (pmem->pktcnt == 0) {
    while (*buf_size && !(*(*buf)&1<<SYNC_BYTE_SHIFT)) {
      (*buf)++;                 /* advance buffer pointer */
      (*buf_size)--;            /* decrement buffer counter */
    }
  }
  

  /* fill pktbuf */
  while (*buf_size && pmem->pktcnt < NUM_PKT_BUF) {
    pmem->pktbuf[pmem->pktcnt++] = *(*buf)++;
    (*buf_size)--;
  }

  /* if the packet is not complete, leave until it is */
  if (pmem->pktcnt < NUM_PKT_BUF) {
    *inp_msg = NULL;
    return SUCCESS;
  }

  /* start with a clean slate */
  pmem->pktcnt = 0; /* reset the packet counter for next time */
  pmem->imsgcnt = 0;  /* reset the count of input messages */
  *inp_msg = NULL;   /* clear the return message handle */


/*******************************************************************************
 * decode the mouse data packet
 *******************************************************************************/

  /*
  ** Hampton Communication Format
  *
  *         d7  d6  d5  d4  d3  d2  d1  d0
  * Byte 0  1   dn  x11 x10 x9 y11 y10  y9
  * Byte 1  0   x8  x7  x6  x5  x4  x3  x2
  * Byte 2  0   y8  y7  y6  y5  y4  y3  y2
  * Byte 3  0   0   0   0   x1  x0  y1  y0
  *
  *  dn - 1 = button down, 0 = button up
  */

  /* save off the old position */
  old_x = status->ptr_cur.x;

  /* compute the new x position */
  tmp = ((u_int16)(pmem->pktbuf[0] & 0x38)) << 6;
  tmp |= ((u_int16)pmem->pktbuf[1]) << 2;
  tmp |= ((u_int16)pmem->pktbuf[3]) >> 2;
  new_x = tmp;

  /* convert to screen coordinate */
  if (!raw_mode)
    {
      /* convert to screen pixels */
      new_x = (((int32)new_x - (int32)pmem->cal_x) * (int32)pmem->screen_width) / (int32)pmem->cal_width;

      /* keep it in bounds */
      P_LIMIT (new_x, status->ptr_min.x, status->ptr_max.x);
      status->ptr_cur.x = new_x;
    }

  /* compute the real change */
  new_x_delta = status->ptr_cur.x - old_x;

  /* save off the old position */
  old_y = status->ptr_cur.y;
  
  /* compute the new y position */
  tmp = ((u_int16)(pmem->pktbuf[0] & 0x07)) << 9;
  tmp |= ((u_int16)pmem->pktbuf[2]) << 2;
  tmp |= ((u_int16)pmem->pktbuf[3]) & 0x03;
  new_y = tmp;

  /* convert to screen coordinates */
  if (!raw_mode)
    {
      /* convert to screen pixels */
      new_y = (((int32)new_y - (int32)pmem->cal_y) * (int32)pmem->screen_height) / (int32)pmem->cal_height;

      /* keep it in bounds */
      P_LIMIT (new_y, status->ptr_min.y, status->ptr_max.y);
      status->ptr_cur.y = new_y;
    }

  /* compute the real change */
  new_y_delta = status->ptr_cur.y - old_y;

  /* grab the new button state */
  new_button_state = ((pmem->pktbuf[0] >> 6) & 0x01);
  button_change = status->button_state ^ new_button_state;
  status->button_state = new_button_state;


/*******************************************************************************
 * build messages packets based on the mouse data packet
 *******************************************************************************/

  /* Convert button number to button bit offset */
#define BUT_NTOB(b) (1<<(b-1))

  if ((new_x_delta != 0) || (new_y_delta != 0) || (raw_mode)) {
    /* generate move message, but only if  */
    pmsg = &((*inp_msg) = &(pmem->imsg[pmem->imsgcnt]))->ptr;
	pmem->imsgcnt++;
    pmsg->com.type = MSG_TYPE_PTR;
    pmsg->com.callback = status->callback;
	pmsg->device_id = mppm->mp_dev_head->device_id;
    pmsg->subtype = INP_PTR_MOVE;
    pmsg->button = 0;
    pmsg->button_state = new_button_state;
    pmsg->position.x = new_x;
    pmsg->position.y = new_y;
    pmsg->keysym = 0;
  }

  /* should a button 1 message be generated? */
  if (button_change & BUT_NTOB(1)) {
    pmsg = &((*inp_msg) = &(pmem->imsg[pmem->imsgcnt]))->ptr;
	pmem->imsgcnt++;
    pmsg->com.type = MSG_TYPE_PTR;
    pmsg->com.callback = status->callback;
	pmsg->device_id = mppm->mp_dev_head->device_id;
    pmsg->subtype = (new_button_state & BUT_NTOB(1)) ?
      INP_PTR_DOWN : INP_PTR_UP;
    pmsg->button = 1;
    pmsg->button_state = new_button_state;
    pmsg->position.x = new_x;
    pmsg->position.y = new_y;
    pmsg->keysym = 0;
  } 


  /* if doing cursor simulation we need to translate the messages */
  if (status->sim_meth == INP_SIM_KEY) {

    cnt = pmem->imsgcnt;
    while (cnt-- > 0) {

      pmsg = &pmem->imsg[cnt].ptr;

      switch (pmsg->subtype) {

      case INP_PTR_MOVE:

        pmem->offset_x = pmsg->position.x - pmem->last_key_x;
		pmem->offset_y = pmsg->position.y - pmem->last_key_y;

        if (abs (pmem->offset_x) >= status->speed.x ||
            abs (pmem->offset_y) >= status->speed.y) {

          pmem->last_key_x = pmsg->position.x;
          pmem->last_key_y = pmsg->position.y;
          build_ptr_keymsg (mppm, (MSG_KEY*)pmsg);

        } else { /* drop this message */
          
		  pmem->offset_x = pmem->offset_y = 0;       /* clear the offset */

		  /* we know that this is always the top message in the stack,
             so just get rid it it */
		  if (--pmem->imsgcnt > 0) {                 /* Is there another message following it?*/
			(*inp_msg)--;                            /* patch where inp_msg points to */
		  } else {
			*inp_msg = NULL;                         /* clear inp_msg */
			return SUCCESS;                          /* no more data so exit now */
		  }
        }
        break;

      case INP_PTR_UP: /* we don't send key messages on up so drop this message */
		/* we do not know where this is in the message stack */
		
		/* if not top of stack, copy the stack down */
		if (--pmem->imsgcnt != cnt) {
		  moves = pmem->imsgcnt - cnt;   /* how many entries do we need to move */
		  memmove (pmsg+moves-1, *inp_msg, sizeof(INP_MSG) * moves);
		}
		/* now adjust the pointer to the top entry */
		if (pmem->imsgcnt > 0) {                 /* Is there another message following it?*/
		  (*inp_msg)--;                            /* patch where inp_msg points to */
		} else {
		  *inp_msg = NULL;                         /* clear inp_msg */
		  return SUCCESS;                          /* no more data so exit now */
		}
		break;

      case INP_PTR_DOWN:
        if (status->button_map[pmsg->button-1])
          build_button_keymsg (mppm, (MSG_KEY*)pmsg, status->button_map[pmsg->button-1]);
        break;
      }
    }
  }

  --pmem->imsgcnt;
  return ((pmem->imsgcnt > 0 || pmem->offset_x || pmem->offset_y) ? EOS_UNFINISHED : SUCCESS);
}


static void build_button_keymsg (MP_MPPM *mppm, MSG_KEY *kmsg, wchar_t keysym)
{
  INP_DEV_STATUS *status = &((PMEM*)(mppm->pmod_mem))->status; /* fast pointer to status block */

  kmsg->com.type = MSG_TYPE_KEY;
  kmsg->com.callback = status->callback;
  kmsg->subtype = INP_KEY_DOWN|INP_KEY_UP;
  kmsg->device_id = mppm->mp_dev_head->device_id;
  kmsg->keysym = keysym;
  kmsg->key_modifiers = INP_KEY_NOMOD;

  return;
}

static error_code build_ptr_keymsg (MP_MPPM *mppm, MSG_KEY *kmsg)
{
  PMEM *pmem = mppm->pmod_mem;            /* fast pointer to pmem block */
  INP_DEV_STATUS *status = &pmem->status; /* fast pointer to status block */

  kmsg->com.type = MSG_TYPE_KEY;
  kmsg->com.callback = status->callback;
  kmsg->subtype = INP_KEY_DOWN|INP_KEY_UP;
  kmsg->device_id = mppm->mp_dev_head->device_id;
  kmsg->key_modifiers = INP_KEY_NOMOD;

  if (pmem->offset_x >= status->speed.x) {
    DEC_TO_0 (pmem->offset_x, status->speed.x);
    if (pmem->offset_y >= status->speed.y) {
      DEC_TO_0 (pmem->offset_y, status->speed.y);
      kmsg->keysym = INP_KEY_CUR_DR;  /* x>0:R, y>0:D */
    } else if (pmem->offset_y <= -status->speed.y) {
      INC_TO_0 (pmem->offset_y, status->speed.y);
      kmsg->keysym = INP_KEY_CUR_UR;  /* x>0:R, y<0:U */
    } else /* y == 0 */ {
      pmem->offset_y = 0;
      kmsg->keysym = INP_KEY_CUR_R;   /* x>0:R, y=0: */
    }
  } else if (pmem->offset_x <= -status->speed.x) {
    INC_TO_0 (pmem->offset_x, status->speed.x);
    if (pmem->offset_y > status->speed.y) {
      DEC_TO_0 (pmem->offset_y, status->speed.y);
      kmsg->keysym = INP_KEY_CUR_DL;  /* x<0:L, y>0:D */
    } else if (pmem->offset_y <= -status->speed.y) {
      INC_TO_0 (pmem->offset_y, status->speed.y);
      kmsg->keysym = INP_KEY_CUR_UL;  /* x<0:L, y<0:U */
    } else /* y == 0 */ {
      pmem->offset_y = 0;
      kmsg->keysym = INP_KEY_CUR_L;   /* x<0:L, y=0: */
    }
  } else /* x == 0 */ {
	pmem->offset_x = 0;
	if (pmem->offset_y >= status->speed.y) {
	  DEC_TO_0 (pmem->offset_y, status->speed.y);
	  kmsg->keysym = INP_KEY_CUR_D;     /* x=0: , y>0:D */
	} else if (pmem->offset_y <= -status->speed.y) {
	  INC_TO_0 (pmem->offset_y, status->speed.y);
	  kmsg->keysym = INP_KEY_CUR_U;     /* x=0: , y<0:U */
	} else
	  return EOS_MAUI_INTERNAL; /* this can't happen! */
  }

  if (pmem->offset_x || pmem->offset_y)
    return EOS_UNFINISHED;

  return SUCCESS;
}


void update_calibration(PMEM *pmem)
{
  ModData        *mdata;              /* calibration data in module */
  mh_com         *modhead;            /* calibration data module */ 
  char *modname = CALIBRATION_MOD_NAME;
  error_code ec;
  u_int16 attr_rev=0,type_lang=0;

  ec = _os_link(&modname,&modhead,(void**)&mdata, &type_lang,&attr_rev);
  if (ec == SUCCESS) {
	/* module there, check sync */
	if (mdata->sync == HAMPSHIRE_SYNC) {
	  /* read in calibration values from data module */
	  
	  /* read values from calibration data module */
	  pmem->cal_x = mdata->x;
	  pmem->cal_y = mdata->y;
	  pmem->cal_width = mdata->width;
	  pmem->cal_height = mdata->height;
	  pmem->screen_width = mdata->screen_width;
	  pmem->screen_height = mdata->screen_height;

	  pmem->status.ptr_max.x = (GFX_POS)pmem->screen_width - 1;
	  pmem->status.ptr_max.y = (GFX_POS)pmem->screen_height - 1;

	  _os_unlink(modhead);
	}
  }
}
