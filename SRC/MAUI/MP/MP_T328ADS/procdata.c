/*****************************************************************************
** ID:     @(#) procdata.c 1.1@(#)
** Date:   1/19/96
******************************************************************************
 *  This file contains functions for processing device data
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
**	 1 01/18/95 created--copied from Wirlwind and modeled PHILMOUS   rws    **
*****************************************************************************/

#define _PROCDATA_C

#include "_key.h"
#include <MAUI/maui_inp.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


/*******************************************************************************
 * LOCAL MACROS
 *******************************************************************************/
#define LIMIT(target, min, max) \
	{if (target < min) target = min; else if (target > max) target = max;}
#define INC_TO_0(target,offset) if((target+=offset) > -offset) target = 0
#define DEC_TO_0(target,offset) if((target-=offset) < offset) target = 0


/*******************************************************************************
 * LOCAL PROTOTYPES
 *******************************************************************************/
/* static void build_button_keymsg (MP_MPPM *mppm, MSG_KEY *kmsg, wchar_t keysym); */


/*******************************************************************************
 * mppm_process_data() -- interpret device data
 *******************************************************************************/
error_code mppm_process_data
(
	MP_MPPM *mppm, 			/* ptr to this module's statics */
	u_char **bufptr,		/* SCF raw data contained in buffer */
	size_t *buf_size, 		/* number of bytes available in buffer */
	MSG_MBOX_ID *mbox_id,	/* mailbox id (don't change contents) */
	INP_MSG **inp_msg		/* reply message placed in inp_msg */
)
{
	u_char*			buf = *bufptr;
	PMEM           *pmem = mppm->pmod_mem;  /* fast pointer to pmem block */
	INP_DEV_STATUS *status = &pmem->status; /* fast pointer to status block */
	MSG_PTR        *pmsg;                   /* fast pointer to pointer messages */


/***********************************************************************
 * build a touchpad packet in the packet buffer
 *
 *	Touchpad data format (from /MWOS/OS9/SRC/IO/SCF/DRVR/MC328ADS/TOUCHPAD/
 *            touchpad.h):
 *
 *						_bit_
 *	_byte_	d7	d6	d5	d4	d3	d2	d1	d0
 *  Byte 0  1   0   0   0   0   0   bt1 bt0
 *  Byte 1  0   0   Y9  Y8  Y7  X9  X8  X7
 *  Byte 2  0   X6  X5  X4  X3  X2  X1  X0
 *  Byte 3  0   Y6  Y5  Y4  Y3  Y2  Y1  Y0
 *
 *  bt1 bt0 = TOUCHPAD state: 01 = Pointer Down, 11 = Pointer Move,
 *                            10 = Pointer Up,   00 = No Touch (Invalid)
 *
 *  X9..X0: x-pixel location of touch (between 0 and X_PIXELS-1)
 *  Y9..Y0: y-pixel location of touch (between 0 and Y_PIXELS-1)
 ***********************************************************************/

	/* If on 1st byte of packet, advance 1 byte at a time until we get a
	 * good start byte (bit 7 set to 1), note: bytes before the start byte
	 * are thrown away.
	 */
	if (pmem->pktcnt == 0) {
		while ( *buf_size && !((*buf)&(1<<7)) ) {
			buf++;			/* advance buffer pointer */
			(*buf_size)--;  /* decrement buffer pointer */
		}
	}

	/* fill pktbuf */
	while (*buf_size && pmem->pktcnt < 4) {
		pmem->pktbuf[pmem->pktcnt++] = *buf++;
		(*buf_size)--;
	}

	/* if the packet is not complete, leave until it is */
	if (pmem->pktcnt < 4) {
		*inp_msg = NULL;	/* make mauip get more bytes from SCF device */
		*bufptr = buf;
		return (SUCCESS);
	}

	/* start with a clean slate */
	pmem->pktcnt = 0; 	/* reset the packet counter for next time */
	*inp_msg = NULL; 	/* clear the return message handle */

/*******************************************************************************
 * decode the touchpad data packet
 *******************************************************************************/

	if (pmem->pktbuf[0] & PKTBUF_PM_BYTE0)
	{
		/* correctly format the touched position */
		status->ptr_cur.x = (GFX_POS)
			(((pmem->pktbuf[1] & 0x07) << 7) | (pmem->pktbuf[2] & 0x7f));

		status->ptr_cur.y = (GFX_POS)
			(((pmem->pktbuf[1] & 0x38) << 4) | (pmem->pktbuf[3] & 0x7f));

/*******************************************************************************
 * build message packets based on the touchpad data packet
 *******************************************************************************/

		pmsg = &((*inp_msg) = (INP_MSG*)&(pmem->imsg))->ptr;
		pmsg->com.type = MSG_TYPE_PTR;
		pmsg->com.callback = status->callback;
		pmsg->device_id = (INP_DEV_ID) mppm->mp_dev_head->device_id;

		if (pmem->pktbuf[0] == PKTBUF_PD_BYTE0) {
		  /* send pointer down message */
		  pmsg->subtype = INP_PTR_DOWN;
		  status->button_state = pmsg->button_state = 1;
		  pmsg->button = 1;
		} else if (pmem->pktbuf[0] == PKTBUF_PM_BYTE0) {
		  /* send pointer move message */
		  pmsg->subtype = INP_PTR_MOVE;
		  pmsg->button_state = 1; /* no need to update status->button_state */
		  pmsg->button = 0;
		} else {
		  /* send pointer up message */
		  pmsg->subtype = INP_PTR_UP;
		  status->button_state = pmsg->button_state = 0;
		  pmsg->button = 1;
		}

		pmsg->position.x = status->ptr_cur.x;
		pmsg->position.y = status->ptr_cur.y;
		pmsg->keysym = 0;
	}

#if 0

	/* if doing cursor simulation we need to translate the messages */
	if (status->sim_meth == INP_SIM_KEY) 
	{
		pmsg = &pmem->imsg.ptr;
		if (pmsg->subtype == INP_PTR_DOWN)
			if (status->button_map[pmsg->button-1])
				build_button_keymsg (mppm, (MSG_KEY*)pmsg, 
							status->button_map[pmsg->button-1]);
	}

#endif

	*bufptr = buf;
	return (SUCCESS);
}


#if 0

static void build_button_keymsg 
(
	MP_MPPM *mppm, 
	MSG_KEY *kmsg, 
	wchar_t keysym
)
{
  kmsg->com.type = MSG_TYPE_KEY;
  kmsg->com.callback = 0;
  kmsg->device_id = (INP_DEV_ID) mppm;
  kmsg->keysym = keysym;
  kmsg->key_modifiers = 0;

  return;
}

#endif

