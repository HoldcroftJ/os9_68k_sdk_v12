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
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  02 10/28/98 Removed ALL_TRGTS                                      rds
#  03 11/19/98 Changed to use MAKESUB                                 mgh
#  04 11/24/98 Add conditional driver makefile call                   mgh
############################################################################
# This makefile will call SPF driver and descriptor makefiles              # 
############################################################################
#                                                                          #

# 
# Conditionally call driver makefile automatically for BSP vs DEVKITS
#
if exists(spfdrvr.mak)
SPFDRVR = spfdrvr
else
SPFDRVR =
endif

TRGTS		=	spfdesc $(SPFDRVR)
MWOS		=	../../../../..

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS):  notarget
	-$(MAKESUB) -f=$@.mak 

notarget:  .
	$(COMMENT)

#                                                                          #
############################################################################
