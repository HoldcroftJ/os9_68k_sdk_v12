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
# This makefile will go into each subdirectory and invoke the makefile
# in there
############################################################################
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
#  01 ??/??/?? Created
#  02 10/21/93 <--------- OS-9 NFS 1.3 Released ------------------->
#  03 01/31/94 updated makefile organization                          bat
#  04 05/16/95 Added targets for o386 and pppc                        sr
#  05 01/03/96 Removed o386 : not supported anymore                   pjw
#     04/22/96 <--------- NFS 1.4 Server Release ------------------>
#  06 05/12/97 Add arm3 target                                        rds
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  07 11/05/98 Change to $(MAKESUB), remove ALL_TRGTS                 mgh
############################################################################
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!                 #

MWOS		=	../../../..
TRGTS		=	DIR MSG SORT

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS):  notarget
	$(CHD) $@; $(MAKESUB)

notarget:  .
	$(COMMENT)
#                                                                           #
#############################################################################
