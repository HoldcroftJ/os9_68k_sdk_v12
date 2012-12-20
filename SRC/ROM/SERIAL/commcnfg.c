/*
 * $Header:   /h0/MWOS/SRC/ROM/SERIAL/VCS/commcnfg.c_v   1.22   09 Jul 1998 11:44:30   alexs  $
 */
  
/*--------------------------------------------------------------------------,
|                                                                           |
|            Copyright 1995-1996 by Microware Systems Corporation           |
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
!                                                                           !
! commcnfg.c - ROM I/O communications configuration module                  !
!                                                                           !
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 95/05/24 Prehistoric                                             ats   |
| 02 95/07/14 History started                                         gls   |
| 03 95/08/24 Added USECNFGDATA build option for config services.     ats   |
| 04 95/10/06 Begin transition to error codes in errno.h.             ats   |
|             ---- OS-9/68K V3.0.2 System-state Debug Support Release ----  |
|             ---- OS9000/PPC V2.0 ----                                     |  
| 05 96/04/26 Added call to init routine of comm port.                gdb   |
| 06 96/05/28 Reverted back to edition #4 functionality.              ats   |
|             Users of the communications port (RomBug and llslip)          |
|             should init and deinit the port explicititly only when        |
|             the port is in use.                                           |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
| 07 96/10/31 Added TOLERATE_PROBE_FAILURE and TEST_FOR_OPERATOR      ats   |
|             build options for dynamic configuration support.              |
|             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
|             ---- Modular Rom Sub-system v1.0 Released ----                |
| 08 98/01/30 Changes to remove warnings.                             gdw   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- OS-9000/SPARC V2.2.7 Released ----                       |
|             ---- Modular Rom v1.1 Sub-component Released ----             |
|             ---- Modular ROM v1.2 Sub-component Released ----             |
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/

#define	EDITION		8
                                          
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
#define	COMMNAME	commname
#else
#include <systype.h>
#endif


/* configuration module initialization */

error_code p2start(Rominfo rinf, u_char *glbls)
{
	Cons_svcs	console = rinf->cons;
#if defined(USECNFGDATA)
	Cnfg_svcs	cnfgsvc = rinf->config;
	char		*commname;
#endif
	Consdev		cdev;
	error_code	stat;

	/* verify a console services module exists */
	
	if (console == NULL)
		return (EOS_NOCONS); /* nope, leave it alone and go bye bye */

#if defined(USECNFGDATA)
	/* verify a configuration services module exists */
	
	if (cnfgsvc == NULL)
		return (EOS_NOCNFGSVC); /* nope, leave it alone and go bye bye */
		
	/* get the communication port configuration */
	
	stat = cnfgsvc->get_config_data(COMM_NAME, 0, rinf, &commname);
	if (stat != SUCCESS)
	 	return (stat);			/* sorry, no comm port configured */
#endif

	/* configure the desired serial ports as comm */	
				
	cdev = console->rom_conslist;
	
	while (cdev != NULL) {
		if (!strcmp((char *)cdev->cons_name, COMMNAME)) break;
		cdev = cdev->cons_next;
	}

	if (cdev != NULL) { /* process the comm port we found */
#if defined(USECNFGDATA)
		u_int8	parity, wordsize, stopbits;
#endif

#if defined(TEST_FOR_OPERATOR)
		if ((console->rom_incons == cdev) || 
			(console->rom_outcons == cdev))
				return(SUCCESS); /* port is configured for console */
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
		
		cnfgsvc->get_config_data(COMM_VECTOR, 0, rinf, &(cdev->cons_vector));
		cnfgsvc->get_config_data(COMM_PRIORITY, 0, rinf, &(cdev->cons_priority));
		cnfgsvc->get_config_data(COMM_LEVEL, 0, rinf, &(cdev->cons_level));
		cnfgsvc->get_config_data(COMM_TIMEOUT, 0, rinf, &(cdev->poll_timeout));
		cnfgsvc->get_config_data(COMM_BAUDRATE, 0, rinf, &(cdev->cons_baudrate));
		
		cnfgsvc->get_config_data(COMM_PARITY, 0, rinf, &parity);
		cnfgsvc->get_config_data(COMM_WORDSIZE, 0, rinf, &wordsize);
		cnfgsvc->get_config_data(COMM_STOPBITS, 0, rinf, &stopbits);
		cdev->cons_parsize = parity | wordsize | stopbits;
		
		cnfgsvc->get_config_data(COMM_FLOW, 0, rinf, &(cdev->cons_flow));
#endif
			
		/* make this comm port available */
		console->rom_in2cons = cdev;
		console->rom_out2cons = cdev;
	}

	return (SUCCESS);
}

