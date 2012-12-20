#############################################################################
#
#  Filename: makefile
#  Purpose:  Build the CDB validation utility
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


MWOS		=	../../../..

TRGTS		=	cdbval

include 		../maui.com

CFILES		=	$(SDIR)/cdbval.c	\
				$(SDIR)/ll.c		\
				$(SDIR)/validate.c	\
				$(SDIR)/utils.c

DEPENDS		=	$(SDIR)/cdbval.h	\
				$(SDIR)/dev_types.h	\
				$(SDIR)/history.h

cdbval: $(ODIR)/STB $(ODIR)/cdbval
	$(COMMENT)

$(ODIR)/cdbval: $(CFILES) $(DEPENDS) $(LIBS_DEP) ./$(MAKENAME)
	$(CODO) $@
	-$(DEL) $@
	$(CODO) $(ODIR)/STB/cdbval.stb
	-$(DEL) $(ODIR)/STB/cdbval.stb
	-$(DEL) $(ODIR)/STB/cdbval.dbg
	$(LINK) $(CFILES) -fd=$@


_clean _purge:  
	$(CODO) $(ODIR)/cdbval
	-$(DEL) $(ODIR)/cdbval
	$(CODO) $(ODIR)/STB/cdbval.stb
	-$(DEL) $(ODIR)/STB/cdbval.stb
	-$(DEL) $(ODIR)/STB/cdbval.dbg

$(ODIR)/STB:
	-$(MAKDIR) $@


#############################################################################
