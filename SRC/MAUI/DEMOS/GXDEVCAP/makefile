# Makefile
#*****************************************************************************
#* makefile to make the MAUI fdraw demo program                             **
#*****************************************************************************
#* Copyright 1996-1998 by Microware Systems Corporation                     **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************

TRGTS		=	gxdevcap
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/gxdevcap.c

gxdevcap:  $(ODIR)/STB $(ODIR)/gxdevcap
	$(COMMENT)

$(ODIR)/gxdevcap:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/gxdevcap.stb
	$(LINK) $(CFILES) -fd=$@

_clean _purge:  .
	$(CODO) $(ODIR)/gxdevcap
	-$(DEL) $(ODIR)/gxdevcap
	$(CODO) $(ODIR)/STB/gxdevcap.stb
	-$(DEL) $(ODIR)/STB/gxdevcap.stb

$(ODIR)/STB:
	-$(MAKDIR) $@


