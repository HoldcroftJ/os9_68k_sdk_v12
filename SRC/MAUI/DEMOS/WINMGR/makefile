# Makefile
#*****************************************************************************
#* makefile to make the MAUI winmgr demo program                            **
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
#*   1 07/28/97 creation                                          bat       **
#*****************************************************************************

TRGTS		=	winmgr
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/winmgr.c

winmgr:  $(ODIR)/STB $(ODIR)/winmgr
	$(COMMENT)

$(ODIR)/winmgr:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/winmgr.stb
	$(LINK) -oln=$(TRGTS)demo $(CFILES) -fd=$@

_clean _purge:
	$(CODO) $(ODIR)/winmgr
	-$(DEL) $(ODIR)/winmgr
	$(CODO) $(ODIR)/STB/winmgr.stb
	-$(DEL) $(ODIR)/STB/winmgr.stb

$(ODIR)/STB:
	-$(MAKDIR) $@
