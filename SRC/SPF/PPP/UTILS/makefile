############################################################################
#                                                                          #
#           Copyright 2000-2002 by Microware Systems Corporation           #
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
#                                                                          #
# Edition History:                                                         #
#                                                                          #
# #   Date     Comments                                                By  #
# --- -------- -----------------------------------------------------  ---  #
#  01 00/12/12 Created                                                djc  #
#  02 01/09/07 Conditionalized directories.                           djc  #
#  03 01/10/17 Removed chat utility.                                  djc  #
#  04 02/01/24 Replaced chat for 68k only.                            djc  #
############################################################################
# makefile that produces the PPP utilities by invoking the makefile        #
# in each sub-directory                                                    #
############################################################################


############################################################################
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!                 #
#                                                                          #

MWOS	=	../../../..
DIRS	=	CHAT PPP_AUTH PPPD

TRGTS =
for DIR in $(DIRS)
if exists($(DIR)/makefile)
TRGTS += $(DIR)
endif
endfor

include $(MWOS)/MAKETMPL/makesub.com

CHAT: .
# only build chat for 68k processors
if $(OS) == "OSK"
	$(CHD) $@ ; $(MAKESUB)
else
	$(COMMENT)
endif

PPP_AUTH PPPD: .
	$(CHD) $@ ; $(MAKESUB)

#                                                                          #
# End of makefile                                                          #
############################################################################

