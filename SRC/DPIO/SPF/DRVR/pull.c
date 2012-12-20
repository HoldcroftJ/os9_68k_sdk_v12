/****************************************************************************
 *  ID:     %A%
 *  Date:   %G%
 ****************************************************************************
 *  Copyright 1995 by Microware Systems Corporation                         *
 *  Reproduced Under License                                                *
 *                                                                          *
 *  This source code is the proprietary confidential property of            *
 *  Microware Systems Corporation, and is provided to licensee              *
 *  solely for documentation and educational purposes. Reproduction,        *
 *  publication, or distribution in any form to any party other than        *
 *  the licensee is strictly prohibited.                                    *
 ****************************************************************************
 * Edition History:                                                         *
 * #   Date     Comments                                          By        *
 * --- -------- ------------------------------------------------- --------- *
 * 1   1/19/96  created						   udayn   
 ****************************************************************************/


#include <defs.h>

#ifndef DEBUG_DATA
#ifdef DEBUG
#define DEBUG_DATA(a,b,c) algn_dbg_data(a,(u_char*)b,10,c)
#else
#define DEBUG_DATA(a,b,c) 
#endif
#endif

/*
** pull() - routine used to pull a driver from below you. 
**
**	desc_name is the name of the descriptor that we are trying to pull
**
**	pulled_deventry is a pointer to the pointer to the device entry of the 
**	pulled  driver. 
**
**	pulled_dpio_deventry is a pointer to the pointer to the dpio device entry
**	of the pulled driver. For OS9000 this is the same as the device entry.
**
**	If *pulled_deventry/pulled_dpio_deventry  is NULL then we attach to the device
**	and set *pulled_deventry/pulled_dpio_deventry else it is assumed that the 
**	attach has already been performed and *pulled_deventry/*pulled_dpio_deventry 
**	are set correctly.
**
**	In the case of drivers like SPLLCSNAP or SPENET where the same driver is 
** 	pulled over and over again, the driver should first call pull with 
**	pulled_deventry/pulled_dpio_deventry  pointing to a NULL deventry 
**	and then store away the returned pointers. In subsequent calls
**	to pull the stored value of pulled_deventry/pulled_dpio_deventry should be passed in.
**	In these drivers the _os_detach must be performed only once in dr_term().
**
**	In the case of drivers like SPSIP where different drivers could be pulled 
**	for different paths, for every path opened the pull function should be called
**	with pulled_deventry/pulled_dpio_deventry pointing to NULL. The device entry
**	should then be detached in SS_CLOSE
**
**
** 
*/ 

error_code pull (Dev_list	deventry, 
		 char 		*desc_name, 
		 Dev_list	*pulled_deventry,
		 Dev_list	*pulled_dpio_deventry
		 ) 

{

    Spf_lustat 		lustat = (Spf_lustat)(deventry->v_lu_stat);
    Spf_lustat 		pulled_lustat;
    error_code 		err;
    path_id 		temp_path;
    spf_ss_pb 		npb;
    spf_ss_open_pb 	opb;
    
    DEBUG_DATA(lustat->lu_dbg,"pull_drvr    ",(u_int32)desc_name);


    /* attach to device name */
    if (*pulled_deventry == NULL ) {

	if ((err = _os_attach(desc_name, 0, pulled_deventry)) != SUCCESS) 
	    return(err);

#ifndef _OS9000
	
	/* In non OS9000 systems we need to get the dpio device entry */
				
	/* open path to device name */
	if ((err = _os_open(desc_name,0,&temp_path)) != SUCCESS) {
	    _os_detach(*pulled_deventry);
	    *pulled_dpio_deventry = *pulled_deventry = NULL;
	    return(err);
	}
	
	/* get the DPIO device entry for this driver */
	npb.code  = SPF_GS_DEVENTRY;
	npb.param = pulled_dpio_deventry;

	if ((err = _os_getstat(temp_path, SS_SPF, &npb)) != SUCCESS) {
	    (void)_os_close(temp_path);
	    _os_detach(*pulled_deventry);
	    *pulled_dpio_deventry = *pulled_deventry = NULL;
	    return(err);
	}

	/* close path to device name */
	(void)_os_close(temp_path);

#else	/* on OS9000 systems */
	   
	 *pulled_dpio_deventry  = *pulled_deventry;

#endif
	
    }

    /* set the down driver */
    lustat->lu_dndrvr = *pulled_dpio_deventry;

    /* find lustat for device driver */
    pulled_lustat = (Spf_lustat)(lustat->lu_dndrvr->v_lu_stat);

    /* save _os_attach() device entry pointer of new 
     * protocol for close/pop.
     */
    pulled_lustat->lu_attachptr = *pulled_deventry;

    /* Now we manipulate the updriver in the pulled driver's lustat
     * to point to our device entry.
     */
    pulled_lustat->lu_updrvr = deventry;
    
    /* now we call open to give the pulled driver a chance to 
     * initialize itself 
     */
    opb.code = SPF_SS_OPEN;
    opb.param = deventry;
    opb.init = NULL;

    if ((err = SMCALL_SS(deventry, lustat->lu_dndrvr, (void*)&opb)) != SUCCESS) {
	_os_detach(*pulled_deventry);
	*pulled_dpio_deventry = *pulled_deventry = NULL;
	return(err);	
    }
    
    /* now we call push  to give the pulled driver a chance to 
     * initialize its push routine  
     */
    npb.code = SPF_SS_PUSH;
    npb.param = deventry;

    if ((err = SMCALL_SS(deventry, lustat->lu_dndrvr, (void*)&npb)) != SUCCESS) {

	npb.code = SPF_SS_CLOSE;
	npb.param = deventry;
	SMCALL_SS(deventry, lustat->lu_dndrvr, (void*)&npb);
	_os_detach(*pulled_deventry);
	*pulled_dpio_deventry = *pulled_deventry = NULL;
	return(err);	

    }

    return SUCCESS;

}    
    

/*
** drop() - routine used to drop a pulled driver from below you.
** 
*/ 

error_code drop (Dev_list deventry, Dev_list dndrvr) {

    Spf_lustat 		lustat = (Spf_lustat)(deventry->v_lu_stat);
    spf_ss_pb 		npb;
    error_code		err;

    DEBUG_DATA(lustat->lu_dbg,"drop    ",0);

    
    /* call the pulled driver's pop setstat  */
    npb.code = SPF_SS_POP;
    npb.param = NULL;
    if ((err = SMCALL_SS(deventry, dndrvr, (void*)&npb)) != SUCCESS) {
	DEBUG_DATA(lustat->lu_dbg,"Pop Fail  ",(u_int32)dndrvr);
	return(err);		
    }

    /* call the pulled driver's close setstat  */
    npb.code = SPF_SS_CLOSE;
    npb.param = deventry;
    if ((err = SMCALL_SS(deventry, dndrvr, (void*)&npb)) != SUCCESS) {
	DEBUG_DATA(lustat->lu_dbg,"CloseErr  ", (u_int32)dndrvr);
	return(err);		
    }

    return SUCCESS;

}

