/*
 * $Header:   /h0/MWOS/SRC/ROM/SERIAL/68681/VCS/io68681.c_v   1.20   09 Jul 1998 11:49:06   alexs  $
 * $Revision:   1.20  $
 */
/*--------------------------------------------------------------------------,
|                                                                           |
|            Copyright 1998-1996 by Microware Systems Corporation           |
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
|																			|
| io68681.c - console driver for 68681 chip.	         					|
|																			|
| Edition History:															|
|																			|
| Ed#	Date	Changes													By	|
| --- --------	------------------------------------------------------- --- |
|   0 89/02/14  Created.                                                rg  |
|   1 89/07/27  Cloned from io8042.c                                    rry |
|               --- OS-9000 V1.1 released ---                               |
|   2 91/04/30  Removed PIC initialization code.                        afh |
|   3 94/06/17  Adapted for modular ROM usage & portabalized.           ats |
|   4 94/09/08  Refined serialization macro.                            ats |
|   5 94/10/31  Adapted use of io68681.h from sc68681.h                 ats |
|   6 94/10/31  Merged changes from calvin.                             ats |
|   7 95/07/14  Added sysedit                                           gls |
|   8 95/11/20  Removed SERIALIZE_M macros (now handled by inc/outc).       |
|               Changed inc/outc to IN/OUT.  Removed rom_config.h.      gdb |
|  11 96/05/08  Recreated.                                              sep |
|  12 96/05/21  Added structure idver field values.                     ats |
|  13 96/07/03  Created access/update routines for shared register      ats |
|               images. Matched baud rate set preferences with the          |
|               corresponding sc68681 driver. Moved notification            |
|               registration to after parameter verification. Removed       |
|               checks for overrun errors since the indication is not       |
|               on a per-character basis (and is not cleared by simply      |
|               reading a character). Corrected error codes.                |
|               Restructured checks for received characters.                |
|               Properly handle SW flow control during writes.              |
|               Corrected order of interrupt enables/disables.              |
|  14 96/08/05  Add collection of receive error and maximum upcall      ats |
|               overhead statistics.                                        |
|  15 96/08/07  Correct H/W handshake for received data.                ats |
|  16 96/09/25  Correct typecast in proto_upcall routine                sep |
|  17 96/12/10  Added conditionals for the MC2681 & MC2682              sep |
|               ---- OS-9/68K V3.0.3 System-state Debug Support Release --- |
|  18 97/02/10  Moved systype.h infront of io68681.h                    sep |
|               ---- OS-9000/ARMv3 V2.2 Released ----                       |
|               ---- OS-9000/ARMv3 V2.2.1 Released ----                     |
|  19 98/02/16	Removed compiler warnings.								rkw |
|               ---- Modular Rom v1.1 Sub-component Released ----           |
|               ---- OS-9000/505EVB V1.0 Released ----                      |
|               ---- Modular ROM v1.2 Sub-component Released ----           |
|  20 99/10/25  Corrected 38400 entries in baud tables.			            |
|				Made psudo dangerious 38.4 always available.		    rkw	|
|               $$                    <RELEASE_INFO>                     $$ |
|                                                                           |
`--------------------------------------------------------------------------*/

#define     EDITION         20

_asm("_sysedit: equ %0", __obj_constant(EDITION));


#define		_IO68681_C_

#include    <rom.h>
#include    <types.h>
#include    <const.h>
#include    <errno.h>
#include    <romerrno.h>
#include    <systype.h>
#include 	"io68681.h"
#include    <p2lib.h>
#include	<inout.h>


/* Default Values */

u_int32 io68681_write(char, Rominfo, Consdev);

#ifdef	_OSK

#ifndef	OEM_GLOBALS_OFFSET
#define OEM_GLOBALS_OFFSET	0
#endif

/* NOTE:
   OS-9/68K drivers must be built with one baud rate set.
   Both the low-level and high-level (SCF) drivers must match.
   If not specified, the default is baud rate set 2
   (which is the same as the released sc68681.a). */

#ifndef BAUD_RATE_SET1
#define BAUD_RATE_SET2	/* Baud Rate Set 2 is the default	*/
#endif

#ifdef	BAUD_RATE_SET1
#define	BAUDSET		BAUDSET1
#endif

#ifdef	BAUD_RATE_SET2
#define	BAUDSET		BAUDSET2
#endif

#endif

#ifdef	_OS9000
/* Conversion to ACR image */
#define	BAUDSET		(data->pref_baudset & USESET2)
#endif

/*
 *   Device Data Structures
 */
 
consdev		cons_r;						/*  Device configuration data  */
dat68681    cons_dat_r;					/*  Device specific data       */
#ifndef	SINGLE_PORT
consdev		comm_r;
dat68681	comm_dat_r;
#endif


/*
 *  Hardware Configuration Definitions
 */
 
/*
 *  The 68681 does not support the following baud rates:
 *  31250, 56000, 57600, 64000, 115200
 */

#ifdef	_OSK	/* OS-9/68K drivers are built to support	*/
				/* one set or the other. Both low-level and	*/
				/* SCF drivers must be built to match.		*/

/* NOTE: These defines use the OS9000 baud standards, which is also adopted in the 
 * modular rom technology. These values DIFFER that the standard OS9 baud rates. Use
 * The CONS_BAUD defines to get the correct ones. 38400 was added to set 1 to allow 
 * useable slip speeds on slow boards, it does make 19200 not useable at the same time
 * for OSk.
 */

#ifdef	BAUD_RATE_SET1
const u_char bauds[] =
{
    NOSUP, BAUD1_50, NOSUP, BAUD1_110, BAUD1_134P5, NOSUP, BAUD1_300, 
    BAUD1_600, BAUD1_1200, NOSUP, NOSUP, BAUD1_2400, NOSUP, 
    BAUD1_4800, BAUD1_7200, BAUD1_9600, NOSUP, BAUD1_38400, 
    NOSUP, NOSUP, NOSUP, NOSUP, NOSUP
};
#endif /* endif baud_rate_set1 */

#ifdef	BAUD_RATE_SET2
const u_char bauds[] =
{
    NOSUP, NOSUP, BAUD2_75, BAUD2_110, BAUD2_134P5, NOSUP, BAUD2_300, 
    BAUD2_600, BAUD2_1200, BAUD2_1800, BAUD2_2000, BAUD2_2400, NOSUP, 
    BAUD2_4800, NOSUP, BAUD2_9600, BAUD2_19200, BAUD1_38400, NOSUP, 
    NOSUP, NOSUP, NOSUP, NOSUP
};
#endif /* endif baud_rate_set2 */
#endif /* endif _OSK */

#ifdef	_OS9000 
const u_char bauds[] =
{
    NOSUP, BAUD1_50, BAUD2_75, BAUD1_110, BAUD1_134P5, NOSUP, BAUD1_300, 
    BAUD1_600, BAUD1_1200, BAUD2_1800, BAUD2_2000, BAUD1_2400, NOSUP, 
    BAUD1_4800, BAUD1_7200, BAUD1_9600, BAUD2_19200, BAUD1_38400, NOSUP, 
    NOSUP, NOSUP, NOSUP, NOSUP
};

const u_char baud_set[] =
{
    NOSUP, USESET1, USESET2, DONTCARE, DONTCARE, NOSUP, DONTCARE, 
    DONTCARE, DONTCARE, USESET2, USESET2, DONTCARE, NOSUP, 
    DONTCARE, USESET1, DONTCARE, USESET2, USESET1, NOSUP, 
    NOSUP, NOSUP, NOSUP, NOSUP
};
#endif
 
const u_char paritys[] =
{
    NO_PARITY, ODD_PARITY, EVEN_PARITY, NOSUP, NOSUP
};
 
const u_char stops[] = 
{
    SB1, SB1P5, SB2
};

const u_char dbits[] =
{
    BIT8, BIT7, BIT6, BIT5
};

/*
 * Utility Routines
 */
 
u_char access_shared_acr(Dat68681 data)
{
	u_char acr_image;
	
#ifdef	_OSK
	acr_image = data->shared_images_ptr->shared_ACR;
#endif
#ifdef	_OS9000
	if (data->devside == BSIDE)
		data = data->otherside->cons_data;
	acr_image = data->acr_image;
#endif
	return(acr_image);
}
 
u_char access_shared_imr(Dat68681 data)
{
	u_char imr_image;
	
#ifdef	_OSK
	imr_image = data->shared_images_ptr->shared_IMR;
#endif
#ifdef	_OS9000
	if (data->devside == BSIDE)
		data = data->otherside->cons_data;
	imr_image = data->imr_image;
#endif
	return(imr_image);
}
 
u_char access_shared_opcr(Dat68681 data)
{
	u_char opcr_image;
	
#ifdef	_OSK
	opcr_image = 0;
#endif
#ifdef	_OS9000
	if (data->devside == BSIDE)
		data = data->otherside->cons_data;
	opcr_image = data->opcr_image;
#endif
	return(opcr_image);
}

void update_shared_acr(Dat68681 data, u_char acr_image)
{
#ifdef	_OSK
	data->shared_images_ptr->shared_ACR = acr_image;
#endif
#ifdef	_OS9000
	if (data->devside == BSIDE)
		data = data->otherside->cons_data;
	data->acr_image = acr_image;
#endif
}

void update_shared_imr(Dat68681 data, u_char imr_image)
{
#ifdef	_OSK
	data->shared_images_ptr->shared_IMR = imr_image;
#endif
#ifdef	_OS9000
	if (data->devside == BSIDE)
		data = data->otherside->cons_data;
	data->imr_image = imr_image;
#endif
}

void update_shared_opcr(Dat68681 data, u_char opcr_image)
{
#ifdef	_OS9000
	if (data->devside == BSIDE)
		data = data->otherside->cons_data;
	data->opcr_image = opcr_image;
#endif
}

void disable_port_rxints(Dat68681 data)
{
    u_char				imr_image;
	u_int32				intmask;
	
	intmask = irq_maskget();
	
	imr_image = access_shared_imr(data);
	
    if(data->devside == ASIDE)
        imr_image &= ~(INT_ENABLE<<CHARXRDY);
    else
        imr_image &= ~(INT_ENABLE<<CHBRXRDY);
        
	update_shared_imr(data, imr_image);
    OUT(data->uart_base+IMR, imr_image);
    
   	irq_restore(intmask);
}

void enable_port_rxints(Dat68681 data)
{
    u_char				imr_image;
	u_int32				intmask;
	
	intmask = irq_maskget();
	
	imr_image = access_shared_imr(data);

    if(data->devside == ASIDE)
	{
        imr_image |= (INT_ENABLE<<CHARXRDY);
    }
    else
	{
        imr_image |= (INT_ENABLE<<CHBRXRDY);
    }

	update_shared_imr(data, imr_image);
    OUT(data->uart_base+IMR, imr_image);

   	irq_restore(intmask);
}

void disable_port_txints(Dat68681 data)
{
    u_char				imr_image;
	u_int32				intmask;
	
	intmask = irq_maskget();

	imr_image = access_shared_imr(data);

    if(data->devside == ASIDE)
        imr_image &= ~(INT_ENABLE<<CHATXRDY);
    else
        imr_image &= ~(INT_ENABLE<<CHBTXRDY);

	update_shared_imr(data, imr_image);
    OUT(data->uart_base+IMR, imr_image);
    
   	irq_restore(intmask);
}

u_int32 read_check(Consdev cdev)
{
	u_char	*port;			/* the port address			*/
	u_char	chstat;			/* channel status register	*/
	u_int32	result;			/* is there a character ?	*/
	Dat68681 data = cdev->cons_data;
   								
	port = cdev->cons_addr;			/* get the port address   */
	chstat = IN((void *)(port+STATR));
	result = (chstat & (1<<RXRDY));
	
	/* Did we have errors on a waiting character? If so, discard it. */
	if (result) {
		u_char	errors = chstat & (1<<FRAMERR | 1<<PARITYERR);

		/* Check for overrun errors */
		if (chstat & (1<<OVRERR)) {
			data->ovrerrs++;
			OUT(port+CMDR, (RSTERROR<<MISCMDS));
		} 
		
		/* Check for character errors */
		if (errors) {
			if (chstat & (1<<PARITYERR))
				data->parerrs++;
			if (chstat & (1<<FRAMERR))
				data->frmerrs++;
			chstat = IN(port+RBUF);
			result = FALSE;
		}

	}
		
	return (result);		/* return the status */
}

/*
 * Notification Routine
 */
 
u_int32 io68681_stat(Rominfo, Consdev, u_int32);

void notification_handler(u_int32 direction, void *cdev)
{
    switch(direction)
    {
        case NTFY_DIR_TOROM:
            io68681_stat(((Dat68681) (((Consdev) cdev)->cons_data))->rinf, cdev,
                         CONS_SETSTAT_ROMBUG_ON);
            break;
            
        case NTFY_DIR_TOSYS:
            io68681_stat(((Dat68681) (((Consdev) cdev)->cons_data))->rinf, cdev,
                        CONS_SETSTAT_ROMBUG_OFF);
            break;
    }
}

/*
 * Console Device Services
 */
 
/* static u_int32 probe(rinf, cdev) - check for existence */
 
u_int32 io68681_probe(Rominfo rinf, Consdev cdev)
{
	return SUCCESS;
}

/*
 * static u_int32 check(rinf, cdev) - check for a character on the device
 */
   
u_int32 io68681_check(Rominfo rinf, Consdev cdev)
{
   								
	if (cdev->cons_csave != 0)
		return(TRUE);
   
	return (read_check(cdev));		/* return the status */
}
 
/*
 * static u_int32 init(rinf, cdev) - initialize the chip
 */
 
u_int32 io68681_init(Rominfo rinf, Consdev cdev)
{
#ifdef	_OSK
	Os_svcs				ossvc;			/* OS services struct	*/
	u_int32				oem_globals_p;	/* OEM globals pointer	*/
#endif
 	register u_char		*port;			/* the port              */
	register u_int32	statimg;		/* status register image */
    register Dat68681   data;           /* Device specific data  */
    u_char              parity, stopbits, databits, baud, mode;
    u_char				acr_image;
    Notify_svcs         ntfy;
    error_code          stat;
    
	port = cdev->cons_addr;				/* get the port address  */
	data = (Dat68681) cdev->cons_data;  /* get device data       */

#ifdef	_OSK
	/*    Check for OEM Globals */
	
	if (!(ossvc = rinf->os))
		return(EOS_NOOSSVC);
		
	if (ossvc->struct_ver < OSV_OEM_GLOBALS)
		return(EOS_NOVERMATCH);
		
	if (!(oem_globals_p = ossvc->oem_globals))
		return(EOS_NOVERMATCH);
		
	data->shared_images_ptr = (OEM_globals)(oem_globals_p +
		data->oem_globals_offset);
#endif	/* _OSK */

    /*
     *    Validate Communication Parameters
     */
    
    parity = (cdev->cons_parsize & CONS_PARITY_MASK) >> CONS_PARITY_SHIFT;
    if(paritys[parity] == NOSUP)
        return(EOS_BADPARAM);
    
    stopbits = (cdev->cons_parsize & CONS_STOP_MASK) >> CONS_STOP_SHIFT;
    if(stops[stopbits] == NOSUP)
        return(EOS_BADPARAM);
            
    databits = (cdev->cons_parsize & CONS_DBITS_MASK) >> CONS_DBITS_SHIFT;
    if(dbits[databits] == NOSUP)
        return(EOS_BADPARAM);
                    
    baud = cdev->cons_baudrate;
    if(bauds[baud] == NOSUP)
        return(EOS_BADPARAM);

#if	defined(_OS9000)&&!defined(SINGLE_PORT)
	data->pref_baudset = baud_set[baud];
	if (data->pref_baudset != DONTCARE) {
		/* We do care about the baud rate set */
		acr_image = access_shared_acr(data);
		if (BAUDSET != (acr_image & BAUDSET2)) {
			/* The new set needs to be different from the last set */
			Dat68681	other_data = (Dat68681) data->otherside->cons_data;
			u_char		other_set = other_data->pref_baudset;
			
			if ((other_set != DONTCARE) && (data->pref_baudset != other_set))
				/* If the other side also cares and it's not the same as us */
				return(EOS_BADPARAM);	/* Forget it! */
		}
	}
#endif

    /*
     *    Program Device
     */
     
    statimg = irq_maskget();		/* shut off interrupts   */	
        
    /* reset receiver           */
    OUT(port+CMDR, RSTRX<<MISCMDS);
            
    /* reset transmitter        */
    OUT(port+CMDR, RSTTX<<MISCMDS);
                
    /* reset error status bits  */
    OUT(port+CMDR, RSTERROR<<MISCMDS);

	/* disable port interrupts on chip */
	disable_port_rxints(data);
	disable_port_txints(data);
            
    /* reset to mode register 1 */
    OUT(port+CMDR, RSTMODE<<MISCMDS);
	
	/* set parity and word size */
	mode = (paritys[parity]<<PARITY) | (dbits[databits]<<BITS_CHAR);
	if (cdev->cons_flow == CONS_HWSHAKE)
	{
		/*  enable hardware handshake  */
		mode |= (ENABLE<<RxRTS);
	}
	OUT(port+MR1, mode);	/*	set parity/databits & RTS config */
	
	/* set stop bits */
	mode = stops[stopbits]<<STOP_BIT;
	if(cdev->cons_flow == CONS_HWSHAKE)
	{
	    /*  enable hardware handshake  */
	    mode |= (ENABLE<<TxCTS);
	}
	OUT(port+MR2, mode);    /*  set stop bits & CTS configuration  */
	
	/* select baud rate set */
	acr_image = access_shared_acr(data);
  
	acr_image &= ~(1<<BAUDRATE);      	/* Clear table select bit to 0 */
	acr_image |= (BAUDSET<<BAUDRATE);	/* Enable defined table */

#ifdef BAUD_RATE_SET2
     if(baud==CONS_BAUDRATE_38400)		/* If 38400 is specified, do it no matter what */
	   acr_image &= ~(1<<BAUDRATE);     /* Really select table 0, to get 38400 */
#endif /* endif baud_rate_set2 */

	update_shared_acr(data, acr_image);	/* Update shared image */
	OUT(data->uart_base+ACR, acr_image);/* update hardware */
	
	/* set transmit and receive baud rate */
	OUT(port+CSR, (bauds[baud]<<RXCLK) | (bauds[baud]<<TXCLK));
	
#if !defined(MC2681) && !defined(MC2682)
    /*
        The interrupt vector register is undefined on the MC2681
        and MC2682 devices
    */

	/* set interrupt vector for polled-int mode */
	OUT(data->uart_base+IVR, cdev->cons_vector);
#endif

	data->parerrs = data->frmerrs = data->ovrerrs = 0;
	data->maxoverhead = 0;
		    
	cdev->cons_flags = 0;
	if(cdev->cons_flow == CONS_HWSHAKE)
	    cdev->cons_flags |= HWSHAKE;
	else if(cdev->cons_flow == CONS_SWSTRICT)
	    cdev->cons_flags |= (SWSHAKE | SWSTRICT);
	else if(cdev->cons_flow == CONS_SWSHAKE)
	    cdev->cons_flags |= SWSHAKE;
	    
	/* enable transmit and receive */
    OUT(port+CMDR, (ENABLE<<TXCMDS) | (ENABLE<<RXCMDS));
	
	irq_restore(statimg);				/* turn on interrupts */
	
	if (cdev->cons_flags & HWSHAKE)		/* assert ~RTS */
		io68681_stat(rinf, cdev, CONS_SETSTAT_RXFLOW_ON);
	
    /*
     *    Register notification handler
     */
     
    if((ntfy = rinf->notify))
    {
        stat = ntfy->reg_hndlr(rinf, 5, notification_handler, cdev,
                            &(((Dat68681) (cdev->cons_data))->hndlr_id));
        if(stat)
            return(stat);
    }

	return SUCCESS;
}
	
/*
 * static u_char read(rinf, cdev) - read a character from device
 */
 
u_char io68681_read(Rominfo rinf, Consdev cdev)
{
	register u_char		*port;			/* the port address */
	u_char				c;				/* the character */

	if ((c = cdev->cons_csave) != 0) 
    {
		cdev->cons_csave = 0;
		return c;
	}
	
	port = cdev->cons_addr;				/* get the port address */

    while(FOREVER)
    {
    	while(!read_check(cdev));
    
	    c = IN(port+RBUF);				/* get the data */

        if(cdev->cons_flags & SWSHAKE)
        {
            /*  software handshaking enabled  */
            if(c == XOFF)
            {
                /*  we got an XOFF  */
                /* mark that we're in flow control and get next character */
                cdev->cons_flags |= TXOFF;
                continue;
            }
            else
            {
                if((cdev->cons_flags & SWSTRICT) == SWSTRICT)
                {
                    /* we're in strict software flow control */
                    if(c == XON)
                    {
                        /* we got the XON */
                        /* no longer in flow control */
                        cdev->cons_flags ^= TXOFF;
                        continue;
                    }
                    else
                        break;    /*  pass the character on  */
                }
                else
                {
                    if(cdev->cons_flags & TXOFF) 
                    {
                        /* not in strict software flow control */
                        /* we got something so we can go on    */
                        /* no longer in flow control           */
                        cdev->cons_flags ^= TXOFF;
                        continue;    /* get the next character */
                    }
                    else
                    {
                        /* we're not in any software flow control */
                        break;
                    }
                }
            }
        }
        else
        {
            /* software flow control is not enabled */
            break;
        }
    }
    
	return c;							/* return the character */
}

/*
 * static u_int32 write(c, rinf, cdev) - write out a character
 */

u_int32 io68681_write(char c, Rominfo rinf, Consdev cdev)
{
	register u_char		*port;				/* the port address */
	register u_char		cget, csave = 0;	/* save the character */

	port = cdev->cons_addr;					/* get the port address */	

	if ((csave = cdev->cons_csave) != 0)
		cdev->cons_csave = 0;
		
	do {	/* deal with flow control while waiting for transmit ready */
		
		if (cdev->cons_flags & SWSHAKE) {
			
			do {
				
				if (read_check(cdev)) {
					
					cget = IN(port+RBUF);
					if (cget == XOFF) {	/* we got an XOFF */
					
						cdev->cons_flags |= TXOFF;
					/*	continue; */
					
					} else if ((cdev->cons_flags & INSTRICT) == INSTRICT) {
					
						if (cget == XON) {	/* we got an XON */
							cdev->cons_flags ^= TXOFF;
						} else {
							csave = cget;
						/*	continue; */
						}
						
					} else if (cdev->cons_flags & TXOFF) {
						
						cdev->cons_flags ^= TXOFF;
						
					} else
					
						csave = cget;
						
				}
				
			} while ((cdev->cons_flags & TXOFF) != 0);
		}
		
	} while ((IN(port+STATR) & (1<<TXRDY)) == 0);	/* wait for xmit rdy */
	
	if (csave != 0)
		cdev->cons_csave = csave;		/* save character (or not) */

	OUT(port+TBUF, c);					/* output the character */

	return(SUCCESS);
}

/*
 * static u_int32 io68681_stat(Rominfo rinf, Consdev cdev, u_int32 code)
 */

u_int32 io68681_stat(Rominfo rinf, Consdev cdev, u_int32 code)
{
    Dat68681 data = (Dat68681) cdev->cons_data;
    u_int32 status = SUCCESS;
    
    switch(code)
    {
        case CONS_SETSTAT_ROMBUG_ON:
            /*  Save current interrupt status  */
            if(cdev->cons_flags & POLINT_ON)
            {
                /*  Saved polled interrupt on status  */
                cdev->cons_flags |= INT_STAT;
            }
            else
            {
                /*  Saved polled interrupt off status  */
                cdev->cons_flags &= ~INT_STAT;
            }
        case CONS_SETSTAT_POLINT_OFF:
            /* disable receive interrupt  */
            disable_port_rxints(data);
            /*  clear POLINT_ON flag  */
            cdev->cons_flags &= ~POLINT_ON;
            break;
            
        case CONS_SETSTAT_POLINT_ON:
            /* ensure timer present */
            if(rinf->timer == NULL)
                return(EOS_NOTIMER);
            /* set POLINT_ON flag */
            cdev->cons_flags |= POLINT_ON;
            /* enable receive interrupt  */
#if !defined(MC2681) && !defined(MC2682)
        /*
            The interrupt vector register is undefined on the MC2681
            and MC2682 devices
         */

           	OUT(data->uart_base+IVR, cdev->cons_vector);
#endif
           	enable_port_rxints(data);
            break;
            
        case CONS_SETSTAT_ROMBUG_OFF:
            /* enable previous interrupts */
            if(cdev->cons_flags & INT_STAT)
            {
                /* interrupts were previously enabled */
                cdev->cons_flags |= POLINT_ON;
                enable_port_rxints(data);
            }
            else
            {
                /* interrupts were previously disabled */
                disable_port_rxints(data);
                cdev->cons_flags &= ~POLINT_ON;
            }
            break;
            
        case CONS_SETSTAT_RXFLOW_OFF:
            if(cdev->cons_flags & HWSHAKE)
            {
                /* deassert RTS */
                if(data->devside == ASIDE)
                    OUT(data->uart_base+ROPR, DEACTIVATE<<CHARTS);
                else
                    OUT(data->uart_base+ROPR, DEACTIVATE<<CHBRTS);
            }
            else
            {
                if(cdev->cons_flags & SWSHAKE)
                {
                    io68681_write(XOFF, rinf, cdev);
                }
            }
            break;
            
        case CONS_SETSTAT_RXFLOW_ON:
            if(cdev->cons_flags & HWSHAKE)
            {
                /* assert RTS */
                if(data->devside == ASIDE)
                    OUT(data->uart_base+SOPR, ACTIVATE<<CHARTS);
                else
                    OUT(data->uart_base+SOPR, ACTIVATE<<CHBRTS);
            }
            else
            {
                if(cdev->cons_flags & SWSHAKE)
                {
                    io68681_write(XON, rinf, cdev);
                }
            }
            break;
            
        default:
            status = EOS_BADPARAM;
    }
    return(status);
}

/*
 * static u_int32 io68681_irq(Rominfo rinf, Consdev cdev)
 */
  
u_int32 io68681_irq(Rominfo rinf, Consdev cdev)
{
    Dat68681 data = (Dat68681) cdev->cons_data;
    u_int32 port = (u_int32) cdev->cons_addr;
    Tim_svcs timr = rinf->timer;
    u_int32 count;
    u_char havechar, cget;
    u_int32 overhead;
    
    /* validate our interrupt */
    
    if(data->devside == ASIDE)
    {
        if(!(IN(data->uart_base+ISR) & (1<<CHARXRDY)))
            return(EOS_NOTMINE);
    }
    else
    {
        if(!(IN(data->uart_base+ISR) & (1<<CHBRXRDY)))
            return(EOS_NOTMINE);
    }
    
    if((cdev->cons_flags & POLINT_ON) == 0)
        return(EOS_UNEXPINT);
        
    /* disable receive interrupts */
    disable_port_rxints(data);
        
    /* initialize the timer */
    (*timr->timer_init)(rinf);
    
    /* start polling for characters */
    while(FOREVER)
    {
        if(!(havechar = read_check(cdev)))
        {
            /* set the timer */
            (*timr->timer_set)(rinf, cdev->poll_timeout);
            
            /* loop waiting for character or timeout */
            do
            {
                count = (*timr->timer_get)(rinf);
                havechar = read_check(cdev);
            } while(!havechar && (count > 0));
        }
            
        if(!havechar)
            break;          /*  done  */
                    
		cget = IN((void *)(port+RBUF));			/* get the data */
        
        /* deliver character upline */
		(*timr->timer_set)(rinf, 0x80000000);
        (cdev->proto_upcall)(rinf, cdev->upcall_data, (char *)&cget);
		overhead = 0x80000000 - (*timr->timer_get)(rinf);
		if (overhead > data->maxoverhead)
			data->maxoverhead = overhead;
    }
    
    /* de-initialize the timer */
    (*timr->timer_deinit)(rinf);
    
    /* enable receive interrupt */
    enable_port_rxints(data);
        
    return(SUCCESS);
}

/*
 * static u_int32 term(rinf, cdev) - terminate the device
 */
 
u_int32 io68681_term(Rominfo rinf, Consdev cdev)
{
	register u_char		*port;			/* the port */
    Dat68681 data = (Dat68681) cdev->cons_data;
	Notify_svcs ntfy;
	
	port = cdev->cons_addr;				/* get the port address */
	
	/* disable interrupts           */
	disable_port_rxints(data);
	
	
	if (cdev->cons_flags & HWSHAKE)		/* negate ~RTS */
		io68681_stat(rinf, cdev, CONS_SETSTAT_RXFLOW_OFF);
	else
		/* wait for all transmissions to be done */
		while ((IN(port+STATR) & (1<<TXEMPTY)) == 0);
	
	/* disable transmit and receive */
	OUT(port+CMDR, (DISABLE<<TXCMDS) | (DISABLE<<RXCMDS));

	/* deregister notification handler */
	if((ntfy = rinf->notify) != 0)
	{
	    ntfy->dereg_hndlr(rinf, ((Dat68681) (cdev->cons_data))->hndlr_id);
	}
	
	return(SUCCESS);
}


/* driver module initialization */

error_code p2start(Rominfo rinf, u_char *glbls)
{
	Cons_svcs	console = rinf->cons;
	Consdev		cdev;
	
	/* verify a console services module exists */
	
	if (console == NULL)
		return (EOS_NOCONS); /* nope, leave it alone and go bye bye */
		
	/* makeup console port entry */
	
	cdev = &cons_r;
	cdev->struct_id = CONSDEVID;
	cdev->struct_ver = CDV_VER_MAX;
	cdev->cons_addr = (void *) CONSADDR;
	cdev->cons_probe = &io68681_probe;
	cdev->cons_init = &io68681_init;
	cdev->cons_term = &io68681_term;
	cdev->cons_read = &io68681_read;
	cdev->cons_write = &io68681_write;
	cdev->cons_check = &io68681_check;
	cdev->cons_stat = &io68681_stat;
	cdev->cons_irq = &io68681_irq;
	cdev->cons_baudrate = CONS_BAUDRATE_9600;
	cdev->cons_vector = CONSVECTOR;
	cdev->cons_priority = CONSPRIORITY;
	cdev->poll_timeout = 2000;
	cdev->cons_abname = (u_char *) CONSABNAME;
	cdev->cons_name = (u_char *) CONSNAME;
	
	cdev->cons_data = (void *) &cons_dat_r;
	cons_dat_r.devside = ASIDE;
	cons_dat_r.uart_base = (u_char *) CONSADDR;
    cons_dat_r.rinf = rinf;
#ifndef	SINGLE_PORT
    cons_dat_r.otherside = &comm_r;
#endif
#ifdef	_OSK
	cons_dat_r.oem_globals_offset = OEM_GLOBALS_OFFSET;
#endif
	
	cdev->cons_next = console->rom_conslist;
	console->rom_conslist = cdev;

#ifndef	SINGLE_PORT
	/* makeup comm port entry */
	
	cdev = &comm_r;
	cdev->struct_id = CONSDEVID;
	cdev->struct_ver = CDV_VER_MAX;
	cdev->cons_addr = (void *) COMMADDR;
	cdev->cons_probe = &io68681_probe;
	cdev->cons_init = &io68681_init;
	cdev->cons_term = &io68681_term;
	cdev->cons_read = &io68681_read;
	cdev->cons_write = &io68681_write;
	cdev->cons_check = &io68681_check;
	cdev->cons_stat = &io68681_stat;
	cdev->cons_irq = &io68681_irq;
	cdev->cons_baudrate = CONS_BAUDRATE_9600;
	cdev->cons_vector = COMMVECTOR;
	cdev->cons_priority = COMMPRIORITY;
	cdev->poll_timeout = 2000;
	cdev->cons_abname = (u_char *) COMMABNAME;
	cdev->cons_name = (u_char *) COMMNAME;
		
	cdev->cons_data = (void *) &comm_dat_r;
    comm_dat_r.devside = BSIDE;
   	comm_dat_r.uart_base = (u_char *) CONSADDR;
    comm_dat_r.rinf = rinf;
    comm_dat_r.otherside = &cons_r;
#ifdef	_OSK
	comm_dat_r.oem_globals_offset = OEM_GLOBALS_OFFSET;
#endif
	
	cdev->cons_next = console->rom_conslist;
	console->rom_conslist = cdev;
#endif
	
	return (SUCCESS);
}
