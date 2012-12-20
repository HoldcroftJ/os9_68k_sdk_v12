#############################################################################
# Makefile to produce communications configuration module         #
#############################################################################
#                                                                           #

MODULE    = commcnfg

PORT    = ../..
TRGTS   = $(MODULE)

include $(PORT)/../make.com

#IDIR   = $(MWOS_IDIR)
IDIR    = RELS
RDIR    = RELS
SDIR    = $(COMMON_ROM)/SERIAL
ODIR    = $(PORT_ROMDIR)

DEFS    = $(PORT_DEFS) $(ROM_DEFS) -v=$(OS_ROMSDIR)/CBOOT/DEFS \
				$(MWOS_DEFS)

MAKERS    = ./makefile

MODREL    = $(RDIR)/$(MODULE).r
MODSRC    = $(SDIR)/$(MODULE).c

LIBS    = $(P2_LIB) $(CLIB) $(SYS)

SPEC_COPTS  = -dP2MODULE -dUSECNFGDATA

COMPILE   = $(CC) $(CPARMS) -eas=$(RDIR) -ar -t=0 $(DEFS) \
				$(SPEC_COPTS) $(COPTS)

LINK    = $(LINKER) $(LPARMS) -k $(LIBS) $(LOPTS) -ols $(COPT_GU0)

notarget: .
	$(COMMENT)

$(TRGTS): _makdir $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS): $(P2_START) $(MODREL) $(P2_LIBN)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(MODULE).map
	$(CODO) $(ODIR)/STB/$(MODULE).stb
	$(LINK) $(P2_START) $(MODREL) -fd=$@ $(REDIR_OUT)$(ODIR)/STB/$(MODULE).map

$(MODREL):  $(MODSRC) $(PORT_SYSTYPEH) $(ROMH) $(MAKERS)
	$(COMPILE) $(SDIR)/$*.c

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  .
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(RDIR)/*.i

_purge:  .
	$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL) $(ODIR)/$(TRGTS)
	$(CODO) $(ODIR)/STB/$(TRGTS).map
	-$(DEL) $(ODIR)/STB/$(TRGTS).map
	$(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL) $(ODIR)/STB/$(TRGTS).stb

#                                                                           #
#############################################################################

