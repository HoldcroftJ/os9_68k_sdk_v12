#############################################################################
#
#  Filename: makefile
#  Purpose:  Build a test CDB module for cdbval
#
#############################################################################
#                                                                           #
#           Copyright 2001 - 2001 by Microware Systems Corporation          #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
# This software is confidential property of Microware Systems Corporation,  #
# and is provided under license for internal development purposes only.     #
# Reproduction, publication, modification, distribution, or creation of     #
# derivative works in any form to any party other than the Licensee is      #
# strictly prohibited, unless expressly provided for under the terms of a   #
# mutually executed written license agreement for this software between the #
# end-user and Microware Systems Corporation, or otherwise expressly        #
# authorized in writing by Microware Systems Corporation. Licensee shall    #
# reproduce this copyright notice exactly as provided herein on any copies  #
# of the software that Licensee is authorized to distribute.                #
#############################################################################


PORT		=	../..
TRGTS       =	cdbval_test_cdb

MAKENAME	=	cdb.mak

include			$(MWOS)/MAKETMPL/make.com

ODIR		=	$(MWOS_ODIR)/MAUIDEMO
SDIR		=	.
OPTS		=	-to=$(OS) -tp=$(CPU) -k -olgu=0.0

$(TRGTS): $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS): $(ODIR) $(SDIR)/cdbval_test_cdb.a $(ODIR)/cdbval ./$(MAKENAME)
	$(CODO) $@
	-$(DEL) $@
	$(CC) $(SDIR)/$*.a $(OPTS) -fd=$@
	$(CHD) $(ODIR)
	-$(DEL) cdbval.tmp
	$(MOVE) cdbval cdbval.tmp
	$(MERGE) cdbval.tmp cdbval_test_cdb -o=cdbval
	$(DEL) cdbval.tmp

$(ODIR):
	-$(MAKDIR) $@

_clean _purge: .
	$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL) $(ODIR)/$(TRGTS)
