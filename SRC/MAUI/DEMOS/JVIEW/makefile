# Makefile
#*****************************************************************************
#* makefile to make the MAUI jview demo program                            **
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

TRGTS		=	jview
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/jview.c

jview:  $(ODIR)/STB $(ODIR)/jview  # $(ODIR)/mt_jview
	$(COMMENT)

$(ODIR)/jview:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/jview.stb
	$(LINK) $(CFILES) -v=../LIBSRC/JPEG/SRC/CORELIB -l=$(MWOS_LIBDIR)/maui_jpg.l -y=$(MWOS_ILIBDIR)/maui_jpg.il -fd=$@

#$(ODIR)/mt_jview:  $(CFILES) $(LIBS_DEP) ./$(MAKENAME)
#	$(CODO) $@
#	$(CODO) $(ODIR)/STB/mt_jview.stb
#	$(LINK) -mt $(CFILES) -v=../LIBSRC/JPEG/SRC/CORELIB -l=$(MWOS_LIBDIR)/maui_jpg.l -y=$(MWOS_ILIBDIR)/maui_jpg.il -fd=$@

_clean _purge:
	$(CODO) $(ODIR)/jview
	-$(DEL) $(ODIR)/jview
	$(CODO) $(ODIR)/STB/jview.stb
	-$(DEL) $(ODIR)/STB/jview.stb
#	$(CODO) $(ODIR)/mt_jview
#	-$(DEL) $(ODIR)/mt_jview
#	$(CODO) $(ODIR)/STB/mt_jview.stb
#	-$(DEL) $(ODIR)/STB/mt_jview.stb

$(ODIR)/STB:
	$(MAKDIR) $@
