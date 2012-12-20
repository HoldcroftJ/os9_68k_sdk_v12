# makefile to make clock modules for MVME177

-x
-b

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system
BASEROOT	= $(ROOT)/68000					# dir system for LIB
CPUROOT		= $(ROOT)/68060
SRCROOT		= $(ROOT)/SRC					# dir system for source

OSDEFS		= $(SRCROOT)/DEFS				# std OS defs
MACDIR		= $(SRCROOT)/MACROS				# std macros
PORTDEFS	= $(SRCROOT)/DEFS				# std port defs

SDIR		= $(SRCROOT)/SYSMODS/GCLOCK		# clock sources
RDIR		= RELS/CLOCK
ODIR		= ../CMDS/BOOTOBJS

SPEC_RFLAGS	= #-aFASTTICK					# tick furiously

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l

TKGEN		= $(SYSRELS)/tickgeneric.r		# generic ticker front-end

# defs files included via defsfile/systype.d
#
INCLUDES	= $(PORTDEFS)/lrcchip.d $(PORTDEFS)/vsbchip.d \
			  $(PORTDEFS)/vmechip.d

SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d $(INCLUDES)
#MAKER		= ./clock.make

MACHINE		= 177							# cpu board number

TICKNAME	= tk$(MACHINE)					# ticker module
RTICKNAME	= $(TICKNAME).r
TICKSRC		= tk167.a						# clock tick module source

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

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#

_build : _makdir $(ODIR)/$(TICKNAME) $(ODIR)/$(RTCNAME)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link ticker module
#
$(ODIR)/$(TICKNAME) : $(SLIB) $(TKGEN) $(RDIR)/$(RTICKNAME)
	-$(CODO) $@
	-$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(TKGEN) $(RDIR)/$(RTICKNAME) -O=$@

$(TKGEN):
	$(ECHO) $(TKGEN_ECHO)

$(RDIR)/$(RTICKNAME): $(SYSDEFS) $(SDIR)/$(TICKSRC) #$(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(TICKSRC) -o=$(RDIR)/$(RTICKNAME)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link On-Board Real Time Clock module
#
$(ODIR)/$(RTCNAME) : $(SLIB) $(RDIR)/$(RRTCNAME)
	-$(CODO) $@
	-$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(RDIR)/$(RRTCNAME) -O=$(ODIR)/$(RTCNAME) -n=rtclock

$(RDIR)/$(RRTCNAME): $(SYSDEFS) $(SDIR)/$(RTCSRC) #$(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(RTCSRC) -o=$(RDIR)/$(RRTCNAME)



# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Standard Cleanup stuff
#
makdir _makdir:
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB
	
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

