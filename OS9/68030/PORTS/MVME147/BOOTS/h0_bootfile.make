# Makefile for h0 bootfile for the MVME147
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
IDOPTS		= -qz=$(FILES)


include		$(MWOS)/MAKETMPL/makeosk.tpl

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
		$(MERGE) $(MRGOPTS) $(REDIR_OUT)$(ODIR)/$(OFILE)
#	$(FIXMOD0) $(UPODIR)/$(OFILE)

ident: $(MAKER)
	$(CHD) ..; $(IDENT) $(IDOPTS)

makdir _makdir: 
    $(MAKDIR) $(UPODIR)

clean _clean:
	-$(CODO) $(UPODIR)/$(OFILE)
	-$(DEL)  $(UPODIR)/$(OFILE)

purge _purge:
	-$(CODO) $(UPODIR)/$(OFILE)
	-$(DEL) $(UPODIR)/$(OFILE)
