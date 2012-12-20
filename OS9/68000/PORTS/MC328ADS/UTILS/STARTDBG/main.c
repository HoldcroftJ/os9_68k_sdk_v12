/*****************************************************************************
** Start up user/system state debugging
******************************************************************************
** Copyright 1996-2000 by Microware Systems Corporation                     **
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
**   1 04/15/96 creation                                          bat       **
**   2 01/22/97 adapted for MC68328-ADS port                      bat       **
**   3 11/08/00 Pulled last bits of netrom/isp.                   rkw       **
*****************************************************************************/

#include <stdlib.h>
#include <types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


void main(int argc, char *argv[]);
void help(void);


/*
** Print Help
*/
void help(void)
{
	/* syntax error -- print out help */
	fprintf(stderr,"Syntax: startdbg [user|system] [slip|netrom]\n");
	fprintf(stderr,"Function: Start remote debugging facilities\n");
	fprintf(stderr,"Options: (none)\n");

	/* exit with error */
	exit(EOS_PARAM);
}

/*
** Program Entry Point
*/
void main(int argc, char *argv[])
{
	int err;
	enum {user,sys} dbgtype = user;
	enum {slip,netrom} dbgdevice = slip;


	/* check command-line parameters */

	if (argc != 3) {
		help();
	}

	if (strcmp((const char*)argv[1],"user") == 0) {
		dbgtype = user;
	} else if (strcmp((const char*)argv[1],"system") == 0) {
		dbgtype = sys;
	} else {
		help();
	}

	if (strcmp((const char*)argv[2],"slip") == 0) {
		dbgdevice = slip;
	} else if (strcmp((const char*)argv[2],"netrom") == 0) {
		dbgdevice = netrom;
	} else {
		help();
	}

	/* start appropriate debugging facilities */
	if (dbgtype == user) {

		/* start SPF user-state debugger */
		if ((err = system("spfndpd<>>>/nil&")) != 0) {
			fprintf(stderr,"ERROR forking 'spfndpd'\n");
			exit(err);
		}

	} else if (dbgtype == sys) {

		/* start Gargoyle system-state debugger */
		if ((err = system("p2init rominfo")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'rominfo'\n");
			exit(err);
		}
		if ((err = system("p2init findcnfg")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'findcnfg'\n");
			exit(err);
		}
		if ((err = system("p2init cnfgfunc")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'cnfgfunc'\n");
			exit(err);
		}
		if ((err = system("p2init notify")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'notify'\n");
			exit(err);
		}
		if ((err = system("p2init console")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'console'\n");
			exit(err);
		}
		if (dbgdevice == slip) {
			if ((err = system("p2init m328tmr2")) != 0) {
				fprintf(stderr,"ERROR p2-initing 'm328tmr2'\n");
				exit(err);
			}
		}
		if (dbgdevice == slip) {
			if ((err = system("p2init flush68k")) != 0) {
				fprintf(stderr,"ERROR p2-initing 'flush68k'\n");
				exit(err);
			}
		}
		if ((err = system("p2init io68681")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'io68681'\n");
			exit(err);
		}
		if ((err = system("p2init conscnfg")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'conscnfg'\n");
			exit(err);
		}
		if (dbgdevice == netrom) {
			if ((err = system("p2init m328tmr2")) != 0) {
				fprintf(stderr,"ERROR p2-initing 'm328tmr2'\n");
				exit(err);
			}
		}
		if (dbgdevice == slip) {
			if ((err = system("p2init commcnfg")) != 0) {
				fprintf(stderr,"ERROR p2-initing 'commcnfg'\n");
				exit(err);
			}
		}
		if ((err = system("p2init protoman")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'protoman'\n");
			exit(err);
		}
		if (dbgdevice == slip) {
			if ((err = system("p2init llslip")) != 0) {
				fprintf(stderr,"ERROR p2-initing 'llslip'\n");
				exit(err);
			}
		}
		if ((err = system("p2init llip")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'llip'\n");
			exit(err);
		}
		if ((err = system("p2init lltcp")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'lltcp'\n");
			exit(err);
		}
		if (dbgdevice == netrom) {
			if ((err = system("p2init llnetrom")) != 0) {
				fprintf(stderr,"ERROR p2-initing 'llnetrom'\n");
				exit(err);
			}
		}
#if 0
		if ((err = system("p2init abort328")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'abort328'\n");
			exit(err);
		}
#endif
		if ((err = system("p2init dbgentry")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'dbgentry'\n");
			exit(err);
		}
		if ((err = system("p2init dbgserv")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'dbgserv'\n");
			exit(err);
		}
		if (dbgdevice == slip) {
			if ((err = system("p2init hlproto")) != 0) {
				fprintf(stderr,"ERROR p2-initing 'hlproto'\n");
				exit(err);
			}
		}
		if (dbgdevice == slip) {
			if ((err = system("undpd -s&")) != 0) {
				fprintf(stderr,"ERROR forking 'undpd'\n");
				exit(err);
			}
		} 
		if ((err = system("p2init sndp")) != 0) {
			fprintf(stderr,"ERROR p2-initing 'sndp'\n");
			exit(err);
		}
	}

	/* exit with success */
	exit(0);
}
