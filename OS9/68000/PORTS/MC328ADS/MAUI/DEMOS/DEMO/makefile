# Makefile
#*****************************************************************************
#* Embedded Systems Demo for MC68328ADS
#*****************************************************************************
#* Copyright 1995 by Microware Systems Corporation                          **
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
#*   1 12/15/95 creation                                          bat       **
#*   2 08/17/99 Improvements for Enhanced 68K 1.0 release         srm       **
#*****************************************************************************

PORT      =   ../../..
TRGTS     =   demo

include $(PORT)/../make.com

SDIR    = $(MWOS)/SRC/WIRELESS/DEMO/DEMO328
ODIR    = $(PORT)/CMDS
LDIR    = $(MWOS_LIBDIR)

DEFS    = $(MWOS_DEFS) -v=$(MWOS)/OS9/68000/PORTS/MC328ADS/DEFS \
			-v=$(SDIR)
LIBS    = -l=$(MWOS_LIBDIR)/mauilib.l -l=$(MWOS_LIBDIR)/mfm.l \
			$(CONV_LIBS) $(MWOS_CSLLIBS) 
FILES   = $(CSTART) $(SDIR)/demo.c $(SDIR)/load.c $(SDIR)/menu_funcs.c
DEFINES =   -dDEBUG
OPTMZ   =   -o=7 -cw
LINK    =   $(LINKER) $(LPARMS) $(DEFINES) $(DEFS) $(LIBS) $(OPTMZ) \
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
