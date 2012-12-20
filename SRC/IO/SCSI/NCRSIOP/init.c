/*
 * init - init routine for scsisiop low level scsi module
 *        (NCRSIOP SCSI I/O Processor)
 */

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1993-2001 by Microware Systems Corporation         |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 96/08/03 Added edition history.                                  rak   |
|             ---- OS-9000/x86 V2.1 Released ----                           |
|             ---- OS-9000/PPC V2.1.1 Released ----                         |
| 02 96/11/27 Setup PCI configuration command register for 810/825.   GbG   |
| 03 97/01/23 Added support for Symbios 875.                          GbG   |
| 04 97/03/20 Added support for MVME172.                              GbG   |
|             ---- OS-9000/ARMv3 V2.2 Released ----                         |
| 05 97/04/21 Added more Symbios controllers via table.                     |
|             Support for SCRIPTS RAM added.                                |
|             Symbios controller may now be mapped as Memory or I/O.        |
|             Debugging code added using ROM services.                GbG   |
|             ---- OS-9000/PPC V2.1.2 Released ----                         |
|             ---- OS-9000/ARMv3 V2.2.1 Released ----                       |
| 06 98/01/29 Fixed compiler warnings.                                GbG   |
| 07 98/01/29 Removed addition warnings from debug mode.              GbG   |
|             ---- OS-9000/SH3 V2.2.6 Released ----                         |
|             ---- SCSI Common Sub-Component v1.0 Released ----             |
| 66 00/07/27 added hwprobe for 710.                                  gdw   |
| 67 00/11/16 Make it compilable for SH-3/SH-4 (OEM only, no hw).     and   |
| 71 01/08/09 change debug code to use dbglog.                        gdw   |
`--------------------------------------------------------------------------*/

#include	<scsisiop.h>

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)
#include	<cache.h>
void cache_flush(Lstat lst, u_int32 *addr, u_int32 size);
#endif

#if defined(NCR53C8XX)
#include	<ibc.h>
#include	<pcicnfg.h>
#include 	<systype.h>

/*
 * Symbios Supported Devices
*/

u_int16 const Symbios_Device_Array[] = {
	PCI_DEVICE_NCR53C810,
	PCI_DEVICE_NCR53C820,
	PCI_DEVICE_NCR53C825,
	PCI_DEVICE_NCR53C815,
	PCI_DEVICE_NCR53C810AP,
	PCI_DEVICE_NCR53C860,
	PCI_DEVICE_NCR53C896,
	PCI_DEVICE_NCR53C895,
	PCI_DEVICE_NCR53C885,
	PCI_DEVICE_NCR53C875,
	PCI_DEVICE_FIREPORT20,
	PCI_DEVICE_FIREPORT40
};

#endif

#ifndef CBOOT
/*
 *	Function:
 *		init - initialize low level driver.
 *
 *  Synopsis:
 *		int init( mod_dev dd, Datmod pdmod )
 *
 *  Description:
 *		This entry point is called by the high level driver to
 *  initialize the low level driver.  Each high level driver that 
 *  connects to this low level driver module will make this call and
 *  as such, this code must insure that hardware initialization only
 *  occurs once.
 *		This routine must:
 *        - create a data module and initialize it for shared access
 *          with the high level driver.
 *        - allocate memory for use as low level static storage and
 *          initialize it a per needs of the low level drivers other
 *          routines. (Remember:  not automatic initialization is 
 *          performed for "subroutine modules"!)
 *        - initialize the hardware associated with the low level 
 *          driver.  ( hardware initialization can be defered to the
 *          first valid command if so desired, but this driver will
 *          perform that initialization here.)
 *        - install interrupt service for the device.
 *
 *  See Also:
 *		term()
 *      OS-9 Technical I/O Manual
 */

#if defined(_OS9000) && !defined(BOOTNCR)

error_code init(Dev_list device_list,u_int32 vect,u_int32 level,
                u_int32 prior,Datmod *pdmod )

#else
#if defined (__STDC__) || defined (_ANSI_EXT)
int init(mod_dev *dd, Datmod *pdmod)
#else
int init(dd, pdmod)
mod_dev		*dd;			/* path descriptor */
Datmod		*pdmod;			/* pointer to pointer to data module */
#endif
#endif
{
	Lstat lst;					/* low level static storage */
	Datmod dmod;				/* pointer to data module info */
	error_code error;			/* the error number holder */
	u_int32 portaddr;			/* hold adjusted address here */
	void *dat_hdr;				/* pointer to data module header */
	int32 sink;					/* put unused results here */
	u_int16 attr_rev,           /* attribute revision word */
	type_lang;                  /* type language word */
	char name[32];              /* used to build data mod & event names */
#if defined(_OSK)
	u_int32 vect = dd->_mvector;
#elif defined(_OS9000)
	u_int8 bus, dev;			/* bus number and device number */
	u_int32 index;
	u_int32 device;
#endif

	/*
	 * build the data module name:
	 * the name is the device port address "ANDED" with 0xfffffff0.
	 * e.g. for a device at 0xfffe4000, the name is "datfffe4000".
	 */	

#if defined(_OSK)

	portaddr = ((int) rbfstat.v_sysio.v_port) & (~0xff);

#elif defined(_OS9000)

	portaddr = ((u_int32) device_list->v_port) & (~0xf); /* address... */

#endif

#if defined(NCR53C710) && !defined(_MPFSH)
        /*
         * do a quick probe of the hardware address in the descriptor to see if
         * the 710 is really there or not.
         */
         error = hwprobe(portaddr);
         if (error != SUCCESS)
             return (error);
#endif


	strcpy (name, "dat");			/* get prefix */
	lxtoa (&name[3], portaddr);		/* build address part */

	attr_rev = MA_REENT<<8;             /* reentrant module */
	type_lang = MT_DATA;                /* data module */

	/* attempt to create data module */
	error  = _os_datmod(name, sizeof (struct datmod), &attr_rev,
			&type_lang, LLMODACCESS, (void **) pdmod,
			(mh_data **) &dat_hdr);

	/*
	 * if no error, this is a "first call" (the data module
	 * was just created) to the scsi port.
	 *
	 * the low level module needs to:
	 *  - initialize data module values in the data module
	 *    (F$DatMod defaults all entries to zero).
	 *  - create data module free/busy event
	 *  - create irq service event
	 *  - add hardware device(s) to IRQ polling system
	 *  - arm hardware device(s) for interrupts
	 *
	 * if error, and error is "known module", then this is
	 * a "non-first call" to the low-level module.  The data
	 * module, event(s) and irq initialization have already
	 * been setup, so the new user needs only to record
	 * itself in the data module.
	 */
	if (error == 0) {
		/* "first-call" to low-level module */
		dmod = *pdmod;					/* get address of data module info */

#if defined(_OS9000)
		dmod->dm_port = (void *) device_list->v_port; /* get the port address */
#endif

		dmod->dm_modhdr = dat_hdr;		/* set the module's header ptr */
		dmod->dm_init = init;			/* set init function pointer */
		dmod->dm_exec = exec;			/* set exec function pointer */
		dmod->dm_term = term;			/* set terminate routine */
        dmod->dm_users = 1;				/* set initial user count */
		dmod->dm_statsize = sizeof (struct lstat);

#if defined(_OS9000)
		if ((error =
			_os_srqmem( (u_int32 *)&dmod->dm_statsize, (void **)&dmod->dm_statics,MEM_ANY)) != SUCCESS)
			return error;

#elif defined(_OSK)
		
		/* get memory for "low-level statics" for this module */
		if (error = _f_srqmem (sizeof (struct lstat), 
									&dmod->dm_statsize, &dmod->dm_statics))
			return error;
#endif			
		/* setup low level static storage */
		lst = dmod->dm_statics;					/* point at static storage */
		clearmem ((char *)lst, dmod->dm_statsize);		/* zero it */

#if defined(OS_CACHE_FLUSH) && defined(_OS9000)
		_os_scache(C_GETCCTL, &lst->cache_cctl, (void **)&sink, 0, 0);
#endif

		lst->dmptr = dmod;						/* record data module ptr */
		lst->ncr_chip_type = 0;

#if defined(NCR53C8XX)

		/* Get device index number	*/

		index = ( portaddr & 0x0000ff00 ) >> 8;
		device = ( portaddr & 0x00ff0000 ) >> 16;

		/*
		 * Look for all Symbios 8xx supported devices
		*/

		/*
		 * The driver may be compiled to work with only one
		 * Symbios controller type if desired. Normally the
		 * driver will scan the PCI bus for any supported controllers.
		 * This option may be removed if desired to force only the
		 * controller type desired to be used.
		 *
		 * On the compiler line include the "-dSYMBIOS_DEVICE_ID=xx"
		 * option. For example to force the driver to only use
		 * the Symbios 875 use the following.
		 * 
		 *      -dSYMBIOS_DEVICE_ID=0xf
		 *
		 */
		 
#if defined(SYMBIOS_DEVICE_ID)

		/* Search for supported Symbios Device */
		if( pci_find_device(PCI_VENDOR_ID_NCR, SYMBIOS_DEVICE_ID,
				  index, &bus, &dev ) != NO_DEVICE ){
			if ( (device == 0) || (device == dev)){
				lst->ncr_chip_type = SYMBIOS_DEVICE_ID;
			}
		}
#else
		for (sink=0; sink< sizeof(Symbios_Device_Array)/sizeof(u_int16); sink++){
		
			/* Search for supported Symbios Device */
			if( pci_find_device(PCI_VENDOR_ID_NCR, Symbios_Device_Array[sink],
						  index, &bus, &dev ) != NO_DEVICE ){
				if ( (device == 0) || (device == dev)){
					lst->ncr_chip_type = Symbios_Device_Array[sink];
				}
			}
			/* Found? */
			if (lst->ncr_chip_type)
				break;
		}
#endif

		if (lst->ncr_chip_type == 0){

			/* OS9000 IOMAN does not detatch on fail to attach */

			#if defined(_OS9000) 
					term(device_list, vect, level, prior, dmod );
			#endif

			return EOS_HARDWARE;
		}

		DPRINTF(DBIT_HW, ("\nSymbios 0x%8x", lst->ncr_chip_type));

		/* Save PCI revision information for device */

		lst->ncr_chip_rev =  (u_int16) (pci_read_configuration_dword(\
				bus, dev, 0,
				offsetof(pci_config_reg, rev_class)) & 0xff);
		
		/* Get PCI I/O Port Address */

		lst->pci_io_base = pci_read_configuration_dword(
					 bus, dev, 0, 
					 offsetof(pci_config_reg,base_addrs[0])) & ~1;

		lst->pci_memory_base = pci_read_configuration_dword(
					 bus, dev, 0, 
					 offsetof(pci_config_reg,base_addrs[1]));

		
#if defined(PCI_IO_MAPPED)
		lst->hdware = lst->pci_io_base | PCI_IO_BASE;				
#else
		lst->hdware = lst->pci_memory_base | PCI_MEM_BASE;		
#endif
		
		
#if defined(SCRIPTS_RAM) && (PCI_MEM_BASE == 0)

		/*
		 * NOTE:
		 * 
		 *	The -dSCRIPTS_RAM option must be in effect to use the 
		 *	special SCRIPTS RAM option. Some system platforms such
		 *	as PowerPC may have problems with this feature.
		 *	On Symbios controllers that support SCRIPTS RAM feature
		 *	the PCI configuration base[2] address will be used for
		 *	scripts. Since running SCRIPTS RAM effects the patch
		 *  code we can at this time only support systems that
		 *  do not include translation of IO/MEMORY spaces.
		 * 
		*/

		/* Get PCI RAM Base Address for SCRIPTS loading */
		lst->pci_ram_base = pci_read_configuration_dword(
					 bus, dev, 0, 
					 offsetof(pci_config_reg,base_addrs[2]));

		if (lst->pci_ram_base){
			lst->pci_ram_base |= PCI_MEM_BASE;
		}

#else
	lst->pci_ram_base = 0;
#endif
		/*
		 * Configure PCI for use with 8xx series controller
		*/
		
		switch(lst->ncr_chip_type){

			case PCI_DEVICE_NCR53C875:
			case PCI_DEVICE_FIREPORT40:
			
				/* 
				 * Set IO/MEM/DMA on in PCI configuration command register 
				 * Set write and invalidate in PCI configuration space 
				*/
				pci_write_configuration_word(
						bus, dev, 0,
						offsetof(pci_config_reg,command_reg), 0x17);
				break;

			default:
				/* 
				 * Set IO/MEM/DMA on in PCI configuration command register 
				*/

				pci_write_configuration_word(
					bus, dev, 0,
					offsetof(pci_config_reg,command_reg), 7);
				break;
		}

		if (vect == 0){
			/* Get PCI irq line */
			vect = pci_get_irq_line( bus, dev, 0);
							
			if( vect == 0 || vect == 0xff ) { /* If it was not configured */

				/* OS9000 IOMAN does not detatch on fail to attach */

				#if defined(_OS9000) 
						term(device_list, vect, level, prior, dmod );
				#endif

				return EOS_BMODE;
			}	
			vect += 0x40;
		}

#else
		lst->hdware = portaddr;					/* record SIOP pointer */
#endif

		lst->selfid = SELFID;					/* set ID for later use */

		/* initialize the thread structures and start the NCR 53C710  */
		if( relocate(lst) != SUCCESS )

#if defined (__STDC__)
			return EOS_PARAM;
#else
			return E_PARAM;
#endif

		initthreads( lst );					/* complete the initialization */

#if defined(_OSK)		

		lst->irqlevel = dd->_mirqlvl;			/* interrupt level  */
		lst->irqvector = dd->_mvector;			/* interrupt vector */
		lst->irqprior = dd->_mpriority;			/* polling table priority */
		
		/* form (68xxx) irq mask and record it for later use */
		lst->mask_level = (dd->_mirqlvl << 8); 

		/* add the device to the system irq polling routine */
		if (error = _os9_irq(dd->_mvector, dd->_mpriority,
					irqsvc, lst, (void *) portaddr))
			return error;

#elif defined(_OS9000)

        lst->irqlevel = level;                  /* interrupt level  */
        lst->irqvector = vect;                  /* interrupt vector */   
        lst->irqprior = prior;                  /* polling table priority */

		/* add siop device to the system irq polling routine */
		if( (error = _os_irq(vect, prior, irqsvc, dmod  )) != SUCCESS ){

			/* OS9000 IOMAN does not detatch on fail to attach */

			#if defined(_OS9000) 
					term(device_list, vect, level, prior, dmod );
			#endif

			return error;
		}

#endif        

#ifdef STATS
		/* initialize the debugging structures */
		lst->grosscmd = lst->ninterr = lst->nentries = 0;
		lst->nextin = &lst->irqlog[0];

		/* to aid the user in finding stats information */

		lst->stats_msg[0] = 'S';	
		lst->stats_msg[1] = 'T';	
		lst->stats_msg[2] = 'A';	
		lst->stats_msg[3] = 'T';	

#endif /* STATS */

		if( error = start_siop( lst ) ){	/* attempt to start the 710 */

			/* OS9000 IOMAN does not detatch on fail to attach */

			#if defined(_OS9000) 
					term(device_list, vect, level, prior, dmod );
			#endif

			return error;
		}
			
		if( error = initsto( lst ) ){

			/* OS9000 IOMAN does not detatch on fail to attach */

			#if defined(_OS9000) 
					term(device_list, vect, level, prior, dmod );
			#endif

			return error;					/* problems installing timer irq */
		}

#if defined(NCR53C710)
		lst->ncr_chip_type = 710;				/* NCR710 is now device installed */	
#endif
	} else 
		/* error from data module creating, check if module exists */
#if defined (__STDC__)
		if (error == EOS_KWNMOD) {
#else
		if (error == E_KWNMOD) {			
#endif

            /* data module already exists, simply link it */
        char *name_ptr = &name[0];  /* temp pointer for link call */
        type_lang = 0;              /* any */
        attr_rev = 0;               /* any */
        if ( (error = 
			_os_link(&name_ptr, (mh_com **)&sink, 
					(void **) pdmod, &type_lang, &attr_rev)) == SUCCESS){

	        dmod = *pdmod;              /* get address of data module info */
	        dmod->dm_users += 1;        /* bump user count */
			}
    }
    return error;

}  /* end init() */

#else
/*  bootstrap version of init */
#ifdef VME167
#include <dev167.h>
#endif /* VME167 */

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

/* static storage for the boot driver */
struct lstat sstorage;			/* model of the driver static storage */
int ttimer,ttimerdone;			/* tick timer variables */
char *svsave,*tvsave;			/* jump table addresses for various vectors */
int lvlsave;					/* stash for the PCC level */

#if defined (__STDC__) || defined (_ANSI_EXT)
int llinit( void )
#else
int llinit()
#endif
{
	Lstat lst;						/* low level static storage */
	error_code error;
	extern char *setexcpt();
	extern int irqsvc();
	
	lst = (Lstat)&sstorage;
	lst->hdware = NCR710Base;				/* scsi chip */
	lst->selfid = SELFID;					/* set ID for later use */
	lst->irqlevel = MIRQLVL;				/* interrupt level  */
	lst->irqvector = MVECTOR;				/* interrupt vector */

	/* form (68xxx) irq mask and record it for later use */
	lst->mask_level = (MIRQLVL << 8); 
	
	/* set up interrupt */
	svsave = setexcpt( MVECTOR, irqsvc );		/* set up irq */
			
	/* initialize the thread structures and start the NCR 53C710  */
	if( relocate(lst) != SUCCESS )

#if defined (__STDC__)
		return EOS_PARAM;
#else
		return E_PARAM;
#endif 

	initthreads( lst );					/* complete the initialization */

	if( error = initsto( lst ) )
		return error;					/* problems installing timer irq */

	lvlsave = mask_irq( lst->mask_level - 0x100);	/* unmask interrupts */

	if( error = start_siop( lst ) )		/* attempt to start the 710 */
		return error;
}
#endif



