# Makefile for bootfile in the MVME172 ROM

-b
-u

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system


MAKER		= rom_bootfile.make

TYPE		= ROMBUG
SDIR		= BOOTLISTS
ODIR		= CMDS/BOOTOBJS/$(TYPE)
UP_ODIR		= ../$(ODIR)

#
# See Rom_Booters.make to determine the size of the initial rom code
# Typically this is 128k and roms are 1 meg leaving 896k for the OS BOOT
#
OFILE		= bootfile
#OFSIZE		= 0x20000		# pad out to  128k boundry
OFSIZE		= 0xe0000		# pad out to 1 meg boundry

FILES		= $(SDIR)/rom.bl

MOPTS		= -b=100k -z=$(FILES)
IOPTS		= -qz=$(FILES)

include		$(MWOS)/MAKETMPL/makeosk.tpl

#
# PF/sufixmod for in-house resident non-root development only
#
if exists(../PF/sufixmod)

if $(HOST) == "k68k"
FIXMOD0		= ../PF/sufixmod
endif

endif

$(UP_ODIR)/$(OFILE) _build : makdir $(MAKER)
	-$(CODO) $(UP_ODIR)/$(OFILE)
	-$(DEL)  $(UP_ODIR)/$(OFILE)
	$(CHD) ..; $(MERGE) $(MOPTS) $(REDIR_OUT)$(ODIR)/$(OFILE)
	$(PADROM) $(OFSIZE) $(UP_ODIR)/$(OFILE)

#	$(FIXMOD0) $(UP_ODIR)/$(OFILE)

makdir _makdir:
	-$(MAKDIR) $(UP_ODIR)

clean _clean:
	-$(CODO) $(UP_ODIR)/$(OFILE)
	-$(DEL)  $(UP_ODIR)/$(OFILE)
	
purge _purge:
	-$(CODO) $(UP_ODIR)/$(OFILE)
	-$(DEL)  $(UP_ODIR)/$(OFILE)

ident: $(MAKER)
	$(IDENT) $(IOPTS)
