/******************************************************************************
*
* DESCRIPTION:	Sfont demo program for MAUI
*
* CREATED ON:	02/06/96
*
* COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
*/
#include <MAUI/mauidemo.h>
#include <MAUI/maui.h>
#include <MAUI/maui_gfx.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <modes.h>

int open( char *name, int mode);
int close( int path );
int read( int path, char *buffer, unsigned count);

/*****************************************************************************
 * Color macros
 *****************************************************************************/
#define _TO_RGB555(r,g,b) ((((r)&0xf8)<<7)|(((g)&0xf8)<<2)|(((b)>>3)&0x1f))
#define _TO_RGB565(r,g,b) ((((r)&0xf8)<<8)|(((g)&0xfc)<<3)|(((b)>>3)&0x1f))
#define _TO_RGB655(r,g,b) ((((r)&0xfc)<<8)|(((g)&0xf8)<<2)|(((b)>>3)&0x1f))
#define _TO_RGB556(r,g,b) ((((r)&0xf8)<<8)|(((g)&0xf8)<<3)|(((b)>>2)&0x2f))
#define _GET_R(rgb) (((rgb)>>16) & 0xff)
#define _GET_G(rgb) (((rgb)>>8) & 0xff)
#define _GET_B(rgb) ((rgb) & 0xff)

/*******************************************************************************
* TYPES & DEFINES
*******************************************************************************/
/* 
 * the options RANGE1 through USER_FILE are mutually exclusive
 */
#define RANGE1			 1
#define RANGE2			 2
#define ALPHA			 3
#define UPPERCASE		 4
#define LOWERCASE		 5
#define NUMERICS		 6
#define SPECIAL			 7
#define USER_RANGE		 8
#define USER_STRING		 9
#define USER_FILE		10

#define BUFSIZE			256
#define FILEREAD                1

/*******************************************************************************
* GLOBALS
*******************************************************************************/
char           *fontname 			= '\0';
char           *range_start			= (char*) 32;
char           *range_end 			= (char*) 127;
char           *user_string 		= '\0';
char           *user_file 			= '\0';
char           *back_color 			= '\0';
char           *fore_color 			= '\0';
char           *screen_color 		= '\0';
char           *blit_char 			= '\0';
BOOLEAN         alternate_font 		= FALSE;
BOOLEAN         wrap		 		= TRUE;
BOOLEAN         negative	 		= FALSE;

u_int32			what_to_display 	= RANGE1;
u_int32			x					= 1;	
u_int32			y					= 0;	
int16			padding_value 		= 0;	
u_int8			cm_index 			= 0;	
u_int32			res_index 			= 0;	
u_int32			output_device 		= 0;	
u_int32			display_info 		= 0;	

GFX_PIXEL 		exptbl[2];    /* Pixel expansion table */
GFX_DIMEN 		ret_width;
GFX_DEV_RES     *devres;	/* Default device resolution */

char			reply[80];

error_code		ec;
/*******************************************************************************
* PROTOTYPES
*******************************************************************************/
static void     parse_options( int argc, char **argv );
static void     print_usage( char *modname );
static void		display_font( u_int32  directive, TXT_CONTEXT_ID txtctxt, TXT_FONT *font );
static void info( TXT_FONT *font, const GFX_DEV_CAP *devcap );

/*******************************************************************************
* main()
*******************************************************************************/
void 
main( int argc, char *argv[] )
{
	GFX_DEV_ID        gfxdev;	/* Graphics device ID */
	const GFX_DEV_CAP *devcap;	/* Device capabilities */
	GFX_DEV_CM        *devcm;	/* Default device coding method */
	GFX_DMAP          *dmap;	/* Visible drawmap */
	GFX_VPORT_ID      vport;	/* Viewport ID */
	TXT_FONT          *font;	/* Font */
	TXT_CONTEXT_ID    txtctx;	/* Text context ID */
	GFX_PIXEL ctmp;

	parse_options( argc, argv );
	/*
	 * Initialize MAUI APIs used by this program
	 */
	maui_set_error_action( MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT, MAUI_ERR_FATAL );
	cdb_init();
	txt_init();

	/*
	 * Open the graphics device, create the gfx shade and get devcap info
	 */
	demo_open_gfx_dev( &gfxdev );
	demo_create_gfx_shade( gfxdev, DEMO_GFX_SHADE, NULL );
	gfx_get_dev_cap( &devcap, gfxdev );

	/*
	 * heed the coding method and resolution changes from the command line
	 */
	if ( cm_index == 0 )
		devcm  = &devcap->cm_info[ 0 ];
	else if ( cm_index <= devcap->num_cm )
		devcm  = &devcap->cm_info[ cm_index ];
	else
		devcm  = &devcap->cm_info[ 0 ];

	if ( res_index == 0 )
		devres  = &devcap->res_info[ 0 ];
	else if ( res_index <= devcap->num_res )
		devres  = &devcap->res_info[ res_index ];
	else
		devres  = &devcap->res_info[ 0 ];

    /* Set expansion table and background color depending on pixel depth */
    switch ( gfx_get_cm_name(devcm->coding_method) ) {
	     case GFX_CM_1BIT:
              ctmp = 0;
              exptbl[0] = 0;
              exptbl[1] = 1;
              break;
         case GFX_CM_2BIT:
              ctmp = 2;
              exptbl[0] = 1;
              exptbl[1] = 3;
              break;
		 case GFX_CM_RGB555:		                /* 16 bit combo's */
			  ctmp = demo_colors[14];
		      exptbl[1] = _TO_RGB555((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      ctmp = demo_colors[8];
		      exptbl[0] = _TO_RGB555((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
			  ctmp = exptbl[0];
			  break;
		 case GFX_CM_RGB655:
			  ctmp = demo_colors[14];
		      exptbl[1] = _TO_RGB655((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      ctmp = demo_colors[8];
		      exptbl[0] = _TO_RGB655((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
			  ctmp = exptbl[0];
			  break;
		 case GFX_CM_RGB565:
		      ctmp = demo_colors[14];
		      exptbl[1] = _TO_RGB565((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      ctmp = demo_colors[8];
		      exptbl[0] = _TO_RGB565((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      ctmp = exptbl[0];
		      break;
		 case GFX_CM_RGB556:
		      ctmp = demo_colors[14];
		      exptbl[1] = _TO_RGB556((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		      ctmp = demo_colors[8];
		      exptbl[0] = _TO_RGB556((_GET_R(ctmp)),(_GET_G(ctmp)) ,(_GET_B(ctmp)));
		  	  ctmp = exptbl[0];
		  	  break;
         default:        
	          ctmp = 8;				 /* 8 bit default */
			  exptbl[0] = 0;
    		  exptbl[1] = 14;
              break;
	}

	if ( exptbl[0] == exptbl[1] )
		fprintf( stderr,"Warning: The foreground color equals the background color.\n\n" );

	/*
	 * Create and setup the destination drawmap
	 */
	gfx_create_dmap( &dmap, MEM_DEF_SHADE );
	gfx_set_dmap_size(dmap, devcm->coding_method,
					  devres->disp_width  / devcm->dm2dp_xmul,
					  devres->disp_height / devcm->dm2dp_ymul );
	gfx_set_dmap_pixmem( dmap, NULL, DEMO_GFX_SHADE, 0 );
	dmap->palette = &demo_palette;
	demo_clear_dmap( gfxdev, dmap, ctmp );

	/*
	 * Create the viewport, put the drawmap in it, and display it
	 */
	ec = gfx_create_vport( &vport, gfxdev, 0, 0, devres->disp_width,
					  devres->disp_height, GFX_VPORT_FRONT);
	gfx_set_vport_dmap ( vport, dmap, 0, 0 );
	gfx_set_vport_state( vport, TRUE );
	gfx_update_display ( gfxdev, FALSE );

	/*
	 * Set the font
	 */
	if ( alternate_font ) {
		ec = demo_get_ucm_font( &font, MEM_DEF_SHADE, fontname );
		if ( ec != SUCCESS ) {
                        fprintf(stderr,"\n%s font not found in memory\n", fontname);
	                fflush(stderr);
			exit( ec );
		}
	} else {
		ec = demo_get_ucm_font( &font, MEM_DEF_SHADE, "default.fnt" );
		if ( ec != SUCCESS ) {
                        fprintf(stderr,"\n'default.fnt' font not found in memory\n");
	                fflush(stderr);
			exit( ec );
		}
	}

	/* 
	 * Create and setup the text context 
	 */
	txt_create_context    ( &txtctx, gfxdev );
	txt_set_context_dst   (  txtctx, dmap );
	txt_set_context_font  (  txtctx, font );
	txt_set_context_exptbl(  txtctx, 2, exptbl );
	txt_set_context_cpad  (  txtctx, padding_value );

	/*
	 * Draw the text string and wait for user response
	 */
	display_font( what_to_display, txtctx, font );

	/*
	 * Process the options that don't depend on the graphics display.
	 */
	if ( display_info )
		info( font, devcap );

	if ( display_info ) {
           fprintf( stderr, "\n" );
	}

	/*
	 * Wait for user response
	 */
        fprintf( stderr, "Press return to quit." );
         fflush(stderr);
        gets( reply );

	if ( display_info ) {
          fprintf( stderr, "\n" );
	}

	/*
	 * Shut down
	 */
	txt_destroy_context( txtctx );
	demo_release_ucm_font( font );

	gfx_set_vport_state( vport, FALSE );
	gfx_update_display( gfxdev, FALSE );
	gfx_destroy_vport( vport );

	mem_sfree( dmap->pixmem_shade, dmap->pixmem );
	gfx_destroy_dmap( dmap );
	mem_destroy_shade( DEMO_GFX_SHADE );
	gfx_close_dev( gfxdev );

	txt_term();
	cdb_term();

	exit( SUCCESS );
}

/*******************************************************************************
* display_font()
*******************************************************************************/
static void 
display_font( u_int32  directive, TXT_CONTEXT_ID txtctxt, TXT_FONT *font )
{
	size_t				string_len;
	u_int32				a_char;
	GFX_DIMEN 			ret_width = 0;
        wchar_t 			txt_array[1];

	wchar_t             txt_alpha_0[]=L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	wchar_t             txt_alpha_1[]=L"abcdefghijklmnopqrstuvwxyz";
	wchar_t             txt_special[]=L"!\"#$%&'()*+,-./ :;<=>?@ [\\]^` {|}~";

        int                 fd;                 /* file descriptor */
	char		    buf[ FILEREAD ];
	size_t		    count_read;
	u_int16             count = FILEREAD;


	if ( y == 0)
		y = font->height + 1;

	switch ( directive )
	{
		/*
		 *
		 */
		case RANGE1:
		case RANGE2:
		case UPPERCASE:
		case LOWERCASE:
		case NUMERICS:
		case USER_RANGE:
 
			string_len = 1;

			for( a_char = (u_int32) range_start; a_char <= (u_int32) range_end; a_char++ )
			{
			  txt_array[ 0 ] = a_char;
			  txt_draw_wcs( &ret_width, txtctxt, txt_array, &string_len, x, y, NULL );
		 
			  x += ret_width;
		 
			  if ( wrap )
				  if ( x + ret_width >= devres->disp_width)
				  {
					  x = 1;
					  y += font->height + 1;
				  }
			}

			break;

		case ALPHA:
              string_len = 26;
			  txt_draw_wcs( &ret_width, txtctxt, txt_alpha_0, &string_len, x, y, NULL );
			  y += font->height + 1;
			  txt_draw_wcs( &ret_width, txtctxt, txt_alpha_1, &string_len, x, y, NULL );
			  break;

		case SPECIAL:
		      string_len = 36;
			  txt_draw_wcs( &ret_width, txtctxt, txt_special, &string_len, x, y, NULL );
			  break;

		case USER_STRING:
			  string_len = strlen(user_string);		  /* find size of user string */
			  txt_draw_mbs( &ret_width, txtctxt, user_string, &string_len, x, y, NULL );
			  break;

		case USER_FILE:

			fd = open( user_file, S_IREAD);
			if ( fd == -1 )
			{
				fprintf( stderr, "cannot open '%s' for read access\n", user_file );
				exit( 1 );
			}

			while ( (count_read = read( fd, buf,  count )) > 0 ) {
			  txt_draw_mbs( &ret_width, txtctxt, buf, &count_read, x, y, NULL );
			  x += ret_width;
			  if ( x + ret_width >= devres->disp_width)
			  {
					  x = 1;
					  y += font->height + 1;
			  }
			}

                        close( fd );
			break;
		
		default:
			break;
			
	}

}


/*******************************************************************************
* info()
*******************************************************************************/
static void info( TXT_FONT *font, const GFX_DEV_CAP *devcap )
{
	int			i;


	/*
	 * Print font information
	 */
	fprintf( stdout, "\nFont Information\n");
	fprintf( stdout, "type:                       TXT_FONTYPE_%s\n", font->font_type==TXT_FONTYPE_MONO?"MONO":"PROP");
	fprintf( stdout, "width:                      %8d\n", font->width);
	fprintf( stdout, "height:                     %8d\n", font->height);
	fprintf( stdout, "ascent:                     %8d\n", font->ascent);
	fprintf( stdout, "descent:                    %8d\n", font->descent);
	fprintf( stdout, "default character:          %8d\n", font->default_char);
	fprintf( stdout, "number of range tables:     %8d\n", font->num_ranges);

	for (i = 0; i < font->num_ranges; i++)
	{
		fprintf( stdout, "\n");
		fprintf( stdout, "range[%d] number of chars:   %8d\n",i,font->range_tbl[i].last_char - 
													            font->range_tbl[i].first_char);
		fprintf( stdout, "range[%d] first_char:        %8d\n",i,font->range_tbl[i].first_char);
		fprintf( stdout, "range[%d] last_char:         %8d\n",i,font->range_tbl[i].last_char);
		fprintf( stdout, "range[%d] offset table:    0x%.8x\n",i,font->range_tbl[i].offset_tbl);
		fprintf( stdout, "range[%d] width table:     0x%.8x\n",i,font->range_tbl[i].width_tbl);
		fprintf( stdout, "range[%d] bitmap:          0x%.8x\n",i,font->range_tbl[i].bitmap);
	}

	/*
	 * Print DEVCAP information
	 */
	fprintf( stdout, "\nDevcap Information\n");
	fprintf( stdout, "hw_type:                      %s\n", devcap->hw_type);
	fprintf( stdout, "hw_subtype:                   %s\n", devcap->hw_subtype);

	if ( devcap->sup_vpmix )
		fprintf( stdout, "supports viewport mixing:      TRUE\n");
	else
		fprintf( stdout, "supports viewport mixing:      FALSE\n");

	if ( devcap->sup_extvid )
		fprintf( stdout, "supports external video:       TRUE\n");
	else
		fprintf( stdout, "supports external video:       FALSE\n");

	if ( devcap->sup_bkcol )
		fprintf( stdout, "supports backdrop color:       TRUE\n");
	else
		fprintf( stdout, "supports backdrop color:       FALSE\n");

	if ( devcap->sup_vptrans )
		fprintf( stdout, "supports vport transparency:   TRUE\n");
	else
		fprintf( stdout, "supports vport transparency:   FALSE\n");

	if ( devcap->sup_vpinten )
		fprintf( stdout, "supports vport intensity:      TRUE\n");
	else
		fprintf( stdout, "supports vport intensity:      FALSE\n");

	if ( devcap->sup_sync )
		fprintf( stdout, "supports vport sync:           TRUE\n");
	else
		fprintf( stdout, "supports vport sync:           FALSE\n");

	if ( devcap->sup_decode )
		fprintf( stdout, "supports viewport mixing:      TRUE\n");
	else
		fprintf( stdout, "supports viewport mixing:      FALSE\n");


	fprintf( stdout, "depth of DAC:                      %d\n", devcap->dac_depth);
	fprintf( stdout, "\nNumber of resolutions:             %d\n", devcap->num_res);

	/*
	 * Print Resolution information
	 */
	for (i = 0; i < devcap->num_res; i++)
	{
		fprintf( stdout, "res_num[%d] display width:     %6d\n", i, devcap->res_info[i].disp_width );
		fprintf( stdout, "res_num[%d] display height:    %6d\n", i, devcap->res_info[i].disp_height );
		fprintf( stdout, "res_num[%d] refresh rate:      %6d\n", i, devcap->res_info[i].refresh_rate );
		fprintf( stdout, "res_num[%d] interlace mode:    %6d\n", i, devcap->res_info[i].intl_mode );
		fprintf( stdout, "res_num[%d] X aspect:          %6d\n", i, devcap->res_info[i].aspect_x );
		fprintf( stdout, "res_num[%d] X aspect:          %6d\n", i, devcap->res_info[i].aspect_y );

	}


	/*
	 * Print Coding Method information
	 */
	fprintf( stdout, "\nNumber of coding methods:          %d\n", devcap->num_cm);

	for (i = 0; i < devcap->num_cm; i++)
	{
      fprintf( stdout, "cm_num[%d] coding method: ", i  );
		switch ( gfx_get_cm_name( devcap->cm_info[i].coding_method ) )
		{
			case GFX_CM_UNKNOWN:
				fprintf( stdout, "GFX_CM_UNKNOWN\n");
				break;
			case GFX_CM_1BIT:
				fprintf( stdout, "GFX_CM_1BIT\n");
				break;
			case GFX_CM_2BIT:
				fprintf( stdout, "GFX_CM_2BIT\n");
				break;
			case GFX_CM_3BIT:
				fprintf( stdout, "GFX_CM_3BIT\n");
				break;
			case GFX_CM_4BIT:
				fprintf( stdout, "GFX_CM_4BIT\n");
				break;
			case GFX_CM_5BIT:
				fprintf( stdout, "GFX_CM_5BIT\n");
				break;
			case GFX_CM_6BIT:
				fprintf( stdout, "GFX_CM_6BIT\n");
				break;
			case GFX_CM_7BIT:
				fprintf( stdout, "GFX_CM_7BIT\n");
				break;
			case GFX_CM_8BIT:
				fprintf( stdout, "GFX_CM_8BIT\n");
				break;
			case GFX_CM_RGB555:
				fprintf( stdout, "GFX_CM_RGB555\n");
				break;
			case GFX_CM_RGB888:
				fprintf( stdout, "GFX_CM_RGB888\n");
				break;
			case GFX_CM_CDI_RL3:
				fprintf( stdout, "GFX_CM_CDI_RL3\n");
				break;
			case GFX_CM_CDI_RL7:
				fprintf( stdout, "GFX_CM_CDI_RL7\n");
				break;
			case GFX_CM_CDI_DYUV:
				fprintf( stdout, "GFX_CM_CDI_DYUV\n");
				break;
			case GFX_CM_1A7_8BIT:
				fprintf( stdout, "GFX_CM_1A7_8BIT\n");
				break;
			case GFX_CM_A1_RGB555:
				fprintf( stdout, "GFX_CM_A1_RGB555\n");
				break;
			case GFX_CM_YCBCR422:
				fprintf( stdout, "GFX_CM_YCBCR422\n");
				break;
			case GFX_CM_YCRCB422:
				fprintf( stdout, "GFX_CM_YCRCB422\n");
				break;
			case GFX_CM_YCBCR420:
				fprintf( stdout, "GFX_CM_YCBCR420\n");
				break;
			case GFX_CM_YCRCB420:
				fprintf( stdout, "GFX_CM_YCRCB420\n");
				break;
            case GFX_CM_RGB565:
				fprintf( stdout, "GFX_CM_RGB565\n");
				break;
            case GFX_CM_RGB655:
				fprintf( stdout, "GFX_CM_RGB655\n");
				break;
            case GFX_CM_RGB556:
				fprintf( stdout, "GFX_CM_RGB556\n");
				break;
			
			default:
				fprintf( stdout, "NOT FOUND\n", i);
				break;
		}
	
	fflush( stdout );

	}


	/*
	 * Print CDB information
	 */


}

/*******************************************************************************
* parse_options()
*******************************************************************************/
static void 
parse_options( int argc, char **argv )
{
	char           *modname = *argv;
	char           *ptr;

	/* parse the command line parameters */
	while ( --argc )
	{
		/* only interested in options right now */
		if ( *( ptr = *++argv ) == '-')
		{
			while ( *++ptr )
			{
				switch ( *ptr | 0x20 )	/* to lower */
				{

				/*
				 * options indicating what to display
				 */
				case 'r':
					if ( (*++ptr) == '1' )
					{
						what_to_display = RANGE1;
						range_start = (char*) 32;
						range_end   = (char*) 127;
					}
					else if ( (*ptr) == '2' )
					{
						what_to_display = RANGE2;
						range_start = (char*) 32;
						range_end   = (char*) 255;
					}
					else if ( (*ptr | 0x20) == 's' )
					{
						if ( *++ptr == '=' )
							ptr++;
						res_index = atoi(  ptr );
						while (*ptr >= '0' && *ptr <= '9') ptr++;
						ptr--;
						break;
					}
					else
						print_usage( modname );

					break;

				case 'a':
					if ( (*++ptr | 0x20) == 'u' )
					{
						what_to_display = UPPERCASE;
						range_start = (char*) 'A';
						range_end   = (char*) 'Z';
					}
					else if ( (*ptr | 0x20) == 'l' )
					{
						what_to_display = LOWERCASE;
						range_start = (char*) 'a';
						range_end   = (char*) 'z';
					}
					else if ( *ptr == '\0' )
					{
						what_to_display = ALPHA;
                        ptr--;
					}
					else
						print_usage( modname );
					
					break;

				case 'n':
					what_to_display = NUMERICS;
					range_start = (char*) '0';
					range_end   = (char*) '9';
					break;

				case 's':
					what_to_display = SPECIAL;
					break;

				case 'u':
					if ( (*++ptr | 0x20) == 'r' )
					{
						if ( *++ptr == '=' )
							ptr++;
						what_to_display = USER_RANGE;

						range_start = (char*) atoi( ptr );
						if ( (ptr = strchr( ptr, ',' )) == NULL )
							print_usage( modname );
						ptr++;
						range_end   =  (char*) atoi( ptr );

                        while ( *++ptr );
                        ptr--;

					}
					else if ( (*ptr | 0x20) == 's' )
					{
						if ( *++ptr == '=' )
							ptr++;
						what_to_display = USER_STRING;
						ptr++;
						user_string = ptr;
						ptr = strchr( ptr, '"' );
						*ptr = '\0';

						ptr--;
					}
					else if ( (*ptr | 0x20) == 'f' )
					{
						if ( *++ptr == '=' )
							ptr++;
						what_to_display = USER_FILE;
						user_file = ptr;
						while ( *++ptr );
						ptr--;

					}
					else
						print_usage( modname );
					
					break;



				/*
				 * additional options on how to display
				 */
				case 'f':
					if ( *++ptr == '=' )
						ptr++;
					fontname = ptr;
					alternate_font = TRUE;
					while ( *++ptr );
					ptr--;

					break;

				case 'c':
					if ( (*++ptr | 0x20) == 'b' )
					{
						if ( *++ptr == '=' )
							ptr++;
						if ( (exptbl[0] = (u_int32) atoi( ptr )) > 15 )
							print_usage( modname );
							
						while ( *++ptr );
						ptr--;
						break;
					}
					else if ( (*ptr | 0x20) == 'f' )
					{
						if ( *++ptr == '=' )
							ptr++;
						if ( (exptbl[1] = (u_int32) atoi( ptr )) > 15 )
							print_usage( modname );
						while ( *++ptr );
						ptr--;
						break;
					}
					else if ( (*ptr | 0x20) == 's' )
					{
						if ( *++ptr == '=' )
							ptr++;
						screen_color = ptr;	
						while ( *++ptr );
						ptr--;
						break;
					}
					else
						print_usage( modname );


				case 'x':
					if ( *++ptr == '=' )
						ptr++;
					x = atoi(  ptr );
					while (*ptr >= '0' && *ptr <= '9') ptr++;
					ptr--;

					break;

				case 'y':
					if ( *++ptr == '=' )
						ptr++;
					y = atoi(  ptr );
					while (*ptr >= '0' && *ptr <= '9') ptr++;
					ptr--;

					break;

				case 'w':
					wrap = TRUE;
					break;

				case 'p':
					negative = FALSE;
					if ( *++ptr == '=' )
						ptr++;
					if ( *ptr == '-' )
					{
						negative = TRUE;
						ptr++;
					}
					else if ( *ptr == '+' )
					{
						negative = FALSE;
						ptr++;
					}

					padding_value = atoi(  ptr );
					if ( negative )
						padding_value = -padding_value;

					while (*ptr >= '0' && *ptr <= '9') ptr++;
					ptr--;

					break;

				case 'm':
						if ( *++ptr == '=' )
							ptr++;
						cm_index = atoi(  ptr );
						while (*ptr >= '0' && *ptr <= '9') ptr++;
						ptr--;
						break;

				case 'o':
					if ( *++ptr == '=' )
						ptr++;
					output_device = atoi(  ptr );
					while (*ptr >= '0' && *ptr <= '9') ptr++;
					ptr--;
					break;

				case 'i':
					display_info = TRUE;
					break;

				case 'b':
					if ( *++ptr == '=' )
						ptr++;
					blit_char = ptr;
					while ( *++ptr );
					ptr--;
					break;


				case 'h':
				case '?':
					print_usage( modname );
					exit( 0 );

				default:
					fprintf( stderr, "\nunknown option: '%c'\n", *ptr );
					print_usage( modname );
					exit( 1 );

				}	/* end of switch */
			}		/* end of while */
		}			/* end of if */
		else
		{
			print_usage( modname );
		}
	}				/* end of while */
}

/*******************************************************************************
* print_usage()
*******************************************************************************/
static void 
print_usage( char *modname )
{
	fprintf( stderr, "Usage: %s <opts>\n", modname );
	fprintf( stderr, "Description: Demo app that displays font on the screen. \n" );
	fprintf( stderr, "<opts>:  -h or -?              This help.\n" );
        fprintf( stderr, "         -i                    print information about font.\n" );
	fprintf( stderr, "         -f[=]<modulename>     reads font from <modulename>.\n" );
	fprintf( stderr, "                               (otherwise 'default.fnt' is used.\n" );
        fprintf( stderr, "         -a                    displays ['a',...,'z'] and ['A',...,'z'].\n" );
        fprintf( stderr, "         -n                    displays ['0',...,'9'].\n" );
        fprintf( stderr, "         -s                    displays ['!',...,'~'].\n" );
        fprintf( stderr, "        -au                    displays ['A',...,'Z'].\n" );
        fprintf( stderr, "        -al                    displays ['a',...,'z'].\n" );
        fprintf( stderr, "        -r1 or -r2             displays [32,127] or [32,255] range of chars.\n" );
        fprintf( stderr, "        -rs[=]<n>              specifies display resolution <n>.\n" );
        fprintf( stderr, "         -m[=]<n>              specifies coding method <n>.\n" );
        fprintf( stderr, "         -x[=]<x_start>        display starting at <x_start> coordinate.\n" );
        fprintf( stderr, "         -y[=]<y_start>        display starting at <y_start> coordinate.\n" );
        fprintf( stderr, "         -p[=]<padding>        display characters with <padding>.\n" );
        fprintf( stderr, "        -ur[=]<start,end>      displays characters within <start,end> range.\n" );
        fprintf( stderr, "        -us[=]\"string\"         displays \"string\" of characters.\n" );
        fprintf( stderr, "        -uf[=]<file_on_disc>   displays content of the file <file_on_disc>.\n" );
        fprintf( stderr, "        -cb[=]<index>          sets font background color to <index> value.\n" );
        fprintf( stderr, "        -cf[=]<index>          sets font foreground color to <index> value.\n" );
	exit( 0 );
}
