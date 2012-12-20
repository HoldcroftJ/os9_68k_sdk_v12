#############################################################################
# Makefile to produce 328TIMER timer driver for ROM P2 modules        #
#############################################################################
#                                                                           #

DRIVER1   = m328tmr1
DRIVER2   = m328tmr2

SRCNAM    = m328tmr

PORT    = ../..
TRGTS   = _makdir $(DRIVER1) $(DRIVER2)

include $(PORT)/../make.com

#IDIR   = $(MWOS_IDIR)
IDIR    = RELS
RDIR    = RELS
SDIR    = $(COMMON_ROM)/TIMERS/M328TMR
ODIR    = $(PORT_ROMDIR)

DEFS    = -v=$(SDIR) $(HW_DEFS) $(PORT_DEFS) $(ROM_DEFS) $(MWOS_DEFS)

MAKERS    = ./makefile

DVRREL1   = $(RDIR)/$(DRIVER1).r
DVRREL2   = $(RDIR)/$(DRIVER2).r
DVRSRC    = $(SDIR)/$(SRCNAM).c

LIBS    = $(CPULIB) $(P2_LIB) $(SYS)

SPEC_COPTS  = -dP2MODULE

COMPILE   = $(CC) $(CPARMS) -eas=$(RDIR) -ar -t=0 $(DEFS) \
				$(SPEC_COPTS) $(COPTS)

LINK    = $(LINKER) $(LPARMS) -k $(LIBS) $(LOPTS) -ols $(COPT_GU0)

notarget: .
	$(COMMENT)

$(DRIVER2): _makdir $(ODIR)/$(DRIVER2)
	$(COMMENT)

$(ODIR)/$(DRIVER2): $(P2_START) $(DVRREL2) $(P2_LIBN)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(DRIVER2).map
	$(CODO) $(ODIR)/STB/$(DRIVER2).stb
	$(LINK) $(P2_START) $(DVRREL2) -fd=$@ $(REDIR_OUT)$(ODIR)/STB/$(DRIVER2).map

$(DVRREL2): $(DVRSRC) $(PORT_SYSTYPEH) $(ROMH) $(MAKERS)
	$(COMPILE) $(DVRSRC) -fd=$(DRIVER2).r -dTIMER2

$(DRIVER1): $(ODIR)/$(DRIVER1)
	$(COMMENT)

$(ODIR)/$(DRIVER1): $(P2_START) $(DVRREL1) $(P2_LIBN)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(DRIVER1).map
	$(CODO) $(ODIR)/STB/$(DRIVER1).stb
	$(LINK) $(P2_START) $(DVRREL1) -fd=$@ $(REDIR_OUT)$(ODIR)/STB/$(DRIVER1).map

$(DVRREL1): $(DVRSRC) $(PORT_SYSTYPEH) $(ROMH) $(MAKERS)
	$(COMPILE) $(DVRSRC) -fd=$(DRIVER1).r -dTIMER1

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  .
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(RDIR)/*.i

_purge:  .
	$(CODO) $(ODIR)/$(DRIVER1) 
	-$(DEL) $(ODIR)/$(DRIVER1) 
	$(CODO) $(ODIR)/STB/$(DRIVER1).map
	-$(DEL) $(ODIR)/STB/$(DRIVER1).map
	$(CODO) $(ODIR)/STB/$(DRIVER1).stb
	-$(DEL) $(ODIR)/STB/$(DRIVER1).stb
	$(CODO) $(ODIR)/$(DRIVER2) 
	-$(DEL) $(ODIR)/$(DRIVER2) 
	$(CODO) $(ODIR)/STB/$(DRIVER2).map
	-$(DEL) $(ODIR)/STB/$(DRIVER2).map
	$(CODO) $(ODIR)/STB/$(DRIVER2).stb
	-$(DEL) $(ODIR)/STB/$(DRIVER2).stb

#                                                                           #
#############################################################################

