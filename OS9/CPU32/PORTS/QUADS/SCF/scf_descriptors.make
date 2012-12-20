# Makefile for the QUADS360 SCF Descriptors

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/CPU32					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs
CPUDEFS		= $(CPUROOT)/DEFS/CPU32			# CPU32 specific defs

# defs files included via defsfile/systype.d
#
INCLUDES	= $(CPUDEFS)/sim360.d \
			  $(CPUDEFS)/ser360.d # $(CPUDEFS)/mc68360defs 

SDIR		= $(SRCROOT)/IO/SCF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS				# output
RDIR		= RELS

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d $(INCLUDES)

MAKER		= scf_descriptors.make			# this file

SPEC_RFLAGS	= #-aFASTCONS -aFASTCOMM		# run ports @ 19,200
PERM		= -p=577						# W:er, GO:ewr module permissions

RC			= r68							# assembler
RFLAGS		= -q -u=. -u=$(OSDEFS) $(SPEC_RFLAGS) -u=$(CPUDEFS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0 $(PERM)

# serial ports
#
SCFDSC		= term
SCFDSCR		= term.r

include		$(MWOS)/MAKETMPL/makeosk.tpl
-bo
-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
$(SCFDSC) _build: $(RDIR) $(ODIR)/$(SCFDSC)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link serial port descriptors
#
$(ODIR)/$(SCFDSC): $(SLIB) $(SCFDSCR)
	-$(CODO) $@
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$@

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(SCFDSCR): $(SYSDEFS) $(MAKER)

$(RDIR):
	-$(MAKDIR) $@

clean _clean:
	-$(DEL) $(RDIR)/$(SCFDSCR)

purge _purge: .
	-$(CODO) $(ODIR)/term
	-$(DEL)  $(ODIR)/term

# end of file

