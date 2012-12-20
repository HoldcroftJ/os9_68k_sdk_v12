#Makefile for MVME162 ROM with ROMBUG

-b
-x

MWOS		= ../../../../..

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
ODIR		= ../CMDS/BOOTOBJS/$(TYPE)

# ROMBUG customization flags

RBUG		=
CBUG		=
TDIR		= "TYPE=$(TYPE)"

ROMTARGET	=
ROMDBG		=
ROMIO		=

# Testing options

MBUGTRC		= #"MBUGTRC=-aMBUGTRC"
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


MAKEOPTS	= $(RBUG) $(CBUG) $(TDIR) \
				$(ROMTARGET) $(ROMDBG) $(ROMIO) \
				$(MBUGTRC) $(RAMLOAD)

MAKER		= ./rombug.make				# this file

FILES		= booters bootfile
PROM		= rombugger

CFPOPTS		= "-s=$(MAKE) -f=* $(MAKEOPTS)"

MOPTS		= -b100
DIROPTS		= -e

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(ODIR)/$(PROM) _build : makdir $(MAKERS) $(MAKER)
	-$(CODO) $(ODIR)/$(PROM)
	-$(DEL)  $(ODIR)/$(PROM)
	$(CHD) $(ODIR); $(MERGE) $(MOPTS) $(FILES) $(REDIR_OUT)$(PROM)
#		$(DIR) $(DIROPTS) $(PROM)

$(MAKERS):
	$(MAKESUB) -f=$@

makdir _makdir:
    -$(MAKDIR) $(RDIR)
 
clean _clean:
	-$(DEL) $(RDIR)/*.*

purge _purge:
	-$(CODO) $(ODIR)/$(PROM)
	-$(DEL)  $(ODIR)/$(PROM)




# end of file
