#include <sysboot.h>

/*
 * Copyright 1990 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

/*------------------------------!
! extract: get memory from rom	!
!------------------------------*/
error_code extract(size, mem)
u_int32			*size;				/* amount of memory to obtain */
u_char			**mem;				/* the memory pointer */
{
	register Dumb_mem	memptr,				/* pointer to the list */
						lastmem = NULL;		/* previous block pointer */
	
	*size = (*size + MINALLOC - 1) & ~(MINALLOC - 1);	/* round up */
	memptr = freememlist;					/* get the memory list addr */
	while ((memptr != NULL) && (memptr->size < *size)) {
		lastmem = memptr;				/* remember previous */
		memptr = memptr->nxt;			/* move down the list */
	}
	if (memptr == NULL)
		return E_MEMFUL;				/* couldn't find enough memory */
		
	*mem = (u_char *) memptr;
	if ((memptr->size -= *size) == 0) {
		if (lastmem != NULL)
			lastmem->nxt = memptr->nxt;	/* point last block to next block 	*/
		else
			freememlist = memptr->nxt;	/* point start of list to next block*/
		
	} else {
		
		/*
		 * since there is still some memory left in this 
		 * block we need to rebuild the block header at
		 * the beginning of the residue.
		 */
		 
		if (lastmem != NULL) {
			lastmem->nxt = (Dumb_mem) ((u_char *) memptr + *size);
			lastmem->nxt->nxt = memptr->nxt;
			lastmem->nxt->size = memptr->size;
		} else {
			freememlist = (Dumb_mem) ((u_char *) memptr + *size);
			freememlist->nxt = memptr->nxt;
			freememlist->size = memptr->size;
		}
	}
	return SUCCESS;
}
