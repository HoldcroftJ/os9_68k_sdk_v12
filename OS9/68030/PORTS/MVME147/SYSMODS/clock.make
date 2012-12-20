# makefile to make clock modules for MVME147

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB
CPUROOT		= $(ROOT)/68030
SRCROOT		= $(ROOT)/SRC					# dir system for source

OSDEFS		= $(SRCROOT)/DEFS				# std OS defs
MACDIR		= $(SRCROOT)/MACROS				# std macros
PORTDEFS	= $(SRCROOT)/DEFS				# std port defs

SDIR		= $(SRCROOT)/SYSMODS/GCLOCK			# clock sources
RDIR		= RELS
ODIR		= ../CMDS/BOOTOBJS

SPEC_RFLAGS	= #-aFASTTICK					# tick furiously

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l

TKGENSRC	= $(SDIR)/tickgeneric.a			# Generic ticker driver for OS9/68K
TKGEN		= $(SYSRELS)/tickgeneric.r		# generic ticker front-end
TKGENMAKE	= tickgeneric.make			# generic ticker makefile

# defs files included via defsfile/systype.d
#
INCLUDES	= $(PORTDEFS)/lrcchip.d $(PORTDEFS)/vsbchip.d \
			  $(PORTDEFS)/vmechip.d

SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d $(INCLUDES)
#MAKER		= ./clock.make

MACHINE		= 147						# cpu board number

TICKNAME	= tk$(MACHINE)					# ticker module
RTICKNAME	= $(TICKNAME).r
TICKSRC		= tk147.a					# clock tick module source

RTCNAME		= rtc$(MACHINE)					# on-board RTC
RRTCNAME	= $(RTCNAME).r
RTCSRC		= rtc48t08.a					# RTC source

RC			= r68							# assembler
RFLAGS		= -qb $(SPEC_RFLAGS) -u=. -u=$(OSDEFS) -u=$(MACDIR) \
			  -u=$(PORTDEFS)

LC			= l68							# linker
LFLAGS		= -g -l=$(SLIB) -gu=0.0

TKGEN_ECHO	= "tickgeneric.r not up to date"

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
$(TICKNAME) _build: makdir $(ODIR)/$(TICKNAME) $(ODIR)/$(RTCNAME)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link ticker module
#
$(ODIR)/$(TICKNAME) : $(SLIB) $(TKGEN) $(RTICKNAME)
	-$(CODO) $@
	-$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(TKGEN) $(RDIR)/$*.r -O=$@

$(TKGEN): $(TKGENSRC)
	$(CHD) $(SDIR) ; $(MAKESUB) -f=$(TKGENMAKE)

$(RTICKNAME): $(SYSDEFS) $(SDIR)/$(TICKSRC) #$(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(TICKSRC) -o=$(RDIR)/$*.r

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link On-Board Real Time Clock module
#
$(ODIR)/$(RTCNAME) : $(SLIB) $(RRTCNAME)
	-$(CODO) $@
	-$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$@ -n=rtclock

$(RRTCNAME): $(SYSDEFS) $(SDIR)/$(RTCSRC) #$(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(RTCSRC) -o=$(RDIR)/$*.r

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Standard Cleanup stuff
#
makdir _makdir:
	$(MAKDIR) $(ODIR)/STB
	$(MAKDIR) $(RDIR)
	
clean _clean:
	-$(DEL) $(RDIR)/$(RTICKNAME)
	-$(DEL) $(RDIR)/$(RRTCNAME)

purge _purge:
	-$(CODO) $(ODIR)/$(TICKNAME)
	-$(DEL)  $(ODIR)/$(TICKNAME)
	-$(CODO) $(ODIR)/STB/$(TICKNAME).stb
	-$(DEL)  $(ODIR)/STB/$(TICKNAME).stb
	-$(CODO) $(ODIR)/$(RTCNAME)
	-$(DEL)  $(ODIR)/$(RTCNAME)
	-$(CODO) $(ODIR)/STB/$(RTCNAME).stb
	-$(DEL)  $(ODIR)/STB/$(RTCNAME).stb

# end of file

