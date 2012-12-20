#*****************************************************************************
#* Makefile for MC68328ADS SCF modules                                      **
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
#*              Corrected copyright date. Updated subdir names.             **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   3 08/01/96 Increased portability. General cleanup.           ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*****************************************************************************

PORT		=	..
TRGTS       =   SC68681 TPAD

include $(PORT)/../makesub.com

$(TRGTS): nulltrg
	-$(CHD) $*; $(MAKEIT) TARGET=$(TARGET) $(SUBTRGT)

nulltrg:  .
	$(COMMENT)

#																			#
#############################################################################
