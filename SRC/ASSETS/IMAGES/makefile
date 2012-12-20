#############################################################################
#                                                                           #
#               Copyright 2001 by Microware Systems Corporation             #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, distribution, or creation of derivative works #
#  in any form to any party other than the licensee is strictly prohibited, #
#  unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date     Comments                                                By   #
# --- -------- ------------------------------------------------------- ---  #
#  01 02/06/01 Created.                                                djc  #
#              $$                    <RELEASE_INFO>                     $$  #
#############################################################################
# create sample image modules in CPU's ASSETS directory.                    #
#############################################################################


TRGTS    = images

FILES    = fun.c4 fun.c7 fun.c8 mwlogo.c4 mwlogo.c7 mwlogo.c8 \
               travel.c4 travel.c7 travel.c8 \
               building.jpg dtv.jpg

MWOS     = ../../..

include $(MWOS)/MAKETMPL/make.com

SDIR     = .
ODIR     = $(MWOS_OSDIR)/$(CPUDIR)/ASSETS/IMAGES

OFILES   = $(FILES:%=$(ODIR)/%)

MKDATMOD = mkdatmod


images: _makdir $(OFILES)
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

#                                                                           #
#############################################################################
