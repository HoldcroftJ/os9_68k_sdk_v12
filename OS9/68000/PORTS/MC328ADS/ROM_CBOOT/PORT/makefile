#*****************************************************************************
#* Makefile for port modules in the MC68328ADS ROM                          **
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
#*   2 11/13/95 Added ROM_128K macro                              rws       **
#*   3 05/14/96 Adopted proper usage of port and base templates.  ats       **
#*              Updated output directory and filename.                      **
#*   4 06/20/96 Build with ROMBug by default.                     ats       **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   5 06/26/96 Added CBOOT build option.                         ats       **
#*              Correct purge target.                                       **
#*   6 08/01/96 Improved portability. General cleanup.            ats       **
#*   8 08/01/96 More cleanup.                                     ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   9 04/14/99 support MAKDIR and CODO                           srm       **
#*  10 06/18/99 Shutting off linker workaround.                   rkw       **
#*****************************************************************************

PORT		=	../..
OLIB		=	port.l
TRGTS		=	lib

include $(PORT)/../make.com

# Testing options -- ROM_128K overriden in makefile
ROM_128K    =   #-asaROM_128K	# 128K EPROMs being used

SDIR		=	.							# specific source dir
RDIR		=	RELS
LIBROOT		=	$(PORT)/LIB/ROM

CPUDEFS		=	$(MWOS_DFTDEF)/MC6832X		# 68328 defs

SYSINIT		=	$(RDIR)/sysinit.r
SYSCON		=	$(RDIR)/syscon.r
BOOTIO		=	$(RDIR)/bootio.r

OBJECTS		=	$(SYSINIT) $(SYSCON) $(BOOTIO)

COMDEFS		=	$(MWOS_OSDDIR)/oskdefs.d

# defs files included via defsfile/systype.d
INCLUDES	=	$(CPUDEFS)/sim328.d $(CPUDEFS)/pllm328.d	\
				$(CPUDEFS)/lcdcm328.d $(CPUDEFS)/rtcm328.d	\
				$(CPUDEFS)/timm328.d $(CPUDEFS)/parp328.d	\
				$(CPUDEFS)/uartm328.d $(CPUDEFS)/spis328.d	\
				$(CPUDEFS)/spim328.d $(CPUDEFS)/pwmm328.d	\
				$(PORT)/systype.d $(COMDEFS)

DEFS		=	-v=. $(PORT_DEFS) -v=$(CPUDEFS) $(OS_MACROS) $(MWOS_DEFS) \
				-v=$(OS_ROMSDIR)/CBOOT/DEFS -v=$(MWOS_DDIR)/IO/SCSI

RBUG		=	-asaROMBUG
ACBOOT		=	-asaCBOOT

CBUG		=	#-dNOBUG
CCBOOT		=	-dCBOOT

INHOUSE		=	-asaL68_WORKAROUND=0 #-asaINHOUSE

SPEC_CFLAGS	=	$(CBUG) $(CCBOOT)

COMPILE		=	$(CC) $(CPARMS) $(DEFS) -eas=$(RDIR) -r -asb \
				$(SPEC_CFLAGS) -asn $(COPTS)

SPEC_RFLAGS	=	$(ROM_128K) $(RBUG) $(ACBOOT) $(INHOUSE) 

ASSEMBL		=	$(ASM) $(APARMS)$(COPT_SC) $(DEFS) -eas=$(RDIR) -r -asb \
				$(SPEC_RFLAGS) -asn $(AOPTS)

lib: _makdir $(LIBROOT)/$(OLIB)
	$(COMMENT)

$(LIBROOT)/$(OLIB): $(OBJECTS)
	-$(CODO) $@
	$(MERGE) $(OBJECTS) $(REDIR_OUT)$@

$(SYSINIT): $(INCLUDES) $(MAKENAME) $(SDIR)/sysinit.a
	$(ASSEMBL) $(SDIR)/sysinit.a

$(SYSCON):	$(INCLUDES) $(MAKENAME) $(SDIR)/syscon.c
	$(COMPILE) $(SDIR)/syscon.c

$(BOOTIO):	$(INCLUDES) $(MAKENAME) $(SDIR)/bootio.c
	$(COMPILE) $(SDIR)/bootio.c

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(LIBROOT)

_clean: .
	-$(DEL) $(BOOTIO)
	-$(DEL) $(SYSINIT)
	-$(DEL) $(SYSCON)

_purge: .
	-$(CODO) $(LIBROOT)/$(OLIB)
	-$(DEL) $(LIBROOT)/$(OLIB)

_lint:  .
	$(COMMENT)

#																			#
#############################################################################

