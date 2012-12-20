#*****************************************************************************
#* make AGFA TrueType font modules in CPU's ASSETS directory.
#*****************************************************************************
#* Copyright 2000-2001 by Microware Systems Corporation                     **
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
#*   1 12/06/00 creation                                          gjb       **
#*   2 02/02/01 Added Benguiat Frisky and Nadianne Bold to list   mas       **
#*   3 02/06/01 Fixed dependencies. Added makdir.  Added utt.ss   djc       **
#*****************************************************************************

TRGTS    =	fonts

FILES    =	albany.ttf albanybd.ttf albanybi.ttf albanyit.ttf \
			cumbb.ttf cumbbi.ttf cumbi.ttf cumbr.ttf \
			thob.ttf thobi.ttf thoi.ttf thor.ttf \
			bengfris.ttf nadiannb.ttf \
                        utt.ss

MWOS     = ../../../../..

include $(MWOS)/MAKETMPL/make.com

SDIR     = .
ODIR     = $(MWOS_OSDIR)/$(CPUDIR)/ASSETS/FONTS/AGFA/TT

OFILES   = $(FILES:%=$(ODIR)/%)

MKDATMOD = mkdatmod


fonts: _makdir $(OFILES)
	$(COMMENT)

for FILE in $(FILES)
$(ODIR)/$(FILE): $(SDIR)/$(FILE)
	$(CODO) $@
	-$(MKDATMOD) -tp=$(CPU) $(SDIR)/$(FILE) $@
endfor

_makdir: $(ODIR)
$(ODIR):
	-$(MAKDIR) $@

_clean: .
	$(COMMENT)

_purge: .
for FILE in $(FILES)
	$(CODO) $(ODIR)/$(FILE)
	-$(DEL) $(ODIR)/$(FILE)
endfor

#*****************************************************************************
