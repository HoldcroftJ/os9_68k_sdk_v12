/*
 * $Header:   /h0/MWOS/SRC/ROM/SERIAL/VCS/conscnfg.c_v   1.24   09 Jul 1998 11:44:38   alexs  $
 */
    
/*--------------------------------------------------------------------------,
|                                                                           |
|            Copyright 1995-1998 by Microware Systems Corporation           |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| conscnfg.c - ROM I/O console configuration module                         |
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 95/05/24 Prehistoric                                             ats   |
| 02 95/07/14 History started                                         gls   |
| 03 95/08/24 Added USECNFGDATA build option for config services.     ats   |
| 04 95/10/06 Begin transition to error codes in errno.h.             ats   |
|             ---- OS-9/68K V3.0.2 System-state Debug Support Release ----  |
|             ---- OS9000/PPC V2.0 ----                                     |  
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
| 05 96/10/31 Added TOLERATE_PROBE_FAILURE and TEST_FOR_OPERATOR      ats   |
|             build options for dynamic configuration support.              |
|             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   |
| 06 97/02/18 Added calls to notification services.  Fixed history.   gdb   |
| 07 97/02/20 Move notification exit to after TEST_FOR_OPERATOR opt.  ats   |
| 08 97/02/21 Only issue notification if os_flag is set.              ats   |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
|             ---- Modular Rom Sub-system v1.0 Released ----                |
| 09 98/01/28 Eliminate compiler warnings.                            ats   |
|             ---- MBX_BSP Released ----                                    |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- OS-9000/SPARC V2.2.7 Released ----                       |
|             ---- Modular Rom v1.1 Sub-component Released ----             |
|             ---- Modular ROM v1.2 Sub-component Released ----             |
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/
                
#define	EDITION		9
                             
_asm("_sysedit: equ %0", __obj_constant(EDITION));

#define NEWINFO
#include <rom.h>

#include <types.h>
#include <const.h>
#include <sysglob.h>
#include <errno.h>
#include <romerrno.h>
#include <string.h>

#include <p2lib.h>

#if defined(USECNFGDATA)
#define	CONSNAME	consname
#else
#include <systype.h>
#endif


/* configuration module initialization */

error_code p2start(Rominfo rinf, u_char *glbls)
{
	Rom_svcs	romsvc = rinf->rom;
	Cons_svcs	console = rinf->cons;
	Os_svcs		ossvc = rinf->os;
#if defined(USECNFGDATA)
	Cnfg_svcs	cnfgsvc = rinf->config;
	char		*consname;
#endif
	Consdev		cdev;
	error_code	stat;
	u_int32		terminate = 0;

	/* verify a console services module exists */
	
	if (console == NULL)
		return (EOS_NOCONS); /* nope, leave it alone and go bye bye */

#if defined(USECNFGDATA)
	/* verify a configuration services module exists */
	
	if (cnfgsvc == NULL)
		return (EOS_NOCNFGSVC); /* nope, leave it alone and go bye bye */
		
	/* get the console configuration */
	
	stat = cnfgsvc->get_config_data(CONS_NAME, 0, rinf, &consname);
	if (stat != SUCCESS)
		return (stat);			/* sorry, no console configured */
#endif

	/* configure the desired serial port as console */	

	cdev = console->rom_conslist;
	
	while (cdev != NULL) {
		if (!strcmp((char *)cdev->cons_name, CONSNAME)) break;
		cdev = cdev->cons_next;
	}

	if (cdev != NULL) { /* process the console we found */
#if defined(USECNFGDATA)
		u_int8	parity, wordsize, stopbits;
#endif

		if (cdev->cons_probe) { /* do probe if routine present */
			stat = cdev->cons_probe(rinf, cdev);
			if (stat != SUCCESS)
#if defined(TOLERATE_PROBE_FAILURE)
				return (SUCCESS);
#else
				return (stat);
#endif
		}

#if defined(USECNFGDATA)
		/* configure remaining console port parameters */
		
		cnfgsvc->get_config_data(CONS_VECTOR, 0, rinf, &(cdev->cons_vector));
		cnfgsvc->get_config_data(CONS_PRIORITY, 0, rinf, &(cdev->cons_priority));
		cnfgsvc->get_config_data(CONS_LEVEL, 0, rinf, &(cdev->cons_level));
		cnfgsvc->get_config_data(CONS_TIMEOUT, 0, rinf, &(cdev->poll_timeout));
		cnfgsvc->get_config_data(CONS_BAUDRATE, 0, rinf, &(cdev->cons_baudrate));
		
		cnfgsvc->get_config_data(CONS_PARITY, 0, rinf, &parity);
		cnfgsvc->get_config_data(CONS_WORDSIZE, 0, rinf, &wordsize);
		cnfgsvc->get_config_data(CONS_STOPBITS, 0, rinf, &stopbits);
		cdev->cons_parsize = parity | wordsize | stopbits;
		
		cnfgsvc->get_config_data(CONS_FLOW, 0, rinf, &(cdev->cons_flow));
#endif
		
		/* make this console available */
		console->rom_incons = cdev;
		console->rom_outcons = cdev;
		
		if (cdev->cons_init) /* init console if routine present */
			(*(cdev->cons_init))(rinf, cdev);
	}

	/* issue the hello message, if any */

	if ((ossvc != NULL) && (ossvc->rom_osflag) && (rinf->notify != NULL))
		rinf->notify->rom_notify(rinf, NTFY_DIR_TOROM);
                  
	if (romsvc->rom_hellomsg)
		console->rom_puts(romsvc->rom_hellomsg, rinf);

#if	defined(TEST_FOR_OPERATOR)

#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

#define	BEL		0x07
#define ESC		0x1b

#define SLIP_END	0xc0

#define ONE_SECOND	1000000

	{
		Tim_svcs timr = rinf->timer;
		u_int32 timeleft, elapsed, useconsole = FALSE;
		char response;		
		
		if (!timr) {
			console->rom_puts("No timer module available - \n", rinf);
			console->rom_puts("Serial console assumed.\n", rinf);
			return (SUCCESS);
		}
		(*(timr->timer_init))(rinf);
		for (elapsed=0; elapsed<3; elapsed++) {
			console->rom_putchar(BEL, rinf);
			(*(timr->timer_set))(rinf, ONE_SECOND);
			do {
				if ((*(cdev->cons_check))(rinf, cdev)) {
					response = (*(cdev->cons_read))(rinf, cdev);
					if (response == ESC) {
						useconsole = TRUE;
						break; /* out of do-while loop */
					}
				}
				timeleft = (*(timr->timer_get))(rinf);
			} while (timeleft > 0);
			if (useconsole)
				break; /* out of 3-second timeout */
		}
		(*(timr->timer_deinit))(rinf);
		if (!useconsole) { /* if no operator responded */
			console->rom_putchar(SLIP_END, rinf); /* help SLIP sync back up */
			console->rom_incons = NULL; /* deconfigure as console */
			console->rom_outcons = NULL;
			terminate = 1;
		} else {
			console->rom_puts("Serial console selected by operator.\n", rinf);
		}
	}
#endif

	if ((ossvc != NULL) && (ossvc->rom_osflag) && (rinf->notify != NULL))
		rinf->notify->rom_notify(rinf, NTFY_DIR_TOSYS);

	if (terminate)
		(*(cdev->cons_term))(rinf, cdev); /* terminate driver */
						
	return (SUCCESS);
}

