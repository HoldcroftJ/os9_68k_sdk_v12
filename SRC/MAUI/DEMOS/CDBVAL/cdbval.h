/*****************************************************************************
 *
 * Filename: cdbval.h
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


#ifdef _MAIN
	#define global
	#define global_v volatile
#else
	#define global extern
	#define global_v extern volatile
#endif


/*
 * Include Files
 */

#define _OPT_PROTOS

#include "history.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <MAUI/maui.h>


/******************************************************************************/


/*
 * Constants and Macros
 */

#define MW_RESERVED_DEV_TYPES 30000 /* 0 - 29999 are reserved by MAUI */


/******************************************************************************/


/*
 * Global Variables, Typedefs, and Structures
 */

/* used for the function msg() */
typedef enum _msg_type
{
	MT_ERROR= 1,
	MT_WARNING
} MSG_TYPE;

/* used to identify the type of value a parameter should have */
typedef enum _param_type
{
	PT_UNKNOWN= 0,      /* an initialization value */
	PT_NUMERIC,         /* a numeric value */
	PT_NUMERIC_2_COMMA, /* two comma separated numeric values */
	PT_STRING,          /* a string enclosed in '"' characters */
	PT_BOOLEAN          /* a parameter that does not have a value */
} PARAM_TYPE;

/*
 * Used to identify whether a particular parameter is required
 * by the device.
 */
typedef enum _dev_param_conditions
{
	DPC_DAR= 1,   /* condition (D)etermined (A)t (R)untime */
	DPC_REQUIRED, /* parameter MUST be specified */
	DPC_OPTIONAL  /* parameter MAY be specified */
} DEV_PARAM_CONDITIONS;

/*
 * Some string parameters can only have certain values.
 * This structure is used for that list and is pointed
 * to by the DEV_PARAMS structure.
 */
typedef struct _param_vals
{
	char                 *pv_name;
	struct _param_vals   *pv_next;
} PARAM_VALS;

/*
 * This data structure is used to hold a list
 * of registered parameters for a particular
 * device type.
 */
typedef struct _dev_params
{
	char                 *dp_name;
	PARAM_TYPE           dp_type;
	DEV_PARAM_CONDITIONS dp_conditions;
	u_int32              dp_count;
	PARAM_VALS           *dp_vals;
	struct _dev_params   *dp_next;
} DEV_PARAMS;

/*
 * This data structure is used to hold the list
 * of registered device types.
 */
typedef struct _dev_type
{
	char                 *dt_name;
	CDB_TYPE             dt_type;
	DEV_PARAM_CONDITIONS dt_conditions;
	u_int32              dt_count;
	DEV_PARAMS           *dt_params;
	struct _dev_type     *dt_next;
} DEV_TYPE;


global char     *g_help;
global u_int8   g_opt_s;
global char     g_colon;
global char     g_cr;
global char     g_slash;
global char     *g_sys;
global u_int32	g_warnings_total;
global u_int32	g_errors_total;
global DEV_TYPE *g_dev_first;


/******************************************************************************/


/*
 * Prototypes
 */

char       *check_ddr(char *ddr, DEV_TYPE *dev_type, u_int32 e_num);
char       *check_device_name(char *ddr, DEV_TYPE *dev_type, u_int32 entry_num, char *dev_name);
char       *check_device_param(char *ddr, DEV_TYPE *dev_type, u_int32 entry_num, char *param, u_int32 param_num);
char       *check_device_type(char *ddr, u_int32 entry_num);
error_code cidata_to_ll(void);
void       validate_char(u_int32 entry_num, char *ddr, char *ch);
void       exit_prog(error_code error, char *cntl, ...);
error_code get_cdb_data(char **cdb_data, size_t *cdb_data_size);
void       init_globals(void);
int        main(u_int32 argc, char *argv[]);
void       msg(MSG_TYPE ew, char *ddr, u_int32 entry_num, char *cntl, ...);
void       parse_args(u_int32 argc, char *argv[]);
void       print_ll(void);
char       *skip_to_next_ddr(char *p);
error_code validate_general_structure(char *mod);
error_code validate_specific_structure(void);


/******************************************************************************/
