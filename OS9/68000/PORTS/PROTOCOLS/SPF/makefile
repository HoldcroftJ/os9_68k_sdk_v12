# Makefile
############################################################################
#           Copyright 1989-1998 by Microware Systems Corporation           #
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
############################################################################
# This makefile will call SPF driver and descriptor makefiles              # 
############################################################################
#									#

TRGTS		=	SLIP PPP ETC
MWOS		=	../../../../..
ALL_TRGTS	=	k68k

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS):  nulltrgt
	$(CHD) $@ ; $(MAKESUB)

nulltrgt:  .
	$(COMMENT)

#																			#
#############################################################################
