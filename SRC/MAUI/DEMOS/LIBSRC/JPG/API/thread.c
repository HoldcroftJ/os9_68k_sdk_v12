/*******************************************************************************
* FILENAME : thread.c
*
* DESCRIPTION :
*
*	JPEG LIB Thread support
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  05/08/00  SRM  Initial version cloned from CLIB
*/

#ifdef _OS9THREAD

#include "jpgcom.h"
#include <stdlib.h>
#include <cglob.h>

/* Lock a resource via a semaphore. The susp_type field specifies if
   the thread can be suspended while locked. */
static void _jpg_lib_lock(semaphore *sem, susp_type type)
{
	/* if requested, first mark thread as unsuspendable */
	if (type == UNSUSPEND)
		_pthread_setunsuspendable();

	/* keep trying until it works (signals abort, but we still want to try) */
	while (_os_sema_p(sem))
		;
}

/* Unlock a resource via a semaphore. The susp_type field specifies if
   the thread can be suspended while locked. */
static void _jpg_lib_unlock(semaphore *sem, susp_type type)
{
	/* release the lock */
	_os_sema_v(sem);

	/* set suspendable if lock set unsuspendable */
	if (type == UNSUSPEND)
		_pthread_setsuspendable();
}

/* The monitor calls extend the _jpg_lib_lock() and
   _jpg_lib_unlock() calls. The susp_type field specifies if the
   thread can be suspended while locked. A monitor is an advanced
   semaphore that maintains owner and link information. A monitor will
   allow an "owner" to do multiple locks and unlocks while blocking
   "non-owners". */
void _jpg_monitor_lock(monitor *m, susp_type type)
{
	if (m->owner == _procid) {
		++m->link;
		return;
	}

	_jpg_lib_lock(&m->sem, type);
		
	m->owner = _procid;
	m->link = 1;
}

void _jpg_monitor_unlock(monitor *m, susp_type type)
{
	while (m->owner != _procid || m->link == 0) {		/* spin forever waiting for someone to investigate */
	}

	if (--m->link == 0) {
		m->owner = 0;
		_jpg_lib_unlock(&m->sem, type);
	}
}

#else
extern int never_defined;
#endif /* _OS9THREAD */
