#*****************************************************************************
#* Makefile for MC68328ADS init modules                                     **
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
#*   2 05/13/96 Adopted proper usage of port templates.           ats       **
#*              Corrected copyright date. Standardise targets.              **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   3 05/17/96 Added FTSYSDBG subdirectory.                      ats       **
#*   4 08/01/96 Increased portability. General cleanup.           ats       **
#*              Deactivated FTSYSDBG subdir for 2nd Beta.                   **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   5 12/05/96 changed TRGTS to DD_DISK from NODISK              bat       **
#*****************************************************************************

PORT		=	..
TRGTS       =   DD_DISK

include $(PORT)/../makesub.com

$(TRGTS): nulltrg
	-$(CHD) $*; $(MAKEIT) TARGET=$(TARGET) $(SUBTRGT)

nulltrg:  .
	$(COMMENT)

#																			#
#############################################################################
