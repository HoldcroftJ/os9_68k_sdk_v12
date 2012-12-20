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
# 02 97/11/06 Added _depend rule.                                           #
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- Makefile Support Sub-component v1.1 Released ----        #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
#             ---- Makefile Support Sub-component v1.1.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2 Released ----        #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2.2 Released ----      #
# 03 98/10/29 Changed targets to depend on TRGTS while changing method of   #
#               recursive calls to work properly for all "actions"    mgh   #
#               Compatibility issues with sudirectory makes may occur.      #
#    98/11/11 Added _makdir common target (Action)                          #
#             ---- Makefile Support Sub-component v1.3   Released ----      #
# 04 98/12/08 Put back MAKEIT calls in this file for compatibility    mgh   #
#               with branching makefile that don't yet use MAKESUB          #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
#	MWOS make template file for common definitions for all subdirectory #
#	(non-product level) makefiles.					    #
#############################################################################

#############################################################################
#	[MWOS] must be predefined to the location of the MWOS directory, by #
#	either full or partial pathlist.				    #
#############################################################################

#############################################################################
#									    #

# Get MWOS Definitions

include $(MWOS)/MAKETMPL/make.tpl

#
# These targets will replace the MAKEIT versions below once all branching
#   makefile have been converted to use MAKESUB.  Using these targets 
#   causes one less recusive call of branching makefiles over the ones below. 
#
# Common Subdirectory Targets

#_makdir: $(TRGTS)
#	$(COMMENT)
#
#_clean:	$(TRGTS)
#	$(COMMENT)
#
#_purge:	$(TRGTS)
#	$(COMMENT)
#
#
#_lint:	$(TRGTS)
#	$(COMMENT)  
#
#
#_depend: $(TRGTS)
#	$(COMMENT)
#
#
#_docmd:  _comsubnulltrg
#	$(MAKE) TARGET=$(TARGET) SUBTRGT=_docmd "DOCMD=$(DOCMD)" $(MAKEOPTS) _all
#
#_comsubnulltrg:  .
#	$(COMMENT)


#
# Compatibiity version until all Branching makefiles are using MAKESUB
#
# Common Subdirectory Targets

_clean:  _comsubnulltrg
	$(MAKEIT) TARGET=$(TARGET) SUBTRGT=_clean _all
	
_purge:  _comsubnulltrg
	$(MAKEIT) TARGET=$(TARGET) SUBTRGT=_purge _all
		
_lint:  _comsubnulltrg
	$(MAKEIT) TARGET=$(TARGET) SUBTRGT=_lint _all
			
_depend:  _comsubnulltrg
	$(MAKEIT) TARGET=$(TARGET) SUBTRGT=_depend _all
			    
_docmd:  _comsubnulltrg
	$(MAKEIT) TARGET=$(TARGET) SUBTRGT=_docmd "DOCMD=$(DOCMD)"  _all
				    
_comsubnulltrg:  .
	$(COMMENT)#								            #
#############################################################################
