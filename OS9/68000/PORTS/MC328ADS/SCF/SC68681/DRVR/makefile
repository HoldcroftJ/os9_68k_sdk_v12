#*****************************************************************************
#* Makefile for the MC68328ADS 68681 SCF Driver                             **
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
#*   2 05/13/96 Adopted proper usage of port and base templates.  ats       **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   3 08/01/96 Improved portability.                             ats       **
#*   4 08/01/96 General cleanup.                                  ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   5 99/04/05 support MAKDIR and CODO                           srm       **
#*****************************************************************************

PORT		=	../../..
TRGTS		=	scf_drvr

include $(PORT)/../make.com

CPUDEFS		=	$(MWOS_DFTDEF)/MC6832X		# 68328 specific defs

# defs files included via defsfile/systype.d
INCLUDES	=	$(CPUDEFS)/m328defs							\
				$(CPUDEFS)/sim328.d $(CPUDEFS)/pllm328.d	\
				$(CPUDEFS)/lcdcm328.d $(CPUDEFS)/rtcm328.d	\
				$(CPUDEFS)/timm328.d $(CPUDEFS)/parp328.d	\
				$(CPUDEFS)/uartm328.d $(CPUDEFS)/spis328.d	\
				$(CPUDEFS)/spim328.d $(CPUDEFS)/pwmm328.d   

SDIR		=	$(OS_SDIR)/IO/SCF/DRVR		# source dir
RDIR		=	RELS
ODIR		=	$(PORT)/CMDS/BOOTOBJS

DEFS		=	-v=. $(PORT_DEFS) -v=$(CPUDEFS) $(OS_MACROS) $(MWOS_DEFS)

MAKERS		=	./makefile

LIBS		=	$(SYS)
SYSN		=	$(MWOS_DFTLIB)/sys.l

SYSDEFS		=	$(MWOS_OSDDIR)/oskdefs.d $(PORT)/systype.d $(INCLUDES)

SCFSTATN	=	$(MWOS_BASLIB)/scfstat.l
MACROS		=	$(OS_MACSDIR)/os9svc.m $(OS_MACSDIR)/longio.m

# serial ports
SCFSRC		=	sc68681.a					# serial driver source
SCFDRV		=	sc68681
SCFDRVR		=	sc68681.r

SPEC_RFLAGS	=	

ASSEMBL		=	$(ASM) $(APARMS) -k -r $(DEFS) \
				$(SPEC_RFLAGS) -eas=$(RDIR) -asn $(AOPTS)

SPEC_LOPTS	=

LINK		=	$(LINKER) $(LPARMS) -k -r -olg $(LIBS) $(SPEC_LOPTS) \
				$(LOPTS) $(COPT_GU0)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
scf_drvr: _makdir $(ODIR)/$(SCFDRV)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link SCF drivers
$(ODIR)/$(SCFDRV): $(SYSN) $(SCFSTATN) $(RDIR)/$(SCFDRVR)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(SCFDRV).stb
	$(LINK) $(SCFSTATN) $(RDIR)/$(SCFDRVR) -fd=$@

$(RDIR)/$(SCFDRVR): $(SDIR)/$(SCFSRC) $(SYSDEFS) $(MACROS) $(MAKERS)
	$(ASSEMBL) $(SDIR)/$(SCFSRC)

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/$(SCFDRVR)

_purge:  nulltrg
	$(CODO) $(ODIR)/$(SCFDRV)
	-$(DEL) $(ODIR)/$(SCFDRV)
	$(CODO) $(ODIR)/STB/$(SCFDRV).stb
	-$(DEL) $(ODIR)/STB/$(SCFDRV).stb

_lint:  nulltrg
	$(COMMENT)

nulltrg:  .
	$(COMMENT)

#																			#
#############################################################################

