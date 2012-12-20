# Makefile
#############################################################################
# File:   %A%
# Date:   %G%
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
# 02  02/04/98 Updated to use lancom.tpl                              pjw
#     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              #
#############################################################################
#	This makefile will make the SPF LANCE device descriptor                 #
#############################################################################
#																			#
#### Add New Descriptor Names Here ##########################################
#																			#

TRGTS		= sple0	sple1

#																			#
#############################################################################

MWOS		= ../../../../../..
ALL_TRGTS	= k020 
MAKENAME	= spfdesc.mak

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl
include $(DPIO)/MAKETMPL/dpio.tpl

RDIR		=	RELS/DESC
ODIR		=	../../CMDS/BOOTOBJS/SPF
SDIR		=	$(MWOS)/SRC/DPIO/SPF/DESC
DESCDIR		=	DEFS

DEFS            =       $(DPIO_DEFS) -v=$(DESCDIR) -v=$(DRVRDIR) $(BSDDEFS)

include $(SDIR)/desc.tpl

#																			#
#############################################################################
