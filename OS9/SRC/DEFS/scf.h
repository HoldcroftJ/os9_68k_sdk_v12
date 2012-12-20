#if !defined(_SCF_H)
#define _SCF_H

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
|  02 98/11/16 Fixed type of v_resv2 (u_char -> short).                wwb	|
|				---- OS-9/68K V3.1 Release ----								|
|  03 00/05/04 Add Baud rate entries, Fix to match io.a baud rates     mgh  |
|                minor adjustments to midi (bug), match to 9000 Values      |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/


#if !defined(_SYSIO_H)
#include <sysio.h>
#endif /* _SYSIO_H */

/*
	SCF static storage.  Required in every SCF driver.
*/
typedef struct scfstatic {
	sysioStatic		v_sysio;/* kernel static storage variables */
	struct scfstatic
				*v_dev2;	/* attached device's static storage ptr */
	char		v_type,		/* device type or parity */
				v_line,		/* lines left until end of page */
				v_pause,	/* immediate pause request */
				v_intr,		/* keyboard interrupt char */
				v_quit,		/* keyboard quit char */
				v_pchr,		/* pause character */
				v_err,		/* accumulated errors */
				v_xon,		/* x-on character */
				v_xoff;		/* x-off character */
	short		v_resv1[5];	/* reserved (used by pipeman) */
	u_char		v_hangup;	/* set to non-zero when data carrier is lost */
	u_char		v_pad1;		/* spare */
	short		v_resv2[6];	/* reserved */
} *Scfstatic;
		
/*
	Sequential character file manager PD definitions
*/
struct scf_pvt {
	Devicetbl	pd_dev2;		/* device tbl ptr for echo device */
	u_short		pd_max;			/* readline high byte count */
	u_char		pd_raw;			/* read/write or Rdlin/wrlin mode */
	u_char		pd_min;			/* devices are owned if clear */
	u_char		pd_lost;		/* non zero if path is dead */
	u_char		pd_scfres[13];	/* reserved for SCF usage */
};

struct scf_opt {
	u_char		pd_dtp;			/* device type */
	u_char		pd_upc;			/* case (0=both, any other value=upper only) */
	u_char		pd_bso;			/* backsp (0=bse, any other value=bse,sp,bse) */
	u_char		pd_dlo;			/* delete (0=bse over line, any other value=crlf) */
	u_char		pd_eko;			/* echo (0=no echo) */
	u_char		pd_alf;			/* autolf (0=no auto lf) */
	u_char		pd_nul;			/* end of line null count */
	u_char		pd_pau;			/* pause (0=no end of page pause) */
	u_char		pd_pag;			/* lines per page */
	u_char		pd_bsp;			/* backspace character */
	u_char		pd_del;			/* delete line character */
	u_char		pd_eor;			/* end of record char (read only) */
	u_char		pd_eof;			/* end of file char */
	u_char		pd_rpr;			/* reprint line char */
	u_char		pd_dup;			/* dup last line char */
	u_char		pd_psc;			/* pause char */
	u_char		pd_int;			/* kbd intr char (ctl c) */
	u_char		pd_qut;			/* kbd quit char (ctl q) */
	u_char		pd_bse;			/* backspace echo character */
	u_char		pd_ovf;			/* line overflow char (bell) */
	u_char		pd_par;			/* parity code */
	u_char		pd_bau;			/* acia baud rate (color computer) */
	short		pd_d2p;			/* dev2 output name offset */
	u_char		pd_xon;			/* x-on char */
	u_char		pd_xoff;		/* x-off char */
	u_char		pd_Tab;			/* Tab character (0=none) */
	u_char		pd_Tabs;		/* Tab field size */
	Devicetbl	pd_tbl;			/* Device table addr (copy) */
	u_short		pd_Col;			/* current column number */
	u_char		pd_err;			/* most recent I/O error status */
	u_char		pd_scfres2[61];	/* reserved for SCF usage */
	u_char		pd_gfmucm[32];	/* reserved for GFM/UCM usage */
};


/*
	 SCF device baud rate codes (used with pd__bau)
*/
#define BAUD50		0x00	/* baud rate =      50 */
#define BAUD75		0x01	/* baud rate =      75 */
#define BAUD110		0x02	/* baud rate =     110 */
#define BAUD134_5	0x03	/* baud rate =     134.5 */
#define BAUD150		0x04	/* baud rate =     150 */
#define BAUD300		0x05	/* baud rate =     300 */
#define BAUD600		0x06	/* baud rate =     600 */
#define BAUD1200	0x07	/* baud rate =    1200 */
#define BAUD1800	0x08	/* baud rate =    1800 */
#define BAUD2000	0x09	/* baud rate =    2000 */
#define BAUD2400	0x0a	/* baud rate =    2400 */
#define BAUD3600	0x0b	/* baud rate =    3600 */
#define BAUD4800	0x0c	/* baud rate =    4800 */
#define BAUD7200	0x0d	/* baud rate =    7200 */
#define BAUD9600	0x0e	/* baud rate =    9600 */
#define BAUD19200	0x0f	/* baud rate =   19200 */
#define BAUD38400	0x10	/* baud rate =   38400 */
#define	BAUD56000	0x11	/* baud rate =   56000 */
#define	BAUD64000	0x12	/* baud rate =   64000 */
#define BAUD31250	0x13	/* baud rate =   31250 (MIDI) */
#define BAUDMIDI	0x13	/* baud rate =   31250 (MIDI) */
#define OLDMIDI		0x14    /* baud rate =   31250 (MIDI) */
#define	BAUD57600	0x15	/* baud rate =   57600 */
/* Keep ALL VALUES BELOW MATCHED TO OS9000 RATES   */
#define	BAUD115200	0x16	/* baud rate =  115200 */
#define BAUD230400  0x17	/* baud rate =  230400 */
#define BAUD460800  0x18	/* baud rate =  460800 */
#define BAUD921600  0x19	/* baud rate =  921600 */
#define	BAUD76800	0x1a	/* baud rate =   76800 */
#define BAUD153600  0x1b	/* baud rate =  153600 */
#define BAUD307200  0x1c	/* baud rate =  307200 */
#define BAUD614400  0x1d	/* baud rate =  614400 */
#define BAUD1228800 0x1e	/* baud rate = 1228800 */

/* Baud rate codes 0x1f - 0x7F are undefined and reserved */
/* Baud rate codes 0x80 - 0xFE may be user defined */

#define EXTERNAL	0xFF	/* baud rate is externally produced */

/*
	SCF device parity codes (used with pd_par, bits 1,0)
*/
#define NOPARITY		(0<<0)
#define ODDPARITY		(1<<0)
/* reserved				(2<<0) */
#define EVENPARITY		(3<<0)

/*
	SCF device wordsize codes (used with pd_par, bits 3,2)
*/
#define WORDSIZE8		(0<<2)
#define WORDSIZE7		(1<<2)
#define WORDSIZE6		(2<<2)
#define WORDSIZE5		(3<<2)

/*
	SCF device stop bits codes (used with pd_par, bits 5,4)
*/
#define ONESTOP			(0<<4)
#define ONE_5STOP		(1<<4)
#define TWOSTOP			(2<<4)
/* reserved				(3<<4) */

/* bits 7,6 of pd_par are reserved */

#define SCFMAN	1
#define	scfpvt	path.fm_pvt.scf
#define scfopt	path.fm_opt.scf

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(_ANSI_EXT) || defined(__STDC__) || defined(__cplusplus)
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

error_code _os_ss_dcoff _OP((path_id, signal_code));
error_code _os_ss_dcon _OP((path_id, signal_code));
error_code _os_ss_dsrts _OP((path_id));
error_code _os_ss_enrts _OP((path_id));

#if defined(_OPT_PROTOS)
int _ss_dcoff _OP((int, int));
int _ss_dcon _OP((int, int));
int _ss_dsrts _OP((int));
int _ss_enrts _OP((int));
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _SCF_H */
