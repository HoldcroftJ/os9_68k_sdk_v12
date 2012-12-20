/*
 * MVME050 Definitions file.
 *
 * Ed.   Date.     Reason.                                          Who.
 * --  --------  ------------------------------------------------   ---
 * 00  89/06/20  history started.                                   wwb
 *               ---- OS-9/68K V2.3 Release ----
 *               ---- OS-9/68K V2.4 Release ----
 *               ---- OS-9/68K V3.0 Release ----
 *               ---- OS-9/68K V3.1 Release ----
 */

#ifndef SHORTIOBASE
#define SHORTIOBASE		0xFFFF0000	/* base address of "short i/o" space */
#endif

#define VME50BASE	SHORTIOBASE+0x1000	/* VME050 base address			*/
#define DISPLAY50	VME50BASE+0xA1	/* Display module, VME050			*/
#define SWITCH50	DISPLAY50		/* Front panel switch, VME050		*/
