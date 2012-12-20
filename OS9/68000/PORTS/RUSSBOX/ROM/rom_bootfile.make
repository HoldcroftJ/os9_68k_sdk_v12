# Makefile for bootfile in the RUSSBOX 990 ROM

-b
-u

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system

MAKER		= rom_bootfile.make

TYPE		= ROMBUG
SDIR		= BOOTLISTS
ODIR		= CMDS/BOOTOBJS/$(TYPE)

OFILE		= bootfile

FILES		= $(SDIR)/rom.bl

MOPTS		= -b=100k -z=$(FILES)
IOPTS		= -qz=$(FILES)

include		$(MWOS)/MAKETMPL/makeosk.tpl

../$(ODIR)/$(OFILE) : makdir 
	$(CODO) $@
	$(DEL)  $@
	$(CHD) ..; $(MERGE) $(MOPTS) $(REDIR_OUT)$(ODIR)/$(OFILE); $(FIXMOD0) $(ODIR)/$(OFILE)

makdir _makdir:
	$(MAKDIR) ../$(ODIR)

clean _clean:
	$(COMMENT)

purge _purge:
	$(CODO) ../$(ODIR)/$(OFILE)
	$(DEL)  ../$(ODIR)/$(OFILE)

ident: $(MAKER)
	$(CHD) ..; $(IDENT) $(IOPTS)

# end of file
