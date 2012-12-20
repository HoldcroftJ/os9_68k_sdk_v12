/*****************************************************************************
 *
 * Filename: history.h
 *  Purpose: A validation utility for CDB modules
 *
 *****************************************************************************
 *                                                                           *
 *           Copyright 2001 - 2001 by Microware Systems Corporation          *
 *                            All Rights Reserved                            *
 *                          Reproduced Under License                         *
 *                                                                           *
 *                                                                           *
 * This software is confidential property of Microware Systems Corporation,  *
 * and is provided under license for internal development purposes only.     *
 * Reproduction, publication, modification, distribution, or creation of     *
 * derivative works in any form to any party other than the Licensee is      *
 * strictly prohibited, unless expressly provided for under the terms of a   *
 * mutually executed written license agreement for this software between the *
 * end-user and Microware Systems Corporation, or otherwise expressly        *
 * authorized in writing by Microware Systems Corporation. Licensee shall    *
 * reproduce this copyright notice exactly as provided herein on any copies  *
 * of the software that Licensee is authorized to distribute.                *
 *****************************************************************************
 * Edition History:
 * #   Date   Comments                                              By
 * --- ------ ----------------------------------------------------- ---------
 * 001 010613 Initial version                                       chriss
 ****************************************************************************/


#ifdef _MAIN

	/* set module edition and attribute/revision word */
	_asm("_sysedit: equ 11");
	_asm("_sysattr: equ 0x8000");

#endif


/****************************************************************************/
