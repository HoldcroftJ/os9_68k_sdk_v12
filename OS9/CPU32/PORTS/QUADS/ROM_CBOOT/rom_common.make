# Makefile for the common boot modules in the QUADS360 ROM
-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/CPU32				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source

SDIR		= $(SRCROOT)/ROM/COMMON		# specific source dir

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..
LIBROOT		= $(RDIR)

SYSDEFS		= $(SRCROOT)/DEFS			# std OS defs
SYSMACS		= $(SRCROOT)/MACROS
CPUDEFS		= $(CPUROOT)/DEFS/CPU32		# CPU32 defs

MAKER		= rom_common.make

VECTORS		= vectors.r
ENETADDR	= nvdftbl.r
BOOTERS		= boot.r
OBJECTS		= $(VECTORS) $(BOOTERS)

OLIB		= rom_common.l

COMDEFS		= $(SYSDEFS)/oskdefs.d
DEFS		= ../systype.d $(COMDEFS)

RBUG		= -aROMBUG

INHOUSE		= #-aINHOUSE
MBUGTRC		= #-aMBUGTRC # enables MBUG tracing and breakpoints for testing

SPEC_RFLAGS	= $(INHOUSE) $(MBUGTRC) -aMANUAL_RAM -aRAMVects -qQUADS360

-mode=compat
RC			= r68
SRCHDIRS	= -u=. -u=.. -u=$(CPUDEFS) -u=$(SYSDEFS) -u=$(SYSMACS)
RFLAGS		= -q -aCBOOT $(RBUG) $(SPEC_RFLAGS) $(SRCHDIRS)

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(RDIR)/$(OLIB) : makdir $(OBJECTS) $(ENETADDR)
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)
	$(CHD) $(RDIR); $(MERGE) $(VECTORS) $(ENETADDR) $(BOOTERS) $(REDIR_OUT)$(OLIB)

$(OBJECTS) : $(DEFS) $(MAKER)

$(ENETADDR): $(DEFS) $(MAKER) ./nvdftbl.a

makdir _makdir:
	$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)

# end of file
