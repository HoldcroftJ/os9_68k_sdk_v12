
/*******************************************************************************
 * FILENAME : procmsg.c
 *
 * DESCRIPTION :
 *
 *	This file contains functions for processing DEVICE commands
 *
 * COPYRIGHT:
 *
 *	This source code is the proprietary confidential property of Microware
 *	Systems Corporation, and is provided to licensee solely for documentation
 *	and educational purposes. Reproduction, publication, or distribution in
 *	form to any party other than the licensee is strictly prohibited.
 *
 * EDITION  DATE      WHO  DETAIL
 *       1  9/01/95   srm  modified from GENREM
 */

#define _PROCMSG_C

#include "_key.h"
#include <string.h>

/*******************************************************************************
 * LOCAL MACROS
 *******************************************************************************/
/* this macro assume that mink <= maxk has been checked already */
#define RANGE_CHECK(mink,maxk,minr,maxr) ((mink >= minr) && (maxk <= maxr))
#define LIMIT(target, min, max) {if (target < min) target = min; else if (target > max) target = max;}

/*******************************************************************************
 * LOCAL PROTOTYPES
 *******************************************************************************/
static error_code cmd_get_dev_cap(PMEM *pmem, MSG_GET_DEV_CAP_REPLY *rmsg);
static error_code cmd_get_dev_status(PMEM *pmem, MSG_GET_DEV_STATUS_REPLY *rmsg);
static error_code cmd_set_ptr_pos(PMEM *pmem, MSG_SET_PTR_POS *msg);
static error_code cmd_set_sim_meth(PMEM *pmem, MSG_SET_SIM_METH *msg);
static error_code cmd_set_ptr_limit(PMEM *pmem, MSG_SET_PTR_LIMIT *msg);
static error_code cmd_set_msg_callback(PMEM *pmem, MSG_SET_MSG_CALLBACK *msg);
static error_code cmd_set_msg_mask(PMEM *pmem, MSG_SET_MSG_MASK *msg);
static error_code cmd_reserve_key(PMEM *pmem, MSG_RESERVE_KEY *msg);
static error_code cmd_release_key(PMEM *pmem, MSG_RELEASE_KEY *msg);
static BOOLEAN cmd_check_keys(PMEM *pmem, MSG_CHECK_KEYS *msg);


/*******************************************************************************
 * mppm_process_msg()
 *******************************************************************************/
error_code mppm_process_msg(MP_MPPM *mppm, MP_DEV_MSG *cmd_msg, MP_DEV_MSG **reply_msg)
{
  PMEM *pmem = mppm->pmod_mem;
  MP_DEV_MSG *rmsg = *reply_msg = &(pmem->xmsg);
  error_code err;
  
  /* init the reply message with the command message */
  *rmsg = *cmd_msg;
  
  switch (cmd_msg->any.cmd)
	{
	case CMD_GET_DEV_CAP:
	  rmsg->get_dev_cap_reply.dcom.cmd = CMD_GET_DEV_CAP_REPLY;
	  rmsg->get_dev_cap_reply.error =
		err = cmd_get_dev_cap(pmem, &rmsg->get_dev_cap_reply);
	  break;
	  
	case CMD_GET_DEV_STATUS:
	  rmsg->get_dev_status_reply.dcom.cmd = CMD_GET_DEV_STATUS_REPLY;
	  rmsg->get_dev_status_reply.error =
		err = cmd_get_dev_status(pmem, &rmsg->get_dev_status_reply);
	  break;
	  
	case CMD_SET_PTR_POS:
	  rmsg->set_ptr_pos_reply.dcom.cmd = CMD_SET_PTR_POS_REPLY;
	  rmsg->set_ptr_pos_reply.error =
		err = cmd_set_ptr_pos(pmem, &cmd_msg->set_ptr_pos);
	  break;
	  
	case CMD_SET_SIM_METH:
	  rmsg->set_sim_meth_reply.dcom.cmd = CMD_SET_SIM_METH_REPLY;
	  rmsg->set_sim_meth_reply.error =
		err = cmd_set_sim_meth(pmem, &cmd_msg->set_sim_meth);
	  break;
	  
	case CMD_SET_PTR_LIMIT:
	  rmsg->set_ptr_limit_reply.dcom.cmd = CMD_SET_PTR_LIMIT_REPLY;
	  rmsg->set_ptr_limit_reply.error =
		err = cmd_set_ptr_limit(pmem, &cmd_msg->set_ptr_limit);
	  break;
	  
	case CMD_SET_MSG_CALLBACK:
	  rmsg->set_msg_callback_reply.dcom.cmd = CMD_SET_MSG_CALLBACK_REPLY;
	  rmsg->set_msg_callback_reply.error =
		err = cmd_set_msg_callback(pmem, &cmd_msg->set_msg_callback);
	  break;
	  
	case CMD_SET_MSG_MASK:
	  rmsg->set_msg_mask_reply.dcom.cmd = CMD_SET_MSG_MASK_REPLY;
	  rmsg->set_msg_mask_reply.error =
		err = cmd_set_msg_mask(pmem, &cmd_msg->set_msg_mask);
	  break;
	  
	case CMD_RESERVE_KEY:
	  rmsg->reserve_key_reply.dcom.cmd = CMD_RESERVE_KEY_REPLY;
	  rmsg->reserve_key_reply.error =
		err = cmd_reserve_key(pmem, &cmd_msg->reserve_key);
	  break;
	  
	case CMD_RELEASE_KEY:
	  rmsg->release_key_reply.dcom.cmd = CMD_RELEASE_KEY_REPLY;
	  rmsg->release_key_reply.error =
		err = cmd_release_key(pmem, &cmd_msg->release_key);
	  break;
	  
	case CMD_CHECK_KEYS:
	  rmsg->check_keys_reply.dcom.cmd = CMD_CHECK_KEYS_REPLY;
	  rmsg->check_keys_reply.present = cmd_check_keys(pmem, &cmd_msg->check_keys);
	  rmsg->check_keys_reply.error = err = SUCCESS;
	  break;
	  
	default:
	  rmsg->badack_reply.dcom.cmd = CMD_BADACK_REPLY;
	  rmsg->badack_reply.error = err = EOS_MAUI_BADACK;
	  break;
	}
  
  return err;
}


static error_code cmd_get_dev_cap(PMEM *pmem, MSG_GET_DEV_CAP_REPLY *rmsg)
{
  INP_DEV_CAP *cap = &rmsg->cap;
  
  cap->ptr_type      = DEV_CAP_PTR_TYPE;
  cap->ptr_buttons   = DEV_CAP_PTR_BUTTONS;
  cap->func_keys     = DEV_CAP_FUNC_KEYS;
  cap->key_modifiers = DEV_CAP_KEY_MODIFIERS;
  cap->key_cap       = DEV_CAP_KEY_CAP;
  
  return SUCCESS;
}


static error_code cmd_get_dev_status(PMEM *pmem, MSG_GET_DEV_STATUS_REPLY *rmsg)
{
  memcpy (&rmsg->status, &pmem->status, sizeof (INP_DEV_STATUS));
  
  return SUCCESS;
}


static error_code cmd_set_ptr_pos(PMEM *pmem, MSG_SET_PTR_POS *msg)
{
  INP_DEV_STATUS *status = &pmem->status;
  
  status->ptr_cur.x = msg->position.x;
  LIMIT (status->ptr_cur.x, status->ptr_min.x, status->ptr_max.x);
  status->ptr_cur.y = msg->position.y;
  LIMIT (status->ptr_cur.y, status->ptr_min.y, status->ptr_max.y);
  
  return SUCCESS;
}


static error_code cmd_set_sim_meth(PMEM *pmem, MSG_SET_SIM_METH *msg)
{
  INP_DEV_STATUS *status = &pmem->status;
  
  switch (msg->sim_meth) {
  case INP_SIM_PTR: 
  case INP_SIM_KEY:
    status->sim_meth = msg->sim_meth;
	status->speed.x = msg->speed.x;
	status->speed.y = msg->speed.y;
	break;

  case INP_SIM_NATIVE: 
    status->sim_meth = DEFAULT_SIM_METH;
	status->speed.x = DEFAULT_SPEED_X;
	status->speed.y = DEFAULT_SPEED_Y;
	break;

  default:
	return EOS_MAUI_BADVALUE;
  }
  
  memcpy (&status->button_map, &msg->button_map, sizeof(status->button_map));
  
  return SUCCESS;
}

static error_code cmd_set_ptr_limit(PMEM *pmem, MSG_SET_PTR_LIMIT *msg)
{
  INP_DEV_STATUS *status = &pmem->status;
  
  status->ptr_min.x = msg->ptr_min.x;
  status->ptr_max.x = msg->ptr_max.x;
  LIMIT (status->ptr_cur.x, status->ptr_min.x, status->ptr_max.x);
  
  status->ptr_min.y = msg->ptr_min.y;
  status->ptr_max.y = msg->ptr_max.y;
  LIMIT (status->ptr_cur.y, status->ptr_min.y, status->ptr_max.y);
  
  return SUCCESS;
}

static error_code cmd_set_msg_callback(PMEM *pmem, MSG_SET_MSG_CALLBACK *msg)
{
  INP_DEV_STATUS *status = &pmem->status;
  
  status->callback = msg->callback;
  
  return SUCCESS;
}


static error_code cmd_set_msg_mask(PMEM *pmem, MSG_SET_MSG_MASK *msg)
{
  INP_DEV_STATUS *status = &pmem->status;
  
  status->write_mask = msg->write_mask;
  
  return SUCCESS;
}


static error_code cmd_reserve_key(PMEM *pmem, MSG_RESERVE_KEY *msg)
{
  u_int16 i,j;
  
  MP_DEV *mp_dev;
  
  /* do we have room for this */
  if (pmem->num_reserve == MAX_RESERVES)
	return EOS_MAUI_NOHWSUPPORT;
  
  
  mp_dev = msg->dcom.dev_id;
  
  /* are any of the requested entries taken */
  for (i = 0; i < pmem->num_reserve && pmem->reserve[i].keysym <= msg->key; i++)
	{
	  if(pmem->reserve[i].keysym == msg->key)
        {
		  if(pmem->reserve[i].mp_dev != mp_dev)
			return EOS_MAUI_ISRESERVED;
		  else  /* It is already reserved by us */
			return SUCCESS;
        }
    }
  
  /* make a space in the list for the new entry */
  for(j=pmem->num_reserve;j>i;j--)
	pmem->reserve[j]=pmem->reserve[j-1];
  pmem->reserve[i].keysym=msg->key;
  pmem->reserve[i].mp_dev=mp_dev;
  
  pmem->num_reserve++;
  
  return SUCCESS;
  
}


static error_code cmd_release_key(PMEM *pmem, MSG_RELEASE_KEY *msg)
{
  u_int16 i,j;
  MP_DEV *mp_dev;
  
  mp_dev = msg->dcom.dev_id;
  
  /* Looking for the proper key that belongs to us */
  for (i = 0; i < pmem->num_reserve && pmem->reserve[i].keysym <= msg->key; i++)
    {
	  if(pmem->reserve[i].keysym == msg->key)
        {
		  if(pmem->reserve[i].mp_dev != mp_dev)
			return EOS_MAUI_NOTRESERVED;
		  else  /* It is  reserved by us */
            {
			  pmem->num_reserve--;
			  
			  /* compress the list to remove the old entry */
			  for(j=i; j< pmem->num_reserve;j++)
				pmem->reserve[j]=pmem->reserve[j+1];
			  
			  /* clear out the last, unused entry */
			  pmem->reserve[pmem->num_reserve].keysym=0;
			  pmem->reserve[pmem->num_reserve].mp_dev=NULL;
			  return SUCCESS;
            }
        }
    }
  return EOS_MAUI_NOTRESERVED;
}


static BOOLEAN cmd_check_keys(PMEM *pmem, MSG_CHECK_KEYS *msg)
{
  wchar_t min_key = msg->min_key;
  wchar_t max_key = msg->max_key;
  
  /* ASCII + DEL */
  if (RANGE_CHECK(min_key, max_key, 0x20, 0x7f))
	return TRUE;
  
  /* 4-Way */
  if (RANGE_CHECK(min_key, max_key, INP_KEY_CUR_L, INP_KEY_CUR_D))
	return TRUE;
  
  /* Control-chars */
  if (RANGE_CHECK(min_key, max_key, 0x01, 0x1f))
	return TRUE;

  return FALSE;
}
