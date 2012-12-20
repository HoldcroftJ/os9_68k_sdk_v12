# Makefile
#*****************************************************************************
#* makefile to make the MAUI winink demo program                            **
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

TRGTS		=	winink
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/winink.c

winink:  $(ODIR)/STB $(ODIR)/winink
	$(COMMENT)

$(ODIR)/winink:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/winink.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:
	$(CODO) $(ODIR)/winink
	-$(DEL) $(ODIR)/winink
	$(CODO) $(ODIR)/STB/winink.stb
	-$(DEL) $(ODIR)/STB/winink.stb

$(ODIR)/STB:
	-$(MAKDIR) $@
