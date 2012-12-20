# Makefile for the RUSSBOX 68900 SCF Descriptors

MWOS		= ../../../../..
ROOT		= ../../../..			# base of dir system
BASEROOT	= $(ROOT)/68000			# dir system for LIB, etc
CPUROOT		= $(ROOT)/68000			# dir system for output
SRCROOT		= $(ROOT)/SRC			# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS		# std OS defs
CPUDEFS		= $(CPUROOT)/DEFS/RUSSBOX	# specific CPU family defs

# defs files included via defsfile/systype.d
#
INCLUDES	= $(CPUDEFS)/rb990.d

SDIR		= $(SRCROOT)/IO/SCF/DESC	# source files
ODIR		= ../CMDS/BOOTOBJS		# output
RDIR		= RELS

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d $(INCLUDES)

MAKER		= scf_descriptors.make          # this file

SPEC_RFLAGS	= #-aFASTCONS -aFASTCOMM	# run ports @ 19,200
PERM		= -p=577			# W:er, GO:ewr module permissions

RC		= r68				# assembler
RFLAGS		= -q -u=. -u=$(OSDEFS) $(SPEC_RFLAGS) -u=$(CPUDEFS)

LC		= l68				# linker
LFLAGS		= -l=$(SLIB) -gu=0.0 $(PERM)

# serial ports
#
TERMDSC     = term
TERMDSCR    = term.r

T0DSC = t0
T0DSCR = t0.r

T1DSC = t1
T1DSCR = t1.r

include $(MWOS)/MAKETMPL/makeosk.tpl
-bo
-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
$(TERMDSC) _build: $(RDIR) $(ODIR)/$(TERMDSC)
	$(COMMENT)

$(T0DSC) _build: $(RDIR) $(ODIR)/$(T0DSC)
	$(COMMENT)

$(T1DSC) _build: $(RDIR) $(ODIR)/$(T1DSC)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link serial port descriptors
#
$(ODIR)/$(TERMDSC): $(SLIB) $(TERMDSCR)
	-$(CODO) $@
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$@

$(ODIR)/$(T0DSC): $(SLIB) $(T0DSCR)
	-$(CODO) $@
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$@

$(ODIR)/$(T1DSC): $(SLIB) $(T1DSCR)
	-$(CODO) $@
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$@


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(TERMDSCR): $(SYSDEFS) $(MAKER)

$(T0DSCR): $(SYSDEFS) $(MAKER)

$(T1DSCR): $(SYSDEFS) $(MAKER)

$(RDIR):
	$(MAKDIR) $@

clean _clean:
	$(DEL) $(RDIR)/$(TERMDSCR) $(RDIR)/$(T0DSCR) $(RDIR)/$(T1DSCR)

purge _purge: .
	$(CODO) $(ODIR)/$(TERMDSC)
	$(DEL)  $(ODIR)/$(TERMDSC)
	$(CODO) $(ODIR)/$(T0DSC)
	$(DEL)  $(ODIR)/$(T0DSC)
	$(CODO) $(ODIR)/$(T1DSC)
	$(DEL)  $(ODIR)/$(T1DSC)

# end of file

