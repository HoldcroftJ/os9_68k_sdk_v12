#############################################################################
#                                                                           #
#              Copyright 2001 by Microware Systems Corporation              #
#              Copyright 2001 by RadiSys Corporation                        #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, modification, distribution, or creation of    #
#  derivative works in any form to any party other than the Licensee is     #
#  strictly prohibited, unless expressly provided for under the terms of a  #
#  mutually executed written license agreement for this software between    #
#  the end-user and Microware Systems Corporation, or otherwise expressly   #
#  authorized in writing by Microware Systems Corporation.  Licensee shall  #
#  reproduce this copyright notice exactly as provided herein on any copies #
#  of the software that Licensee is authorized to distribute.               #
#                                                                           #
#  RadiSys is a registered trademark of RadiSys Corporation.                #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date     Comments                                                By   #
# --- -------- ------------------------------------------------------- ---- #
#  01 01/11/29 Created.                                                djc  #
#  02 02/01/04 Conditionalized zebos.mak for specific processors.      djc  #
#############################################################################
# Change DIRS to match names of make files to be called                     #
#############################################################################
#                                                                           #

MWOS	=	../..
DIRS	=	inetdb.mak zebos.mak

TRGTS =
for DIR in $(DIRS)
if exists( $(DIR) )
TRGTS += $(DIR)
endif
endfor

include $(MWOS)/MAKETMPL/makesub.com

inetdb.mak: .
	$(MAKESUB) -f=$@

zebos.mak: .
if $(TARGET) == "pppc" || $(TARGET) == "arm4"
	$(MAKESUB) -f=$@
else
	$(COMMENT)
endif

#                                                                           #
#############################################################################
