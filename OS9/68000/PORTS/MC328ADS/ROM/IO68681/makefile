#############################################################################
# Makefile to produce io68681 ll driver for ROM P2 modules          #
#############################################################################
#                                                                           #

DRIVER    = io68681

PORT    = ../..
TRGTS   = $(DRIVER)

include $(PORT)/../make.com

#IDIR   = $(MWOS_IDIR)
IDIR    = RELS
RDIR    = RELS
SDIR    = $(COMMON_ROM)/SERIAL/68681
ODIR    = $(PORT_ROMDIR)

LIB_DIR   = $(MWOS)/OS9/68000/LIB

DEFS    = -v=$(SDIR) $(HW_DEFS) $(PORT_DEFS) $(ROM_DEFS) $(MWOS_DEFS) \
				-v=$(PORT)/../../../SRC/MACROS -v=$(PORT) \
				-v=$(PORT)/../../DEFS/MC6832X

MAKERS    = ./makefile

DVRREL    = $(RDIR)/io68681.r
DVRSRC    = $(SDIR)/io68681.c
DVRHDR    = $(SDIR)/io68681.h
PARTHDR   = $(HW_DDIR)/68681.h

DEBUG_LIB = #$(MWOS_LIBDIR)/dbg_mod.l
DEBUG_LIBS  = #-l=$(MWOS_LIBDIR)/dbg_mod.l -ill=$(MWOS_ILIBDIR)/dbg_mod.il

LIBS    = $(DEBUG_LIBS) $(MWOS_LIBS) $(CPULIB) $(P2_LIB) -l=$(LIB_DIR)/sys.l

# override BUS_WIDTH and BYTE_OFF definitions in 68681.h
BUS_WIDTH =   #-dBUS_WIDTH=2
BYTE_OFF  = #-dBYTE_OFFSET=0

SPEC_COPTS  = -dNEWINFO -dP2MODULE $(BUS_WIDTH) $(BYTE_OFF) \
				#-dDEBUG # -dHW_FLOW_CTRL

COMPILE   = $(CC) $(CPARMS) -eas=$(RDIR) -ar -o=7 -s=10 $(DEFS) \
				$(SPEC_COPTS) $(COPTS) $(COPT_CODEADDR)

LINK    = $(LINKER) $(LPARMS) -k $(LIBS) $(LOPTS) -ols -oln=$(DRIVER) \
			$(COPT_GU0)

notarget: .
	$(COMMENT)

$(TRGTS): _makdir $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS): $(P2_START) $(DVRREL) $(P2_LIBN)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(TRGTS).map
	$(CODO) $(ODIR)/STB/$(TRGTS).stb
	$(LINK) $(P2_START) $(DVRREL) -fd=$@ $(REDIR_OUT)$(ODIR)/STB/$(TRGTS).map

$(DVRREL):  $(DVRSRC) $(DVRHDR) $(PARTHDR) $(PORT_SYSTYPEH) $(ROMH) $(MAKERS)
	$(COMPILE) $(DVRSRC)

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

