#*****************************************************************************
#* Makefile to make clock modules for MC68328ADS                            **
#*****************************************************************************
#* Copyright 1995, 1996 by Microware Systems Corporation                    **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************
#* Edition History:                                                         **
#* #   Date     Comments                                          By        **
#* --- -------- ------------------------------------------------- --------- **
#*   1 10/02/95 creation                                          bat       **
#*   2 04/30/96 moved to new directory structure                  bat       **
#*   3 05/13/96 Adopted proper usage of port templates.           ats       **
#*              Corrected copyright date. Correctd port defs dir.           **
#*   4 05/13/96 Adopted proper usage of makefile templates.       ats       **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   5 81/01/96 General cleanup.                                  ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   6 99/04/05 support MAKDIR and CODO                           srm       **
#*   7 99/07/08 Adding explicit shut off for assembler branch               **
#*                optimizaitons (-as=-bt).                        rkw       ** 
#*****************************************************************************


MACHINE		=	68328                           # cpu board number
TICKNAME	=	tk$(MACHINE)                    # ticker module

PORT		=	..
TRGTS		=	tk rtc
MAKENAME	=	makefile

include $(PORT)/../make.com

CPUDEFS		=	$(MWOS_DFTDEF)/MC6832X		# 6832X specific defs

SDIR		=	$(OS_SDIR)/SYSMODS/GCLOCK	# clock sources
RDIR		=	RELS
ODIR		=	$(PORT)/CMDS/BOOTOBJS

DEFS		=	-v=. $(PORT_DEFS) -v=$(CPUDEFS) $(MWOS_DEFS)

SPEC_RFLAGS	=	#-dFASTTICK					# tick furiously

TKGEN		=	$(MWOS_BASLIB)/tickgeneric.r	# generic ticker front-end

# defs files included via defsfile/systype.d
INCLUDES	=	$(CPUDEFS)/m328defs							\
				$(CPUDEFS)/sim328.d $(CPUDEFS)/pllm328.d	\
				$(CPUDEFS)/lcdcm328.d $(CPUDEFS)/rtcm328.d	\
				$(CPUDEFS)/timm328.d $(CPUDEFS)/parp328.d	\
				$(CPUDEFS)/uartm328.d $(CPUDEFS)/spis328.d	\
				$(CPUDEFS)/spim328.d $(CPUDEFS)/pwmm328.d

SYSDEFS		=	$(MWOS_OSDDIR)/oskdefs.d $(PORT)/systype.d $(INCLUDES)

MACHINE		=	68328						# cpu board number

TICKNAME	=	tk$(MACHINE)				# ticker module
RTICKNAME	=	$(TICKNAME).r
TICKSRC		=	tk$(MACHINE).a				# clock tick module source

RTCNAME		=	rtc$(MACHINE)				# real time clock module
RRTCNAME	=	$(RTCNAME).r
RTCSRC		=	rtc$(MACHINE).a				# real time clock module source

LIBS		=	$(SYS)
LIBSN		=	$(MWOS_DFTLIB)/sys.l

ASSEMBL		=	$(ASM) $(APARMS) -k -r $(SPEC_RFLAGS) $(DEFS) \
				-eas=$(RDIR) -asn $(AOPTS) -as=-bt

SPEC_LOPTS	=	

LINK		=	$(LINKER) $(LPARMS) -k -r $(LIBS) $(SPEC_LOPTS) $(LOPTS) \
				$(COPT_GU0)


$(TRGTS) : _makdir $(ODIR)/$(TICKNAME) $(ODIR)/$(RTCNAME)
	$(COMMENT)

$(ODIR)/$(TICKNAME): $(LIBSN) $(TKGEN) $(RDIR)/$(TICKNAME).r
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(TICKNAME).stb
	$(LINK) $(TKGEN) $(RDIR)/$(TICKNAME).r -fd=$@

$(RDIR)/$(TICKNAME).r: $(SYSDEFS) $(SDIR)/$(TICKSRC) ./makefile
	$(ASSEMBL) $(SDIR)/$(TICKSRC) -fd=$(TICKNAME).r

$(ODIR)/$(RTCNAME): $(LIBSN) $(RDIR)/$(RTCNAME).r
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(RTCNAME).stb
	$(LINK) $(RDIR)/$(RTCNAME).r -fd=$(ODIR)/$(RTCNAME) -oln=rtclock

$(RDIR)/$(RTCNAME).r: $(SYSDEFS) $(SDIR)/$(RTCSRC) ./makefile
	$(ASSEMBL) $(SDIR)/$(RTCSRC) -fd=$(RTCNAME).r

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/$(TICKNAME).r
	-$(DEL) $(RDIR)/$(RTCNAME).r

_purge:  nulltrg
	$(CODO) $(ODIR)/$(TICKNAME)
	-$(DEL) $(ODIR)/$(TICKNAME)
	$(CODO) $(ODIR)/STB/$(TICKNAME).stb
	-$(DEL) $(ODIR)/STB/$(TICKNAME).stb
	$(CODO) $(ODIR)/$(RTCNAME)
	-$(DEL) $(ODIR)/$(RTCNAME)
	$(CODO) $(ODIR)/STB/$(RTCNAME).stb
	-$(DEL) $(ODIR)/STB/$(RTCNAME).stb

_lint:  nulltrg
	$(COMMENT)

nulltrg:  .
	$(COMMENT)

#																			#
#############################################################################
