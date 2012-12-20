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
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
#  01 09/27/96 Created
#  02 02/01/97 Added -dMWBSD so header files include properly         rds
#     02/19/97 <--------- SPF LAN Comm Pack v3.0 Release ---------->
#  03 05/04/97 Add arm3 to targets, cleaned up makefile               rds
#     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
#     05/20/97 <--------- SPF LAN Comm Pack v3.1.1 Release -------->
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  04 10/28/98 Removed ALL_TRGTS                                      rds
############################################################################

#### Add New Descriptor Names Here #########################################
#                                                                          #

TRGTS		=	tcp0

#                                                                          #
############################################################################

MWOS		=	../../../../..
MAKENAME	=	spfdesc.mak

include $(MWOS)/MAKETMPL/make.com

RDIR		=	RELS/DESC/$(MWOS_LDIR)
ODIR		=	$(MWOS_ODIR)/BOOTOBJS/SPF
SDIR		=	$(MWOS)/SRC/DPIO/SPF/DESC
DESCDIR		=	DEFS

include $(SDIR)/desc.tpl

#                                                                          #
############################################################################
