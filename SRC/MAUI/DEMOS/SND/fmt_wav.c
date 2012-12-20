
/*******************************************************************************
 *
 * DESCRIPTION:	Source for RIFF-WAVE audio file format 
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */

#include <modes.h>
#include <string.h>
#include <stdio.h>
#include <sg_codes.h>
#include "fmt_com.h"


error_code au_wav_init_smap(AUPath au_path, SND_SMAP *smap)
{
  WAVE_CHUNK chunk;
  error_code err;
  u_int32 filesize;
  u_int32 size;
  u_int32 pos = 0;

  /* clear smap structure */
  memset (smap, 0, sizeof(*smap));
  
  /* make sure we are at the start of the file */
  if ((err = au_seek(au_path, 0)) != SUCCESS)
	return err;

  size = sizeof(chunk);
  /* read the first chunk to see if it is a RIFF header */
  if (((err = au_read (au_path, &chunk, &size)) != SUCCESS) ||
	  (size != sizeof(chunk)))
	return err;
  pos += size;

  /* Verify the RIFF header */
  if (chunk.ckid != WAVE_ID_RIFF)
	return EOS_MAUI_BADID;
  filesize = LE_DATA_SWAP32(chunk.cksize) + size;

  size = sizeof(chunk.ckid);
  /* read the next chunk to verify that it is a WAVE */
  if (((err = au_read (au_path, &chunk.ckid, &size)) != SUCCESS) ||
	  (size != sizeof(chunk.ckid)))
	return err;
  pos += size;

  if (chunk.ckid != WAVE_ID_WAVE)
	return EOS_MAUI_BADID;

  while (pos < filesize) {

	/* seek the start of the next chunk */
    if ((err = au_seek(au_path, pos)) != SUCCESS)
	  return err;

	size = sizeof(chunk);
	/* read the chunks header */
	if (((err = au_read (au_path, &chunk, &size)) != SUCCESS) ||
		(size != sizeof(chunk)))
	  return err;
	/* figure out where we go next time */
	pos += LE_DATA_SWAP32(chunk.cksize) + size;

	switch (chunk.ckid) {

	case WAVE_ID_FMT:         /* Format Chunk */
	  { 
		WAVE_FORMAT_FIELDS fmt;

		size = sizeof(fmt);
		/* read the format data */
		if (((err = au_read (au_path, &fmt, &size)) != SUCCESS) ||
			(size != sizeof(fmt)))
		  return err;

		smap->num_channels = LE_DATA_SWAP16(fmt.Channels);
		smap->sample_rate = LE_DATA_SWAP32(fmt.SamplesPerSec);

		switch (LE_DATA_SWAP16(fmt.FormatTag)) {
		case WAVE_FORMAT_PCM:
		  if ((smap->sample_size = LE_DATA_SWAP16(fmt.BitsPerSample)) == 8)
            smap->coding_method = SND_CM_PCM_ULINEAR|SND_CM_LSBYTE1ST;
          else
            smap->coding_method = SND_CM_PCM_SLINEAR|SND_CM_LSBYTE1ST;
		  break;

		case WAVE_FORMAT_ALAW:
		  smap->coding_method = SND_CM_PCM_ALAW;
		  smap->sample_size = 8;
		  break;

		case WAVE_FORMAT_MULAW:
		  smap->coding_method = SND_CM_PCM_ULAW;
		  smap->sample_size = 8;
		  break;
		  
		case WAVE_FORMAT_CS_IMA_ADPCM:
		  smap->coding_method = SND_CM_ADPCM_IMA;
		  smap->sample_size = 4;
		  /* SamplesPerBlock is ignored for now */
		  break;

 	 /* case WAVE_FORMAT_DVI_ADPCM: removed because duplicate case */
		case WAVE_FORMAT_IMA_ADPCM:
		case WAVE_FORMAT_DSPGROUP_TRUESPEECH:
		case WAVE_FORMAT_ADPCM:
		case WAVE_FORMAT_IBM_CVSD:
		case WAVE_FORMAT_OKI_ADPCM:
		case WAVE_FORMAT_DIGISTD:
		case WAVE_FORMAT_DIGIFIX:
		case WAVE_FORMAT_YAMAHA_ADPCM:
		case WAVE_FORMAT_SONARC:
		case WAVE_FORMAT_ECHOSC1:
		case WAVE_FORMAT_AUDIOFILE_AF36:
		case WAVE_FORMAT_APTX:
		case WAVE_FORMAT_AUDIOFILE_AF10:
		case WAVE_FORMAT_DOLBY_AC2:
		case IBM_FORMAT_MULAW:
		case IBM_FORMAT_ALAW:
		case IBM_FORMAT_ADPCM:
		case WAVE_FORMAT_CREATIVE_ADPCM:
		  return EOS_MAUI_NOTIMPLEMENTED;

		case WAVE_FORMAT_UNKNOWN:
		default:
		  return EOS_MAUI_BADVALUE;

		}
		break;
	  }
	case WAVE_ID_DATA:         /* Data Chunk */
	  /* if sample rate is not defined yet there was no Format Chunk */
	  if (!smap->sample_rate)
		return EOS_MAUI_BADPOS;

	  /* get the size of the data in this read */
	  smap->buf_size = LE_DATA_SWAP32(chunk.cksize);

	  /* return with the file pointer at the start of the data */
	  return SUCCESS;

	default:
	  /* ignore unknown chunk types */
	  break;
	}
  }

  /* if we made it here we never did find the data chunk */
  return EOS_EOF;
}


/* Create file <name> based on data in <smap>. See _os_create for
   description of the other arguments. */
error_code au_wav_create(AUPath au_path, SND_SMAP *smap)
{
  error_code err;

  /* write bad header until we fix it in au_wav_close() */
  au_path->format.wav.riff_id = WAVE_MAGIC_OPENPATH;
  au_path->format.wav.riff_size = LE_DATA_SWAP16(0); /* LE_DATA_SWAP 0? At least it is consistent */
  au_path->format.wav.wave_id = WAVE_ID_WAVE;
  au_path->format.wav.fmt_id = WAVE_ID_FMT;
  au_path->format.wav.fmt_size = LE_DATA_SWAP32(sizeof(WAVE_FORMAT_FIELDS));
  switch (snd_get_cm_name(smap->coding_method)) {
  case SND_CM_PCM_ULAW:              /* ULAW encoded PCM */
	au_path->format.wav.fmt.FormatTag = LE_DATA_SWAP16(WAVE_FORMAT_MULAW);
	break;
  case SND_CM_PCM_ALAW:              /* ALAW encoded PCM */
	au_path->format.wav.fmt.FormatTag = LE_DATA_SWAP16(WAVE_FORMAT_ALAW);
	break;
  case SND_CM_PCM_ULINEAR:            /* Linear Unsigned encoded PCM */
	/* WAVE specifies 1 to 8 bits are unsigned */
	if (smap->sample_size > 8)
	  return EOS_MAUI_BADCODEMETH;
	au_path->format.wav.fmt.FormatTag = LE_DATA_SWAP16(WAVE_FORMAT_PCM);
	break;
  case SND_CM_PCM_SLINEAR:            /* Linear Signed encoded PCM */
    /* WAVE specifies that 9 or more bits be signed, LSB */
	if ((smap->sample_size <= 8) || (SND_CM_LSBYTE1ST & smap->coding_method))
	  return EOS_MAUI_BADCODEMETH;
	au_path->format.wav.fmt.FormatTag = LE_DATA_SWAP16(WAVE_FORMAT_PCM);
	break;
  case SND_CM_ADPCM_IMA:             /* IMA ADPCM */
	au_path->format.wav.fmt.FormatTag = LE_DATA_SWAP16(WAVE_FORMAT_CS_IMA_ADPCM);
	break;
  case SND_CM_ADPCM_G721:            /* CCITT G.721 ADPCM */
  case SND_CM_ADPCM_G723:            /* CCITT G.723 ADPCM */
	return EOS_MAUI_NOTIMPLEMENTED;
  default:
	return EOS_MAUI_BADVALUE;
  }
  au_path->format.wav.fmt.Channels = LE_DATA_SWAP16(smap->num_channels);
  au_path->format.wav.fmt.SamplesPerSec = LE_DATA_SWAP32(smap->sample_rate);
  { u_int32 tmp = smap->num_channels * smap->sample_size * smap->sample_rate / 8;
	au_path->format.wav.fmt.AvgBytesPerSec = LE_DATA_SWAP32(tmp);
  }
  au_path->format.wav.fmt.BlockAlign = LE_DATA_SWAP16(((smap->sample_size * smap->num_channels)+7)/8);
  au_path->format.wav.fmt.BitsPerSample = LE_DATA_SWAP16(smap->sample_size);

  au_path->format.wav.data_id = WAVE_ID_DATA;
  au_path->format.wav.data_size = 0;

  {
	u_int32 size = sizeof(au_path->format.wav);

	if ((err = au_write(au_path, &au_path->format.wav, &size)) != SUCCESS 
		|| size != sizeof(au_path->format.wav)) {
	  return err;
	}
  }

  return SUCCESS;
}

error_code au_wav_close(AUPath au_path)
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
  if ((err = au_seek(au_path, 0)) != SUCCESS)
	if (result == SUCCESS) result = err;

  /* fix the magic field of the header */
  au_path->format.wav.riff_id = WAVE_MAGIC;
  {
	u_int32 tmp = au_path->bytesWritten
	  + (sizeof(au_path->format.wav)
		 - sizeof(au_path->format.wav.riff_id)
		 - sizeof(au_path->format.wav.riff_size));
	au_path->format.wav.riff_size = LE_DATA_SWAP32(tmp);
  }
  au_path->format.wav.data_size = LE_DATA_SWAP32(au_path->bytesWritten);

  /* Rewrite the header */
  {
	u_int32 size = sizeof(au_path->format.wav);

	if ((err = _os_write(au_path->path, &au_path->format.wav, &size)) != SUCCESS 
		|| size != sizeof(au_path->format.wav))
	  if (result == SUCCESS) result = err;
  }

  return result;
}
