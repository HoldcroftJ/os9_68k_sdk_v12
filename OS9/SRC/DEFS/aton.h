#if !defined(_ATON_H)
#define _ATON_H

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
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
|  01 98/07/29 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#define _16s_MID		19			/* middle of _tab16s table		*/
#define _ONES_MID		17			/* middle of _tab1s table		*/

/* _aton() number types returned */
typedef enum _number_type {
	_ERROR,			/* flag for error 		*/
	_INTEGER,		/* integer returned		*/
	_REAL			/* real returned		*/
} _number_type;

/* _aton() number returned */
typedef struct _number {
	unsigned	Int;	/* integer number		*/
	double		Real;	/* real number			*/
} _number;

/* 
 * ascii to integer/double converter
 * _number_type	_aton(char **, _number *);
 * 
 * The first parameter is a pointer to a string
 * hopefully containing a ascii number.  This string
 * pointer is updated upon return with the string
 * pointer pointing at the end of the ascii string
 * passed.
 *
 * The second parameter is a pointer to the above
 * described _number type.  This upon return will
 * contain the integer/double number.
 *
 * On error ERROR will be returned, and the first parameter
 * will be updated to the point where the error occured,
 * and the second parameter value will be undefined.
 */ 
 
extern double 		_tab16s[],
					_tab1s[];

#if defined(__cplusplus) || ((defined(_ANSI_EXT) || defined(__STDC__)) && defined(_OPT_PROTOS))
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

_number_type _aton _OP((char **, _number *));

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _ATON_H */
