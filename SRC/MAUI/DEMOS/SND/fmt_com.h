
/*******************************************************************************
 *
 * DESCRIPTION:	Header for audio file format API
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */

#ifndef _FMT_COM_H
#define _FMT_COM_H

#include <MAUI/maui_snd.h>
#include "fmt_snd.h"
#include "fmt_wav.h"
#include <const.h>
#include <module.h>

typedef enum {
  AU_FILE_TYPE_SND  = 0,
  AU_FILE_TYPE_WAVE = 1
} AUFileType;

typedef enum {
  AU_ACCESS_MODE_READ  = 0,
  AU_ACCESS_MODE_WRITE = 1
} AUAccessMode;

typedef struct aupath {
  AUFileType type;    /* Indicates file type */
  AUAccessMode mode;  /* Indicates read if opened or
                         write if created */
  mh_com *modptr;     /* Points to head of module, NULL if a file */
  u_int32 datptr;     /* Tracks current "file position" in bytes */
  u_int32 datend;     /* Only used in module mode to mark eof in bytes */
  u_int32 bytesWritten; /* Track data bytes written */
  path_id path;       /* Contains path to file in modptr is NULL */
  union {
	AUWavStruct wav;
	AUSndStruct snd;
  } format;
} *AUPath;


#define DO_SWAP32(VAL) (((((u_int32)(VAL)) & 0xff000000) >> 24) | \
					 ((((u_int32)(VAL)) & 0x00ff0000) >> 8) |  \
					 ((((u_int32)(VAL)) & 0x0000ff00) << 8) |  \
					 ((((u_int32)(VAL)) & 0x000000ff) << 24))
#define DO_SWAP16(VAL) (((((u_int16)(VAL)) & 0xff00) >> 8) |      \
					 ((((u_int16)(VAL)) & 0x00ff) << 8))
#ifdef _BIG_END
#define BE_DATA_SWAP32(VAL) ((u_int32)(VAL))
#define BE_DATA_SWAP16(VAL) ((u_int16)(VAL))
#define LE_DATA_SWAP32(VAL) DO_SWAP32(VAL)
#define LE_DATA_SWAP16(VAL) DO_SWAP16(VAL)
#else
#define BE_DATA_SWAP32(VAL) DO_SWAP32(VAL)
#define BE_DATA_SWAP16(VAL) DO_SWAP16(VAL)
#define LE_DATA_SWAP32(VAL) ((u_int32)(VAL))
#define LE_DATA_SWAP16(VAL) ((u_int16)(VAL))
#endif

/*******************************************************************************
 * Prototypes
 *******************************************************************************/
error_code au_open(AUPath *au_path, SND_SMAP *smap, char *name);
error_code au_create(AUPath *au_path, SND_SMAP *smap, AUFileType
						  ftype, char* name, u_int32 mode, u_int32
						  perm, u_int32 size);
error_code au_write_data(AUPath au_path, void *buf, u_int32 *count);
error_code au_write(AUPath au_path, void *buf, u_int32 *count);
error_code au_read(AUPath au_path, void *buf, u_int32 *count);
error_code au_ptrread(AUPath au_path, void **buf, u_int32 *count);
error_code au_seek(AUPath au_path, u_int32 offset);
error_code au_size(AUPath au_path, u_int32 size);
error_code au_close(AUPath au_path, u_int32 size);

/* SND Prototypes */
error_code au_snd_init_smap(AUPath au_path, SND_SMAP *smap);
error_code au_snd_create(AUPath au_path, SND_SMAP *smap);
error_code au_snd_close(AUPath au_path);

/* WAVE Prototypes */
error_code au_wav_init_smap(AUPath au_path, SND_SMAP *smap);
error_code au_wav_create(AUPath au_path, SND_SMAP *smap);
error_code au_wav_close(AUPath au_path);
#endif /* _FMT_COM_H */
