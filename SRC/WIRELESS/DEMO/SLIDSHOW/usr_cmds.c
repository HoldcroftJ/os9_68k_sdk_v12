/*****************************************************************************
** NAME:    usr_cmds(int arg_num, char *opt)
** USAGE:   Verifies user commands and returns a pointer to array containing
** 			valid user selections.
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
**   1 05/07/96 creation                                          pbt       **
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <types.h>
#include "usr_cmds.h"

#define TK_RATE 0
#define HALT_IMG 1

								
/* function prototypes */

void disp_ex(void);			/* function that displays the valid list of options */
						
/****************************************************************************/

extern u_int32 *usr_cmds(int arg_num, char *opts)


{
	u_int32 usr_sel[2];	/* array which contains valid user selections */
	u_int32 x= 0;			/* variable to hold loop count */
	u_int32 next = 1;		/* variable to hold result of string compare function */



	/* check for usage */

		if(arg_num != 2)
		{
			disp_ex();
			return(NULL);
		}

	/* check for correct options */
	/* the acceptable command line options are:
		-d=display each image once at a 5sec/image rate
		-r=display the demo recursively, i.e. loop continuously
		   at a rate of 2sec, 5sec or 10sec
		-h=halt display of the selected image number */
	
    /* begin a series of string compares to see if user has specified
		a valid option, and to assign the proper tick rate (TK_RATE)
		and the image to halt on (HALT_IMG)*/

	/* set x=1 to enter loop */
	x=1;
	
    while(x)
    {
       	if((next=strcmp(opts,"-?")) == 0)	/* a match has occurred */
		{
			/* set next=1 so usage is displayed */
			next = 1;
			break;
		}
		if((next=strcmp(opts,"-d")) == 0)	/* a match has occurred */
		{
			usr_sel[TK_RATE]=500;
			usr_sel[HALT_IMG]=11;
			break;
		}
		else if((next=strcmp(opts,"-r=2")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=0;
			break;
		}
		else if((next=strcmp(opts,"-r=5")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=500;
			usr_sel[HALT_IMG]=0;
			break;
		}
		else if((next=strcmp(opts,"-r=10")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=1000;
			usr_sel[HALT_IMG]=0;
			break;
		}
		else if((next=strcmp(opts,"-h=1")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=1;
			break;
		}
		else if((next=strcmp(opts,"-h=2")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=2;
			break;
		}
		else if((next=strcmp(opts,"-h=3")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=3;
			break;
		}
		else if((next=strcmp(opts,"-h=4")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=4;
			break;
		}
		else if((next=strcmp(opts,"-h=5")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=5;
			break;
		}
		else if((next=strcmp(opts,"-h=6")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=6;
			break;
		}
		else if((next=strcmp(opts,"-h=7")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=7;
			break;
		}
		else if((next=strcmp(opts,"-h=8")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=8;
			break;
		}
		else if((next=strcmp(opts,"-h=9")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=9;
			break;
		}
		else if((next=strcmp(opts,"-h=10")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=10;
			break;
		}
		else if((next=strcmp(opts,"-h=11")) == 0)    /* a match has occurred */
		{
			usr_sel[TK_RATE]=200;
			usr_sel[HALT_IMG]=11;
			break;
		}
		else
			x=0; /* exit while loop, no matches have occurred */
		
	}
	/* check if there are no valid matches */
	
    	if(next !=0)
    	{
			disp_ex();	
			return(NULL);
        }

    return(usr_sel);	
}

void disp_ex()
{
	printf("Syntax: slidshow <opt>\n");
	printf("Function: run slidshow demo\n");
	printf("Options:\n");
	printf("\t-d\t\t\tdisplay each slide once at 5sec/image\n");
	printf("\t-r=<num>\tdisplay demo recursively at <num> sec/image\n");
	printf("\t\t\t\tnum=2,5 or 10\n");
	printf("\t-h=<num>\thalt demo on image <num>\n");
	printf("\t\t\t\tnum= 1,2 ... ,10 or 11\n");
}
