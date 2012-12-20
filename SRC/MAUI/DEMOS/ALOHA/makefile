# Makefile
#*****************************************************************************
#* makefile to make the MAUI aloha demo program                             **
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

TRGTS		=	aloha
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/aloha.c

aloha:  $(ODIR)/STB $(ODIR)/aloha
	$(COMMENT)

$(ODIR)/aloha:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/aloha.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:  
	$(CODO) $(ODIR)/aloha
	-$(DEL) $(ODIR)/aloha
	$(CODO) $(ODIR)/STB/aloha.stb
	-$(DEL) $(ODIR)/STB/aloha.stb

$(ODIR)/STB:
	-$(MAKDIR) $@


