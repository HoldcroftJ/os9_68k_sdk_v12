# Makefile
#*****************************************************************************
#* Makefile for MC68328-ADS PwrMan objects                                  **
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
#*     01/17/97 <--------------- PwrMan v2.1 --------------->               **
#*****************************************************************************

PORT		=	..
MWOS		=	$(PORT)/../../../..
TRGTS       =   PWREXT PWRPLCY
ALL_TRGTS	=	k68k
MAKENAME	=	makefile
include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS): nulltrg
	$(CHD) $*; $(MAKEIT) TARGET=$(TARGET) $(SUBTRGT)

nulltrg:  .
	$(COMMENT)

#																			#
#############################################################################

