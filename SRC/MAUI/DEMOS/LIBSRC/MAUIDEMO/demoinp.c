/*******************************************************************************
*
* DESCRIPTION:	Common functions for demo programs. The caller can use demo_init()
*               and demo_term() before calling functions in this file, or else they are 
*		responsible for calling the necessary individual *_init() functions 
*		needed before calling functions in this file.
*
* AUTHOR:		Todd Earles
*
* CREATED ON:	02/06/96
*
* COPYRIGHT:	Copyright 1994 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
*/
#define DEMO_INP
#include "democom.h"

/*******************************************************************************
* NAME:		demo_open_inp_dev()
*
* USAGE:	Open the primary input device.
*/
error_code demo_open_inp_dev(INP_DEV_ID *ret_inpdev, MSG_MBOX_ID mbox)
{
	error_code ec;
	char devname[CDB_MAX_DNAME];

	/* Clear device ID in case we get an error */
	*ret_inpdev = NULL;

	cdb_init();
	/* Get the name of the input device from the CDB */
	if ((ec = cdb_get_ddr(CDB_TYPE_REMOTE, 1, devname, NULL)) != SUCCESS) {
		return demo_error(ec, "Failed to find the input device name in the CDB\n");
	}
	cdb_term();

	/* Open the device */
	if ((ec = inp_open_dev(ret_inpdev, mbox, devname)) != SUCCESS) {
		return demo_error(ec, "Failed to open input device \"%s\"\n", devname);
	}

	return SUCCESS;
}
