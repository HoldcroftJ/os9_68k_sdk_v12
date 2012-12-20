# Makefile for the MVME167 RBF Descriptors
# Teac FC-1 Floppy Descriptors

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68040					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs

TYPE		= TEACFC1						# sub-type

SDIR		= $(SRCROOT)/IO/RBF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS/$(TYPE)		# output
RDIR		= RELS/$(TYPE)

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= rbf_teac_descriptors.make		# this file

SPEC_RFLAGS	=

RC			= r68							# assembler
RFLAGS		= -q -u=. -u=$(OSDEFS) $(SPEC_RFLAGS) -a$(TYPE)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0

#
# disk devices
#
DDNAME		= dd
RBFDD		= d0							# devices aka /dd
RBFDDR		= d0.r

include		$(MWOS)/MAKETMPL/makeosk.tpl
-bo
-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
rbf_teac_descriptors.date _build: makdir $(RBFDD)
	$(TOUCH) rbf_teac_descriptors.date

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link disk descriptors
#
$(RBFDD): $(SLIB) $(RBFDDR)
	-$(CODO) $(ODIR)/$*
	-$(CODO) $(ODIR)/$(DDNAME)_$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$(DDNAME)_$* -n=$(DDNAME)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(RBFDDR): $(SYSDEFS) $(MAKER)

makdir _makdir:
    -$(MAKDIR) $(RDIR)
    -$(MAKDIR) $(ODIR)
 
clean _clean:
	-$(DEL) $(RDIR)/$(RBFDDR)

purge _purge:
	-$(CODO) $(ODIR)/d0
	-$(CODO) $(ODIR)/dd_d0
	-$(DEL)  $(ODIR)/d0
	-$(DEL)  $(ODIR)/dd_d0

# end of file

