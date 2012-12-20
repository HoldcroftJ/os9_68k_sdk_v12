/*******************************************************************************
* DESCRIPTION :
*
*	This file contains the memory allocation functions. These functions are
*	used to provide support for drivers that need to allocate graphics memory
*	for processes to use. This happens if the memory is pseudo memory, or if
*	the memory cannot (or should not) be put in the systems memory list.
*
* COPYRIGHT:
*
*	This source code is the proprietary confidential property of Microware
*	Systems Corporation, and is provided to licensee solely for documentation
*	and educational purposes. Reproduction, publication, or distribution in
*	form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/12/95  TJE  First version
*/
#include <gdv_priv.h>
#include <memory.h>
#include <modes.h>

static error_code alloc_color(void *start_ptr, size_t *size, void **mem_ptr,
	u_int32 color);
static error_code dealloc_color(void *start_ptr, size_t size, void *mem_ptr,
	u_int32 color);
static error_code alloc_shade(void *notused, size_t *size, void **mem_ptr,
	u_int32 color);
static error_code dealloc_shade(void *notused, size_t size, void *mem_ptr,
	u_int32 color);

/*******************************************************************************
* NAME:		gdv_create_mem_color()
*
* USAGE:	Create a color of memory. This function should be called once to
*			create each color of memory. The call to this function should be
*			done within the code for GDV_INIT_HW().
*/
error_code gdv_create_mem_color(u_int32 color, MEM_SHADE_TYPE shade_type, void
	*start_ptr, size_t size)
{
	error_code ec;

	/* Make sure the color fits in a 16 bit value */
	if ((color & 0xffff0000) != 0) {
		return EOS_MAUI_BADVALUE;
	}

	/* Create a shade with a shade ID that is the same as the color */
	if ((ec = mem_create_shade(color, shade_type, 0, 0, 0, MEM_OV_SEPARATE,
		FALSE)) != SUCCESS) {
		return ec;
	}

	/* Set the allocator and deallocator functions for the shade. The */
	/* initial size on the allocator forces the memory to be assigned to */
	/* the shade immediately. */
	if ((ec = mem_set_alloc(color, alloc_color, start_ptr, size)) != SUCCESS) {
		mem_destroy_shade(color);
		return ec;
	}
	if ((ec = mem_set_dealloc(color, dealloc_color, start_ptr)) != SUCCESS) {
		mem_destroy_shade(color);
		return ec;
	}

	return SUCCESS;
}

/*******************************************************************************
* NAME:		gdv_destroy_mem_color()
*
* USAGE:	Destroy a color of memory. This function should be called once to
*			destroy each color of memory. The call to this function should be
*			done within the code for GDV_TERM_HW().
*/
error_code gdv_destroy_mem_color(u_int32 color)
{
	/* Destroy the shade */
	return mem_destroy_shade(color);
}

/*******************************************************************************
* NAME:		alloc_color()
*
* USAGE:	This is the allocation function used by gdv_create_mem_color()
*			above.
*/
static error_code alloc_color(void *start_ptr, size_t *size, void **mem_ptr,
	u_int32 color)
{
	/* Just return the memory pointer passed to this function */
	*mem_ptr = start_ptr;

	return SUCCESS;
}

/*******************************************************************************
* NAME:		dealloc_color()
*
* USAGE:	This is the de-allocation function used by gdv_create_mem_color()
*			above.
*/
static error_code dealloc_color(void *start_ptr, size_t size, void *mem_ptr,
	u_int32 color)
{
	return SUCCESS;
}

/*******************************************************************************
* NAME:		gdv_create_mem_shade()
*
* USAGE:	Create a shade of memory. This function should be called once to
*			create each shade of memory. The call to this function should be
*			done within the code for GDV_INIT_DVATCH().
*/
error_code gdv_create_mem_shade(GFX_DEV *gfxdev, u_int32 shade, MEM_SHADE_TYPE
	shade_type, u_int32 color, size_t initial_size, size_t grow_size)
{
	error_code ec;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;

	/* Make sure the shade fits in a 16 bit value */
	if ((shade & 0xffff0000) != 0) {
		return EOS_MAUI_BADVALUE;
	}

	/* The real shade number is the combination of the shade_hiword for this */
	/* logical device, and the caller supplied shade number. */
	shade = (dvatch->shade_hiword << 16) | (shade & 0xffff);

	/* Create the shade */
	if ((ec = mem_create_shade(shade, shade_type, color, 0, grow_size,
		MEM_OV_SEPARATE, FALSE)) != SUCCESS) {
		return ec;
	}

	/* Set the allocation boundary size if specified */
	#ifdef GDV_PIXMEM_BNDRY
	if ((ec = mem_set_alloc_bndry(shade, GDV_PIXMEM_BNDRY)) != SUCCESS) {
		mem_destroy_shade(shade);
		return ec;
	}
	#endif

	/* Set the allocator and deallocator functions for the shade. The */
	/* initial size on the allocator forces the memory to be assigned to */
	/* the shade immediately. */
	if ((ec = mem_set_alloc(shade, alloc_shade, NULL, initial_size))
		!= SUCCESS) {
		mem_destroy_shade(shade);
		return ec;
	}
	if ((ec = mem_set_dealloc(shade, dealloc_shade, NULL)) != SUCCESS) {
		mem_destroy_shade(shade);
		return ec;
	}

	return SUCCESS;
}

/*******************************************************************************
* NAME:		gdv_destroy_mem_shade()
*
* USAGE:	Destroy a shade of memory. This function should be called once to
*			destroy each shade of memory. The call to this function should be
*			done within the code for GDV_TERM_DVATCH().
*/
error_code gdv_destroy_mem_shade(GFX_DEV *gfxdev, u_int32 shade)
{
	error_code ec, result = SUCCESS;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;

	/* Make sure the shade fits in a 16 bit value */
	if ((shade & 0xffff0000) != 0) {
		return EOS_MAUI_BADVALUE;
	}

	/* The real shade number is the combination of the shade_hiword for this */
	/* logical device, and the caller supplied shade number. */
	shade = (dvatch->shade_hiword << 16) | (shade & 0xffff);

	/* Free all the segments in the shade and destroy the shade */
	if ((ec = mem_sfree_all(shade)) != SUCCESS) {
		result = ec;
	}
	if ((ec = mem_destroy_shade(shade)) != SUCCESS) {
		result = ec;
	}

	return result;
}

/*******************************************************************************
* NAME:		alloc_shade()
*
* USAGE:	This is the allocation function used by gdv_create_mem_shade()
*			above.
*/
static error_code alloc_shade(void *notused, size_t *size, void **mem_ptr,
	u_int32 color)
{
	error_code ec;

	/* Allocate the memory */
	if ((ec = mem_malloc(mem_ptr, color, *size)) != SUCCESS) {
		return ec;
	}

	return SUCCESS;
}

/*******************************************************************************
* NAME:		dealloc_shade()
*
* USAGE:	This is the de-allocation function used by gdv_create_mem_shade()
*			above.
*/
static error_code dealloc_shade(void *notused, size_t size, void *mem_ptr,
	u_int32 color)
{
	error_code ec;

	/* Free the memory */
	if ((ec = mem_sfree(color, mem_ptr)) != SUCCESS) {
		return ec;
	}

	return SUCCESS;
}

/*******************************************************************************
* NAME:		_gdv_alloc_mem()
*
* USAGE:	Allocate driver memory for the application.
*/
error_code _gdv_alloc_mem(Dev_list dev_entry, GFX_DEV *gfxdev, size_t *size,
	void **mem_ptr, u_int32 color)
{
	error_code ec;
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;
	size_t alloc_size = *size;
	char *alloc_ptr;

	/* If a prefix or postfix area is required, then add it to the size */
	#ifdef GDV_MEM_PREFIX
	alloc_size += GDV_MEM_PREFIX;
	#endif
	#ifdef GDV_MEM_POSTFIX
	alloc_size += GDV_MEM_POSTFIX;
	#endif

	/* The real shade number is the combination of the shade_hiword for this */
	/* logical device, and the caller supplied shade number. */
	color = (dvatch->shade_hiword << 16) | (color & 0xffff);

	/* Allocate the memory */
	if ((ec = mem_malloc(&alloc_ptr, color, alloc_size)) != SUCCESS) {
		return ec;
	}
	*mem_ptr = alloc_ptr;

	/* If a prefix area was added then adjust the segment pointer that is */
	/* returned to the application. */
	#ifdef GDV_MEM_PREFIX
	*mem_ptr = alloc_ptr + GDV_MEM_PREFIX;
	#endif

#ifdef GDV_MEM_INIT
        /* Initialize memory if user-defined function is specified */ 
	GDV_MEM_INIT(dev_entry, *mem_ptr, alloc_size, 0); 
#endif

#ifdef GDV_FE_SYSTATE
        /* Permit the memory for the application (not the prefix or the postfix) */
        _os_permit(*mem_ptr, *size, S_IREAD | S_IWRITE, gfxdev->pid);
#else
        /* Permit all the memory for the application and driver user state fast entry points */
        _os_permit((void *)alloc_ptr, alloc_size, S_IREAD | S_IWRITE, gfxdev->pid);
#endif

	return SUCCESS;	
}

/*******************************************************************************
* NAME:		_gdv_dealloc_mem()
*
* USAGE:	Deallocate driver memory for the application.
*/
error_code _gdv_dealloc_mem(Dev_list dev_entry, GFX_DEV *gfxdev, size_t size,
	void *mem_ptr, u_int32 color)
{
	GDV_DVATCH *dvatch = gfxdev->shared->gdv_attachment;

	/* The real shade number is the combination of the shade_hiword for this */
	/* logical device, and the caller supplied shade number. */
	color = (dvatch->shade_hiword << 16) | (color & 0xffff);

#ifdef GDV_FE_SYSTATE
        /* Protect the memory from the application (not the prefix or postfix) */
        _os_protect(mem_ptr, size, S_IREAD | S_IWRITE, gfxdev->pid);
#else
        /* Adjust size of the memory to be protected */
        #ifdef GDV_MEM_PREFIX
        size += GDV_MEM_PREFIX;
        #endif
        #ifdef GDV_MEM_POSTFIX
        size += GDV_MEM_POSTFIX;
        #endif
 
        /* Protect the memory from the application including prefix or postfix */
        #ifdef GDV_MEM_PREFIX
        _os_protect((char *)mem_ptr - GDV_MEM_PREFIX, size, S_IREAD | S_IWRITE, gfxdev->pid);
        #else
        _os_protect(mem_ptr, size, S_IREAD | S_IWRITE, gfxdev->pid);
        #endif 
#endif

	/* Deallocate the memory and return the result. If a prefix area was */
	/* added when the segment was allocated, then adjust the pointer so we */
	/* can return the entire block. */
	#ifdef GDV_MEM_PREFIX
		return mem_sfree(color, (char *)mem_ptr - GDV_MEM_PREFIX);
	#else
		return mem_sfree(color, mem_ptr);
	#endif
}

