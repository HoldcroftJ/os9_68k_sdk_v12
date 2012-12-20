/*
 * readswitch50 () - read MVME050 front panel switch or local
 *                   cpu switch/jumper-block to return current
 *                   user selectable options.
 *
 * History.
 *
 * Ed.   Date.     Reason.                                          Who.
 * --  --------  ------------------------------------------------   ---
 * 00  89/06/20  history started.                                   wwb
 *               ---- OS-9/68K V2.3 Release ----
 * 01  90/09/05  converted novme050() into asm code, so that SR's   wwb
 *               Trace Bit can be preserved.
 *               ---- OS-9/68K V2.4 Release ----
 * 02  93/05/18  Modified for Ultra C. Assembly code extracted.     ats
 *               ---- OS-9/68K V3.0 Release ----
 *               ---- OS-9/68K V3.1 Release ----
 */

extern void		novme050();

/*--------------------------------------------------------------!
! readswitch50: Read either the VME050 front panel switches,	!
! or the on-board CPU switches and return them to the caller.	!
!--------------------------------------------------------------*/
u_char readswitch50()
{
 	register u_char	*tflagptr;		/* pointer to trapflag 				*/
 	u_char			tflag_image;	/* image of trapflag 				*/
	u_int32			orgbustrap;		/* original bus trap handler address*/
	u_char			switches;		/* system switches value			*/
	
	/*----------------------------------------------------------------------!
	! Develop a pointer to the REAL address of the debugger's "trapflag"	!
	!----------------------------------------------------------------------*/
	tflagptr = &trapflag - sysb_gptr + (u_char *) exjmptab;

	/*------------------------------------------------------------------!
	! Save and clear the trapflag and install the bus trap handler in	!
	! the exception jump table for the memory search routine.			!
	!------------------------------------------------------------------*/
	tflag_image = *tflagptr;					/* save an image of trapflag*/
	*tflagptr = 0;									/* clear the trapflag	*/
	orgbustrap = setexcpt(BUS_TRAP, novme050);

	/*--------------------------------------------------!
	! Get the switches from the VME050 or the CPU card	!
	!--------------------------------------------------*/
	if (setjmp(gp_btrap) == 0) {
		switches = *((u_char *) SWITCH50);
	} else switches = *((u_char *) CPU_SWITCHES);
	
	/*--------------------------------------------------!
	! reset the original bus error exception handler	!
	! address and restore the debugger's trapflag		!
	!--------------------------------------------------*/
	setexcpt(BUS_TRAP, orgbustrap);
	*tflagptr = tflag_image;

	return switches;
}

/*
 * this is the original C code:
 */
/*
void novme050()
{
	longjmp(gp_btrap, 1);
}
*/

/*
 * this is compiler output for the above, with the inclusion 
 * of the SR preservation.
 */
#ifdef _UCC
_asm(" use <novme050.a>");
#else
@ use <novme050.a>
#endif

/* end of file */

