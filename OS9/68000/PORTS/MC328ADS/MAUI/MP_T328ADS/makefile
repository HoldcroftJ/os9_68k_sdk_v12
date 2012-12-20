# Makefile
#############################################################################
#                                                                           #
#              Copyright 1996 by Microware Systems Corporation              #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, distribution, or creation of derivative works #
#  in any form to any party other than the licensee is strictly prohibited, #
#  unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                           #
#############################################################################

#
# Conditionally call driver makefile automatically for BSP vs DEVKITS
#
if exists(pmod.mak)
DRVRMAKE = pmod.mak
else
DRVRMAKE = 
endif

PORT		=	../..
MAK_TRGTS	=	cdb.mak $(DRVRMAKE) 
TRGTS		=	$(MAK_TRGTS)

include $(PORT)/../makesub.com

$(MAK_TRGTS):
	-$(MAKESUB) -f=$@

if exists(TOUCH_CAL)
$(DIR_TRGTS):
	$(CHD) $@ ; $(MAKESUB)
endif

#																			#
#############################################################################

