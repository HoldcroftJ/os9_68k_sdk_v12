# Makefile for the MVME172 PCF Descriptors

-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68060					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs

SDIR		= $(SRCROOT)/IO/PCF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS				# output
RDIR		= RELS

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= ./pcf_descriptors.make		# this file

SPEC_RFLAGS	=

RC			= r68							# assembler
RFLAGS		= -q -u=. -u=$(OSDEFS) $(SPEC_RFLAGS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB)

# tape devices
#
PCFDSC		= pcd0
PCFDSCR		= pcd0.r

include		$(MWOS)/MAKETMPL/makeosk.tpl

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
_build: makdir $(PCFDSC)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link disk descriptors
#
$(PCFDSC): $(SLIB) $(PCFDSCR)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(PCFDSCR): $(SYSDEFS) $(MAKER)


makdir _makdir:
    -$(MAKDIR) $(RDIR)
    -$(MAKDIR) $(ODIR)
 
clean _clean:
	-$(DEL) $(RDIR)/pcd0.r

purge _purge:
	-$(CODO) $(ODIR)/pcd0
	-$(DEL)  $(ODIR)/pcd0

# end of file
