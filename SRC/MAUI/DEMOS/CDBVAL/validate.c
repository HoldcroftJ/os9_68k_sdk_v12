/*****************************************************************************
 *
 * Filename: validate.c
 *  Purpose: A validation utility for CDB modules
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


/*****************************************************************************
	VALIDATE GENERAL STRUCTURE

	This function initializes all of our global variables.
*****************************************************************************/


error_code
validate_general_structure(
	char *mod
	)
{
	char        *mod_name= mod;
	mh_com      *mod_head;
	void        *mod_exec;
	u_int16     type_lang;
	u_int16     attr_rev;
	u_int32     e_num;
	char        *ddr;
	char        *p;
	char        *eom;
	status_code status;

	printf("\nModule \"%s\":\n", mod);

	/* link to the module */
	type_lang= 0;
	status= _os_link(	& mod_name,
						& mod_head,
						& mod_exec,
						& type_lang,
						& attr_rev);
	if(status != SUCCESS)
		exit_prog(status, "_os_link(\"%s\") failed!", mod);

	/* make sure this is a CDB module */
	if((type_lang >> 8) != MT_CDBDATA)
		exit_prog(EOS_PARAM, "The module \"%s\" is not a CDB module!", mod);

	/* calculate the start and end of the module's contents */
	p= mod_exec;
	eom= ((char*)mod_head) + MODSIZE(mod_head);

	/* examine module contents */
	e_num= 1;
	ddr= p;
	g_errors_total= 0;
	while(*p  &&  p < eom)
	{
		/* only printable characters and carriage returns are valid */
		validate_char(e_num, ddr, p);

		/* is this the beginning of a new DDR */
		if(*p == g_cr)
		{
			if(*(p - 1) != g_colon)
				msg(MT_ERROR, ddr, e_num, "DDR does not end with a '%c':", g_colon);

			e_num++;
			ddr= p + 1;
		}

		p++;
	}

	/* did we find the NULL terminator */
	if(p == eom)
		msg(MT_ERROR, ddr, e_num, "the CDB data in \"%s\" is not NULL terminated:", mod);

	/* did we find the NULL terminator in the right place */
	if(*(p - 1) != g_cr)
		msg(MT_ERROR, ddr, e_num, "there is no carriage return before the NULL terminator:");

	printf("Total DDRs found    : %d\n", e_num - 1);
	printf("Total errors found: %d\n", g_errors_total);

	status= _os_unlink(mod_head);
	if(status != SUCCESS)
		exit_prog(status, "_os_unlink(\"%s\") failed!", mod);

	return SUCCESS;
}


/*****************************************************************************
	VALIDATE SPECIFIC STRUCTURE

	This function validates the individual CDB entries for all CDB modules
	in memory.
*****************************************************************************/


error_code
validate_specific_structure(void)
{
	char        *cdb_data;
	size_t      cdb_data_size;
	DEV_TYPE    *dt;
	u_int32     e_num;
	u_int32     dev_type;
	char        *p;
	char        *eod;
	char        *ddr;
	status_code status;

	status= get_cdb_data( & cdb_data, & cdb_data_size);
	if(status != SUCCESS)
		exit_prog(status, "get_cdb_data() failed!");

	/* examine CDB entries */
	e_num= 1;
	p= cdb_data;
	eod= cdb_data + cdb_data_size;
	g_warnings_total= 0;
	g_errors_total= 0;
	while(*p  &&  p < eod)
	{
		ddr= p;

		/* get device type */
		dev_type= atoi((char*)p);

		/* try to find device type */
		dt= g_dev_first;
		while(dt != NULL  &&  dt->dt_type != dev_type)
			dt= dt->dt_next;

		if(dt != NULL)
			dt->dt_count++;
		else if(dev_type < MW_RESERVED_DEV_TYPES)
			msg(MT_ERROR, ddr, e_num, "device type %d is not registered and within the Microware reserved range of 0 - %d:"
				, dev_type, MW_RESERVED_DEV_TYPES - 1); /* DDR error 1 */

		/* check the specifics of the ddr */
		p= check_ddr(p, dt, e_num);

		/* we should have ended on a carriage return */
		if(*p != g_cr)
		{
			msg(MT_ERROR, ddr, e_num, "entry does not end with a carriage return, skipping to next DDR:"); /* DDR error 6 */

			/*
			 * Attempt recovery by skipping to the end of this DDR and returning.
			 * Upon return, we will process the next DDR.
			 */
			p= skip_to_next_ddr(p);
		}

		/* skip past the carriage return */
		p++;

		e_num++;
	}

	/* make sure required device types appeared */
	dt= g_dev_first;
	while(dt != NULL)
	{
		if(dt->dt_conditions == DPC_REQUIRED  &&  dt->dt_count == 0)
			msg(MT_ERROR, NULL, e_num, "Required device type \"%s\" was not found!"
				, dt->dt_name);

		dt= dt->dt_next;
	}

	printf("Total DDRs found    : %d\n", e_num - 1);
	printf("Total warnings found: %d\n", g_warnings_total);
	printf("Total errors found  : %d\n", g_errors_total);

	return SUCCESS;
}


/*****************************************************************************
	CHECK DDR

	This function validates an individual DDR entry. If dev_type is NULL,
	then this DDR device type is not registered and therefore some tests
	are not performed.

	Note that the following are not checked:
		- That all device names are unique.
*****************************************************************************/


char *
check_ddr(
	char     *ddr,
	DEV_TYPE *dev_type,
	u_int32  e_num
	)
{
	DEV_PARAMS *dp;
	char       *p= ddr;
	char       *p_save;
	u_int32    len;
	u_int32    p_count;

	/* initialize counters for this device type */
	if(dev_type != NULL)
	{
		dp= dev_type->dt_params;
		while(dp != NULL)
		{
			dp->dp_count= 0;
			dp= dp->dp_next;
		}
	}

	/*
	 * CHECK DEVICE TYPE
	 */
	p= check_device_type(ddr, e_num);

	/* we should have ended on a colon */
	if(*p != g_colon)
	{
		msg(MT_ERROR, ddr, e_num, "the device type does not end with a '%c' character:"
			, g_colon); /* DDR error 2 */

		/*
		 * Attempt recovery by skipping to the end of this DDR and returning.
		 * Upon return, we will process the next DDR.
		 */
		return skip_to_next_ddr(p);
	}

	/* skip past the colon */
	p++;

	/*
	 * CHECK DEVICE NAME
	 */
	p= check_device_name(ddr, dev_type, e_num, p);

	/* we should have ended on a colon */
	if(*p != g_colon)
	{
		msg(MT_ERROR, ddr, e_num, "the device name does not end with a '%c' character:"
			, g_colon); /* DDR error 3 */

		/*
		 * Attempt recovery by skipping to the end of this DDR and returning.
		 * Upon return, we will process the next DDR.
		 */
		return skip_to_next_ddr(p);
	}

	/* skip past the colon */
	p++;

	/*
	 * CHECK DEVICE PARAMETERS
	 */
	len= 0;
	p_count= 1;
	while(*p != g_cr  &&  *p)
	{
		p_save= p;
		p= check_device_param(ddr, dev_type, e_num, p_save, p_count);
		len += p - p_save + 1; /* "+ 1" for the ':' */

		/* we should have ended on a colon */
		if(*p != g_colon)
		{
			msg(MT_ERROR, ddr, e_num, "parameter %d does not end with a '%c' character:"
				, p_count, g_colon); /* DDR error 4 */

			/*
			 * Attempt recovery by skipping to the end of this DDR and returning.
			 * Upon return, we will process the next DDR.
			 */
			return skip_to_next_ddr(p);
		}

		p_count++;

		/* skip past the colon */
		p++;
	}

	/* was the parameter list within the length limit */
	if(len > CDB_MAX_PARAM)
		msg(MT_ERROR, ddr, e_num, "the parameter list is longer than CDB_MAX_PARAM (%d):"
			, CDB_MAX_PARAM); /* DDR error 5 */

	/* make sure that all DPC_REQUIRED parameters have been found. */
	if(dev_type != NULL)
	{
		dp= dev_type->dt_params;
		while(dp != NULL)
		{
			if(dp->dp_conditions == DPC_REQUIRED)
			{
				if(dp->dp_count == 0)
					msg(MT_ERROR, ddr, e_num, "the required parameter \"%s\" is missing:"
						, dp->dp_name); /* DDR error 7 */
			}

			if(dp->dp_count > 1)
				msg(MT_WARNING, ddr, e_num, "the parameter \"%s\" occurs %d times:"
					, dp->dp_name, dp->dp_count); /* DDR warning 1 */

			dp= dp->dp_next;
		}
	}

	return p;
}


/*****************************************************************************
	VALIDATE CHARACTER

	This function tests to see if a character is invalid.
*****************************************************************************/


void
validate_char(
	u_int32 entry_num,
	char    *ddr,
	char    *ch
	)
{
	/* only printable characters and carriage returns are valid */
	if(isprint(*ch) == 0  &&  *ch != g_cr)
		msg(MT_ERROR, ddr, entry_num, "character %d is invalid (0x%02x):"
			, ch - ddr + 1, *ch); /* DDR error 8 */

	return;
}


/*****************************************************************************
	SKIP TO NEXT DDR

	This function is used when we have found an error that we are
	pretty sure will mess up any further processing on the current DDR.
	We kip to the end of the current DDR and return a pointer to the
	carriage return at the end.
*****************************************************************************/


char *
skip_to_next_ddr(
	char *p
	)
{
	while(*p)
	{
		if(*p == g_cr  ||  *(p - 1) == g_colon)
			break;

		p++;
	}

	/*
	 * If we hit the end of the CDB data, back up one so that
	 * we catch the NULL in the calling function(s).
	 */
	if(*p == '\0')
		return p--;

	return p;
}


/*****************************************************************************
	CHECK DEVICE TYPE

	Verify the device type section of the DDR.
*****************************************************************************/


char *
check_device_type(
	char    *ddr,
	u_int32 entry_num
	)
{
	char *p= ddr;

	/*
	 * Make sure that the device type is a numeric field
	 * without any other chars.
	 */
	while(*p != g_colon  &&  *p != g_cr  &&  *p)
	{
		if(isdigit(*p++) == 0)
			msg(MT_ERROR, ddr, entry_num, "character %d of the device type is non-numeric (0x%02x):"
				, p - ddr, *(p - 1)); /* DDR error 9 */
	}

	return p;
}


/*****************************************************************************
	CHECK DEVICE NAME

	Verify the device name section of the DDR.
*****************************************************************************/


char *
check_device_name(
	char     *ddr,
	DEV_TYPE *dev_type,
	u_int32  entry_num,
	char     *dev_name
	)
{
	char   *p= dev_name;
	char   buf[CDB_MAX_DNAME + 1];
	char   *bp;
	u_int8 sys;

	/* get the device name */
	bp= buf;
	while(*p != g_colon  &&  *p != g_cr  &&  *p)
	{
		/* only printable characters and carriage returns are valid */
		validate_char(entry_num, ddr, p);

		/* don't overwrite "buf" */
		if((bp - buf) <= CDB_MAX_DNAME)
			*bp++ = *p;

		p++;
	}

	*bp= '\0';

	sys= FALSE;
	if(dev_type != NULL)
	{
		if(dev_type->dt_type == CDB_TYPE_SYSTEM)
		{
			sys= TRUE;

			/* the device name for CDB_TYPE_SYSTEM must be "sys" */
			if(strcmp(buf, g_sys) != 0)
				msg(MT_ERROR, ddr, entry_num, "the name for device type CDB_TYPE_SYSTEM must be \"%s\":"
					, g_sys); /* DDR error 26 */
		}

		/*
		 * Change the conditions for special parameters based upon
		 * whether the device name is "/enet" and the device type
		 * is either CDB_TYPE_CTRLCHAN or CDB_TYPE_DATACHAN.
		 */
		else if(dev_type->dt_type == CDB_TYPE_CTRLCHAN
			||  dev_type->dt_type == CDB_TYPE_DATACHAN)
		{
			DEV_PARAMS           *dp= dev_type->dt_params;
			DEV_PARAM_CONDITIONS cond;

			if(strcmp(buf, "/enet") == 0)
				cond= DPC_REQUIRED;
			else
				cond= DPC_OPTIONAL;

			while(dp != NULL)
			{
				/*
				 * These two device parameters are required when
				 * the device name is "/enet"
				 */
				if(strcmp(dp->dp_name, "SH") == 0
							||  strcmp(dp->dp_name, "SS") == 0)
					dp->dp_conditions= cond;

				dp= dp->dp_next;
			}
		}
	}

	/* the device name must begin with a '/' (with the exception of "sys") */
	if(buf[0] != g_slash  &&  sys == FALSE)
		msg(MT_ERROR, ddr, entry_num, "the device name must begin with a '%c' character:"
			, g_slash); /* DDR error 10 */

	/* is the device name within the length limit */
	if(strlen(buf) > CDB_MAX_DNAME)
		msg(MT_ERROR, ddr, entry_num, "the device name is longer than CDB_MAX_DNAME (%d):"
			, CDB_MAX_DNAME); /* DDR error 11 */

	return p;
}


/*****************************************************************************
	CHECK DEVICE PARAM

	Verify a single parameter from the parameter list.

	The parameter will have the following syntax:

		<PARAM><DATA_TYPE><VALUE>:

		PARAM:     a two character achronym
		DATA_TYPE: either a '#', a '=', or, in the case of a
		           boolean PARAM, nothing
		VALUE:     a value appropriate for PARAM, in the case
		           of a boolean PARAM, nothing
*****************************************************************************/


char *
check_device_param(
	char     *ddr,
	DEV_TYPE *dev_type,
	u_int32  entry_num,
	char     *param,
	u_int32  param_num
	)
{
	DEV_PARAMS *dp;
	PARAM_VALS *pv;
	char       buf[CDB_MAX_PARAM + 1];
	char       *bp;
	char       *p= param;
	char       *err_msg= "parameter %d should have a '%c' delimiter between the parameter name and value:";
	PARAM_TYPE param_type= PT_UNKNOWN;
	u_int32    len;
	u_int32    comma_count;

	/* get parameter name */
	bp= buf;
	while(*p != g_colon  &&  *p != '#'  &&  *p != '='  &&  *p != g_cr  &&  *p)
	{
		/* only printable characters and carriage returns are valid */
		validate_char(entry_num, ddr, p);

		/* parameter names are restricted to two upper-case alpha characters */
		if(isupper(*p) == 0)
			msg(MT_ERROR, ddr, entry_num, "parameter %d, parameter names must be two upper-case alpha characters:"
				, param_num); /* DDR error 12 */

		if((bp - buf) <= 3)
			*bp++ = *p;

		p++;
	}

	*bp= '\0';

	/* is parameter name too short (this is probably a case of "::") */
	len= strlen(buf);
	if(len == 0)
		msg(MT_ERROR, ddr, entry_num, "parameter %d is of zero length:"
			, param_num); /* DDR error 13 */

	/* is parameter name too long (maximum length is 2) */
	if(len != 2)
		msg(MT_ERROR, ddr, entry_num, "parameter %d is not two characters long:"
			, param_num); /* DDR error 14 */

	/*
	 * If this is a registered device type, determine whether this
	 * parameter is registered for this device type.
	 */
	dp= NULL;
	pv= NULL;
	if(dev_type != NULL)
	{
		/* look for the parameter in the device's list */
		dp= dev_type->dt_params;
		while(dp != NULL  &&  strcmp(buf, dp->dp_name))
			dp= dp->dp_next;

		if(dp != NULL)
		{
			param_type= dp->dp_type;
			dp->dp_count++;
			pv= dp->dp_vals;
		}
		else
		{
			msg(MT_WARNING, ddr, entry_num, "parameter %d is not registered for this device type:"
				, param_num); /* DDR warning 2 */

			/*
			 * If we did not find that this is a registered parameter, try
			 * and determine what it is ourselves.
			 */
			if(*p == '=')
				param_type= PT_STRING;
			else if(*p == '#')
				param_type= PT_NUMERIC;
			else if(*p == ':')
				param_type= PT_BOOLEAN;
			else
			{
				msg(MT_ERROR, ddr, entry_num, "parameter %d is of an unknown type (not string, numeric, or boolean):"
					, param_num); /* DDR error 15 */

				/*
				 * If the parameter type is unknown, we can't check
				 * anything else, so skip to the end of this parameter.
				 */
				while(*p  &&  *p != g_colon  &&  *p != g_cr)
					p++;

				return p;
			}
		}
	}

	/* check parameter data type */
	switch(param_type)
	{
		case PT_STRING:
			if(*p != '=')
				msg(MT_ERROR, ddr, entry_num, err_msg, param_num, '=');  /* DDR error 16 */
			break;

		case PT_NUMERIC:
		case PT_NUMERIC_2_COMMA:
			if(*p != '#')
				msg(MT_ERROR, ddr, entry_num, err_msg, param_num, '#'); /* DDR error 17 */
			break;

		case PT_BOOLEAN:
			if(*p != ':')
				msg(MT_ERROR, ddr, entry_num, "parameter %d should not have a value:", param_num); /* DDR error 18 */

			/*
			 * If the parameter data type is boolean, there is nothing more
			 * to do.
			 */
			return p;

		default:
			/*
			 * There is no default because if we did not determine the
			 * parameter type above, we would have returned.
			 */
			break;
	}

	/* skip past parameter data type symbol ('=' or '#') */
	p++;

	/* if the parameter data type is PT_STRING, make sure the value begins with a '"' char */
	if(param_type == PT_STRING)
	{
		if(*p != '"')
			msg(MT_ERROR, ddr, entry_num, "parameter %d is of type string. The parameter value must begin with a '\"':"
				, param_num); /* DDR error 19 */

		p++; /* skip the opening '"' */
	}

	/* get parameter value */
	bp= buf;
	comma_count= 0;
	while(*p  &&  *p != g_colon  &&  *p != g_cr)
	{
		if(param_type == PT_NUMERIC  ||  param_type == PT_NUMERIC_2_COMMA)
		{
			if(isdigit(*p) == 0)
			{
				if(*p == ',')
				{
					if(param_type == PT_NUMERIC_2_COMMA)
					{
						comma_count++;
						if(comma_count > 1)
							msg(MT_ERROR, ddr, entry_num, "parameter %d, too many commas in the numeric value:"
								, param_num); /* DDR error 20 */
					}
					else
						msg(MT_ERROR, ddr, entry_num, "parameter %d, commas are not allowed in this parameter type:"
							, param_num); /* DDR error 21 */
				}
				else
					msg(MT_ERROR, ddr, entry_num, "parameter %d, character %d of the parameter value is non-numeric (0x%02x):"
						, param_num, bp - buf + 1, *p); /* DDR error 22 */
			}
		}
		else
			validate_char(entry_num, ddr, p);

		/* don't overwrite "buf" */
		if((bp - buf) <= CDB_MAX_PARAM)
			*bp++ = *p;

		p++;
	}

	if(param_type == PT_NUMERIC_2_COMMA  &&  comma_count == 0)
		msg(MT_ERROR, ddr, entry_num, "parameter %d, the comma is missing from the value:"
			, param_num); /* DDR error 23 */

	/* if the parameter data type is PT_STRING, make sure the value ends with a '"' char */
	if(param_type == PT_STRING)
	{
		if(*(p - 1) != '"')
			msg(MT_ERROR, ddr, entry_num, "parameter %d is of type string. The parameter value must end with a '\"':"
				, param_num); /* DDR error 24 */

		*(bp - 1)= '\0'; /* remove the closing '"' from the device value */
	}
	else
		*bp= '\0';

	/*
	 * If this is a registered device type and parameter, and it
	 * has a set of "legal" values, check this value against it.
	 */
	if(pv != NULL)
	{
		/* look for the parameter value */
		pv= dp->dp_vals;
		while(pv != NULL  &&  strcmp(buf, pv->pv_name))
			pv= pv->pv_next;

		if(pv == NULL)
			msg(MT_ERROR, ddr, entry_num, "parameter %d, \"%s\" is not a legal value for this parameter:"
				, param_num, buf); /* DDR error 25 */
	}
	/* else, there isn't anything else to check */

	return p;
}


/****************************************************************************/
