
/*******************************************************************************
 *
 * DESCRIPTION:	Common function header used in both play and record
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */


#include <MAUI/mfm_snd.h>
#include <modes.h>
#include <types.h>
#include <sg_codes.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <string.h>
#include <const.h>

#define eprint(expr) printf (#expr " = %s\n", strerror(expr))
#define dprint(expr) printf (#expr " = %d\n", expr)
#define xprint(expr) printf (#expr " = 0x%x\n", expr)
#define strprint(expr) printf (#expr " = '%s'\n", expr)
#define bprint(expr) printf (#expr " = %s\n", expr ? "TRUE" : "FALSE")
#define cmprint(expr) print_coding_method (#expr " = ", expr)

extern BOOLEAN printdevcaps;
extern BOOLEAN printstatus;
extern BOOLEAN verbose;

/*******************************************************************************
 * Prototypes
 *******************************************************************************/
void print_fileinfo(SND_SMAP *smap);
void print_smap(SND_SMAP *smap, char *title);
void print_snd_status(path_id path);
void print_snd_devcap(SND_DEV_CAP *devcap);
int errmsg(error_code err, char *msg, ...);
