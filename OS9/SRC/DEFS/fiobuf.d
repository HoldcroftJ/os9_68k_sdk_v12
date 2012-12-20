 opt -l
 nam Unix style FILE iobuf definitions

*****************************************************************************
*                                                                           *
*              Copyright 1998 by Microware Systems Corporation              *
*                            All Rights Reserved                            *
*                          Reproduced Under License                         *
*                                                                           *
*  This software is confidential property of Microware Systems Corporation, *
*  and is provided under license for internal development purposes only.    *
*  Reproduction, publication, modification, distribution, or creation of    *
*  derivative works in any form to any party other than the Licensee is     *
*  strictly prohibited, unless expressly provided for under the terms of a  *
*  mutually executed written license agreement for this software between    *
*  the end-user and Microware Systems Corporation, or otherwise expressly   *
*  authorized in writing by Microware Systems Corporation.  Licensee shall  *
*  reproduce this copyright notice exactly as provided herein on any copies *
*  of the software that Licensee is authorized to distribute.               *
*                                                                           *
*****************************************************************************
* Edition History
*  #   Date    Changes Made												by
* -- -------- --------------------------------------------------------  ---
* 01 01-15-01 Genesis, Created for use with 68k cstart to reference     mgh *
*               the stderr file io path number
*             $$                    <RELEASE_INFO>                     $$   *
*
edition set 01 current edition number


****************************************
* _iobuf Assembly defs
*   Use for getting stdin, stdout, stderr paths for system state processes
* NOTE: Currently 68k Compiler will align pointers to word boundries and
*   stuctures to Int boundries
* _niob is an array of FOPEN_MAX _iobuf buffers (32)
*
 org 0
_iobuf_ptr		do.l 1	Buffer pointer
_iobuf_base		do.l 1	Buffer base address
_iobuf_end		do.l 1	Buffer end address
_iobuf_flag 	do.w 1	FIle Status
_iobuf_fd		do.w 1	File Path Number
_iobuf_ungetc	do.b 2	Room for at least 2 ungetc characters
_iobuf_obufsiz	do.w 1	Old Buffer size (obsolete)
_iobuf_ifunc	do.l 1	Function pointer to use for input
_iobuf_ofunc	do.l 1	Function pointer to use for output
_iobuf_bufsiz	do.l 1	size of data buffer
_iobuf_filepos	do.l 1	current "real" file position of start of buffer
_iobuf_nungetc	do.b 1	number of charaxters in _ungetc buffer
				do.b 1	Align with C structure (to word Boundry)
_iobuf_wptr		do.l 1	pointer ot end of written data
_iobuf_save		do.b 1	for 'ungetc" when unbuffered
				do.b 1	Align with C structure (to word & Int Boundry)
_iobuf_size		equ .	Size of _iobuf stucture

 opt l

