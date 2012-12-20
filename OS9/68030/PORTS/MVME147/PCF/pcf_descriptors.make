# Makefile for the MVME147 PCF Descriptors

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68030					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs

SDIR		= $(SRCROOT)/IO/PCF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS				# output
RDIR		= RELS

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= ./pcf_descriptors.make		# this file

SPEC_RFLAGS	=  -a=TEACFC1

RC			= r68							# assembler
RFLAGS		= -q -u=. -u=$(OSDEFS) $(SPEC_RFLAGS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0

# tape devices
#
PCFDSC		= pcd0
PCFDSCR		= pcd0.r

include		$(MWOS)/MAKETMPL/makeosk.tpl
-bo
-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
$(PCFDSC) _build: makdir $(ODIR)/$(PCFDSC)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link disk descriptors
#
$(ODIR)/$(PCFDSC): $(SLIB) $(PCFDSCR)
	-$(CODO) $@
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$@

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(PCFDSCR): $(SYSDEFS) $(MAKER)

makdir _makdir:
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)
 
clean _clean:
	-$(DEL) $(RDIR)/$(PCFDSCR)


purge _purge:
	-$(CODO) $(ODIR)/$(PCFDSC)
	-$(DEL)  $(ODIR)/$(PCFDSC)

# end of file

