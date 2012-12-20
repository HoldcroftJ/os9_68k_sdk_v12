#############################################################################
# Makefile include MC328ADS ROM config data module              #
#############################################################################
#                                                                           #

MODULE    = cnfgdata

TRGTS   = $(MODULE)

PORT    = ../..
include $(PORT)/../make.com
 
RDIR    = RELS
SDIR        =   $(COMMON_ROM)/CNFGDATA
ODIR    = $(PORT_ROMDIR)

PORT_DEFS = -v=$(PORT) -v=$(SDIR)
PORT_SYSTYPEH = $(PORT)/systype.h

DEFS    = $(PORT_DEFS) $(ROM_DEFS) $(MWOS_DEFS)
 
MAKERS      =   ./makefile

PSECT   =   $(ROM_LIBDIR)/datastrt.r
 
MODREL    = $(RDIR)/$(MODULE).r
MODSRC    = $(SDIR)/$(MODULE).c


#LLDEVTYPE = -dLLNETROM
LLDEVTYPE = -dLLSLIP

HW_FLOW   = #-dHW_FLOW_CTRL
ALTCONS   = #-dALTCONS 
SPEC_COPTS  = -dCONS68681 $(ALTCONS) $(HW_FLOW) $(LLDEVTYPE)

#COMPILE68K = $(CC) $(CPARMSNC)$(COPT_LC) -eas=$(RDIR) -r \
#       $(DEFS) $(SPEC_COPTS) $(COPTS) -c

COMPILE68K  = $(CC) -td=$(TEMP) -to=$(OS) -tp=$(CPU)lc -eas=$(RDIR) -r \
				$(DEFS) $(SPEC_COPTS) $(COPTS) -c

SPEC_LOPTS  =

LINK        =   $(LINKER) $(LPARMS) -k -oln=$(MODULE) \
				$(SPEC_LOPTS) $(LOPTS) $(COPT_GU0)
 
$(TRGTS): _makdir $(ODIR)/$(TRGTS)
		$(COMMENT)
 
$(ODIR)/$(TRGTS):   $(PSECT) $(MODREL)
	-$(CODO) $@
	-$(CODO) $(ODIR)/STB/$(MODULE).map
	-$(CODO) $(ODIR)/STB/$(MODULE).stb
	$(LINK) $(PSECT) $(MODREL) -fd=$@ -b $(REDIR_OUT)$(ODIR)/STB/$(MODULE).map
 
$(MODREL): $(MODSRC) $(PORT_SYSTYPEH) $(MAKERS)
	$(COMPILE68K) $(MODSRC)

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

