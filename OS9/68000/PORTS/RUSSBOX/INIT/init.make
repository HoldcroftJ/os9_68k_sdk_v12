# Makefile for the RUSSBOX 990 Init Module

-x
-bo

MWOS		= ../../../../..
ROOT		= ../../../..			# base of dir system
BASEROOT	= $(ROOT)/68000			# dir system for LIB, etc
CPUROOT		= $(ROOT)/68000			# dir system for output
SRCROOT		= $(ROOT)/SRC			# dir system for source

SDIR		= $(SRCROOT)/SYSMODS/INIT	# source dir
RDIR		= RELS
ODIR		= ../CMDS/BOOTOBJS/INITS

OSDEFS		= $(SRCROOT)/DEFS		# std OS defs

CPUDEFS		= $(CPUROOT)/DEFS/RUSSBOX	# RB990-specific defs
#CPUDEFS =

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

# defs files included via defsfile/systype.d
#
INCLUDES = $(CPUDEFS)/rb990.d

MAKER		= ./init.make               # this file

# Init module
#
INIT		= init				# Init module name
INITSRC		= $(SDIR)/$(INIT).a		# source file

RC		= r68				# assembler
RFLAGS		= -qb -u=. -u=$(OSDEFS) -u=$(CPUDEFS)

LC		= l68				# linker
LFLAGS		= -l=$(SLIB) -n=$(INIT)

include		$(MWOS)/MAKETMPL/makeosk.tpl

-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
$(INIT) _build : _makdir $(ODIR)/$(INIT)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link Init module for rom-based
#
$(ODIR)/$(INIT) : $(SLIB) $(INIT).r
	$(CODO) $(ODIR)/$(INIT)
	$(DEL)  $(ODIR)/$(INIT)
	$(CODO) $(ODIR)/STB/$(INIT).stb
	$(DEL)  $(ODIR)/STB/$(INIT).stb
	$(DEL)  $(ODIR)/STB/$(INIT).dbg
	$(LC) $(LFLAGS) $(RDIR)/$(INIT).r -O=$(ODIR)/$(INIT)

$(INIT).r: $(INITSRC) $(SYSDEFS) $(MAKER)
	$(RC) $(RFLAGS) $(RFLAGS) -o=$(RDIR)/$@ $(INITSRC)

makdir _makdir: $(RDIR) $(ODIR)
$(RDIR) $(ODIR):
	$(MAKDIR) $@

clean _clean:
	$(DEL) $(RDIR)/$(INIT).r
	$(COMMENT)

purge _purge:
	$(CODO) $(ODIR)/$(INIT)
	$(DEL)  $(ODIR)/$(INIT)
	$(CODO) $(ODIR)/STB/$(INIT).stb
	$(DEL)  $(ODIR)/STB/$(INIT).stb
	$(DEL)  $(ODIR)/STB/$(INIT).dbg
	$(COMMENT)

# end of file
