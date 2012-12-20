# Makefile for the common boot modules in the MVME177 ROM
-mode=compat
-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system

BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/68060				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source

SDIR		= $(SRCROOT)/ROM/COMMON		# specific source dir

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..

SYSDEFS		= $(SRCROOT)/DEFS			# std OS defs
SYSMACS		= $(SRCROOT)/MACROS

MAKER		= rom_common.make

OBJECTS		= vectors.r mbugboot.r boot.r nvram.r

OLIB		= rom_common.l

COMDEFS		= $(SYSDEFS)/oskdefs.d $(SYSDEFS)/nvram.d
DEFS		= ../systype.d $(COMDEFS)

RBUG		= -aROMBUG

INHOUSE		= #-aINHOUSE
MBUGTRC		= #-aMBUGTRC # enables MBUG tracing and breakpoints for testing
RAMLOAD		= #-aRAMLOAD # support rombug load directly for porting
ROMSTB		= #-aROMSTB

SPEC_RFLAGS	= $(INHOUSE) $(MBUGTRC) $(RAMLOAD) $(ROMSTB) -aMANUAL_RAM #-aFASTCONS

RC			= r68
SRCHDIRS	= -u=. -u=$(SYSDEFS) -u=$(SYSMACS)
RFLAGS		= -q -aCBOOT $(RBUG) $(SPEC_RFLAGS) $(SRCHDIRS)

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(RDIR)/$(OLIB) :makdir $(OBJECTS)
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)
	$(CHD) $(RDIR); $(MERGE) $(MOPTS) $(OBJECTS) $(REDIR_OUT)$(OLIB)

$(OBJECTS) : $(DEFS) $(MAKER)


makdir _makdir:
	$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/vectors.r
	-$(DEL) $(RDIR)/mbugboot.r
	-$(DEL) $(RDIR)/boot.r
	-$(DEL) $(RDIR)/nvram.r

purge _purge:
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)
