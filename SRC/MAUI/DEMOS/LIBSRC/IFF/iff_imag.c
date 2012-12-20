/*----------------------------------------------------------------------\
!                                                                       !
!																		!
!	Name: Iff_imag.c													!
!																		!
!	Function: Reader/Writer Code for IMAG type FORM's.					!
!																		!
!   Revision History:                                                   !
!   #       Reason for Change                           By      Date    !
!  ---- ----------------------------------------------- --- --------    !
!1	Created based on iff_rstr.c	                		srm	88/07/27	!
!2	Mods for Version 0.99								srm	89/02/01	!
!3	******** Release 0.99D **********									!
!4  little_endian and alignment issues for ARM          srm 97/08/15    !
\----------------------------------------------------------------------*/

/**************************************************************************
*                                                                         *
*              Copyright 1988 by Microware Systems Corporation            *
*                         Reproduced Under License.                       *
*                                                                         *
* This source code is the proprietary confidential property of Microware  *
* Systems Corporation, and is provided to licensee for documentation and  *
* educational purposes only. Reproduction, publication, or distribution in*
* any form to any party other than the licensee is strictly prohibited.   *
*                                                                         *
**************************************************************************/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <modes.h>
#include <sg_codes.h>
#include <types.h>
#include <MAUI/iff.h>
#include <iffrw.h>
#include <MAUI/iff_imag.h>
#include <errno.h>
#include <const.h>

extern int iff_skip_chunk(IFF_CONTEXT *, int);
extern int iff_start_chunk(IFF_CONTEXT *, int , int );
extern int iff_skip_bytes(IFF_CONTEXT *, int);
extern int iff_write_form(IFF_CONTEXT *, int, int);
extern int iff_write_data(IFF_CONTEXT *, char *, int);
extern int iff_is_zero (int *, int);
extern int iff_write_chunk(IFF_CONTEXT *, int, char *, 
                           int , char *, int);

/*****************************************************************************
 *
 * IFF_Write_IMAG - Write the header part of an IMAG type FORM.
 *
 * Input:	Context - Pointer to context structure.
 *			IMAG - Raster structure.
 *			datasize - Size of image data.
 * Output:	-1 is returned on error.
 *
 */
int 
iff_write_imag(IFF_CONTEXT *context, IFF_IMAG *imag, int datasize)
{
  int formsize, plte_size, yuv_size, user_size;
  char ipar_flag;
  
  /* make sure this context was opened for writing */
  if (context->mode == IFF_READ) 
	{
      errno = EOS_IFF_RONLY;
      return -1;
	}
  
  /* write the FORM group information */
  /* compute if write ipar */
  ipar_flag = !iff_is_zero ((int*)&(imag->ipar), SIZEOF_IFF_IPAR);
  
  /* compute the palette size */
  plte_size = imag->plte.plte_count * SIZEOF_IFF_COLOR;
  plte_size = IFF_MAKE_EVEN (plte_size);
  
  /* compute yuv size */
  yuv_size = (imag->ihdr.ihdr_model == IFF_MDL_DYUV) *	/* model must be dyuv */
    (imag->ihdr.ihdr_dyuv_kind == IFF_DYUV_EACH) *		/* kind must be each */
      imag->ihdr.ihdr_height * SIZEOF_IFF_YUV;
  yuv_size = IFF_MAKE_EVEN (yuv_size);
  
  /* compute user size */
  user_size = imag->user_data ? strlen (imag->user_data) + 1 : 0;
  
  /* compute the image data size */
  datasize = IFF_MAKE_EVEN (datasize);
  /* compute the form size */
  if (datasize == IFF_SIZE_UNKNOWN) {
    formsize = IFF_SIZE_UNKNOWN;
  } else {
    formsize =  + SIZEOF_IFF_IHDR;	/* image header */
    if (ipar_flag)
      formsize += SIZEOF_IFF_CHUNK + SIZEOF_IFF_IPAR;
    if (plte_size)
      formsize += SIZEOF_IFF_CHUNK + SIZEOF_IFF_PLTE + plte_size;
    if (yuv_size)
      formsize += SIZEOF_IFF_CHUNK + yuv_size;
    if (user_size)
      formsize += SIZEOF_IFF_CHUNK + user_size;
    if (datasize)
      formsize += SIZEOF_IFF_CHUNK + datasize;
  }
  /* write IMAG id */
  if (iff_write_form(context, IFF_ID_IMAG, formsize) == -1)
    return -1;
  
  /* write the IHDR header */
  {
    u_char tihdr[SIZEOF_IFF_IHDR];
    u_char *ptr = tihdr;
    
    /* handle potential alignment issues */
    iff_set16(ptr, imag->ihdr.ihdr_width);     ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->ihdr.ihdr_line_size); ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->ihdr.ihdr_height);    ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->ihdr.ihdr_model);     ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->ihdr.ihdr_depth);     ptr+=sizeof(IFF_U_16);
    *ptr++ = imag->ihdr.ihdr_dyuv_kind;
    *ptr++ = imag->ihdr.ihdr_dyuv_start.y;
    *ptr++ = imag->ihdr.ihdr_dyuv_start.u;
    *ptr   = imag->ihdr.ihdr_dyuv_start.v;
    if (iff_write_chunk(context, IFF_ID_IHDR, (void*)tihdr, SIZEOF_IFF_IHDR, 
                        0, 0) == -1)
      return -1;
  }
  
  /* write the IPAR chunk if necessary */
  if (ipar_flag) {
    u_char tipar[SIZEOF_IFF_IPAR];
    u_char *ptr = tipar;
    
    /* handle potential alignment issues */
    iff_set16(ptr, imag->ipar.ipar_x_off);     ptr+=sizeof(IFF_16);
    iff_set16(ptr, imag->ipar.ipar_y_off);     ptr+=sizeof(IFF_16);
    iff_set16(ptr, imag->ipar.ipar_x_ratio);   ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->ipar.ipar_y_ratio);   ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->ipar.ipar_x_page);    ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->ipar.ipar_y_page);    ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->ipar.ipar_x_grab);    ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->ipar.ipar_y_grab);    ptr+=sizeof(IFF_U_16);
    *ptr++ = imag->ipar.ipar_trans.r;
    *ptr++ = imag->ipar.ipar_trans.g;
    *ptr++ = imag->ipar.ipar_trans.b;
    *ptr++ = imag->ipar.ipar_mask.r;
    *ptr++ = imag->ipar.ipar_mask.g;
    *ptr   = imag->ipar.ipar_mask.b;
    
    if (iff_write_chunk(context, IFF_ID_IPAR, (void*)tipar, SIZEOF_IFF_IPAR, 
                        0, 0) == -1)
      return -1;
  }
  
  /* write the PLTE chunk if necessary */
  if (plte_size) {
    u_char tplte[SIZEOF_IFF_PLTE];
    u_char *ptr = tplte;
    
    /* handle potential alignment issues */
    iff_set16(ptr, imag->plte.plte_offset);     ptr+=sizeof(IFF_U_16);
    iff_set16(ptr, imag->plte.plte_count);      ptr+=sizeof(IFF_U_16);
    
    if (iff_write_chunk(context, IFF_ID_PLTE, (void*)tplte, SIZEOF_IFF_PLTE, 
                        (char*)imag->plte_data, plte_size) == -1)
      return -1;
  }
  
  /* write the YUVS chunk if necessary */
  if (yuv_size)
    if (iff_write_chunk(context, IFF_ID_YUVS, 0, 0, 
                        (char*)imag->yuv_data, yuv_size) == -1)
      return -1;
  
  /* write the USER chunk if necessary */
  if (user_size)
    if (iff_write_chunk(context, IFF_ID_USER, 0, 0, 
                        imag->user_data, user_size) == -1)
      return -1;
  
  /* write the start of the IDAT data chunk */
  if (datasize)
    if (iff_start_chunk(context, IFF_ID_IDAT, datasize) == -1)
      return -1;
  
  return 0;
}

/*****************************************************************************
 *
 * IFF_Read_IMAG - Read the header part of an IMAG type FORM.
 *
 * Input:	Context - Pointer to context structure.
 *			IMAG - Raster video structure.
 * Output:	The size of the DATA chunk is returned. -1 is returned on error.
 *
 */
int 
iff_read_imag( IFF_CONTEXT *context, IFF_IMAG *imag)
{
  u_int32 size;
  IFF_CHUNK chunk;
  u_char chunk_buf[SIZEOF_IFF_CHUNK];
  u_char *ptr;
  
  /* make sure this context was opened for reading */
  if (context->mode == IFF_WRITE) {
    errno = EOS_IFF_WONLY;
    return -1;
  }
  
  /* make sure its an IMAG type FORM */
  if (context->formid != IFF_ID_IMAG) {
    errno = EOS_IFF_READER;
    return -1;
  }
  
  /* read and process chunks within the IMAG type FORM */
  while (1) {
    /* read next chunk and dispatch on its type */
    size = SIZEOF_IFF_CHUNK;
    if (_os_read(context->path, ptr = chunk_buf, &size) != SUCCESS)
      return -1;
    
    /* cope with potential alignment issues */
    chunk.id   = iff_get32(ptr);       /* chunk.id   */
    chunk.size = iff_get32(ptr+4);     /* chunk.size */
    
    context->filepos += SIZEOF_IFF_CHUNK;
    context->formlen += SIZEOF_IFF_CHUNK;
    
    if (!imag) {
      errno = EOS_IFF_BADPARM;
      return -1;
    }
    switch (chunk.id) {
    case IFF_ID_IHDR:	/* standard header */
      {
        u_char tihdr[SIZEOF_IFF_IHDR];
        u_char *ptr = tihdr;
        
        /* psudo version check */
        if (chunk.size != SIZEOF_IFF_IHDR) {
          errno = EOS_IFF_BADCHUNKSIZE;
          return -1;
        }
        size = SIZEOF_IFF_IHDR;
        if (_os_read(context->path, ptr, &size) != SUCCESS)
          return -1;
        
        /* cope with potential alignment issues */
        imag->ihdr.ihdr_width = iff_get16(ptr);     ptr+=sizeof(IFF_U_16);
        imag->ihdr.ihdr_line_size = iff_get16(ptr); ptr+=sizeof(IFF_U_16);
        imag->ihdr.ihdr_height = iff_get16(ptr);    ptr+=sizeof(IFF_U_16);
        imag->ihdr.ihdr_model = iff_get16(ptr);     ptr+=sizeof(IFF_U_16);
        imag->ihdr.ihdr_depth = iff_get16(ptr);     ptr+=sizeof(IFF_U_16);
        imag->ihdr.ihdr_dyuv_kind = *ptr++;
        imag->ihdr.ihdr_dyuv_start.y = *ptr++;
        imag->ihdr.ihdr_dyuv_start.u = *ptr++;
        imag->ihdr.ihdr_dyuv_start.v = *ptr;

        context->filepos += SIZEOF_IFF_IHDR;
        context->formlen += SIZEOF_IFF_IHDR;
        imag->plte.plte_offset = 0;
        imag->plte.plte_count = 0;
      }
      break;
      
    case IFF_ID_IPAR:	/* optional image parameters */
      {
        u_char tipar[SIZEOF_IFF_IPAR];
        u_char *ptr = tipar;
        
        /* psudo version check */
        if (chunk.size != SIZEOF_IFF_IPAR) {
          errno = EOS_IFF_BADCHUNKSIZE;
          return -1;
        }
        size = SIZEOF_IFF_IPAR;
        if (_os_read(context->path, ptr, &size) != SUCCESS)
          return -1;
        
        /* cope with potential alignment issues */
        imag->ipar.ipar_x_off = iff_get16(ptr);     ptr+=sizeof(IFF_16);
        imag->ipar.ipar_y_off = iff_get16(ptr);     ptr+=sizeof(IFF_16);
        imag->ipar.ipar_x_ratio = iff_get16(ptr);   ptr+=sizeof(IFF_U_16);
        imag->ipar.ipar_y_ratio = iff_get16(ptr);   ptr+=sizeof(IFF_U_16);
        imag->ipar.ipar_x_page = iff_get16(ptr);    ptr+=sizeof(IFF_U_16);
        imag->ipar.ipar_y_page = iff_get16(ptr);    ptr+=sizeof(IFF_U_16);
        imag->ipar.ipar_x_grab = iff_get16(ptr);    ptr+=sizeof(IFF_U_16);
        imag->ipar.ipar_y_grab = iff_get16(ptr);    ptr+=sizeof(IFF_U_16);
        imag->ipar.ipar_trans.r = *ptr++;
        imag->ipar.ipar_trans.g = *ptr++;
        imag->ipar.ipar_trans.b = *ptr++;
        imag->ipar.ipar_mask.r = *ptr++;
        imag->ipar.ipar_mask.g = *ptr++;
        imag->ipar.ipar_mask.b = *ptr;
        
        context->filepos += SIZEOF_IFF_IPAR;
        context->formlen += SIZEOF_IFF_IPAR;
      }
      break;
      
    case IFF_ID_PLTE:	/* optional palette */
      {
        u_char tplte[SIZEOF_IFF_PLTE];
        u_char *ptr = tplte;
        
        if (!imag->plte_data) {
          if (iff_skip_chunk (context, chunk.size) == -1)
            return -1;
        } else {
          size = SIZEOF_IFF_PLTE;
          if (_os_read(context->path, ptr, &size) != SUCCESS)
            return -1;
          
          /* handle potential alignment issues */
          imag->plte.plte_offset = iff_get16(ptr);    ptr+=sizeof(IFF_U_16);
          imag->plte.plte_count = iff_get16(ptr);     ptr+=sizeof(IFF_U_16);
          
          context->filepos += SIZEOF_IFF_PLTE;
          context->formlen += SIZEOF_IFF_PLTE;
          
          size = imag->plte.plte_count * SIZEOF_IFF_COLOR;
          if (_os_read(context->path, imag->plte_data, &size) != SUCCESS)
            return -1;
          
          context->filepos += size;
          context->formlen += size;
          
          if (IFF_IS_ODD(size)) {
            if (iff_skip_bytes(context, 1) == -1)
              return -1;
          }
          if (imag->ihdr.ihdr_model == IFF_MDL_PLTE)
            return context->datasize = 0;
        }
      }
      break;
      
    case IFF_ID_YUVS:	/* optional DYUV start values */
      if (!imag->yuv_data) {
        if (iff_skip_chunk (context, chunk.size) == -1)
          return -1;
      } else {
        size = imag->ihdr.ihdr_height * SIZEOF_IFF_YUV;
        if (_os_read(context->path, imag->yuv_data, &size) != SUCCESS)
          return -1;
        context->filepos += size;
        context->formlen += size;
        if (IFF_IS_ODD(size)) {
          if (iff_skip_bytes(context, 1) == -1)
            return -1;
        }
      }
      break;
      
    case IFF_ID_USER:	/* comment chunk */
      if (iff_skip_chunk (context, chunk.size) == -1)
        return -1;
      break;
      
    case IFF_ID_IDAT:	/* actual image data */
      context->datasize = chunk.size;
      return context->datasize;
      
    default:		/* illegal chunk id */
      errno = EOS_IFF_NOTIFF;
      return -1;
    }
  }
}

int
iff_skip_chunk ( IFF_CONTEXT *context, int size)
{
  u_int32 pos;
  
  if (_os_gs_pos(context->path, &pos) != SUCCESS)
    return -1;
  if (_os_seek(context->path, size+pos) != SUCCESS)
    return -1;
  context->filepos += size;
  context->formlen += size;
  return 0;
}

int
iff_is_zero (int *s, int n)/* check for 'n' bytes of memory at 's' is zero */
{
  while ((n -= sizeof(*s)) > 0)
    if (*s++ != 0)
      return 0;
  if (n < 0)
    n += sizeof(*s);
  while (n-- > 0)
    if (*((char*)s) != 0)
      return 0;
    else
      (char*)s++;
  return 1;
}
