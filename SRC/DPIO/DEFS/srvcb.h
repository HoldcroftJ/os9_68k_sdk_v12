#if !defined(_SRVCB_H)
#define _SRVCB_H

/*
 * $Header:   /h0/OS9000/SRC/DEFS/VCS/srvcb.h_v   1.3   25 Mar 1993 12:41:34   ROBB  $
 * $Revision:   1.3  $
 */

/*--------------------------------------------------------------------------,
!                                                                           !
!              Copyright 1989 by Microware Systems Corporation              !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!---------------------------------------------------------------------------!
!                                                                           !
!  Edition History:                                                         !
!   #   Date    Comments                                                By  !
!  -- -------- -------------------------------------------------------- --- !
!   1 88/02/23 Created                                                  rg  !
!   2 88/05/27 Corrected i_attach_pb and i_detach_pb definitions.       DJN !
!   3 88/05/27 Added f_unqu definition.                                 DJN !
!	4 88/07/07 Added SS_ and GS_ parameter sub-block structures			jal	!
!   5 88/08/02 Corrected i_allpd_pb and i_retpd_pb structures.          DJN !
!   6 88/08/05 Added i_iofork_pb and i_ioexit structures.				DJN	!
!   7 88/08/09 Added new I/O queueing definitions.                      DJN !
!	8 89/01/23 Added definitions for gs_fdaddr getstat. 				djl !
!   9 89/03/19 Added definitions for gs_dsize getstat.                  djl !
!  10 89/03/24 Changed ss_wtrack parameter block structure.             djl !
!  11 89/04/13 Added ss_link_pb parameter block structure.              djl !
!  12 89/04/20 Added cache related calls ss_cache, gs_cstat.            djl !
!  13 89/05/08 Added structures for sbf stat calls.                     djl !
!  14 89/05/11 Added f_get_mdp_pb parameter block.						afh !
!  15 89/05/12 Added the following system call parameter blocks:			!
!			   f_forkm_pb, f_chainm_pb, f_dforkm_pb, f_slinkm_pb,			!
!			   f_tlinkm_pb.												afh	!
!  16 89/05/18 Added fields to f_fork_pb and f_forkm_pb.				afh !
!  17 89/05/30 Added fields to f_sysid_pb.								afh !
!  18 89/07/31 Added gs_crc parameter block                             djl !
!  19 89/08/11 Added f_getsys_pb and f_setsys_pb parameter blocks.		afh !
!  20 89/10/26 Deleted pb dealing with julian, gregor and perr.			rcb	!
!  21 90/03/08 Fixed f_salarm_pb, remove unused signal parameter.		afh !
!  22 90/04/03 Added gs_edt parameter block.							afh	!
!  23 90/05/07 Added f_sigrs_pb parameter block.						afh !
!  24 90/06/04 Added i_getdl_pb parameter block.						afh !
!  25 91/07/24 Added gs_cpypd_pb get status parameter block.			afh !
!  26 92/04/16 Changed parity in _os_gs_parity to 32 bits.              djl !
!  27 92/09/17 Added pb for 4 VPC-related calls F$GETVPCMEM             amg !
!  28 94/10/19 Eleventh hour addition of RTNFM getstat and setstat			!
!			   parameter blocks for the DAVID group.					afh !
!                                                                           !
`--------------------------------------------------------------------------*/

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if !defined(_MODULE_H)
#include <module.h>
#endif

#if !defined(_MODDIR_H)
#include <moddir.h>
#endif

#if !defined(_PROCESS_H)
#include <process.h>
#endif

#if !defined(_IO_H)
#include <io.h>
#endif

#if !defined(_FORK_H)
#include <fork.h>
#endif

#if !defined(_RBF_H)
#include <rbf.h>
#endif

#if !defined(_DEXEC_H)
#include <dexec.h>
#endif

#if !defined(_ALLOC_H)
#include <alloc.h>
#endif

#if !defined(_SETTRAP_H)
#include <settrap.h>
#endif

/*********************************/
/* Common parameter block format */
typedef struct syscb {
	u_int16
		code,		/* function code number */
		edition;	/* function edition number */
	error_code
		error;		/* error code 0=success n=errnum */
	u_int32
		param_size;	/* size of parameter list */
} syscb;


/****************************************************/
/* parameter blocks for user state service routines */


/************************/
/* i/o service requests */

/* alias pathlist service request parameter block */
typedef struct i_alias_pb {
	syscb cb;
	u_char
		*alias_name,	/* assigned name */
		*real_name;		/* device name */
} i_alias_pb, *I_alias_pb;

/* attach service request parameter block */
typedef struct i_attach_pb {
	syscb cb;
	u_char
		*name;
	u_int16
		mode;
	Dev_list 
		dev_tbl;		/* output */
} i_attach_pb, *I_attach_pb;

/* chdir service request parameter block */
typedef struct i_chdir_pb {
	syscb cb;
	u_char
		*name;
	u_int16
		mode;
} i_chdir_pb, *I_chdir_pb;

/* close service request parameter block */
typedef struct i_close_pb {
	syscb cb;
	path_id
		path;
} i_close_pb, *I_close_pb;

/* create service request parameter block */
typedef struct i_create_pb {
	syscb cb;
	u_char
		*name;
	u_int16
		mode;
	path_id
		path;		/* output */
	u_int32 
		perm,
		size;
} i_create_pb, *I_create_pb;

/* delete service request parameter block */
typedef struct i_delete_pb {
	syscb cb;
	u_char
		*name;
	u_int16
		mode;
} i_delete_pb, *I_delete_pb;

/* detach service request parameter block */
typedef struct i_detach_pb {
	syscb cb;
	Dev_list 
		dev_tbl;
} i_detach_pb, *I_detach_pb;

/* dup service request parameter block */
typedef struct i_dup_pb {
	syscb cb;
	path_id
		dup_path,
		new_path;		/* output */
} i_dup_pb, *I_dup_pb;

/* get system device list pointer service request parameter block */
typedef struct i_getdl_pb {
	syscb cb;
	Dev_list
		dev_list_ptr;
} i_getdl_pb, *I_getdl_pb;

/* getstat service request parameter block */
typedef struct i_getstat_pb {
	syscb cb;
	path_id
		path;
	u_int16 
		gs_code;
	void 
		*param_blk;		/* input/output */
} i_getstat_pb, *I_getstat_pb;

/* makdir service request parameter block */
typedef struct i_makdir_pb {
	syscb cb;
	u_char
		*name;
	u_int16
		mode;
	u_int32 
		perm,
		size;
} i_makdir_pb, *I_makdir_pb;

/* open service request parameter block */
typedef struct i_open_pb {
	syscb cb;
	u_char
		*name;
	u_int16
		mode;
	path_id
		path;		/* output */
} i_open_pb, *I_open_pb;

typedef struct i_rdalst_pb {
	syscb cb;
	u_char *buffer;		/* buffer to put alias element */
	u_int32 count;		/* size of buffer */
} i_rdalst_pb, *I_rdalst_pb;

/* read service request parameter block */
typedef struct i_read_pb {
	syscb cb;
	path_id
		path;
	u_char
		*buffer;
	u_int32 
		count;		/* input/output */
} i_read_pb, *I_read_pb;

/* readln service request parameter block */
typedef struct i_readln_pb {
	syscb cb;
	path_id
		path;
	u_char
		*buffer;
	u_int32 
		count;		/* input/output */
} i_readln_pb, *I_readln_pb;

/* seek service request parameter block */
typedef struct i_seek_pb {
	syscb cb;
	path_id
		path;
	u_int32 
		offset;
} i_seek_pb, *I_seek_pb;

/* setstat service request parameter block */
typedef struct i_setstat_pb {
	syscb cb;
	path_id
		path;
	u_int16 
		ss_code;
	void 
		*param_blk;		/* input/output */
} i_setstat_pb, *I_setstat_pb;

/* write service request parameter block */
typedef struct i_write_pb {
	syscb cb;
	path_id
		path;
	u_char
		*buffer;
	u_int32 
		count;		/* input/output */
} i_write_pb, *I_write_pb;

/* writeln service request parameter block */
typedef struct i_writeln_pb {
	syscb cb;
	path_id
		path;
	u_char
		*buffer;
	u_int32 
		count;		/* input/output */
} i_writeln_pb, *I_writeln_pb;

/****************************/
/* general service requests */

/* set alarm clock service request parameter block */
typedef struct f_alarm_pb {
	syscb cb;
	alarm_id
		alrm_id;	/* input/output */
	u_int16
		alrm_code;
	u_int32
		time,
		date;
	signal_code
		signal;
} f_alarm_pb, *F_alarm_pb;

/* set alternate module directory parameter block */
typedef struct f_altmdir_pb {
	syscb cb;
	u_char 
		*name;
} f_altmdir_pb, *F_altmdir_pb;

/* cache control service request parameter block */
typedef struct f_cache_pb {
	syscb cb;
	u_int32
		control;
} f_cache_pb, *F_cache_pb;

/* chain service request parameter block */
typedef struct f_chain_pb {
	syscb cb;
	u_int16
		priority,
		path_cnt;
	u_char
		*mod_name;
	u_char
		*params;
	u_int32 
		mem_size,
		param_size;
	u_int16
		type_lang;
} f_chain_pb, *F_chain_pb;

/* chainm by module pointer service request parameter block */
typedef struct f_chainm_pb {
	syscb cb;
	u_int16
		priority,
		path_cnt;
	Mh_com
		mod_head;
	u_char
		*params;
	u_int32 
		mem_size,
		param_size;
} f_chainm_pb, *F_chainm_pb;

/* change module directories service request */
typedef struct f_chmdir_pb {
	syscb cb;
	u_char
		*name;
} f_chmdir_pb, *F_chmdir_pb;

/* clear signal queue service request */
typedef struct f_clrsigs_pb {
	syscb cb;
	process_id
		proc_id;
} f_clrsigs_pb, *F_clrsigs_pb;

/* change module directory user identifier */
typedef struct f_cmdperm_pb {
	syscb cb;
	u_char
		*name;
	u_int16
		perm;
} f_cmdperm_pb, *F_cmdperm_pb;

/* compare name service request parameter block */
typedef struct f_cmpnam_pb {
	syscb cb;
	u_int32
		length;
	u_char
		*string,
		*pattern;
	int32
		result;
} f_cmpnam_pb, *F_cmpnam_pb;

/* copy memory service request parameter block */
typedef struct f_cpymem_pb {
	syscb cb;
	process_id
		proc_id; 
	u_char
		*from,
		*to;
	u_int32 
		count; 
} f_cpymem_pb, *F_cpymem_pb;

/* generate crc service request parameter block */
typedef struct f_crc_pb {
	syscb cb;
	u_char
		*start;
	u_int32 
		count,
		accum;		/* input/output */
} f_crc_pb, *F_crc_pb;

/* create data module service request parameter block */
typedef struct f_datmod_pb {
	syscb cb;
	u_char
		*mod_name; 
	u_int32 
		size;
	u_int16
		attr_rev,	/* input/output */
		type_lang,	/* output */
		perm;
	void
		*mod_exec;	/* output */
	Mh_com
		mod_head;	/* output */
} f_datmod_pb, *F_datmod_pb;

/* delete module directory service request */
typedef struct f_delmdir_pb {
	syscb cb;
	u_char
		*name;
} f_delmdir_pb, *F_delmdir_pb;

/* debug execute call service request parameter block */
typedef struct f_dexec_pb {
	syscb cb;
	process_id
		proc_id;		/* input - process identifier */
	dexec_mode
		mode;			/* input - dexec execution mode */
	u_char 
		*params;		/* input - pointer to additional parameter(s) */
	u_int32 
		num_instr,		/* input/output - number of instructions */
		tot_instr,		/* output - total instructions executed so far */
		except,			/* exception number (on exception only) */
		addr;			/* fault address (on exception only) */
	u_int16
		num_bpts,		/* input - number of break points */
		**brk_pts;		/* input - break point list */
	dexec_status
		status;			/* output - return status code */
	error_code
		exit_status;	/* output - exit status of child process */
} f_dexec_pb, *F_dexec_pb;

/* debug exit call service request parameter block */
typedef struct f_dexit_pb {
	syscb cb;
	process_id
		proc_id;
} f_dexit_pb, *F_dexit_pb;

/* debug fork call service request parameter block */
typedef struct f_dfork_pb {
	syscb cb;
	u_int16
		priority,
		path_cnt;
	process_id
		proc_id;		/* output */
	Regs
		reg_stack;		/* input/output */
	Fregs
		freg_stack;		/* input/output */
	u_char
		*mod_name;
	u_char
		*params;
	u_int32 
		mem_size,
		param_size;
	u_int16
		type_lang;
} f_dfork_pb, *F_dfork_pb;

/* debug fork by module pointer service request parameter block */
typedef struct f_dforkm_pb {
	syscb cb;
	u_int16
		priority,
		path_cnt;
	process_id
		proc_id;		/* output */
	Regs
		reg_stack;		/* input/output */
	Fregs
		freg_stack;		/* input/output */
	Mh_com
		mod_head;
	u_char
		*params;
	u_int32 
		mem_size,
		param_size;
} f_dforkm_pb, *F_dforkm_pb;

/*******************************************************************/
/* Event routines - cb._fcode are the same for all these routines */

/* event creat service request parameter block */
typedef struct f_evcreat_pb {
	syscb cb;
	u_int16
		ev_code,
		wait_inc,
		sig_inc,
		perm,
		color;
	event_id
		ev_id;		/* output */
	u_char
		*ev_name;
	int32 
		value;
} f_evcreat_pb, *F_evcreat_pb;

/* event link service request parameter block */
typedef struct f_evlink_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;		/* output */
	u_char
		*ev_name;
} f_evlink_pb, *F_evlink_pb;

/* event unlink service request parameter block */
typedef struct f_evunlnk_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
} f_evunlnk_pb, *F_evunlnk_pb;

/* event delete service request parameter block */
typedef struct f_evdelet_pb {
	syscb cb;
	u_int16
		ev_code;
	u_char
		*ev_name;
} f_evdelet_pb, *F_evdelet_pb;

/* event info service request parameter block */
typedef struct f_evinfo_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	u_int32
		size;
	u_char
		*buffer;
} f_evinfo_pb, *F_evinfo_pb;

/* event signal service request parameter block */
typedef struct f_evsignl_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	u_int32 
		actv_flag;
} f_evsignl_pb, *F_evsignl_pb;

/* event read service request parameter block */
typedef struct f_evread_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;	/* output */
} f_evread_pb, *F_evread_pb;

/* event pulse service request parameter block */
typedef struct f_evpulse_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id; 
	int32 
		value;	/* input/output */
	u_int32
		actv_flag;
} f_evpulse_pb, *F_evpulse_pb;

/* event set service request parameter block */
typedef struct f_evset_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32 
		value;		/* input/output */
	u_int32
		actv_flag;
} f_evset_pb, *F_evset_pb;

/* event set relative service request parameter block */
typedef struct f_evsetr_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32 
		value;		/* input/output */
	u_int32
		actv_flag;
} f_evsetr_pb, *F_evsetr_pb;

/* event wait service request parameter block */
typedef struct f_evwait_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	signal_code
		signal;		/* output */
	u_int32 
		min_val,
		max_val;
} f_evwait_pb, *F_evwait_pb;

/* event wait relative service request parameter block */
typedef struct f_evwaitr_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	signal_code
		signal;		/* output */
	u_int32 
		min_val,	/* input/output */
		max_val;	/* input/output */
} f_evwaitr_pb, *F_evwaitr_pb;

/* event set OR service request parameter block */
typedef struct f_evsetor_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	u_int32 
		mask,
		actv_flag;
} f_evsetor_pb, *F_evsetor_pb;

/* event set AND service request parameter block */
typedef struct f_evsetand_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	u_int32 
		mask,
		actv_flag;
} f_evsetand_pb, *F_evsetand_pb;

/* event set XOR service request parameter block */
typedef struct f_evsetxor_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	u_int32 
		mask,
		actv_flag;
} f_evsetxor_pb, *F_evsetxor_pb;

/* event wait any bits set service request parameter block */
typedef struct f_evanyset_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	signal_code
		signal;		/* output */
	u_int32 
		mask;
} f_evanyset_pb, *F_evanyset_pb;

/* event wait all bits set service request parameter block */
typedef struct f_evallset_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	signal_code
		signal;		/* output */
	u_int32 
		mask;
} f_evallset_pb, *F_evallset_pb;

/* event wait any bits clear service request parameter block */
typedef struct f_evanyclr_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	signal_code
		signal;		/* output */
	u_int32 
		mask;
} f_evanyclr_pb, *F_evanyclr_pb;

/* event wait all bits clear service request parameter block */
typedef struct f_evallclr_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	signal_code
		signal;		/* output */
	u_int32 
		mask;
} f_evallclr_pb, *F_evallclr_pb;

/* event wait test and set service request parameter block */
typedef struct f_evtstset_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	signal_code
		signal;		/* output */
	u_int32 
		mask;
} f_evtstset_pb, *F_evtstset_pb;

/* event wait change service request parameter block */
typedef struct f_evchange_pb {
	syscb cb;
	u_int16
		ev_code;
	event_id
		ev_id;
	int32
		value;		/* output */
	signal_code
		signal;		/* output */
	u_int32 
		mask;
	u_int32
		pattern;
} f_evchange_pb, *F_evchange_pb;


/* exit service request parameter block */
typedef struct f_exit_pb {
	syscb cb;
	status_code
		status;
} f_exit_pb, *F_exit_pb;

/* find module entry service request */
typedef struct f_findmod_pb {
	syscb cb;
	u_int16
		type_lang;	/* input/ouput */
	Mod_dir
		moddir_entry;	/* output */
	u_char
		*mod_name;
} f_findmod_pb, *F_findmod_pb;

/* fork service request parameter block */
typedef struct  f_fork_pb {
	syscb cb;
	u_int16
		priority,
		path_cnt;
	process_id
		proc_id;		/* output */
	u_char
		*mod_name,
		*params;
	u_int32 
		mem_size,
		param_size;
	u_int16
		type_lang;
	u_int16
		orphan;
} f_fork_pb, *F_fork_pb;

/* fork by module pointer service request parameter block */
typedef struct  f_forkm_pb {
	syscb cb;
	u_int16
		priority,
		path_cnt;
	process_id
		proc_id;		/* output */
	Mh_com
		mod_head;
	u_char
		*params;
	u_int32 
		mem_size,
		param_size;
	u_int16
		orphan;
} f_forkm_pb, *F_forkm_pb;

/* get free memory block map request service request parameter block */
typedef struct f_gblkmp_pb {
	syscb cb;
	Mem_list
		start;
	u_char
		*buffer;		/* intput/output */
	u_int32 
		size,
		min_alloc,	/* output */
		num_segs,	/* output */
		tot_mem,	/* output */
		free_mem;	/* output */
} f_gblkmp_pb, *F_gblkmp_pb;

/* getime service request parameter block */
typedef struct f_getime_pb {
	syscb cb;
	u_int32 
		time,		/* output */
		ticks;		/* output */
} f_getime_pb, *F_getime_pb;

/* get module directory pathlists service request parameter block */
typedef struct f_get_mdp_pb {
	syscb cb;
	u_char
		*current,
		*alternate;
} f_get_mdp_pb, *F_get_mdp_pb;

/* get module directory service request parameter block */
typedef struct f_get_moddir_pb {
	syscb cb;
	u_char
		*buffer;
	u_int32 
		count;	/* input/output */
} f_get_moddir_pb, *F_get_moddir_pb;

/* get process table service request parameter block */
typedef struct f_get_prtbl_pb {
	syscb cb;
	u_char
		*buffer; 
	u_int32 
		count;	/* input/output */
} f_get_prtbl_pb, *F_get_prtbl_pb;


/* get system global variable service request parameter block */
typedef struct f_getsys_pb {
	syscb cb;
	u_int32
		offset;		/* offset into system globals */
	u_int32
		size;		/* size of variable: 1, 2, or 4 bytes */
	union {
		u_char 
			byt;	/* byte size variable */
		u_int16
			wrd;	/* word size variable */
		u_int32
			lng;	/* lonk size variable */
	} sysvar;
} f_getsys_pb, *F_getsys_pb;


/* get process descriptor copy service request parameter block */
typedef struct f_gprdsc_pb {
	syscb cb;
	process_id
		proc_id;
	u_char
		*buffer; 
	u_int32 
		count;
} f_gprdsc_pb, *F_gprdsc_pb;

/* process identification service request parameter block */
typedef struct f_id_pb {
	syscb cb;
	process_id
		proc_id;	/* output */
	u_int16
		priority,	/* output */ 
		age;		/* output */
	int32
		schedule;	/* output */
	owner_id
		user_id;	/* output */
} f_id_pb, *F_id_pb;

/* initialize process data area service request */
typedef struct f_init_data_pb {
	syscb cb;	
	Mh_com
		mod_head;
	u_char 
		*data;
} f_init_data_pb, *F_init_data_pb;

/* signal intercept service request parameter block */
typedef struct f_intercept_pb {
	syscb cb;
	u_int32 
		(*function)();
	void 
		*data_ptr;
} f_intercept_pb, *F_intercept_pb;

/* module link service request parameter block */
typedef struct f_link_pb {
	syscb cb;
	u_char
		*mod_name; 
	Mh_com
		mod_head;		/* output */
	void 
		*mod_exec;		/* output */
	u_int16
		type_lang,	/* input/output */
		attr_rev;	/* output */
} f_link_pb, *F_link_pb;

/* module link by module pointer service request parameter block */
typedef struct f_linkm_pb {
	syscb cb;
	Mh_com
		mod_head;		/* input */
	void 
		*mod_exec;		/* output */
	u_int16
		type_lang,		/* output */
		attr_rev;		/* output */
} f_linkm_pb, *F_linkm_pb;

/* module load service request parameter block */
typedef struct f_load_pb {
	syscb cb;
	u_char
		*mod_name; 
	Mh_com
		mod_head;		/* output */
	void 
		*mod_exec;		/* output */
	u_int32 
		mode;
	u_int16
		type_lang,	/* output */
		attr_rev,	/* output */
		color;
} f_load_pb, *F_load_pb;

/* create module directory service request parameter block */
typedef struct f_makmdir_pb {
	syscb cb;
	u_char 
		*name;
	u_int16
		perm;
} f_makmdir_pb, *F_makmdir_pb;

/* memory area resize service request parameter block */
typedef struct f_mem_pb {
	syscb cb;
	u_char
		*mem_ptr;		/* output */
	u_int32 
		size;		/* input/output */
} f_mem_pb, *F_mem_pb;

/* get module header address given a pointer service request */
typedef struct f_modaddr_pb {
	syscb cb;
	u_char
		*mem_ptr;		/* input */
	Mh_com
		mod_head;		/* output */
} f_modaddr_pb, *F_modaddr_pb;

/* parse name service request parameter block */
typedef struct f_prsnam_pb {
	syscb cb;
	u_char
		*name;
	u_int32
		length;		/* output */
	u_char
		delimiter,		/* output */
		*updated;		/* output */
} f_prsnam_pb, *F_prsnam_pb;

/* return from interrupt exception service request parameter block */
typedef struct f_rte_pb {
	syscb cb;
	u_int32
		mode;		/* mode of return - 0 = normal signal return
										1 = clean stack only (for longjmp) */
} f_rte_pb, *F_rte_pb;

/* send signal service request parameter block */
typedef struct f_send_pb {
	syscb cb;
	process_id
		proc_id;
	signal_code
		signal;
} f_send_pb, *F_send_pb;

/* generate crc in module service request parameter block */
typedef struct f_setcrc_pb {
	syscb cb;
	Mh_com
		mod_head;
} f_setcrc_pb, *F_setcrc_pb;

/* setime service request parameter block */
typedef struct f_setime_pb {
	syscb cb;
	u_int32 
		time;
} f_setime_pb, *F_setime_pb;

/* set priority service request parameter block */
typedef struct f_setpr_pb {
	syscb cb;
	process_id
		proc_id;
	u_int16
		priority;
} f_setpr_pb, *F_setpr_pb;

/* set system global variable service request parameter block */
typedef struct f_setsys_pb {
	syscb cb;
	u_int32
		offset;		/* offset into system globals */
	u_int32
		size;		/* size of variable: 1, 2, or 4 bytes */
	union {
		u_char 
			byt;	/* byte size variable */
		u_int16
			wrd;	/* word size variable */
		u_int32
			lng;	/* lonk size variable */
	} sysvar;
} f_setsys_pb, *F_setsys_pb;

/* set user id service request parameter block */
typedef struct f_setuid_pb {
	syscb cb;
	owner_id 
		user_id;		/* output */
} f_setuid_pb, *F_setuid_pb;

/* signal masking service request parameter block */
typedef struct f_sigmask_pb {
 	syscb cb;
	u_int32 
		mode;
} f_sigmask_pb, *F_sigmask_pb;

/* resize process' signal queue block parameter block */
typedef struct f_sigrs_pb {
	syscb cb;
	u_int32
		signals;
} f_sigrs_pb, *F_sigrs_pb;

/* sleep service request parameter block */
typedef struct f_sleep_pb {
	syscb cb;
	u_int32 
		ticks;		/* input/output */
	signal_code
		signal;		/* output */
} f_sleep_pb, *F_sleep_pb;

/* link subroutine library module service request parameter block */
typedef struct  f_slink_pb {
	syscb cb;
	u_int16
		sub_num;	/* input */
	u_char
		*mod_name;	/* input */
	void
		*lib_exec;	/* output */
	u_char
		*mem_ptr;	/* output */
	Mh_com
		mod_head;	/* output */
} f_slink_pb, *F_slink_pb;

/* link subroutine library module by pointer service request parameter block */
typedef struct  f_slinkm_pb {
	syscb cb;
	u_int16
		sub_num;	/* input */
	Mh_com
		mod_head;	/* input */
	void
		*lib_exec;	/* output */
	u_char
		*mem_ptr;	/* output */
} f_slinkm_pb, *F_slinkm_pb;

/* memory request service request parameter block */
typedef struct f_srqmem_pb {
	syscb cb;
	u_char
		*mem_ptr;		/* output */
	u_int32 
		size;		/* input/output */
	u_int16
		color;
} f_srqmem_pb, *F_srqmem_pb;

/* memory return service request parameter block */
typedef struct f_srtmem_pb {
	syscb cb;
	u_char
		*mem_ptr;
	u_int32 
		size;
} f_srtmem_pb, *F_srtmem_pb;

/* set error trap handler service request parameter block */
typedef struct f_strap_pb {
	syscb cb;
	u_char
		*excpt_stack;
	Strap
		init_tbl;
} f_strap_pb, *F_strap_pb;

/* sysdbg call service request parameter block */
typedef struct f_sysdbg_pb {
	syscb cb;
	u_int32
		*param1,
		*param2;
} f_sysdbg_pb, *F_sysdbg_pb;

/* return system identification service request parameter block */
typedef struct f_sysid_pb {
	syscb cb;
	u_int32
		oem,
		serial,
		mpu_type,
		os_type,
		fpu_type;
	int32
		time_zone;
	u_int32
		resv1,
		resv2;
	u_char
		*sys_ident,
		*copyright,
		*resv3;
} f_sysid_pb, *F_sysid_pb;

/* exit from trap handler termination routine service request parameter block */
typedef struct f_texit_pb {
	syscb cb;
	u_int16
		trap_num;
} f_texit_pb, *F_texit_pb;

/* install user trap handler service request parameter block */
typedef struct f_tlink_pb {
	syscb cb;
	u_int16
		trap_num;
	u_char
		*mod_name;
	void 
		*lib_exec,		/* output */
		*mod_head,		/* output */
		*params;		/* output */
	u_int32 
		mem_size;
} f_tlink_pb, *F_tlink_pb;

/* install user trap handler by pointer service request parameter block */
typedef struct f_tlinkm_pb {
	syscb cb;
	u_int16
		trap_num;
	Mh_com
		mod_head;
	void 
		*lib_exec,		/* output */
		*params;		/* output */
	u_int32 
		mem_size;
} f_tlinkm_pb, *F_tlinkm_pb;

/* user accounting service request parameter block */
typedef struct f_uacct_pb {
	syscb cb;
	u_int16
		acct_code;
	Pr_desc
		proc_desc;
/*	void 
		*vsect_ptr;  */
} f_uacct_pb, *F_uacct_pb;

/* module unlink service request parameter block */
typedef struct f_unlink_pb {
	syscb cb;
	Mh_com 
		mod_head;
} f_unlink_pb, *F_unlink_pb;

/* module unload service request parameter block */
typedef struct f_unload_pb {
	syscb cb;
	u_char
		*mod_name;
	u_int16
		type_lang;
} f_unload_pb, *F_unload_pb;

/* wait service request parameter block */
typedef struct f_wait_pb {
	syscb cb;
	process_id
		child_id;	/* output */
	status_code
		status;		/* output */
} f_wait_pb, *F_wait_pb;

/* get VPC Memory address parameter block */
typedef struct f_getvpcmem_pb {
	syscb cb;
	process_id  vpc_pid;  /* input */
	u_int32     vpc_mem;  /* output*/
} f_getvpcmem_pb, *F_getvpcmem_pb;

/* Get VPC Shared Memory Event parameter block */
typedef struct f_vpc_gmemev_pb {
	syscb		cb;
	process_id	vpc_pid;
	event_id	ev_id;
	u_int32		*size;
} f_vpc_gmemev_pb, *F_vpc_gmemev_pb;

/* Drop VPC Shared Memory Event parameter block */
typedef struct f_vpc_dmemev_pb {
	syscb		cb;
	process_id	vpc_pid;
} f_vpc_dmemev_pb, *F_vpc_dmemev_pb;

/* struct for Get VPC Processes Table */
typedef struct f_get_vpcptab_pb {
	syscb		cb;
	u_int32		*count;
	process_id	*table;
} f_get_vpcptab_pb, *F_get_vpcptab_pb;

/**********************************************************/
/*** parameter blocks for system state service routines ***/
/**********************************************************/

typedef struct f_aproc_pb {
	syscb cb;
	process_id
		proc_id;
} f_aproc_pb, *F_aproc_pb;

typedef struct f_allprc_pb {
	syscb cb;
	process_id
		proc_id;	/* output */
	Pr_desc
		proc_desc;	/* output */
} f_allprc_pb, *F_allprc_pb;

typedef struct f_alltsk_pb {
	syscb cb;
	Pr_desc
		proc_desc;
} f_alltsk_pb, *F_alltsk_pb;

/* verify memory is accessable (spu) service request parameter block */
typedef struct f_chkmem_pb {
	syscb cb;
	u_int32
		size;
	u_int16
		mode;
	u_char
		*mem_ptr;
	Pr_desc
		proc_desc;
} f_chkmem_pb, *F_chkmem_pb;

typedef struct f_deltsk_pb {
	syscb cb;
	Pr_desc
		proc_desc;
} f_deltsk_pb, *F_deltsk_pb;

/* get process desc ptr service request parameter block */
typedef struct f_findpd_pb {
	syscb cb;
	process_id
		proc_id;
	Pr_desc
		proc_desc;	/* output */
} f_findpd_pb, *F_findpd_pb;

typedef struct f_irq_pb {
	syscb cb;
	u_int16
		vector,
		priority;
	void 
		*irq_entry;
	u_char
		*statics;
} f_irq_pb, *F_irq_pb;

typedef struct f_move_pb {
	syscb cb;
	u_char
		*from,
		*to;
	u_int32 
		count;
} f_move_pb, *F_move_pb;

typedef struct f_msfork_pb {
	syscb cb;
	u_char
		*msdos_name;		/* msdos file path list */
	u_int32
		*bios_data,			/* bios data area */
		data_size,			/* size of msdos area */
		(*bios_init)();		/* pointer to bios init routine */
	u_int16
		mode;			/* open mode for msdos & prog */		
	u_char unforking; /* 0 if forking, 1 if unforking */
} f_msfork_pb, *F_msfork_pb;

typedef struct f_nproc_pb {
	syscb cb;
} f_nproc_pb, *F_nproc_pb;

typedef struct f_permit_pb {
	syscb cb;
	process_id
		pid;
	u_int32
		size;
	u_char
		*mem_ptr;
	u_int16
		perm;
} f_permit_pb, *F_permit_pb;

typedef struct f_protect_pb {
	syscb cb;
	process_id
		pid;
	u_int32
		size;
	u_char 
		*mem_ptr;
	u_int16
		perm;
} f_protect_pb, *F_protect_pb;

typedef struct f_retpd_pb {
	syscb cb;
	process_id
		proc_id;
} f_retpd_pb, *F_retpd_pb;

/* set alarm clock service request parameter block */
typedef struct f_salarm_pb {
	syscb cb;
	alarm_id
		alrm_id;	/* input/output */
	u_int16
		alrm_code;
	u_int32
		time,
		date,
		flags;		
	u_int32
		(*function)();
	void
		*func_pb;
} f_salarm_pb, *F_salarm_pb;

/* system state cache control service request parameter block */
typedef struct f_scache_pb {
	syscb cb;
	u_int32
		control;
	u_int32
		(*cctl)();
	void
		*cctl_data;
} f_scache_pb, *F_scache_pb;

typedef struct f_sspd_pb {
	syscb cb;
	process_id
		proc_id;
} f_sspd_pb, *F_sspd_pb;

typedef struct f_ssvc_pb {
	syscb cb;
	u_int32
		count;
	u_int16
		state_flag;
	void 
		*init_tbl,
		*params;
} f_ssvc_pb, *F_ssvc_pb;

typedef struct f_vmodul_pb {
	syscb cb;
	Mh_com 
		mod_head,
		mod_block;
	u_int32 
		block_size;
} f_vmodul_pb, *F_vmodul_pb;

typedef struct f_vpermit_pb {
	syscb cb;
	process_id
		pid;
	u_int32
		size;
	u_char
		*pmem_ptr,
		*vmem_ptr;
	u_int16
		perm;
} f_vpermit_pb, *F_vpermit_pb;

/* Allocate path descriptor service request */
typedef struct i_allpd_pb {
	syscb cb;
	u_int32
		size;			/* requested size of path descriptor */
	Pd_com
		path_desc;		/* output */
} i_allpd_pb, *I_allpd_pb;

/* Acquire resource lock service request. */
typedef struct f_acqlk_pb {
	syscb cb;
	lock_id
		lid;				/* queue to acquire */
	signal_code
		signal;				/* if signal occurs */
} f_acqlk_pb, *F_acqlk_pb;

/* Conditionally acquire resource lock service request. */
typedef struct f_caqlk_pb {
	syscb cb;
	lock_id
		lid;				/* queue to acquire */
} f_caqlk_pb, *F_caqlk_pb;

/* Create resource lock service request. */
typedef struct f_crlk_pb {
	syscb cb;
	lock_id
		lid;				/* output: new queue id */
} f_crlk_pb, *F_crlk_pb;

typedef struct f_ddlk_pb {
	syscb cb;
	process_id
		proc_id;
} f_ddlk_pb, *F_ddlk_pb;

/* Delete resource lock service request. */
typedef struct f_dellk_pb {
	syscb cb;
	lock_id
		lid;				/* queue to delete */
} f_dellk_pb, *F_dellk_pb;

/* Release resource lock service request. */
typedef struct f_rellk_pb {
	syscb cb;
	lock_id
		lid;				/* queue to release */
} f_rellk_pb, *F_rellk_pb;

/* Translate a memory address */
typedef struct f_transadd_pb {
	syscb cb;
	process_id
		pid;				/* process context to use */
	u_int32
		op;					/* what translation to do */
	u_char 
		*saddr,				/* the address we have */
		*daddr;				/* the address we want */
} f_transadd_pb, *F_transadd_pb;

/* Find a free space in a user map */
typedef struct f_findfree_pb {
	syscb cb;
	process_id
		pid;				/* the process context to use */
	u_int32
		direction,			/* from upper or from lower */
		size;				/* amount of free space needed */
	u_char	
		*paddr,				/* the physical address */
		*vaddr;				/* a suitable virtual location */
} f_findfree_pb, *F_findfree_pb;

/* Wait on resource lock service request. */
typedef struct f_waitlk_pb {
	syscb cb;
	lock_id
		lid;				/* queue to wait */
	signal_code
		signal;				/* if signal occurs */
} f_waitlk_pb, *F_waitlk_pb;

typedef struct i_cioproc_pb {
	syscb cb;
	process_id
		proc_id;		/* process id for I/O process descriptor */
	void
		*buffer;		/* pointer to buffer for I/O process descriptor */
	u_int32
		count;			/* number of bytes desired */
} i_cioproc_pb, *I_cioproc_pb;

/* Find path descriptor service request */
typedef struct i_getpd_pb {
	syscb cb;
	path_id
		path;
	Pd_com
		path_desc;	/* output */
} i_getpd_pb, *I_getpd_pb;

/* Return pointer to I/O process descriptor */
typedef struct i_gioproc_pb {
	syscb cb;
	process_id
		proc_id;		/* process id for I/O process descriptor */
	Io_proc
		proc_desc;		/* output, I/O process descriptor */
} i_gioproc_pb, *I_gioproc_pb;

/* Return copy of I/O process descriptor */
/* I/O delete service request parameter block */
typedef struct i_iodel_pb {
	syscb cb;
	Mh_com
		mod_head;
} i_iodel_pb, *I_iodel_pb;

/* Terminate I/O for exiting process service request parameter block */
typedef struct i_ioexit_pb {
	syscb cb;
	process_id
		proc_id;		/* process id of terminating process */
	u_int32
		path_cnt;		/* paths to remain open, high order bit */
						/* set means chain. */
} i_ioexit_pb, *I_ioexit_pb;

/* Setup I/O for new process service request parameter block */
typedef struct i_iofork_pb {
	syscb cb;
	process_id
		par_proc_id,	/* process id of parent */
		new_proc_id;	/* process id of new process */
	u_int32
		path_cnt;	/* number of paths to be inherited from parent */
} i_iofork_pb, *I_iofork_pb;

/* Return path descriptor service request */
typedef struct i_retpd_pb {
	syscb cb;
	path_id
		path;
	u_int32
		size;				/* size of path descriptor */
} i_retpd_pb, *I_retpd_pb;

/* Translate user path to system path service request. */
typedef struct i_tranpn_pb {
	syscb cb;
	process_id
		proc_id;
	path_id
		user_path,
		sys_path;
} i_tranpn_pb, *I_tranpn_pb;

/*--------------------------------------------------------------------------!
! setstat parameter sub-block structure defs								!
!--------------------------------------------------------------------------*/

/* SS_PathOpt parameter sub-block	*/
typedef struct ss_popt_pb {
	u_int32
		popt_size;					/* size of default options to copy	*/
	void
		*user_popts;				/* default options buffer pointer	*/
} ss_popt_pb,*Ss_popt_pb;

/* SS_sendsig parameter sub-block	*/
typedef struct ss_sendsig_pb {
	signal_code
		signal;						/* user defined signal code			*/
} ss_sendsig_pb,*Ss_sendsig_pb;

/* SS_DCOn parameter sub-block	*/
typedef struct ss_dcon_pb {
	signal_code
		signal;						/* user defined signal code			*/
} ss_dcon_pb,*Ss_dcon_pb;

/* SS_DCOff parameter sub-block	*/
typedef struct ss_dcoff_pb {
	signal_code
		signal;						/* user defined signal code			*/
} ss_dcoff_pb,*Ss_dcoff_pb;

/* SS_Size parameter sub-block	*/
typedef struct ss_size_pb {
	u_int32
		filesize;					/* new file size					*/
} ss_size_pb, *Ss_size_pb;

/* SS_wtrack parameter sub-block	*/
typedef struct ss_wtrack_pb {
	void		*trkbuf,			/* the track buffer					*/
				*ilvtbl;			/* the interleave table				*/
	u_int32		track,				/* track number						*/
				head,				/* the head number					*/
				interleave;			/* interleave value					*/
} ss_wtrack_pb, *Ss_wtrack_pb;

/* SS_Fd parameter sub-block	*/
typedef struct ss_fd_pb {
	Fd_stats	fd_info;			/* info for the fd					*/
} ss_fd_pb, *Ss_fd_pb;

/* SS_Ticks parameter sub-block */
typedef struct ss_ticks_pb {
	u_int32		delay;				/* lockout sleep count 				*/
} ss_ticks_pb, *Ss_ticks_pb;

/* SS_Lock parameter sub-block	*/
typedef struct ss_lock_pb {
	u_int32		size;				/* lockout size						*/
} ss_lock_pb, *Ss_lock_pb;

/* SS_LUOPT parameter sub-block	*/
typedef struct ss_luopt_pb {
	u_int32
		luopt_size;					/* size of logical unit options to copy	*/
	void
		*user_luopts;				/* logical unit options buffer pointer */
} ss_luopt_pb,*Ss_luopt_pb;

/* SS_Attr parameter sub-block	*/
typedef struct ss_attr_pb {
	u_int32		attr;				/* attributes						*/
} ss_attr_pb, *Ss_attr_pb;
	
/* SS_DevOpt parameter sub-block	*/
typedef struct ss_dopt_pb {
	u_int32
		dopt_size;					/* size of path options to copy	*/
	void
		*user_dopts;				/* device options buffer pointer */
} ss_dopt_pb,*Ss_dopt_pb;

/* SS_Rename parameter sub block 		*/
typedef struct ss_rename_pb {
	char
		*newname;					/* the new file name */
} ss_rename_pb, *Ss_rename_pb;

/* Ss_FillBuff parameter sub-block	*/
typedef struct ss_fillbuff_pb {
	u_int32
		size;						/* size of buffer passed			*/
	u_char
		*user_buff;					/* user buffer pointer				*/
} ss_fillbuff_pb, *Ss_fillbuff_pb;

/* Ss_link parameter sub-block 		*/
typedef struct ss_link_pb {
	u_char
		*link_path;					/* pathlist for the link			*/
} ss_link_pb, *Ss_link_pb;

/* Ss_cache parameter sub-block 	*/
typedef struct ss_cache_pb {
	u_int32	
		enblflag,					/* cache enable/disable flag 		*/
		drvcsize;					/* size of cache for drive in bytes */
} ss_cache_pb, *Ss_cache_pb;

/* Ss_skip_pb parameter sub-block 	*/
typedef struct ss_skip_pb {
	int32
		blks;							/* SIGNED number of blocks to skip */
} ss_skip_pb, *Ss_skip_pb;

/* Ss_erase_pb parameter sub-block 	*/
typedef struct ss_erase_pb {
	int32
		blks;							/* number of blocks to erase */
} ss_erase_pb, *Ss_erase_pb;

/* Ss_rfm_pb read file mark parameter block*/
typedef struct ss_rfm_pb {
	u_int32
		cnt;
} ss_rfm_pb, *Ss_rfm_pb;

/* Ss_wfm_pb write file mark parameter block */
typedef struct ss_wfm_pb {
	u_int32
		cnt;							/* number of marks to write */
} ss_wfm_pb, *Ss_wfm_pb;

/*--------------------------------------------------------------------------!
! getstat parameter sub-block structure defs								!
!--------------------------------------------------------------------------*/

/* GS_PathOpt parameter sub-block	*/
typedef struct gs_popt_pb {
	u_int32
		popt_size;					/* size of current options to copy */
	void
		*user_popts;				/* current path options buffer pointer */
} gs_popt_pb,*Gs_popt_pb;

/* GS_Ready parameter sub-block	*/
typedef struct gs_ready_pb {
	u_int32
		incount;					/* return input buffer count		*/
} gs_ready_pb,*Gs_ready_pb;

/* GS_DevOpt parameter sub-block	*/
typedef struct gs_dopt_pb {
	u_int32
		dopt_size;					/* size of device options to copy	*/
	void
		*user_dopts;				/* device path options buffer pointer */
} gs_dopt_pb,*Gs_dopt_pb;

/* GS_LUOPT parameter sub-block	*/
typedef struct gs_luopt_pb {
	u_int32
		luopt_size;					/* size of logical unit options to copy	*/
	void
		*user_luopts;				/* logical unit options buffer pointer	*/
} gs_luopt_pb,*Gs_luopt_pb;

/* GS_Size parameter sub-block	*/
typedef struct gs_size_pb {
	u_int32		filesize;			/* current file size (output)		*/
} gs_size_pb, *Gs_size_pb;

/* GS_Pos parameter sub-block	*/
typedef struct gs_pos_pb {
	u_int32		filepos;			/* current file position (output)	*/
} gs_pos_pb, *Gs_pos_pb;

/* GS_EOF parameter sub-block	*/
typedef struct gs_eof_pb {
	u_int32		eof;				/* eof == 1 if eof					*/
} gs_eof_pb, *Gs_eof_pb;

typedef struct gs_devname_pb {
	u_char		*namebuf;			/* the buffer for the device name	*/
} gs_devname_pb, *Gs_devname_pb;

typedef struct gs_devtype_pb {
	u_int16		type;				/* type */
	u_int16		class;				/* class (sequential or random) */
} gs_devtype_pb, *Gs_devtype_pb;

typedef struct gs_fd_pb {
	u_int32		info_size;			/* amount of fd info to copy 		*/
	Fd_stats	fd_info;			/* the buffer for the fd			*/
} gs_fd_pb, *Gs_fd_pb;

typedef struct gs_fdinf_pb {
	u_int32		info_size,			/* size of the info 				*/
				fd_blk_num;			/* block number of the fd 			*/
	Fd_stats	fd_info;			/* the buffer for the fd 			*/
} gs_fdinf_pb, *Gs_fdinf_pb;

typedef struct gs_showuse_pb {
	int16		mode,				/* file manager/driver mode flag	*/
				size;				/* user buffer size					*/
	u_int32		*user_buf;			/* user buffer pointer				*/
} gs_showuse_pb, *Gs_showuse_pb;

typedef struct gs_fdaddr_pb {
	u_int32		fd_blkaddr;			/* block address of fd				*/
} gs_fdaddr_pb, *Gs_fdaddr_pb;

/* GS_DSIZE parameter block */
typedef struct gs_dsize_pb {
	u_int32		totblocks,			/* total blocks on the disk 		*/
				blocksize;			/* size of block 					*/
} gs_dsize_pb, *Gs_dsize_pb;

/* GS_CSTATS parameter block */
typedef struct gs_cstats_pb {
	Cachestats	cache_inf;			/* the cache information structure 	*/
} gs_cstats_pb, *Gs_cstats_pb;

/* GS_PARITY parameter block */
typedef struct gs_parity_pb {
	Fd_stats	fd;					/* the fd buffer */
	u_int32		parity;				/* the resulting parity */
} gs_parity_pb, *Gs_parity_pb;

/* SS_EDT parameter block */
typedef struct gs_edt_pb {
	u_int32
		edition;
} gs_edt_pb, *Gs_edt_pb;

/* SS_COPYPD parameter block */
typedef struct gs_cpypd_pb {
	u_int32
		size;
	void
		*path_desc;
} gs_cpypd_pb, *Gs_cpypd_pb;

/* Generic Setstat parameter block for RTNFM setstat calls */
typedef struct ss_rtnfm_pb {
	u_int16 	ss_code;
	u_int16		rsvd;
	void		*param1;
	void		*param2;
} ss_rtnfm_pb, *Ss_rtnfm_pb;

/* Generic Getstat parameter block for RTNFM getstat calls */
typedef struct gs_rtnfm_pb {
	u_int16 	gs_code;
	u_int16		rsvd;
	void		*param1;
	void		*param2;
} gs_rtnfm_pb, *Gs_rtnfm_pb;

#endif
