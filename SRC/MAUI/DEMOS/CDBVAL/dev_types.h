/*****************************************************************************
 *
 * Filename: dev_types.h
 *  Purpose: The initialized data for all registered CDB_TYPEs.
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
 ****************************************************************************/


DEV_TYPE g_devs_table[]=
{
	{ "CDB_TYPE_SYSTEM"		, CDB_TYPE_SYSTEM	, DPC_REQUIRED }, /* System */
	{ "CDB_TYPE_CDC"		, CDB_TYPE_CDC		, DPC_OPTIONAL }, /* CD-Control Unit */
	{ "CDB_TYPE_SOUND"		, CDB_TYPE_SOUND	, DPC_OPTIONAL }, /* Sound Processor */
	{ "CDB_TYPE_GRAPHIC"	, CDB_TYPE_GRAPHIC	, DPC_OPTIONAL }, /* Video Output Processor */
	{ "CDB_TYPE_NVRAM"		, CDB_TYPE_NVRAM	, DPC_OPTIONAL }, /* Non-volatile Random Access memory */
	{ "CDB_TYPE_REMOTE"		, CDB_TYPE_REMOTE	, DPC_OPTIONAL }, /* Remote Pointing and Key Devices */
	{ "CDB_TYPE_IROUT"		, CDB_TYPE_IROUT	, DPC_OPTIONAL }, /* IR Output Blaster */
	{ "CDB_TYPE_PIPEDEV"	, CDB_TYPE_PIPEDEV	, DPC_OPTIONAL }, /* Pipe Device */
	{ "CDB_TYPE_SER"		, CDB_TYPE_SER		, DPC_OPTIONAL }, /* SCF Serial Device */
	{ "CDB_TYPE_PRNT"		, CDB_TYPE_PRNT		, DPC_OPTIONAL }, /* SCF Parallel Printer Device */
	{ "CDB_TYPE_MIDI"		, CDB_TYPE_MIDI		, DPC_OPTIONAL }, /* SCF MIDI device */
	{ "CDB_TYPE_LED"		, CDB_TYPE_LED		, DPC_OPTIONAL }, /* LED device */
	{ "CDB_TYPE_RAM"		, CDB_TYPE_RAM		, DPC_OPTIONAL }, /* RAM extensions */
	{ "CDB_TYPE_FLASH"		, CDB_TYPE_FLASH	, DPC_OPTIONAL }, /* FLASH RAM */
	{ "CDB_TYPE_FD"			, CDB_TYPE_FD		, DPC_OPTIONAL }, /* RBF (Universal Format) Floppy Disk */
	{ "CDB_TYPE_HD"			, CDB_TYPE_HD		, DPC_OPTIONAL }, /* RBF Hard Disk */
	{ "CDB_TYPE_PCFD"		, CDB_TYPE_PCFD		, DPC_OPTIONAL }, /* PCF (MS-DOS Format) Floppy Disk */
	{ "CDB_TYPE_PCHD"		, CDB_TYPE_PCHD		, DPC_OPTIONAL }, /* PCF (MS-DOS Format) Hard Disk */
	{ "CDB_TYPE_TAPE"		, CDB_TYPE_TAPE		, DPC_OPTIONAL }, /* SBF Format Magnetic Tape */
	{ "CDB_TYPE_MPV"		, CDB_TYPE_MPV		, DPC_OPTIONAL }, /* MPEG Video */
	{ "CDB_TYPE_MPA"		, CDB_TYPE_MPA		, DPC_OPTIONAL }, /* MPEG Audio */
	{ "CDB_TYPE_ANET"		, CDB_TYPE_ANET		, DPC_OPTIONAL }, /* NFM (Arcnet) Local Area Network */   
	{ "CDB_TYPE_ENET"		, CDB_TYPE_ENET		, DPC_OPTIONAL }, /* IFMAN (Ethernet) Local Area Network */
	{ "CDB_TYPE_ISDN"		, CDB_TYPE_ISDN		, DPC_OPTIONAL }, /* ISM (ISDN) Wide Area Network */
	{ "CDB_TYPE_RTNFM"		, CDB_TYPE_RTNFM	, DPC_OPTIONAL }, /* RTNFM Real-Time Wide Area Network */
	{ "CDB_TYPE_SPF"		, CDB_TYPE_SPF		, DPC_OPTIONAL }, /* SPF Device */
	{ "CDB_TYPE_CTRLCHAN"	, CDB_TYPE_CTRLCHAN	, DPC_OPTIONAL }, /* Control Channel device */
	{ "CDB_TYPE_DATACHAN"	, CDB_TYPE_DATACHAN	, DPC_OPTIONAL }, /* Data Channel device */
	{ "CDB_TYPE_MACFD"		, CDB_TYPE_MACFD	, DPC_OPTIONAL }, /* MACFM floppy disk */
	{ "CDB_TYPE_MACHD"		, CDB_TYPE_MACHD	, DPC_OPTIONAL }, /* MACFM hard disk */
	{ "CDB_TYPE_WIN"		, CDB_TYPE_WIN		, DPC_OPTIONAL }, /* MAUI Win logical device */
	{ (CDB_TYPE)0 }                       /* End of list */
};

DEV_PARAMS g_params_table[]=
{
	/* CDB_TYPE_SYSTEM */
	{ "CP",				PT_STRING,			DPC_OPTIONAL },
	{ "LE",				PT_BOOLEAN,			DPC_OPTIONAL },
	{ "OS",				PT_STRING,			DPC_OPTIONAL },
	{ "RV",				PT_STRING,			DPC_OPTIONAL },
	{ "DV",				PT_STRING,			DPC_OPTIONAL },
	{ "MM",				PT_STRING,			DPC_OPTIONAL },
	{ "SR",				PT_NUMERIC_2_COMMA,	DPC_OPTIONAL },
	{ "GR",				PT_NUMERIC_2_COMMA,	DPC_OPTIONAL },
	{ "ED",				PT_NUMERIC,			DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_CDC */
	{ "SP",				PT_NUMERIC,			DPC_OPTIONAL },
	{ "DV",				PT_NUMERIC,			DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_SOUND */
	{ NULL },

	/* CDB_TYPE_GRAPHIC */
	{ "DE",				PT_NUMERIC,         DPC_OPTIONAL },
	{ "LI",				PT_NUMERIC,         DPC_OPTIONAL },
	{ "AI",				PT_STRING,			DPC_REQUIRED },
	{ "RE",				PT_NUMERIC,         DPC_OPTIONAL },
	{ "GR",				PT_NUMERIC_2_COMMA,	DPC_OPTIONAL },
	{ "PR",				PT_NUMERIC_2_COMMA,	DPC_OPTIONAL },
	{ "PL",				PT_NUMERIC,         DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_NVRAM */
	{ "SZ",				PT_NUMERIC,			DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_REMOTE */
	{ "TY",				PT_STRING,			DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_IROUT */
	{ NULL },

	/* CDB_TYPE_PIPEDEV */
	{ NULL },

	/* CDB_TYPE_SER */
	{ NULL },

	/* CDB_TYPE_PRNT */
	{ NULL },

	/* CDB_TYPE_MIDI */
	{ NULL },

	/* CDB_TYPE_LED */
	{ "DW",				PT_NUMERIC,         DPC_OPTIONAL },
	{ "TY",				PT_STRING,			DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_RAM */
	{ NULL },

	/* CDB_TYPE_FLASH */
	{ NULL },

	/* CDB_TYPE_FD */
	{ NULL },

	/* CDB_TYPE_HD */
	{ NULL },

	/* CDB_TYPE_PCFD */
	{ NULL },

	/* CDB_TYPE_PCHD */
	{ NULL },

	/* CDB_TYPE_TAPE */
	{ NULL },

	/* CDB_TYPE_MPV */
	{ "MV",				PT_STRING,			DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_MPA */
	{ NULL },

	/* CDB_TYPE_ANET */
	{ NULL },

	/* CDB_TYPE_ENET */
	{ NULL },

	/* CDB_TYPE_ISDN */
	{ NULL },

	/* CDB_TYPE_RTNFM */
	{ "SD",				PT_BOOLEAN,			DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_SPF */
	{ NULL },

	/* CDB_TYPE_CTRLCHAN */
	/*
	 * These next two parameters are a special case where
	 * they are required IF the device name is "/enet"
	 */
	{ "SH",				PT_STRING,			DPC_DAR },
	{ "SS",				PT_STRING,			DPC_DAR },
	{ "AP",				PT_STRING,			DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_DATACHAN */
	{ "HD",				PT_BOOLEAN,			DPC_OPTIONAL },
	/*
	 * These next two parameters are a special case where
	 * they are required IF the device name is "/enet"
	 */
	{ "SH",				PT_STRING,			DPC_DAR },
	{ "SS",				PT_STRING,			DPC_DAR },
	{ "AP",				PT_STRING,			DPC_OPTIONAL },
	{ NULL },

	/* CDB_TYPE_MACFD */
	{ NULL },

	/* CDB_TYPE_MACHD */
	{ NULL },

	/* CDB_TYPE_WIN */
	{ NULL }
};

PARAM_VALS g_vals_table[]=
{
	/* CDB_TYPE_SYSTEM */

	/* CP */
	{ NULL },
	/* LE */
	{ NULL },
	/* OS */
	{ "OS9" },
	{ "OS9000" },
	{ NULL },
	/* RV */
	{ NULL },
	/* DV */
	{ NULL },
	/* MM */
	{ NULL },
	/* SR */
	{ NULL },
	/* GR */
	{ NULL },
	/* ED */
	{ NULL },


	/* CDB_TYPE_CDC */

	/* SP */
	{ NULL },
	/* DV */
	{ NULL },


	/* CDB_TYPE_SOUND */

	{ NULL },


	/* CDB_TYPE_GRAPHIC */

	/* DE */
	{ NULL },
	/* LI */
	{ NULL },
	/* AI */
	{ "MAUI" },
	{ NULL },
	/* RE */
	{ NULL },
	/* GR */
	{ NULL },
	/* PR */
	{ NULL },
	/* PL */
	{ NULL },


	/* CDB_TYPE_NVRAM */

	/* SZ */
	{ NULL },


	/* CDB_TYPE_REMOTE */

	/* TY */
	{ "key" },
	{ "ptr" },
	{ "cmb" },
	{ NULL },


	/* CDB_TYPE_IROUT */

	{ NULL },


	/* CDB_TYPE_PIPEDEV */

	{ NULL },


	/* CDB_TYPE_SER */

	{ NULL },


	/* CDB_TYPE_PRNT */

	{ NULL },


	/* CDB_TYPE_MIDI */

	{ NULL },


	/* CDB_TYPE_LED */

	/* DW */
	{ NULL },
	/* TY */
	{ "alpha" },
	{ "numeric" },
	{ "dot" },
	{ NULL },


	/* CDB_TYPE_RAM */

	{ NULL },


	/* CDB_TYPE_FLASH */

	{ NULL },


	/* CDB_TYPE_FD */

	{ NULL },


	/* CDB_TYPE_HD */

	{ NULL },


	/* CDB_TYPE_PCFD */

	{ NULL },


	/* CDB_TYPE_PCHD */

	{ NULL },


	/* CDB_TYPE_TAPE */

	{ NULL },


	/* CDB_TYPE_MPV */

	/* MV */
	{ "MPEG1" },
	{ "MPEG2" },
	{ NULL },


	/* CDB_TYPE_MPA */

	{ NULL },


	/* CDB_TYPE_ANET */

	{ NULL },


	/* CDB_TYPE_ENET */

	{ NULL },


	/* CDB_TYPE_ISDN */

	{ NULL },


	/* CDB_TYPE_RTNFM */

	/* SD */
	{ NULL },


	/* CDB_TYPE_SPF */

	{ NULL },

	/* CDB_TYPE_CTRLCHAN */

	/*
	 * These next three parameters are a special case where
	 * they are required IF the device name is "/enet"
	 */
	/* SH */
	{ NULL },
	/* SS */
	{ NULL },
	/* AP */
	{ NULL },


	/* CDB_TYPE_DATACHAN */

	/* HD */
	{ NULL },

	/*
	 * These next three parameters are a special case where
	 * they are required IF the device name is "/enet"
	 */
	/* SH */
	{ NULL },
	/* SS */
	{ NULL },
	/* AP */
	{ NULL },


	/* CDB_TYPE_MACFD */
	{ NULL },


	/* CDB_TYPE_MACHD */
	{ NULL },


	/* CDB_TYPE_WIN */
	{ NULL }
};


/******************************************************************************/
