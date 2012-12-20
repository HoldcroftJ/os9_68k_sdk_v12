/*******************************************************************************
*
* DESCRIPTION:	This is responsible for calling the necessary *_init() functions
*               used in the maudemo.l library.
*
* AUTHOR:		Jason Henson
*
* CREATED ON:	03/15/2000
*
* COPYRIGHT:	Copyright 2000 Microware Systems Corporation. All Rights
*				Reserved. Use and/or distribution subject to terms and
*				conditions of applicable Microware license agreement(s).
*
*/

#include <MAUI/mauidemo.h>
#include <MAUI/maui.h>

void demo_init(){
	
	txt_init();
	blt_init();
	inp_init();
	mem_init();
}


