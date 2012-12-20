############################################################################
#                                                                          #
#           Copyright 1989-2001 by Microware Systems Corporation           #
#                           All Rights Reserved                            #
#                         Reproduced Under License                         #
#                                                                          #
# This software is confidential property of Microware Systems Corporation, #
# and is provided under license for internal development purposes only.    #
# Reproduction, publication, distribution, or creation of derivative works #
# in any form to any party other than the licensee is strictly prohibited, #
# unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                          #
############################################################################
#                                                                          #
# Edition History:                                                         #
#                                                                          #
# #   Date     Comments                                                By  #
# --- -------- -----------------------------------------------------  ---  #
# 01  04/23/99 Creation.                                              mas  #
# 02  02/06/01 Updated for current make templates.                    djc  #
############################################################################
# This makefile will call TT and MT mkdatmod makefiles                     # 
############################################################################
#                                                                          #

TRGTS		=	MT TT
MWOS		=	../../../..

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS):  .
	$(CHD) $@ ; $(MAKESUB)

#                                                                          #
############################################################################
