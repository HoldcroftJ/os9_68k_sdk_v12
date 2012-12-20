/*
 * $Header:   /h0/MWOS/SRC/ROM/CNFGFUNC/VCS/cnfgfunc.c_v   1.29   09 Jul 1998 11:44:16   alexs  $
 */
 
/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1995-1998 by Microware Systems Corporation         |
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
| cnfgfunc.c - rom configuration structure access functions                 |
|																			|
| edition history															|
|																			|
| ed#   date    changes                                        		   by	|
| --- -------- ------------------------------------------------------- ---	|
|  1  95/07/28 created                                        		   Nib	|
|  2  95/08/01 Added RELATIVE build option.                            gs   |
|  3  95/08/15 First design feedback update.                           ats  |
|  4  95/08/21 Design walkthrough update.                              ats  |
|  5  95/08/24 Add missing structure ids.                              ats  |
|  6  95/09/07 Make functions and constant data visible.               ats  |
|              Correct array address computations.                          |
|  7  95/09/14 If-then-elses replace switches to workaround compiler   ats  |
|              problem with switches of enums.                              |
|              Added processing for use_alt field of if struct.             |
|  8  95/09/21 Renamed LLPM_COUNT to LLPM_IFCOUNT.                     ats  |
|  9  95/09/25 Add necessary size parameter to get_uint8p().           ats  |
| 10  95/10/06 Begin transition to error codes in errno.h.             ats  |
| 11  95/10/06 Added processing for LLPM_IF_SUBNET_MASK,               ats  |
|              LLPM_IF_BRDCST_ADDRESS, and LLPM_IF_GW_ADDRESS.              |
|              --- OS-9/68K V3.0.2 System-state Debug Support Release ---   |
| 12  95/12/28 Fixed parameters to get_cptr() call for LLPM_IF_NAME    rak  |
| 13  96/01/05 Added processing for BOOTER_PARAMS & BOOTER_AUTODELAY,  ats  |
|              passed sizes for llif and booter records (V2),               |
|              and expanded addresses in llif records (V2).                 |
|              *** WARNING *** Editions prior to this one will not          |
|              recognize a V2 record properly.                              |
|              ---- OS9000/PPC V2.0 ----                                    |
|              ---- OS-9000/x86 V2.1 Released ----                          |
| 14 96/10/01 Made Edition 12 fix apply also for RELATIVE code         rak  |
| 15 96/10/14 Linked with new p2lib to fix getrinf().                  gdb  |
|              ---- OS-9000/PPC V2.1.1 Released ----                        |
|              ---- OS-9/68K V3.0.3 System-state Debug Support Release ---  |
|              ---- OS-9000/ARMv3 V2.2 Released ----                        |
|              ---- OS-9000/ARMv3 V2.2.1 Released ----                      |
|              ---- Modular Rom Sub-system v1.0 Released ----               |
| 16 98/01/28 Eliminate compiler warnings.                                  |
|              ---- MBX_BSP Released ----                                   |
|              ---- OS-9000/SH3 V2.2.6 Released ----                        |
|              ---- OS-9000/SPARC V2.2.7 Released ----                      |
|              ---- Modular Rom v1.1 Sub-component Released ----            |
|              ---- Modular ROM v1.2 Sub-component Released ----            |
|              $$             <RELEASE_INFO>                            $$  |
`--------------------------------------------------------------------------*/

#define		EDITION		16

_asm("_sysedit:	equ	%0", __obj_constant(EDITION));

#define	CONFIG_P2FUNCTION

#define		NEWINFO
#include	<const.h>
#include	<types.h>
#include	<string.h>
#include	<errno.h>
#include	<romerrno.h>

#include	<rom.h>
#include	<cnfgdata.h>
#include	<module.h>

#if	defined(CONFIG_P2FUNCTION)
#include	<p2lib.h>
#endif

/* Resolve the module field naming differences between OS-9 and OS-9000 */

#ifdef	_OSK
#ifndef MODNAME
#define MODNAME(mod)	((u_char*)((u_char*)mod+((Mh_com)mod)->_mname))
#endif
#define m_exec			_mexec
#endif

/* Default Configuration data module name */

#ifndef	CNFGDATA_MODNAME
#define	CNFGDATA_MODNAME	"cnfgdata"
#endif


/* Make the configuration services record global so space is saved
   in not calling the memory allocate routines. */

cnfg_svcs config_r;

     
/*
 *  int cmpid( const char *s1, const char *s2)
 *  This routine compare ID in s1 and s2.
 *  If all 4 letters was matched, this routine return 0.
 */
 
static int cmpid( const u_int8 *s1, const u_int8 *s2)
{
	int	i;

	for( i = 4 ; i > 0 && *s1 == *s2 ; s1++, s2++, i-- ) ;
	return	i;
}


/*
 * These small functions will be inlined.
 */

static	void	get_cptr(char	**d, char **s, char *top)
	{ *d = (char *)((u_int32) *s + (u_int32)top); }
static	void	get_uint8p(u_int8 *d, u_int8 *s, u_int32 size)
	{ u_int32 i; for (i=0; i<size; i++,d++,s++) *d = *s; }
static	void	get_uint8(u_int8 *d, const u_int8 *s) { *d = *s; }
static	void	get_uint16(u_int16 *d, const u_int16 *s) { *d = *s; }
static	void	get_uint32(u_int32 *d, const u_int32 *s) { *d = *s;}


/*
 * CNFG_entry_header search_entry(Mh_exec cnfgmod, u_char *type)
 * 
 * This routine returns an pointer to a configuration data 
 * header.
 * if no valid header was found in configuration module,
 * this routine returns NULL.
 */
  
CNFG_entry_header search_entry(Mh_exec cnfgmod, u_char *type)
{
	u_int8	**p;
	u_int8	*entry;
	
	p = (u_int8 **)((u_int32)cnfgmod + cnfgmod->m_exec);

	/* check 1st entry pointer */
	if( *p == NULL ) return NULL;	/* no entry exists in the module. */
	do
	{

/* get entry point */
#ifdef 	RELATIVE	/* compiler generates self-relative offset */
		entry = (u_int8 *)((u_int32)p + (u_int32) *p + 1);		
#else				/* compiler generates absolute offset */
		entry = (u_int8 *)((u_int32)cnfgmod + (u_int32) *p);
#endif /* RELATIVE */

		/* check CNFG_ID and Entry type. */
 		if( cmpid( entry, (u_int8 *)CNFG_S_ID) == 0 && cmpid( &(entry)[4], (u_int8 *)type) == 0 )
			return((CNFG_entry_header)entry);
	}while( *++p != NULL );	/* loop until the end of entry */

	return(NULL);
}


/*
 * Offset table for CONS/COMM Configuration data entry.
 */
 
const u_int32	cons_offset_table[] =
{
	OFFSET(cnfg_console *, ch.cnfg_rev),
	OFFSET(cnfg_console *, cons_name),
	OFFSET(cnfg_console *, cons_vector),
	OFFSET(cnfg_console *, cons_priority),
	OFFSET(cnfg_console *, cons_level),
	OFFSET(cnfg_console *, cons_timeout),
	OFFSET(cnfg_console *, cons_parity),
	OFFSET(cnfg_console *, cons_baudrate),
	OFFSET(cnfg_console *, cons_wordsize),
	OFFSET(cnfg_console *, cons_stopbits),
 	OFFSET(cnfg_console *, cons_flow),
};


/*
 * error_code get_cons_config(enum config_element_id id, Mh_exec cnfgmod,
 *								 void *buf)
 * 
 */
 
error_code	get_cons_config(enum config_element_id id,
	Mh_exec cnfgmod, void *buf)
{

	CNFG_comm	p;			/* pointer to a configuration entry */
	u_int32		offset;		/* offset value */

	if( id >= COMM_START )	/* Is this request for COMM Parameter */
	{
		/* get Communication port data entry point */
		p = (CNFG_comm)search_entry(cnfgmod, (u_int8 *)CNFG_TYPE_S_COMM);
		offset = cons_offset_table[id - COMM_START];
	}
	else
	{
		/* get Communication console data entry point */
		p = (CNFG_comm)search_entry(cnfgmod, (u_int8 *)CNFG_TYPE_S_CONS);
		offset = cons_offset_table[id - CONS_START];
	}

	/* if no entry was found, return error */
	if( p == NULL ) return	EOS_UNKCNFG;

	
/*	switch(id)
	{ */

	/* Place (char *) fields here */
/*	case	CONS_NAME:	case	COMM_NAME: */
	if ((id == CONS_NAME) || (id == COMM_NAME)) {
		get_cptr( (char **)buf, (char **) (offset + (u_int32)p),
#ifdef 	RELATIVE	/* compiler generates self-relative offset */
			(char *)(offset + (u_int32) p + 1));
#else				/* compiler generates absolute offset */
			(char *)cnfgmod);
#endif /* RELATIVE */
/*		break; */
	
	/* Place (u_int8) fields here */
/*	case	CONS_PARITY:	case	COMM_PARITY:
	case	CONS_BAUDRATE:	case	COMM_BAUDRATE:
	case	CONS_WORDSIZE:	case	COMM_WORDSIZE:
	case	CONS_STOPBITS:	case	COMM_STOPBITS:
	case	CONS_FLOW:		case	COMM_FLOW: */
	} else if ((id == CONS_PARITY) || (id == COMM_PARITY) ||
	           (id == CONS_BAUDRATE) || (id == COMM_BAUDRATE) ||
	           (id == CONS_WORDSIZE) || (id == COMM_WORDSIZE) ||
	           (id == CONS_STOPBITS) || (id == COMM_STOPBITS) ||
	           (id == CONS_FLOW) || (id == COMM_FLOW))
	{
		get_uint8( (u_int8 *)buf, (u_int8 *) (offset + (u_int32)p));		
/*		break; */

	/* Place (u_int16) fields here */
/*	case	CONS_REVS:		case	COMM_REVS: */
	} else if ((id == CONS_REVS) || (id == COMM_REVS)) {
		get_uint16( (u_int16 *)buf, (u_int16 *) (offset + (u_int32)p));	
/*		break; */

	/* Place (u_int32) fields here */
/*	case	CONS_VECTOR:	case	COMM_VECTOR:
	case	CONS_PRIORITY:	case	COMM_PRIORITY:
	case	CONS_LEVEL:		case	COMM_LEVEL:
	case	CONS_TIMEOUT:	case	COMM_TIMEOUT: */
	} else if ((id == CONS_VECTOR) || (id == COMM_VECTOR) ||
			   (id == CONS_PRIORITY) || (id == COMM_PRIORITY) ||
			   (id == CONS_LEVEL) || (id == COMM_LEVEL) ||
			   (id == CONS_TIMEOUT) || (id == COMM_TIMEOUT))
	{
		get_uint32( (u_int32 *)buf, (u_int32 *) (offset + (u_int32)p));
/*		break; */
		
/*	default: */
	} else {
		return	EOS_UNKCNFG;
	}
	return	SUCCESS;
}


/*
 * Offset table for DEBUG Configuration data entry.
 */


const u_int32	debug_offset_table[] =
{
	OFFSET(cnfg_debug *, ch.cnfg_rev),	
	OFFSET(cnfg_debug *, debug_default_client),
	OFFSET(cnfg_debug *, debug_call_at_cold),
};


/*
 * error_code	get_debug_config(enum config_element_id id, Mh_exec cnfgmod,
 *									void *buf)
 *
 * This function returns an entry of debug configuration data.
 * If some error is occured in this function, it always returns E_UNKCNFG.
 */
 
error_code	get_debug_config(enum config_element_id id, 
	Mh_exec cnfgmod, void *buf)
{
	CNFG_debug	p;			/* pointer to a debug configuration data */
	u_int32		offset;		/* offset valuse	*/
		
	/* get pointer to debug configuration data entry */
	p = (CNFG_debug)search_entry(cnfgmod, (u_int8 *)CNFG_TYPE_S_DEBG);
	/* if no entry was found in configuration module.
	   return error. */
	if( p == NULL ) return	EOS_UNKCNFG;

	/* get an offset valuse for requested entry */
	offset = debug_offset_table[id - DEBUG_START];
	
/*	switch(id)
	{ */

	/* Place (char *) field at here */
/*	case	DEBUG_NAME: */
	if (id == DEBUG_NAME)
	{
		get_cptr( (char **)buf, (char **) (offset + (u_int32)p),
#ifdef 	RELATIVE	/* compiler generates self-relative offset */
			(char *)(offset + (u_int32) p + 1));
#else				/* compiler generates absolute offset */
			(char *)cnfgmod);
#endif /* RELATIVE */
/*		break; */

	/* Place (u_int32) field at here */
/*	case	DEBUG_COLD_FLAG: */
	} else if (id == DEBUG_COLD_FLAG) {
		get_uint32( (u_int32 *)buf, (u_int32 *) (offset + (u_int32)p));
/*		break; */

	/* Place (u_int16) field at here */
/*	case	DEBUG_REVS: */
	} else if (id == DEBUG_REVS) {
		get_uint16( (u_int16 *)buf, (u_int16 *) (offset + (u_int32)p));
/*		break; */

/*	default: */
	} else {
		return	EOS_UNKCNFG;
	}
	return	SUCCESS;	
}

/*
 * The offset tables of LLPM data structures.
 *
 */
   
const u_int32	llpm_offset_table[] =
{
	OFFSET(cnfg_protoman *, ch.cnfg_rev),	
	OFFSET(cnfg_protoman *, maxllpmprotos),
	OFFSET(cnfg_protoman *, maxrcvmbufs),
	OFFSET(cnfg_protoman *, maxllpmconns),
	OFFSET(cnfg_protoman *, llpm_count),
};

const u_int32	llif_offset_table[] =
{
	OFFSET(llpm_interface_data *, ip_address),
	OFFSET(llpm_interface_data *, subnet_mask),
	OFFSET(llpm_interface_data *, brdcst_address),
	OFFSET(llpm_interface_data *, gw_address),
	OFFSET(llpm_interface_data *, mac_address),
	OFFSET(llpm_interface_data *, hwtype),
	OFFSET(llpm_interface_data *, alt_parity),
	OFFSET(llpm_interface_data *, alt_baudrate),
	OFFSET(llpm_interface_data *, alt_wordsize),
	OFFSET(llpm_interface_data *, alt_stopbits),
	OFFSET(llpm_interface_data *, alt_flow),
	OFFSET(llpm_interface_data *, if_flags),
	OFFSET(llpm_interface_data *, if_name),
	OFFSET(llpm_interface_data *, port_address),
	OFFSET(llpm_interface_data *, if_vector),
	OFFSET(llpm_interface_data *, if_priority),
	OFFSET(llpm_interface_data *, if_level),
	OFFSET(llpm_interface_data *, alt_timeout),
	OFFSET(llpm_interface_data *, use_alt),
};

const u_int32	v1_llif_offset_table[] =
{
	OFFSET(v1_llpm_interface_data *, ip_address),
	OFFSET(v1_llpm_interface_data *, subnet_mask),
	OFFSET(v1_llpm_interface_data *, brdcst_address),
	OFFSET(v1_llpm_interface_data *, gw_address),
	OFFSET(v1_llpm_interface_data *, mac_address),
	OFFSET(v1_llpm_interface_data *, hwtype),
	OFFSET(v1_llpm_interface_data *, alt_parity),
	OFFSET(v1_llpm_interface_data *, alt_baudrate),
	OFFSET(v1_llpm_interface_data *, alt_wordsize),
	OFFSET(v1_llpm_interface_data *, alt_stopbits),
	OFFSET(v1_llpm_interface_data *, alt_flow),
	OFFSET(v1_llpm_interface_data *, if_flags),
	OFFSET(v1_llpm_interface_data *, if_name),
	OFFSET(v1_llpm_interface_data *, port_address),
	OFFSET(v1_llpm_interface_data *, if_vector),
	OFFSET(v1_llpm_interface_data *, if_priority),
	OFFSET(v1_llpm_interface_data *, if_level),
	OFFSET(v1_llpm_interface_data *, alt_timeout),
	OFFSET(v1_llpm_interface_data *, use_alt),
};


/*
 *
 *	error_code get_llif_config(enum config_element_id id, Mh_exec cnfgmod,
 *								void *buf)
 *
 */

error_code	get_llif_config(enum config_element_id id,
	u_int32 index,Mh_exec cnfgmod, void *buf, Rominfo rinf)
{
	CNFG_protoman	p;	/* Pointer to protoman configuration data */
	LLPM_interface_data pif; /* Pointer to interface configuration data */
	u_int32		offset; /* offset valuse */
	u_int32		datasize;	/* size of interface configuration data record */
	u_int32		addrsize;	/* size of address arrays */

	/* check protoman entry in configuration module */
	p = (CNFG_protoman)search_entry(cnfgmod, (u_int8 *)CNFG_TYPE_S_LLPM);
	if( p == NULL ) return	EOS_UNKCNFG;	/* if no entry, return error */
	
	/* check if specified index is within range */
	if( index >= p->llpm_count ) return EOS_UNKCNFG;
	
	/* form pointer to entry array */
	get_cptr((char **)&pif, (char **) &p->llpm_config,
#ifdef	RELATIVE	/* compiler generates self-relative offset */
		(char *)(&p->llpm_config)+1);
#else				/* compiler generates absolute offset */
		(char *)cnfgmod);
#endif

	/* index to array entry */
	if (p->ch.cnfg_rev == LLPM_VER_NODATASIZE)
		datasize = LLPM_V1_DATASIZE;
	else
		datasize = p->llpm_datasize;
	pif  = (LLPM_interface_data)((u_int32)pif + (index * datasize));

	/* get data offset value from table */
	if (p->ch.cnfg_rev == LLPM_VER_NODATASIZE) {
		offset = v1_llif_offset_table[id - LLPM_IF_START];
		addrsize = 8;
	} else {
		offset = llif_offset_table[id - LLPM_IF_START];
		addrsize = 16;
	}

/*	switch(id)
	{ */
	/* place (u_int8 *) fields here */
/*	case	LLPM_IF_MAC_ADDRESS: */
	if (id == LLPM_IF_MAC_ADDRESS) {
		if( pif->hwtype != LLPM_ETHER ) return EOS_UNKCNFG;
		get_uint8p((u_int8 *) buf, (u_int8 *)(offset + (u_int32)pif), addrsize);
/*		break; */

/*	case	LLPM_IF_IP_ADDRESS: */
	} else if ((id == LLPM_IF_IP_ADDRESS) ||
			   (id == LLPM_IF_SUBNET_MASK) ||
			   (id == LLPM_IF_BRDCST_ADDRESS) ||
			   (id == LLPM_IF_GW_ADDRESS))
	{
		get_uint8p((u_int8 *) buf, (u_int8 *)(offset + (u_int32)pif), addrsize);
/*		break; */
		
	/* place(char *) fields here */
/*	case	LLPM_IF_NAME: */
	} else if (id == LLPM_IF_NAME) {
		get_cptr( (char **)buf, (char **) (offset + (u_int32)pif),
#ifdef 	RELATIVE	/* compiler generates self-relative offset */
			(char *)(offset + (u_int32)pif + 1));
#else				/* compiler generates absolute offset */
			(char *)cnfgmod);
#endif /* RELATIVE */
/*		break; */
		
	/* place (u_int8) fields here */
/*	case	LLPM_IF_ALT_PARITY:
	case	LLPM_IF_ALT_BAUDRATE:
	case	LLPM_IF_ALT_WORDSIZE:
	case	LLPM_IF_ALT_STOPBITS:
	case	LLPM_IF_ALT_FLOW: */
	} else if ((id == LLPM_IF_ALT_PARITY) || (id == LLPM_IF_ALT_BAUDRATE) ||
			   (id == LLPM_IF_ALT_WORDSIZE) || (id == LLPM_IF_ALT_STOPBITS) ||
			   (id == LLPM_IF_ALT_FLOW))
	{
		if( pif->hwtype != LLPM_SLIP ) return EOS_UNKCNFG;
		get_uint8((u_int8 *) buf, (u_int8 *)(offset + (u_int32)pif));
/*		break; */

/*	case	LLPM_IF_TYPE: */
	} else if (id == LLPM_IF_TYPE) {
		get_uint8((u_int8 *) buf, (u_int8 *)(offset + (u_int32)pif));
/*		break; */

	/* place (u_int16) fields here */
/*	case	LLPM_IF_FLAGS: */
	} else if (id == LLPM_IF_FLAGS) {
		get_uint16((u_int16 *) buf, (u_int16 *)(offset + (u_int32)pif));
/*		break; */
		
	/* place (u_int32) fields here */
/*	case	LLPM_IF_ALT_TIMEOUT: */
	} else if (id == LLPM_IF_ALT_TIMEOUT) {
		if( pif->hwtype != LLPM_SLIP ) return EOS_UNKCNFG;
		get_uint32((u_int32 *) buf, (u_int32 *)(offset + (u_int32)pif));
/*		break; */

/*	case	LLPM_IF_PORT_ADDRESS:
	case	LLPM_IF_VECTOR:
	case	LLPM_IF_PRIORITY:
	case	LLPM_IF_LEVEL:
	case	LLPM_IF_USE_ALT: */
	} else if ((id == LLPM_IF_PORT_ADDRESS) || (id == LLPM_IF_VECTOR) ||
			   (id == LLPM_IF_PRIORITY) || (id == LLPM_IF_LEVEL) ||
			   (id == LLPM_IF_USE_ALT))
	{
		get_uint32((u_int32 *) buf, (u_int32 *)(offset + (u_int32)pif));
/*		break; */

/*	default: */
	} else {
		return	EOS_UNKCNFG;
	}
	return	SUCCESS;
}


/*
 *
 *	error_code get_llpm_config(enum config_element_id id, Mh_exec cnfgmod,
 *								void *buf)
 *
 */

error_code	get_llpm_config(enum config_element_id id,
	Mh_exec cnfgmod, void *buf)
{
	CNFG_protoman	p;	/* Pointer to protoman configuration data */
	u_int32		offset; /* offset valuse */

	/* check protoman entry in configuration module */
	p = (CNFG_protoman)search_entry(cnfgmod, (u_int8 *)CNFG_TYPE_S_LLPM);
	if( p == NULL ) return	EOS_UNKCNFG;	/* if no entry, return error */

	/* get data offset value from table */
	offset = llpm_offset_table[id - LLPM_START];

/*	switch(id)
	{ */

	/* Place (u_int16) fields here */
/*	case	LLPM_REVS:
	case	LLPM_MAXLLPMPROTOS:
	case	LLPM_MAXRCVMBUFS:
	case	LLPM_MAXLLPMCONNS: */
	if ((id == LLPM_REVS) || (id == LLPM_MAXLLPMPROTOS) ||
		(id == LLPM_MAXRCVMBUFS) || (id == LLPM_MAXLLPMCONNS))
	{
		get_uint16( (u_int16 *)buf, (u_int16 *) (offset + (u_int32)p));
/*		break; */

	/* Place (u_int32) fields here */
/*	case	LLPM_IFCOUNT: */
	} else if (id == LLPM_IFCOUNT) {
		get_uint32( (u_int32 *)buf, (u_int32 *) (offset + (u_int32)p));
/*		break; */

/*	default: */
	} else {
		return	EOS_UNKCNFG;
	}
	return	SUCCESS;
}


/*
 * The offset tables of BOOT data structures.
 *
 */
   
const u_int32	boot_offset_table[] =
{
	OFFSET(cnfg_bootsys *, ch.cnfg_rev),	
	OFFSET(cnfg_bootsys *, boot_count),
	OFFSET(cnfg_bootsys *, boot_cmdsize),
};

const u_int32	booter_offset_table[] =
{
	OFFSET(boot_data *, boot_abname),
	OFFSET(boot_data *, boot_newab),
	OFFSET(boot_data *, boot_newname),
	OFFSET(boot_data *, boot_automenu),
	OFFSET(boot_data *, boot_params),
	OFFSET(boot_data *, autoboot_delay),
};


/*
 *
 *	error_code get_booter_config(enum config_element_id id, u_int32 index,
 *									Mh_exec cnfgmod, void *buf, Rominfo rinf)
 *
 */

error_code	get_booter_config(enum config_element_id id, 
	u_int32 index, Mh_exec cnfgmod, void *buf, Rominfo rinf)
{
	CNFG_bootsys	p;	/* Pointer to bootsys configuration data */
	BOOT_data pbt; /* Pointer to booter configuration data */
	u_int32		offset; /* offset valuse */
	u_int32		datasize; /* size of booter configuration data record */

	/* check protoman entry in configuration module */
	p = (CNFG_bootsys)search_entry(cnfgmod, (u_int8 *)CNFG_TYPE_S_BOOT);
	if( p == NULL ) return	EOS_UNKCNFG;	/* if no entry, return error */
	
	/* check if specified index is within range */
	if( index >= p->boot_count ) return EOS_UNKCNFG;
	
	/* form pointer to entry array */
	get_cptr((char **)&pbt, (char **) &p->boot_config,
#ifdef	RELATIVE	/* compiler generates self-relative offset */
		(char *)(&p->boot_config)+1);
#else				/* compiler generates absolute offset */
		(char *)cnfgmod);
#endif

	/* index to array entry */
	if (p->ch.cnfg_rev == BOOT_VER_NODATASIZE)
		datasize = BOOT_V1_DATASIZE;
	else
		datasize = p->boot_datasize;
	pbt = (BOOT_data)((u_int32)pbt + (index * datasize));

	/* get data offset value from table */
	offset = booter_offset_table[id - BOOTER_START];

/*	switch(id)
	{ */
	
	/* place (char *) fields here */
/*	case	BOOTER_ABNAME: */
	if (id == BOOTER_ABNAME) {
		get_cptr( (char **)buf, (char **) (offset + (u_int32)pbt),
#ifdef	RELATIVE	/* compiler generates self-relative offset */
			(char *)(offset + (u_int32)pbt + 1));
#else				/* compiler generates absolute offset */
			(char *)cnfgmod);
#endif
/*	break; */

/*	case	BOOTER_PARAMS: */
	} else if (id == BOOTER_PARAMS) {
		if (p->ch.cnfg_rev < BOOT_VER_PARAMS)
			return	EOS_UNKCNFG;
		else {
			char *tempbuf;
		
			get_cptr( &tempbuf, (char **) (offset + (u_int32)pbt), NULL);
			if (tempbuf != NULL) {
				tempbuf = tempbuf + (u_int32)
#ifdef	RELATIVE	/* compiler generates self-relative offset */
					(offset + (u_int32)pbt + 1);
#else				/* compiler generates absolute offset */
					cnfgmod;
#endif
			}
			*((char **)buf) = tempbuf;
		}
/*		break; */
		
/*	case	BOOTER_NEWAB:
	case	BOOTER_NEWNAME: */
	} else if ((id == BOOTER_NEWAB) || (id == BOOTER_NEWNAME)) {
		char *tempbuf;
		
		get_cptr( &tempbuf, (char **) (offset + (u_int32)pbt), NULL);
		if (tempbuf != NULL) {
			tempbuf = tempbuf + (u_int32)
#ifdef	RELATIVE	/* compiler generates self-relative offset */
				(offset + (u_int32)pbt + 1);
#else				/* compiler generates absolute offset */
				cnfgmod;
#endif
		}
		*((char **)buf) = tempbuf;
/*		break; */
		
	/* place (u_int8) fields here */
/*	case	BOOTER_AUTOMENU: */
	} else if (id == BOOTER_AUTOMENU) {
		get_uint8((u_int8 *) buf, (u_int8 *)(offset + (u_int32)pbt));
/*		break; */
		
	/* place (u_int32) fields here */
/*	case	BOOTER_AUTODELAY: */
	} else if (id == BOOTER_AUTODELAY) {
		get_uint32((u_int32 *) buf, (u_int32 *)(offset + (u_int32)pbt));
/*		break; */

/*	default: */
	} else {
		return	EOS_UNKCNFG;
	}
	return	SUCCESS;
}


/*
 *
 *	error_code get_boot_config(enum config_element_id id, Mh_exec cnfgmod,
 *								void *buf)
 *
 */

error_code	get_boot_config(enum config_element_id id,
	Mh_exec cnfgmod, void *buf)
{
	CNFG_bootsys	p;	/* Pointer to bootsys configuration data */
	u_int32		offset; /* offset valuse */

	/* check bootsys entry in configuration module */
	p = (CNFG_bootsys)search_entry(cnfgmod, (u_int8 *)CNFG_TYPE_S_BOOT);
	if( p == NULL ) return	EOS_UNKCNFG;	/* if no entry, return error */

	/* get data offset value from table */
	offset = boot_offset_table[id - BOOT_START];

/*	switch(id)
	{ */

	/* Place (u_int16) fields here */
/*	case	BOOT_REVS: */
	if (id == BOOT_REVS) {
		get_uint16( (u_int16 *)buf, (u_int16 *) (offset + (u_int32)p));
/*		break; */

	/* Place (u_int32) fields here */
/*	case	BOOT_CMDSIZE: */
	} else if (id == BOOT_CMDSIZE) {
		if (p->ch.cnfg_rev < BOOT_VER_CMDSIZE)
			return	EOS_UNKCNFG;
		get_uint32( (u_int32 *)buf, (u_int32 *) (offset + (u_int32)p));
/*		break; */

/*	case	BOOT_COUNT: */
	} else if (id == BOOT_COUNT) {
		get_uint32( (u_int32 *)buf, (u_int32 *) (offset + (u_int32)p));
/*		break; */

/*	default: */
	} else {
		return	EOS_UNKCNFG;
	}
	return	SUCCESS;
}


/*
 * Offset table for Notify Services Configuration data entry.
 */


const u_int32	ntfy_offset_table[] =
{
	OFFSET(cnfg_ntfysvc *, ch.cnfg_rev),	
	OFFSET(cnfg_ntfysvc *, max_notifiers),
};


/*
 * error_code	get_ntfy_config(enum config_element_id id, Mh_exec cnfgmod,
 *									void *buf)
 *
 * This function returns an entry of notification service  configuration data.
 * If some error is occured in this function, it always returns E_UNKCNFG.
 */
 
error_code	get_ntfy_config(enum config_element_id id,
	Mh_exec cnfgmod, void *buf)
{
	CNFG_ntfysvc	p;		/* pointer to notification services configuration data */
	u_int32		offset;		/* offset valuse	*/
		
	/* get pointer to notification services configuration data entry */
	p = (CNFG_ntfysvc)search_entry(cnfgmod, (u_int8 *)CNFG_TYPE_S_NTFY);
	/* if no entry was found in configuration module.
	   return error. */
	if( p == NULL ) return	EOS_UNKCNFG;

	/* get an offset valuse for requested entry */
	offset = ntfy_offset_table[id - NTFY_START];
	
/*	switch(id)
	{ */

	/* Place (u_int32) field at here */
/*	case	NTFY_MAX_NOTIFIERS: */
	if (id == NTFY_MAX_NOTIFIERS) {
		get_uint32( (u_int32 *)buf, (u_int32 *) (offset + (u_int32)p));
/*		break; */

	/* Place (u_int16) field at here */
/*	case	NTFY_REVS: */
	} else if (id == NTFY_REVS) {
		get_uint16( (u_int16 *)buf, (u_int16 *) (offset + (u_int32)p));
/*		break; */

/*	default: */
	} else {
		return	EOS_UNKCNFG;
	}
	return	SUCCESS;	
}


/*
 * Module Service Entry Point
 *
 * get_config_data( enum config_element_id id, u_int32 index,
 *						Rominfo rinf, void *buf);
 *
 */

error_code	get_config_data(enum config_element_id id, 
	u_int32 index, Rominfo rinf, void *buf)
{
	Cnfg_svcs cnfgsvcs = rinf->config;
	Mh_exec	cnfgmod;
	
 	cnfgmod = (Mh_exec)(cnfgsvcs->config_data);
	
	/* Check data module pointer.	*/
	if(  cnfgmod == NULL ) {
		Mod_list maybedata = rinf->modules->rom_modlist;
		
		while (maybedata != NULL) {
			cnfgmod = (Mh_exec)maybedata->modptr;
			if (strcmp(CNFGDATA_MODNAME, (char *)MODNAME(cnfgmod)) == 0)
				break;
			maybedata = maybedata->mod_next;
		}
		if (maybedata == NULL) return(EOS_UNKCNFG);
		cnfgsvcs->config_data = cnfgmod;
	}

	if( ( CONS_START <= id && CONS_END > id ) ||
		( COMM_START <= id && COMM_END > id ) )
	{
		/* Console or Communication port data request	*/
		return get_cons_config(id, cnfgmod, buf);
	}
	else if( DEBUG_START <= id && DEBUG_END > id )
	{
		/* Debugger information request */
		return get_debug_config(id, cnfgmod, buf);		
	}
	else if( LLPM_START <= id && LLPM_END > id )
	{
		/* llpm information request	*/
		return get_llpm_config(id, cnfgmod, buf);		
	}
	else if( LLPM_IF_START <= id && LLPM_IF_END > id )
	{
		/* llif information request */
		return get_llif_config(id, index, cnfgmod, buf, rinf);
	}
	else if( BOOT_START <= id && BOOT_END > id )
	{
		/* boot system information request */
		return get_boot_config(id, cnfgmod, buf);
	}
	else if( BOOTER_START <= id && BOOTER_END > id )
	{
		/* booter information request */
		return get_booter_config(id, index, cnfgmod, buf, rinf);
	}
	else if( NTFY_START <= id && NTFY_END > id )
	{
		/* boot system information request */
		return get_ntfy_config(id, cnfgmod, buf);
	}
	return(EOS_UNKCNFG);
}


/* configuration  module initialization */

error_code p2start(Rominfo rinf, u_char *glbls)
{
	Cnfg_svcs	cnfgsvc;
	
	/* check for an existing configuration services module */
	
	if( rinf->config != NULL )
		return (EOS_DUPCNFG); /* yup, leave it alone and go bye bye */

	/* create the configuration services module structure */
	
	cnfgsvc = &config_r;
	
	cnfgsvc->struct_id = CNFGSVCID;
	cnfgsvc->struct_ver = CNFG_VER_MAX;
	
	cnfgsvc->get_config_data = &get_config_data;

	rinf->config = cnfgsvc;
	return (SUCCESS);
}

/* ------------------------------------------------------------------------- */
