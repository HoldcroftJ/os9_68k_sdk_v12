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
TRGTS       = cdb

include $(PORT)/../make.com

ODIR		= $(PORT)/CMDS/BOOTOBJS/MAUI
SDIR		= .
OPTS		= -to=$(OS) -tp=$(CPU) -k -olgu=0.0

$(TRGTS): DIRS $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/cdb : $(SDIR)/cdb.a
	$(CODO) $@
	$(CC) $(SDIR)/$*.a $(OPTS) -fd=$@

DIRS: .
	-$(MAKDIR) $(ODIR)

_clean _purge: .
	$(CODO) $(ODIR)/cdb
	-$(DEL) $(ODIR)/cdb
