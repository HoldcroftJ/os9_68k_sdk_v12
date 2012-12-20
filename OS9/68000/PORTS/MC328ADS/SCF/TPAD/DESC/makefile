# Makefile
#*****************************************************************************
#* ID:     %A%
#* Date:   %G%
#*****************************************************************************
#* Makefile for MC68328ADS 68681 SCF descriptors.                           **
#*****************************************************************************
#* Copyright 1996 by Microware Systems Corporation                          **
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
#*   1 04/30/96 creation                                          bat       **
#*   2 05/13/96 Adopted proper usage of port and base templates.  ats       **
#*              Corrected usage of -v=$(PORT).                              **
#*   3 05/15/96 Corrected definition of FASTCONS, FASTCOMM.       ats       **
#*   4 05/20/96 Corrected output directory.                       ats       **
#*   5 05/20/96 Added p1 descriptor.                              ats       **
#*              ---- OS-9/68K 68328 Support Beta Release ----               **
#*   6 07/08/96 Modified makefile for TPAD usage                  rws       **
#*   7 99/04/05 support MAKDIR and CODO                           srm       **
#*****************************************************************************

PORT		=	../../..

TRGTS		=	tpad_desc

include $(PORT)/../make.com

CPUDEFS		=	$(MWOS_DFTDEF)/MC6832X			# 68328 specific defs

SDIR		=	$(OS_SDIR)/IO/SCF/DESC			# source files
ODIR		=	$(PORT_OBJDIR)/DESC/TPAD		# output
RDIR		=	RELS

DEFS		=	-v=. $(PORT_DEFS) -v=$(CPUDEFS) $(MWOS_DEFS)

SYSDEFS		=	$(MWOS_OSDDIR)/oskdefs.d $(PORT)/systype.d

LIBS		=	$(SYS)
SYSN		=	$(MWOS_BASLIB)/sys.l

SPEC_RFLAGS	=	-asaCYC_ALM_PERIOD=3 -asaSAMPLES_PER_POINT=3

ASSEMBL		=	$(ASM) $(APARMS) -k -r $(DEFS) \
				$(SPEC_RFLAGS) -eas=$(RDIR) -asn $(AOPTS)

SPEC_LOPTS	=	

LINK		=	$(LINKER) $(LPARMS) -k -r -olS $(LIBS) $(SPEC_LOPTS) \
				$(LOPTS) $(COPT_GU0)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
$(TRGTS): _makdir $(ODIR)/tpad
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link serial port descriptors

$(ODIR)/tpad: $(SYSN) $(RDIR)/tpad.r
	$(CODO) $@
	$(CODO) $(ODIR)/STB/tpad.stb
	$(LINK) $(RDIR)/tpad.r -fd=$@

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly

$(RDIR)/tpad.r: $(SYSDEFS) ./makefile
	$(ASSEMBL) $(SPEC_RFLAGS) -asaX_PIXELS=240 -asaY_PIXELS=160 \
	$(SDIR)/tpad.a -fd=tpad.r -asn

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/tpad.r

_purge:  nulltrg
	$(CODO) $(ODIR)/tpad
	-$(DEL) $(ODIR)/tpad
	$(CODO) $(ODIR)/STB/tpad.stb
	-$(DEL) $(ODIR)/STB/tpad.stb

_lint:  nulltrg
	$(COMMENT)

nulltrg:  .
	$(COMMENT)

#																			#
#############################################################################

