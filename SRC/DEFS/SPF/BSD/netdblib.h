/****************************************************************************
 *  ID:		%A%
 *  Date:	%G%
 ****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   0 ??/??/?? Birth                                                  ???
 *     09/16/93 <-------- ISP 2.0 Release -------------------------->
 *   1 05/03/94 Added inetd.conf support                               bat
 *   2 07/22/94 Added DNS support (RESOLV_CONF/HOST_CONF)              bat
 *     01/17/96 <-------- ISP 2.1 Release -------------------------->
 *   3 12/18/96 Changed to support MAX_MODS dynamic modules            tmd
 *     02/19/97 <-------- SPF LAN Comm Pack v3.0 Release ----------->
 *     05/07/97 <-------- SPF LAN Comm Pack v3.1 Release ----------->
 *   4 06/27/97 Added C++ support                                      apw
 *   5 10/08/97 Copied from SRC/DEFS/INET to SRC/DEFS/SPF/BSD          pjw
 *     01/14/98 Removed maxhostnamelen and included <sys/param.h>      rds
 *   6 01/22/98 Added RPC_FILE and DATA_SIZE_NEW macro                 tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#ifndef _NETDBLIB_H
#define _NETDBLIB_H

#if (defined(_OSK) || defined(_OS9000))
#include <types.h>
#include <module.h>
#include <sys/param.h>
#endif

#define MAXALIASES              35
#define IMAXSTRLEN              128
#define IMAXARGLEN              128

#if !defined(MODSIZE)
#ifdef _OSK
#define MODSIZE(mod) ((u_int32)((Mh_com)mod)->_msize)
#else
#define MODSIZE(mod) ((u_int32)((Mh_com)mod)->m_size)
#endif
#endif /* !defined(MODSIZE) */

#if !defined(MODACC)
#ifdef _OSK
#define MODACC(mod) ((u_int16)((Mh_com)mod)->_maccess)
#else
#define MODACC(mod) ((u_int16)((Mh_com)mod)->m_access)
#endif
#endif /* !defined(MODACC) */

#if !defined(MODOWN)
#ifdef _OSK
#define MODOWN(mod) ((u_int16)((Mh_com)mod)->_mowner)
#else
#define MODOWN(mod) ((u_int16)((Mh_com)mod)->m_group_user)
#endif
#endif /* !defined(MODOWN) */

#if !defined(MODGRP)
#ifdef _OSK
#define MODGRP(mod) ((u_int16)(((Mh_com)mod)->_mowner>>16))
#else
#define MODGRP(mod) ((u_int16)((Mh_com)mod)->m_group)
#endif
#endif /* !defined(MODGRP) */

#define MAX_MODS           4   /* max number of modules to search */

#define INETDB_MOD_NAME "inetdb"

#define	COMPAT			0x01
#define SYSREV			0x0001

#define VERSION_ID		9

#define ALIGN                   4  /* FIX make idbgen use this */
#define CRCSIZE                 3  /* FIX get from module header? */

#define HOSTS			0
#define HOSTS_EQUIV		1
#define NETWORKS		2
#define PROTOCOLS		3
#define SERVICES		4
#define INETD_CONF		5
#define RESOLV_CONF		6
#define HOST_CONF		7
#define OLD_STYLE		7 /* marks last of old style "files" */
#define NUMFILES		8

/* "file" numbers > OLD_STYLE automatically get an EMPTY record indicator.
   We don't do this for "files" <= OLD_STYLE to support old inetdb mods. */
#define NEW_STYLE		8 /* indicate start of new style "files" */
#define HOST_INTS		8 /* host interfaces "file" */
#define HOSTNAME		9 /* name of host */
#define ROUTES         10 /* routes used by IP at startup*/
#define RPC_FILE       11 /* RCP (/etc/rpc) */

/* WARNING... netdb uses an int32 to store flags to determine if a "file"
   has been opened (see netdb_link).  This must be changed if netdb is to
   use "files" > 32. */

/* All future entries should have as their first field a flag of type
   EMPTY_TYPE indicating whether the entry is empty or not.  Making
   it 4 bytes allows for "good" alignment. */
#define EMPTY_TYPE              u_int32
#define EMPTY_VALUE             (EMPTY_TYPE) 0

/* type used at head of "file" to indicate how many entries */
#define CNTTYPE			u_int16

/* type used at head of record to indicate its length */
#define LENTYPE			u_int16

/* size of smallest record.  Must include space for LENTYPE and space for
   the largest empty_record indicator (currently 4 bytes), and must be a
   multiple of ALIGN */
#define MIN_REC_SIZE           ((sizeof(LENTYPE)+4+ALIGN-1)&(~(ALIGN-1)))

#define IGNORE_CHAR(c)		((c=='#')||(c==';')||(c=='*'))

#define ECOUNTSIZE		sizeof(u_int16)

#undef TRUE
#define TRUE			(1)
#undef FALSE
#define FALSE			(0)
#undef NULL
#define NULL                    (0)


/* flag to indicated we should ignore entry count. used for getting records
   from modules where all the records are empty */

#define IGNORE_ENTRY_COUNT      (1)

#define FIRST_ENTRY(globs,type) \
	(void*)(((int)globs[type].table) + sizeof(WORD))



struct fileentry {
	u_char			*entries;
	u_int32			num;
};

/* don't add anything AFTER _offtab!  _offtab can grow > NUMFILES */
typedef struct {
	u_int32			_id;
	u_int32			_offtab[NUMFILES];
} ndbmod;

typedef struct al {
	char			*name;
	struct al		*next;
} aliaslist;

struct netglobs_old {
	u_int32			offset;
	u_char			*table;
	u_int16			entcnt;
};

struct netglobs {
	int            mod_index;          /* Which module we are using */
	char           *dataptr[MAX_MODS]; /* pointer to start of entry */
	char           *endptr[MAX_MODS];   /* pointer to end of entry */
	char           *curpos;            /* pointer to current position */
	u_int16        curcnt;             /* current record count */
};

#if (defined(_OSK) || defined(_OS9000))
struct metaglobs {
	int    mod_count;            /* How many modules we've linked */
	int    file_count;           /* How many "files" for each module */
	mh_com *module[MAX_MODS];    /* pointer to linked module */
	u_int16 can_write[MAX_MODS]; /* flag saying if we can write to mod */
	struct netglobs *glob_ptr;   /* pointer to array of netglobs */
};
#endif

/* HOWMANY_MODS(struct metaglobs *Mgp) */
#define HOWMANY_MODS(Mgp) (int) ((Mgp)->mod_count)

/* HOWMANY_FILES(struct metaglobs *Mgp) */
#define HOWMANY_FILES(Mgp) (int) ((Mgp)->file_count)

/* HOWMANY_ENTRIES(struct metaglobs *Mgp, int mod, int type) */

#define HOWMANY_ENTRIES(Mgp, mod, type) \
        (* (CNTTYPE *)(Mgp)->glob_ptr[type].dataptr[mod])

/* HOWMANY_ENTRIES2(struct netglobs *Ngp, int mod) */

#define HOWMANY_ENTRIES2(Ngp, mod) \
        (* (CNTTYPE *)(Ngp)->dataptr[mod])

/* macro to get pointer to EMPTY indicator given pointer to length */
/* EMPTYPTR(void *currec) - currec points at first byte of record */
#define EMPTYPTR(currec) (((LENTYPE *) (currec)) + 1)

/* macro to get total size of record (data & len) given pointer to length */
/* REC_SIZE(void *currec)  - currec points at length (e.g. ngp->curpos) */
#define REC_SIZE(currec) (*((LENTYPE *)(currec)))

/* macro to get size of data in record given pointer to length */
/* REC_DATA_SIZE(void *currec)  - currec points at length (e.g. ngp->curpos) */
#define REC_DATA_SIZE(currec) ((*((LENTYPE *)(currec))) - sizeof(LENTYPE))

/* macro to get size of just data given pointer to data */
/* DATA_SIZE(void *rec_ptr) - rec_ptr points at first byte of data */
#define DATA_SIZE(rec_ptr) ((*(((LENTYPE *) (rec_ptr)) - 1)) - sizeof(LENTYPE))

/* macro to get size of just data given pointer to data with empty indicator  */
#define DATA_SIZE_NEW(rec_ptr) (* ((LENTYPE *) ( ((char *)(rec_ptr)) - sizeof(LENTYPE) - sizeof(EMPTY_TYPE) ) ) - sizeof(LENTYPE) - sizeof(EMPTY_TYPE))

/* macro to find the beginning of the record given a pointer to the data */
/* REC_BEGIN(void *rec_ptr, type)  - rec_ptr points at first byte of data */
#define REC_BEGIN(rec_ptr, type) ((type) > OLD_STYLE) ?  \
              (((char *) (rec_ptr)) - sizeof(LENTYPE) - sizeof(EMPTY_TYPE)) : \
              (((char *) (rec_ptr)) - sizeof(LENTYPE))

#if (defined(_OSK) || defined(_OS9000))

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
int        ndb_is_empty(void *rec, int type);
void       ndb_make_empty(void *curpos, int type, int zero_record);
error_code ndb_rewind(struct metaglobs *mgp, int type);
error_code ndb_getnext(struct metaglobs *mgp, void **recpp, int type);
error_code ndb_defrag(struct metaglobs *mgp, int type);
error_code ndb_add_rec(struct metaglobs *mgp, int type, void *rec, int rec_len);
error_code ndb_del_cur_rec(struct metaglobs *mgp, int type);
error_code ndb_del_rec(struct metaglobs *mgp, void *del_rec, int type);
error_code ndb_create_ndbmod(char *modname, int num_files, int *file_sizes,
                             u_int32 perm, u_int16 rev);
error_code ndb_link_ndbmod(struct metaglobs **mgpp);
error_code ndb_unlink_ndbmod(struct metaglobs *mgp);
error_code _nextentry(struct metaglobs *mgp, void **recpp, int type,
                      int ignore_count);
#if defined(__cplusplus)
}
#endif /*__cplusplus */
#else
int        ndb_is_empty();
void       ndb_make_empty();
error_code ndb_rewind();
error_code ndb_getnext();
error_code ndb_defrag();
error_code ndb_add_rec();
error_code ndb_del_cur_rec();
error_code ndb_del_rec();
error_code ndb_create_ndbmod();
error_code ndb_link_ndbmod();
error_code ndb_unlink_ndbmod();
error_code _nextentry();
#endif

#endif

#endif /* _NETDBLIB_H */
