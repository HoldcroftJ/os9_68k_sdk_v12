/*****************************************************************************
** ID:     @(#) stat.c 1.4@(#)
** Date:   3/22/96
******************************************************************************
 * This module contains the following functions:
 * getstat() is used to get the status of the MC68328ADS Touchpanel device
 * putstat() is used to set the status of the MC68328ADS Touchpanel device
******************************************************************************
** Copyright 1995 by Microware Systems Corporation                          **
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
**	 1 09/13/95 creation                                          rws       **
*****************************************************************************/

#include "touchpad.h"


/*
 * Function to get device status
 *		Call with:
 *			status call function code
 *			caller's register stack image ptr
 *			caller's path descriptor ptr
 *			pointer to system variables
 *			pointer to static storage area
 *		Return:
 *			Error code (0 if none)
 */
error_code getstat 
(
	u_int code,
	REGISTERS  *registers,
	Pathdesc path_desc,
	Sysglobs globals,
	TStatics lst
)
{
	error_code error = SUCCESS;		/* error holder */
	dcmd_struct *dcmd;				/* for setstat (SS_DCmd) usage */

    switch (code) 
    {
		case (SS_EOF):		/* check for end-of-file -- return SUCCESS */
			break;

		case (SS_Ready):	/* get number of bytes pending */
			/*
			 * Note: as per OS-9 Technical I/O Manual:
			 * if data is available, the count should be put into 
			 * registers->d[1] and getstat return with SUCCESS; if no
			 * data is available, getstat should return with
			 * EOS_NOTRDY.
			 */
			if ((registers->d[1] = bytes_avail) == 0)
				error = EOS_NOTRDY;	/* return Not Ready */
			break;

		/*
		 * _os_getstat() Parameters passed:
		 *	registers->a[0] = ptr to location SS_DCmd parameter block
		 */
		case (SS_DCmd):		/* application specific direct command */

			dcmd = (dcmd_struct *)registers->a[0];

			if (dcmd->command == CALIBRATE)
			{
				/* pass up calibration parameters */
				dcmd->ss_specific.cal_data.ss_X_min = X_min;
				dcmd->ss_specific.cal_data.ss_Y_min = Y_min;
				dcmd->ss_specific.cal_data.ss_X_max = X_max;
				dcmd->ss_specific.cal_data.ss_Y_max = Y_max;
			}
			else 
				error = EOS_UNKSVC;

			break;

        /* Unknown or Unimplemented Service Request */
        default:	
            error = EOS_UNKSVC;		/* return Unknown Service */
    }
    return error;
}



/*
 * Function to set device status.  
 *      Call with:
 *          status call function code
 *          caller's register stack image ptr
 *			caller's path descriptor ptr
 *			pointer to system variables
 *			pointer to static storage area
 *      Return:
 *          Error code (0 if none)
 */
error_code putstat 
(
	u_int code,
	REGISTERS *registers,
	Pathdesc path_desc,
	Sysglobs globals,
	TStatics lst
)
{
	error_code error = SUCCESS;    	/* error holder */	
	dcmd_struct *dcmd;				/* for setstat (SS_DCmd) usage */

	status_reg old_mask;	/* used to save current sr IRQ mask level */
	extern status_reg mask_irq (status_reg mask);	/* function in read.c */
	
	switch (code)
	{
		case (SS_Open):		
			/* 
			 * SS_Open is called by SCF whenever a new path to the
			 * device is opened -- just return SUCCESS
			 */
			break;

		case (SS_Close):	/* path being closed */
		case (SS_Relea):	/* release SS_SSig */
		
			/* mask ticker interrupt to prevent alarm handler being invoked 
			 * so there can be no contention on datapid between putstat() 
			 * and alarm_h() */
			old_mask = mask_irq (0x0700);

			if (datapid == path_desc->path.pd_cpr &&
				dataid == path_desc->path.pd_pd)
			{
				datapid = 0; 		/* clear down signal condition */
				dataid = 0;
				datasig = 0;
			}
			else 
                error = EOS_NOTRDY; /* return Not Ready */
                                				
			mask_irq (old_mask);	/* unmask alarm interrupt */
       		break;
       	
		case (SS_SSig):		/* send signal on data ready */

			/*
			 * SS_SSig is used when a signal is to be sent upon data ready. 
			 * Parameters passed:
			 *	registers->d[2] = signal number to be sent 
			 */

			/* mask ticker interrupt to prevent alarm handler being invoked 
			 * so there can be no contention on datapid between putstat() 
			 * and alarm_h() */
			old_mask = mask_irq (0x0700);

			if (datapid != 0)			/* another process is waiting */
                error = EOS_NOTRDY; 

        	else if (bytes_avail == PKTBUFSIZ)	/* data pending -- send signal now */
         		_os_send (path_desc->path.pd_cpr, registers->d[2]);

         	else						/* set up signal data static area */
        	{
         		datapid = path_desc->path.pd_cpr;
         		dataid = path_desc->path.pd_pd;
      			datasig = registers->d[2];
			
				/* set bytes_avail to 0 in case of error previously */
				bytes_avail = 0;
         	}

			mask_irq (old_mask);	/* unmask alarm interrupt */
         	break;

		/*
		 * _os_setstat() Parameters passed:
		 *	registers->a[0] = ptr to location SS_DCmd parameter block
		 */
		case (SS_DCmd):		/* application specific direct command */

			dcmd = (dcmd_struct *)registers->a[0];

			if (dcmd->command == GET_POINT)
			{
				/* Set up statics to inform alarm_irqsvc() that the next touch 
				 * will be for a known pixel location */
				cal_flag = TRUE;
				if (ss_ptr == 0)
				{
					ss_x0_pos = dcmd->ss_specific.gfx_point.x_pos;
					ss_y0_pos = dcmd->ss_specific.gfx_point.y_pos;
					/* ss_ptr is handled in irqs.c */
				}
				else
				{
					ss_x1_pos = dcmd->ss_specific.gfx_point.x_pos;
					ss_y1_pos = dcmd->ss_specific.gfx_point.y_pos;
					/* ss_ptr is handled in irqs.c */
				}
			}
			else if (dcmd->command == CALIBRATE)
			{
				/* set calibration based on passed values */
				X_min = dcmd->ss_specific.cal_data.ss_X_min;
				Y_min = dcmd->ss_specific.cal_data.ss_Y_min;
				X_max = dcmd->ss_specific.cal_data.ss_X_max;
				Y_max = dcmd->ss_specific.cal_data.ss_Y_max;

				delta_x = (1024 * (X_max - X_min)) / (x_pixels - 1);
				delta_y = (1024 * (Y_max - Y_min)) / (y_pixels - 1);
			}
			else
				error = EOS_UNKSVC;
				
			break;

        /* Unknown or Unimplemented Service Request */
        default:
			error = EOS_UNKSVC;		/* return Unknown Service */
	}
	
	return (error);
}

