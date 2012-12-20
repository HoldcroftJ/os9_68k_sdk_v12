# Makefile
#*****************************************************************************
#* This makefile builds a MAUI CDB module
#*****************************************************************************
#* Copyright 1995 by Microware Systems Corporation                          **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************

PORT		=	../..
MAKENAME    =   cdb.mak
TRGTS       =   cdb_touch

include $(PORT)/../make.com

ODIR		= $(PORT)/CMDS/BOOTOBJS/MAUI
SDIR		= .
OPTS		= -to=$(OS) -tp=$(CPU) -k -olgu=0.0

$(TRGTS): DIRS $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/cdb_touch : $(SDIR)/cdb.a ./makefile
	$(CODO) $@
	$(CC) $(SDIR)/cdb.a $(OPTS) -fd=$@

DIRS: .
	-$(MAKDIR) $(ODIR)

_clean _purge: .
	$(CODO) $(ODIR)/cdb_touch
	-$(DEL) $(ODIR)/cdb_touch
