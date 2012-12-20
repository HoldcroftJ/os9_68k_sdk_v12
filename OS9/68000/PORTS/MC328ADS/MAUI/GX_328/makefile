# Makefile
#############################################################################
#                                                                           #
#              Copyright 1996 by Microware Systems Corporation              #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, distribution, or creation of derivative works #
#  in any form to any party other than the licensee is strictly prohibited, #
#  unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                           #
#############################################################################

PORT		=	../..
TRGTS		=	desc.mak

if exists(drvr.mak)
TRGTS	+=	drvr.mak
endif

include $(PORT)/../makesub.com

$(TRGTS):  notarget
	$(MAKE) -f $@ $(MAKEOPTS) TARGET=$(TARGET) $(SUBTRGT)

notarget:  .
	$(COMMENT)

#																			#
#############################################################################
