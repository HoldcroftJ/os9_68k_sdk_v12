/*
 * term - terminate use of the scsisiop low level module
 *       (NCRSIOP SCSI I/O Processor)
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|    Copyright 1991, 1994, 1996-1997 by Microware Systems Corporation       |
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
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 96/08/03 Added edition history.                                  rak   |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
| 02 97/03/20 Added support for MVME172.                              GbG   |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
| 03 97/04/20 Scripts local usage RAM is now returned if used.        GbG   |
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
|             $$                    <RELEASE_INFO>                     $$   |
`--------------------------------------------------------------------------*/
 
#include	<scsisiop.h>

/*
 *	Function:
 *		term - terminate the use of the low level driver. 
 *
 *  Synopsis:
 *		int term( mod_dev dd, Datmod dmod )
 *
 *  Description:
 *		This routine is called by the high level driver's term()
 *  when the user count of the device goes to 0.  Since the user
 *  count on a device is not nessesarilly the same as the user count
 *  of the SCSI bus in general this routine must also check to 
 *  be certain that all SCSI devices have been terminated.
 *		When all devices on the bus are free this routine must:
 *        - terminate all interrupts from the SIOP
 *        - remove the SIOP interrupt from the polling table
 *        - free the low level static storage
 *        - free the data module associated with the low level driver.
 *
 *  See Also:
 *		OS-9 Technical I/O Manual
 */
#ifndef CBOOT

#if defined (_OS9000)
error_code term (Dev_list device_list, u_int32 vect,
                   u_int32 level, u_int32 prior, Datmod dmod )
#else                   
#if defined (__STDC__) || defined (_ANSI_EXT)
int term (mod_dev *dd, Datmod dmod)
#else
int term (dd, dmod)
mod_dev					*dd;		/* device descriptor */
Datmod					dmod;		/* data module */
#endif
#endif
{
	error_code			error;		/* the error code */
	error_code			lerror;		/* the error code */
	Lstat				lst;		/* low level statics */
	u_int32				sink;		/* put junk here */

	lst = dmod->dm_statics;			/* get pointer to stats */
	
    error = lerror = SUCCESS;		/* until proven otherwise... */
    
	/* if last user of data module, clean up */
	if ((--dmod->dm_users == 0) && (lst)){
		
		/* halt the SIOP, disable interrupts and DMA's */
		if (lst->ncr_chip_type != 0){
			stop_siop( lst );
			freesto( lst );		/* ignor errors, what can be done anyway? */
		} else 
#if defined (__STDC__) 
			lerror =  EOS_HARDWARE;		/* No device found */
#else
			lerror =  E_HARDWARE;		/* No device found */
#endif
				

		/* remove the device from the irq polling system */

#if defined (_OSK)
		error = _os9_irq (lst->irqvector, lst->irqprior, 0, lst, (void *)lst->hdware);
#elif defined (_OS9000)
		error = _os_irq ( lst->irqvector, lst->irqprior, 0, dmod  );
#endif		
		/* return low-level statics */
		if (lst) {

#if defined(_OS9000)

#if defined(NCR53C8XX)
			/* If SCRIPTS RAM is allocated then return it */
			
			if (lst->pci_ram_base == 0)
				error = _os_srtmem (lst->script_sz, (void *)lst->act_script);
#endif
			error = _os_srtmem (dmod->dm_statsize, lst);

#elif defined(_OSK)
			error = _f_srtmem (dmod->dm_statsize, lst);
#endif
			dmod->dm_statics = 0;			/* only return the memory once */
		}
	}  /* end if no users left */

	/* unlink the data module */
	if (sink = _os_unlink (dmod->dm_modhdr))
		error = sink;						/* keep the error */

	return (lerror == SUCCESS) ? error : lerror;	/* return any/latest error */
}
#else
#ifdef VME167
#include <dev167.h>
#endif  /* VME167 */

#ifdef VME177
#include <dev177.h>
#endif /* VME177 */

#ifdef VME162
#include <mcchip.h>
#include <dev162.h>
#endif /* VME162 */

#ifdef VME172
#include <mcchip.h>
#include <dev172.h>
#endif /* VME172 */

#if defined (__STDC__) || defined (_ANSI_EXT)
int llterm (mod_dev *dd, Datmod dmod)
#else
int llterm (dd, dmod)
mod_dev					*dd;		/* device descriptor */
Datmod					dmod;		/* data module */
#endif
{
	error_code			error;		/* the error code */
	Lstat				lst;		/* low level statics */
	u_int32				sink;		/* put junk here */
	extern char *svsave;
	extern int lvlsave;
	extern struct lstat	sstorage;	/* cboot static storage */


	/* halt the SIOP, disable interrupts and DMA's */
	lst = &sstorage;

#if defined(_OSK)
	mask_irq( lvlsave );
#elif defined(_OS9000)
	irq_change( lvlsave );
#endif
	stop_siop( lst );
	error = setexcpt(MVECTOR, svsave);
	freesto( lst );
	return error;							/* return any/latest error */
}
#endif

