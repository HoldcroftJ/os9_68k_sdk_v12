# Makefile for the I/O driver in the QUADS360 ROM
-mode=compat
-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/CPU32				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source

SDIR		= $(SRCROOT)/ROM/SERIAL		# specific source dir

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..

SYSDEFS		= $(SRCROOT)/DEFS			# std OS defs
CPUDEFS		= $(CPUROOT)/DEFS/CPU32		# CPU32 defs files
SYSMACS		= $(SRCROOT)/MACROS

# defs files included via defsfile/systype.d
#
INCLUDES	= $(CPUDEFS)/sim360.d $(CPUDEFS)/ser360.d
# the above are included by:  $(CPUDEFS)/mc68360defs

MAKER		= rom_serial.make

OBJECTS		= io68360.r

OLIB		= rom_serial.l

COMDEFS		= $(SYSDEFS)/oskdefs.d
DEFS		= ../systype.d $(COMDEFS) $(INCLUDES)

RBUG		= -aROMBUG

INHOUSE		= #-aINHOUSE
MBUGTRC		= #-aMBUGTRC # enables MBUG tracing and breakpoints for testing
BPLOAD		= #-aBPLOAD  # support boofile/rombug load directly for porting

SPEC_RFLAGS	= $(INHOUSE) $(MBUGTRC) $(BPLOAD)

RC			= r68
SRCHDIRS	= -u=. -u=$(CPUDEFS) -u=$(SYSDEFS) -u=$(SYSMACS)
RFLAGS		= -q -aCBOOT $(RBUG) $(SPEC_RFLAGS) $(SRCHDIRS)

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(RDIR)/$(OLIB) : makdir $(OBJECTS)
	-$(CODO) $@
	-$(DEL)  $@
	$(CHD) $(RDIR); $(MERGE) $(MOPTS) $(OBJECTS) $(REDIR_OUT)$(OLIB)

$(RDIR)/$(OBJECTS) : $(DEFS) $(MAKER)

makdir _makdir:
	-$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/io68360.r

purge _purge:
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)

# end of file

