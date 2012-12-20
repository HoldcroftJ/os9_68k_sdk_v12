# Makefile
############################################################################
#           Copyright 1989-1997 by Microware Systems Corporation           #
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
# 01  ??/??/?? Created
# 02  04/28/97 Added p386 to ALL_TRGTS                                rds
# 03  05/04/97 Added arm3 to ALL_TRGTS                                rds
#     05/05/97 <----------SPF LAN Comm Pack v3.1 Release----------->
#     05/20/97 <----------SPF LAN Comm Pack v3.1.1 Release--------->
#     04/01/98 <----------SPF LAN Comm Pack v3.2 Release----------->
############################################################################
#  This makefile will make the SPF Ethernet/ARP protocol descriptors       #
############################################################################

#### Add New Descriptor Names Here #########################################
#                                                                          #

TRGTS		=	enet

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
