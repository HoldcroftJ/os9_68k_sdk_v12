# Makefile
#*****************************************************************************
#* This makefile builds a MAUI Input Process Protocol Module
#*****************************************************************************
#* Copyright 1998 by Microware Systems Corporation                          **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************

PORT		=	../..
MAKENAME    =   pmod.mak

TRGTS		=	mp_t328ads

USER_OPTS   =
USER_HEADERS = 
USER_RFILES = 
USER_LIBS   =

include $(PORT)/../make.com

ODIR		= $(PORT)/CMDS/BOOTOBJS/MAUI
SDIR		= $(MWOS)/SRC/MAUI/MP/MP_T328ADS
RDIR		= RELS
IDIR		= $(RDIR)/$(HOSTTYPE)

include $(SDIR)/../pmod.com

#
# Put USER_RFILES rules (if any) here
#

