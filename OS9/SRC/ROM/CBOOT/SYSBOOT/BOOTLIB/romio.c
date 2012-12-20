/*
 * Copyright 1990, 1993 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 */

#include <sysboot.h>

/*------------------------------------------!
! InChar: input one character from console	!
!------------------------------------------*/
char InChar()
{
	return ( ( (char (*)()) ((void *) &(bootjmp->inchar)) )() );
}


/*----------------------------------------------!
! InChChek: input one character from console	!
!			unblocked - no echo					!
!----------------------------------------------*/
int InChChek()
{
	return ( ( (int (*)()) ((void *) &(bootjmp->inchchek)) )() );
}


/*------------------------------------------!
! OutChar: output one character to console	!
!------------------------------------------*/
error_code OutChar(ochr)
char	ochr;
{
	return ( ( (error_code (*)()) ((void *) &(bootjmp->outchar)) )(ochr) );
}


/*------------------------------------------------------!
! OutHex: output a nibble to the console in hexadecimal	!
!------------------------------------------------------*/
error_code OutHex(nibble)
char	nibble;
{
	return ( ( (error_code (*)()) ((void *) &(bootjmp->outhex)) )(nibble) );
}


/*------------------------------------------------------!
! Out1Hex: output a byte to the console in hexadecimal	!
!------------------------------------------------------*/
error_code Out1Hex(byte)
u_char	byte;
{
	return ( ( (error_code (*)()) ((void *) &(bootjmp->out1hex)) )(byte) );
}


/*------------------------------------------------------!
! Out2Hex: output a word to the console in hexadecimal	!
!------------------------------------------------------*/
error_code Out2Hex(word)
u_int16	word;
{
	return ( ( (error_code (*)()) ((void *) &(bootjmp->out2hex)) )(word) );
}


/*------------------------------------------------------!
! Out4Hex: output a word to the console in hexadecimal	!
!------------------------------------------------------*/
error_code Out4Hex(longword)
u_int32	longword;
{
	return ( ( (error_code (*)()) ((void *) &(bootjmp->out4hex)) )(longword) );
}
