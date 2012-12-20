#############################################################################
#                                                                           #
#              Copyright 1996 by Microware Systems Corporation              #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, distribution, or creation of derivative works #
#  in any form to any party other than the licensee is strictly prohibited, #
#  unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date    Comments                                                By    #
# -- -------- ------------------------------------------------------- ---   #
# 01 96/08/03 Added edition history.                                  rak   #
#             ---- OS-9000/x86 V2.1 Released ----                           #
#             ---- OS-9000/PPC V2.1.1 Released ----                         #
#             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   #
#             ---- OS-9000/ARMv3 V2.2 Released ----                         #
#             ---- OS-9000/ARMv3 V2.2.1 Released ----                       #  
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- Makefile Support Sub-component v1.1 Released ----        #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
#             ---- Makefile Support Sub-component v1.1.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2 Released ----        #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2.2 Released ----      #
#             ---- Makefile Support Sub-component v1.3   Released ----      #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
#                                                                           # 
#############################################################################
#	$Workfile:   make.com  $
#	$Revision:   1.17  $
#	$Modtime:   26 Aug 1998 16:07:58  $
#	$Date:   26 Aug 1998 16:07:58  $
#############################################################################
#	MWOS make template file for common definitions for all product level	#
#	makefiles.																#
#############################################################################

#############################################################################
#	[MWOS] must be predefined to the location of the MWOS directory, by		#
#	either full or partial pathlist.										#
#############################################################################

#############################################################################
#																			#

# Get MWOS Definitions
include $(MWOS)/MAKETMPL/make.tpl


# Common Product-Level Targets

_docmd:  _commnulltrg
	$(COMMENT)
	$(DOCMD)
	$(COMMENT)

_comnulltrg:  .
	$(COMMENT)

#																			#
#############################################################################
