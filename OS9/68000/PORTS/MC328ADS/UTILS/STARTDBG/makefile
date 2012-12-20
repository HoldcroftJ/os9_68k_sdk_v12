# Makefile
#*****************************************************************************
#* Start up user/system state debugging
#*****************************************************************************
#* Copyright 1996-1997 by Microware Systems Corporation                     **
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
#*   1 04/15/96 creation                                          bat       **
#*   2 04/13/99 support MAKDIR and CODO                           srm       **
#*****************************************************************************

PORT            =       ../..
ONAME           =       startdbg
TRGTS           =       $(ONAME)

include $(PORT)/../make.com

SDIR            =       .
ODIR            =       $(PORT)/CMDS

DEFS            =       $(MWOS_DEFS)
LIBS            =       $(MWOS_CSLLIBS)
FILES           =       $(CSTART) $(SDIR)/main.c
DEFINES         =       
OPTMZ           =       -o=7 -cw
LINK            =       $(LINKER) $(LPARMS) $(LIBS) $(OPTMZ) $(COPT_JMPTBL) \
						-olS -k $(LOPTS) $(COPT_GU0)

$(ONAME): _makdir $(ODIR)/$(ONAME)
	$(COMMENT)

$(ODIR)/$(ONAME): $(FILES)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(ONAME).stb
	$(LINK) $(FILES) -fd=$@ -oln=$(ONAME)

_makdir: .
	$(MAKDIR) $(ODIR)/STB

_clean _purge: .
	$(CODO) $(ODIR)/$(ONAME)
	-$(DEL) $(ODIR)/$(ONAME)
	$(CODO) $(ODIR)/STB/$(ONAME).stb
	-$(DEL) $(ODIR)/STB/$(ONAME).stb
	$(CODO) $(ODIR)/STB/$(ONAME).dbg
	-$(DEL) $(ODIR)/STB/$(ONAME).dbg


