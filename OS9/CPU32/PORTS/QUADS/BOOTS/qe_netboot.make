# Makefile for QE (NETWORK BOOT) bootfile for the QUADS

-b
-u

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system

MAKER		= qe_netboot.make

SDIR		= BOOTLISTS
ODIR		= CMDS/BOOTOBJS/BOOTFILES
UPODIR		= ../$(ODIR)

OFILE		= qe_netboot.bf

FILES		= $(SDIR)/qe_netboot.bl

MRGOPTS		= -z=$(FILES)
IOPTS		= -qz=$(FILES)

#
# PF/sufixmod for in-house resident non-root development only
#
if exists(../PF/sufixmod)

if $(HOST) == k68k
FIXMOD0		= ../PF/sufixmod
endif

endif


include		$(MWOS)/MAKETMPL/makeosk.tpl

$(UPODIR)/$(OFILE) _build : makdir $(MAKER)
	-$(CODO) $(UPODIR)/$(OFILE)
	-$(DEL)  $(UPODIR)/$(OFILE)
	$(CHD) ..; $(MERGE) $(MRGOPTS) $(REDIR_OUT)$(ODIR)/$(OFILE)
#	$(FIXMOD0) $(UPODIR)/$(OFILE)


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

