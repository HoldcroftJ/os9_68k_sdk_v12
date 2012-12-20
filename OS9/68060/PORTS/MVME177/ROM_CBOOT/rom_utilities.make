# Makefile for utilities in the MVME177 ROM

-b
-bo
-u

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system

MAKER		= rom_utilities.make

TYPE		= ROMBUG
SDIR		= BOOTLISTS
ODBASE		= CMDS/BOOTOBJS
ODBASEU		= ../$(ODBASE)

OFILE		= utilities
OFSIZE      = 0x40000					# MAX SIZE OF MERGED UTILITIES

FILES		= $(SDIR)/rom_utilities.ml
OFPATH		= $(ODBASE)/$(TYPE)/$(OFILE)

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
PADROMIT    = $(PADROM) $(OFSIZE) $(OFPATH)


$(ODBASEU)/$(TYPE)/$(OFILE) : makdir
	-$(CODO) $@
	-$(DEL) $@
	$(CHD) ..; $(MERGE) $(MRGOPTS) $(REDIR_OUT)$(OFPATH); $(FIXMOD0) $(OFPATH); $(PADROMIT)

ident:
	$(IDENT) $(IDOPTS)

makdir _makdir:
    -$(MAKDIR) $(ODBASEU)/$(TYPE)
 
clean _clean:
	$(ECHO) "No rels to clean"

purge _purge:
	-$(CODO) $(ODBASEU)/$(TYPE)/$(OFILE)
	-$(DEL)  $(ODBASEU)/$(TYPE)/$(OFILE)

