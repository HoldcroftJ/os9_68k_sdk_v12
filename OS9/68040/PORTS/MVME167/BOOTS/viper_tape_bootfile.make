# Makefile for Viper tape bootfile for the MVME167

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

include		$(MWOS)/MAKETMPL/makeosk.tpl
-u
-b

#
# PF/sufixmod for in-house resident non-root development only
#
if $(HOST) == "k68k"

if exists(PF/sufixmod)
FIXMOD0     = PF/sufixmod
endif

endif

$(UPODIR)/$(OFILE) _build : makdir $(MAKER)
	-$(CODO) $(UPODIR)/$(OFILE)
	$(DEL)  $(UPODIR)/$(OFILE)
	$(CHD) ..; \
		$(MERGE) $(MRGOPTS) $(REDIR_OUT) $(ODIR)/$(OFILE)
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
