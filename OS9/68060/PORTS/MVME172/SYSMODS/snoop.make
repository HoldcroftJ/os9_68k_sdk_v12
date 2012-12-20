# Makefile for the MVME172 Port specific modules

-b
-x

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/68060				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source

SDIR		= .							# source dir root
RDIR		= RELS
ODIR		= ../CMDS/BOOTOBJS

OSDEFS		= $(SRCROOT)/DEFS			# std OS defs
MACDIR		= $(SRCROOT)/MACROS			# std macros

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d
MACROS		= #$(MACDIR)/os9svc.m $(MACDIR)/longio.m

MAKER		= ./snoop.make				# this file

SPEC_RFLAGS	=

# Port specific snooper module
#
SNOOPER		= snoop172					# bus snooper module

RC			= r68						# assembler
RFLAGS		= -qb -u=. -u=$(OSDEFS) -u=$(MACDIR) $(SPEC_RFLAGS)

LC			= l68						# linker
LFLAGS		= -g -l=$(SLIB) -gu=0.0

include		$(MWOS)/MAKETMPL/makeosk.tpl

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Starting point
#
_build: makdir $(SNOOPER)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link system module
#
$(SNOOPER): $(SLIB) $(SNOOPER).r
	-$(CODO) $(ODIR)/$*
	-$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(RDIR)/$(SNOOPER).r -O=$(ODIR)/$(SNOOPER)

$(SNOOPER).r:	./$(SNOOPER).a $(SYSDEFS) $(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(SNOOPER).a -o=$(RDIR)/$(SNOOPER).r

makdir _makdir:
    -$(MAKDIR) $(RDIR)
    -$(MAKDIR) $(ODIR)/STB
 
clean _clean:
	-$(DEL) $(RDIR)/$(SNOOPER).r

purge _purge:
	-$(CODO) $(ODIR)/$(SNOOPER)
	-$(DEL)  $(ODIR)/$(SNOOPER)
	-$(CODO) $(ODIR)/STB/$(SNOOPER).stb
	-$(DEL)  $(ODIR)/STB/$(SNOOPER).stb
#	-$(DEL)  $(ODIR)/STB/$(SNOOPER).dbg

# end of file

