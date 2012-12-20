# Makefile for the common boot modules in the RUSSBOX 990 ROM
# This primarily is 'vectors.a', 'boot.a' and nvram if applicable.
#
-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..			# base of dir system
SRCROOT		= $(ROOT)/SRC			# dir system for source
COMMON		= $(SRCROOT)/ROM/COMMON

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..
LIBROOT		= $(RDIR)

SYSDEFS		= $(SRCROOT)/DEFS		# std OS defs
SYSMACS		= $(SRCROOT)/MACROS

MAKER		= rom_common.make

SRCS = vectors.a boot.a
OBJECTS		= vectors.r boot.r

OLIB		= rom_common.l

COMDEFS		= $(SYSDEFS)/oskdefs.d
DEFS		= ../systype.d $(COMDEFS)

RBUG		= -aROMBUG

INHOUSE		= #-aINHOUSE
MBUGTRC		= #-aMBUGTRC # enables MBUG tracing and breakpoints for testing

#SPEC_RFLAGS	= $(INHOUSE) $(MBUGTRC) -aMANUAL_RAM -aRAMVects -qQUADS360
SPEC_RFLAGS =

-mode=compat
RC		= r68
SRCHDIRS	= -u=. -u=.. -u=$(SYSDEFS) -u=$(SYSMACS) -u=$(COMMON)
RFLAGS		= -q $(RBUG) $(SPEC_RFLAGS) $(SRCHDIRS)

include $(MWOS)/MAKETMPL/makeosk.tpl

$(RDIR)/$(OLIB) : makdir $(OBJECTS)
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)
	$(CHD) $(RDIR); $(MERGE) $(OBJECTS) $(REDIR_OUT)$(OLIB)

$(OBJECTS) : $(SRCS) $(DEFS) $(MAKER)

makdir _makdir:
	$(MAKDIR) $(RDIR)

clean _clean:
	$(DEL) $(RDIR)/*.r

purge _purge:
	$(CODO) $(RDIR)/$(OLIB)
	$(DEL)  $(RDIR)/$(OLIB)

# end of file
