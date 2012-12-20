/*
 * jmemnobs.c
 *
 * Copyright (C) 1992-1994, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file provides a really simple implementation of the system-
 * dependent portion of the JPEG memory manager.  This implementation
 * assumes that no backing-store files are needed: all required space
 * can be obtained from malloc().
 * This is very portable in the sense that it'll compile on almost anything,
 * but you'd better have lots of main memory (or virtual memory) if you want
 * to process big images.
 * Note that the max_memory_to_use option is ignored by this implementation.
 */

#define JPEG_INTERNALS

#include <MAUI/maui_mem.h>

#ifdef MEM_SHOW
#include <stdlib.h>
#endif
#include "jinclude.h"
#include "jpeglib.h"
#include "jmemsys.h"		/* import the system-dependent declarations */

#ifndef HAVE_STDLIB_H		/* <stdlib.h> should declare malloc(),free() */
extern void * malloc JPP((size_t size));
extern void free JPP((void *ptr));
#endif

/*#ifndef OS_OS9000*/
#ifndef _OS9000
#define JPEG_SHADE 3
#else
#define JPEG_SHADE MEM_DEF_SHADE
#endif

/*
 * Memory allocation and freeing are controlled by the regular library
 * routines malloc() and free().
 */

GLOBAL void *
jpeg_get_small (j_common_ptr cinfo, size_t sizeofobject)
{
	error_code error;
	void * tmp;

	error=mem_malloc(&tmp,JPEG_SHADE,sizeofobject);
	if(error != 0)
		tmp=NULL;
#ifdef MEM_SHOW	
    system("echo \"Allocating\";procs -e");
#endif	
	return tmp;
}

GLOBAL void
jpeg_free_small (j_common_ptr cinfo, void * object, size_t sizeofobject)
{
	mem_free(object);
#ifdef MEM_SHOW	
    system("echo \"De-allocating\";procs -e");
#endif
}


/*
 * "Large" objects are treated the same as "small" ones.
 * NB: although we include FAR keywords in the routine declarations,
 * this file won't actually work in 80x86 small/medium model; at least,
 * you probably won't be able to process useful-size images in only 64KB.
 */

GLOBAL void FAR *
jpeg_get_large (j_common_ptr cinfo, size_t sizeofobject)
{
	error_code error;
	void * tmp;

	error=mem_malloc(&tmp,JPEG_SHADE,sizeofobject);
	if(error != 0)
		tmp=NULL;
#ifdef MEM_SHOW	
    system("echo \"Allocating\";procs -e");
#endif
	return tmp;
}

GLOBAL void
jpeg_free_large (j_common_ptr cinfo, void FAR * object, size_t sizeofobject)
{
	mem_free(object);
#ifdef MEM_SHOW	
    system("echo \"De-allocating\";procs -e");
#endif

}


/*
 * This routine computes the total memory space available for allocation.
 * Here we always say, "we got all you want bud!"
 */

GLOBAL long
jpeg_mem_available (j_common_ptr cinfo, long min_bytes_needed,
		    long max_bytes_needed, long already_allocated)
{
  return max_bytes_needed;
}


/*
 * Backing store (temporary file) management.
 * Since jpeg_mem_available always promised the moon,
 * this should never be called and we can just error out.
 */

GLOBAL void
jpeg_open_backing_store (j_common_ptr cinfo, backing_store_ptr info,
			 long total_bytes_needed)
{
  ERREXIT(cinfo, JERR_NO_BACKING_STORE);
}


/*
 * These routines take care of any system-dependent initialization and
 * cleanup required.  Here, there isn't any.
 */

GLOBAL long
jpeg_mem_init (j_common_ptr cinfo)
{
	error_code ec;
	
	if((ec=mem_init())!= SUCCESS)
		return ec;
	/*#ifndef OS_OS9000*/
#ifndef _OS9000
	if((ec=mem_create_shade(JPEG_SHADE, MEM_SHADE_NORMAL, 0, 0, 1024, 
							MEM_OV_ATTACHED,OFF))!=SUCCESS)
		return ec;
#endif`
	
	return 0;
}

GLOBAL void
jpeg_mem_term (j_common_ptr cinfo)
{
  /*#ifndef OS_OS9000*/
#ifndef _OS9000
	mem_destroy_shade(JPEG_SHADE);
#endif
	mem_term();
}
