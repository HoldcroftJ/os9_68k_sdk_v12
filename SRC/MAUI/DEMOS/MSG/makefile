# Makefile
#*****************************************************************************
#* makefile to make the MAUI msgrdr/msgwrtr demo programs                   **
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

TRGTS		=	msg
MAKENAME	=	makefile

include ../maui.com

HFILES		=	$(SDIR)/_msg.h

msg:  $(ODIR)/STB $(ODIR)/msgrdr $(ODIR)/msgwrtr
	$(COMMENT)

$(ODIR)/msgrdr:  $(SDIR)/msgrdr.c $(HFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/msgrdr.stb
	$(LINK) $(SDIR)/msgrdr.c -fd=$@
$(ODIR)/msgwrtr:  $(SDIR)/msgwrtr.c $(HFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/msgwrtr.stb
	$(LINK) $(SDIR)/msgwrtr.c -fd=$@

_clean _purge:  
	$(CODO) $(ODIR)/msgrdr
	-$(DEL) $(ODIR)/msgrdr
	$(CODO) $(ODIR)/STB/msgrdr.stb
	-$(DEL) $(ODIR)/STB/msgrdr.stb
	$(CODO) $(ODIR)/msgwrtr
	-$(DEL) $(ODIR)/msgwrtr
	$(CODO) $(ODIR)/STB/msgwrtr.stb
	-$(DEL) $(ODIR)/STB/msgwrtr.stb

$(ODIR)/STB:
	-$(MAKDIR) $@
