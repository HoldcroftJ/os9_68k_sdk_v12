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
 *  01 10/21/93 removed current directory from include search list in  gkm
 *              makefile differentiate between #include <dir.h> and
 *              #include "dir.h".
 *              <--------- OS-9 NFS 1.3 Released ------------------->            
 *  02 03/03/95 Typecast the param of xdr_free to match prototype      sr
 *     04/22/96 <--------- NFS 1.4 Server Release ------------------>
 *  13 12/09/97 Removed compiler warnings                              rds
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

_asm("_sysedit: equ 13");

#include <RPC/rpc.h>
#include <dir.h>
#include "dir.h"

extern int errno;
extern char *malloc();
extern char *strcpy();

readdir_res *
readdir_1(dirname)
	nametype *dirname;
{
	DIR *dirp;
	struct direct *d;
	namelist nl;
	namelist *nlp;
	static readdir_res res; /* must be static! */
	
	/*
	 * Open directory
	 */
	dirp = opendir(*dirname);
	if (dirp == NULL) {
		res.errno = errno;
		return (&res);
	}

	/*
	 * Free previous result
	 */
	xdr_free(xdr_readdir_res,(char *)&res);

	/*
	 * Collect directory entries
	 */
	nlp = &res.readdir_res_u.list;
	while (d = readdir(dirp)) {
		nl = *nlp = (namenode *) malloc(sizeof(namenode));
		nl->name = malloc(strlen(d->d_name)+1);
		strcpy(nl->name, d->d_name);
		nlp = &nl->next;
	}
	*nlp = NULL;

	/*
	 * Return the result
	 */
	res.errno = 0;
	closedir(dirp);
	return (&res);
}
