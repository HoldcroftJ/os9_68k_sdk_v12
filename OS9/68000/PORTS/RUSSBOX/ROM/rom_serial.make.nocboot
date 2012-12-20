# Makefile for the I/O driver in the RUSSBOX 990 ROM
#
-mode=compat
-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..			# base of dir system
BASEROOT	= $(ROOT)/68000			# dir system for LIB, etc
SRCROOT		= $(ROOT)/SRC			# dir system for source

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)

SYSDEFS		= $(SRCROOT)/DEFS		# std OS defs
SYSMACS		= $(SRCROOT)/MACROS

# defs files included via defsfile/systype.d
#
INCLUDES	=

MAKER		= rom_serial.make

SRC		= iorb990.a
OBJECTS		= iorb990.r

OLIB		= rom_serial.l

COMDEFS		= $(SYSDEFS)/oskdefs.d
DEFS		= ../systype.d $(COMDEFS) $(INCLUDES)

RBUG		= -aROMBUG

INHOUSE		= #-aINHOUSE
MBUGTRC		= #-aMBUGTRC # enables MBUG tracing and breakpoints for testing
BPLOAD		= -aBPLOAD  # support boofile/rombug load directly for porting

SPEC_RFLAGS	= $(INHOUSE) $(MBUGTRC) $(BPLOAD)

RC		= r68
SRCHDIRS	= -u=. -u=$(SYSDEFS) -u=$(SYSMACS)
RFLAGS		= -q $(RBUG) $(SPEC_RFLAGS) $(SRCHDIRS)

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(RDIR)/$(OLIB) : makdir $(OBJECTS)
	-$(CODO) $@
	-$(DEL)  $@
	$(CHD) $(RDIR); $(MERGE) $(MOPTS) $(OBJECTS) $(REDIR_OUT)$(OLIB)

$(RDIR)/$(OBJECTS) : $(DEFS) $(MAKER) $(SRC)

makdir _makdir:
	-$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/iorb990.r

purge _purge:
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)

# end of file

