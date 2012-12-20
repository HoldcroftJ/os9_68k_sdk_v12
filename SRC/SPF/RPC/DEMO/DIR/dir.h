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
 *  01 10/21/93 <--------- OS-9 NFS 1.3 Released ------------------->            
 *     04/22/96 <--------- NFS 1.4 Server Release ------------------>
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

#define MAXNAMELEN 255

typedef char *nametype;
bool_t xdr_nametype();


typedef struct namenode *namelist;
bool_t xdr_namelist();


struct namenode {
	nametype name;
	namelist next;
};
typedef struct namenode namenode;
bool_t xdr_namenode();


struct readdir_res {
	int errno;
	union {
		namelist list;
	} readdir_res_u;
};
typedef struct readdir_res readdir_res;
bool_t xdr_readdir_res();


#define DIRPROG ((u_long)76)
#define DIRVERS ((u_long)1)
#define READDIR ((u_long)1)
extern readdir_res *readdir_1();

