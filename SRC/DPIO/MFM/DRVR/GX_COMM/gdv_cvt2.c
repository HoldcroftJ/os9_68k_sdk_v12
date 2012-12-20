/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 2001 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 95/01/05 First versions                                          TJE  |
|  02 99/07/24 Improve color calculations                              srm  |
|  03 01/06/11 Added support for CLUT entries of type GFX_COLOR_A8_RGB BDL  |
|              for the ITVC15 port.                                         |
|  04 01/06/15 Conditionalized some of the A8_RGB support.             gdw  |
`--------------------------------------------------------------------------*/

/****************************************************************************
* DESCRIPTION :
*
*   This file contains functions that convert any valid color type to the color
*   type required by the caller.
*
*/

#include <gdv_priv.h>

/*******************************************************************************
* CONVERT COLORS TO RGB
*******************************************************************************/

#ifdef GDV_INCLUDE_CVT2_RGB

static GFX_COLOR_TYPE cvt2rgb_color_type = GFX_COLOR_NONE;
static void *cvt2rgb_colors = NULL;

/*******************************************************************************
* NAME:     gdv_cvt2_rgb_setup()
*
* USAGE:    Setup for converting any color type to GFX_RGB.
*/
error_code gdv_cvt2_rgb_setup(GFX_COLOR_TYPE color_type, void *colors)
{
  switch (color_type) {
  case GFX_COLOR_RGB:
  case GFX_COLOR_YUV:
  case GFX_COLOR_YCBCR:
  case GFX_COLOR_A8_RGB:
    cvt2rgb_color_type = color_type;
    cvt2rgb_colors = colors;
    break;
  default:
    return EOS_MAUI_BADCOLORTYPE;
  }

  return SUCCESS;
}

/*******************************************************************************
* NAME:     gdv_cvt2_rgb()
*
* USAGE:    Convert the specified color to RGB. The color_index is used to
*           specify which entry in "colors" (see setup function) should be
*           converted. The first entry in "colors" is 0.
*/
GFX_RGB gdv_cvt2_rgb(u_int16 color_index)
{
    int32 r, g, b;

    /* Switch based on the (source) color type */
    switch (cvt2rgb_color_type) {

        /* RGB to RGB */
        case GFX_COLOR_RGB: {
            GFX_RGB *cvt2rgb_colors_rgb = cvt2rgb_colors;
            return cvt2rgb_colors_rgb[color_index] | 0xFF000000;
        }

        /* RGB with Alpha Byte to RGB */
        case GFX_COLOR_A8_RGB: {
            GFX_A8_RGB *cvt2rgb_colors_a8_rgb = cvt2rgb_colors;
            return cvt2rgb_colors_a8_rgb[color_index];
        }

        /* YUV to RGB */
        case GFX_COLOR_YUV: {
            GFX_YUV *cvt2rgb_colors_yuv = cvt2rgb_colors;
            u_int32 y, ry;
            int8 u, v;
            GFX_YUV yuv_color;

            yuv_color = cvt2rgb_colors_yuv[color_index];
            y = (u_char)((yuv_color >> 16) & 0xff);
            u = (char)((yuv_color >> 8) & 0xff);
            v = (char)(yuv_color & 0xff);

            /* Color Spaces FAQ - David Bourgin
                 y  u      v
               r 1  0      1.14
               g 1 -0.396 -0.581
               b 1  2.029  0
               Factor by 1<<15 */
            ry = 32768*y;
            /* rgb should be unsigned but the calculation is not
               exact, we have to round for signed values */
            r = (ry) +           + (37356*v);
            r = (r < 0) ? (r-(1<<14))/(1<<15) : (r+(1<<14))/(1<<15); /* signed rounding */
            g = (ry) - (12976*u) - (19038*v);
            g = (g < 0) ? (g-(1<<14))/(1<<15) : (g+(1<<14))/(1<<15); /* signed rounding */
            b = (ry) + (66486*u);
            b = (b < 0) ? (b-(1<<14))/(1<<15) : (b+(1<<14))/(1<<15); /* signed rounding */
            break;
        }

        /* YCBCR to RGB */
        case GFX_COLOR_YCBCR: {
            GFX_YCBCR *cvt2rgb_colors_ycbcr = cvt2rgb_colors;
            int32 y, cb, cr;
            GFX_YCBCR ycbcr_color;

            ycbcr_color = cvt2rgb_colors_ycbcr[color_index];
            /* cast to (char) for sign extension */
            y = (ycbcr_color >> 16) & 0xff;
            cb = (char)((ycbcr_color >> 8) & 0xff);
            cr = (char)(ycbcr_color & 0xff);

            /* colorspace-faq -- FREQUENTLY ASKED QUESTIONS ABOUT GAMMA AND COLOR
                       y           cb          cr
                    r 0.00456621  0           0.00625893
                    g 0.00456621 -0.00153632 -0.00318811
                    b 0.00456621  0.00791071  0
               Using an additional factor of 1<<16 with their 256
               */
            {
              int32 ry = 299*y; /* calcuate this one once */
              r = ((ry            + (410*cr)) * 256 + (1<<15)) / (1<<16); /* unsigned rounding */
              g = ((ry - (101*cb) - (209*cr)) * 256 + (1<<15)) / (1<<16); /* unsigned rounding */
              b = ((ry + (518*cb)           ) * 256 + (1<<15)) / (1<<16); /* unsigned rounding */
            }
            break;
        }

        default:
          return EOS_MAUI_BADCOLORTYPE; /* We should never get here */
    }

    /* Return the color */
    if (r < 0) r = 0; else if (r > 255) r = 255;
    if (g < 0) g = 0; else if (g > 255) g = 255;
    if (b < 0) b = 0; else if (b > 255) b = 255;
#ifndef SUPPORT_A8_RGB
    return ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
#else
    return (0xFF000000 | (r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
#endif
}

#endif /* GDV_INCLUDE_CVT2_RGB */


/*******************************************************************************
* CONVERT COLORS TO YUV
*******************************************************************************/

#ifdef GDV_INCLUDE_CVT2_YUV

static GFX_COLOR_TYPE cvt2yuv_color_type = GFX_COLOR_NONE;
static void *cvt2yuv_colors = NULL;

/*******************************************************************************
* NAME:     gdv_cvt2_yuv_setup()
*
* USAGE:    Setup for converting any color type to GFX_YUV.
*/
error_code gdv_cvt2_yuv_setup(GFX_COLOR_TYPE color_type, void *colors)
{
    switch (color_type) {
        case GFX_COLOR_RGB:
        case GFX_COLOR_A8_RGB:
        case GFX_COLOR_YUV:
            cvt2yuv_color_type = color_type;
            cvt2yuv_colors = colors;
            break;
        default:
            return EOS_MAUI_BADCOLORTYPE;
    }

    return SUCCESS;
}

/*******************************************************************************
* NAME:     gdv_cvt2_yuv()
*
* USAGE:    Convert the specified color to YUV. The color_index is used to
*           specify which entry in "colors" (see setup function) should be
*           converted. The first entry in "colors" is 0.
*/
GFX_YUV gdv_cvt2_yuv(u_int16 color_index)
{
  int32 y, u, v;

    /* Switch based on the (source) color type */
    switch (cvt2yuv_color_type) {

        /* RGB to YUV */
        case GFX_COLOR_RGB:
        case GFX_COLOR_A8_RGB: {
            GFX_RGB *cvt2yuv_colors_rgb = cvt2yuv_colors;
            int32 r, g, b;
            GFX_RGB rgb_color;

            rgb_color = cvt2yuv_colors_rgb[color_index];
            r = (u_char)((rgb_color >> 16) & 0xff);
            g = (u_char)((rgb_color >> 8) & 0xff);
            b = (u_char)(rgb_color & 0xff);

            /* Color Spaces FAQ - David Bourgin
                  r      g      b
               y  0.299  0.587  0.114
               u -0.147 -0.289  0.436
               v  0.615 -0.515 -0.1
               Factor by 1<<15 */
            y = (( 9798*r) + (19235*g) + ( 3736*b) + (1<<14)) / (1<<15); /* unsigned rounding */
            u = ((-4817*r) - ( 9470*g) + (14287*b));
            u = (u < 0) ? (u-(1<<14))/(1<<15) : (u+(1<<14))/(1<<15); /* signed rounding */
            v = ((20152*r) - (16876*g) - ( 3277*b));
            v = (v < 0) ? (v-(1<<14))/(1<<15) : (v+(1<<14))/(1<<15); /* signed rounding */
            break;
        }

        /* YUV to YUV */
        case GFX_COLOR_YUV: {
            GFX_YUV *cvt2yuv_colors_yuv = cvt2yuv_colors;
            return cvt2yuv_colors_yuv[color_index];
        }

        default:
            return EOS_MAUI_BADCOLORTYPE; /* We should never get here */
    }

    /* Return the color */
    if (y < 0) y = 0; else if (y > 255) y = 255;
    if (u < -128) u = -128; else if (u > 127) u = 127;
    if (v < -128) v = -128; else if (v > 127) v = 127;
    return ((y & 0xff) << 16) | ((u & 0xff) << 8) | (v & 0xff);
}

#endif /* GDV_INCLUDE_CVT2_YUV */


/*******************************************************************************
* CONVERT COLORS TO YCBCR
*******************************************************************************/

#ifdef GDV_INCLUDE_CVT2_YCBCR

static GFX_COLOR_TYPE cvt2ycbcr_color_type = GFX_COLOR_NONE;
static void *cvt2ycbcr_colors = NULL;

/*******************************************************************************
* NAME:     gdv_cvt2_ycbcr_setup()
*
* USAGE:    Setup for converting any color type to GFX_YCBCR.
*/
error_code gdv_cvt2_ycbcr_setup(GFX_COLOR_TYPE color_type, void *colors)
{
    switch (color_type) {
        case GFX_COLOR_RGB:
        case GFX_COLOR_A8_RGB:
        case GFX_COLOR_YCBCR:
            cvt2ycbcr_color_type = color_type;
            cvt2ycbcr_colors = colors;
            break;
        default:
            return EOS_MAUI_BADCOLORTYPE;
    }

    return SUCCESS;
}

/*******************************************************************************
* NAME:     gdv_cvt2_ycbcr()
*
* USAGE:    Convert the specified color to YCBCR. The color_index is used to
*           specify which entry in "colors" (see setup function) should be
*           converted. The first entry in "colors" is 0.
*/
GFX_YCBCR gdv_cvt2_ycbcr(u_int16 color_index)
{
    int32 y, cb, cr;

    /* Switch based on the (source) color type */
    switch (cvt2ycbcr_color_type) {

        /* RGB to YCBCR */
        case GFX_COLOR_RGB:
        case GFX_COLOR_A8_RGB: {
            GFX_RGB *cvt2ycbcr_colors_rgb = cvt2ycbcr_colors;
            int32 r, g, b;
            GFX_RGB rgb_color;

            rgb_color = cvt2ycbcr_colors_rgb[color_index];
            r = (rgb_color >> 16) & 0xff;
            g = (rgb_color >> 8) & 0xff;
            b = rgb_color & 0xff;

            /* colorspace-faq -- FREQUENTLY ASKED QUESTIONS ABOUT GAMMA AND COLOR
                     r       g       b
               y   65.481 128.553  24.966
               cb -37.797 -74.203 112
               cr 112     -93.786 -18.214
               Using an additional factor of 1<<8 with their 256 */
            y =  (((16763*r) + (32910*g) + ( 6391*b)) / 256 + (1<<7)) / (1<<8); /* unsigned rounding */
            cb = ((-9676*r) - (18996*g) + (28672*b)) / 256;
            cb = (cb < 0) ? (cb-(1<<7))/(1<<8) : (cb+(1<<7))/(1<<8); /* signed rounding */
            cr = ((28672*r) - (24009*g) - ( 4663*b)) / 256;
            cr = (cr < 0) ? (cr-(1<<7))/(1<<8) : (cr+(1<<7))/(1<<8); /* signed rounding */
            break;
        }

        /* YCBCR to YCBCR */
        case GFX_COLOR_YCBCR: {
            GFX_YCBCR *cvt2ycbcr_colors_ycbcr = cvt2ycbcr_colors;
            return cvt2ycbcr_colors_ycbcr[color_index];
        }

        default:
          return EOS_MAUI_BADCOLORTYPE; /* We should never get here */
    }

    /* Return the color */
    if (y < 0) y = 0; else if (y > 255) y = 255;
    if (cb < -128) cb = -128; else if (cb > 127) cb = 127;
    if (cr < -128) cr = -128; else if (cr > 127) cr = 127;
    return ((y & 0xff) << 16) | ((cb & 0xff) << 8) | (cr & 0xff);
}

#endif /* GDV_INCLUDE_CVT2_YCBCR */
