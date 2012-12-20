# Makefile for the sysinit extensions module in the RUSSBOX 990

-mode=compat
-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..			# base of dir system
BASEROOT	= $(ROOT)/68000			# dir system for LIB, etc
SRCROOT		= $(ROOT)/SRC			# dir system for source

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..

ODIR		= ../CMDS/BOOTOBJS/$(TYPE)

SYSDEFS		= $(SRCROOT)/DEFS		# std OS defs

MAKER		= rom_initext.make

OBJ		= initext
OBJECT		= $(RDIR)/$(OBJ).r

COMDEFS		= $(SYSDEFS)/oskdefs.d
DEFS		= ../systype.d $(COMDEFS)

RBUG		= -aROMBUG

INHOUSE		= #-aINHOUSE
MBUGTRC		= #-aMBUGTRC # enables MBUG tracing and breakpoints for testing
BPLOAD		= -aBPLOAD  # support boofile/rombug load directly for porting
INHOUSEXT	= #-aINHOUSEXT		# Inhouse extensions flag

SPEC_RFLAGS	= $(INHOUSE) $(MBUGTRC) $(BPLOAD) $(INHOUSEXT)

RC		= r68
SRCHDIRS	= -u=. -u=$(SYSDEFS)
RFLAGS		= -q $(RBUG) $(SPEC_RFLAGS) $(SRCHDIRS)

LC		= l68
LFLAGS		= -r=0 -gu=0.0

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(ODIR)/$(OBJ) : makdir $(OBJECT)
	$(CODO) $(ODIR)/$(OBJ)
	$(CODO) $(ODIR)/STB/$(OBJ).stb
	$(CODO) $(ODIR)/STB/$(OBJ).map
	$(DEL)	 $(ODIR)/$(OBJ)
	$(DEL)	 $(ODIR)/STB/$(OBJ).stb
	$(DEL)	 $(ODIR)/STB/$(OBJ).map
	$(DEL)	 $(ODIR)/STB/$(OBJ).dbg
	$(LC) $(LFLAGS) $(OBJECT) -O=$@ $(REDIR_OUT)$(ODIR)/STB/$(OBJ).map

$(OBJECT) : $(DEFS) $(MAKER)

makdir _makdir:
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

clean _clean:
	$(DEL) $(RDIR)/initext.r

purge _purge:
	$(CODO) $(ODIR)/$(OBJ)
	$(DEL)  $(ODIR)/$(OBJ)

# end of file

