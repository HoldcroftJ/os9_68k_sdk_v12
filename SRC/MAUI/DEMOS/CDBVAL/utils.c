/*****************************************************************************
 *
 * Filename: utils.c
 *  Purpose: Utility functions for cdbval.
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

#include "cdbval.h"


/*****************************************************************************
	INIT GLOBALS

	This function initializes all of our global variables.
*****************************************************************************/


void
init_globals(void)
{
	status_code status;

	/* initialize globals */
	g_help= "\
Syntax:  cdbval <opts>\n\
Purpose: CDB validation.\n\
Options:\n\
  -d         Dump all CDB data in system memory.\n\
             This is useful to see what order the CDB data will\n\
             appear in to an application.\n\
  -p         Print out registered device type information.\n\
  -s=<name>  Validate the general structure of the CDB module <name>\n\
             in memory (bypassing the CDB API) and exit. This test is\n\
             useful if errors occur when running cdbval which might\n\
             be caused by things like invalid characters or a missing\n\
             NULL terminator, etc.\n\
  -?         You already know this one.\n\
\n\
Note: If no options are not specified, cdbval uses the CDB API to\n\
      validate each entry in all CDB modules in memory.\n\n";

	g_opt_s= FALSE;
	g_colon= ':';
	g_cr= '\n';
	g_slash= '/';
	g_sys= "sys";

	/* convert static CDB info to a linked list */
	status= cidata_to_ll();
	if(status != SUCCESS)
		exit_prog(status, "idata_to_ll() failed!");

	return;
}


/*****************************************************************************
	PARSE ARGUMENTS

	This function parses all of the command-line arguments passed to moplay.
*****************************************************************************/


void
parse_args(
	u_int32 argc,
	char    *argv[]
   )
{
	u_int16     i;
	char        *arg;
	status_code status;

	for(i= 1; i < argc; i++)	    
	{
		arg= argv[i];
		if(*arg++ == '-')
		{
		 	switch(tolower(*arg++))
			{
				/* dump all available CDB data in the system */
				case 'd':
					{
						char   *cdb_data;
						size_t cdb_data_size;

						status= get_cdb_data( & cdb_data, & cdb_data_size);
						if(status != SUCCESS)
							exit_prog(status, "get_cdb_data() failed!");

						printf(cdb_data);
						putchar(g_cr);
						putchar(g_cr);
					}
					exit_prog(SUCCESS, NULL);
					break;

				/* print out registered device type info */
				case 'p':
					print_ll();
					exit_prog(SUCCESS, NULL);
			 		break;

				/* validate general (S)tructure */
				case 's':
					if(*arg == '=')
						arg++;

					if(g_opt_s == FALSE)
					{
						printf("\nGeneral structure validation:\n");
						g_opt_s= TRUE;
					}

					status= validate_general_structure(arg);
					if(status != SUCCESS)
						exit_prog(status, "validate_general_structure() failed!");
	   				break;

				/* help */
				case '?':
					printf(g_help);
					exit_prog(SUCCESS, NULL);
			 		break;

			 	default:
					exit_prog(1, "Unknown option \"%s\"", argv[i]);
	 				break;
	  		}
   		}
		else
			exit_prog(1, "Unknown parameter \"%s\"", argv[i]);
	 }

	 return;
}


/*****************************************************************************
	GET CDB DATA

	This function gets a copy of all of the CDB information.
*****************************************************************************/


error_code
get_cdb_data(
	char   **cdb_data,
	size_t *cdb_data_size
	)
{
	status_code status;

	cdb_init();

	/* get a copy of all CDB data */
	status= cdb_get_size(cdb_data_size);
	if(status != SUCCESS)
		exit_prog(status, "cdb_get_size() failed!");

	*cdb_data= malloc(*cdb_data_size);
	if(cdb_data == NULL)
		exit_prog(EOS_MEMFUL, "malloc(cdb_data_size) failed!");

	status= cdb_get_ncopy(*cdb_data, cdb_data_size);
	if(status != SUCCESS)
		exit_prog(status, "cdb_get_ncopy() failed!");

	cdb_term();

	return SUCCESS;
}


/*****************************************************************************
	ERROR MESSAGE

	This function prints an error message and increments the total
	error count.

	The control string passed should NOT be '\n' terminated.
*****************************************************************************/


void
msg(
	MSG_TYPE ew,
	char     *ddr,
	u_int32  entry_num,
	char     *cntl,
	...
	)
{
	va_list arg;

	/* print the error/warning */
	printf("%s, DDR entry %d: "
		, (ew == MT_ERROR) ? "Error" : "Warning"
		, entry_num);
	va_start(arg, cntl);
	vprintf(cntl, arg);
	va_end(arg);
	putchar(g_cr);

	if(ddr != NULL)
	{
		/* print the DDR */
		while(*ddr  &&  *ddr != g_cr)
			putchar(*ddr++);

		putchar(g_cr);
	}

	putchar(g_cr);

	/* increment the error/warning count */
	if(ew == MT_ERROR)
		g_errors_total++;
	else if(ew == MT_WARNING)
		g_warnings_total++;
	else
		exit_prog(EOS_PARAM, "Invalid message type (0x%x) passed to msg()!", ew);

	return;
}


/*****************************************************************************
	EXIT PROGRAM

	This function allows the user to print an error message to stderr as well
	as exit the program. This simplifies the rest of the code base because
	at each point where an error has been detected, you only need to call
	this function instead of printing your own message and calling exit
	yourself (not to mention the "{}").

	The control string passed should NOT be '\n' terminated.

	If the control string is NULL nothing is printed,
	only _os_exit() is called.
*****************************************************************************/


void
exit_prog(
	error_code error,
	char       *cntl,
	...
	)
{
	va_list arg;

	/*
	 * if the control string is NULL, do not print it.
	 * This is useful when cleanly terminating the
	 * program
	 */
	if(cntl)
	{
		fprintf(stderr, "%s: ", _prgname());
		va_start(arg, cntl);
		vfprintf(stderr, cntl, arg);
		va_end(arg);
		fprintf(stderr, "\n");
	}

	if(error != SUCCESS)
	{
		fprintf(stderr, "\nError code #%03d:%03d\n"
#if defined(_OS9000)
			, (error & 0xffff0000) >> 16, error & 0x0000ffff
#elif defined(_OSK)
			, (error & 0x0000ff00) >>  8, error & 0x000000ff
#endif
			);
	}

	_os_exit(error);
}


/****************************************************************************/
