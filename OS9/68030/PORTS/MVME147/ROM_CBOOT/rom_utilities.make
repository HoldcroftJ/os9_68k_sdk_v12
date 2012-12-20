# Makefile for utilities in the MVME147 ROM


ROOT		= ../../..					# base of dir system

TMPDIR		= /dd

MAKER		= rom_utilities.make

TYPE		= ROMBUG
SDIR		= BOOTLISTS
ODBASE		= CMDS/BOOTOBJS

OFILE		= utilities
OFSIZE		= 0x40000

FILES		= $(SDIR)/rom_utilities.ml

TOUCH		= touch
CHD			= chd
MERGE		= merge
MOPTS		= -b=100k -z=$(FILES)
REDIR		= >-
#
# PF/sufixmod for in-house resident non-root development only
#
#FIXMOD		= PF/sufixmod
FOPTS		= -uo=0.0
FIXMOD		= fixmod $(FOPTS)
PADROM		= padrom $(OFSIZE) $(ODBASE)/$(TYPE)/$(OFILE)
DELETE		= del
IDENT		= ident
IOPTS		= -qz=$(FILES)

-u

$(ODBASE)/$(TYPE)/$(OFILE) : $(MAKER)
	$(MERGE) $(MOPTS) $(REDIR)$@; $(FIXMOD) $@; $(PADROM)

-b

ident : $(MAKER)
	$(IDENT) $(IOPTS)
