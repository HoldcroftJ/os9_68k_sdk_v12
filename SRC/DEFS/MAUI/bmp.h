/*******************************************************************************
 *
 * DESCRIPTION:	.bmp file definition
 *
 * COPYRIGHT:	Copyright 2000 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 *
 */

#ifndef _BMP_H
#define _BMP_H

/* .bmp files have the following format:

	bitmap file header
	bitmap info header
	rgb color table
	bitmap image data
*/

typedef struct {
	u_int16	type;			/* always "BM" */
	u_int32	filesize;		/* size of the file in bytes */
	u_int16	reserved1;		/* always 0000 */
	u_int16 reserved2;		/* always 0000 as well */
	u_int32	offset;			/* how many bytes to bitmap data */
	
	u_int32	infosize;		/* size of bitmap info structure */
	u_int32	width;			/* width in pixels */
	u_int32	height;			/* height in pixels */
	u_int16 planes;			/* planes, always set to 1 */
	u_int16	bitcount;		/* bits per pixel (1, 4, 8, or 24) */
	u_int32	compression;	/* type of compression BI_RGB, BI_RLE8, RLE4 */
	u_int32	imagesize;		/* size of image data (0 if uncompressed) */
	u_int32	xpixels;		/* pixels per meter */
	u_int32 ypixels;		/* pixels per meter */
	u_int32	colors;			/* colors used */
	u_int32 impcolors;		/* # of important colors */
} BMPHEADER	;

#define BMPHEADERSIZE 54

typedef struct {
	u_int8	blue;
	u_int8	green;
	u_int8	red;
	u_int8	nada;
} RGB;

#endif /* _BMP_H */
