#############################################################################
#                                                                           #
#              Copyright 2001 by Microware Systems Corporation              #
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
#                                                                           #
# Edition History:                                                          #
# #   Date     Comments                                                By   #
# --- -------- ------------------------------------------------------- ---  #
#  01 01/02/06 Created.                                                djc  #
#  02 01/05/03 Removed AGFA.                                           djc  #
#############################################################################
# makefile that calls sub-makefiles in asset directories.                   #
#############################################################################


#############################################################################
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!                  #
#                                                                           #

MWOS	=	../..

DIRS	=	FONTS IMAGES

TRGTS =
for DIR in $(DIRS)
if exists ($(DIR)/makefile)
TRGTS += $(DIR)
endif
endfor

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS):  .
	-$(CHD) $@ ; $(MAKESUB)

#                                                                           #
# End of makefile                                                           #
#############################################################################
