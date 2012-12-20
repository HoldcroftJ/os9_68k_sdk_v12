# Makefile for the QUADS360 Serial/Parallel I/O Module Drivers

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/CPU32				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source
CPUDEFS		= $(CPUROOT)/DEFS/CPU32			# CPU32 specific defs

# defs files included via defsfile/systype.d
#
INCLUDES	= $(CPUDEFS)/sim360.d \
			  $(CPUDEFS)/ser360.d # $(CPUDEFS)/mc68360defs 

SDIR		= $(SRCROOT)/IO/SCF/DRVR	# source dir
RDIR		= RELS
ODIR		= ../CMDS/BOOTOBJS

OSDEFS		= $(SRCROOT)/DEFS			# std OS defs
MACDIR		= $(SRCROOT)/MACROS			# std macros

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d $(INCLUDES)

SCFSTAT		= $(SYSRELS)/scfstat.l
MACROS		= $(MACDIR)/os9svc.m $(MACDIR)/longio.m

MAKER		= scf_drivers.make			# this file

SPEC_RFLAGS	= -m2

# serial ports
#
SCFSRC		= sc68360.a					# serial driver source
SCFDRV		= sc68360					# note: driver name = sc68360
SCFDRVR		= sc68360.r

RC			= r68						# assembler
RFLAGS		= -qb -u=. -u=$(OSDEFS) -u=$(MACDIR) $(SPEC_RFLAGS) -u=$(CPUDEFS)

LC			= l68						# linker
LFLAGS		= -g -l=$(SLIB) -gu=0.0

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything

$(SCFDRV) _build: $(RDIR) $(ODIR)/STB $(ODIR)/$(SCFDRV)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link SCF drivers

$(ODIR)/$(SCFDRV): $(SLIB) $(SCFDRVR)
	-$(CODO) $@
	-$(DEL)  $@
	-$(CODO) $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.dbg
	$(LC) $(LFLAGS) $(SCFSTAT) $(RDIR)/$(SCFDRVR) -O=$@

$(SCFDRVR): $(SDIR)/$(SCFSRC)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly

$(SCFDRVR): $(SYSDEFS) $(MACROS) $(MAKER)

$(RDIR):
	-$(MAKDIR) $@
$(ODIR)/STB:
	-$(MAKDIR) $@

clean _clean: .
	-$(DEL) $(RDIR)/$(SCFDRVR)

purge _purge: .
	-$(CODO) $(ODIR)/$(SCFDRV)
	-$(DEL)  $(ODIR)/$(SCFDRV)
	-$(CODO) $(ODIR)/STB/$(SCFDRV).stb
	-$(DEL)  $(ODIR)/STB/$(SCFDRV).stb
	-$(DEL)  $(ODIR)/STB/$(SCFDRV).dbg

# end of file

