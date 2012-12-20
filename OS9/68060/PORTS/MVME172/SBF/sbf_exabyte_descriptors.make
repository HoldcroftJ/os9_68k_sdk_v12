# Makefile for the MVME172 SBF Descriptors - EXABYTE devices

-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68060					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs

TAPETYPE	= EXABYTE						# tape type/model

SDIR		= $(SRCROOT)/IO/SBF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS/$(TAPETYPE)	# output
RDIR		= RELS/$(TAPETYPE)

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= ./sbf_exabyte_descriptors.make	# this file

SPEC_RFLAGS	=

RC			= r68							# assembler
RFLAGS		= -q -a$(TAPETYPE) -u=. -u=$(OSDEFS) $(SPEC_RFLAGS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB)

# tape devices
#
SBFDSC		= mt2
SBFDSCR		= mt2.r

include		$(MWOS)/MAKETMPL/makeosk.tpl

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link tape device descriptors
#
$(SBFDSC) _build : makdir $(ODIR)/$(SBFDSC)
	$(COMMENT)

$(ODIR)/$(SBFDSC) : $(SLIB) $(SBFDSCR)
	$(CODO) $@
	$(LC) $(LFLAGS) $(RDIR)/$(SBFDSCR) -O=$@

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(SBFDSCR): $(SYSDEFS) $(MAKER)


makdir _makdir:
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)
 
clean _clean: .
	-$(DEL) $(RDIR)/$(SBFDSCR)

purge _purge: .
	-$(CODO) $(ODIR)/mt2
	-$(DEL)  $(ODIR)/mt2

# end of file

