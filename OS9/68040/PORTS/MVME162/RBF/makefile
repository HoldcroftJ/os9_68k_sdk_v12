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
#  00 12/01/98 Example makefile to call other xxx.make files            mgh
#  01 01/06/99 Changed to use _nulltrg                                  mgh
#  02 03/25/99 Created this makefile from example                       mgh
#  03 04/09/99 Tweeked for MVME162                                      djl
############################################################################
#                                                                          #
-bo

#
# run Makefiles in this Directory and subdirectories
#
TRGTS		=	rbf_descriptors.make \
				rbf_nvram_descriptors.make \
				rbf_vccs_descriptors.make \
				rbf_teac_descriptors.make

if exists(rbf_sccs_descriptors.make)
TRGTS		+= 	rbf_sccs_descriptors.make
endif

if exists(rbftl.make)
TRGTS		+= 	rbftl.make
endif


ALL_TRGTS	=	k040
MWOS		=	../../../../..

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS): _nulltrg
	-$(MAKESUB) -f=$@

#                                                                           #
#############################################################################
