
/*----------------------------------------------------------------------\
!																		!
!	Name: R888toDYUV													!
!																		!
!	Function: Convert RGB888 images to DYUV								!
!																		!
!	Revision History:													!
!	 #	Reason for Change								By  Date		!
!  ---- -----------------------------------------------	---	--------	!
!	1	Created											tje	87/10/03	!
!	2	Update to IFF 0.99, and changed opt/arg pars	srm	89/01/13	!
!	3	******** Release 0.99D **********								!
\----------------------------------------------------------------------*/

@_sysedit:	equ	3

/****************************************************************************
*																			*
*              Copyright 1988 by Microware Systems Corporation              *
*                         Reproduced Under License.                         *
*																			*
* This  source  code is  the proprietary confidential property of Microware	*
* Systems Corporation, and is  provided  to licensee  for documentation and	*
* educational purposes only. Reproduction, publication, or  distribution in	*
* any form to any party other than the licensee is strictly prohibited.		*
*																			*
****************************************************************************/

#include <iff.h>
#include <iff_imag.h>
#include <stdio.h>
#include <errno.h>
#include <modes.h>

char *malloc();

IFF_IMAG InImag, OutImag;
char *usage = "\
Function: Convert RGB888 images to DYUV.\n\
Syntax:   R888toDYUV\n\
Where:    Input is standard input.\n\
          Output is standard output.\n\
Options:  no options.\n\n";

/*****************************************************************************
*
* Mainline
*
*/
main(argc,argv)
int argc;
char **argv;
{
	register IFF_CONTEXT *incontext,*outcontext;
	register int bufsize, outbufsize, lastsize = 0, width, height;
	register u_char *inbuf, *outbuf, *uvbuf;

	parsopts (argc, argv);

	/* open IFF contexts */
	if ((incontext=iff_open(fileno(stdin),IFF_READ,IFF_ID_IMAG)) == NULL)
		exit(_errmsg(errno,"Can't open IFF context for input.\n"));
	if ((outcontext=iff_open(fileno(stdout),IFF_WRITE,IFF_ID_IMAG)) == NULL)
		exit(_errmsg(errno,"Can't open IFF context for output.\n"));

	/* read the next FORM */
	while ((bufsize = iff_read_what_next(incontext))) {
		if (bufsize == -1)
			exit(_errmsg(errno,"Can't read start of next FORM.\n"));
		if (bufsize != IFF_ID_IMAG)
			exit(_errmsg(errno,"FORM type must be IMAG.\n"));

 		/* read the IMAG header */
		if ((bufsize=iff_read_imag(incontext,&InImag)) == -1)
			exit(_errmsg(errno,"Error reading IMAG header.\n"));
		if (InImag.ihdr.ihdr_model != IFF_MDL_RGB888 
		  || InImag.ihdr.ihdr_depth != 24)
			exit(_errmsg(errno, "%sMust be an RGB888 with pixel depth of 24.\n", usage));

		/* build a DYUV IMAG header */
		width = InImag.ihdr.ihdr_width;
		height = InImag.ihdr.ihdr_height;
		memcpy (&OutImag, &InImag, sizeof IFF_IMAG);
		OutImag.ihdr.ihdr_model = IFF_MDL_DYUV;
		OutImag.ihdr.ihdr_depth = 8;
		outbufsize = (OutImag.ihdr.ihdr_line_size = 
			width + (3 - ((width - 1) % 4))) * height;
		OutImag.ihdr.ihdr_dyuv_kind = IFF_DYUV_ONE;
		OutImag.ihdr.ihdr_dyuv_start.y = 0x10;
		OutImag.ihdr.ihdr_dyuv_start.u = 
			OutImag.ihdr.ihdr_dyuv_start.v = 0x80;

		/* get memory to put the image in */
		if (lastsize < bufsize) {	/* bigger then the last buffer */
			if (lastsize)			/* was there a last buffer */
				free (inbuf);	/* free the smaller buffer */
			if ((inbuf = (u_char *)malloc(bufsize + outbufsize<<1)) == NULL)
				exit(_errmsg(errno, "Can't allocate working buffers.\n"));
			uvbuf = (outbuf = inbuf + bufsize) + outbufsize;
			lastsize = bufsize;	/* reset the last buffer */
		}
		/* read the image */
		if (iff_read_data(incontext,inbuf,bufsize) != bufsize)
			exit(_errmsg(errno,"Error reading IMAG data.\n"));

		/* convert the image */
		r888todyuv(width,height,inbuf,outbuf,uvbuf);

		/* write it */
		if (iff_write_imag(outcontext,&OutImag,outbufsize) == -1)
			exit(_errmsg(errno,"Can't write IMAG header.\n"));
		if (iff_write_data(outcontext,outbuf,outbufsize) == -1)
			exit(_errmsg(errno,"Can't write IMAG data.\n"));
		if (iff_write_end(outcontext) == -1)
			exit(_errmsg(errno,"Can't write IMAG end.\n"));
	}
	if (iff_close(incontext) == -1)
		exit(_errmsg(errno,"Can't close input context.\n"));
	if (iff_close(outcontext) == -1)
		exit(_errmsg(errno,"Can't close output context.\n"));
	if (lastsize)			/* was there a last buffer */
		free (inbuf);	/* free the smaller buffer */
}

/*****************************************************************************
*
* R888toDYUV	Convert an RGB888 image to DYUV.
*
* Passed:		Width = Image width in pixels.
*				Height = Image height in pixels.
*				InBuf = Input buffer.
*				OutBuf = Output buffer.
*				UVBuf = Buffer for temporary UV values.
* Returned:		No arguments
*
*/
r888todyuv(width,height,inbuf,outbuf,uvbuf)
register int width, height;
register u_char *inbuf,*outbuf,*uvbuf;
{
	register int temp, pad, widthx3;

	/* convert the image one line at a time */
	ini_yuv();
	inidelta();
	pad = 3 - ((width - 1) % 4);
	widthx3 = (width<<1)+width;	/* inbuf += width*3; */
	while (height--) {
		rgbtoyuv(inbuf,outbuf,uvbuf,width);
		todelta(outbuf,uvbuf,width);
		inbuf += widthx3;
		outbuf += width;
		for (temp = pad; temp--; *outbuf++ = 0);	/* add pad bytes */
	}
}

/*****************************************************************************
*
* ParseOpt		Option parser.
*
* Passed:		Ptr = Pointer to the options.
* Returned:		No arguments
*
*/
parsopts (argc, argv)
register argc;
register char **argv;
{
	register char *p;
	register int pcnt = 0;

	/* Syntax:   R888toDYUV */

	while (--argc>0) {
		if (*(p = *++argv) == '-')
			while (*++p > ' ') switch (*p|0x20) {
				case '?':
					fputs (usage, stderr);
					exit (0);
				default:
					fputs (usage, stderr);
					exit(_errmsg(1, "'%c' illegal option\n", *p));
			}
		else {
			fputs (usage, stderr);
			exit(_errmsg(1, "'%s' illegal argument\n", p));
		}
	}
}
