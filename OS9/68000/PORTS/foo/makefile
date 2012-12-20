#*****************************************************************************
#* Makefile for entire MC68328ADS port                                      **
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
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   3 08/01/96 Increased portability. General cleanup.           ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   4 12/05/96 Added RBF & ISP subdirectories                    bat       **
#*   5 04/06/99 Remove ISP subdirectories                         srm       **
#*   6 07/16/99 Do not ship IrDA for 68328 for Enh 68k V1.0       lrh       **
#*****************************************************************************

PORT	=       .

if exists(CLOCK/makefile)
CLOCK	= CLOCK
else
CLOCK	=
endif

if exists(PWRMAN/makefile)
PWRMAN	= PWRMAN
else
PWRMAN	=
endif

if exists(SYSIF/makefile)
SYSIF	= SYSIF
else
SYSIF	= 
endif

if exists(ROM/makefile)
ROM	= ROM
else
ROM	= 
endif

TRGTS	=   $(CLOCK) INIT SCF RBF $(PWRMAN) $(SYSIF) UTILS $(ROM) ROM_CBOOT

include $(PORT)/../makesub.com

$(TRGTS): nulltrg
	$(CHD) $*; $(MAKEIT) TARGET=$(TARGET) $(SUBTRGT)

nulltrg:  .
	$(COMMENT)

#                                                                                                                                                       #
#############################################################################

