
/*******************************************************************************
 * Filename : procdata.c
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
 *
 * EDITION  DATE      WHO  DETAIL
 *       1  09/01/95   srm  modified from GENREM
 *       2  06/10/96   srm  update to MAUI 2.0
 *       3  10/18/01   and  CF2898: Added support for vt100 function keys
 */

#define _PROCDATA_C

#include "_key.h"
#include <MAUI/maui_inp.h>
#include <string.h>
#include <errno.h>



/*******************************************************************************
 * LOCAL MACROS
 *******************************************************************************/
#define INCREMENT(target,offset,limit) if ((target += offset) > limit) target = limit
#define DECREMENT(target,offset,limit) if ((target -= offset) < limit) target = limit
#define BETWEEN(key,minr,maxr) ((key >= minr) && (key <= maxr))


/*******************************************************************************
 * LOCAL PROTOTYPES
 *******************************************************************************/
static error_code button_down (MP_MPPM *mppm, MSG_PTR *pmsg, u_int16 button);
static error_code button_up (MP_MPPM *mppm, MSG_PTR *pmsg, u_int16 button);
static void build_keymsg (MP_MPPM *mppm, MSG_MBOX_ID *mbox_id, MSG_KEY *kmsg, wchar_t key);
static MP_DEV* search_reserved (PMEM *pmem, wchar_t key);

/*******************************************************************************
 * mppm_process_data()
 *******************************************************************************/
/* note that mbox_id is only changed if this key has been reserved */
error_code 
mppm_process_data(MP_MPPM *mppm, u_char **buf, size_t *buf_size, MSG_MBOX_ID *mbox_id, INP_MSG **inp_msg)
{
  PMEM           *pmem = mppm->pmod_mem;
  INP_DEV_STATUS *status = &pmem->status;
  wchar_t        key;
  error_code     return_status;
  
  /* see if there is any UNFINISHED business */
  if (pmem->but_up_flag != 0) {
    MSG_PTR *pmsg;
    pmsg = &((*inp_msg) = &(pmem->imsg))->ptr;
    button_up(mppm, pmsg, pmem->but_up_flag);
	return (pmem->pktcnt ? EOS_UNFINISHED : SUCCESS);
  }
  
  
  *inp_msg = NULL;
  return_status = SUCCESS;
  
  /*** simple vt100 ESC parsing, for cursor keys only ***/
  
  /* fill pktbuf with up to NUM_PKT_BUF chars */
  while (*buf_size && pmem->pktcnt < NUM_PKT_BUF) {
    pmem->pktbuf[pmem->pktcnt++] = *(*buf)++;
    (*buf_size)--;
  }

  /* get more data if we don't have enough data for the esc '[' sequence */
  switch (pmem->pktcnt) {
  case 1:
	if (pmem->pktbuf[0] == 0x1b)
	  return SUCCESS; /* wait for more data */
	key = pmem->pktbuf[0];
	pmem->pktcnt = 0;
	break;
  case 2:
	if (pmem->pktbuf[0] == 0x1b && pmem->pktbuf[1] == 0x5b)
	  return SUCCESS; /* wait for more data */
	return_status = EOS_UNFINISHED;
	key = pmem->pktbuf[0];
	pmem->pktbuf[0] = pmem->pktbuf[1];
	pmem->pktcnt--;
	break;
  case 3:
	if (pmem->pktbuf[0] == 0x1b && pmem->pktbuf[1] == 0x5b) {
	  switch (pmem->pktbuf[2]) {
	  case 0x41: key= INP_KEY_CUR_U; pmem->pktcnt = 0; break;  
	  case 0x42: key= INP_KEY_CUR_D; pmem->pktcnt = 0; break;  
	  case 0x43: key= INP_KEY_CUR_R; pmem->pktcnt = 0; break;  
	  case 0x44: key= INP_KEY_CUR_L; pmem->pktcnt = 0; break;
	  default:
		return_status = EOS_UNFINISHED;
		key = pmem->pktbuf[0];
		pmem->pktbuf[0] = pmem->pktbuf[1];
		pmem->pktbuf[1] = pmem->pktbuf[2];
		pmem->pktcnt--;
		break;
	  }
	} else if (pmem->pktbuf[0] == 0x1b && pmem->pktbuf[1] == 0x4f) {
	  switch (pmem->pktbuf[2]) {
	  case 0x50: key= INP_KEY_NUMLCK; pmem->pktcnt = 0; break;  
	  case 0x51: key= 0x2f; pmem->pktcnt = 0; break;  
	  case 0x52: key= 0x2a; pmem->pktcnt = 0; break;  
	  case 0x53: key= 0x2d; pmem->pktcnt = 0; break;
	  default:
		return_status = EOS_UNFINISHED;
		key = pmem->pktbuf[0];
		pmem->pktbuf[0] = pmem->pktbuf[1];
		pmem->pktbuf[1] = pmem->pktbuf[2];
		pmem->pktcnt--;
		break;
	  }
	} else {
	  return_status = EOS_UNFINISHED;
	  key = pmem->pktbuf[0];
	  pmem->pktbuf[0] = pmem->pktbuf[1];
	  pmem->pktbuf[1] = pmem->pktbuf[2];
	  pmem->pktcnt--;
	}
	break;
  case 4:
	if (pmem->pktbuf[0] == 0x1b && pmem->pktbuf[1] == 0x5b &&
	    pmem->pktbuf[3] == 0x7e) {
	  switch (pmem->pktbuf[2]) {
	  case 0x31: key= INP_KEY_INSERT; pmem->pktcnt = 0; break;
	  case 0x32: key= INP_KEY_HOME; pmem->pktcnt = 0; break;
	  case 0x33: key= INP_KEY_PAGE_U; pmem->pktcnt = 0; break;
	  case 0x34: key= INP_KEY_DEL; pmem->pktcnt = 0; break;
	  case 0x35: key= INP_KEY_END; pmem->pktcnt = 0; break;
	  case 0x36: key= INP_KEY_PAGE_D; pmem->pktcnt = 0; break;  
	  default:
		return_status = EOS_UNFINISHED;
		key = pmem->pktbuf[0];
		pmem->pktbuf[0] = pmem->pktbuf[1];
		pmem->pktbuf[1] = pmem->pktbuf[2];
		pmem->pktbuf[2] = pmem->pktbuf[3];
		pmem->pktcnt--;
		break;
	  }
	} else {
	  return_status = EOS_UNFINISHED;
	  key = pmem->pktbuf[0];
	  pmem->pktbuf[0] = pmem->pktbuf[1];
	  pmem->pktbuf[1] = pmem->pktbuf[2];
	  pmem->pktbuf[2] = pmem->pktbuf[3];
	  pmem->pktcnt--;
	}
	break;
  case 5:
	if (pmem->pktbuf[0] == 0x1b && pmem->pktbuf[1] == 0x5b &&
	    pmem->pktbuf[2] == 0x31 && pmem->pktbuf[4] == 0x7e) {
	  switch (pmem->pktbuf[3]) {
	  case 0x31: key= INP_KEY_F1; pmem->pktcnt = 0; break;
	  case 0x32: key= INP_KEY_F2; pmem->pktcnt = 0; break;
	  case 0x33: key= INP_KEY_F3; pmem->pktcnt = 0; break;
	  case 0x34: key= INP_KEY_F4; pmem->pktcnt = 0; break;
	  case 0x35: key= INP_KEY_F5; pmem->pktcnt = 0; break;
	  case 0x37: key= INP_KEY_F6; pmem->pktcnt = 0; break;
	  case 0x38: key= INP_KEY_F7; pmem->pktcnt = 0; break;
	  case 0x39: key= INP_KEY_F8; pmem->pktcnt = 0; break;  
	  default:
		return_status = EOS_UNFINISHED;
		key = pmem->pktbuf[0];
		pmem->pktbuf[0] = pmem->pktbuf[1];
		pmem->pktbuf[1] = pmem->pktbuf[2];
		pmem->pktbuf[2] = pmem->pktbuf[3];
		pmem->pktbuf[3] = pmem->pktbuf[4];
		pmem->pktcnt--;
		break;
	  }
	} else if (pmem->pktbuf[0] == 0x1b && pmem->pktbuf[1] == 0x5b &&
	    pmem->pktbuf[2] == 0x32 && pmem->pktbuf[4] == 0x7e) {
	  switch (pmem->pktbuf[3]) {
	  case 0x30: key= INP_KEY_F9; pmem->pktcnt = 0; break;
	  case 0x31: key= INP_KEY_F10; pmem->pktcnt = 0; break;
	  case 0x33: key= INP_KEY_F11; pmem->pktcnt = 0; break;
	  case 0x34: key= INP_KEY_F12; pmem->pktcnt = 0; break;
	  default:
		return_status = EOS_UNFINISHED;
		key = pmem->pktbuf[0];
		pmem->pktbuf[0] = pmem->pktbuf[1];
		pmem->pktbuf[1] = pmem->pktbuf[2];
		pmem->pktbuf[2] = pmem->pktbuf[3];
		pmem->pktbuf[3] = pmem->pktbuf[4];
		pmem->pktcnt--;
		break;
	  }
	} else {
	  return_status = EOS_UNFINISHED;
	  key = pmem->pktbuf[0];
	  pmem->pktbuf[0] = pmem->pktbuf[1];
	  pmem->pktbuf[1] = pmem->pktbuf[2];
	  pmem->pktbuf[2] = pmem->pktbuf[3];
	  pmem->pktbuf[3] = pmem->pktbuf[4];
	  pmem->pktcnt--;
	}
	break;
  default:
	/* with a three byte buffer you should not be able to get here */
	return EOS_MAUI_INTERNAL;
  }
  
  /* throw away unknown keys */
  if (/* ASCII + DEL */
	  BETWEEN(key, 0x20, 0x7f) ||
	  /* 4-Way */
	  BETWEEN(key, INP_KEY_CUR_L, INP_KEY_CUR_D) ||
	  /* Control-chars */
	  BETWEEN(key, 0x01, 0x1f) ||
	  /* Function keys */
	  BETWEEN(key,INP_KEY_F1, INP_KEY_F12) ||
	  /* "Insert" + "Page Down" */ 
	  ((key == INP_KEY_INSERT) || (key == INP_KEY_HOME) ||
	   (key == INP_KEY_PAGE_U) || (key == INP_KEY_DEL) ||
	   (key == INP_KEY_END) || (key == INP_KEY_PAGE_D))) 
	  {
	  
	  switch (status->sim_meth) {
	  case INP_SIM_PTR:             /* Simulation pointer if necessary */
		{
		  MSG_PTR *pmsg;
		  
		  pmsg = &((*inp_msg) = &(pmem->imsg))->ptr;
		  memset (pmsg, 0, sizeof(MSG_PTR));
		  pmsg->com.type = MSG_TYPE_PTR;
		  pmsg->com.callback = status->callback;
		  pmsg->device_id = mppm->mp_dev_head->device_id;
		  pmsg->subtype = INP_PTR_MOVE;
		  pmsg->keysym = key;
		  
		  switch (key) {
#ifdef CURS_8WAY
		  case INP_KEY_CUR_UR:
			INCREMENT (status->ptr_cur.x, status->speed.x, status->ptr_max.x);
#endif
		  case INP_KEY_CUR_U:
			DECREMENT (status->ptr_cur.y, status->speed.y, status->ptr_min.y);
			pmsg->position.x = status->ptr_cur.x;
			pmsg->position.y = status->ptr_cur.y;
			break;
			
#ifdef CURS_8WAY
		  case INP_KEY_CUR_UL:
			DECREMENT (status->ptr_cur.y, status->speed.y, status->ptr_min.y);
#endif
		  case INP_KEY_CUR_L:
			DECREMENT (status->ptr_cur.x, status->speed.x, status->ptr_min.x);
			pmsg->position.x = status->ptr_cur.x;
			pmsg->position.y = status->ptr_cur.y;
			break;
			
#ifdef CURS_8WAY
		  case INP_KEY_CUR_DR:
			INCREMENT (status->ptr_cur.y, status->speed.y, status->ptr_max.y);
#endif
		  case INP_KEY_CUR_R:
			INCREMENT (status->ptr_cur.x, status->speed.x, status->ptr_max.x);
			pmsg->position.x = status->ptr_cur.x;
			pmsg->position.y = status->ptr_cur.y;
			break;
			
#ifdef CURS_8WAY
		  case INP_KEY_CUR_DL:
			DECREMENT (status->ptr_cur.x, status->speed.x, status->ptr_min.x);
#endif
		  case INP_KEY_CUR_D:
			INCREMENT (status->ptr_cur.y, status->speed.y, status->ptr_max.y);
			pmsg->position.x = status->ptr_cur.x;
			pmsg->position.y = status->ptr_cur.y;
			break;
			
		  default:
			/* check button associations */
			{
			  u_int16 i;
			  
			  for (i = 0; i < INP_MAX_BUTTONS; i++) {
				if (key == status->button_map[i])
				  return button_down (mppm, pmsg, i);
			  }
			}
			
			/* everything else is sent on as a MSG_KEY */
			build_keymsg (mppm, mbox_id, &((*inp_msg) = &(pmem->imsg))->key, key);
			break;
		  }
		  break;
		}
	  case INP_SIM_NATIVE:          /* Use native mode */
	  case INP_SIM_KEY:             /* Simulation key symbols if necessary */
	  default:
		{
		  /* send on the MSG_KEY */
		  build_keymsg (mppm, mbox_id, &((*inp_msg) = &(pmem->imsg))->key, key);
		}
		break;
	  }
	}
  
  return return_status;
}

static error_code button_down (MP_MPPM *mppm, MSG_PTR *pmsg, u_int16 button)
{
  PMEM *pmem = (PMEM*)mppm->pmod_mem;
  INP_DEV_STATUS *status = &pmem->status;
  
  button++;
  
  pmsg->com.type = MSG_TYPE_PTR;
  pmsg->com.callback = status->callback;
  pmsg->device_id = mppm->mp_dev_head->device_id;
  pmsg->subtype = INP_PTR_DOWN;
  pmsg->button_state = (1 << (pmsg->button = button));
  pmsg->position.x = status->ptr_cur.x;
  pmsg->position.y = status->ptr_cur.y;
  
  pmem->but_up_flag = button;
  
  return EOS_UNFINISHED;
}

static error_code button_up (MP_MPPM *mppm, MSG_PTR *pmsg, u_int16 button)
{
  PMEM *pmem = (PMEM*)mppm->pmod_mem;
  INP_DEV_STATUS *status = &pmem->status;
  
  pmsg->com.type = MSG_TYPE_PTR;
  pmsg->com.callback = status->callback;
  pmsg->device_id = mppm->mp_dev_head->device_id;
  pmsg->subtype = INP_PTR_UP;
  pmsg->button = button;
  pmsg->button_state = 0;
  pmsg->position.x = status->ptr_cur.x;
  pmsg->position.y = status->ptr_cur.y;
  
  pmem->but_up_flag = 0;
  
  return SUCCESS;
}

static void build_keymsg (MP_MPPM *mppm, MSG_MBOX_ID *mbox_id, MSG_KEY *kmsg, wchar_t key)
{
  PMEM *pmem = (PMEM*)mppm->pmod_mem;
  INP_DEV_STATUS *status = &pmem->status;
  MP_DEV *mpdev;
  
  memset (kmsg, 0, sizeof (MSG_KEY));
  kmsg->com.type = MSG_TYPE_KEY;
  kmsg->subtype = INP_KEY_DOWN|INP_KEY_UP;
  kmsg->keysym = key;
  kmsg->key_modifiers = INP_KEY_NOMOD;
  
  /* handle key reservation */
  mpdev = search_reserved (pmem, key);
  if (mpdev != NULL) {
    /* reserved */
    kmsg->device_id = mpdev->device_id;
    *mbox_id = mpdev->app_mbox_id;
  } else {
    /* not reserved */
    kmsg->device_id = mppm->mp_dev_head->device_id;
  }
  kmsg->com.callback = status->callback;
  
  return;
}


static MP_DEV* search_reserved (PMEM *pmem, wchar_t key)
{
  u_int16 i;
  
  for (i = 0; i < pmem->num_reserve && pmem->reserve[i].keysym <= key; i++) 
    {
      if (key == pmem->reserve[i].keysym)
        return pmem->reserve[i].mp_dev;
    }
  return NULL;
}

