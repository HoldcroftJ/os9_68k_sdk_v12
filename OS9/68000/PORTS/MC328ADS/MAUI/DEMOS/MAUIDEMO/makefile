# Makefile
#*****************************************************************************
#* Demo program to show how to output text to a drawmap in MAUI and         **
#* wait for user input.                                                     **
#*****************************************************************************
#* Copyright 1996 by Microware Systems Corporation                          **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************
#* Edition History:                                                         **
#* #   Date     Comments                                          By        **
#* --- -------- ------------------------------------------------- --------- **
#*   1 03/15/96 creation                                          bat       **
#*   2 05/23/96 modified for use in port directory                rws       **
#*   3 08/17/99 Improvements for Enhanced 68K 1.0 release         srm       **
#*****************************************************************************

PORT        =   ../../..
TRGTS       =   mauidemo

include $(PORT)/../make.com

SDIR        =   $(MWOS)/SRC/WIRELESS/DEMO/MAUIDEMO
ODIR        =   $(PORT)/CMDS
LDIR        =   $(MWOS_LIBDIR)

DEFINES     = -dTEST_2BIT_DRAW \
				-dTEST_1BIT_DRAW \
				-dTEST_2BIT_IMG \
				-dTEST_1BIT_IMG
#          -dTEST_1BIT_TEXT
#          -dTEST_1BIT_ALL

DEFS        =   $(MWOS_DEFS) -v=$(MWOS)/OS9/68000/PORTS/MC328ADS/DEFS \
				-v=$(SDIR)
LIBS        =   -l=$(MWOS_LIBDIR)/maui.l -l=$(MWOS_LIBDIR)/mfm.l \
				$(CONV_LIBS) $(MWOS_CSLLIBS) 
FILES       =   $(CSTART) $(SDIR)/main.c $(SDIR)/ucmfont.c $(SDIR)/load.c

OPTMZ       =   -o=7 -cw

LINK        =   $(LINKER) $(LPARMS) $(DEFINES) $(DEFS) $(LIBS) $(OPTMZ) \
				$(COPT_JMPTBL) -olS -k $(LOPTS)

$(TRGTS): DIRS $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS): $(FILES) ./makefile
	-$(CODO) $@
	-$(CODO) $(ODIR)/STB/$*.stb
	-$(CODO) $(ODIR)/STB/$*.dbg
	$(LINK) $(FILES) -fd=$@

DIRS: .
	-$(MAKDIR) $(ODIR)
	-$(MAKDIR) $(ODIR)/STB

_clean _purge: .
	-$(CODO) $(ODIR)/$(TRGTS)
	-$(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(CODO) $(ODIR)/STB/$(TRGTS).dbg
	-$(DEL) $(ODIR)/$(TRGTS)
	-$(DEL) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL) $(ODIR)/STB/$(TRGTS).dbg
