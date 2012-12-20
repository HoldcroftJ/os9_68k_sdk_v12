# Makefile
#*****************************************************************************
#* makefile to make the MAUI inp demo program                               **
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

TRGTS		=	inp
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/inp.c

inp:  $(ODIR)/STB $(ODIR)/inp
	$(COMMENT)

$(ODIR)/inp:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/inp.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:  
	$(CODO) $(ODIR)/inp
	-$(DEL) $(ODIR)/inp
	$(CODO) $(ODIR)/STB/inp.stb
	-$(DEL) $(ODIR)/STB/inp.stb

$(ODIR)/STB:
	-$(MAKDIR) $(ODIR)/STB
