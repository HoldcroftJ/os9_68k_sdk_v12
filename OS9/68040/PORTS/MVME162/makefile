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
#  03 08/17/00 fix the directory cascading.                             gdw
############################################################################
#                                                                          #
#
# Conditionally call directory makes automatically if they exist
#
#
# run Makefile  in these Directories
#
if exists(SCSI/makefile)
SCSI		= SCSI
else
SCSI		= 
endif

if exists(SYSMODS/makefile)
SYSMODS		= SYSMODS
else
SYSMODS		= 
endif

TRGTS	=	INIT $(SYSMODS) SCF RBF SBF $(SCSI) SPF PCF

#if exists(ISP)
#TRGTS	+=	ISP
#endif

TRGTS	+=	BOOTS ROM_CBOOT

ALL_TRGTS	=	k040
MWOS		=	../../../..

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS): .
	$(CHD) $@;  $(MAKESUB)

#                                                                           #
#############################################################################
