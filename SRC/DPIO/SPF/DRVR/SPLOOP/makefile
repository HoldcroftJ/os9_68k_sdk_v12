# Makefile
#############################################################################
#           Copyright 1989-1998 by Microware Systems Corporation            #
#                           All Rights Reserved                             #
#                         Reproduced Under License                          #
#                                                                           #
# This software is confidential property of Microware Systems Corporation,  #
# and is provided under license for internal development purposes only.     #
# Reproduction, publication, distribution, or creation of derivative works  #
# in any form to any party other than the licensee is strictly prohibited,  #
# unless expressly authorized in writing by Microware Systems Corporation.  #
#                                                                           #
#############################################################################
#									    
# Edition History:
#
# #   Date     Comments                                                 By
# --- -------- -------------------------------------------------------  ---
#  00 12/01/98 Example makefile to call other xxx.make files            mgh
#  01 12/02/98 Added history, changed to MAKESUB, makesub.com           mgh
#############################################################################
# This makefile will make the all of the SPF port-specific modules.         #
#############################################################################
#                                                                           #
# 
# Conditionally call driver makefile automatically for BLS vs OEM

if exists(spfdrvr.mak)
SPFDRVR = spfdrvr
else
SPFDRVR =
endif
if exists(spfdbg.mak)
SPFDBG = spfdbg
else
SPFDBG =
endif

TRGTS		=	$(SPFDRVR) $(SPFDBG) spfdesc

MWOS		=	../../../../..
MAKENAME	=	makefile

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS):  notarget
	$(MAKESUB) -f=$@.mak

notarget:  .
	$(COMMENT)

#                                                                           #
#############################################################################
