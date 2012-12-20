/* Copyright 1988 by Microware Systems Corporation */

/*----------------------------------------------------------------------\
!																		!
!	Name: Iff_IMAG.h													!
!																		!
!	Function: Definitions for readers and writers of iff IMAG files.	!
!																		!
!	Revision History:													!
!	 #	Reason for Change								By  Date		!
!  ---- -----------------------------------------------	---	--------	!
!	1	Creat							                srm	88/07/27	!
!	2	Mods for Version 0.99							srm	89/01/09	!
\----------------------------------------------------------------------*/

#ifndef _IFF_IMAG_H
#define _IFF_IMAG_H

/*****************************************************************************
*
* IFF IMAG Constants
*
*/
#define IFF_ID_IMAG IFF_MAKEID('I','M','A','G')	/* CD-I image */
#define IFF_ID_IHDR IFF_MAKEID('I','H','D','R')	/* standard header */
#define IFF_ID_IPAR IFF_MAKEID('I','P','A','R')	/* optional image parameters */
#define IFF_ID_PLTE IFF_MAKEID('P','L','T','E')	/* optional palette */
#define IFF_ID_YUVS IFF_MAKEID('Y','U','V','S')	/* optional DYUV start values */
#define IFF_ID_USER IFF_MAKEID('U','S','E','R')	/* optional user-defined data */
#define IFF_ID_IDAT IFF_MAKEID('I','D','A','T')	/* actual image data */

/*****************************************************************************
*
* Image Types Definitions
*
*/

/* Definitions for values used in the field ihdr_model */

#define	IFF_MDL_NONE	0	/* no known color model */
#define	IFF_MDL_RGB888	1	/* red, green, blue - 8 bits per color */
#define	IFF_MDL_RGB555	2	/* Green Book absolute RGB */
#define	IFF_MDL_DYUV	3	/* Green Book Delta YUV */
#define	IFF_MDL_CLUT8	4	/* Green Book 8 bit CLUT */
#define	IFF_MDL_CLUT7	5	/* Green Book 7 bit CLUT */
#define	IFF_MDL_CLUT4	6	/* Green Book 4 bit CLUT */
#define	IFF_MDL_CLUT3	7	/* Green Book 3 bit CLUT */
#define	IFF_MDL_RL7		8	/* Green Book runlength coded 7 bit CLUT */
#define	IFF_MDL_RL3		9	/* Green Book runlength coded 3 bit CLUT */
#define	IFF_MDL_PLTE	10	/* color palette only */

/* NOTE - For the following values of the field ihdr_model:
	IFF_MDL_RGB888
	IFF_MDL_RGB555
	IFF_MDL_DYUV
	IFF_MDL_CLUT8
	IFF_MDL_CLUT7
	IFF_MDL_CLUT4
	IFF_MDL_CLUT3
the following relationships apply:
	image size = ihdr_height * ihdr_line_size
	ihdr_line_size = (ihdr_width * ihdr_depth)/8 + required padding
*/

/* Definitions for values used in the field ihdr_dyuv_kind */

#define	IFF_DYUV_ONE		0			/* one start value for all scan lines */
#define	IFF_DYUV_EACH		1			/* start value for each scan line */

/*****************************************************************************
*
* Iff Structure for IMAG (video) type FORM's and chunks
*
*/
typedef struct {		/* Image Header */
	IFF_U_16	ihdr_width;				/* logical width of image (number 
											of significant pixels in each 
											scan line) */
	IFF_U_16	ihdr_line_size;			/* physical width of image (number of 
											bytes in each scan line, including 
											any data required at the end of 
											each scan line for padding.  This 
											field is not used when ihdr_model 
											= IFF_MDL_RL7 or IFF_MDL_RL3. When 
											ihdr_model = IFF_MDL_RGB555, this 
											field defines the size of one scan 
											line of the upper or lower portion 
											of the pixel data, but not he size 
											of them both together. */
	IFF_U_16	ihdr_height;			/* logical height of image (number of 
											scan lines) */
	IFF_U_16	ihdr_model;				/* image model (coding method) */
	IFF_U_16	ihdr_depth;				/* physical size of pixel (number of 
											bits per pixel used in storing 
											image data) */
	IFF_U_8		ihdr_dyuv_kind;			/* if ihdr_model = IFF_MDL_DYUV,
											indicates whether there is one
											DYUV start value for all scan
											lines (in ihdr_dyuv_start), or
											whether each scan line has its
											own start value in the YUVS chunk
											which follows */
	IFF_YUV		ihdr_dyuv_start;		/* start values for DYUV image if 
											ihdr_dyuv_kind is IFF_DYUV_ONE */
} IFF_IHDR;
#define SIZEOF_IFF_IHDR (sizeof(IFF_U_16)*5+sizeof(IFF_U_8)+sizeof(IFF_YUV))

typedef struct {	/* Image Parameters */
	IFF_16		ipar_x_off, ipar_y_off;		/* offset of origin in source image
												[0 <= ipar_x_off <= ipar_x_page]
												[0 <= ipar_y_off <= ipar_y_page] */
	IFF_U_16	ipar_x_ratio, ipar_y_ratio;	/* aspect ratio of pixels in
												source image; ratio is 
												ipar_y_ratio/ipar_x_ratio.
												E.g. 1.333:1 is ipar_y_ratio=4,
												ipar_x_ratio=3 */
	IFF_U_16	ipar_x_page, ipar_y_page;	/* size of source image */
	IFF_U_16	ipar_x_grab, ipar_y_grab;	/* loc of hot spot within image */
	IFF_COLOR	ipar_trans;					/* transparent color */
	IFF_COLOR	ipar_mask;					/* mask color */
} IFF_IPAR;
#define SIZEOF_IFF_IPAR (sizeof(IFF_16)*2+sizeof(IFF_U_16)*6+sizeof(IFF_COLOR)*2)

typedef struct {		/* Palette */
	IFF_U_16	plte_offset;				/* offset (entry number) from
												start of CLUT where this
												palette is to be loaded */
	IFF_U_16	plte_count;					/* number of entries in this */
} IFF_PLTE;
#define SIZEOF_IFF_PLTE (sizeof(IFF_16)*2)

typedef struct {		/* IMAG FORM */
	IFF_IHDR ihdr;						/* Image header */
	IFF_IPAR ipar;						/* Image Parameters */
	IFF_PLTE plte;						/* Palette */
	u_char	*plte_data;					/* pointer to palette data */
	IFF_YUV *yuv_data;					/* YUV Start Values */
	char	*user_data;					/* User defined data */
} IFF_IMAG;

/* NOTE - IFF_IMAG.user currently never returns any data, but will write a 
			null terminated string into the IFF file */

#ifdef __cplusplus
extern "C" {
#endif

/* Prototypes */
int iff_read_imag(IFF_CONTEXT *context, IFF_IMAG *imag);
int iff_write_imag(IFF_CONTEXT *context, IFF_IMAG *imag, int datasize);

#ifdef __cplusplus
}
#endif

#endif /*_IFF_H*/
