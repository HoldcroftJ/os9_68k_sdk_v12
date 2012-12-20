# Makefile for port modules in the RUSSBOX 990 ROM
# This primarily is 'sysinit.a'.

-bo
-x
-mode=compat

MWOS		= ../../../../..
ROOT		= ../../../..			# base of dir system

BASEROOT	= $(ROOT)/68000			# dir system for LIB, etc
CPUROOT		= $(ROOT)/68000			# dir system for output
SRCROOT		= $(ROOT)/SRC			# dir system for source

SDIR		= .				# specific source dir

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..

BOOTDEFS	= $(SRCROOT)/ROM/CBOOT/DEFS
SHRSCSIDEFS	= $(MWOS)/SRC/DEFS/IO/SCSI	# shared SCSI defs
SCSIDEFS	= $(SRCROOT)/IO/SCSI/DEFS

SYSDEFS		= $(SRCROOT)/DEFS		# std OS defs
CPUDEFS		= $(CPUROOT)/DEFS/RUSSBOX	# cpu variant defs

SYSMACS		= $(SRCROOT)/MACROS

CDEFS		= $(ROOT)/../SRC/DEFS		# std C defs

# defs files included via defsfile/systype.d
#
INCLUDES	= $(CPUDEFS)/rb990.d
# the above are included by:  $(CPUDEFS)/mc68990defs

MAKER		= rom_port.make

SYSINIT		= sysinit.r
SYSCON		= syscon.r
OBJECTS		= $(SYSINIT) $(SYSCON)

OLIB		= rom_port.l

COMDEFS		= $(SYSDEFS)/oskdefs.d
DEFS		= ../systype.d $(COMDEFS) $(INCLUDES)

SC_DEPS		= syscon.c ../systype.h


RBUG		= -aROMBUG
INHOUSE		= #-aINHOUSE
MBUGTRC		= #-aMBUGTRC # enables MBUG tracing and breakpoints for testing

SPEC_RFLAGS	= $(RBUG) $(INHOUSE) $(MBUGTRC)

CBUG		= #-dNOBUG

SPEC_CFLAGS	= $(CBUG)

include		$(MWOS)/MAKETMPL/makeosk.tpl

TMPDIR		= $(TEMP)

CSRCHDIRS	= -v=. -v=.. -v=$(BOOTDEFS) -v=$(SHRSCSIDEFS) -v=$(SCSIDEFS) \
			  -v=$(SYSDEFS) -v=$(CDEFS)
CFLAGS		= -qst=$(TMPDIR) -O=0 -dCBOOT $(SPEC_CFLAGS) $(CSRCHDIRS)

RC			= r68
RSRCHDIRS	= -u=. -u=$(CPUDEFS) -u=$(SYSDEFS) -u=$(SYSMACS)
RFLAGS		= -q -aCBOOT $(SPEC_RFLAGS) $(RSRCHDIRS) -m2


$(RDIR)/$(OLIB) : makdir $(OBJECTS)
	$(CODO) $@ 
	$(DEL)  $@ 
	$(CHD) $(RDIR); $(MERGE) $(MOPTS) $(OBJECTS) $(REDIR_OUT)$(OLIB)

$(SYSINIT) : $(DEFS) $(MAKER) $(SDIR)/sysinit.a

$(SYSCON) : $(SC_DEPS) $(MAKER)

makdir _makdir:
	$(MAKDIR) $(RDIR)

clean _clean:
	$(DEL) $(RDIR)/$(SYSINIT)
	$(DEL) $(RDIR)/$(SYSCON)

purge _purge:
	$(CODO) $(RDIR)/$(OLIB)
	$(DEL)  $(RDIR)/$(OLIB)

# end of file

