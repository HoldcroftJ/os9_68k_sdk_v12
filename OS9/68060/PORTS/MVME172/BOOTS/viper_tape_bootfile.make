# Makefile for Viper tape bootfile for the MVME177

-u
-b

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system

MAKER		= viper_tape_bootfile.make

SDIR		= BOOTLISTS
ODIR		= CMDS/BOOTOBJS/BOOTFILES
UPODIR		= ../$(ODIR)

OFILE		= viper_tape.bf

FILES		= $(SDIR)/viper_tape.bl

MRGOPTS		= -b=100k -z=$(FILES)
IDOPTS		= -qz=$(FILES)

#
# PF/sufixmod for in-house resident non-root development only
# Comment out FIXMOD0 to use HOST Template defined fixmod
#
if exists(../PF/sufixmod)

if $(HOST) == k68k
FIXMOD0     = ../PF/sufixmod
endif

endif

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(UPODIR)/$(OFILE) _build : makdir $(MAKER)
	-$(CODO) $(UPODIR)/$(OFILE)
	-$(DEL)  $(UPODIR)/$(OFILE)
	$(CHD) ..; $(MERGE) $(MRGOPTS) $(REDIR_OUT)$(ODIR)/$(OFILE)
#	$(FIXMOD0) $(UPODIR)/$(OFILE)


ident: $(MAKER)
	$(CHD) .. ; $(IDENT) $(IDOPTS)

makdir _makdir: 
    $(MAKDIR) $(UPODIR)

clean _clean:
	-$(CODO) $(UPODIR)/$(OFILE)
	-$(DEL)  $(UPODIR)/$(OFILE)

purge _purge:
	-$(CODO) $(UPODIR)/$(OFILE)
	-$(DEL)  $(UPODIR)/$(OFILE)

