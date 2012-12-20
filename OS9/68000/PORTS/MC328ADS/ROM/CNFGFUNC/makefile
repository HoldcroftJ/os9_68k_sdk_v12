#############################################################################
# Makefile to produce the cnfgfunc ROM P2 module                        #
#############################################################################
#                                                                           #

MODULE    = cnfgfunc

TRGTS   = $(MODULE)

PORT    = ../..
include $(PORT)/../make.com

LINKMTHD  = ocode         # ocode or icode

IDIR    = RELS
RDIR    = RELS
SDIR    = $(COMMON_ROM)/CNFGFUNC
ODIR    = $(PORT_ROMDIR)

DEFS    = $(ROM_DEFS) $(MWOS_DEFS)

MODREL    = $(RDIR)/$(MODULE).r

MODICODE  = $(IDIR)/$(MODULE).i

MODSRC    = $(SDIR)/$(MODULE).c

CRELS   = $(MODREL)

CICODE    = $(MODICODE)

LIBS    = $(P2_LIBS) $(CLIBS) $(OS_LIBS) $(SYS)

SPEC_COPTS  = -dRELATIVE

COMPILE   = $(CC) $(CPARMSNC)$(COPT_SC) -a $(DEFS) $(COPTS) $(SPEC_COPTS)
RCOMPILE  = $(COMPILE) -eas=$(RDIR) -t=0 -r $(COPT_CODEADDR)
ICOMPILE  = $(COMPILE) -efe=$(IDIR)

LINK    = $(LINKER) $(LPARMS) -k $(LIBS) $(LOPTS) -ols -t=0
ILINK   = $(LINK) -r $(COPT_CODEADDR)

notarget: nulltrg
	$(COMMENT)

$(MODULE):  _makdir $(ODIR)/$(MODULE)
	$(COMMENT)

include $(COMMON_ROM)/$(LINKMTHD).com

$(MODREL):  $(MODSRC)
	$(RCOMPILE) $(MODSRC)

$(MODICODE):  $(MODSRC)
	$(ICOMPILE) $(MODSRC)

$(MODREL) $(MODICODE):  $(MODSRC) $(ROMH) $(ROM_DDIR)/p2lib.h \
	$(ROM_DDIR)/cnfgdata.h

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  .
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(RDIR)/*.i

_purge:  .
	-$(CODO) $(ODIR)/$(MODULE)
	-$(DEL) $(ODIR)/$(MODULE)
	-$(CODO) $(ODIR)/STB/$(MODULE).map
	-$(DEL) $(ODIR)/STB/$(MODULE).map
	-$(CODO) $(ODIR)/STB/$(MODULE).stb
	-$(DEL) $(ODIR)/STB/$(MODULE).stb

#                                                                           #
#############################################################################

