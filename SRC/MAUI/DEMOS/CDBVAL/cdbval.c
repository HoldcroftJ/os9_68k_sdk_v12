/*****************************************************************************
 *
 * Filename: cdbval.c
 *  Purpose: A validation utility for CDB modules
 *
 *****************************************************************************
 *                                                                           *
 *           Copyright 2001 - 2001 by Microware Systems Corporation          *
 *                            All Rights Reserved                            *
 *                          Reproduced Under License                         *
 *                                                                           *
 *                                                                           *
 * This software is confidential property of Microware Systems Corporation,  *
 * and is provided under license for internal development purposes only.     *
 * Reproduction, publication, modification, distribution, or creation of     *
 * derivative works in any form to any party other than the Licensee is      *
 * strictly prohibited, unless expressly provided for under the terms of a   *
 * mutually executed written license agreement for this software between the *
 * end-user and Microware Systems Corporation, or otherwise expressly        *
 * authorized in writing by Microware Systems Corporation. Licensee shall    *
 * reproduce this copyright notice exactly as provided herein on any copies  *
 * of the software that Licensee is authorized to distribute.                *
 ****************************************************************************/


/*
 * Include Files
 */

#define _MAIN

#include "cdbval.h"


/*****************************************************************************
	MAIN
*****************************************************************************/


int
main(
	u_int32 argc,
	char    *argv[]
	)
{
	status_code status;

	/* initialize globals */
	init_globals();

	/* parse the command line arguments */
	parse_args(argc, argv);

	/* if we checked general structure (-s), it's time to exit */
	if(g_opt_s != FALSE)
		exit_prog(SUCCESS, NULL);

	/* validate the specific structure of all CDB modules */
	status= validate_specific_structure();
	if(status != SUCCESS)
		exit_prog(status, "validate_specific_structure() failed!");

	/* exit program */
	exit_prog(SUCCESS, NULL);
}


/****************************************************************************/
