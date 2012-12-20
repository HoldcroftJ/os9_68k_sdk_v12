/****************************************************************************
 *           Copyright 1989-1997 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 * Definitions file for making SPF device descriptors.                      *
 ****************************************************************************/

#include <SPF/item.h>

#ifdef enet
/***************************************************************************/
/****              Device Descriptor for SPF spenet driver              ****/
/****                                                                   ****/
/****   This section contains the configurable parameters for spenet    ****/
/***************************************************************************/

/*
** spip allows multiple IP addresses to be associated with a single
** interface. This value is the maximum number of addresses allowed
** on each ethernet interface.
*/
#define MAXADDR_PER_IFACE 4

/*
** spenet runs a cyclic timer that is used to remove old arp entries.
** This value defines the timer interval in seconds.
*/
#define TIMER_INT 60

/*
** If a completed (ie. we received an arp reply) entry is not used in
** this many timer intervals it is deleted.
*/
#define KILL_C 20

/*
** If an entry remains incomplete (ie. no arp reply received for our
** arp request) for this many timer intervals it is deleted.
*/
#define KILL_I 3


/***************************************************************************/
/****            Nothing below this line should be changed              ****/
/***************************************************************************/

/*
** We use the logical unit number as version number so the driver
** can tell when it has an old descriptor and use sensible defaults
** for the new values.
*/
#define LUN			6			/* logical unit number */

#define DRV_NAME	"spenet"	/* device driver name */

#define TXSIZE		1500		/* transmit packet max size */
#define TXOFFSET	14			/* required header space */

#define PROTTYPE	SPF_PR_ENET	/* Template (transparent) protocol ID	*/

#include <SPENET/defs.h>

#endif /* enet */
