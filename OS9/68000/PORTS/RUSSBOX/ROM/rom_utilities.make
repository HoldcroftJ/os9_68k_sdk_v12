# Makefile for utilities in the RUSSBOX 990 ROM

-b
-bo
-u

MWOS		= ../../../../..
ROOT		= ../../../..			# base of dir system

MAKER		= rom_utilities.make

TYPE		= ROMBUG
SDIR		= BOOTLISTS
ODBASE		= CMDS/BOOTOBJS
ODIR		= $(ODBASE)/$(TYPE)
UP_ODIR		= ../$(ODIR)

OFILE		= utilities
OFSIZE		= 0x40000

FILES		= $(SDIR)/rom_utilities.ml

include		$(MWOS)/MAKETMPL/makeosk.tpl

#
# PF/sufixmod for in-house resident non-root development only
#
if $(HOST) == "k68k"

if exists(../PF/sufixmod)
FIXMOD0     = ../PF/sufixmod
endif

endif

IDOPTS		= -qz=$(FILES)
MRGOPTS		= -b=100k -z=$(FILES)

#
# Note: $(PADROM) defined by template includes
#
PADROMIT    = $(PADROM) $(OFSIZE) $(ODIR)/$(OFILE)

$(OFILE) : makdir $(UP_ODIR)/$(OFILE)
	$(COMMENT)

$(UP_ODIR)/$(OFILE): .
	$(CODO) $@
	$(DEL) $@
	$(CHD) ..; $(MERGE) $(MRGOPTS) $(REDIR_OUT)$(ODIR)/$*; $(FIXMOD0) $(ODIR)/$*

ident:
	$(CHD) ..; $(IDENT) $(IDOPTS)

makdir _makdir:
	$(MAKDIR) $(UP_ODIR)
 
clean _clean: .
	$(ECHO) "No rels to clean"

purge _purge: .
	$(CODO) $(UP_ODIR)/$(OFILE)
	$(DEL)  $(UP_ODIR)/$(OFILE)

