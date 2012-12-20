/*******************************************************************************
*
* DESCRIPTION:	Common functions for demo programs. The caller can use demo_init()
*               and demo_term() before calling functions in this file, or else they are 
*		responsible for calling the necessary individual *_init() functions 
*		needed before calling functions in this file.
*
*
* AUTHOR:		Todd Earles
*
* CREATED ON:	02/06/96
*
* COPYRIGHT:	Copyright 1994 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
*/
#ifndef _DEMOCOM_H
#define _DEMOCOM_H
#include <MAUI/mauidemo.h>

error_code demo_error(error_code ec, char *str, ...);

#endif /* _DEMOCOM_H */
