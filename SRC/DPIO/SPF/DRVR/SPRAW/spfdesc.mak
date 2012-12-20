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
#  01 06/29/97 Created                                                rds
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  02 11/20/98 Remove ALL_TRGTS                                       mgh
############################################################################

#### Add New Descriptor Names Here ##########################################
#                                                                           #

TRGTS		=	raw0

#                                                                           #
#############################################################################

MWOS		=	../../../../..

MAKENAME	=	spfdesc.mak

include $(MWOS)/MAKETMPL/make.com

RDIR		=	RELS/DESC/$(MWOS_LDIR)
ODIR		=	$(MWOS_ODIR)/BOOTOBJS/SPF
SDIR		=	$(MWOS)/SRC/DPIO/SPF/DESC
DESCDIR		=	DEFS

include $(SDIR)/desc.tpl

#                                                                           #
#############################################################################
