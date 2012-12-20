# Makefile for the MVME162 SCF Descriptors
-bo
-x

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB, etc
CPUROOT		= $(ROOT)/68040					# dir system for output
SRCROOT		= $(ROOT)/SRC					# dir system for sources
OSDEFS		= $(SRCROOT)/DEFS				# std OS defs

SDIR		= $(SRCROOT)/IO/SCF/DESC		# source files
ODIR		= ../CMDS/BOOTOBJS				# output
RDIR		= RELS

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

MAKER		= scf_descriptors.make			# this file

SPEC_RFLAGS	= #-aFASTCONS -aFASTCOMM		# special flags
PERM        = -p=577                        # W:er, GO:ewr module permissions

RC			= r68							# assembler
RFLAGS		= -q -u=. -u=$(OSDEFS) $(SPEC_RFLAGS)

LC			= l68							# linker
LFLAGS		= -l=$(SLIB) -gu=0.0 $(PERM)

# serial ports
#
SCFDSC		= term   t1   t2   t3   p1   p2   p3
SCFDSCR		= term.r t1.r t2.r t3.r p1.r p2.r p3.r

include		$(MWOS)/MAKETMPL/makeosk.tpl

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
scf_descriptors.date _build: makdir $(SCFDSC)
	$(TOUCH) scf_descriptors.date

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link serial port descriptors
#
term: term.r $(SLIB)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
t1: t1.r $(SLIB)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
t2: t2.r $(SLIB)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
t3: t3.r $(SLIB)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
p1: p1.r $(SLIB)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
p2: p2.r $(SLIB)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
p3: p3.r $(SLIB)
	-$(CODO) $(ODIR)/$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*
									
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(SCFDSCR): $(SYSDEFS) $(MAKER)


makdir _makdir:
    -$(MAKDIR) $(RDIR)
    -$(MAKDIR) $(ODIR)/STB
 
clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	-$(CODO) $(ODIR)/term
	-$(CODO) $(ODIR)/t1
	-$(CODO) $(ODIR)/t2
	-$(CODO) $(ODIR)/t3
	-$(CODO) $(ODIR)/p1
	-$(CODO) $(ODIR)/p2
	-$(CODO) $(ODIR)/p3
	-$(DEL)  $(ODIR)/term
	-$(DEL)  $(ODIR)/t1
	-$(DEL)  $(ODIR)/t2
	-$(DEL)  $(ODIR)/t3
	-$(DEL)  $(ODIR)/p1
	-$(DEL)  $(ODIR)/p2
	-$(DEL)  $(ODIR)/p3

# end of file

