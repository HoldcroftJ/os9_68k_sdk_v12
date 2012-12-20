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
#  1  10/21/96 Initial Version                                        billw
#     05/07/97 <***** SPF LAN Comm Pack v3.1 Release **********>
#  2  06/03/97 Removed mkdir CMDS/BOOTOBJS/SPF                        pjw
#     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
#############################################################################

#### Add New Descriptor Names Here ##########################################
#																			#
TRGTS		=	lcp0
#																			#
#############################################################################

MWOS		=	../../../../../../..
ODIR		=	../../../CMDS/BOOTOBJS/SPF

ALL_TRGTS	=	k68k

MAKENAME	=	spfdesc.mak

include $(MWOS)/MAKETMPL/make.com

RDIR		=	RELS/DESC
SDIR		=	$(MWOS)/SRC/DPIO/SPF/DESC
DESCDIR		=	DEFS

include $(SDIR)/desc.tpl

_purgeall:  nulltrg
    -$(DEL) $(ODIR)/$(TRGTS)

#																			#
#############################################################################



