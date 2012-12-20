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
 *  01 10/21/93 <----------- OS-9 NFS 1.3 Released --------------->    gkm
 *  02 04/06/95 Changed rpc_db structure.  It now includes definition
 *              for maximum entries that can be specified in rpcdb.
 *              Added if define control structures to selectively
 *              include and define structures for OS9/OS9000 or other
 *              OS. Added macro MAX_RPCDB_SPACE.  This is now generic
 *              for OS9/OS9000/Unix/Dos OSs.                           sr
 *  03 10/16/95 Added code for UNIX alignment of rpcdb structure       pjw
 *  04 01/19/98 New style rpc_db for SPF                               rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 *  05 05/19/98 Added #ifdef for multiple include protection           rds
 ****************************************************************************/

#ifndef _RPC_OS9RPCDB_H
#define _RPC_OS9RPCDB_H

#if defined(_OSK) || defined(_OS9000)
#ifndef _types
#include <types.h>
#endif

#ifndef ctime
#include <time.h>
#endif
#endif

#define HASHTABLE_SIZE 97
#define MAX_RPC_STATS 64
#define NFS_MAP "nfs_map"
#define NFSD_MAP "nfsd_map"

struct map_entry {
	u_int32 local;
	u_int32 remote;
};

struct map_entry_list {
	struct map_entry map;
	struct map_entry_list *next;
};

/*
** structure for the rpc database - we'll use a data module here
** so the whole thing can be rom-based
*/
struct rpc_db {
	char	recovery_dir[128];				/* backup/recovery directory */
	int		use_nfs_map;					/* 1 if client g/uid mapping */
	int		use_nfsd_map;					/* 1 if server g/uid mapping */
	int		collect_stats;					/* 1 if stats collected */
	u_int32	defuid;							/* default uid (client) */
	u_int32	defgid;							/* default gid (client) */
	short	defuid_d;						/* default uid (server) */
	short	defgid_d;						/* default gid (server) */
	int		rpcstat[MAX_RPC_STATS];			/* rpc statistics */
	int		nentries;						/* number of map_entry structures */
	int		num_cuid;						/* number of client uid mappings */
	int		num_cgid;						/* number of client gid mappings */
	int		num_suid;						/* number of server uid mappings */
	int		num_sgid;						/* number of server gid mappings */
	int		num_buckets;					/* size of hash table */
	int		reserved[4];					/* reserved space */
	/*
	** The map structure is declared to be only of size 1, but when the module
	** is created it will contain <nentries> map_entry structures.
	*/
	struct map_entry map[1];
};

typedef struct rpc_db rpc_db;

#define s_rpc_calls    0
#define s_rpc_badcalls 1
#define s_rpc_nullrecv 2
#define s_rpc_badlen   3
#define s_rpc_xdrcall  4

#define s_nfs_calls    5
#define s_nfs_badcalls 6
#define s_nfs_null     7
#define s_nfs_getattr  8
#define s_nfs_setattr  9
#define s_nfs_root     10
#define s_nfs_lookup   11
#define s_nfs_readlink 12
#define s_nfs_read     13
#define s_nfs_wrcache  14
#define s_nfs_write    15
#define s_nfs_create   16
#define s_nfs_remove   17
#define s_nfs_rename   18
#define s_nfs_link     19
#define s_nfs_symlink  20
#define s_nfs_mkdir    21
#define s_nfs_rmdir    22
#define s_nfs_readdir  23
#define s_nfs_fsstat   24

#define c_rpc_calls    30
#define c_rpc_badcalls 31
#define c_rpc_retrans  32
#define c_rpc_badxid   33
#define c_rpc_timeout  34
#define c_rpc_wait     35
#define c_rpc_newcred  36

#define c_nfs_calls    37
#define c_nfs_badcalls 38
#define c_nfs_nclget   39
#define c_nfs_nclsleep 40
#define c_nfs_null     41
#define c_nfs_getattr  42
#define c_nfs_setattr  43
#define c_nfs_root     44
#define c_nfs_lookup   45
#define c_nfs_readlink 46
#define c_nfs_read     47
#define c_nfs_wrcache  48
#define c_nfs_write    49
#define c_nfs_create   50
#define c_nfs_remove   51
#define c_nfs_rename   52
#define c_nfs_link     53
#define c_nfs_symlink  54
#define c_nfs_mkdir    55
#define c_nfs_rmdir    56
#define c_nfs_readdir  57
#define c_nfs_fsstat   58


#endif /* _RPC_OS9RPCDB_H */
