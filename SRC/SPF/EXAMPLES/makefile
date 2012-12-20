# Makefile
############################################################################
#           Copyright 1989-1998 by Microware Systems Corporation           #
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
# This makefile will go into each subdirectory and invoke the makefile 
# in there
############################################################################
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
# 01  12/03/98 Added Copywrite information.                            sr
#############################################################################


TRGTS		=	EXAMPLE1 EXAMPLE2 EXAMPLE3
MWOS		= 	../../..

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS): nulltrg
	$(CHD) $@; $(MAKESUB)

nulltrg:  .
	$(COMMENT)

	
# End of makefile						
					
#############################################################################
