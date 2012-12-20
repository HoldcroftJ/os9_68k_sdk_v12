# Makefile for the MVME162 RBF Descriptors
# Non-volatile Ramdisk (in SRAM)

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68040					# dir system for output
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
NOMBUG		= #-aNOMBUG						# Iff 162Bug not installed

NVRFLAGS    = $(LXSRAM) $(BIGSRAM) $(NOMBUG)

SPEC_RFLAGS	= -aDEVTAPE

RC			= r68							# assembler
RFLAGS		= -q -u=. -u=$(RBFDESC) -u=$(OSDEFS) $(NVRFLAGS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0

TOUCH		= touch							# touch utility


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
rbf_nvram_descriptors.date _build: $(RBFDD) $(RBFDSC)
	$(TOUCH) rbf_nvram_descriptors.date

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link disk descriptors
#
$(RBFDSC) : $(SLIB) $(RBFDSCR)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*

$(RBFDD) : $(SLIB) $(RBFDDR)
	-$(CODO) $(ODIR)/$*
	-$(CODO) $(ODIR)/$(DDNAME)_$*
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
	-$(DEL) $(RDIR)/rnvfmt.r
	-$(DEL) $(RDIR)/rnv.r

purge _purge:
	-$(CODO) $(ODIR)/rnvfmt
	-$(CODO) $(ODIR)/rnv
	-$(CODO) $(ODIR)/dd_rnv
	-$(DEL) $(ODIR)/rnvfmt
	-$(DEL) $(ODIR)/rnv
	-$(DEL) $(ODIR)/dd_rnv

# end of file

