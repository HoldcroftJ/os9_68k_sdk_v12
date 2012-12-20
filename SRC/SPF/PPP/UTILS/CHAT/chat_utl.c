/****************************************************************************
 *  ID:		%A%
 *  Date:	%G%
 ****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *   1 05/29/96 Initial version                                        billw
 *     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
 *  18 12/15/97 Fixed to stop compiler warnings                        tmd
 *     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
 ****************************************************************************/

/****************************************************************************
 * This design is based on RFC 1144 and the contents therein, so to be safe,
 * The copyright notice is included:
 *
 * Copyright (c)1989 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that the above copyright notice and this paragraph are duplicates in all
 * such forms and that any documentation, advertising materials, and other
 * materials relating to such distribution and use acknowledge the software
 * was developed by the University of California, Berkely. The name of the
 * University may not be used to endorse or promote products derived from
 * this software without specific prior permission.
 ****************************************************************************/

#include "chat.h"

int match_string( Chat_str p, char c )
{
	if( *(p->cursor) == c )
	{
		(p->cursor)++;

		if( *(p->cursor) == '\0' )
			return( 1 );
	}

	else
		p->cursor = p->str;

	return( 0 );
}

/**********************************************************************/
/* This is simply a wrapper function for strtok that allows us to     */
/* print the tokens if we want to.                                    */
/**********************************************************************/
char* next_token( char* src, char* delim )
{
	char*	word = strtok( src, delim );

	if( debug && word )
		printf( "Token = <%s> \n", word );

	return( word );
}

/**********************************************************************/
/* This function will get the next word in the sentence, and copy it  */
/* into the caller's buffer.                                          */
/**********************************************************************/
void get_next_word( char* buf, int maxlen )
{
	char*	word = next_token( NULL, " \t.,=\n" );

	if( word )
	{
		strncpy( buf, word, maxlen );
		buf[maxlen-1] = '\0';
	}

	else
		buf[0] = '\0';
}

/**********************************************************************/
/* This function will check for the word 'on' or 'off', and return    */
/* one or zero as the result.                                         */
/**********************************************************************/
int get_boolean( void )
{
	char*	word = next_token( NULL, " \t.,=\n" );

	if( ! word )
		invalid_cmd();

	if( strnccmp( word, "on" ) == 0 )
		return( 1 );

	else if( strnccmp( word, "off" ) == 0 )
		return( 0 );

	invalid_cmd();

	/* not reached */
	return(0);
}

/**********************************************************************/
/* This function will check to make sure all characters in a word are */
/* decimal digits, and return the decimal value.                      */
/**********************************************************************/
int get_integer( void )
{
	int	valid = 0, value = 0;

	value = check_integer( next_token( NULL, " \t,\n" ), &valid );
	if( ! valid ) invalid_cmd();

	return( value );
}

int check_integer( char* word, int* valid )
{
	int	value = 0;

	if( ! word )
	{
		*valid = 0;
		return( 0 );
	}

	do
	{
		if( isdigit(*word) )
		{
			value *= 10;
			value += (*word++ - '0');
		}

		else
		{
			*valid = 0;
			return( 0 );
		}

	} while( *word != '\0' );

	*valid = 1;
	return( value );
}

void check_cmds( void )
{
	if( cmd_path || cmd_file )
	{
		printf( "Cannot specify multiple -z options. \n" );
		show_help( E_PARAM );
	}
}

int check_aborts( char c )
{
	Chat_str	p = aborts;

	while( p )
	{
		if( match_string( p, c ) )
		{
			if( verbose )
			{
				clean_line();
				printf( "Got abort string ( %s ) \n", p->str );
			}

			return( E_SIGABRT );
		}

		p = p->next;
	}

	return( 0 );
}

/**********************************************************************/
/* This function will skip over whitespace characters in a string and */
/* return a pointer to the first non-white character in the string    */
/**********************************************************************/
char* skipwhite( char* s )
{
	while( *s != '\0' && isspace( *s ) )
		s++;

	return( s );
}

/**********************************************************************/
/* This function does the same thing as strcmp(), but converts each   */
/* character to lower case prior to comparison. The original strings  */
/* are not altered.                                                   */
/**********************************************************************/
int strnccmp( char* s, char* t )
{
	while( tolower(*s) == tolower(*t) )
	{
		if( *s == '\0' )
			return( 0 );

		s++;
		t++;
	}

	return( *s - *t );
}

/**********************************************************************/
/* This function is like strnccmp() above, but will allow the first   */
/* string to be longer than the second string.                        */
/**********************************************************************/
int strnnccmp( char* s, char* t )
{
	while( tolower(*s) == tolower(*t) )
	{
		if( *s == '\0' )
			return( 0 );

		s++;
		t++;
	}

	if( *t == '\0' )
		return( 0 );

	return( *s - *t );
}

#if 0

/**********************************************************************/
/* atof() and strtod() don't return the correct value. Here is a      */
/* version of atof() that works correctly.                            */
/**********************************************************************/
#define check_sign(a,b)	if( *a=='+') a++; else if( *a=='-' ) { a++; b = -1; }

double atof( char* s )
{
	double	val = 0.0, mant = 0.0, mdiv = 1.0;
	int		sign = 1, exp_sign = 1, exp = 0;
	int		is_exp = 0, is_fract = 0, not_done = 1;

	/* Skip leading whitespace */

	s = skipwhite( s );

	/* Check for plus or minus */

	check_sign( s, sign );

	/* Handle mantissa digits */

	while( isdigit( *s ) )
	{
		mant *= 10.0;
		mant += (double)(*s - '0');
		s++;
	}

	if( *s == '.' )
	{
		s++;

		while( isdigit( *s ) )
		{
			mant *= 10.0;
			mant += (double)(*s - '0');
			mdiv *= 10.0;
			s++;
		}

		if( mdiv > 1.0 )
			mant /= mdiv;
	}

	/* Handle exponent digits */

	if( *s == 'e' )
	{
		s++;

		/* Check for plus or minus */

		check_sign( s, exp_sign );

		while( isdigit( *s ) )
		{
			exp *= 10;
			exp += (*s - '0');
			s++;
		}
	}

	mant *= sign;
	exp *= exp_sign;

	val = mant;

	if( exp > 0 )
	{
		while( exp > 0 )
		{
			val *= 10;
			exp--;
		}
	}

	else if( exp < 0 )
	{
		while( exp < 0 )
		{
			val /= 10;
			exp++;
		}
	}

	return( val );
}

#endif

/**********************************************************************/
/* This function parses a modem string.                               */
/**********************************************************************/
void parse_string( char* s, char* buf, int sending )
{
	char	cur_chr, *s1;
    int		add_return = sending;

	s1 = buf;

	while( s && *s != '\0' )
	{
		cur_chr = *s++;

		if( cur_chr == '^' )
		{
			cur_chr = *s++;

			if( cur_chr == '\0' )
			{
				*s1++ = '^'; 
				break;
			}

			cur_chr &= 0x1F;

			if( cur_chr != 0 )
				*s1++ = cur_chr;

			continue;
		}

		if( cur_chr != '\\' )
		{
			*s1++ = cur_chr;
			continue;
		}

		/*** Handle escape sequence '\?' ***/

		cur_chr = *s++;

		if( cur_chr == '\0' )
		{
			if( sending )
			{
				*s1++ = '\\';
				*s1++ = '\\';
			}

			break;
		}

		switch( cur_chr )
		{
		case 'b':	/* Send break character */
			*s1++ = '\b';
			break;

		case 'c':	/* Don't add carriage return. Valid only at end of line. */
			if( sending && *s == '\0' )
				add_return = 0;
			else
				*s1++ = cur_chr;
			break;

		case '\\':
		case 'K':
		case 'p':
		case 'd':
			if( sending )
				*s1++ = '\\';

			*s1++ = cur_chr;
			break;

		case 'n':	/* Insert newline */
			*s1++ = '\n';
			break;

		case 'N':
			if (sending)
			{
				*s1++ = '\\';
				*s1++ = '\0';
			}
			else
				*s1++ = 'N';
			break;

		case 'q':	/* Toggle quiet mode */
			quiet = ! quiet;
			break;

		case 'r':	/* Insert carriage return */
			*s1++ = '\r';
			break;

		case 's':	/* Insert space */
			*s1++ = ' ';
			break;

		case 't':	/* Insert tab */
			*s1++ = '\t';
			break;
	    
		default:
			if( isodigit(cur_chr) )
			{
				int	n = 0, val = 0;

				while( (n++ < 3) && isodigit(cur_chr) )
				{
					val <<= 3;
					val |= (cur_chr - '0');

					cur_chr = *s++;

					if( sending && (verbose || debug) )
					{
						printqc( stdout, cur_chr );
					}
				}

				if( val || sending )
				{
					if( sending && (val == '\\' || val == 0) )
						*s1++ = '\\';

					*s1++ = val;
				}

				break;
			}

			if( sending )
			{
				*s1++ = '\\';
			}

			*s1++ = cur_chr;
			break;
		}
	}

	if( add_return )
	{
		*s1++ = '\r';
	}

	*s1++ = '\0'; /* guarantee closure */
	*s1++ = '\0'; /* terminate the string */
}

void printqc( FILE* fp, char c )
{
	if( quiet )
	{
		fputc( '*', fp );
		fflush( fp );
	}
	else
	{
		printchar( fp, c );
	}
}

void printchar( FILE* fp, char c )
{
	if( show_hex_vals )
	{
		fprintf( fp, "%02x", (u_int8) c );

		if( delim && (u_int8) c == delim )
		{
			if( n_hexchars > 0 )
			{
				fputc( '\n', fp );
				n_hexchars = 0;
				fflush( fp );
				return;
			}
		}

		n_hexchars++;

		if( n_hexchars & 0x03 == 0 )
		{
			fputc( ' ', fp );
		}

		fflush( fp );
		return;
	}
	else if( c >= ' ' )
	{
		fputc( c, fp );

		if( show_data_chars )
			fputc( ' ', fp );

		line_dirty = 1;
	}
	else if( show_ctrl_chars )
	{
		fprintf( fp, "<%02x>", (u_int8) c );
		line_dirty = 1;
	}
	else if( show_data_chars )
	{
		fprintf( fp, "??", (u_int8) c );
	}
	else if( c >= 0 && c <= 31 )
	{
		line_dirty = 1;
		fprintf( fp, "<%s>", esc_codes[c] );
	}

	if( show_data_chars )
	{
		fprintf( fp, " = <%02x>\n", (u_int8) c );
		line_dirty = 0;
	}

	fflush( fp );
}

void printstring( FILE* fp, char* s )
{
	while( *s != '\0' )
		printchar( fp, *s++ );
}

void set_alarm( u_int32 delay )
{
	if( delay )
	{
		u_int32	tm = SECONDS( delay );

		if( debug )
		{
			clean_line();
			printf( "setting alarm: delay = %x \n", tm );
		}

		status = _os_alarm_set( &alrm_id, ALARM_SIG, tm );

		if( debug )
			printf( "_os_alarm_set() returned %x \n", status );
	}

	alarmed = 0;
}

void reset_alarm( u_int32 delay )
{
	if( delay && alrm_id )
	{
		u_int32	tm = SECONDS( delay );

		if( debug )
		{
			clean_line();
			printf( "resetting alarm: delay = %x \n", tm );
		}
		
#ifndef _OSK
		status = _os_alarm_reset( alrm_id, ALARM_SIG, tm );

		if( debug )
			printf( "_os_alarm_reset() returned %x \n", status );
#else
		status = _os_alarm_delete( alrm_id );

		if( debug )
			printf( "_os_alarm_delete() returned %x \n", status );

		status = _os_alarm_set( &alrm_id, ALARM_SIG, tm );

		if( debug )
			printf( "_os_alarm_set() returned %x \n", status );
#endif
	}

	alarmed = 0;
}

void clear_alarm( void )
{
	if( alrm_id )
	{
		if( debug )
		{
			clean_line();
			printf( "clearing alarm \n" );
		}

		status = _os_alarm_delete( alrm_id );

		if( debug )
			printf( "_os_alarm_delete() returned %x \n", status );
	}

	alarmed = 0;
	n_retries = 0;
}

int check_alarm( char* msg )
{
	int	sig = 0;

	if( quit_sig )
	{
		sig = E_SIGTERM;
	}

	else if( alarmed )
	{
		sig = E_SIGALRM;

		if( verbose )
		{
			clean_line();
			printf( "%s \n", msg );
		}

		if( n_retries >= retries )
			sig = E_SIGABRT;

		retry = 1;
		n_retries++;
	}

	clear_alarm();
	return( sig );
}

Chat_str new_string( char* word )
{
	int			len;
	Chat_str	this;

	if( ! word )
		invalid_cmd();

	len = strlen( word );
	this = (Chat_str) malloc( len + sizeof(chat_str) );

	if( ! this )
	{
		printf( "Out of memory \n" );
		term_prog( E_NORAM );
	}

	this->next = NULL;
	strcpy( this->str, word );
	this->cursor = this->str;

	return( this );
}

u_int32 atox( char* str, u_int8 numdigits, char** cursor )
{
	u_int32	val = 0;

	while( numdigits && isxdigit(*str) )
	{
		val <<= 4;

		if( isdigit(*str) )			/* 0 - 9 */
		{
			val += (*str - '0');
		}
		else					/* A - F */
		{
			val += (tolower(*str) - 'a' + 0x0a);
		}

		numdigits--;
		str++;
	}

	if( cursor )
	{
		*cursor = str;
	}

	return( val );
}
