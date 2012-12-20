/* Copyright 1988 by Microware Systems Corporation */

/*----------------------------------------------------------------------\
!																		!
!	Name: Iff_AIFF.h													!
!																		!
!	Function: Definitions for readers and writers of iff AIFF files.	!
!																		!
!	Revision History:													!
!	 #	Reason for Change								By  Date		!
!  ---- -----------------------------------------------	---	--------	!
!	1	Creat							                srm	88/07/27	!
!	2	Mods for Version 0.99							srm	89/01/09	!
\----------------------------------------------------------------------*/

#ifndef _IFF_AIFF_H
#define _IFF_AIFF_H

/*****************************************************************************
*
* IFF AIFF Constants
*
*/

#define IFF_ID_AIFF IFF_MAKEID('A','I','F','F')	/* id for AIFF type FORM */
#define IFF_ID_COMM IFF_MAKEID('C','O','M','M')	/* common chunk */
#define IFF_ID_SSND IFF_MAKEID('S','S','N','D')	/* PCM sound chunk */
#define IFF_ID_APCM IFF_MAKEID('A','P','C','M')	/* ADPCM sound chunk */
#define IFF_ID_MARK IFF_MAKEID('M','A','R','K')	/* marker chunk */
#define IFF_ID_INST IFF_MAKEID('I','N','S','T')	/* instrument chunk */
#define IFF_ID_MIDI IFF_MAKEID('M','I','D','I')	/* MIDI chunk */
#define IFF_ID_AESD IFF_MAKEID('A','E','S','D')	/* audio recording chunk */
#define IFF_ID_APPL IFF_MAKEID('A','P','P','L')	/* application specific chunk */
#define IFF_ID_COMT IFF_MAKEID('C','O','M','T')	/* comment chunk */
#define IFF_ID_NAME IFF_MAKEID('N','A','M','E')	/* name chunk */
#define IFF_ID_AUTH IFF_MAKEID('A','U','T','H')	/* author chunk */
#define IFF_ID_ANNO IFF_MAKEID('A','N','N','O')	/* annotation chunk */
#define IFF_ID_C 	IFF_MAKEID('(','C',')',' ')	/* copyright chunk */

/*****************************************************************************
*
* Audio Type Definitions
*
*/

#define	IFF_FRQ_44_1	44100
#define	IFF_FRQ_37_8	37800
#define	IFF_FRQ_18_9	18900

#ifndef	IFF_AUD_MONO
#define	IFF_AUD_MONO		1
#define	IFF_AUD_STEREO		2
#endif

/*****************************************************************************
*
* Iff Structure for AIFF (audio) type FORM's.
*
*/

typedef struct {
	IFF_U_32	offset;
	IFF_U_32	blockSize;
} blockAlign;
	
typedef struct {
	IFF_U_16	numChannels;
	IFF_U_32	numSampleFrames;
	IFF_U_16	sampleSize;
	extended	sampleRate;
	IFF_U_32	sampleType;
	blockAlign	blkAlgn;
} IFF_AIFF;

#ifdef __cplusplus
extern "C" {
#endif

/* Prototypes */
int iff_read_aiff(IFF_CONTEXT *context, IFF_AIFF *aiff);
int iff_write_aiff(IFF_CONTEXT *context, IFF_AIFF aiff, int datasize);

#ifdef __cplusplus
}
#endif

#endif 
