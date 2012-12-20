#############################################################################
#           Copyright 1989-2000 by Microware Systems Corporation            #
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
#  01 01/06/99 Changed to use _nulltrg                                  mgh
#  02 03/25/99 Created this makefile from example                       mgh
#  03 08/18/00 Use port makesub.com.                                    djc
############################################################################
#                                                                          #
#
# Conditionally call directory makes automatically if they exist
#

PORT		=	.

#
# run Makefile  in these Directories
#
DIRS		=	INIT SYSMODS SCF RBF SBF SCSI SPF PCF BOOTS ROM_CBOOT
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
