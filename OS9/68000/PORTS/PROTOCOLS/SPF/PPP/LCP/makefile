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
#  1  04/02/98 Creation
#     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
#  2  11/24/98 Change to use MAKESUB, simplify                        mgh
#############################################################################

TRGTS           =       spfdesc 
MWOS		=	../../../../../../..
ALL_TRGTS       =       k68k

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS):  notarget
        -$(MAKESUB) -f=$@.mak

notarget:  .
        $(COMMENT)
