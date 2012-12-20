# Makefile for the MVME162 SBF Descriptors - TEAC MT2 devices

-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68040					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs

TAPETYPE	= TEACMT2						# tape type/model

SDIR		= $(SRCROOT)/IO/SBF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS/$(TAPETYPE)	# output
RDIR		= RELS/$(TAPETYPE)

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= ./sbf_teac_descriptors.make	# this file

SPEC_RFLAGS	=

RC			= r68							# assembler
RFLAGS		= -q -a$(TAPETYPE) -u=. -u=$(OSDEFS) $(SPEC_RFLAGS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0

# tape devices
#
SBFDSC		= mt0
SBFDSCR		= mt0.r

include		$(MWOS)/MAKETMPL/makeosk.tpl

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link tape device descriptors
#
$(SBFDSC) _build: makdir $(SLIB) $(SBFDSCR)
	-$(CODO) $(ODIR)/$(SBFDSC)
	$(LC) $(LFLAGS) $(RDIR)/$(SBFDSCR) -O=$(ODIR)/$(SBFDSC)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(SBFDSCR): $(SYSDEFS) $(MAKER)


makdir _makdir:
    -$(MAKDIR) $(RDIR)
    -$(MAKDIR) $(ODIR)
 
clean _clean:
	-$(DEL) $(RDIR)/$(SBFDSCR)

purge _purge:
	-$(CODO) $(ODIR)/$(SBFDSC)
	-$(DEL)  $(ODIR)/$(SBFDSC)


# end of file

