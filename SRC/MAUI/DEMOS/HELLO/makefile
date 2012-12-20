# Makefile
#*****************************************************************************
#* makefile to make the MAUI hello demo program                             **
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

TRGTS		=	hello
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/hello.c

hello:  $(ODIR)/STB $(ODIR)/hello
	$(COMMENT)

$(ODIR)/hello:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/hello.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:  
	$(CODO) $(ODIR)/hello
	-$(DEL) $(ODIR)/hello
	$(CODO) $(ODIR)/STB/hello.stb
	-$(DEL) $(ODIR)/STB/hello.stb

$(ODIR)/STB:
	-$(MAKDIR) $@


