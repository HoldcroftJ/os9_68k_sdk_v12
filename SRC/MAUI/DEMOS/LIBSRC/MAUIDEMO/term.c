/*******************************************************************************
*
* DESCRIPTION:	This is responsible for calling the necessary *_term() functions
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

void demo_term(){

	mem_term();	
	inp_term();
	blt_term();
	txt_term();

}

