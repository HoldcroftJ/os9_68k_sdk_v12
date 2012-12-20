#*****************************************************************************
#* make AGFA font modules in CPU's ASSETS directory.
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
#*   2 02/02/01 Changed targets to microtype fonts 		  mas       **
#*   3 02/06/01 Fixed dependencies.  Added makdir.                djc       **
#*****************************************************************************

TRGTS    = fonts

FILES    = mw_java.fco mwp_java.fco surfer.fco surfplug.fco umt.ss

MWOS     = ../../../../..

include $(MWOS)/MAKETMPL/make.com

SDIR     = .
ODIR     = $(MWOS_OSDIR)/$(CPUDIR)/ASSETS/FONTS/AGFA/MT

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
