#*****************************************************************************
#* Makefile for MC68328ADS Modular ROM Components                           **
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
#*   2 05/14/96 Adopted proper usage of port templates.           ats       **
#*              Corrected copyright date. Updated subdir names.             **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   3 05/14/96 Added FasTrak system state debug support subdirs. ats       **
#*   4 08/02/96 Deactivated FasTrak support subdirs. Gen cleanup. ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   5 07/22/99 Added if exists to support BLS vs. OEM installs   lrh       **
#*   6 08/04/99 Moved CBoot Rom code to ROM_CBOOT                 mgh       **
#*****************************************************************************

PORT    = ..
TRGTS	= CNFGDATA


if exists(ABORT328/makefile)
TRGTS	+=	ABORT328
endif

if exists(CNFGFUNC/makefile)
TRGTS	+=	CNFGFUNC
endif

if exists(CONSCNFG/makefile)
TRGTS	+=	CONSCNFG
endif

if exists(COMMCNFG/makefile)
TRGTS	+=	COMMCNFG
endif

if exists(IO68681/makefile)
TRGTS	+=	IO68681
endif

if exists(M328TIMR/makefile)
TRGTS	+=	M328TIMR
endif

include $(PORT)/../makesub.com

$(TRGTS): nulltrg
	-$(CHD) $*; $(MAKEIT) TARGET=$(TARGET) $(SUBTRGT)

nulltrg:  .
	$(COMMENT)

#                                                                           #
#############################################################################
