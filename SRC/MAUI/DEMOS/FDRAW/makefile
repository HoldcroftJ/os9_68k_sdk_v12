# Makefile
#*****************************************************************************
#* makefile to make the MAUI fdraw demo program                             **
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

TRGTS		=	fdraw
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/fdraw.c

fdraw:  $(ODIR)/STB $(ODIR)/fdraw
	$(COMMENT)

$(ODIR)/fdraw:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/fdraw.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:  
	$(CODO) $(ODIR)/fdraw
	-$(DEL) $(ODIR)/fdraw
	$(CODO) $(ODIR)/STB/fdraw.stb
	-$(DEL) $(ODIR)/STB/fdraw.stb

$(ODIR)/STB:
	-$(MAKDIR) $@


