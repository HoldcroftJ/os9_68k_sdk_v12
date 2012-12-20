# Makefile
#*****************************************************************************
#* ID:     %A%
#* Date:   %G%
#*****************************************************************************
#* Makefile for MC68328ADS TouchpadSCF modules                              **
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
#*              Corrected copyright date.                                   **
#*              ---- OS-9/68K 68328 Support Beta Release ----               **
#*   3 07/08/96 Modified for TPAD usage from SC68681 usage        rws       **
#*****************************************************************************

PORT		=	../..
TRGTS       =   DESC DRVR TEST

include $(PORT)/../makesub.com

$(TRGTS): nulltrg
	-$(CHD) $*; $(MAKEIT) TARGET=$(TARGET) $(SUBTRGT)

nulltrg:  .
	$(COMMENT)

#																			#
#############################################################################
