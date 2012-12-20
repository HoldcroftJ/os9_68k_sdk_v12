# Makefile
#*****************************************************************************
#* Makefile for MC68328 LCD Driver
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
#*   1 02/13/96 creation                                          bat       **
#*   2 08/06/97 pulled into MAUI release                          bat       **
#*****************************************************************************

#### Put Driver Names and Options Here ######################################
#																			#
TRGTS		=	gx_328
DRVR		=	GX_328

# DEFINES definitions
#   PWR_AWARE : make driver PwrMan-compatible (also need pwrman.l/.il in ADDITIONAL_LIBS)
USR_DEFINES	=   

#DEBUG		=	-g
DEBUG		=	

#																			#
#############################################################################

PORT		=	../..
MAKENAME	=	drvr.mak
include $(PORT)/../make.com

ODIR		=	$(PORT)/CMDS/BOOTOBJS/MAUI
RDIR		=	RELS/DRVR
IDIR		=	$(RDIR)/$(HOSTTYPE)
DESCDIR		=	.
ADDITIONAL_IFILES   =
ADDITIONAL_LIBS = # -l=$(MWOS_LIBDIR)/pwrman.l -ill=$(MWOS_ILIBDIR)/pwrman.il

include $(MWOS)/SRC/DPIO/MFM/DRVR/$(DRVR)/drvr.tpl

#																			#
#############################################################################

