# Makefile
#*****************************************************************************
#* makefile to make the MAUI msginfo demo program                           **
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

TRGTS		=	msginfo
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/msginfo.c

msginfo:  $(ODIR)/STB $(ODIR)/msginfo
	$(COMMENT)

$(ODIR)/msginfo:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/msginfo.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:  
	$(CODO) $(ODIR)/msginfo
	-$(DEL) $(ODIR)/msginfo
	$(CODO) $(ODIR)/STB/msginfo.stb
	-$(DEL) $(ODIR)/STB/msginfo.stb

$(ODIR)/STB:
	-$(MAKDIR) $@
