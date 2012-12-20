/*----------------------------------------------------------------------\
!                                                                       !
!   Name: Iff.c                                                         !
!                                                                       !
!   Function: Common IFF reader/writer code.                            !
!                                                                       !
!   Revision History:                                                   !
!   #       Reason for Change                           By      Date    !
!  ---- ----------------------------------------------- --- --------    !
!1  Creat                                               tod 88/06/22    !
!2  Mods for AIFF and IMAG                              srm 88/07/28    !
!3  fixed bug on PIPE EOF                               srm 88/08/02    !
!4  add iff_seek                                        srm 88/08/04    !
!5  iff_seek to return datapos                                          !
!   fixed iff_skip_data to conform to doc               srm 88/08/15    !
!6  fix iff_open to not require enclosing CAT           srm 88/10/11    !
!7  Mods for Version 0.99, fixed datasize padding                       !
!   on non-data ending FORMS (IMAG-PLTE)                srm 89/02/01    !
!8  ******** Release 0.99D **********                                   !
!9  moved context->datlen += length from                                !
!   iff_skip_bytes() to iff_skip_data() this fixed                      !
!   the problem with odd sized PLTE and YUVS chunks                     !
!   in iff_read_imag(), also changed the call to                        !
!   iff_skip_bytes() to iff_skip_data() in                              !
!   iff_seek_data() to make sure it is called                           !
!   within a data chunk                                 srm 89/04/05    !
!10     fixed iff_filetype to check DT_PIPE instead of                  !
!   DT_RBF, will now read audio IFF off of a CD,                        !
!   also fixed calculation of sampleFrames in                           !
!   iff_write_end                                       srm 89/09/13    !
!11 modified iff_open to free the memory for the                        !
!   context structure if an error is detected           ebm 91/08/15    !
!12 updated for ultra c portability                     ebm 93/05/14    !
!13 little_endian correction                            yap 95/08/29    !
!14 little_endian and alignment issues for ARM          srm 97/08/15    !
!15 Change E_* to EOS_* to make it ANSI-compliant       and 98/05/19    !
!16 move iff_skipbuf to the stack for threads           srm 00/05/05    !
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
#include <modes.h>
#include <types.h>
#include <errno.h>
#include <MAUI/iff.h>
#include <iffrw.h>
#include <MAUI/iff_aiff.h>  /* needed in iff_write_end() */
#include <io.h>
#include <sg_codes.h>
#include <memory.h>
#include <const.h>
#ifdef _OSK
#include <sgstat.h>
#endif

#define BITS_PER_BYTE   8

int iff_start_chunk(IFF_CONTEXT*, int , int );   
int iff_write_nulls(int, int);
int iff_skip_bytes( IFF_CONTEXT *, int);
int iff_filetype(path_id);

/*****************************************************************************
 *
 * IFF_Open - Open an iff context on the specified path.
 *
 * Input:   Path - Path number.
 *          RWMode - Read/write mode.
 *          ID - CAT or FORM id (IFF_CAT_UNKNOWN if not known).
 * Output:  Pointer to IFF context. Null returned on error.
 *
 */
IFF_CONTEXT *iff_open(short path, short rwmode,int id)
{
  IFF_CONTEXT *context;
  IFF_GROUP group;
  u_char group_buf[SIZEOF_IFF_GROUP];
  u_char *ptr;
  IFF_U_32 size;
  
  /* allocate memory for the structure and initialize it */
  size = sizeof(IFF_CONTEXT);
  if (_os_srqmem(&size,(void**)&context,MEM_ANY) != SUCCESS)
    return NULL;
  context->path = path;
  context->mode = rwmode;
  context->nextformread = 0;
  context->filepos = 0;
  context->filetype = iff_filetype(path);
  context->catid = 0;
  context->catpos = 0;
  context->catsize = 0;
  context->catlen = 0;
  context->formid = 0;
  context->formpos = 0;
  context->formsize = 0;
  context->formlen = 0;
  context->datapos = 0;
  context->datasize = -1;
  context->datalen = 0;
  context->framepos = 0;
  context->framefctr = 1;
  
  /* further processing is dependent on whether its a reader or writter */
  switch (rwmode) {
  case IFF_READ:    /* READER CODE */
    
    /* read the first group (must be CAT or FORM) */
    size = sizeof(group_buf);
    _os_read(path, ptr = group_buf, &size);
    if (size != sizeof(group_buf)) {
      errno = EOS_IFF_NOTIFF;
      /* free up the structure memory */
      _os_srtmem(sizeof(IFF_CONTEXT),(void*)context);
      return NULL;
    }
    
    /* deal with possible mis-alignment of members */
    group.type = iff_get32(ptr);
    group.size = iff_get32(ptr+4);
    group.id   = iff_get32(ptr+8);
    
    context->filepos += sizeof(group_buf);
    if (id != IFF_CAT_UNKNOWN) {
      if ((group.type != IFF_ID_CAT && group.type != IFF_ID_FORM) 
          || group.id != id) {
        errno = EOS_IFF_BADCAT;
        /* free up the structure memory */
        _os_srtmem(sizeof(IFF_CONTEXT),
                   (void*)context);
        return NULL;
      }
    }
    switch (group.type) {
    case IFF_ID_CAT:    /* group type is CAT */
      context->catid = group.id;
      context->catpos = context->filepos - 8;
      context->catsize = group.size;
      context->catlen = 4;
      break;
    case IFF_ID_FORM:   /* group type is FORM */
      context->nextformread = 1;
      context->formid = group.id;
      context->formpos = context->filepos - 8;
      context->formsize = group.size;
      context->formlen = 4;
      break;
    default:            /* group type is not CAT or FORM */
      errno = EOS_IFF_NOTIFF;
      /* free up the structure memory */
      _os_srtmem(sizeof(IFF_CONTEXT),
                 (void*)context);
      return NULL;
    }
    break;
  case IFF_WRITE:   /* WRITER CODE */
    /* write the first group (must be CAT) */
    /* deal with possible mis-alignment of members */
    ptr = group_buf;
    iff_set32(ptr,   IFF_ID_CAT);         /* group.type */
    iff_set32(ptr+4, IFF_SIZE_UNKNOWN);   /* group.size */
    iff_set32(ptr+8, id);                 /* group.id   */
    size = sizeof(group_buf);
    
    _os_write(path, ptr, &size);
    if (size != sizeof(group_buf)) {
      /* free up the structure memory */
      _os_srtmem(sizeof(IFF_CONTEXT),
                 (void*)context);
      return NULL;
    }
    
    context->filepos += sizeof(group_buf);
    context->catid = id;
    context->catpos = context->filepos - 8;
    context->catsize = IFF_SIZE_UNKNOWN;
    context->catlen = 4;
    break;
  default:      /* NOT READ OR WRITE */
    /* free up the structure memory */
    _os_srtmem(sizeof(IFF_CONTEXT),context);
    errno = EOS_IFF_BADPARM;
    return NULL;
  }
  
  return (context);
}

/*****************************************************************************
 *
 * IFF_FileType - Return the file type for the specified path.
 *
 * Input:   Path - Path number.
 * Output:  File type.
 *
 */
int iff_filetype(path_id path)
{
#ifdef _OSK
  struct sgbuf optbuf;
  u_int32 size = 128;
  
  _os_gs_popt(path,&size,&optbuf);
  return (optbuf.sg_class == DT_PIPE ? IFF_PIPE : IFF_RBF);
#else
  u_int16 type, class;
  _os_gs_devtyp(path, &type, &class);
  return (type == DT_PIPE ? IFF_PIPE : IFF_RBF);
#endif
}

/*****************************************************************************
 *
 * IFF_Close - Close an iff context.
 *
 * Input:   Context - Pointer to context structure.
 * Output:  -1 is returned on error.
 *
 */
int 
iff_close(IFF_CONTEXT *context)
{
  IFF_U_32 size;
  u_char buf[sizeof(IFF_U_32)];
  
  /* update the CAT size */
  if (context->filetype == IFF_RBF && context->catsize == IFF_SIZE_UNKNOWN) {
    if (_os_seek(context->path, context->catpos) != SUCCESS)
      return -1;
    
    /* deal with possible mis-alignment of members */
    iff_set32(buf, context->catlen);
    size = sizeof(IFF_U_32);
    if (_os_write(context->path, buf, &size) != SUCCESS)
      return -1;
    
    if (_os_seek(context->path, context->filepos) != SUCCESS)
      return -1;
  }
  
  /* deallocate the context structure */
  _os_srtmem(sizeof(IFF_CONTEXT),context);
  
  return 0;
}

/*****************************************************************************
 *
 * IFF_Write_Form - Write start of FORM group. Called by all "IFF_Write"
 *          functions.
 *
 * Input:   Context - Pointer to context structure.
 *          FormId - FORM id.
 *          FormSize - Size of FORM minus the FORM header.
 * Output:  -1 is returned on error.
 *
 */
int 
iff_write_form(IFF_CONTEXT *context,
               int formid,
               int formsize)
{
  u_char group_buf[SIZEOF_IFF_GROUP];
  u_char *ptr;
  IFF_U_32 size;
  IFF_U_32 group_size;
  
  /* deal with possible mis-alignment of members */
  ptr = group_buf;
  iff_set32(ptr,   IFF_ID_FORM);  /* group.type */
  group_size = (formsize == IFF_SIZE_UNKNOWN) ? IFF_SIZE_UNKNOWN :
    sizeof(group_buf) - 8 + formsize;
  iff_set32(ptr+4, group_size);   /* group.size */
  iff_set32(ptr+8, formid);       /* group.id   */
  size = sizeof(group_buf);
  
  if (_os_write(context->path, ptr, &size) != SUCCESS)
    return -1;
  
  context->filepos += sizeof(group_buf);
  context->catlen += sizeof(group_buf);
  context->formid = formid;
  context->formpos = context->filepos - 8;
  context->formsize = group_size;
  context->formlen = 4;
  context->datasize = -1;
  
  return 0;
}

/*****************************************************************************
 *
 * IFF_Write_Chunk - Write a chunk (all execpt the actual data).
 *
 * Input:   Context - Pointer to context structure.
 *          ChunkID - Chunk id.
 *          Data    - Pointer to the data.
 *          DataSize - Size of data in the chunk.
 *          Ptr     - Pointer to additional data.
 *          PtrSize - Size of additional data in the chunk.
 * Output:  -1 is returned on error.
 *
 */
int 
iff_write_chunk(IFF_CONTEXT *context,
                int chunkid,
                char *data,
                int datasize,
                char *ptr,
                int ptrsize)
{
  /* write the chunk header */
  if (iff_start_chunk(context,chunkid,datasize+ptrsize) == -1)
    return -1;
  /* write the chunk data */
  if (datasize)
    if (_os_write(context->path,data,(IFF_U_32*)&datasize) != SUCCESS) {
      return -1;
    }
  /* write the additional chunk data */
  if (ptrsize)
    if (_os_write(context->path,ptr,(IFF_U_32*)&ptrsize) != SUCCESS) {
      return -1;
    }
  /* update all the counters */
  context->filepos += (datasize += ptrsize);
  context->catlen += datasize;
  context->formlen += datasize;
  context->datasize = 0;            /* srm - 89/02/01 */
  
  return 0;
}

/*****************************************************************************
 *
 * IFF_Start_Chunk - Write start of a chunk (all execpt the actual data).
 *
 * Input:   Context - Pointer to context structure.
 *          ChunkID - Chunk id.
 *          DataSize - Size of data in the chunk.
 * Output:  -1 is returned on error.
 *
 */
int
iff_start_chunk(IFF_CONTEXT *context,
                int chunkid,
                int datasize)  
{
  u_char chunk_buf[SIZEOF_IFF_CHUNK];
  u_char *ptr;
  IFF_U_32 size;
  
  /* deal with possible mis-alignment of members */
  ptr = chunk_buf;
  iff_set32(ptr, chunkid);       /* chunk.id   */
  iff_set32(ptr+4, datasize);    /* chunk.size */
  size = sizeof(chunk_buf);
  
  if (_os_write(context->path, ptr, &size) != SUCCESS) {
    return -1;
  }
  
  context->filepos += sizeof(chunk_buf);
  context->catlen += sizeof(chunk_buf);
  context->formlen += sizeof(chunk_buf);
  context->datapos = context->filepos - 4;
  context->datasize = datasize;
  context->datalen = 0;
  
  return 0;
}

/*****************************************************************************
 *
 * IFF_Write_Data - Write data for a FORM.
 *
 * Input:   Context - Pointer to context structure.
 *          Buffer - Pointer to data buffer.
 *          Length - Number of bytes to write.
 * Output:  Number of bytes written. -1 is returned on error.
 *
 */
int 
iff_write_data(IFF_CONTEXT *context,
               char *buffer,
               int length)
{
  /* make sure this context was opened for writing */
  if (context->mode == IFF_READ) {
    errno = EOS_IFF_RONLY;
    return -1;
  }
  
  /* make sure the DATA chunk header has been written */
  if (context->datasize == -1) {
    errno = EOS_IFF_NOT_DATA;
    return -1;
  }
  
  /* make sure we are not writing more bytes than allowed */
  if (context->datasize != IFF_SIZE_UNKNOWN) {
    if (length > context->datasize-context->datalen)
      length = context->datasize-context->datalen;
  }
  
  /* write the data */
  if (length) {
    if (_os_write(context->path,buffer,(IFF_U_32*)&length) != SUCCESS) {
      return -1;
    }
    context->filepos += length;
    context->catlen += length;
    context->formlen += length;
    context->datalen += length;
    return length;
  } else {
    return 0;
  }
}

/*****************************************************************************
 *
 * IFF_Write_End - Done writting the data portion of a FORM. Seek back and
 *          update the FORM and DATA sizes.
 *
 * Input:   Context - Pointer to context structure.
 * Output:  -1 is returned on error.
 *
 */
int 
iff_write_end(IFF_CONTEXT *context)
{
  int c;
  int temp=0;
  u_char buf[sizeof(IFF_U_32)];
  IFF_U_32 size;
  
  /* make sure this context was opened for writing */
  if (context->mode == IFF_READ) {
    errno = EOS_IFF_RONLY;
    return -1;
  }
  
  /* extend the data size to the correct length */
  if (context->datasize == IFF_SIZE_UNKNOWN) {
    if (IFF_IS_ODD(context->datalen)) {
      iff_write_nulls(context->path,1);
      context->filepos++;
      context->catlen++;
      context->formlen++;
      context->datalen++;
    }
  } else {
    if ((c=context->datasize - context->datalen) != 0) {
      iff_write_nulls(context->path,c);
      context->filepos += c;
      context->catlen += c;
      context->formlen += c;
      context->datalen += c;
    }
  }
  
  /* check for RBF type file */
  if (context->filetype == IFF_RBF) {
    
    /* update the FORM size */
    if (context->formsize == IFF_SIZE_UNKNOWN) {
      if (_os_seek(context->path, context->formpos) != SUCCESS)
        return -1;
      
      /* deal with possible mis-alignment of members */
      iff_set32(buf, context->formlen);
      size = sizeof (IFF_U_32);
      if (_os_write(context->path, buf, &size) != SUCCESS)
        return -1;
    }
    
    /* update the DATA size */
    if (context->datasize == IFF_SIZE_UNKNOWN) {
      if (context->catid == IFF_ID_AIFF) {
        /* update AIFF numSampleFrames */
        temp = (context->datalen - sizeof (blockAlign)) / context->framefctr;
        if (_os_seek(context->path, context->framepos) != SUCCESS)
          return -1;
        
        /* deal with possible mis-alignment of members */
        iff_set32(buf, context->formlen);
        size = sizeof (IFF_U_32);
        if (_os_write(context->path, buf, &size) != SUCCESS)
          return -1;
        
      }
      if (_os_seek(context->path, context->datapos) != SUCCESS)
        return -1;
      
      /* deal with possible mis-alignment of members */
      iff_set32(buf, temp);
      size = sizeof (IFF_U_32);
      if (_os_write(context->path, buf, &size) != SUCCESS)
        return -1;
    }
    
    /* seek back to the current point in the file */
    if (_os_seek(context->path, context->filepos) != SUCCESS)
      return -1;
  }
  
  return 0;
}

/*****************************************************************************
 *
 * IFF_Write_Nulls - Write the specified number of filler characters (NULL) to
 *          the specified path.
 *
 * Input:   Path - Path number
 *          Count - Number of NULL's to write.
 * Output:  -1 is returned on error.
 *
 */
int 
iff_write_nulls(int path, int count)
{
  char null[1];
  IFF_U_32 size;
  
  null[0] = 0;
  size = 1;
  while (count--) {
    if (_os_write(path,null,&size) != SUCCESS)
      return -1;
  }
  return 0;
}

/*****************************************************************************
 *
 * IFF_Read_What_Next - Read the start of the next FORM to determine its type.
 *
 * Input:   Context - Pointer to context structure.
 * Output:  Form type is returned. -1 is returned on error and 0 at eof.
 *
 */
int 
iff_read_what_next(IFF_CONTEXT *context)
{
  IFF_GROUP group;
  u_char group_buf[SIZEOF_IFF_GROUP];
  u_char *ptr;
  IFF_U_32 size;
  
  /* make sure this context was opened for reading */
  if (context->mode == IFF_WRITE) {
    errno = EOS_IFF_WONLY;
    return -1;
  }
  
  /* skip to the start of the next FORM */
  if (context->nextformread) {
    context->nextformread = 0;
  } else {
    if (context->formsize == IFF_SIZE_UNKNOWN) {
      errno = EOS_IFF_SIZEOS_UNKNOWN;
      return -1;
    } else {
      if (iff_skip_bytes(context,context->formsize-context->formlen)
          == -1)
        return -1;
    }
    
    /* read the FORM group structure */
    size = sizeof(group_buf);
    if (_os_read(context->path, ptr = group_buf, &size) != SUCCESS)
      return -1;
    if (size != sizeof(group_buf))
      return ((int32)size);
    
    /* deal with possible mis-alignment of members */
    group.type = iff_get32(ptr);
    group.size = iff_get32(ptr+4);
    group.id   = iff_get32(ptr+8);
    
    if (group.type != IFF_ID_FORM) {
      errno = EOS_IFF_NOTIFF;
      return -1;
    }
    context->filepos += sizeof(group_buf);
    context->formid = group.id;
    context->formsize = group.size;
    context->formlen = 4;
    context->datasize = -1;
    context->datalen = 0;
  }
  
  return (context->formid);
}

/*****************************************************************************
 *
 * IFF_Read_Data - Read data in a FORM.
 *
 * Input:   Context - Pointer to context structure.
 *          Buffer - Pointer to data buffer.
 *          Length - Number of bytes to write.
 * Output:  Number of bytes read. -1 is returned on error.
 *
 */
int 
iff_read_data(IFF_CONTEXT *context, char *buffer, int length)
{
  /* make sure this context was opened for reading */
  if (context->mode == IFF_WRITE) {
    errno = EOS_IFF_WONLY;
    return -1;
  }
  
  /* make sure the DATA chunk header has been read */
  if (context->datasize == -1) {
    errno = EOS_IFF_NOT_DATA;
    return -1;
  }
  
  /* make sure we are not reading more bytes than allowed */
  if (context->datasize != IFF_SIZE_UNKNOWN) {
    if (length > context->datasize-context->datalen)
      length = context->datasize-context->datalen;
  }
  
  /* read the data */
  if (length) {
    if (_os_read(context->path,buffer,(IFF_U_32*)&length) != SUCCESS) {
      return -1;
    }
    context->filepos += length;
    context->formlen += length;
    context->datalen += length;
    return length;
  } else {
    return 0;
  }
}

/*****************************************************************************
 *
 * IFF_Skip_Data - Skip specified number of bytes.
 *
 * Input:   Context - Pointer to context structure.
 *          Length - Number of bytes to skip.
 * Output:  Number of bytes skipped. -1 is returned on error.
 *
 */
int
iff_skip_data(IFF_CONTEXT *context, int length)
{
  /* make sure this context was opened for reading */
  if (context->mode == IFF_WRITE) {
    errno = EOS_IFF_WONLY;
    return -1;
  }
  
  /* make sure the DATA chunk header has been read */
  if (context->datasize == -1) {
    errno = EOS_IFF_NOT_DATA;
    return -1;
  }
  
  /* make sure we are not skiping more bytes than allowed */
  if (context->datasize != IFF_SIZE_UNKNOWN) {
    if (!length || length > context->datasize-context->datalen)
      length = context->datasize-context->datalen;
  }
  
  /* skip specified number of bytes */
  if (length) {
    context->datalen += length;
    if (iff_skip_bytes(context,length) == -1)
      return -1;
  }
  
  return length;
}

/*****************************************************************************
 *
 * IFF_Skip_Bytes - Skip specified number of bytes.
 *
 * Input:   Context - Pointer to context structure.
 *          Length - Number of bytes to skip.
 * Output:  -1 is returned on error.
 *
 */
int 
iff_skip_bytes(IFF_CONTEXT *context, int length)
{
  IFF_U_32 size1,size2;
  char iff_skipbuf[IFF_SKIP_SIZE];    /* size of buffer used to skip data */
  
  context->filepos += length;
  context->formlen += length;
  
  /* seek if its an RBF device, otherwise just read the bytes */
  if (context->filetype == IFF_RBF) {
    if (_os_seek(context->path, context->filepos) != SUCCESS)
      return -1;
  } else {
    if (length < 0) {
      errno = EOS_IFF_PIPEOS_SEEK;
      return -1;
    }
    while (length) {
      size1 = (length < IFF_SKIP_SIZE) ? length : IFF_SKIP_SIZE;
      size2 = size1;
      _os_read(context->path,iff_skipbuf,&size1);
      if (size1 != size2)
        return -1;
      length -= size1;
    }
  }
  
  return 0;
}

/*****************************************************************************
 *
 * IFF_Seek - Seek within a data section
 *
 * Input:   Context  - Pointer to context structure.
 *          Position - Byte offset
 *          Place    - 0 - from the beginning of the file
 *                     1 - from the current file position
 *                     2 - from the end of the file
 * Output:  -1 is returned on error.
 *
 */
int 
iff_seek(IFF_CONTEXT *context, int position, int place)
{
  /* make sure this context was opened for reading */
  if (context->mode == IFF_WRITE) {
    errno = EOS_IFF_WONLY;
    return -1;
  }
  
  /* make sure the DATA chunk header has been read */
  if (context->datasize == -1) {
    errno = EOS_IFF_NOT_DATA;
    return -1;
  }
  
  /* make sure the DATA chunk has a known size */
  if (context->datasize == IFF_SIZE_UNKNOWN) {
    errno = EOS_IFF_SIZEOS_UNKNOWN;
    return -1;
  }
  
  switch (place) {
  case 0:       /* from the beginning of the file */
    break;
  case 1:       /* from the current file position */
    position += context->datalen;
    break;
  case 2:       /* from the end of the file */
    position -= context->datasize;
    break;
  default:
    errno = EOS_IFF_BADPARM;
    return -1;
  }
  
  /* make sure we are still in the data section */
  if (position < 0 || position > context->datasize) {
    errno = EOS_IFF_NOT_DATA;
    return -1;
  }
  
  /* was ... iff_skip_bytes (...); - 890405*/
  place = iff_skip_data (context, position - context->datalen);
  return (place == -1 ? place : context->datalen);
}
