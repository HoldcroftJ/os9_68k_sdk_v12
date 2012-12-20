#############################################################################
# Makefile to produce abort328 module                   #
#############################################################################
#                                                                           #

MODULE    = abort328

PORT    = ../..
TRGTS   = $(MODULE)

include $(PORT)/../make.com

IDIR    = $(MWOS_IDIR)
RDIR    = RELS
SDIR    = .
ODIR    = $(PORT_ROMDIR)

CPUDEFS   = $(MWOS_DFTDEF)/MC6832X    # 68328 defs

DEFS    = $(PORT_DEFS) $(HW_DEFS) $(ROM_DEFS) \
				-v=$(CPUDEFS) -v=$(OS_ROMSDIR)/CBOOT/DEFS \
				$(MWOS_DEFS)

MAKERS    = ./makefile

MODREL    = $(RDIR)/$(MODULE).r
MODSRC    = $(SDIR)/$(MODULE).c

LIBS    = $(P2_LIB) $(CLIB) $(SYS)

SPEC_COPTS  = 

COMPILE   = $(CC) $(CPARMS) -eas=$(RDIR) -ar -t=0 $(DEFS) \
				$(SPEC_COPTS) $(COPTS)

LINK    = $(LINKER) $(LPARMS) -k $(LIBS) $(LOPTS) -ols $(COPT_GU0)

notarget: nulltrg
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
	$(CODO) $(ODIR)/$(MODULE)
	-$(DEL) $(ODIR)/$(MODULE)
	$(CODO) $(ODIR)/STB/$(MODULE).map
	-$(DEL) $(ODIR)/STB/$(MODULE).map
	$(CODO) $(ODIR)/STB/$(MODULE).stb
	-$(DEL) $(ODIR)/STB/$(MODULE).stb

#                                                                           #
#############################################################################

