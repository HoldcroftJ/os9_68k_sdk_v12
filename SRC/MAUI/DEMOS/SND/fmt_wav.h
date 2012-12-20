
/*******************************************************************************
 *
 * DESCRIPTION:	Header for RIFF-WAVE audio file format 
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */

#ifndef _FMT_WAV_H
#define _FMT_WAV_H


#include <MAUI/maui_snd.h>

/*******************************************************************************
 * 
 * This is the 'typical' format of a Windows .WAV file
 * 
 * 4 bytes "RIFF"
 * 4 bytes basically the size of the WAVE chunk
 * 4 bytes "WAVE" (we only care about WAVE chunks)
 *     Any number of chunks (really sub-chunks) follow, a chunk is defined as:
 *     4 byte identifier
 *     4 bytes size
 *         size number of bytes (chunk specific)
 * 
 * Any program must expect and ignore unknown chunks.
 * 
 * Two chunk types are required, the "fmt " chunk and the "data" chunk,
 * The "fmt " chunk must come before a "data" chunk
 * I am not sure if you can have multiple "fmt "+"data" chunks in the same file
 * 
 * The "fmt " (Format) Chunk:
 * 
 *     WORD wFormatTag;              // Format category
 *                   0001h WAVE_FORMAT_PCM
 *                   etc
 *     WORD wChannels;               // Number of channels
 *     DWORD   dwSamplesPerSec;      //  Sampling rate
 *     DWORD   dwAvgBytesPerSec;     // For buffer estimation
 *     WORD wBlockAlign;             // Data block size
 * 
 * The "data" (Data) Chunk:
 * 
 *     The WAVE_FORMAT_PCM (0001h) is the only format defined in the RIFFMCI.TXT
 *     document and the only format I have seen to date (not that that means
 *     much), so the following applies only to WAVE_FORMAT_PCM format tags.
 * 
 *     wChannels=1 is mono, wChannels=2 is stereo, all other values are undefined
 * 
 *     From the RIFFMCI.TXT file:
 *     ---cut---
 *     The following diagrams show the data packing for a 8-bit mono and stereo
 *     WAVE files:
 * 
 *     Data Packing for 8-Bit Mono PCM:
 *     Sample 1     Sample 2    Sample 3     Sample 4
 *     Channel 0    Channel 0   Channel 0    Channel 0
 * 
 *     Data Packing for 8-Bit Stereo PCM:
 *            Sample 1                 Sample 2
 *     Channel 0    Channel 1   Channel 0    Channel 0
 *      (left)       (right)     (left)       (right)
 * 
 *     The following diagrams show the data packing for 16-bit
 *     mono and stereo WAVE files:
 * 
 *     Data Packing for 16-Bit Mono PCM:
 *            Sample 1                 Sample 2
 *     Channel 0    Channel 0   Channel 0    Channel 0
 *     low-order   high-order   low-order   high-order
 *       byte         byte        byte         byte
 * 
 *     Data Packing for 16-Bit Stereo PCM:
 *                        Sample 1
 *     Channel 0    Channel 0   Channel 1    Channel 1
 *      (left)       (left)      (right)      (right)
 *     low-order   high-order   low-order   high-order
 *       byte         byte        byte         byte
 * 
 *     Data Format of the Samples
 * 
 *     Each sample is contained in an integer i. The size of i is the smallest
 *     number of bytes required to contain the specified sample size. The least
 *     significant byte is stored first. The bits that represent the sample
 *     amplitude are stored in the most significant bits of i, and the remaining
 *     bits are set to zero.
 * 
 *     For example, if the sample size (recorded in nBitsPerSample) is 12 bits,
 *     then each sample is stored in a two-byte integer. The least significant
 *     four bits of the first (least significant) byte is set to zero.Amplitude,
 *     setting waveform
 * 
 *     The data format and maximum and minimums values for PCM waveform samples of
 *     various sizes are as follows:
 * 
 *     Sample Size         Data Format         Maximum Value    Minimum Value
 *     One to eight bits   Unsigned integer    255 (0xFF)       0
 *     Nine or more bits   Signed integer i    Largest positive Most negative
 *                                              value of i       value of i
 * 
 *     For example, the maximum, minimum, and midpoint values
 *     for 8-bit and 16-bit PCM waveform data are as follows:
 * 
 *     Format       Maximum        Minimum Value    Midpoint Value                         Value
 *     8-bit PCM    255 (0xFF)     0                128 (0x80)
 *     16-bit PCM   32767 (0x7FFF) -32768 (0x8000)  0
 *     ---cut---
 */

/* WAVE magic strings */
#define WAVE_ID_RIFF	BE_DATA_SWAP32(0x52494646)	/* "RIFF" */
#define WAVE_ID_WAVE	BE_DATA_SWAP32(0x57415645)	/* "WAVE" */
#define WAVE_ID_FMT		BE_DATA_SWAP32(0x666d7420)	/* "fmt " */
#define WAVE_ID_DATA	BE_DATA_SWAP32(0x64617461)	/* "data" */

#define WAVE_MAGIC WAVE_ID_RIFF
#define WAVE_MAGIC_OPENPATH BE_DATA_SWAP32(0x46464952)

typedef struct  {
  u_int32 ckid;
  u_int32 cksize;
} WAVE_CHUNK;

typedef struct {
  u_int16 FormatTag;      /* Format category */
  u_int16 Channels;       /* Number of channels */
  u_int32 SamplesPerSec;  /* Sampling rate */
  u_int32 AvgBytesPerSec; /* For buffer estimation */
  u_int16 BlockAlign;     /* Data block size for buffer alignment */
  u_int16 BitsPerSample;
} WAVE_FORMAT_FIELDS;

typedef struct {
  u_int32 riff_id;
  u_int32 riff_size;
  u_int32 wave_id;
  u_int32 fmt_id;
  u_int32 fmt_size;
  WAVE_FORMAT_FIELDS fmt;
  u_int32 data_id;
  u_int32 data_size;
} AUWavStruct;

/* FormatTag */
#define WAVE_FORMAT_UNKNOWN			0x0000
#define WAVE_FORMAT_PCM				0x0001 
#define WAVE_FORMAT_ADPCM			0x0002
#define WAVE_FORMAT_IBM_CVSD		0x0005
#define WAVE_FORMAT_ALAW			0x0006
#define WAVE_FORMAT_MULAW			0x0007
#define WAVE_FORMAT_OKI_ADPCM		0x0010
#define WAVE_FORMAT_DVI_ADPCM		0x0011 /* Blocked IMA ADPCM */
#define WAVE_FORMAT_IMA_ADPCM		WAVE_FORMAT_DVI_ADPCM
#define WAVE_FORMAT_DSPGROUP_TRUESPEECH	0x0022
#define WAVE_FORMAT_DIGISTD			0x0015
#define WAVE_FORMAT_DIGIFIX			0x0016
#define WAVE_FORMAT_YAMAHA_ADPCM	0x0020
#define WAVE_FORMAT_SONARC			0x0021
#define WAVE_FORMAT_ECHOSC1			0x0023
#define WAVE_FORMAT_AUDIOFILE_AF36	0x0024
#define WAVE_FORMAT_APTX			0x0025
#define WAVE_FORMAT_AUDIOFILE_AF10	0x0026
#define WAVE_FORMAT_DOLBY_AC2		0x0030
#define WAVE_FORMAT_CS_IMA_ADPCM    0x0039 /* Crystal Semiconductor
											  non-blocked IMA ADPCM */

#define IBM_FORMAT_MULAW			0x0101
#define IBM_FORMAT_ALAW				0x0102
#define IBM_FORMAT_ADPCM			0x0103
#define WAVE_FORMAT_CREATIVE_ADPCM	0x0200


typedef struct wavepath {
  WAVE_FORMAT_FIELDS fmt;
  path_id path;
} *WAVEPath;

/*******************************************************************************
 * Prototypes
 *******************************************************************************/

#endif /* _FMT_WAV_H */
