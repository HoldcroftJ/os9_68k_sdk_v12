# Makefile for the MVME177 RBF Descriptors
# RBVCCS (Variable logical sector sized) versions

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68060					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs

TYPE		= VCCS							# sub-type

SDIR		= $(SRCROOT)/IO/RBF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS/$(TYPE)		# output
RDIR		= RELS/$(TYPE)

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= rbf_vccs_descriptors.make		# this file

SPEC_RFLAGS	=

RC			= r68							# assembler
RFLAGS		= -q -u=. -u=$(OSDEFS) $(SPEC_RFLAGS) -a$(TYPE)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0

# disk devices
#
RBFDSC		= h0fmt   h1fmt
RBFDSCR		= h0fmt.r h1fmt.r
DDNAME		= dd
RBFDD		= h0   h1						# devices aka /dd
RBFDDR		= h0.r h1.r

include		$(MWOS)/MAKETMPL/makeosk.tpl

-bo
-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
rbf_vccs_descriptors.date _build: makdir $(RBFDSC) $(RBFDD)
	$(TOUCH) rbf_vccs_descriptors.date

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link disk descriptors
#
$(RBFDSC): $(SLIB) $(RBFDSCR)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*

$(RBFDD): $(SLIB) $(RBFDDR)
	-$(CODO) $(ODIR)/$*
	-$(CODO) $(ODIR)/$(DDNAME)_$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$(DDNAME)_$* -n=$(DDNAME)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(RBFDSCR) $(RBFDDR): $(SYSDEFS) $(MAKER)


makdir _makdir:
    -$(MAKDIR) $(RDIR)
    -$(MAKDIR) $(ODIR)
 
clean _clean:
	-$(DEL) $(RDIR)/h0fmt.r
	-$(DEL) $(RDIR)/h1fmt.r
	-$(DEL) $(RDIR)/h0.r
	-$(DEL) $(RDIR)/h1.r

purge _purge:
	-$(CODO) $(ODIR)/h0fmt
	-$(CODO) $(ODIR)/h1fmt
	-$(CODO) $(ODIR)/h0
	-$(CODO) $(ODIR)/dd_h0
	-$(CODO) $(ODIR)/h1
	-$(CODO) $(ODIR)/dd_h1
	-$(DEL)  $(ODIR)/h0fmt
	-$(DEL)  $(ODIR)/h1fmt
	-$(DEL)  $(ODIR)/h0
	-$(DEL)  $(ODIR)/dd_h0
	-$(DEL)  $(ODIR)/h1
	-$(DEL)  $(ODIR)/dd_h1


# end of file

