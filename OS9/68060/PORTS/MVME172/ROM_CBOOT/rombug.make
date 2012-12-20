#Makefile for MVME172 ROM with ROMBUG
-bo
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
#				rom_utilities.make

MAKEOPTS	= $(RBUG) $(CBUG) $(TDIR) \
				$(ROMTARGET) $(ROMDBG) $(ROMIO) \
				$(MBUGTRC) $(RAMLOAD)

MAKER		= ./rombug.make				# this file

FILES		= 	$(ODIR)/booters \
				$(ODIR)/bootfile
#				$(ODIR)/utilities

PROM		= rombugger

MOPTS		= -b100
DIROPTS		= -e

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(PROM) _build : makdir $(MAKERS) $(ODIR)/$(PROM)
	$(COMMENT)

$(ODIR)/$(PROM) :  $(FILES) $(MAKER)
	-$(CODO) $@
	-$(DEL)  $@
	 $(MERGE) $(MOPTS) $(FILES) $(REDIR_OUT)$@
#		$(DIR) $(DIROPTS) $@

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
