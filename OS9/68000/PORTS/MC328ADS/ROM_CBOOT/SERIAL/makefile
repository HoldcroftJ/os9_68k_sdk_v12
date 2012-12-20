#*****************************************************************************
# Makefile for the I/O driver in the MC68328ADS ROM                         **
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
#*   2 05/14/96 Adopted proper usage of port and base templates.  ats       **
#*              Updated output directory and filename.                      **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   3 06/25/96 Turned on ROMBUG build option.                    ats       **
#*   4 06/26/96 Added CBOOT build option.                         ats       **
#*   5 08/01/96 Improved portability. General Cleanup.            ats       **
#*   6 08/01/96 More cleanup.                                     ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   7 04/14/99 support MAKDIR and CODO                           srm       **
#*****************************************************************************

PORT    = ../..
OLIB    = serial.l
TRGTS   = lib

include $(PORT)/../make.com

SDIR    = $(OS_SDIR)/ROM/SERIAL   # specific source dir
RDIR    = RELS
LIBROOT   = $(PORT)/LIB/ROM

CPUDEFS   = $(MWOS_DFTDEF)/MC6832X    # 68328 defs files

OBJECTS   = $(RDIR)/io68681.r
COMDEFS   = $(MWOS_OSDDIR)/oskdefs.d

# defs files included via defsfile/systype.d
INCLUDES  = $(CPUDEFS)/sim328.d $(CPUDEFS)/pllm328.d  \
				$(CPUDEFS)/lcdcm328.d $(CPUDEFS)/rtcm328.d  \
				$(CPUDEFS)/timm328.d $(CPUDEFS)/parp328.d \
				$(CPUDEFS)/uartm328.d $(CPUDEFS)/spis328.d  \
				$(CPUDEFS)/spim328.d $(CPUDEFS)/pwmm328.d   \
				$(PORT)/systype.d $(COMDEFS)

DEFS    = -v=. $(PORT_DEFS) -v=$(CPUDEFS) \
				$(OS_MACROS) $(MWOS_DEFS)

RBUG    = -asaROMBUG
CBOOT   = -asaCBOOT

INHOUSE   = #-asaINHOUSE

SPEC_RFLAGS = $(INHOUSE) \
			 #-asaFASTCONS -asaFASTCOMM

ASSEMBL   = $(ASM) $(APARMS)$(COPT_SC) $(DEFS) \
				-eas=$(RDIR) $(RBUG) $(CBOOT) $(SPEC_RFLAGS) -r -asn $(AOPTS)

lib: _makdir $(LIBROOT)/$(OLIB)
	$(COMMENT)

$(LIBROOT)/$(OLIB): $(OBJECTS)
	-$(CODO) $@
	$(MERGE) $(OBJECTS) $(REDIR_OUT)$@

$(OBJECTS): $(INCLUDES) $(MAKENAME) $(SDIR)/io68681.a
	$(ASSEMBL) $(SDIR)/io68681.a

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(LIBROOT)

_clean: .
	-$(DEL) $(OBJECTS)

_purge: .
	-$(CODO) $(LIBROOT)/$(OLIB)
	-$(DEL) $(LIBROOT)/$(OLIB)

_lint:  .
	$(COMMENT)

#                                     #
#############################################################################

