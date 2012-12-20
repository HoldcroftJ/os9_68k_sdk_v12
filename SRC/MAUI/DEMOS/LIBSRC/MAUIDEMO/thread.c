/*******************************************************************************
* FILENAME : thread.c
*
* DESCRIPTION :
*
*	MAUI Thread support
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  05/05/00  SRM  Initial version
*/

#ifdef _OS9THREAD

#include <MAUI/mauidemo.h>
#include <MAUI/maui_mem.h>
#include <stdlib.h>
#include <cglob.h>

pthread_once_t _mauidemo_gdata_init = PTHREAD_ONCE_INIT;
pthread_key_t _mauidemo_gdata_data_key;

typedef struct {
	void *tdata;					/* thread specific data pointer		*/
	void (*dest)(void *);			/* destructor						*/
} psect;

/* Items that need to be seen as one instance per thread				*/
typedef struct {
	u_int32 dest_count;				/* number of calls to destructor	*/
	psect vector[DEMO_NUM_THREAD_SPECIFIC_DATAS]; /* vector of datas	*/
} thread_data;

static void free_datas(void *_vector)
{
	thread_data *tdata = (thread_data *)_vector;
	psect *v;
	int i;

	/* until we're called the last time, reinstall ourselves so we stay around
	   for as long as possible so people can call library functions from their
       destructors */
	if (++tdata->dest_count < PTHREAD_DESTRUCTOR_ITERATIONS) {
		/* put us back */
		pthread_setspecific(_mauidemo_gdata_data_key, tdata);
	} else {
		/* destroy all the datas and the vector itself */
		v = &tdata->vector[0];
		for (i = 0; i < DEMO_NUM_THREAD_SPECIFIC_DATAS; ++i) {
			if (v->tdata) {
				/* call destructor if specified */
				if (v->dest)
					v->dest(v->tdata);
				free(v->tdata);
			}
			++v;
		}
        free(v->tdata);
	}
}

static void create_key(void)
{
	pthread_key_create(&_mauidemo_gdata_data_key, free_datas);
}

void *demo_getdata(u_int32 ndx, size_t size, void (*initdata)(void *), void (*termdata)(void *))
{
	thread_data *tdata;
	void *mem;

	/* make sure key exists for threads to use */
	if (pthread_once(&_mauidemo_gdata_init, create_key) != 0) {
		return NULL;
	}

	/* make sure vector exists for this thread */
	if ((tdata = pthread_getspecific(_mauidemo_gdata_data_key)) == NULL) {
		if ((tdata = calloc(1, sizeof(thread_data))) == NULL) {
			return NULL;
		}
		if (pthread_setspecific(_mauidemo_gdata_data_key, tdata) != 0) {
			return NULL;
		}
	}

	/* make sure memory exists for psect */
	if ((mem = tdata->vector[ndx].tdata) == NULL) {
		if ((mem = calloc(1, size)) == NULL)
			return NULL;
		if (initdata)
			initdata(mem);
		tdata->vector[ndx].tdata = mem;
		tdata->vector[ndx].dest = termdata;
	}

	return mem;
}

/* Lock a resource via a semaphore. The demo_susp_type field specifies if
   the thread can be suspended while locked. */
void demo_lib_lock(semaphore *sem, demo_susp_type type)
{
	/* if requested, first mark thread as unsuspendable */
	if (type == DEMO_UNSUSPEND)
		_pthread_setunsuspendable();

	/* keep trying until it works (signals abort, but we still want to try) */
	while (_os_sema_p(sem))
		;
}

/* Unlock a resource via a semaphore. The demo_susp_type field specifies if
   the thread can be suspended while locked. */
void demo_lib_unlock(semaphore *sem, demo_susp_type type)
{
	/* release the lock */
	_os_sema_v(sem);

	/* set suspendable if lock set unsuspendable */
	if (type == DEMO_UNSUSPEND)
		_pthread_setsuspendable();
}

/* The monitor calls extend the demo_lib_lock() and
   demo_lib_unlock() calls. The demo_susp_type field specifies if the
   thread can be suspended while locked. A monitor is an advanced
   semaphore that maintains owner and link information. A monitor will
   allow an "owner" to do multiple locks and unlocks while blocking
   "non-owners". */
void demo_monitor_lock(demo_monitor *m, demo_susp_type type)
{
	if (m->owner == _procid) {
		++m->link;
		return;
	}

	demo_lib_lock(&m->sem, type);
		
	m->owner = _procid;
	m->link = 1;
}

void demo_monitor_unlock(demo_monitor *m, demo_susp_type type)
{
	while (m->owner != _procid || m->link == 0) {		/* spin forever waiting for someone to investigate */
	}

	if (--m->link == 0) {
		m->owner = 0;
		demo_lib_unlock(&m->sem, type);
	}
}

#else
extern int never_defined;
#endif /* _OS9THREAD */
