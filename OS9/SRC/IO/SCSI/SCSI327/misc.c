/*
 * misc - miscellaneous functions for vme327 low level module
 */
 
/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include "scsi327.h"

/*
 *  initdrv()  --  perform any initialization nessesary for a specific 
 *                 device.
 */
int initdrv( lst, tarid )
register Lstat lst;			/* static storage */
int tarid;
{
	register int error;				/* error return */
	register int *evslot;			/* event ID pointer */
	int sink;                       /* the bag which holds unnessesary things */
	char name[16];					/* place to build names */
	extern void makeevname();
	
	evslot = (int *) &lst->sc_events[tarid];	/* pointer to the ID */

	if( *evslot == 0 ) {				/* create an event for this device */
		makeevname( name, lst->hdware, tarid );	/* generate name */
		if( (error = _fev_creat(CBFREE, -1, 1, name, evslot, &sink)))
				return error;			/* could not create the event !!!! */
	}
	return 0;		/* all ok */
}

/*
 * clearmem(mptr, size) - clear a number of bytes
 */
 
void clearmem(mptr, size)
register char	*mptr;			/* memory block ptr */
register int	size;			/* memory size */
{
	register int	i;			/* general purpose int */
	
	for (i = 0; i < size; i++)
		*mptr++ = 0;
}

/*
 * lxtoa(stg, num) - convert long to 8 ascii hex digits 
 */
void lxtoa(stg, num)
register char	*stg;
register long	num;
{
	register int	i;
	
	for (i = 7; i >= 0; i--, num >>= 4)
		stg[i] = hexchar(num & 0xf);
	stg[8] = '\0';		/* terminate */
}

/*
 * int hexchar(n) - return ascii hex equivalent of n 
 */
int hexchar(n)
register int	n;
{
	if (n > 9)
		n += 0x27;		/* for range a - f */
	return (n + 0x30);
}

/*
 * strcpy(s1, s2) - another version of string copy
 */
strcpy(s1, s2)
register char	*s1, *s2;
{
	while (*s1++ = *s2++);
}

/* 
 *  makeevname - create an event name
 *
 * name conventions for events:
 * event names are built in the form 
 * "c<portaddress>", where:
 *
 * c =  'i' for interrupt service event.
 * <portaddress> is the name of the device port
 * address "ANDED" with 0xfffffff0.  Added to the
 * end is an '_x' where x is the SCSI address of the
 * target device.
 *
 * e.g. for a device at 0xfffe4000, the event
 * names is:
 *
 *          irq event:  "ifffe4000_x"
 */
 
void makeevname( cpt, addr, id )
register char *cpt;		/* name string to fill */
long addr;				/* port address */
int id;					/* device SCSI ID */
{
	cpt[0] = 'i';			/* flag it as an interrupt event */
	lxtoa(&cpt[1], addr);	/* board address */
	cpt[9] = '_';
	cpt[10] = '0' + id;		/* id number */
	cpt[11] = 0;			/* terminate the string */
}
