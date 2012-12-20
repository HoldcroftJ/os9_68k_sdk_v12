# makefile to make clock modules for RUSSBOX 990

MWOS		= ../../../../..
ROOT		= ../../../..			# base of dir system
BASEROOT	= $(ROOT)/68000			# dir system for LIB
CPUROOT		= $(ROOT)/RUSSBOX
SRCROOT		= $(ROOT)/SRC			# dir system for source

OSDEFS		= $(SRCROOT)/DEFS		# std OS defs
MACDIR		= $(SRCROOT)/MACROS		# std macros
PORTDEFS	= $(SRCROOT)/DEFS		# std port defs

CPUDEFS		= $(CPUROOT)/DEFS/RUSSBOX	# RB990-specific defs

SDIR		= $(SRCROOT)/SYSMODS/GCLOCK	# clock sources
RDIR		= RELS
ODIR		= ../CMDS/BOOTOBJS

SPEC_RFLAGS	= #-aFASTTICK			# tick furiously

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
#ULIB = $(SYSRELS)/usr.l

TKGENSRC	= ./tick68990.a			# generic ticker driver source
TKGEN		= $(RDIR)/tick68990.r		# generic ticker front-end


INCLUDES	=

SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d $(INCLUDES)
MAKER		= ./clock.make

MACHINE		= 68990				# cpu board number

TICKNAME	= tk$(MACHINE)			# ticker module
RTICKNAME	= $(RDIR)/$(TICKNAME).r
TICKSRC		= tk$(MACHINE).a		# clock tick module source

RTCNAME		= rtc$(MACHINE)			# real time clock module
RRTCNAME	= $(RDIR)/$(RTCNAME).r
RTCSRC		= rtc$(MACHINE).a		# real time clock module source

#FUNCS = $(SRCROOT)/DEFS/funcs.a

RC		= r68				# assembler
RFLAGS		= -qb $(SPEC_RFLAGS) -u=. -u=$(OSDEFS) -u=$(MACDIR)\
 -u=$(PORTDEFS) -u=$(CPUDEFS)

LC		= l68				# linker
LFLAGS		= -g -l=$(SLIB) -gu=0.0
#ULFLAGS = -g -l=$(ULIB) -gu=0.0

TKGEN_ECHO	= "tickgeneric.r not up to date"

include		$(MWOS)/MAKETMPL/makeosk.tpl

-x
-bo


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
all: $(TICKNAME) $(RTCNAME)

$(TICKNAME) _build : $(RDIR) $(ODIR)/STB $(ODIR)/$(TICKNAME)
	$(COMMENT)

$(RTCNAME) _build : $(RDIR) $(ODIR)/STB $(ODIR)/$(RTCNAME)
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
# Link rtc module
#
$(ODIR)/$(RTCNAME) : $(SLIB) $(RRTCNAME)
	-$(CODO) $@
	-$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(RRTCNAME) -O=$@

$(RRTCNAME): $(SYSDEFS) $(SDIR)/$(RTCSRC) $(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(RTCSRC) -o=$@


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Standard Cleanup stuff
#
$(RDIR):
	$(MAKDIR) $@
$(ODIR)/STB:
	$(MAKDIR) $@

clean _clean: .
	$(DEL) $(RDIR)/*.r

purge _purge: .
	$(CODO) $(ODIR)/$(TICKNAME)
	$(DEL)  $(ODIR)/$(TICKNAME)
	$(CODO) $(ODIR)/STB/$(TICKNAME).stb
	$(DEL)  $(ODIR)/STB/$(TICKNAME).stb
	$(CODO) $(ODIR)/$(RTCNAME)
	$(DEL)  $(ODIR)/$(RTCNAME)
	$(CODO) $(ODIR)/STB/$(RTCNAME).stb
	$(DEL)  $(ODIR)/STB/$(RTCNAME).stb

# end of file

