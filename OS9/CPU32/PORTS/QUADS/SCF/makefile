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
#  01 06/24/99 Cloned from mvme162 version                              djl
#  02 07/28/00 Added BLS support.                                       GbG
#  03 08/08/00 Removed ALL_TRGTS. Use port makesub.com.                 djc
############################################################################
#                                                                          #
-bo

if exists(scf_drivers.make)
DRVRMAKE = scf_drivers.make
else
DRVRMAKE = 
endif

#
# run Makefile  in these Directories
#
TRGTS		= scf_descriptors.make $(DRVRMAKE) 
PORT		= ../

include $(PORT)/../makesub.com

$(TRGTS): .
	-$(MAKESUB) -f=$@

#                                                                           #
#############################################################################

