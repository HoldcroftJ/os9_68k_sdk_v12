# Makefile for the MVME172 RBF Descriptors
# Non-volatile Ramdisk (in SRAM)

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68060					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources

OSDEFS		= $(SRCROOT)/DEFS				# std OS defs
RBFDESC		= $(SRCROOT)/IO/RBF/DESC		# RBF descriptors

SDIR		= .								# source files
ODIR		= ../CMDS/BOOTOBJS				# output
RDIR		= RELS

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= rbf_nvram_descriptors.make	# this file

LXSRAM		= #-aLXSRAM						# Iff 128KB SRAM on `LX
BIGSRAM		= #-aBIGSRAM					# Iff 2MB SRAM on `LX
NOMBUG		= #-aNOMBUG						# Iff 172Bug not installed

NVRFLAGS    = $(LXSRAM) $(BIGSRAM) $(NOMBUG)

SPEC_RFLAGS	= -aDEVTAPE

RC			= r68							# assembler
RFLAGS		= -q -u=. -u=$(RBFDESC) -u=$(OSDEFS) $(NVRFLAGS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0


# disk devices
#
RBFDSC		= rnvfmt
RBFDSCR		= rnvfmt.r

RBFDSSRC	= rnvfmt.a

DDNAME		= dd

RBFDD		= rnv							# devices aka /dd
RBFDDR		= rnv.r

RBFDDSRC	= rnv.a

include		$(MWOS)/MAKETMPL/makeosk.tpl
-bo
-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
rbf_nvram_descriptors.date _build: makdir $(RBFDD) $(RBFDSC)
	$(TOUCH) rbf_nvram_descriptors.date

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link disk descriptors
#
$(RBFDSC) : $(SLIB) $(RBFDSCR)
	$(CODO) $(ODIR)/$*
	-$(DEL) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*

$(RBFDD) : $(SLIB) $(RBFDDR)
	$(CODO) $(ODIR)/$*
	-$(DEL) $(ODIR)/$*
	$(CODO) $(ODIR)/$(DDNAME)_$*
	-$(DEL) $(ODIR)/$(DDNAME)_$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$(DDNAME)_$*  -n=$(DDNAME)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(RBFDSCR): $(SYSDEFS) $(MAKER) $(SDIR)/$(RBFDSSRC)
	$(RC) $(RFLAGS) $(SDIR)/$(RBFDSSRC) -o=$(RDIR)/$@

$(RBFDDR): $(SYSDEFS) $(MAKER) $(SDIR)/$(RBFDDSRC)
	$(RC) $(RFLAGS) $(SDIR)/$(RBFDDSRC) -o=$(RDIR)/$@


makdir _makdir:
    -$(MAKDIR) $(RDIR)
    -$(MAKDIR) $(ODIR)
 
clean _clean:
	-$(DEL) $(RDIR)/$(RBFDSCR)
	-$(DEL) $(RDIR)/$(RBFDDR)

purge _purge:
	$(CODO) $(ODIR)/rnv
	$(CODO) $(ODIR)/dd_rnv
	$(CODO) $(ODIR)/rnvfmt
	-$(DEL) $(ODIR)/rnv
	-$(DEL) $(ODIR)/dd_rnv
	-$(DEL) $(ODIR)/rnvfmt


# end of file
