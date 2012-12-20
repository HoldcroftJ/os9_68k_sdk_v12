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
#  00 05/11/99 Created this makefile from example                       lrh
#  01 07/27/00 Made BLS ready.                                          GbG
#  02 08/08/00 Removed ALL_TRGTS. Use port makesub.com.                 djc
############################################################################
#                                                                          #
-bo

#
# run Makefile  in these Directories
#

if exists (clock.make)
TRGTS = clock.make
else
TRGTS =	
endif

PORT = ../

include $(PORT)/../makesub.com

$(TRGTS): .
	$(MAKESUB) -f=$@

#                                                                           #
#############################################################################
