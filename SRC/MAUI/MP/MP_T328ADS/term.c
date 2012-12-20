/*****************************************************************************
** ID:     @(#) term.c 1.1@(#)
** Date:   1/19/96
******************************************************************************
 * 	This file contains functions for terminating the MAUI Process Protocol 
 *  Module
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
**	 1 01/19/96 created--copied from Wirlwind and modeled           rws     **
**              PHILMOUS                                                    **
*****************************************************************************/


#define _TERM_C

#include "_key.h"
#include <string.h>


/*******************************************************************************
 * mppm_term() -- unregister a process
 *******************************************************************************/
error_code mppm_term (MP_MPPM *mppm)
{
  return SUCCESS;
}

/*******************************************************************************
 * mppm_detach() -- unregister a device
 *******************************************************************************/
error_code mppm_detach (MP_DEV *inp_dev)
{
  return SUCCESS;
}

