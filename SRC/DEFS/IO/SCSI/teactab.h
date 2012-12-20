/*
 * teactab - Teac setup tables
*/

/*--------------------------------------------------------------------------,
|                                                                           |
|             Copyright 1996-1998 by Microware Systems Corporation          |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:														    |
| #   Date    Comments                                                By    |
| -- -------- ------------------------------------------------------- ---   |
| 01 96/02/03 Teac support via tables added                           GbG   |
|              ---- OS-9/68K V3.0.3 Released ----                           |
|              ---- OS-9000/x86 V2.1 Released ----                          |
|              ---- OS-9000/PPC V2.1.1 Released ----                        |
| 02 97/03/26 Removed extra const.                                    GbG   |
|              ---- OS-9000/ARMv3 V2.2 Released ----                        |
|              ---- OS-9000/ARMv3 V2.2.1 Released ----                      |
| 03 98/01/31 Fixed compiler warnings.                                GbG   |
|              ---- OS-9000/SH3 V2.2.6 Released ----                        |
|              ---- SCSI Common Sub-Component v1.0 Released ----            |
|              $$                    <RELEASE_INFO>                     $$  |
|--------------------------------------------------------------------------*/

/*
 * To keep the Teac Driver in sync with the format table
 * the following define must match the define in "misc.c",
 * namely "TEAC_SYNC".
*/

#define TEAC_TABLE_SYNC 1

/*
 * Teac drive differences are now handled via tables
 * to add support for a new Teac drive simply include the
 * pin setup information as described in the "teac_fmts"
 * structure.
*/

typedef const struct teac_fmts {
	char	const	*dseries;		/* drive series FC-1 FC-5 */
	char	const	*dtype;			/* drive type "HF" "JHF" */
	u_int8	const	fc_firm;		/* firmware origin 0=USA 1=WORLD */
	u_int8	const	trdy;			/* ready state */
	u_int8	const	pin4;			/* pin1 and pin4 */
	u_int8	const	pin34_250;		/* pin2 and pin34 - rate 250 */
	u_int8	const	medtype_250;	/* media type - rate 250 */
	u_int8	const	pin34_500;		/* pin2 and pin34 - rate 500 */
	u_int8	const	medtype_500; 	/* media type - rate 500 */
	u_int8	const	pin34_1000;		/* pin2 and pin34 - rate 1000 */
	u_int8	const	medtype_1000; 	/* media type - rate 1000 */
} teac_fmts, *Teac_fmts;


#define MAX_TEAC_FMTS 100

teac_fmts teac_desc[] = {
		
	/* Spare type : ( User Patchable ) */

	{ "XX-X", "XXX", 0xa0, 
		0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,	},

	/* GF type : USA Version (untested) */

	{ "FC-1", "GF ", 1, 
		0,								/* trdy */
		(MS_PIN4 << 4)  | MS_PIN1,		/* pin 1 & pin 4 */
		(MS_PIN34 << 4) | MS_PIN2LD,	/* Pin2 and Pin 34 Rate:250 */
		MS_LD,							/* Media Type 250 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:500 */
		MS_HD,							/* Media Type 500 */
		0xff,							/* (NA) Pin2 and Pin 34 Rate:1000 */
		0xff,							/* (NA) Media Type 1000 */
		},

	/* GF type : World Version (tested) */
	
	{ "FC-1", "GF ", 0, 
		0,								/* trdy */
		(MS_PIN4 << 4)  | MS_PIN1,		/* pin 1 & pin 4 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:250 */
		MS_LD,							/* Media Type 250 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:500 */
		MS_HD,							/* Media Type 500 */
		0xff,							/* (NA) Pin2 and Pin 34 Rate:1000 */
		0xff,							/* (NA) Media Type 1000 */
		},

	/* HF type : USA Version (tested) */

	{ "FC-1", "HF ", 1, 
		0,								/* trdy */
		(MS_PIN4 << 4)  | MS_PIN1,		/* pin 1 & pin 4 */
		(MS_PIN34 << 4) | MS_PIN2LD,	/* Pin2 and Pin 34 Rate:250 */
		MS_LD,							/* Media Type 250 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:500 */
		MS_HD,							/* Media Type 500 */
		0xff,							/* (NA) Pin2 and Pin 34 Rate:1000 */
		0xff,							/* (NA) Media Type 1000 */
		},

	/* HF type : World Version (tested) */
	
	{ "FC-1", "HF ", 0, 
		0,								/* trdy */
		(MS_PIN4 << 4)  | MS_PIN1,		/* pin 1 & pin 4 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:250 */
		MS_LD,							/* Media Type 250 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:500 */
		MS_HD_ER,						/* Media Type 500 */
		0xff,							/* (NA) Pin2 and Pin 34 Rate:1000 */
		0xff,							/* (NA) Media Type 1000 */
		},

	/* JHF type : USA Version (untested) */

	{ "FC-1", "JHF", 1, 
		0,								/* trdy */
		(MS_PIN4 << 4)  | MS_PIN1,		/* pin 1 & pin 4 */
		(MS_PIN34 << 4) | MS_PIN2LD,	/* Pin2 and Pin 34 Rate:250 */
		MS_LD,							/* Media Type 250 */
		(MS_PIN34 << 4) | MS_PIN2LD,	/* Pin2 and Pin 34 Rate:500 */
		MS_HD,							/* Media Type 500 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:1000 */
		MS_ED,							/* Media Type 1000 */
		},

	/* JHF type : World Version (tested) */

	{ "FC-1", "JHF", 0,
		0,								/* trdy */
		(MS_PIN4 << 4)  | MS_PIN1,		/* pin 1 & pin 4 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:250 */
		MS_LD,							/* Media Type 250 */
		(MS_PIN34 << 4) | MS_PIN2LD,	/* Pin2 and Pin 34 Rate:500 */
		MS_HD_ER,						/* Media Type 500 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:1000 */
		MS_ED,							/* Media Type 1000 */
		},

	/* JGF type : World Version (tested) */

	{ "FC-1", "JGF", 0,
		0,								/* trdy */
		(MS_PIN4 << 4)  | MS_PIN1,		/* pin 1 & pin 4 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:250 */
		MS_LD,							/* Media Type 250 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:500 */
		MS_HD,							/* Media Type 500 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:1000 */
		MS_ED,							/* Media Type 1000 */
		},

	/* FC-1 HGF type : World Version (untested) */

	{ "FC-1", "HGF", 0, 
		0, 								/* trdy */
		(MS_PIN4 << 4)  | MS_PIN1,		/* pin 1 & pin 4 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:250 */
		MS_LD,							/* Media Type 250 */
		(MS_PIN34 << 4)	| MS_PIN2HD,	/* Pin2 and Pin 34 Rate:500 */
		MS_HD_ER,						/* Media Type 500 */
		0xff,							/* (NA) Pin2 and Pin 34 Rate:1000 */
		0xff,							/* (NA) Media Type 1000 */
		},

	/* FC-5 HGF type : World Version (tested) */
	
	{ "FC-5", "HGF", 0, 
		0x80, 							/* trdy */
		(MS_PIN4_LS << 4)  | MS_PIN1_1_6M,	/* pin 1 & pin 4 */
		(MS_PIN34 << 4) | MS_PIN2HD,	/* Pin2 and Pin 34 Rate:250 */
		MS_LD,							/* Media Type 250 */
		(MS_PIN34 << 4)	| MS_PIN2HD,	/* Pin2 and Pin 34 Rate:500 */
		MS_HD_ER,						/* Media Type 500 */
		0xff,							/* (NA) Pin2 and Pin 34 Rate:1000 */
		0xff,							/* (NA) Media Type 1000 */
		},

	/* WARNING: DO NOT DELETE THE FOLLOWING ENTRY. */

	{ "END ", "END ", 0, 0, 0, 0, 0,	0, 0, 0, 0,	}

};


