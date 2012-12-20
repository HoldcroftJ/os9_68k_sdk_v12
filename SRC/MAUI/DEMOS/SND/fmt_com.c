
/*******************************************************************************
 *
 * DESCRIPTION:	Common function used in both play and record
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */
#include <modes.h>
#include <stdlib.h>
#include <sg_codes.h>
#include <string.h>
#include "fmt_com.h"
#include "fmt_snd.h"
#include "fmt_wav.h"


#if defined(_OSK)
  #if !defined(MODNAME)
    #define MODNAME(mod) ((u_char*)((u_char*)mod + ((Mh_com)mod)->_mname))
  #endif
  #define MODSIZE(mod) ((u_int32)((Mh_com)mod)->_msize)
  #define MODEXEC(mod) ((u_int32)((Mh_exec)mod)->_mexec)
#else
  #if !defined(MODNAME)
    #define MODNAME(mod) ((u_char*)((u_char*)mod + ((Mh_com)mod)->m_name))
  #endif
  #define MODSIZE(mod) ((u_int32)((Mh_com)mod)->m_size)
  #define MODEXEC(mod) ((u_int32)((Mh_com)mod)->m_exec)
#endif

error_code au_open(AUPath *au_path_ptr, SND_SMAP *smap, char *name)
{
  AUPath au_path;
  error_code err;
  u_int32 size;
  u_int32 id;
  u_int16 type_lang = mktypelang(MT_DATA, ML_ANY);
  u_int16 attr_rev = 0;
  char *mod_name;

  if ((*au_path_ptr = au_path = (AUPath) malloc(sizeof(*au_path)))
      == NULL)
	return errno;

  au_path->mode = AU_ACCESS_MODE_READ;
  au_path->modptr = NULL;
  au_path->datptr =
    au_path->datend =
      au_path->bytesWritten = 0;

  /* search first as a module, then a file */
  mod_name = name;
  if (_os_link(&mod_name, &au_path->modptr, (void**)&au_path->datptr,
               &type_lang, &attr_rev) == SUCCESS) {

    /* found as a module */
    au_path->path = 0;

    /* There really is not a good way to figure out exactly where the
       data ends in a data module from just the module header. For
       now, we will estimate the end point as one of two ways. Case
       #1) If the module name is after the exec pointer, we will
       assume the end is where the NAME starts. Case #2) If the module
       name is before the exec pointer, we will say the end is where
       the CRC is.

       This method has the disadvantage of fixmod or somebody renaming
       the module, resulting in left over old string fragments in the
       module. But this method will have to do for now. */

    /* set datend to where the name is (Case #1) */
    au_path->datend = (u_int32)MODNAME(au_path->modptr);

    /* Check for (Case #2) */
    if (au_path->datend < au_path->datptr) {
      au_path->datend = (u_int)au_path->modptr + MODSIZE(au_path->modptr) - 4;
    }

    /* look at the first four bytes to guess what kind of file it is */
    memcpy (&id, (void*)au_path->datptr, sizeof (u_int32));
    au_path->datptr += 4;


  } else if ((err = _os_open(name, FAM_READ, &au_path->path)) == SUCCESS) {

    /* opened as a file */
    au_path->modptr = NULL;
    au_path->datptr =
      au_path->datend = 0;
    size = sizeof (id);

    /* read the first four bytes to guess what kind of file it is */
    if (((err = au_read(au_path, &id, &size)) != SUCCESS) ||
        (size != sizeof (id))) {
      free (au_path);
      *au_path_ptr = NULL;
      return err;
    }

  } else {

    /* can't find it, too bad */
    free (au_path);
    *au_path_ptr = NULL;
    return err;

  }
	
  switch (id) {
  case BE_DATA_SWAP32(SND_MAGIC):
  case LE_DATA_SWAP32(SND_MAGIC):
    err = au_snd_init_smap(au_path, smap);
    break;

  case BE_DATA_SWAP32(WAVE_MAGIC):
  case LE_DATA_SWAP32(WAVE_MAGIC):
    err = au_wav_init_smap(au_path, smap);
    break;

  default:
	err = EOS_MAUI_BADID;
    break;
  }

  if (err != SUCCESS) {
    free (au_path);
    *au_path_ptr = NULL;
  }

  return err;
}


error_code au_create(AUPath *au_path_ptr, SND_SMAP *smap, 
                     AUFileType ftype, char* name, u_int32 mode,
                     u_int32 perm, u_int32 filesize)
{
  AUPath au_path;
  error_code err;

  if ((*au_path_ptr = au_path = (AUPath) malloc(sizeof(*au_path)))
      == NULL)
	return errno;

  au_path->mode = AU_ACCESS_MODE_WRITE;
  au_path->modptr = NULL;
  au_path->datptr =
    au_path->datend =
      au_path->bytesWritten = 0;

  if (filesize) {
	err = _os_create (name, mode|FAM_SIZE, &au_path->path, perm, filesize);
  } else {
	err = _os_create (name, mode, &au_path->path, perm);
  }
  if (err != SUCCESS) {
	free (au_path);
	*au_path_ptr = NULL;
	return err;
  }

  switch (au_path->type = ftype) {
  case AU_FILE_TYPE_SND:
	err = au_snd_create(au_path, smap);
	break;

  case AU_FILE_TYPE_WAVE:
	err = au_wav_create(au_path, smap);
	break;

  default:
	err = EOS_MAUI_BADVALUE;
  }

  if (err != SUCCESS) {
	_os_close (au_path->path);
	free (au_path);
	*au_path_ptr = NULL;
  }

  return err;
}


error_code au_write(AUPath au_path, void *buf, u_int32 *count)
{
  error_code err;

  if (au_path->mode != AU_ACCESS_MODE_WRITE || au_path->modptr != NULL)
    return EOS_BMODE;

  /* write the data */
  if ((err = _os_write(au_path->path, buf, count)) != SUCCESS)
	return err;

  /* update the size field */
  au_path->datptr += *count;

  return SUCCESS;
}


error_code au_write_data(AUPath au_path, void *buf, u_int32 *count)
{
  error_code err;

  if ((err = au_write(au_path, buf, count)) != SUCCESS)
    return err;

  au_path->bytesWritten += *count;

  return SUCCESS;
}


error_code au_size(AUPath au_path, u_int32 size)
{
  if (au_path->mode != AU_ACCESS_MODE_WRITE || au_path->modptr != NULL)
    return EOS_BMODE;

  /* fix the file size */
  return _os_ss_size(au_path->path, size);
}


error_code au_read(AUPath au_path, void *buf, u_int32 *count)
{
  error_code err;

  if (au_path->mode != AU_ACCESS_MODE_READ)
    return EOS_BMODE;

  if (au_path->modptr == NULL) {
    
    /* read the data */
    if ((err = _os_read(au_path->path, buf, count)) != SUCCESS)
      return err;

    /* update the size field */
    au_path->datptr += *count;

  } else {
    /* calculate new file position after read */
    u_int32 newpos = au_path->datptr + *count;

    /* handle end-of-file case */
    if ( newpos >= au_path->datend) {
      if (au_path->datptr > au_path->datend ||
         (*count = au_path->datend - au_path->datptr) == 0) {
        return EOS_EOF;
      }
    }

    memcpy(buf, (void*)au_path->datptr, *count);

    /* update the size field */
    au_path->datptr = newpos;
  }

  return SUCCESS;
}

error_code au_ptrread(AUPath au_path, void **ret_buf, u_int32 *count)
{
  if (au_path->mode != AU_ACCESS_MODE_READ ||au_path->modptr == NULL)
    return EOS_BMODE;
    
  { /* calculate new file position after read */
    u_int32 newpos = au_path->datptr + *count;

    /* handle end-of-file case */
    if ( newpos >= au_path->datend) {
      if (au_path->datptr > au_path->datend ||
         (*count = au_path->datend - au_path->datptr) == 0) {
        return EOS_EOF;
      }
    }

    /* update the ret_buf and size fields */
    *ret_buf = (void*)au_path->datptr;
    au_path->datptr = newpos;
  }

  return SUCCESS;
}


error_code au_seek(AUPath au_path, u_int32 offset)
{
  error_code err;

  if (au_path->modptr == NULL) {
    
    /* write the data */
    if ((err = _os_seek(au_path->path, offset)) != SUCCESS)
      return err;

    au_path->datptr = offset;

  } else {

    au_path->datptr = (u_int32)au_path->modptr +
      MODEXEC(au_path->modptr) + offset;

  }

  return SUCCESS;
}


error_code au_close(AUPath au_path, u_int32 size)
{
  error_code err;
  error_code result = SUCCESS;

  /* If a size was specified on the close, truncate the file to this
     new length */
  if (size)
    au_path->datptr = size;
  
  /* call the appropriate close */
  switch (au_path->type) {
  case AU_FILE_TYPE_SND:
    err = au_snd_close(au_path);
    break;
  case AU_FILE_TYPE_WAVE:
    err = au_wav_close(au_path);
    break;
  default:
    err = EOS_MAUI_BADVALUE;
  }
  if (err != SUCCESS)
    result = err;
  
  if (au_path->modptr != NULL) {

    if ((err = _os_unlink(au_path->modptr)) != SUCCESS)
      if (result == SUCCESS)
        result = err;

  } else {
    
    /* close the file */
    if ((err = _os_close(au_path->path)) != SUCCESS)
      if (result == SUCCESS)
        result = err;

  }

  /* Free the au_path */
  free(au_path);

  return result;
}
