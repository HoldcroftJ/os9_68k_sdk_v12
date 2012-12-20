############################################################################
#           Copyright 1989-2000 by Microware Systems Corporation           #
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
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
#     ??/??/?? Creation
#  02 11/10/98 Change to MAKSUB, simplify,                            mgh
#  03 08/08/00 Removed ALL_TRGTS.  Use port makesub.com.              djc
############################################################################
# This makefile will call SPF driver and descriptor makefiles              # 
############################################################################
#                                                                          #

TRGTS		=	SPQUICC ETC
PORT		=	..

include $(PORT)/../makesub.com

$(TRGTS): .
	-$(CHD) $@ ; $(MAKESUB)

#                                                                           #
#############################################################################
