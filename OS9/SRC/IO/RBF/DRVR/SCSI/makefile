# Makefile for all RBF/SCSI drivers

DIRS = RB327 RB5400 RBFLOP RBSCCS RBTEAC RBVCCS

TRGTS =
for DIR in $(DIRS)
if exists ($(DIR)/makefile)
TRGTS += $(DIR)
endif
endfor

MWOS	= ../../../../../..

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS): .
	$(CHD) $*; $(MAKESUB)

# end of file
