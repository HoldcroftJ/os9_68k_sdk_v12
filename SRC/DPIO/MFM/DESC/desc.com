#############################################################################
# FILE:  desc.com															#
#																			#
# This makefile will make the MFM Device Descriptors.						#
# It is inteded to be called form desc.tpl, snddesc.tpl, or msgdesc.tpl     #
#############################################################################

DPIO        =   $(MWOS)/SRC/DPIO
MFM         =   $(DPIO)/MFM
include $(DPIO)/MAKETMPL/dpio.tpl

OSK_DESC	=	
OS9000_DESC	=	
USER_DEFS   =   # can be pre-defined in calling makefile
MACROS		=	-d$(OSK_DESC) -d$(OS9000_DESC) -dMFM_DESC $(USER_DEFS)

DEFS		=	-v=$(DESCDIR) -v=$(MFM)/DRVR -v=$(MFM)/DRVR/$(DRVRCOMM) -v=$(MFM)/DRVR/$(DRVR) -v=$(MFM)/DEFS $(DPIO_DEFS)

USER_OPTS   =   # can be pre-defined in calling makefile
ACOMPILE	=	$(CC) -to=$(OS) -tp=$(CPU) -td=$(TEMP) -o=0 $(DEFS) $(MACROS) -ebe=$(RDIR) -r $(COPTS) $(USER_OPTS)
RCOMPILE	=	$(CC) -to=$(OS) -tp=$(CPU) -td=$(TEMP) -o=0 $(DEFS) $(MACROS) -eas=$(RDIR) -r $(COPTS) $(USER_OPTS)
LINK		=	$(LINKER) -to=$(OS) -tp=$(CPU) -td=$(TEMP) -o=0 -k -r $(LOPTS) $(USER_OPTS)

$(TRGTS):
	$(MAKEIT) TARGET=$(TARGET) $(OS)_DESC=$@ $(OS).desc -f $(MAKENAME)

OSK.desc: $(RDIR) $(ODIR) $(ODIR)/$(OSK_DESC)
	$(COMMENT)
$(ODIR)/$(OSK_DESC): $(SDIR)/cnst.c $(SDIR)/stat.c $(SDIR)/os9.c $(DESCDIR)/mfm_desc.h $(MAKENAME)
	#
	# Attempt version control checkout
	#
    $(CODO) $@
	#
	# now make the os9000 descriptor
	#
	$(ACOMPILE) $(SDIR)/cnst.c; \
	$(RM_VSECT) $(RDIR)/cnst.o $(RDIR)/cnst.a; \
	$(RCOMPILE) $(RDIR)/cnst.a; \
	$(RCOMPILE) $(SDIR)/stat.c; \
	$(RCOMPILE) $(SDIR)/modes.c; \
	$(LINK) $(SYSTART) $(RDIR)/cnst.r $(RDIR)/stat.r $(RDIR)/modes.r -fd=$(RDIR)/os9000; \
	# \
	# now make the os9 descriptor \
	# \
	$(ACOMPILE) $(SDIR)/os9.c; \
	$(RM_VSECT) $(RDIR)/os9.o $(RDIR)/os9.a; \
	$(RCOMPILE) $(RDIR)/os9.a; \
	$(LINK) $(DESCSTART) $(RDIR)/os9.r -fd=$(RDIR)/os9 -oln=$(OSK_DESC); \
	# \
	# now merge the two together \
	# \
	$(MERGE) $(RDIR)/os9 $(RDIR)/os9000 >$@; \
	$(CHTYPE_DESC) $@; \
	$(LINK) $(SYSTART) $(RDIR)/cnst.r $(RDIR)/stat.r $(RDIR)/modes.r -fd=$(RDIR)/os9000 -ilgu=0.0
	$(LINK) $(DESCSTART) $(RDIR)/os9.r -fd=$(RDIR)/os9 -oln=$(OSK_DESC) -ilgu=0.0
	#
	# make sure all temp files are deleted
	#
	-$(DEL) $(RDIR)/cnst.a
	-$(DEL) $(RDIR)/cnst.o
	-$(DEL) $(RDIR)/cnst.r
	-$(DEL) $(RDIR)/stat.r
	-$(DEL) $(RDIR)/modes.r
	-$(DEL) $(RDIR)/os9.a
	-$(DEL) $(RDIR)/os9.o
	-$(DEL) $(RDIR)/os9.r
	-$(DEL) $(RDIR)/os9
	-$(DEL) $(RDIR)/os9000

OS9000.desc: $(RDIR) $(ODIR) $(ODIR)/$(OS9000_DESC)
	$(COMMENT)
$(ODIR)/$(OS9000_DESC): $(SDIR)/cnst.c $(SDIR)/stat.c $(DESCDIR)/mfm_desc.h $(MAKENAME)
	#
	# Attempt version control checkout
	#
    $(CODO) $@
	#
	# make the os9000 descriptor
	#
	$(ACOMPILE) $(SDIR)/cnst.c; \
	$(RM_VSECT) $(RDIR)/cnst.o $(RDIR)/cnst.a; \
	$(RCOMPILE) $(RDIR)/cnst.a; \
	$(RCOMPILE) $(SDIR)/stat.c; \
	$(RCOMPILE) $(SDIR)/modes.c; \
	$(LINK) $(DESCSTART) $(RDIR)/cnst.r $(RDIR)/stat.r $(RDIR)/modes.r -fd=$@ -ilgu=0.0
	#
	# make sure all temp files are deleted
	#
	-$(DEL) $(RDIR)/cnst.a
	-$(DEL) $(RDIR)/cnst.o
	-$(DEL) $(RDIR)/cnst.r
	-$(DEL) $(RDIR)/stat.r
	-$(DEL) $(RDIR)/modes.r

$(RDIR) $(ODIR) : 
    -$(MAKDIR) $@

_lint:
	$(COMMENT)

nulltrg: .

#																			#
#############################################################################

