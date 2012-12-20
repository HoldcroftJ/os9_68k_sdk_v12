# Makefile for RUSSBOX 990 ROM without ROMBUG

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

MAKERS		= \
				rom_common.make \
				rom_serial.make \
				rom_port.make \
				rom_image.make \
				rom_initext.make \

MAKEOPTS	= $(RBUG) $(CBUG) $(TDIR) \
				$(PASSTRGT) $(ROMDBG) $(ROMIO) \
				$(MBUGTRC)

MAKER		= ./rom.make				# this file

ROMSTB		= # $(ODIR)/STB/rom.stb

FILES		= $(ODIR)/rom $(ODIR)/initext $(ROMSTB)
PROM		= rommer

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(PROM) _build : makdir $(MAKERS) $(ODIR)/$(PROM)
	$(COMMENT)

$(ODIR)/$(PROM) : .
	-$(CODO) $@
	-$(DEL)  $@
	$(MERGE) $(FILES) $(REDIR_OUT)$@

$(MAKERS): .
	$(MAKESUB) -f=$@

#rom.date: $(MAKER)
#	$(CFP) $(CFPOPTS) $(MAKERS)
#	$(MERGE) $(FILES) $(REDIR)$(ODIR)/$(PROM); $(DIR) $(DIROPTS) $(ODIR)/$(PROM)

makdir _makdir:
	-$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	-$(CODO) $(ODIR)/$(PROM)
	-$(DEL)  $(ODIR)/$(PROM)


# end of file
