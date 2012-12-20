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
# 01  ??/??/?? Created
#     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>
# 02  11/24/98 Add ALL_TRGTS, change to MAKESUB,conditional drvr call mgh
# 03  08/16/00 Use port makesub.com.                                  djc
############################################################################
# This makefile will make the ethernet drivers (debug and no-debug) and the#
# descriptor.                                                              #
############################################################################
# 
# Conditionally call driver makefile automatically for BSP vs DEVKITS
#
if exists(spfdrvr.mak)
SPFDRVR = spfdrvr
else
SPFDRVR =
endif

TRGTS		=	spfdesc $(SPFDRVR) #spfdbg
PORT		=	../..

include $(PORT)/../makesub.com

$(TRGTS):  .
	-$(MAKESUB) -f=$@.mak

#                                                                           #
#############################################################################
