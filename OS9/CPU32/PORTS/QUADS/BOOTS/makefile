#############################################################################
#                Copyright 2000 by Microware Systems Corporation            #
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
#  00 07/27/00 Example makefile to call other xxx.make files            GbG
#  01 08/08/00 Remove ALL_TRGTS. Use port makesub.com.                  djc
############################################################################
#                                                                          #
-bo

#
# run Makefile  in these Directories
#
TRGTS		=	qe_netboot.make

PORT		=	..

include $(PORT)/../makesub.com

$(TRGTS): .
	-$(MAKESUB) -f=$@

#                                                                           #
#############################################################################
