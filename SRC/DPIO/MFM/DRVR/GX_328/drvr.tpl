#############################################################################
# FILE:  drvr.tpl                                                           #
#                                                                           #
# This makefile is used from the PORTS directory make the MC68328 LCD       #
# Driver.                                                                   #
#############################################################################

DPIO        =   $(MWOS)/SRC/DPIO
MFM         =   $(DPIO)/MFM

include $(DPIO)/MAKETMPL/dpio.tpl

DRDIR       =   $(MFM)/DRVR
GDVDIR      =   $(DRDIR)/GX_COMM
HWDIR       =   $(DRDIR)/$(DRVR)

RDIR        =   RELS/DRVR
IDIR        =   $(RDIR)/$(HOSTTYPE)

DEFS        =   -v=. -v=$(DESCDIR) -v=$(HWDIR) -v=$(GDVDIR) -v=$(MFM)/DEFS $(DPIO_DEFS)

MAUI_LIBS   =   -l=$(MWOS_LIBDIR)/maui_ss.l -ill=$(MWOS_ILIBDIR)/maui_ss.il $(ADDITIONAL_LIBS)
LIBS        =   $(MAUI_LIBS) $(CONV_LIBS) $(CPULIB) $(MWOS_LIBS)

CFILES      =   
DEPENDFILES =   $(GDVDIR)/defs.h $(GDVDIR)/gdv_priv.h \
                $(HWDIR)/config.h $(HWDIR)/global.h \
                $(HWDIR)/hardware.h $(HWDIR)/static.h \
                $(DESCDIR)/mfm_desc.h

# DEFINES definitions
#   PWR_AWARE : make driver PwrMan-compatible (also need pwrman.l/.il in LIBS)
DEFINES     =   $(USR_DEFINES)

RFILES      =   $(DPIO_DRVSTART) $(RDIR)/gdv_main.r $(RDIR)/ifiles.r
IFILES      =   $(IDIR)/gdv_dev.i \
                $(IDIR)/gdv_ep.i \
                $(IDIR)/gdv_vp.i \
                $(IDIR)/dvbkcol.i \
                $(IDIR)/dvextvid.i \
                $(IDIR)/dvtran.i \
                $(IDIR)/dvvpmix.i \
                $(IDIR)/hardware.i \
                $(IDIR)/irq.i \
                $(IDIR)/static.i \
                $(IDIR)/updtdpy.i \
                $(IDIR)/vpdmap.i \
                $(IDIR)/vpdmpos.i \
                $(IDIR)/vpintens.i \
                $(IDIR)/vppos.i \
                $(IDIR)/vprestak.i \
                $(IDIR)/vpsize.i \
                $(IDIR)/vpstate.i \
                $(ADDITIONAL_IFILES)


# do not use the -iom option on any file where a function is declared
# which is refered to as a pointer elsewhere.  The -iom option causes
# inlined functions to be discarded, regardless of whether they are 
# used by pointer elsewhere.  This gives an unresolved reference during
# linking.
OPTMZ       =   -o=7 -t=1 -s=10 -cw -iom

COMPILE     =   $(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
                $(COPT_CODEADDR) $(DEBUG) $(COPTS)
ASSEMBL     =   $(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
                $(COPT_CODEADDR) $(DEBUG) $(LIBS) $(COPTS)
LINK        =   $(LINKER) $(LPARMS) $(COPT_CODEADDR) -k -r $(OPTMZ) \
                $(DEBUG) $(LIBS) $(LOPTS)

$(TRGTS): DIRS $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS):  $(RFILES)
	$(CODO) $@
    $(CODO) $(ODIR)/STB/$*.stb
	$(LINK) $(RFILES) -fd=$@ -oln=$* -olgu=0.0; \
	$(CHTYPE_DRVR) $@;

$(RDIR)/gdv_main.r:  $(GDVDIR)/gdv_main.c $(DEPENDFILES)
	$(ASSEMBL) $(GDVDIR)/gdv_main.c
$(RDIR)/ifiles.r:  $(IFILES)
	$(ASSEMBL) $(IFILES) -fd=ifiles.r

$(IDIR)/gdv_dev.i: $(GDVDIR)/gdv_dev.c $(DEPENDFILES)
	$(COMPILE) $(GDVDIR)/gdv_dev.c
$(IDIR)/gdv_ep.i: $(GDVDIR)/gdv_ep.c $(DEPENDFILES)
	$(COMPILE) $(GDVDIR)/gdv_ep.c
$(IDIR)/gdv_vp.i: $(GDVDIR)/gdv_vp.c $(DEPENDFILES)
	$(COMPILE) $(GDVDIR)/gdv_vp.c
$(IDIR)/dvbkcol.i: $(HWDIR)/dvbkcol.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/dvbkcol.c
$(IDIR)/dvextvid.i: $(HWDIR)/dvextvid.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/dvextvid.c
$(IDIR)/dvtran.i: $(HWDIR)/dvtran.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/dvtran.c
$(IDIR)/dvvpmix.i: $(HWDIR)/dvvpmix.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/dvvpmix.c
$(IDIR)/hardware.i: $(HWDIR)/hardware.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/hardware.c
$(IDIR)/irq.i: $(HWDIR)/irq.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/irq.c
$(IDIR)/static.i: $(HWDIR)/static.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/static.c
$(IDIR)/updtdpy.i: $(HWDIR)/updtdpy.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/updtdpy.c
$(IDIR)/vpdmap.i: $(HWDIR)/vpdmap.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/vpdmap.c
$(IDIR)/vpdmpos.i: $(HWDIR)/vpdmpos.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/vpdmpos.c
$(IDIR)/vpintens.i: $(HWDIR)/vpintens.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/vpintens.c
$(IDIR)/vppos.i: $(HWDIR)/vppos.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/vppos.c
$(IDIR)/vprestak.i: $(HWDIR)/vprestak.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/vprestak.c
$(IDIR)/vpsize.i: $(HWDIR)/vpsize.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/vpsize.c
$(IDIR)/vpstate.i: $(HWDIR)/vpstate.c $(DEPENDFILES)
	$(COMPILE) $(HWDIR)/vpstate.c

DIRS: nulltrg
	-$(MAKDIR) $(ODIR)/STB
	-$(MAKDIR) $(IDIR)

_clean:  nulltrg
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(IDIR)/*.i

_purge:  nulltrg
	$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL) $(ODIR)/$(TRGTS)
    $(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL) $(ODIR)/STB/$(TRGTS).stb

_lint:  nulltrg
	-$(DEL) $(TRGTS).lnt
	$(LINT) $(LINTPARMS) $(DEFS) $(DEFINES) $(CFILES) >$(TRGTS).lnt

nulltrg:  .
	$(COMMENT)

