/*****************************************************************************
 *
 * Filename: ll.c
 *  Purpose: Functions for handling the registered device types linked lists.
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
#include "dev_types.h"


/*****************************************************************************
	CONVERT INITIALIZED DATA TO LINKED LIST

	This function converts the initialized DEV_TYPE, DEV_PARAMS, and
	PARAM_VALS arrays into linked list structures that are used to verify
	the *registered* MAUI CDB_TYPEs.
*****************************************************************************/


error_code
cidata_to_ll(void)
{
	DEV_TYPE   *dt= g_devs_table;
	DEV_TYPE   *dt_end= & g_devs_table[sizeof(g_devs_table) / sizeof(DEV_TYPE) - 1];
	DEV_TYPE   *ll_dt= NULL;
	DEV_PARAMS *dp= g_params_table;
	DEV_PARAMS *ll_dp= NULL;
	PARAM_VALS *pv= g_vals_table;
	PARAM_VALS *ll_pv= NULL;
	void       *tmp;

	g_dev_first= NULL;

	/* initialize a device type structure */
	while(dt < dt_end)
	{
		/* allocate device type link */
		tmp= malloc(sizeof(DEV_TYPE));
		if(tmp == NULL)
			exit_prog(EOS_MEMFUL, "malloc(DEV_TYPE) failed!");

		if(g_dev_first == NULL)
			g_dev_first= ll_dt= tmp;
		else
		{
			ll_dt->dt_next= tmp;
			ll_dt= ll_dt->dt_next;
		}

		/* initialize device type structure */
		*ll_dt= *dt++;

		/* initialize the parameter list for this device type */
		ll_dp= NULL;
		while(dp->dp_name != NULL)
		{
			/* allocate device parameter link */
			tmp= malloc(sizeof(DEV_PARAMS));
			if(tmp == NULL)
				exit_prog(EOS_MEMFUL, "malloc(DEV_PARAMS) failed!");

			if(ll_dt->dt_params == NULL)
				ll_dt->dt_params= ll_dp= tmp;
			else
			{
				ll_dp->dp_next= tmp;
				ll_dp= ll_dp->dp_next;
			}

			/* initialize device parameter structure */
			*ll_dp= *dp++;

			/* initialize the legal values list for this parameter */
			ll_pv= NULL;
			while(pv->pv_name != NULL)
			{
				/* allocate device parameter link */
				tmp= malloc(sizeof(PARAM_VALS));
				if(tmp == NULL)
					exit_prog(EOS_MEMFUL, "malloc(PARAM_VALS) failed!");

				if(ll_dp->dp_vals == NULL)
					ll_dp->dp_vals= ll_pv= tmp;
				else
				{
					ll_pv->pv_next= tmp;
					ll_pv= ll_pv->pv_next;
				}

				/* initialize device parameter structure */
				*ll_pv= *pv++;
			}

			pv++;

			if(ll_pv != NULL)
				ll_pv->pv_next= NULL;
		}

		dp++;

		if(ll_dp != NULL)
			ll_dp->dp_next= NULL;
		else
			pv++;
	}

	if(ll_dt != NULL)
		ll_dt->dt_next= NULL;

	return SUCCESS;
}


/*****************************************************************************
	PRINT LINKED LIST

	This function prints out the contents of the device type linked list.
*****************************************************************************/


void
print_ll(void)
{
	DEV_TYPE   *ll_dt;
	DEV_PARAMS *ll_dp;
	PARAM_VALS *ll_pv;
	int        padd;

	 printf("\
Device Type                    Parameters  Legal Values\n\
-----------------------------  ----------  ------------------------------------\n");

	/* print device types */
	ll_dt= g_dev_first;
	while(ll_dt != NULL)
	{
		/* print device type */
		printf("%10d: %s", ll_dt->dt_type, ll_dt->dt_name);

		if(ll_dt->dt_params != NULL)
		{
			padd= 29 - 12 /* 12 for "%10d: " */
				- strlen(ll_dt->dt_name);
			while(padd-- > 0)
				putchar(' ');

			/* print the device type parameters */
			ll_dp= ll_dt->dt_params;
			while(ll_dp != NULL)
			{
				if(ll_dp == ll_dt->dt_params)
					printf("%6s%-2s", "", ll_dp->dp_name);
				else
					printf("%35s%-2s", "", ll_dp->dp_name);

				/* print the legal values for the parameter */
				ll_pv= ll_dp->dp_vals;
				while(ll_pv != NULL)
				{
					if(ll_pv == ll_dp->dp_vals)
						printf("      %s", ll_pv->pv_name);
					else
						printf(", %s", ll_pv->pv_name);

					ll_pv= ll_pv->pv_next;
				}

				putchar(g_cr);

				ll_dp= ll_dp->dp_next;
			}

			if(ll_dt->dt_params == NULL)
				putchar(g_cr);
		}
		else
			putchar(g_cr);

		putchar(g_cr);

		ll_dt= ll_dt->dt_next;
	}

	return;
}


/****************************************************************************/
