#*****************************************************************************
#* Makefile for the common boot modules in the MC68328ADS ROM               **
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
#*   6 08/01/96 Improved portability. General cleanup.            ats       **
#*   7 08/01/96 Corrected ROM_128K build option.                  ats       **
#*   8 08/01/96 More cleanup.                                     ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   9 04/14/99 support MAKDIR and CODO                           srm       **
#*  10 10/25/99 Removed COPT_SC which caused fatal branch resize. rkw       **
#*****************************************************************************

PORT		=	../..
OLIB		=	common.l
TRGTS		=	lib

include $(PORT)/../make.com

# Testing options -- overriden in makefile
ROM_128K    =	#-asaROM_128K	# 128K EPROMs being used

SDIR		=	$(OS_SDIR)/ROM/COMMON		# specific source dir
RDIR		=	RELS
LIBROOT		=	$(PORT)/LIB/ROM

CPUDEFS		=	$(MWOS_DFTDEF)/MC6832X		# 68328 defs

SYSBOOT		=	#$(RDIR)/sysboot.r
OBJECTS		=	$(RDIR)/vectors.r $(RDIR)/boot.r $(SYSBOOT)

COMDEFS		=	$(MWOS_OSDDIR)/oskdefs.d
INCLUDES	=	$(PORT)/systype.d $(COMDEFS)

DEFS		=	-v=. $(PORT_DEFS) -v=$(CPUDEFS) -v=$(OS_MACSDIR) $(MWOS_DEFS)

RBUG		=	-asaROMBUG
CBOOT		=	-asaCBOOT

INHOUSE		=	#-asaINHOUSE

SPEC_RFLAGS	=	$(INHOUSE) $(ROM_128K) -asaMANUAL_RAM -asaDONTENTER_DEBUGGER -b \
				#-asaFASTCONS -asaFASTCOMM

ASSEMBL		=	$(ASM) $(APARMS) -as=-bt $(DEFS) -eas=$(RDIR) -r  \
				$(RBUG) $(CBOOT) $(SPEC_RFLAGS) -asn $(AOPTS)           \
				# -asb can't use for modular rom

lib: _makdir $(LIBROOT)/$(OLIB)
	$(COMMENT)

$(LIBROOT)/$(OLIB): $(OBJECTS)
	-$(CODO) $@
	$(MERGE) $(OBJECTS) $(REDIR_OUT)$@

$(RDIR)/vectors.r: $(INCLUDES) $(MAKENAME)
	$(ASSEMBL) $(SDIR)/vectors.a

$(RDIR)/boot.r: $(INCLUDES) $(MAKENAME)
	$(ASSEMBL) $(SDIR)/boot.a

$(RDIR)/sysboot.r: $(INCLUDES) $(MAKENAME)
	$(ASSEMBL) $(SDIR)/sysboot.a

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(LIBROOT)

_clean: .
	-$(DEL) $(RDIR)/vectors.r
	-$(DEL) $(RDIR)/boot.r
	-$(DEL) $(RDIR)/sysboot.r

_purge: .
	-$(CODO) $(LIBROOT)/$(OLIB)
	-$(DEL) $(LIBROOT)/$(OLIB)

_lint:  .
	$(COMMENT)

#																			#
#############################################################################

