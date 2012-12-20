/*******************************************************************************
* DESCRIPTION :
*
*	This file conatins functions (glue code) that are used to link the driver
*	library code directly with your application.
*
*	THIS IS ONLY USED FOR DEBUGGING. DO NOT USE FOR THE FINAL PRODUCTION
*	VERSION OF YOUR DRIVER(S) OR APPLICATION(S).
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  11/30/95  TJE  Converted from earlier work by BAT and CJV
*/
#include <gdv_priv.h>
#include <stdio.h>
#include <srvcb.h>
#include <process.h>
#include <modes.h>

dev_list _dbg_dev_entry;			/* Fake device entry */
mfm_path_desc _dbg_path_desc;		/* Fake path descriptor */
mh_drvr _dbg_drvmod;				/* Fake module header for driver code */
mh_dev _dbg_devmod;					/* Fake module header for descriptor code */

int getpid(void);
error_code _os_setstat(path_id path, u_int32 code, void *pb);

/*******************************************************************************
* DEFINE AND INITIALIZE THE LOGICAL UNIT STATIC
*******************************************************************************/

extern mfm_lu_stat my_lu;
mfm_lu_stat dbg_lu =   {
  /* General Logical Unit Information Fields */
  SHARE,  /* Path sharing flag */
  { 0 },  /* RESERVED FOR FUTURE USE! */
  0,      /* Number of attaches using logical unit */
  0,      /* Path use count */
  LUN,    /* lu_num: logical unit number */
  
  (void*)PORTADDR, /* lu_port: port address	*/
  
  { 0 }, /* lu_rsv2[2]: RESERVED FOR FUTURE USE! */

  { 0 }, /* lu_rsv3[40]: RESERVED FOR FUTURE USE! */
  
  /* lu_luopts: logical unit options section */
  {
	sizeof(mfm_lu_opt),			/* lu_optsize: logical unit options size*/
	DT_MFM,						/* lu_class: device type				*/
	0,							/* lu_errs: accumulated I/O errors		*/
	{ 0 },						/* lu_rsv2[27]: RESERVED FOR FUTURE USE!*/
  },
  
#ifdef LU_SPECIFICS_INIT
  LU_SPECIFICS_INIT			/* logical unit specific static storage */
#endif
	
};

/*******************************************************************************
* NAME:		_dbg_dr_iniz
*
* USAGE:	Initialize the graphics device.
*/
error_code _dbg_dr_iniz(void)
{
	event_id evid;
	static i_setstat_pb ss_pb;

	/* Copy the initialized dbg_lu to the un-initialized my_lu */
	my_lu = dbg_lu;

	/* Permit the graphics memory so the driver code can access it while */
	/* it is in user state. */
	#if defined(MEM_BASE_ADDRESS) && defined(MEM_SIZE)
		_os_permit((void*)MEM_BASE_ADDRESS, MEM_SIZE, S_IREAD | S_IWRITE,
			getpid());
	#endif

	/* Permit the PCI space so the driver code can access it while it is in */
	/* user state. */
	#if defined(PCI_BASE_ADDRESS) && defined(PCI_SIZE)
		_os_permit((void*)PCI_BASE_ADDRESS, PCI_SIZE, S_IREAD | S_IWRITE,
			getpid());
	#endif

	/* Setup fake module pointers so the permit and protect of the driver */
	/* and descriptor code works correctly. */
	_dbg_dev_entry.v_dr_mod = &_dbg_drvmod;
	_dbg_dev_entry.v_dd_mod = &_dbg_devmod;
	#ifdef _OSK
		_dbg_drvmod._mh._msize = 1;
		_dbg_devmod._mh._msize = 1;
	#else
		_dbg_drvmod.m_size = 1;
		_dbg_devmod.m_size = 1;
	#endif
	_dbg_dev_entry.v_cproc = getpid();
	_dbg_path_desc.pd_common.pd_cproc = getpid();

	/* Setup device entry and logical unit storage */
	_dbg_dev_entry.v_lu_stat = &my_lu;
	_dbg_dev_entry.v_lu_size = sizeof(my_lu);
	my_lu.irq_evname = NULL;
	printf("GDV_DBG: Warning: IRQs are not supported in the debug version.\n");
	fflush(stdout);

	/* Call the real entry point */
	dr_iniz(&_dbg_dev_entry);

	/* Do the SS_OPEN setstat */
	ss_pb.path = 0;
	ss_pb.ss_code = SS_OPEN;
	return _os_setstat(0, SS_OPEN, &ss_pb);
}

/*******************************************************************************
* NAME:		_dbg_dr_term
*
* USAGE:	Terminate use of the graphics device.
*/
error_code _dbg_dr_term(void)
{
	error_code ec, result = SUCCESS;
	static i_setstat_pb ss_pb;

	/* Do the SS_CLOSE setstat */
	ss_pb.path = 0;
	ss_pb.ss_code = SS_CLOSE;
	return _os_setstat(0, SS_CLOSE, &ss_pb);

	/* Call the real entry point and return */
	if ((ec = dr_term(&_dbg_dev_entry)) != SUCCESS) {
		result = ec;
	}

	/* Protect the graphics memory */
	#if defined(MEM_BASE_ADDRESS) && defined(MEM_SIZE)
		_os_protect((void*)MEM_BASE_ADDRESS, MEM_SIZE,S_IREAD | S_IEXEC,
			getpid());
	#endif

	/* Protect the PCI space */
	#if defined(PCI_BASE_ADDRESS) && defined(PCI_SIZE)
		_os_protect((void*)PCI_BASE_ADDRESS, PCI_SIZE, S_IREAD | S_IEXEC,
			getpid());
	#endif

	return result;
}

/*******************************************************************************
* NAME:		_os_getstat()
*
* USAGE:	Get status entry point.
*/
error_code _os_getstat(path_id path, u_int32 code, void *pb)
{
	gs_mod_pb gs_pb;

	/* Call the real entry point and return */
	gs_pb.mod_code = code;
	gs_pb.mod_size = 0;
	gs_pb.mod_param = pb;
	return dr_getstat(&_dbg_dev_entry, &gs_pb);
}

/*******************************************************************************
* NAME:		_os_setstat()
*
* USAGE:	Set status entry point.
*/
error_code _os_setstat(path_id path, u_int32 code, void *pb)
{
	ss_mod_pb ss_pb;

	/* Call the real entry point and return */
	ss_pb.mod_code = code;
	ss_pb.mod_size = 0;
	ss_pb.mod_param = pb;
	ss_pb.mod_pd = &_dbg_path_desc;
	return dr_setstat(&_dbg_dev_entry, &ss_pb);
}

/*******************************************************************************
* Dummy functions. These functions are here so that the application does not
* have to link to the system libraries that include them. They are never
* called.
*******************************************************************************/

error_code _os_irq(u_int32 vector, u_int32 priority, void *irq_entry, void
	*statics)
{
	return SUCCESS;
}

#ifdef change_static
#undef change_static
#endif
void *change_static(void *dest_stat)
{
	return NULL;
}

void irq_enable(void)
{
	static BOOLEAN first_time = TRUE;

	if (first_time == TRUE) {
		first_time = FALSE;
		printf("irq_enable() not supported in the debug version\n");
		fflush(stdout);
	}
}

void irq_disable(void)
{
	static BOOLEAN first_time = TRUE;

	if (first_time == TRUE) {
		first_time = FALSE;
		printf("irq_enable() not supported in the debug version\n");
		fflush(stdout);
	}
}
