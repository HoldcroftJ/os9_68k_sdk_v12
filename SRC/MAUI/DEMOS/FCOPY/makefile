# Makefile
#*****************************************************************************
#* makefile to make the MAUI fcopy demo program                             **
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

TRGTS		=	fcopy
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/fcopy.c

fcopy:  $(ODIR)/STB $(ODIR)/fcopy
	$(COMMENT)

$(ODIR)/fcopy:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/fcopy.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:  
	$(CODO) $(ODIR)/fcopy
	-$(DEL) $(ODIR)/fcopy
	$(CODO) $(ODIR)/STB/fcopy.stb
	-$(DEL) $(ODIR)/STB/fcopy.stb

$(ODIR)/STB:
	-$(MAKDIR) $@


