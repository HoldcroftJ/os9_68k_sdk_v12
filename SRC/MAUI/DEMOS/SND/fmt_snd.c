
/*******************************************************************************
 *
 * DESCRIPTION:	Source for Sun/Next audio file format 
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */

#include <string.h>
#include "fmt_com.h"


error_code au_snd_init_smap(AUPath au_path, SND_SMAP *smap)
{
  AUSndStruct snd_header;
  error_code err;
  u_int32 size;

  /* clear smap structure */
  memset (smap, 0, sizeof(*smap));
  
  /* make sure we are at the start of the file */
  if ((err = au_seek (au_path, 0)) != SUCCESS)
    return err;
    
  size = sizeof(snd_header);
  /* read in the sound header */
  if (((err = au_read (au_path, &snd_header, &size)) != SUCCESS) ||
      (size != sizeof(snd_header)))
    return err;

  /* byteswap header if little endian */
#if !_BIG_END
  snd_header.magic = BE_DATA_SWAP32(snd_header.magic);
  snd_header.dataLocation = BE_DATA_SWAP32(snd_header.dataLocation);
  snd_header.dataSize = BE_DATA_SWAP32(snd_header.dataSize);
  snd_header.dataFormat = BE_DATA_SWAP32(snd_header.dataFormat);
  snd_header.samplingRate = BE_DATA_SWAP32(snd_header.samplingRate);
  snd_header.channelCount = BE_DATA_SWAP32(snd_header.channelCount);
#endif

  /* set the file pointer to the start of the data */
  if ((err = au_seek(au_path, snd_header.dataLocation)) != SUCCESS)
    return err;

  /* get the size of the data in this read */
  smap->buf_size = snd_header.dataSize;

  /* convert the data format */
  switch (snd_header.dataFormat) {
  case SND_FORMAT_MULAW_8:      /* 8-bit mu-law samples*/
	smap->coding_method  = SND_CM_PCM_ULAW;
	smap->sample_size = 8;
	break;

  case SND_FORMAT_ALAW_8:      /* 8-bit a-law samples*/
	smap->coding_method  = SND_CM_PCM_ALAW;
	smap->sample_size = 8;
	break;

  case SND_FORMAT_LINEAR_8:     /* 8-bit linear samples*/
	smap->coding_method  = SND_CM_PCM_ULINEAR;
	smap->sample_size = 8;
	break;
	
  case SND_FORMAT_LINEAR_16:    /* 16-bit linear samples*/
	smap->coding_method  = SND_CM_PCM_SLINEAR;
	smap->sample_size = 16;
	break;
	
  case SND_FORMAT_LINEAR_24:    /* 24-bit linear samples*/
	smap->coding_method  = SND_CM_PCM_SLINEAR;
	smap->sample_size = 24;
	break;
	
  case SND_FORMAT_LINEAR_32:    /* 32-bit linear samples*/
	smap->coding_method  = SND_CM_PCM_SLINEAR;
	smap->sample_size = 32;
	break;
	
  case SND_FORMAT_ADPCM_G721: 
	smap->coding_method  = SND_CM_ADPCM_G721;
	smap->sample_size = 4;  /* this size needs to be verified */
	break;

  case SND_FORMAT_ADPCM_G723_3:
  case SND_FORMAT_ADPCM_G723_5:
	smap->coding_method  = SND_CM_ADPCM_G723;
	smap->sample_size = 4;  /* this size needs to be verified */
	break;

  default:
	return EOS_MAUI_BADVALUE;
  }	

  smap->num_channels = snd_header.channelCount;
  smap->sample_rate = snd_header.samplingRate;
  
  return SUCCESS;
}


/* Create file <name> based on data in <smap>. See _os_create for
   description of the other arguments. */
error_code au_snd_create(AUPath au_path, SND_SMAP *smap)
{
  error_code err;

  /* write bad header until we fix it in au_snd_close() */
  au_path->format.snd.magic = SND_MAGIC_OPENPATH;
  au_path->format.snd.dataLocation = sizeof(au_path->format.snd);
  au_path->format.snd.dataSize = 0;
  au_path->format.snd.dataFormat = 0;

  /* convert the data format */
  switch (smap->coding_method) {
  case SND_CM_PCM_ULAW:
	switch (smap->sample_size) {
	case 8:
	  au_path->format.snd.dataFormat = 1;    /* SND_FORMAT_MULAW_8      8-bit mu-law samples*/
	  break;	
	default:
	  break;
	}
	break;

  case SND_CM_PCM_ALAW:
	switch (smap->sample_size) {
	case 8:
	  au_path->format.snd.dataFormat = 27;    /* SND_FORMAT_ALAW_8*/
	  break;	
	default:
	  break;
	}
	break;

  case SND_CM_PCM_ULINEAR:
	switch (smap->sample_size) {
	case 8:
	  au_path->format.snd.dataFormat = 2;    /* SND_FORMAT_LINEAR_8     8-bit linear samples*/
	  break;
	default:
	  break;
	}
	break;

  case SND_CM_PCM_SLINEAR:
	switch (smap->sample_size) {
	case 16:
	  au_path->format.snd.dataFormat = 3;    /* SND_FORMAT_LINEAR_16    16-bit linear samples*/
	  break;
	case 24:
	  au_path->format.snd.dataFormat = 4;    /* SND_FORMAT_LINEAR_24    24-bit linear samples*/
	  break;
	case 32:
	  au_path->format.snd.dataFormat = 5;    /* SND_FORMAT_LINEAR_32    32-bit linear samples*/
	  break;
	default:
	  break;
	}
	break;

  case SND_CM_ADPCM_G721:
	switch (smap->sample_size) {
	case 0:
	  au_path->format.snd.dataFormat = 23;    /* SND_FORMAT_ADPCM_G721*/
	  break;
	default:
	  break;
	}
	break;

  case SND_CM_ADPCM_G723:
	switch (smap->sample_size) {
	case 0:
#if 0
	  au_path->format.snd.dataFormat = 25;    /* SND_FORMAT_ADPCM_G723_3*/
#endif
	  au_path->format.snd.dataFormat = 26;    /* SND_FORMAT_ADPCM_G723_5*/
	  break;
	default:
	  break;
	}
	break;
  default:
	break;
  }	
  if (!au_path->format.snd.dataFormat)
	return EOS_MAUI_NOTIMPLEMENTED;

  au_path->format.snd.samplingRate = smap->sample_rate;
  au_path->format.snd.channelCount = smap->num_channels;
  *(u_int32*)&au_path->format.snd.info = 0;

  {
	u_int32 size = sizeof(au_path->format.snd);

	if ((err = au_write(au_path, &au_path->format.snd, &size)) != SUCCESS 
		|| size != sizeof(au_path->format.snd))
	  return err;
  }

  return SUCCESS;
}

error_code au_snd_close(AUPath au_path)
{
  error_code err;
  error_code result = SUCCESS;
  
  /* We don't need to do anything special for modules */
  if (au_path->modptr != NULL)
    return SUCCESS;

  /* fix the file size */
  if ((err = au_size(au_path, au_path->datptr)) != SUCCESS)
	result = err;

  /* seek back to the start */
  if ((err = au_seek (au_path, 0)) != SUCCESS)
	if (result == SUCCESS) result = err;

  /* fix the magic and size fields of the header */
  au_path->format.snd.magic = SND_MAGIC;
  au_path->format.snd.dataSize = au_path->bytesWritten;

  /* Rewrite the header */
  {
	u_int32 size = sizeof(au_path->format.snd);
	AUSndStruct snd_header = au_path->format.snd;

	/* byteswap header if little endian */
#if !_BIG_END
	snd_header.magic = BE_DATA_SWAP32(snd_header.magic);
	snd_header.dataLocation = BE_DATA_SWAP32(snd_header.dataLocation);
	snd_header.dataSize = BE_DATA_SWAP32(snd_header.dataSize);
	snd_header.dataFormat = BE_DATA_SWAP32(snd_header.dataFormat);
	snd_header.samplingRate = BE_DATA_SWAP32(snd_header.samplingRate);
	snd_header.channelCount = BE_DATA_SWAP32(snd_header.channelCount);
#endif

	if ((err = au_write(au_path, &snd_header, &size)) != SUCCESS 
		|| size != sizeof(au_path->format.snd))
	  if (result == SUCCESS) result = err;
  }

  return result;
}

