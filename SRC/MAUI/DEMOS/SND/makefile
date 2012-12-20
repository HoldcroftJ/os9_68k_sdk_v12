# Makefile
#*****************************************************************************
#* makefile to make the MAUI auplay/aurecord demo programs                  **
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

TRGTS		=	snd
MAKENAME	=	makefile

include ../maui.com

CFILES		=	$(SDIR)/common.c \
				$(SDIR)/fmt_com.c \
				$(SDIR)/fmt_snd.c \
				$(SDIR)/fmt_wav.c
IRELS		=	$(IDIR)/common.i \
				$(IDIR)/fmt_com.i \
				$(IDIR)/fmt_snd.i \
				$(IDIR)/fmt_wav.i
HFILES		=	$(SDIR)/common.h \
				$(SDIR)/fmt_com.h \
				$(SDIR)/fmt_snd.h \
				$(SDIR)/fmt_wav.h

LOCAL_LIBS	=	-l=$(MWOS_DFTLIB)/mfm.l -ill=$(MWOS_ILIBDIR)/mfm.il

OPT =
if $(CPU) == "ARMV3"
OPT += -bepv # armv3 does not support volatile halfwords
endif

snd:  $(ODIR)/STB $(IDIR) $(ODIR)/auplay $(ODIR)/aurecord
	$(COMMENT)
$(ODIR)/auplay: $(IDIR)/auplay.i $(IRELS)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/auplay.stb
	$(LINK) $(IDIR)/auplay.i $(IRELS) $(LOCAL_LIBS) -fd=$@ $(OPT)
$(ODIR)/aurecord: $(IDIR)/aurecord.i $(IRELS)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/aurecord.stb
	$(LINK) $(IDIR)/aurecord.i $(IRELS) $(LOCAL_LIBS) -fd=$@ $(OPT)

$(IDIR)/common.i : $(SDIR)/common.c
	$(COMPILE) $(SDIR)/common.c $(OPT)
$(IDIR)/fmt_com.i : $(SDIR)/fmt_com.c
	$(COMPILE) $(SDIR)/fmt_com.c $(OPT)
$(IDIR)/fmt_snd.i : $(SDIR)/fmt_snd.c
	$(COMPILE) $(SDIR)/fmt_snd.c $(OPT)
$(IDIR)/fmt_wav.i : $(SDIR)/fmt_wav.c
	$(COMPILE) $(SDIR)/fmt_wav.c $(OPT)
$(IDIR)/auplay.i : $(SDIR)/auplay.c
	$(COMPILE) $(SDIR)/auplay.c $(OPT)
$(IDIR)/aurecord.i : $(SDIR)/aurecord.c
	$(COMPILE) $(SDIR)/aurecord.c $(OPT)

_clean: .
	-$(DEL) $(IDIR)/*.i

_purge: .
	$(CODO) $(ODIR)/auplay
	-$(DEL) $(ODIR)/auplay
	$(CODO) $(ODIR)/STB/auplay.stb
	-$(DEL) $(ODIR)/STB/auplay.stb
	$(CODO) $(ODIR)/aurecord
	-$(DEL) $(ODIR)/aurecord
	$(CODO) $(ODIR)/STB/aurecord.stb
	-$(DEL) $(ODIR)/STB/aurecord.stb

$(ODIR)/STB $(IDIR):
	-$(MAKDIR) $@
