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
#  00 05/11/99 Created this makefile from example                       lrh
############################################################################
#                                                                          #
-bo

#
# run Makefile  in these Directories
#
TRGTS		=	snoop.make

if exists (clock.make)
TRGTS		+=	clock.make
endif

ALL_TRGTS	=	k040
MWOS		=	../../../../..

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS): _nulltrg
	-$(MAKESUB) -f=$@

#                                                                           #
#############################################################################
