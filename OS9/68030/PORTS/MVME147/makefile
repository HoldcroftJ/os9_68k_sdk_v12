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
#  00 05/19/99 Created this makefile from example                       lrh
#  01 06/25/99 Changed ROM dirctory to ROM_CBOOT                        mgh
############################################################################
#                                                                          #
#
# Conditionally call directory makes automatically if they exist
#
#
# run Makefile  in these Directories
#

PORT = .
DIRS	=	INIT SYSMODS SCF RBF SBF SCSI SPF PCF BOOTS ROM_CBOOT

TRGTS   =
for DIR in $(DIRS)
if exists ($(DIR)/makefile)
TRGTS   +=      $(DIR)
endif
endfor

include $(PORT)/../makesub.com

$(TRGTS): .
	-$(CHD) $@;  $(MAKESUB)

#                                                                           #
#############################################################################

