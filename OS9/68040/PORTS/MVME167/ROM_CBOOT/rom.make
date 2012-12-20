# Makefile for MVME167 ROM without ROMBUG

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

MBUGTRC		= #"MBUGTRC=-aMBUGTRC"
RAMLOAD		= #"RAMLOAD=-aRAMLOAD"

if exists(oem.com)
include oem.com
else
OEM_MAKERS	=
endif

MAKERS		= \
				$(OEM_MAKERS) \
				rom_initext.make \
				rom_booters.make \
				rom_bootfile.make

MAKEOPTS	= $(RBUG) $(CBUG) $(TDIR) \
				$(PASSTRGT) $(ROMDBG) $(ROMIO) \
				$(MBUGTRC) $(RAMLOAD)

MAKER		= ./rom.make				# this file

FILES		= booters bootfile
PROM		= rommer

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(ODIR)/$(PROM) _build : makdir $(MAKERS) $(MAKER)
	-$(CODO) $(ODIR)/$(PROM)
	-$(DEL)  $(ODIR)/$(PROM)
	$(CHD) $(ODIR); $(DEL) $(PROM); $(MERGE) $(FILES) $(REDIR_OUT)$(PROM)
#	$(DIR) $@

$(MAKERS):
	$(MAKESUB) -f=$@

makdir _makdir:
    -$(MAKDIR) $(RDIR)
 
clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	-$(CODO) $(ODIR)/$(PROM)
	-$(DEL)  $(ODIR)/$(PROM)

# end of file
