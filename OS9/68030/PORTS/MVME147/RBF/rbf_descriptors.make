# Makefile for the MVME147 RBF Descriptors
# Generic Version descriptors

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system (OS9)
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68030					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs

SDIR		= $(SRCROOT)/IO/RBF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS				# output
RDIR		= RELS

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= ./rbf_descriptors.make			# this file

SPEC_RFLAGS	= -aDEVTAPE

RC			= r68							# assembler
RFLAGS		= -q -u=. -u=$(OSDEFS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0

# disk devices
#

DDNAME		= dd

RBFDD		= r0							# devices aka /dd
RBFDDR		= r0.r

RBFDDFMT	= r0fmt							# devices aka /dd
RBFDDFMTR	= r0fmt.r

R0NAME		= r0

RBFDD3M		= r0_3M
RBFDD3MR	= r0_3M.r

RBFDDSRC	= r0.a
RBFDDFMTSRC	= r0fmt.a

include		$(MWOS)/MAKETMPL/makeosk.tpl
-bo
-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
$(RBFDD) $(RBFDD3M) $(RBFDDFMT) _build : makdir \
		$(ODIR)/$(RBFDD) $(ODIR)/$(RBFDD3M) $(ODIR)/$(RBFDDFMT)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link disk descriptors
#
$(ODIR)/$(RBFDD) $(ODIR)/$(RBFDD3M) $(ODIR)/$(RBFDDFMT): $(SLIB) \
		$(RBFDDR) $(RBFDD3MR) $(RBFDDFMTR)
	-$(CODO) $(ODIR)/$*
	-$(CODO) $(ODIR)/$(DDNAME)_$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$* -n=$(R0NAME)
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$(DDNAME)_$* -n=$(DDNAME)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(RBFDDR): $(SYSDEFS) $(MAKER)

$(RBFDDFMTR): $(SYSDEFS) $(MAKER) $(SDIR)/$(RBFDDFMTSRC)
	$(RC) $(RFLAGS) $(SDIR)/$(RBFDDFMTSRC) -o=$(RDIR)/$@

$(RBFDD3MR): $(SYSDEFS) $(MAKER) $(SDIR)/$(RBFDDSRC)
	$(RC) $(RFLAGS) $(SPEC_RFLAGS) $(SDIR)/$(RBFDDSRC) -o=$(RDIR)/$@

makdir _makdir:
	-$(MAKDIR) $(RDIR)
 
clean _clean: .
	-$(DEL) $(RDIR)/r0.r
	-$(DEL) $(RDIR)/r0fmt.r
	-$(DEL) $(RDIR)/r0_3M.r

purge _purge: .
	-$(CODO) $(ODIR)/r0
	-$(CODO) $(ODIR)/dd_r0
	-$(CODO) $(ODIR)/r0_3M
	-$(CODO) $(ODIR)/dd_r0_3M
	-$(CODO) $(ODIR)/r0fmt
	-$(CODO) $(ODIR)/dd_r0fmt
	-$(DEL)  $(ODIR)/r0
	-$(DEL)  $(ODIR)/dd_r0
	-$(DEL)  $(ODIR)/r0_3M
	-$(DEL)  $(ODIR)/dd_r0_3M
	-$(DEL)  $(ODIR)/r0fmt
	-$(DEL)  $(ODIR)/dd_r0fmt

# end of file

