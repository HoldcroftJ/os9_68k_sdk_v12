# Makefile
############################################################################
#  ID:         %A%
#  Date:       %G%
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
# 01  ??/??/?? Created
#     05/05/97 <----------SPF LAN Comm Pack v3.1 Release----------->
# 02  10/21/97 Moved down to SPF/SPQUICC directory from SPF           pjw
#     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
############################################################################
#   This makefile will make the SPF QUICC driver descriptors               #
############################################################################

#### Add New Descriptor Names Here ##########################################
#																			#

TRGTS		=	spqe0

#																			#
#############################################################################

MWOS		=	../../../../../..
ALL_TRGTS	=	kc32
MAKENAME	=	spfdesc.mak
include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl
include $(DPIO)/MAKETMPL/dpio.tpl

RDIR		=	RELS/DESC
ODIR		=	../../CMDS/BOOTOBJS/SPF
SDIR		=	$(MWOS)/SRC/DPIO/SPF/DESC
DESCDIR		=	DEFS
DEFS        =   $(DPIO_DEFS) -v=$(DESCDIR) -v=$(DRVRDIR) $(BSDDEFS) -v=../..

include $(SDIR)/desc.tpl

#																			#
#############################################################################

