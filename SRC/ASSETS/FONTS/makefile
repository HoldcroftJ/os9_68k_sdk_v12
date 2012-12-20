#*****************************************************************************
#* make font modules in CPU's ASSETS directory.
#*****************************************************************************
#* Copyright 2001 by Microware Systems Corporation                          **
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
#*   1 02/06/01 creation                                          djc       **
#*****************************************************************************

TRGTS    = fonts

FILES    = bold13.fnt bold25.fnt default.fnt font8x8 norm13.fnt

MWOS     = ../../..

include $(MWOS)/MAKETMPL/make.com

SDIR     = .
ODIR     = $(MWOS_OSDIR)/$(CPUDIR)/ASSETS/FONTS

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
