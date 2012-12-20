# Makefile
#*****************************************************************************
#* makefile to make the MAUI windraw demo program                           **
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

TRGTS		=	windraw
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/windraw.c

windraw:  $(ODIR)/STB $(ODIR)/windraw
	$(COMMENT)

$(ODIR)/windraw:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/windraw.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:
	$(CODO) $(ODIR)/windraw
	-$(DEL) $(ODIR)/windraw
	$(CODO) $(ODIR)/STB/windraw.stb
	-$(DEL) $(ODIR)/STB/windraw.stb

$(ODIR)/STB:
	-$(MAKDIR) $@
