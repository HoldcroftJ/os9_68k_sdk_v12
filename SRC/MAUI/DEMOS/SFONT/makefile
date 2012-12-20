# Makefile
#*****************************************************************************
#* makefile to make the MAUI sfont demo program                             **
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

TRGTS		=	sfont
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/sfont.c

COPT		= -g

sfont:  $(ODIR)/STB $(ODIR)/sfont
	$(COMMENT)

$(ODIR)/sfont:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/sfont.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:  
	$(CODO) $(ODIR)/sfont
	-$(DEL) $(ODIR)/sfont
	$(CODO) $(ODIR)/STB/sfont.stb
	-$(DEL) $(ODIR)/STB/sfont.stb

$(ODIR)/STB:
	-$(MAKDIR) $@
