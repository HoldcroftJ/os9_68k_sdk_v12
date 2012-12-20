# Makefile RUSSBOX 990 ROM with ROMBUG

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

MBUGTRC = #"MBUGTRC=-aMBUGTRC"

#############################################
# What goes into the ROM image, in what order
#############################################
# COMMON BOOT: vectors.r, boot.r (rom_common.make)
# BOARD-SPECIFIC BOOT: sysinit.r (rom_port.make)
#                      ioxxx.r (rom_serial.make)
#                      initext.r (rom_initext.make)
# rombug
# rom_image.make ==> $(ODIR)/rombug
#
# $(ODIR)/rombug
# CORE: kernel, ticker, scf/rbf drivers and descriptors, OS9p2 (rom_bootfile.make)
# UTILS: (rom_utilities.make)
# rombug.make (this file) ==> $(ODIR)/rombugger
#
# ---
# One could split this makefile into two; one that creates a dual-stage boot
# (rombug+bootfile), (utilities) or
# (rombug), (bootfile+utilities)
# The second stage would be loaded into SpcMem, as defined in systype.d, via ROMbug
# using the Comm device.
#
####################################
MAKERS = \
 rom_common.make \
 rom_port.make \
 rom_serial.make \
 rom_image.make \
 rom_initext.make \
 rom_bootfile.make \
 rom_utilities.make

MAKEOPTS	= $(RBUG) $(CBUG) $(TDIR)\
 $(ROMTARGET) $(ROMDBG) $(ROMIO)\
 $(MBUGTRC)

MAKER		= ./romfull.make		# this file

ROMSTB		= # $(ODIR)/STB/rombug.stb

#FILES		= $(ODIR)/rombug $(ODIR)/initext $(ODIR)/bootfile $(ODIR)/utilities $(ROMSTB)
FILES		= $(ODIR)/rombug $(ODIR)/initext $(ODIR)/bootfile $(ODIR)/utilities $(ROMSTB)

MOPTS		= -b100

PROM		= romfull

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(PROM) _build : makdir $(MAKERS) $(ODIR)/$(PROM)
	$(COMMENT)

$(ODIR)/$(PROM) : .
	-$(CODO) $@
	-$(DEL)  $@
	$(MERGE) $(FILES) $(REDIR_OUT)$@

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

