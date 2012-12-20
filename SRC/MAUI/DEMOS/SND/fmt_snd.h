
/*******************************************************************************
 *
 * DESCRIPTION:	Header for Sun/Next audio file format 
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */

#ifndef _FMT_SND_H
#define _FMT_SND_H

#include <MAUI/maui_snd.h>

#define SND_MAGIC 0x2e736e64
#define SND_MAGIC_OPENPATH 0x646e732e

#define SND_FORMAT_UNSPECIFIED		0	/* unspecified format */
#define SND_FORMAT_MULAW_8			1	/* 8-bit mu-law samples */
#define SND_FORMAT_LINEAR_8			2	/* 8-bit linear samples */
#define SND_FORMAT_LINEAR_16		3	/* 16-bit linear samples */
#define SND_FORMAT_LINEAR_24		4	/* 24-bit linear samples */
#define SND_FORMAT_LINEAR_32		5	/* 32-bit linear samples */
#define SND_FORMAT_FLOAT			6	/* floating-point samples */
#define SND_FORMAT_DOUBLE			7	/* double-precision float samples */
#define SND_FORMAT_INDIRECT			8	/* fragmented sampled data */
#define SND_FORMAT_NESTED			9	/* ? */
#define SND_FORMAT_DSP_CORE			10	/* DSP program */
#define SND_FORMAT_DSP_DATA_8		11	/* 8-bit fixed-point samples */
#define SND_FORMAT_DSP_DATA_16		12	/* 16-bit fixed-point samples */
#define SND_FORMAT_DSP_DATA_24		13	/* 24-bit fixed-point samples */
#define SND_FORMAT_DSP_DATA_32		14	/* 32-bit fixed-point samples */
#define SND_FORMAT_DISPLAY			16	/* non-audio display data */
#define SND_FORMAT_MULAW_SQUELCH	17	/* ? */
#define SND_FORMAT_EMPHASIZED		18	/* 16-bit linear with emphasis */
#define SND_FORMAT_COMPRESSED		19	/* 16-bit linear with compression */
#define SND_FORMAT_COMPRESSED_EMPHASIZED 20	/* A combination of the two above */
#define SND_FORMAT_DSP_COMMANDS		21	/* Music Kit DSP commands */
#define SND_FORMAT_DSP_COMMANDS_SAMPLES 22
#define SND_FORMAT_ADPCM_G721		23
#define SND_FORMAT_ADPCM_G722		24
#define SND_FORMAT_ADPCM_G723_3		25
#define SND_FORMAT_ADPCM_G723_5		26
#define SND_FORMAT_ALAW_8			27

typedef struct {
  u_int32 magic;               /* magic number SND_MAGIC */
  u_int32 dataLocation;        /* offset or pointer to the data */
  u_int32 dataSize;            /* number of bytes of data */
  u_int32 dataFormat;          /* the data format code */
  u_int32 samplingRate;        /* the sampling rate */
  u_int32 channelCount;        /* the number of channels */
  char    info[4];            /* optional text information */
} AUSndStruct;


/*******************************************************************************
 * Prototypes
 *******************************************************************************/

#endif /* _FMT_SND_H */
