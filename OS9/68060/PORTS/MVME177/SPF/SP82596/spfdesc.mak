# Makefile
#############################################################################
# File:   %A%
# Date:   %G%
#############################################################################
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
#   1 ??/??/?? Creation
#     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              #
#############################################################################
#	This makefile will make the SPF  82596 driver descriptors	#
#############################################################################
#									#

#### Add New Descriptor Names Here ##########################################
#									#
TRGTS		=	spie0
#									#
#############################################################################

MWOS		=	../../../../../..
ALL_TRGTS   	=   k020
MAKENAME	=	spfdesc.mak
include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl
include $(DPIO)/MAKETMPL/dpio.tpl

RDIR		=	RELS/DESC
ODIR		=	../../CMDS/BOOTOBJS/SPF
SDIR		=	$(MWOS)/SRC/DPIO/SPF/DESC
DESCDIR		=	DEFS
DEPENDS		=	$(MWOS)/SRC/DPIO/SPF/DRVR/SP82596/defs.h \
			    DEFS/spf_desc.h
DEFS            =       $(DPIO_DEFS) -v=$(DESCDIR) -v=$(DRVRDIR) $(BSDDEFS)

include $(SDIR)/desc.tpl

#																			#
#############################################################################



