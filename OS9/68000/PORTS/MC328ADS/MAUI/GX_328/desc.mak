# Makefile
#*****************************************************************************
#* Makefile for MC68328-ADS LCD MAUI descriptor objects                     **
#*****************************************************************************
#* Copyright 1996-1997 by Microware Systems Corporation                     **
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
#*   2 08/12/97 pulled into MAUI release                          bat       **
#*****************************************************************************

#### Add New Descriptor Names Here ##########################################
#																			#
TRGTS		=	gfx gfx1 gfx2
DRVR		=	GX_328
#																			#
#############################################################################

PORT		=	../..
MAKENAME	=	desc.mak
include $(PORT)/../make.com

RDIR		=	RELS/DESC
ODIR		=	$(PORT)/CMDS/BOOTOBJS/MAUI
SDIR		=	$(MWOS)/SRC/DPIO/MFM/DESC
DESCDIR		=	.

# pass -gu=0.0 to desc.tpl
USER_OPTS	=	$(COPT_GU0)

include $(SDIR)/desc.tpl

_purge _clean:  .
	$(CODO) $(ODIR)/gfx
	-$(DEL) $(ODIR)/gfx
	$(CODO) $(ODIR)/gfx1
	-$(DEL) $(ODIR)/gfx1
	$(CODO) $(ODIR)/gfx2
	-$(DEL) $(ODIR)/gfx2

#																			#
#############################################################################

