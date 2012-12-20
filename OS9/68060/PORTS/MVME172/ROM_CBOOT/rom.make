# Makefile for MVME172 ROM without ROMBUG
-b
-bo
-x

MWOS		= ../../../../..

TYPE		= NOBUG
RDIR		= RELS/$(TYPE)
ODIR		= ../CMDS/BOOTOBJS/$(TYPE)

# ROM customization flags

RBUG		= "RBUG="
CBUG		= "CBUG=-dNOBUG"
TDIR		= "TYPE=$(TYPE)"

PASSTRGT	= "TARGET=rom"
ROMDBG		= "ROMDBG="
ROMIO		= #"ROMIO="

# Testing options

#MBUGTRC	= #"MBUGTRC=-aMBUGTRC"
#MBUGTRC	= #"MBUGTRC=-aMBUGTRC=0x00180000"   # 172Bug VBR at $00180000
MBUGTRC		= #"MBUGTRC=-aMBUGTRC=0xFFE00000"   # 172Bug VBR at $FFE00000

RAMLOAD		= #"RAMLOAD=-aRAMLOAD"

#
# Conditional makes for OEM/BLS Packages
# 
if exists(oem.com)        
include oem.com                  
else 
OEM_MAKERS =  
endif 

MAKERS		= \
				$(OEM_MAKERS) \
				rom_initext.make \
				rom_booters.make \
				rom_bootfile.make 
#				rom_utilities.make

MAKEOPTS	= $(RBUG) $(CBUG) $(TDIR) \
				$(PASSTRGT) $(ROMDBG) $(ROMIO) \
				$(MBUGTRC) $(RAMLOAD)

MAKER		= ./rom.make				# this file

FILES		= $(ODIR)/booters $(ODIR)/bootfile  #$(ODIR)/utilities
PROM		= rommer

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(PROM) _build : makdir $(MAKERS) $(ODIR)/$(PROM)
	$(COMMENT)

$(ODIR)/$(PROM) : $(FILES) $(MAKER)
	-$(CODO) $(ODIR)/$(PROM)
	-$(DEL)  $(ODIR)/$(PROM)
	$(MERGE) $(FILES) $(REDIR_OUT)$@
#	$(DIR) $@

$(MAKERS): .
	$(MAKESUB) -f=$@

makdir _makdir:
	-$(MAKDIR) $(RDIR)
 
clean _clean: .
	-$(DEL) $(RDIR)/*

purge _purge: .
	-$(CODO) $(ODIR)/$(PROM)
	-$(DEL)  $(ODIR)/$(PROM)


# end of file
