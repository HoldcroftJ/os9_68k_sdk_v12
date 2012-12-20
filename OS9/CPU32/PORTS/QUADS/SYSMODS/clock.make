# makefile to make clock modules for QUADS360

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB
CPUROOT		= $(ROOT)/CPU32
SRCROOT		= $(ROOT)/SRC					# dir system for source

OSDEFS		= $(SRCROOT)/DEFS				# std OS defs
MACDIR		= $(SRCROOT)/MACROS				# std macros
PORTDEFS	= $(SRCROOT)/DEFS				# std port defs

CPUDEFS		= $(CPUROOT)/DEFS/CPU32			# CPU32 specific defs

SDIR		= $(SRCROOT)/SYSMODS/GCLOCK		# clock sources
RDIR		= RELS
ODIR		= ../CMDS/BOOTOBJS

SPEC_RFLAGS	= #-aFASTTICK					# tick furiously

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l

TKGENSRC	= $(SDIR)/tickgeneric.a			# generic ticker driver source
TKGEN		= $(SYSRELS)/tickgeneric.r		# generic ticker front-end

# defs files included via defsfile/systype.d
#
INCLUDES	= $(CPUDEFS)/sim360.d $(CPUDEFS)/ser360.d
# included by mc68360defs 

SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d $(INCLUDES)
MAKER		= ./clock.make

MACHINE		= 68360							# cpu board number

TICKNAME	= tk$(MACHINE)					# ticker module
RTICKNAME	= $(RDIR)/$(TICKNAME).r
TICKSRC		= tk68360.a						# clock tick module source

RC			= r68							# assembler
RFLAGS		= -qb $(SPEC_RFLAGS) -u=. -u=$(OSDEFS) -u=$(MACDIR) \
			  -u=$(PORTDEFS) -u=$(CPUDEFS)

LC			= l68							# linker
LFLAGS		= -g -l=$(SLIB) -gu=0.0

TKGEN_ECHO	= "tickgeneric.r not up to date"

include		$(MWOS)/MAKETMPL/makeosk.tpl

-x
-bo


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
$(TICKNAME) _build : $(RDIR) $(ODIR)/STB $(ODIR)/$(TICKNAME)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link ticker module
#
$(ODIR)/$(TICKNAME) : $(SLIB) $(TKGEN) $(RTICKNAME)
	-$(CODO) $@
	-$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(TKGEN) $(RDIR)/$*.r -O=$@

$(TKGEN): $(TKGENSRC)
	-$(CODO) $@
	$(RC) $(RFLAGS) $(TKGENSRC) -o=$@
	$(ECHO) $(TKGEN_ECHO)

$(RTICKNAME): $(SYSDEFS) $(SDIR)/$(TICKSRC) $(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(TICKSRC) -o=$@

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Standard Cleanup stuff
#
$(RDIR):
	$(MAKDIR) $@
$(ODIR)/STB:
	$(MAKDIR) $@

clean _clean: .
	-$(DEL) $(RDIR)/*.r

purge _purge: .
	-$(CODO) $(ODIR)/$(TICKNAME)
	-$(DEL)  $(ODIR)/$(TICKNAME)
	-$(CODO) $(ODIR)/STB/$(TICKNAME).stb
	-$(DEL)  $(ODIR)/STB/$(TICKNAME).stb

# end of file

