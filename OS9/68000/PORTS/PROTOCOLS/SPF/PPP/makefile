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
#   1 10/21/96 Initial Version                                        billw
#     05/07/97 <***** SPF LAN Comm Pack v3.1 Release **********>
#   2 10/21/97 Cleaned up to call subdirectories correctly            pjw
#     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
#   3 08/21/98 Removed make.com template                              rds
#   4 11/24/98 Change to MAKSUB, simplify                             mgh
#############################################################################

MWOS		=	../../../../../..

TRGTS		=	HDLC LCP IPCP
ALL_TRGTS	=	k68k

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS):	nulltrg
	$(CHD) $@; $(MAKESUB)

nulltrg	: .
	$(COMMENT)
