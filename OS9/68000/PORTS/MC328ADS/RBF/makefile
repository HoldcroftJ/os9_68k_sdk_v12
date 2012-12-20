#*****************************************************************************
#* Makefile for MC68328ADS RBF modules                                      **
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
#*   1 11/15/96 creation                                          bat       **
#*****************************************************************************

PORT		=	..
TRGTS       =   RAM

include $(PORT)/../makesub.com

$(TRGTS): nulltrg
	$(CHD) $*; $(MAKEIT) TARGET=$(TARGET) $(SUBTRGT)

nulltrg:  .
	$(COMMENT)

#																			#
#############################################################################

