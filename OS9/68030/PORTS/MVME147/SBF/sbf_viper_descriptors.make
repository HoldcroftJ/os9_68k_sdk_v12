# Makefile for the MVME147 SBF Descriptors - VIPER devices

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68030					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs

TAPETYPE	= VIPER							# tape type/model

SDIR		= $(SRCROOT)/IO/SBF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS/$(TAPETYPE)	# output
RDIR		= RELS/$(TAPETYPE)

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= ./sbf_viper_descriptors.make	# this file

SPEC_RFLAGS	=

RC			= r68							# assembler
RFLAGS		= -q -a$(TAPETYPE) -u=. -u=$(OSDEFS) $(SPEC_RFLAGS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0

# tape devices
#
SBFDSC		= mt0   mt1   dat
SBFDSCR		= mt0.r mt1.r dat.r

include		$(MWOS)/MAKETMPL/makeosk.tpl
-bo
-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
_build: makdir $(ODIR)/$(SBFDSC)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link tape device descriptors
#
$(ODIR)/$(SBFDSC): $(SLIB) $(SBFDSCR)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(SBFDSCR): $(SYSDEFS) $(MAKER)

makdir _makdir:
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)
 
clean _clean:
	-$(DEL) $(RDIR)/mt0.r
	-$(DEL) $(RDIR)/mt1.r
	-$(DEL) $(RDIR)/dat.r


purge _purge:
	-$(CODO) $(ODIR)/mt0
	-$(CODO) $(ODIR)/mt1
	-$(CODO) $(ODIR)/dat
	-$(DEL)  $(ODIR)/mt0
	-$(DEL)  $(ODIR)/mt1
	-$(DEL)  $(ODIR)/dat

# end of file

