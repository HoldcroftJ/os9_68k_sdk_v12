/****************************************************************************
 * File:	%A%
 * Date:	%G%
 ****************************************************************************
 * In-band and out-of-band support definitions for SPF
 ****************************************************************************
 * Copyright 1995 by Microware Systems Corporation                          *
 * Reproduced Under License                                                 *
 *                                                                          *
 * This source code is the proprietary confidential property of             *
 * Microware Systems Corporation, and is provided to licensee               *
 * solely for documentation and educational purposes. Reproduction,         *
 * publication, or distribution in any form to any party other than         *
 * the licensee is strictly prohibited.                                     *
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                            By      *
 * --- -------- -------------------------------------------------   ---     *
 *   1 04/10/96 Creation for out-of-band/in-band support for SPF    cas     *
 *   2 08/10/96	Added OOB for ATM support			    udayn   *
 * 231 04/17/97 <---------------- SPF v2.2 (beta)--------------->   taka    *
 * 231 04/21/97 renamed item call prototype ite_path_getprofile()           *
 *              and ite_path_setprofile() to ite_path_profileget()          *
 *              and ite_path_profileset() respectively              taka    *
 * 226 06/26/97 <---------------- SPF V2.2 ---------------------->  taka    *
 *     03/25/98 cosmetic change to remove a compiler warning        taka    *
 *              <---------------- SPF v2.2.1 -------------------->  taka    *
 * 255 07/20/98 <---------------- SPF v2.2.2 -------------------->  taka    *
 ****************************************************************************/

#if !defined(_SPF_OOB_H)
#define _SPF_OOB_H

#include <types.h>
#include <SPF/item.h>

/****************************************************************************/

typedef struct ib_cfg_pb  ib_cfg_pb;
typedef struct ib_cfg_pb* Ib_cfg_pb;

#define IB_NAME_SIZE	16

/* In-band configuration parameter block. Returned by out of band protocols
 * when an ite_ctl_resadd() API call is made. This is a generalization
 * of the resource configuration parameter block used by the DSM-cc
 * User/network protocol driver for creating resources for an interactive
 * session. There, many in-band resources may be created as a result of
 * a single connection reqeust. Typical standard telecomm protocols create
 * only one in-band resource for every out-of-band connection request.
 */

struct ib_cfg_pb {

	Ib_cfg_pb	ib_next;		/* Next pointer for list		*/
	char		ib_name[IB_NAME_SIZE];	/* Descriptor to open IB path	*/
	u_int32		oob_syspath;		/* Out of band protocol system path	*/
	u_int16		ib_flags;

		#define	NEW_IB_RES	0x0001	/* New In-band resource*/
		#define	CLEAR_NEWIB	0xFFFE	/* Clear NEW_IB_RES bit after listing	*/

	u_int16		ib_obj_type;

		#define IB_OBJ_ATM	1	/* ATM object parameters in ib_object	*/
		#define IB_OBJ_BRI	2	/* Basic Rate ISDN parameters			*/
		
	u_int8		ib_object[32];		/* In-band object cast per IB type	*/

	error_code  (*ib_callback)(Ib_cfg_pb ibpb, void *spb);
	error_code  (*oob_callback)(Ib_cfg_pb ibpb, void *spb);

		#define IB_STATE_CHANGE 0x01    /* Change in oob connection state	*/
		#define IB_CHECK_IN 	0x02    /* In-band checks in when opened	*/
		#define CB_CONN_TERM    0x03    /* In-band conn being terminated	*/

	void        *ib_deventry;
	void        *oob_deventry;
	u_char      ib_state;      		/* The current state of this resource entry */

		#define IB_FREE         0x00
		#define IB_AWAIT_ADD    0x01
		#define IB_ACTIVE       0x02
		#define IB_AWAIT_DEL    0x03
		#define IB_RESERVED     0x04 	/* Placeholder for group IB handling  */
		#define IB_WAIT_CHECKIN 0x05

	u_char      ib_rsv2[3];
};


/**** Object structures for casting ib_object array on it's ib_obj_type ****/

/* Structure for in-band object IB_OBJ_ATM */
typedef struct ibobj_atm {
	u_int16	atm_vpi,
			atm_vci;
} ibobj_atm, *Ibobj_atm; 

/* Structure for in-band object IB_OBJ_BRI */
typedef struct ibobj_bri {
	u_char	bri_callref;			/* Call reference		*/
	u_char	bri_chanid;				/* Channel ID			*/
		#define BRI_CHAN_B1	1
		#define BRI_CHAN_B2	2
	u_char	bri_rsv1[2];			/* Long word alignment	*/
} ibobj_bri, *Ibobj_bri;

/****************************************************************************
 * Connection structure used for OOB signalling 
 *
 * This structure specifies a service type ( MEPG, IP etc etc).
 * This allows the signalling software to pick out a matching profile. The 
 * application can then use the default profile conection parameters. It can 
 * also get the profile for that service and network and change the connection
 * parameters for that profile using the getstat/setstat calls ite_get_profile
 * and ite_set_profile
 *
 * The profile for ATM is specified in atm_pr.h and is called atm_profile.
 * The profile for ISDN is specified in isdn_pr.h. Each profile structure
 * has a first byte that identfies the structure type 
 */

/* Max size of the profile structure */

#define MAX_PR_SIZE 256   		

/* Types of profile structure */
#define PR_STRUCT_ATM  	1		/* found in atm_pr.h	*/
#define PR_STRUCT_BRI 	2		/* found in isdn_pr.h	*/

typedef struct conn_type {

    u_char  	conn_svc_type;		    /* Connection Service Type 	    */
    
        #define ITE_SVC_VOICE	    1	    /* Voice Service 		*/
        #define ITE_SVC_DATA_ANY    2 	    /* Any data service 	*/
        #define ITE_SVC_DATA_MPEG   3	    /* MPEG data		    */
        #define ITE_SVC_DATA_IP     4	    /* IP data			    */
    
    u_char		conn_reserved;			    /* reserved 		    	*/
    u_char  	conn_rate_qualifier;	    /* rate qualifier 		    */

        #define ITE_RATE_DEFAULT    0       /* Use default data rate        */
        #define ITE_RATE_PROFILE    1       /* Use Rate from the Profile    */
        #define ITE_RATE_KBPS       2       /* Kilobits per second          */
        #define ITE_RATE_MBPS       3       /* Megabits per second          */
    
    u_int32 	conn_fwd_rate;              /* Average Forward Data rate    */
    u_int32 	conn_bwd_rate;              /* Average Backward Data rate   */

    addr_type 	our_subaddress;    	    	/* if not needed set to 0	    */
    addr_type 	their_subaddress;  	    	/* if not needed set to 0 	    */

} conn_type, *Conn_type;

/****************************************************************************/
/* structure returning status of the connection */

typedef struct conn_return { 

    u_int32 rtn_status;                 /* ITEM defined returned status */
    
        #define CONN_SUCCESS        0
        #define CONN_FAILED         1

    u_int16 rtn_class;

        #define RTN_Q2931           SPF_PR_Q2931
        #define RTN_X25             SPF_PR_X25
        #define RTN_Q931            SPF_PR_Q931
        #define RTN_DSMCC_UN        SPF_PR_SCDP

    u_int16 rtn_size;
    u_char  rtn_array[16];

} conn_return, *Conn_return;

/****************************************************************************/
/* structure passed in with the notify pb in the ntfy_ctl field during the 
 * ite_ctl_connect call
 */

typedef struct conn_cfg  {

        conn_type   connect_params;     /* Call configuration           */
        conn_return connect_return;     /* Call return status           */

} conn_cfg, *Conn_cfg;

/****************************************************************************/
/**** Function Prototypes ****/

extern error_code ite_ibpath_open(Ib_cfg_pb, path_id *ib_path);

/* get the default profile values for a particular profile */
extern error_code ite_path_profileget(path_id path, conn_type *conn,
										u_int32 *pr_size, void *pr_buffer);

/* set the values for a particular profile for this path. Any connection
 * opened using this path will use these values for the profile specified.
 */
extern error_code ite_path_profileset(path_id path, conn_type *conn,
										u_int32 *pr_size, void *pr_buffer);

#endif  /* #ifndef  _SPF_OOB_H */
