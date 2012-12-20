/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1996 by Microware Systems Corporation              |
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
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---  |
|  01 96/10/24 Added edition history.                                  rak  |
|              ---- OS-9/68K V3.0.3 System-state Debug Support Release ---  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

typedef struct { /* ROM jump (actually branch) table entry */
	u_int16		opcode,
				offset;
} rbt_entry, *Rbt_Entry;


#define RJT_ENTRY_COUNT 10
#define DEBUG_ENTRY		4

#define BRAS_OP 	0x6000
#define JMPL_OP 	0x4ef9

#define MOVEML_OP1	0x48e7c001
#define MOVEL_OP1	0x203affb2
#define MOVEL_OP2	0x223afff2
#define	JSR_OP		0x4eb9
#define MOVEL_OP3	0x2f400008
#define MOVEML_OP2	0x4cdf0003
#define RTS_OP		0x4e75

#define RJT_SYNC	0xabbafada

typedef struct { /* relocated ROM Jump table entry */
	u_int16		opcode;
	u_int32		address;
} rjt_entry, *Rjt_Entry;


typedef struct { /* replacement ROM jump table */
	rbt_entry	branch[RJT_ENTRY_COUNT];	/* replacement branch table */
	void		*newinfo;					/* pointer to new rominfo table */
	rjt_entry	jumps[RJT_ENTRY_COUNT];		/* actual jump table */
	u_int32		rjt_sync;					/* sync code for new table */
	/* debugger glue follows */
	u_int32		oldebug;		/* old debugger entry point */
	u_int32		moveml_op1;		/* movem.l d0/a7,-(sp) save regs */
	u_int32		movel_op1;		/* move.l newinfo(pc),d0 get newinfo field */
	u_int32		movel_op2;		/* move.l oldebug(pc),d1 get old debug entry */
	u_int16		jsr_op;			/* jsr get_debug_entry */
	u_int32		gde_addr;
	u_int32		movel_op3;		/* move.l d0,8(sp) pass transfer address */
	u_int32     moveml_op2;		/* movem.l (sp)+,d0 restore regs */
	u_int16		rts_op;
} new_rjt, *New_rjt;

