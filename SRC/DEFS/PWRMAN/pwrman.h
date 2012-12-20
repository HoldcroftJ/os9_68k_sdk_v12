/*****************************************************************************
** Power Manager definitions                                                **
******************************************************************************
** Copyright 1995 by Microware Systems Corporation                          **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**   1 10/02/95 Creation                                          bat       **
**   2 10/25/95 Design change                                     bat       **
**   3 12/22/95 Design Change                                     rmm       **
**   4 03/04/96 Ported to OS-9000                                 rmm       **
**   5 04/30/96 Changes for adding PwrExt module                  rmm       **
**   6 05/30/96 Updated for PwrMan v1.0.Beta structures           bat       **
**   7 12/23/96 system/user state and super/normal user flags     ecm       **
**   8 01/12/97 added link, unlink, copyglobs functions           ecm       **
**     01/17/97 <--------------- PwrMan v2.1 --------------->               **
**     05/28/98 <--------------- PwrMan v2.1.1 ------------->               **
*****************************************************************************/

#ifndef _PWRMAN_H
#define _PWRMAN_H


/*
** Header Files
*/

#include <types.h>
#include <sysglob.h>
#include <signal.h>
#include <errno.h>
#include <module.h>

#ifdef _OS9000
#include <svctbl.h>
#include <srvcb.h>
#endif


/*
** Macro Definitions
*/

/* PwrMan version string length */
#define PWR_VERS_LEN                            32

/* PwrMan parameter-block edition # */
#define PWR_PB_EDITION                          1

/* PwrMan device ID string length */
#define PWR_IDLEN                 		16


/* PwrMan Error-Codes */

	/* none defined yet */



/* PwrMan Sub-Codes */

#define PWRMAN_MIN                          0x00    /* minimum PwrMan subcode */
	#define PWRMAN_CHECK                    0x00    /* check for PwrMan iniz */
	#define PWRMAN_REGISTER                 0x01    /* register pwr-routine */
	#define PWRMAN_UNREGISTER               0x02    /* unregister pwr-routine */
	#define PWRMAN_STATEADD                 0x03    /* add power state */
	#define PWRMAN_STATEREMOVE              0x04    /* remove power state */
	#define PWRMAN_DEBUG                    0x05    /* For the pwrstat utility */
	#define PWRMAN_CHANGE                   0x06    /* change power states */
	#define PWRMAN_CALLBACK                 0x07    /* direct callback function */
	#define PWRMAN_LINK_PLCY                0x08    
	#define PWRMAN_UNLINK_PLCY              0x09    
	#define PWRMAN_COPYGLOB                 0x0a    /* get a copy of pwrman globals */
	#define PWRMAN_LINK_EXT                 0x0b  
	#define PWRMAN_UNLINK_EXT               0x0c  
#define PWRMAN_MAX                                      0x3F    /* maximum PwrMan subcode */

#define PWRPLCY_MIN                                     0x40    /* min PwrPlcy subcode  */
	#define PWRPLCY_MW_MIN                  0x40    /* min PwrPlcy Microware subcode */
		#define PWRPLCY_INIT            0x40    /* PwrPlcy initialization */
		#define PWRPLCY_DEINIT          0x41    /* PwrPlcy deinitialization */
		#define PWRPLCY_EV_NOTIFY       0x42    /* notification power-event */
		#define PWRPLCY_EV_REQUEST      0x43    /* request power-event */
	/* sample policy power states */
		#define PWRPLCY_STATE_OFF               0x6C    /* System OFF state */
		#define PWRPLCY_STATE_SUSPEND   0x6D    /* System SUSPEND state */
		#define PWRPLCY_STATE_IDLE              0x6E    /* System IDLE state */
		#define PWRPLCY_STATE_ON                0x6F    /* System ON state */
	#define PWRPLCY_MW_MAX                  0x6F    /* max PwrPlcy Microware subcode */
	#define PWRPLCY_USR_MIN                 0x70    /* min PwrPlcy user subcode     */
	#define PWRPLCY_USR_MAX                 0x9F    /* max PwrPlcy user subcode     */
#define PWRPLCY_MAX                                     0x9F    /* max PwrPlcy subcode  */

#define PWREXT_MIN                                      0xA0    /* min PwrExt subcode */
	#define PWREXT_MW_MIN                   0xA0    /* min PwrExt Microware subcode */
		#define PWREXT_INIT                     0xA0    /* PwrExt initialization */
		#define PWREXT_DEINIT           0xA1    /* PwrExt deinitialization */
	#define PWREXT_MW_MAX                   0xCF    /* max PwrExt Microware subcode */
	#define PWREXT_USR_MIN                  0xD0    /* min PwrExt user subcode */
	#define PWREXT_USR_MAX                  0xFF    /* max PwrExt user subcode */
#define PWREXT_MAX                                      0xFF    /* max PwrExt subcode */


/*
** PwrMan Global Definitions
*/

/* forward defined type definitions */
typedef struct pwr_state pwr_state, *Pwr_state;
typedef struct pwr_devreg pwr_devreg, *Pwr_devreg;
typedef struct pwrman_globals pwrman_globals, *Pwrman_globals;

/* power level (either system-wide or device-specific) */
typedef u_int32 pwr_level, *Pwr_level;

/* type of device (with respect to power consumption/generation) */
typedef enum pwr_devtype { consumer, supplier, neutral, other } pwr_devtype;

/* local power-mode structure for a power-aware device */
typedef struct pwr_localmode {
	pwr_level level;                                /* Local power mode level */
	int32 maxload;                                  /* max energy load or drain (mW) */
	int32 minload;                                  /* min energy load or drain (mW) */
	u_int32 entrytime;                              /* worst-case device entry time (ms) */
	u_int32 exittime;                               /* worst-case device exit time (ms) */
	char lpm_name[PWR_IDLEN];               /* local power-mode name */
	u_int8 context_pres;                    /* device context preserved flag */
	u_int8 rsv1[11];                                /* reserved */
} pwr_localmode, *Pwr_localmode;

/* energy condition structure for a power-aware device */
typedef struct pwr_devcond {
	Pwr_localmode lpm;                              /* ptr to array of local power modes */
	u_int32 num_lpm;                                /* # of local power modes */
	pwr_devtype dev_type;                   /* device type */
	pwr_level pres_level;                   /* present power level */
	int32 pres_energy;                              /* present energy level (mW) */
	int32 pres_drain;                               /* present energy drain (mW/hr) */
	u_int8 rsv1[24];                                /* reserved */
} pwr_devcond, *Pwr_devcond;

/* power-event structure */
typedef struct pwr_event {
	char *devid;                                    /* ptr to device identifier */
	Pwr_devcond devcond;                    /* ptr to device condition struct */
	pwr_level old_lpm;                              /* ptr to old local power mode */
	pwr_level new_lpm;                              /* ptr to new local power mode */
	int32 old_energy;                               /* old energy level (mW) */
	int32 new_energy;                               /* new energy level (mW) */
	int32 old_drain;                                /* old drain/[supply] (mW/hr) */
	int32 new_drain;                                /* new drain/[supply] (mW/hr) */
	u_int8 rsv1[16];                                /* reserved */
} pwr_event, *Pwr_event;
	

/*
** PwrMan System Call Parameter Blocks
*/

/* PwrMan System-Call Parameter-Block Types */
typedef struct f_pwrman_pb {
	#if defined(_OS9000)
		syscb cb;                                   /* OS-9000 common parameter block */
	#endif
	u_int8 edition;                                 /* current PwrMan version number */
	u_int8 subcode;                                 /* PwrMan sub-code */
	u_int8 rsv[2];                                  /* reserved */
} f_pwrman_pb, *F_pwrman_pb;

/* F_PWRMAN/PWRMAN_CHECK */
typedef struct f_pwrman_check_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	char *version;                                  /* version string buffer ptr */
} f_pwrman_check_pb, *F_pwrman_check_pb;

/* F_PWRMAN/PWRMAN_ADD */
typedef struct f_pwrman_add_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	pwr_level syslevel;                             /* state level */
	u_int32 priority;                               /* priority of entry */
	pwr_level devlevel;                             /* device level */
	void* devpb;                                    /* additional device param block */
	char* id;                                               /* entry id ptr */
} f_pwrman_add_pb, *F_pwrman_add_pb;

/* F_PWRMAN/PWRMAN_REMOVE */
typedef struct f_pwrman_remove_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	pwr_level syslevel;                             /* state level */
	u_int32 priority;                               /* priority of entry */
	pwr_level devlevel;                             /* device level */
	void* devpb;                                    /* additional device param block */
	char* id;                                               /* entry id ptr */
} f_pwrman_remove_pb, *F_pwrman_remove_pb;

/* F_PWRMAN/PWRMAN_COPYGLOB */
typedef struct f_pwrman_copyglob_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	pwrman_globals *pwrglob;                        /* ptr to copy of powerman globals */
} f_pwrman_copyglob_pb, *F_pwrman_copyglob_pb;

/* F_PWRMAN/PWRMAN_DEBUG */
typedef struct f_pwrman_debug_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	const pwrman_globals **pwrglob;                 /* pointer to powerman globals */
} f_pwrman_debug_pb, *F_pwrman_debug_pb;

/* F_PWRMAN/PWRMAN_CHANGE */
typedef struct f_pwrman_change_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	pwr_level syslevel;                             /* PwrMan's state level to check */
} f_pwrman_change_pb, *F_pwrman_change_pb;

/* F_PWRMAN/PWRMAN_CALLBACK */
typedef struct f_pwrman_callback_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	pwr_level devlevel;                             /* device level */
	void* devpb;                                    /* additional device param block */
	char* id;                                               /* entry id ptr */
} f_pwrman_callback_pb, *F_pwrman_callback_pb;

/* F_PWRMAN/PWRMAN_LINK_PLCY */
typedef struct f_pwrman_link_plcy_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	char *mname;                  /* name of pwrplcy module */
} f_pwrman_link_plcy_pb, *F_pwrman_link_plcy_pb;

/* F_PWRMAN/PWRMAN_UNLINK_PLCY */
typedef struct f_pwrman_unlink_plcy_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
} f_pwrman_unlink_plcy_pb, *F_pwrman_unlink_plcy_pb;

/* F_PWRMAN/PWRMAN_LINK_EXT */
typedef struct f_pwrman_link_ext_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	char *mname;                  /* name of pwrext module */
} f_pwrman_link_ext_pb, *F_pwrman_link_ext_pb;

/* F_PWRMAN/PWRMAN_UNLINK_EXT */
typedef struct f_pwrman_unlink_ext_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
} f_pwrman_unlink_ext_pb, *F_pwrman_unlink_ext_pb;

/* F_PWRMAN/PWRMAN_REG */
typedef struct f_pwrman_reg_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	error_code (*func)(void *funcparam, pwr_level pwrlvl, void *devpb);
									/* call-back function*/
	void* funcparam;                                /* call-back parameter */
	Pwr_devcond devpwrdef;                  /* device power definition */   
	char* id;                                               /* entry id ptr */
} f_pwrman_reg_pb, *F_pwrman_reg_pb;

/* F_PWRMAN/PWRMAN_UNREG */
typedef struct f_pwrman_unreg_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	error_code (*func)(void *funcparam, pwr_level pwrlvl, void *devpb);
									/* call-back function */
	void* funcparam;                                /* call-back parameter */
	Pwr_devcond devpwrdef;                  /* device power definition */   
	char* id;                                               /* entry id ptr */
} f_pwrman_unreg_pb, *F_pwrman_unreg_pb;


/* F_PWRMAN/PWRPLCY_INIT */
typedef struct f_pwrplcy_init_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
} f_pwrplcy_init_pb, *F_pwrplcy_init_pb;

/* F_PWRMAN/PWRPLCY_TERM */
typedef struct f_pwrplcy_term_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
} f_pwrplcy_term_pb, *F_pwrplcy_term_pb;

/* F_PWRMAN/PWRPLCY_EV_REQUEST */
typedef struct f_pwrplcy_ev_request_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	Pwr_event pwrevent;                             /* Ptr to power-event structure */
} f_pwrplcy_ev_request_pb, *F_pwrplcy_ev_request_pb;

/* F_PWRMAN/PWRPLCY_EV_NOTIFY */
typedef struct f_pwrplcy_ev_notify_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
	Pwr_event pwrevent;                             /* Ptr to pwrevent structure */
} f_pwrplcy_ev_notify_pb, *F_pwrplcy_ev_notify_pb;


/* F_PWRMAN/PWREXT_INIT */
typedef struct f_pwrext_init_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
} f_pwrext_init_pb, *F_pwrext_init_pb;

/* F_PWRMAN/PWREXT_TERM */
typedef struct f_pwrext_term_pb {
	f_pwrman_pb pwrcom;                             /* PwrMan common parameter block */
} f_pwrext_term_pb, *F_pwrext_term_pb;


/*
** PwrMan Internals Definitions (subject to change)
*/

/* structure for the power state table */
struct pwr_state {

	Pwr_state nextstate;                    /* next system power state ptr */
	Pwr_state nextentry;                    /* next entry for this power state ptr */

	u_int32 priority;                               /* entry priority level */
	pwr_level syslevel;                             /* system power level */
	pwr_level devlevel;                             /* device power level for this state */
	void *devpb;                                    /* device parameter block for this state */

	char id[PWR_IDLEN];                             /* string id */
	error_code (*func)(void *funcparam, pwr_level devlevel, void *devpb);
									/* device power call-back function */
	void *funcparam;                                /* device call-back function parameter */

	u_int8 rsv1[16];                                /* reserved */
};

/* structure for the device registry table */
struct pwr_devreg {

	Pwr_devreg nextentry;                   /* next device entry ptr */

	char id[PWR_IDLEN];                             /* string id */
	error_code (*func)(void *funcparam, pwr_level devlevel, void *devpb);
									/* device call-back function */
	void *funcparam;                                /* device call-back function parameter */

	Pwr_devcond devpwrdef;                  /* device's energy condition ptr */

	u_int8 rsv1[12];                                /* reserved */
};

/* Global Variables to PwrMan, pwrplcy, and pwrext
*/
struct pwrman_globals {

	Sysglobs sglobptr;                              /* system globals pointer */

	Pwr_state phead;                                /* head of the Power State table */
	Pwr_state pfree;                                /* power state table free pool */

	Pwr_devreg dthead;                              /* head of the device registry table */
	Pwr_devreg dtfree;                              /* device registry table free pool */

	pwr_level currlevel;                    /* current system power level */

	void* pwrplcy_mem;                              /* PwrPlcy-specific memory pointer */
	error_code (*pwrplcy_func)(F_pwrman_pb pb, Pwrman_globals ldptr);
									/* PwrPlcy entry point function */
	void* pwrext_mem;                               /* PwrExt-specific memory pointer */
	error_code (*pwrext_func)(F_pwrman_pb pb, Pwrman_globals ldptr);
									/* PwrExt entry point function */
	u_int8 from_usrstate;                           /* usr state flag */
		#define FROM_SYSSTATE 0x00
		#define FROM_USRSTATE 0x01
	
	u_int8 from_super;                              /*super user flag */
		#define FROM_NONSUPER 0x00
		#define FROM_SUPER 0x01

	u_int8 ret_on_err;                              /* return on state-change err flag */
	u_int8 rsv1[1];                                 /* reserved */

	error_code (*pwrman_func)(F_pwrman_pb pb, Pwrman_globals ldptr);
									/* PwrMan entry point function */

	Mh_com pwrplcy_modhead;           /* pointer to PwrPlcy module head */
	Mh_com pwrext_modhead;            /* pointer to PwrExt module head */

	u_int8 rsv2[12];                                /* reserved */
};


/*
** Function Prototypes
*/

#if defined(__STDC__) || defined(_ANSI_EXT) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

extern error_code _os_pwr_add(char id[PWR_IDLEN], pwr_level syslevel, u_int32 priority, pwr_level devlevel, void* devpb);
extern error_code _os_pwr_callback(char id[PWR_IDLEN], pwr_level devlevel, void *devpb);
extern error_code _os_pwr_change(pwr_level syslevel);
extern error_code _os_pwr_check(char version[PWR_VERS_LEN]);
extern error_code _os_pwr_copyglob(pwrman_globals *pwrglob);
extern error_code _os_pwr_debug(const pwrman_globals **pwrglob);
extern error_code _os_pwr_ev_notify(Pwr_event pwrevent);
extern error_code _os_pwr_ev_request(Pwr_event pwrevent);
extern error_code _os_pwr_reg(char id[PWR_IDLEN],error_code (*func)(void *funcparam, pwr_level devlevel, void *devpb), void* funcparam, Pwr_devcond devpwrdef);
extern error_code _os_pwr_remove(char id[PWR_IDLEN], pwr_level syslevel, u_int32 priority, pwr_level devlevel ,void* devpb);
extern error_code _os_pwr_unreg(char id[PWR_IDLEN],error_code (*func)(void *funcparam, pwr_level devlevel, void *devpb), void* funcparam, Pwr_devcond devpwrdef);
extern error_code _os_pwr_link_plcy(char id[PWR_IDLEN]);
extern error_code _os_pwr_unlink_plcy(void);
extern error_code _os_pwr_link_ext(char id[PWR_IDLEN]);
extern error_code _os_pwr_unlink_ext(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif



#endif /* _PWRMAN_H */
