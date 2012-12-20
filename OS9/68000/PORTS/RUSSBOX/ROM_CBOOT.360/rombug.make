# Makefile QUADS360 ROM with ROMBUG

-b
-x

MWOS		= ../../../../..

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
ODIR		= ../CMDS/BOOTOBJS/$(TYPE)

# ROMBUG customization flags

RBUG		=
CBUG		=
TDIR	    = "TYPE=$(TYPE)"


ROMTARGET	=
ROMDBG		=
ROMIO		=

# Testing options

MBUGTRC		= #"MBUGTRC=-aMBUGTRC"


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
				rom_bootfile.make \
				rom_utilities.make


MAKEOPTS	= $(RBUG) $(CBUG) $(TDIR) \
				$(ROMTARGET) $(ROMDBG) $(ROMIO) \
				$(MBUGTRC)

MAKER		= ./rombug.make				# this file

ROMSTB		= # $(ODIR)/STB/rombug.stb

MOPTS		= -b100

FILES		= $(ODIR)/rombug $(ODIR)/initext $(ROMSTB)
PROM		= rombugger

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(PROM) _build : makdir $(MAKERS) $(ODIR)/$(PROM)
	$(COMMENT)

$(ODIR)/$(PROM) : .
	-$(CODO) $@
	-$(DEL)  $@
	$(MERGE) $(MOPTS) $(FILES) $(REDIR_OUT)$@

$(MAKERS): .
	$(MAKESUB) -f=$@

makdir _makdir: $(RDIR)

$(RDIR):
	-$(MAKDIR) $@

clean _clean: .
	-$(DEL) $(RDIR)/*.r

purge _purge: .
	-$(CODO) $(ODIR)/$(PROM)
	-$(DEL)  $(ODIR)/$(PROM)

# end of file

