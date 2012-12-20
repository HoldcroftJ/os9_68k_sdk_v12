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

#include <sysboot.h>

/*----------------------------------------------------------!
! savememlist:	Take a snapshot of the memory list for easy	!
!				reconstruction after errors					!
!															!
! passed:	mlbuf - pointer to memory list snapshot buffer	!
! returns:	the number of entries in the memory list		!
!----------------------------------------------------------*/
u_int32 savememlist(mlbuf)
register u_int32	*mlbuf;
{
	register Dumb_mem	mlist;
	register u_int32	entries = 0;
	
	mlist = freememlist;				/* get the free memory list pointer	*/
	do {
		*mlbuf++ = (u_int32) mlist;		/* save the memory list entry ptr	*/
		*mlbuf++ = mlist->size;			/* save its size					*/
		entries++;						/* increment the number of entries	*/
	} while ((mlist = mlist->nxt) != 0); /* get the pointer to the next entry*/
										/* loop until it's the last one		*/
	*mlbuf++ = (u_int32) mlist;			/* mark the end of the list			*/
	return entries;
}
	

/*----------------------------------------------------------!
! restmemlist:	Restores the memory list from a snapshot	!
!				buffer created by "savememlist"				!
!															!
! passed:	mlbuf - pointer to memory list snapshot buffer	!
! returns:	nothing											!
!----------------------------------------------------------*/
restmemlist(mlbuf)
register u_int32	*mlbuf;
{
	register Dumb_mem	mlist;

	freememlist = (Dumb_mem) *mlbuf++;		/* restore the memlist pointer	*/
	mlist = freememlist;					/* get pointer to first entry	*/
	do {
		mlist->size = *mlbuf++;				/* restore its size field		*/
		mlist->nxt = (Dumb_mem) *mlbuf++;	/* insert ptr to next entry		*/
	} while ((mlist = mlist->nxt) != 0);	/* get ptr to next entry (if any)*/
}											/* loop until done				*/


/*----------------------------------------------------------------------!
! flipmemlist: invert the order of the blocks in the system memory list	!
!----------------------------------------------------------------------*/
flipmemlist()
{
	register Mlb_entry	mldown, mlup;
	register Dumb_mem	ml_pointer;
	register u_int32	entries, ml_size;
	mlb_entry			mlbuf[32];
	
	if ((entries = savememlist(mlbuf)) > 1) {
		mlup = &mlbuf[0];
		mldown = &mlbuf[entries - 1];
		while (mldown > mlup) {
			ml_pointer = mldown->pointer;
			ml_size = mldown->size;
			mldown->pointer = mlup->pointer;
			mldown->size = mlup->size;
			mlup->pointer = ml_pointer;
			mlup->size = ml_size;
			mlup++;
			--mldown;
		}
		/*--------------------------------------!
		! "restore" the inverted memory list	!
		!--------------------------------------*/
		restmemlist(mlbuf);
	}
}


/*----------------------------------------------------------------------!
! fixmemlist:	Sort the blocks in the system memory list according to	!
!				address with the lowest first, concantenate any adjacent!
!				blocks and then relink the memory list					!
!----------------------------------------------------------------------*/
void fixmemlist()
{
	register int16		entry;
	register Dumb_mem	mptr;
	register u_int32	*mlptr, *j;
	Dumb_mem			mlbuf[64];

	mptr = freememlist;
	entry = 1;
	/*--------------------------------------------------!
	! count the number of entries in the memory list	!
	! and put them into an array to be sorted			!
	!--------------------------------------------------*/
	mlptr = (u_int32 *) mlbuf;
	*mlptr++ = (u_int32) mptr;
	while ((*mlptr++ = (u_int32) (mptr = mptr->nxt)) != 0)
		entry++;

	/*----------------------------------------------!
	! if there is more than one entry, sort them	!
	! by address and merge any adjacent blocks		!
	!----------------------------------------------*/
	if (entry > 1) {
		sort_mem(mlbuf, entry);

		j = mlptr = (u_int32 *) mlbuf;
		j++;
		for (;;) {
			while ((*mlptr + ((Dumb_mem) *mlptr)->size) == *j) {
				((Dumb_mem) *mlptr)->size += ((Dumb_mem) *j)->size;
				j++;
			}
			if ((((Dumb_mem) *mlptr)->nxt = (Dumb_mem) *j) == 0)
				break;
			mlptr = j;
			j++;
		}
		freememlist = *mlbuf;
	}
}
	

void sort_mem(mems, count)
register u_int32		**mems;
register unsigned long	count;
{
	register long			j,
							i, 
							gap; 
	register u_int32		*temp;
	
	for(gap = count / 2; gap > 0; gap /= 2) {
		for(i = gap; i < count; i++) {
			for (j = i - gap; j >= 0; j -= gap) {
				if(mems[j] <= mems[j + gap])
					break;
				temp = mems[j];
				mems[j] = mems[j + gap];
				mems[j + gap] = temp;
			}
		}
	}
}
