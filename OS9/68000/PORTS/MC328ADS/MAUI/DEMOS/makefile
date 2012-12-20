# Makefile
#*****************************************************************************
#* Makefile for MP68328-ADS MAUI DEMO objects                               **
#*****************************************************************************
#* Copyright 1997 by Microware Systems Corporation                          **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************

PORT		=	../..
TRGTS		=	DEMO DRAW MAUIDEMO PENP SLIDSHOW

include $(PORT)/../makesub.com

$(TRGTS): nulltrg
	-$(CHD) $*; $(MAKEIT) TARGET=$(TARGET) $(SUBTRGT)

nulltrg:  .
	$(COMMENT)

