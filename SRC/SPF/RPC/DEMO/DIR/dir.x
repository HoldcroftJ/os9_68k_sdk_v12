/****************************************************************************
 *  ID:		@(#) dir.x 1.3@(#)
 *  Date:	4/5/98
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

const MAXNAMELEN = 255;		/* maximum length of a directory entry */

typedef string nametype<MAXNAMELEN>;	/* a directory entry */

typedef struct namenode *namelist;	/* a link in the listing */

/*
 * A node in the directory listing
 */
struct namenode {
	nametype name;		/* name of directory entry */
	namelist next;		/* next entry */
};

/*
 * The result of a READDIR operation.
 */
union readdir_res switch (int errno) {
case 0:
	namelist list;	/* no error: return directory listing */
default:
	void;		/* error occurred: nothing else to return */
};

/*
 * The directory program definition
 */
program DIRPROG {
	version DIRVERS {
		readdir_res
		READDIR(nametype) = 1;
	} = 1;
} = 76;
