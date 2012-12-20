# Makefile for the MVME167 Serial/Parallel I/O Module Drivers

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/68040				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source

SDIR		= $(SRCROOT)/IO/SCF/DRVR	# source dir
RDIR		= RELS
ODIR		= ../CMDS/BOOTOBJS

OSDEFS		= $(SRCROOT)/DEFS			# std OS defs
MACDIR		= $(SRCROOT)/MACROS			# std macros

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d
SCFSTAT		= $(SYSRELS)/scfstat.l
MACROS		= $(MACDIR)/os9svc.m $(MACDIR)/longio.m

MAKER		= scf_drivers.make			# this file

MACHINE		= 167						# cpu board number

SPEC_RFLAGS	= -aNODATAPORT

# serial ports
#
SCFDRV		= sc$(MACHINE)				# note new driver name
SCFDRVR		= $(SCFDRV).r
SCFSRC		= sccd2401.a				# serial driver source

SCPDRV		= scp$(MACHINE)				# parallel driver name
SCPDRVR		= $(SCPDRV).r
SCPSRC		= scp167.a					# parallel driver source

RC			= r68						# assembler
RFLAGS		= -qb -u=. -u=$(OSDEFS) -u=$(MACDIR) $(SPEC_RFLAGS)

LC			= l68						# linker
LFLAGS		= -g -l=$(SLIB) -gu=0.0

include		$(MWOS)/MAKETMPL/makeosk.tpl
-bo
-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything

scf_drivers.date _build: makdir $(SCFDRV) $(SCPDRV)
	$(TOUCH) scf_drivers.date

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link SCF drivers

$(SCFDRV): $(SLIB) $(SCFDRVR)
	-$(CODO) $(ODIR)/$*
	-$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(SCFSTAT) $(RDIR)/$(SCFDRVR) -O=$(ODIR)/$*

$(SCPDRV): $(SLIB) $(SCPDRVR)
	-$(CODO) $(ODIR)/$*
	-$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(SCFSTAT) $(RDIR)/$(SCPDRVR) -O=$(ODIR)/$*


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly

$(SCFDRVR): $(SDIR)/$(SCFSRC) $(SYSDEFS) $(MACROS) $(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(SCFSRC) -o=$(RDIR)/$*.r

$(SCPDRVR): $(SDIR)/$(SCPSRC) $(SYSDEFS) $(MACROS) $(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(SCPSRC) -o=$(RDIR)/$*.r

makdir _makdir:
    -$(MAKDIR) $(RDIR)
    -$(MAKDIR) $(ODIR)/STB
 
clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	-$(CODO) $(ODIR)/$(SCFDRV)
	-$(CODO) $(ODIR)/STB/$(SCFDRV).stb

	-$(DEL)  $(ODIR)/$(SCFDRV)
	-$(DEL)  $(ODIR)/STB/$(SCFDRV).stb
	-$(DEL)  $(ODIR)/STB/$(SCFDRV).dbg

	-$(CODO) $(ODIR)/$(SCPDRV)
	-$(CODO) $(ODIR)/STB/$(SCPDRV).stb

	-$(DEL)  $(ODIR)/$(SCPDRV)
	-$(DEL)  $(ODIR)/STB/$(SCPDRV).stb
	-$(DEL)  $(ODIR)/STB/$(SCPDRV).dbg

# end of file

