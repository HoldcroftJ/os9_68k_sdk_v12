/* net.h */

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
*	edition history
*	ed	date		reason											by
* 	--	--------	-----------------------------------------------	---
*	01	86/03/15	devided into header file						ken
*	02	86/05/14	added date/time in NET_nodes module table entry	ken
*	03	86/06/16	6809 compatibility adjustment					ken
*	04	87/06/05	nmon -n/-s options new							ken
*	05	87/06/16	NET_users module definition						ken
*	06	87/07/03	name _ndnfmed for used-to-be reserved byte		ken
*	--- use NFM edition # for which "net.h" is edited ---
*	028	87/11/11	_gs/_ss_ library code temporarily in net.h		ken
*		87/10/30	adjusted NET_nodes entry definitions			ken
*		87/11/11	added _nd_why - last UP/DOWN reasons			ken
*	029 87/11/16	added _nd_cdate/_ctime for last _nd_why change	ken
*	031	88/01/04	added/adjusted date/time fields					ken
*		88/01/18	added _nd/_li/_lo/_qo/_qi/date/time, in julian	ken
*	036	88/04/04	added _nd_mxcp for max logical message capacity	ken
*	037	88/04/08	added _nd_prvpak for packet integrity check		ken
*	040	88/05/02	added EDITION: nfm edition#						ken
*	042	88/09/08	added _nd_.. driver's portion					ken
*	043	88/09/23	updated edition # following new "sys.l"			ken
*	044	89/04/06	updated edition # for nfm 44.					wwb
*	045	89/07/18	updated edition # for nfm 45.					wwb
*	046	90/02/28	updated edition # for nfm 46.					wwb
*   047 90/07/26    updated edition # for nfm 47.                   wwb
*	048	90/08/27	fixed problem with station id (long->short).	wwb
*					(thanks kenm).
*                    ---- OS-9/68K V2.4 Release ----
*   049 02/08/18    Edition bumped for changes in NFM eta'al        Rwb
*                   COMPEDIT added,  utils will test against this
*                   for >=.  Utils will not have to track NFM in
*                   in the future.  
*	050 09/28/94	fixed options buffer size problem, updated		wwb
*					edition # for release and converted to
*					UCC compatible asm language.
*                    ---- OS-9/68K V3.0 Release ----
*	051	03/27/96	removed _sysedit definition.					wwb
*	052	98/07/29	Updated for new copyright info.					wwb
*                    ---- OS-9/68K V3.1 Release ----
*				$$			<RELEASE_INFO>			$$						|
*---------------------------------------------------------------------- */

/*
 *  The following definitions are for the utilities Edition number and
 *  that of NFM.  In the past,  the NFM and utility editions were 
 *  forced to track 1:1.  This is excessive.  In the future,  the only
 *  time the utilities should change is when the data structures are
 *  modified or bugs repaired.  the definition COMPEDIT is provided as
 *  a compatability flag.  This COMPEDIT should be changed IF there
 *  is a change to the global structures.
 */

#define		EDITION		50		/* current edition          */

#define     COMPEDIT    48		/* minimum for compatabilty */

/* nfm, path/device descriptor, _nodes and _users module constants	*/
#define	DT_NFM		4		/* still missing value in common defs	*/

#define	NODENAMEL	31		/* max station/node name length			*/
#define	DEVNAMEL	31		/* max nfm device name length			*/

#define	NSTATIONS	100		/* max # of entries on _nodes			*/
#define	MAXNUSERS	100		/* max # of users in _users				*/

#define	N_ATTR		0x8000	/* data module attribute				*/
#define	N_PERM		0x0333	/* data module permission				*/
#define	UNKNOWN		0		/* _nodes station id: 0 for reserved	*/

#define	N_REQPATHS	1		/* _gs_npaths() get # of request paths	*/
#define	N_SRVPATHS	2		/* _gs_npaths() get # of serving paths	*/

#define	N_PLOCK		-1		/* _ss_nusrmdl() lock   _users module	*/
#define	N_PUNLOCK	0		/* _ss_nusrmdl() unlock _users module	*/

/*	additional info for memory module entry. 68K needs <module.h>	*/
typedef	struct
	{	struct	modhcom		_mh;	/*  common header info			*/
		int					_mexec;	/*  offset 68K LONG, 09 WORD	*/
	}	mod_dat;

/* --------------------------------------------------------------------
/*	entry on node name/id table memory module
/*	- date/time field are all LONG. note date/time carried on the
/*	  messages are WORDs.										ed. 31	*/

typedef	struct								/* note short:WORD 16 bit	*/
	{			 int	_nd_name;			/* offset to the node name	*/
		unsigned short	_nd_id;				/* node ID					*/
		unsigned char	_nd_actv,			/* BOOLEAN: node active		*/
						_nd_nfmed;			/* nfm edition # if known	*/
		unsigned short	_nd_mxcp;			/* max message size for node*/
		unsigned short	_nd_prvpak;			/* nfm ed #37 integirty chk	*/
		unsigned int	_nd_udate,_nd_utime;/* his UP/DOWN date/time	*/
		unsigned int	_nd_cdate,_nd_ctime;/* last _nd_why change time	*/
		unsigned char	_nd_why;			/* last UP/DOWN reason		*/
				 char	_nd_rsv1;			/* align					*/
		unsigned int	_nd_lidate,_nd_litime;	/* last input in julian	*/
		unsigned int	_nd_lodate,_nd_lotime;	/* last output			*/
		unsigned int	_nd_qidate,_nd_qitime;	/* last his INQ confirmed*/
		unsigned int	_nd_qodate,_nd_qotime;	/* last our INQ confirmed*/
		unsigned short	fill[1];			/* ( align for long )		*/
		unsigned int	_nd_drv[4];			/* driver's portion			*/
	}	one_nod;

/* --------------------------------------------	*/
/* _nodes module _nd_why values					*/

#define		_N_BCUP		1		/* input or our output UP B/C	*/
#define		_N_BCCONF	2		/* our UP B/C confirmed			*/
#define		_N_BCDOWN	3		/* input DOWN B/C				*/
#define		_N_INQIN	4		/* input INQ					*/
#define		_N_INQCONF	5		/* INQ-response seq complete	*/
#define		_N_INQDOWN	6		/* INQ time-out					*/

/* --------------------------------------------	*/
/* entry on _users module						*/

#define	U_ENTS		6		/* # of fields in one_usr 	*/
#define	U_RNODE		0		/* one_usr[subscript]		*/
#define	U_RGRP		1
#define	U_RUSR		2
#define	U_SVGRP		3
#define	U_SVUSR		4
#define	U_SVPERM	5

typedef struct
	{	unsigned short	_u_rnode;	/* requester node id		*/
		unsigned short	_u_rgroup;	/* requester group id		*/
		unsigned short	_u_ruser;	/* requester user id		*/
		unsigned short	_u_svgroup;	/* group id for the user	*/
		unsigned short	_u_svuser;	/* user id					*/
		unsigned short	_u_svperm;	/* access permision for him	*/
				 short	_u_rsv1,_u_rsv2;
	} one_usr;

/* values on each entry	*/
#define	U_ANY	(0xffff)	/* "any" node id/group/user			*/
#define	U_DFT	(0xffff)	/* "defaults" to orignal group/user	*/
#define	U_NODE	(0xfffe)	/* use the requester's node id		*/

/* ------------------------------------------------	*/
/*	PD_OPT field definition							*/
struct	nsgbuf
	{	unsigned char
				sg_class,		/*	DT_NFM							*/
				sg_mnet,		/*	BOOLEAN: multi-station network	*/
				sg_bc,			/*	BOOLEAN: hardware broadcast available */
				sg_fill;
		unsigned short
				sg_dest,		/*	station id of THIS path			*/
				sg_src,			/*	our station id					*/
				sg_broad,		/*	driver's broadcasting code		*/
				sg_rtyn,		/*	# of retries for nfm			*/
				sg_rtyd,		/*	# of retries for driver			*/
				sg_mulv;		/*	retry multiplyer				*/
		void	*sg_hwb;		/*	hardware message buff address	*/
		char	sg_null[108];
	};

/* end of file */
