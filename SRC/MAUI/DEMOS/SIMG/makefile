# Makefile
#*****************************************************************************
#* makefile to make the MAUI showimg demo program                           **
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

TRGTS		=	showimg
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/show_img.c

showimg:  $(ODIR)/STB $(ODIR)/showimg
	$(COMMENT)

$(ODIR)/showimg:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/showimg.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:  
	$(CODO) $(ODIR)/showimg
	-$(DEL) $(ODIR)/showimg
	$(CODO) $(ODIR)/STB/showimg.stb
	-$(DEL) $(ODIR)/STB/showimg.stb

$(ODIR)/STB:
	-$(MAKDIR) $@
