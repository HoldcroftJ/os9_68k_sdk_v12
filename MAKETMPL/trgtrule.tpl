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
# 04 98/10/29 minor changes                                            mgh  #
#             ---- Makefile Support Sub-component v1.3   Released ----      #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
# Target specific rules.													#
#############################################################################
#																			#
VCS_VERSION = 

docmd:  _nulltrg
	$(MAKEIT) TARGET=$(TARGET) _docmd

_vcs_check:  _nulltrg
	$(MAKE) TARGET=$(TARGET) "DOCMD=pvlog -bl VCS/*" $(MAKEOPTS) _docmd
_vcs_log:  _nulltrg
	$(MAKE) TARGET=$(TARGET) "DOCMD=pvlog -bn VCS/*" $(MAKEOPTS) _docmd
_vcs_version:  _nulltrg
	$(MAKE) TARGET=$(TARGET) "DOCMD=pvcs -v$(VCS_VERSION) VCS/*" $(MAKEOPTS) \
	_docmd

# Specified Targets
_trgts:  $(TRGTS)
	$(COMMENT)

#																			#
#############################################################################

