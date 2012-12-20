# Makefile for bootfile in the MVME172 ROM
# Makefile for d0 bootfile for the MVME172

-b
-u

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system

MAKER		= rom_bootfile.make

TYPE		= ROMBUG

SDIR		= BOOTLISTS
ODIR		= CMDS/BOOTOBJS/BOOTFILES
UPODIR		= ../$(ODIR)

OFILE		= rom.bf
OFSIZE		= 0x20000

FILES		= $(SDIR)/rom.bl

MRGOPTS		= -z=$(FILES)
IOPTS		= -qz=$(FILES)
#
# PF/sufixmod for in-house resident non-root development only
#
if exists(PF/sufixmod)
FIXMOD0		= PF/sufixmod
endif

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(UPODIR)/$(TYPE)/$(OFILE) _build : $(MAKER)
	-$(CODO) $(UPODIR)/$(TYPE)/$(OFILE)
	-$(DEL)  $(UPODIR)/$(TYPE)/$(OFILE)
	$(CHD) .. ; $(MERGE) $(MRGOPTS) $(REDIR_OUT)$(ODIR)/$(TYPE)/$(OFILE)
	$(PADROM) $(OFSIZE) $(UPODIR)/$(TYPE)/$(OFILE)
#	$(FIXMOD0) $(UPODIR)/$(TYPE)/$(OFILE)

ident: $(MAKER)
	$(CHD) .. ; $(IDENT) $(IOPTS)

makdir _makdir:
	$(MAKDIR) $(UPODIR)
	
clean _clean:
	-$(CODO) $(UPODIR)/$(OFILE)
	-$(DEL)  $(UPODIR)/$(OFILE)

purge _purge:
	-$(CODO) $(UPODIR)/$(OFILE)
	-$(DEL)  $(UPODIR)/$(OFILE)

