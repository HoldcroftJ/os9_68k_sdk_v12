/*********************************************************************** 
* NOTICE
* All files contained on this disk are subject to the licensing conditions
* issued by MOTOROLA Inc.
*
* All files are copyright 1993 by MOTOROLA Inc. 
************************************************************************/

/*
 * tm.h
 * definitions for the TM controllers
 */

/*
 * transmit BD's
 */
#define	T_R	0x8000		/* ready bit */
#define	T_M	0x4000		/* motorola */
#define	T_W	0x2000		/* wrap bit */
#define	T_I	0x1000		/* interrupt on completion */
#define	T_L	0x0800		/* last in frame */
#define	T_CRC	0x0400		/* transmit CRC (when last) */

#define	TM_T_ERROR	0x000f
#define	T_UN	0x0002		/* error: underrun */
#define	T_CT	0x0001		/* error: CTS lost */

/*
 * receive BD's
 */
#define	R_E	0x8000		/* buffer empty */
#define	R_M	0x4000		/* motorola */
#define	R_W	0x2000		/* wrap bit */
#define	R_I	0x1000		/* interrupt on reception */
#define	R_L	0x0800		/* last BD in frame */
#define	R_F	0x0400		/* first BD in frame */

#define	R_ERROR	0x00ff
#define	R_DE	0x0040		/* DPLL error */
#define	R_LG	0x0020		/* frame too long */
#define	R_NO	0x0010		/* non-octet aligned */
#define	R_AB	0x0008		/* received abort sequence */
#define	R_CR	0x0004		/* receive CRC error */
#define	R_OV	0x0002		/* receive overrun */
#define	R_CD	0x0001		/* carrier detect lost */


/*
 * tm interrupts
 */
#define	TM_GLr	0x1000	/* glitch rx */
#define	TM_GLt	0x0800	/* glitch tx */
#define	TM_DCC	0x0400	/* DPLL CS changed */
#define	TM_GRA	0x0080	/* Graceful stop complete */
#define	TM_TXE	0x0010	/* transmit error */
#define	TM_BSY	0x0004	/* busy condition */
#define	TM_TBD	0x0002	/* transmit buffer */
#define	TM_RBD	0x0001	/* receive buffer */
