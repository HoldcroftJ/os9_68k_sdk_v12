/* Copyright 1988 by Microware Systems Corporation */

/*----------------------------------------------------------------------\
!																		!
!	Name: Iff.h															!
!																		!
!	Function: Definitions for readers and writers of iff files.			!
!																		!
!	Revision History:													!
!	 #	Reason for Change								By  Date		!
!  ---- -----------------------------------------------	---	--------	!
!	1	Creat							                tod	88/06/22	!
!	2	Mods for AIFF and IMAG							srm	88/07/27	!
!	3	Mods for Version 0.99							srm	89/01/09	!
!	4	Added check for multiple include of <types.h>	msm 90/07/10    !
!   5   Cut IFF_SKIP_SIZE to reduce stack requirements  srm 00/05/05    !
!   6 	change iff_write_date prototype to				mgh 01/03/15	!
|         iff_write_data                                                !
\----------------------------------------------------------------------*/

#ifndef _IFF_H
#define _IFF_H

#ifndef _types
#include <types.h>
#endif

/*****************************************************************************
*
* IFF Types
*
*/
typedef	char				IFF_8;
typedef	short				IFF_16;
typedef	int					IFF_32;
typedef double				extended;

typedef	unsigned char		IFF_U_8;
typedef	unsigned short		IFF_U_16;
typedef	unsigned int		IFF_U_32;

/************* this typedef should move to iff_imag.h ***************/
typedef struct {
	IFF_U_8				r, g, b;
} IFF_COLOR;
#define SIZEOF_IFF_COLOR (sizeof(IFF_U_8)*3)

/************* this typedef should move to iff_imag.h ***************/
typedef struct {
	IFF_U_8				y, u, v;
} IFF_YUV;
#define SIZEOF_IFF_YUV (sizeof(IFF_U_8)*3)

/*****************************************************************************
*
* IFF Macros
*
*/
#define IFF_MAKEID(a,b,c,d) ( (int)(a)<<24L | (int)(b)<<16L | (c)<<8 | (d) )

/*****************************************************************************
*
* IFF Constants
*
*/
#define IFF_SKIP_SIZE		256			/* buffer size for skipping data */
#define IFF_CAT_UNKNOWN		0			/* CAT type (id) is unknown */
#define IFF_SIZE_UNKNOWN	0x80000000L	/* size is unknown (for writing) */
#define IFF_READ			0			/* file is open for reading */
#define IFF_WRITE			1			/* file is open for writing */


/*****************************************************************************
*
* Structure for iff information pertaining to a path. This structure is 
* created by the "iff_open()" function and is deallocated by the "iff_close()"
* function.
*
*/
typedef struct {
	short path;							/* path number for file */
	short mode;							/* read/write mode for this path */
	short nextformread;					/* next FORM has already been read */
	int filepos;						/* current file position */
	short filetype;						/* file type (RBF or PIPE) */
	int catid;							/* CAT id */
	int catpos;							/* file position for size of CAT */
	int catsize;						/* original size of CAT */
	int catlen;							/* current length of CAT */
	int formid;							/* FORM id */
	int formpos;						/* file position for size of FORM */
	int formsize;						/* original size of FORM */
	int formlen;						/* current length of FORM */
	int datapos;						/* file position for size of data */
	int datasize;						/* original size of data */
	int datalen;						/* current length of data */
	int framepos;						/* file position of SampleFrame */
	double framefctr;					/* SampleFrame = datasize / framefctr */
} IFF_CONTEXT;


#ifdef __cplusplus
extern "C" {
#endif

/* Prototypes */
int iff_close(IFF_CONTEXT *context);
IFF_CONTEXT *iff_open(short path, short rwmode, int catid);
int iff_read_data(IFF_CONTEXT *context, char *buffer, int length);
int iff_read_what_next(IFF_CONTEXT *context);
int iff_seek(IFF_CONTEXT *context, int position, int place);
int iff_skip_data(IFF_CONTEXT *context, int length);
int iff_write_data(IFF_CONTEXT *context, char *buffer, int length);
int iff_write_end(IFF_CONTEXT *context);

#ifdef __cplusplus
}
#endif

	
#endif /*_IFF_H*/

