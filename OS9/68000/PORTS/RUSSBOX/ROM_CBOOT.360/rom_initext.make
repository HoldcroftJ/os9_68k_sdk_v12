# Makefile for the sysinit extentions module in the QUADS360 ROM

-mode=compat
-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/CPU32				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source

#SDIR		= $(SRCROOT)/ROM/COMMON		# specific source dir
SDIR		= .							# specific source dir

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..

ODIR		= ../CMDS/BOOTOBJS/$(TYPE)

SYSDEFS		= $(SRCROOT)/DEFS			# std OS defs
CPUDEFS		= $(CPUROOT)/DEFS/CPU32		# CPU32 defs

# defs files included via defsfile/systype.d
#
INCLUDES	= $(CPUDEFS)/sim360.d $(CPUDEFS)/ser360.d
# the above are included by:  $(CPUDEFS)/mc68360defs

MAKER		= rom_initext.make

OBJ			= initext
OBJECT		= $(RDIR)/$(OBJ).r

COMDEFS		= $(SYSDEFS)/oskdefs.d
DEFS		= ../systype.d $(COMDEFS) $(INCLUDES)

RBUG		= -aROMBUG


INHOUSE		= #-aINHOUSE
MBUGTRC		= #-aMBUGTRC # enables MBUG tracing and breakpoints for testing
BPLOAD		= #-aBPLOAD  # support boofile/rombug load directly for porting
INHOUSEXT	= #-aINHOUSEXT				# Inhouse extensions flag

SPEC_RFLAGS	= $(INHOUSE) $(MBUGTRC) $(BPLOAD) $(INHOUSEXT)

RC			= r68
SRCHDIRS	= -u=. -u=$(CPUDEFS) -u=$(SYSDEFS)
RFLAGS		= -q -aCBOOT $(RBUG) $(SPEC_RFLAGS) $(SRCHDIRS)

LC			= l68
LFLAGS		= -r=0 -gu=0.0

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(ODIR)/$(OBJ) : makdir $(OBJECT)
	-$(CODO) $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)	 $(ODIR)/$(OBJ)
	-$(DEL)	 $(ODIR)/STB/$(OBJ).stb
	-$(DEL)	 $(ODIR)/STB/$(OBJ).map
	-$(DEL)	 $(ODIR)/STB/$(OBJ).dbg
	$(LC) $(LFLAGS) $(OBJECT) -O=$@ $(REDIR_OUT)$(ODIR)/STB/$(OBJ).map

$(OBJECT) : $(DEFS) $(MAKER)

makdir _makdir:
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

clean _clean:
	-$(DEL) $(RDIR)/initext.r

purge _purge:
	-$(CODO) $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/$(OBJ)

# end of file

