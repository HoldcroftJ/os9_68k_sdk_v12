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
#  03 04/09/99 Enhanced for MVME147                                     lrh
#  04 08/09/00 Use port makesub.com.                                    djc
############################################################################
#                                                                          #
-bo

#
# run these makefiles
#
TRGTS		=	rbf_descriptors.make \
				rbf_teac_descriptors.make \
				rbf_vccs_descriptors.make

PORT		=	..

include $(PORT)/../makesub.com

$(TRGTS): .
	-$(MAKESUB) -f=$@

#                                                                           #
#############################################################################
