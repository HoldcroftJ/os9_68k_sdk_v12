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
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
# 01  05/05/97 Created                                                rds
# 02  11/04/98 change to $(MAKESUB), remove ALL_TRGTS etc.            mgh
# 03  01/23/01 Added MULTICAST.                                       djc
############################################################################

MWOS		=	../../../..
TRGTS		=	AF_INET.TCP AF_INET.UDP MULTICAST
MAKENAME	=	makefile

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS):  .
	$(CHD) $@; $(MAKESUB)

#                                                                           #
#############################################################################
