# Makefile for h0 bootfile for the MVME177

-b
-bo
-u

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system

MAKER		= h0_bootfile.make

SDIR		= BOOTLISTS
ODIR		= CMDS/BOOTOBJS/BOOTFILES
UPODIR		= ../$(ODIR)

OFILE		= h0.bf

FILES		= $(SDIR)/h0.bl

MRGOPTS		= -b=100k -z=$(FILES)
#MRGOPTS		= -z=$(FILES)
IDOPTS		= -qz=$(FILES)

#
# PF/sufixmod for in-house resident non-root development only
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
	-$(DEL) $(UPODIR)/$(OFILE)
