#############################################################################
#                                                                           #
#           Copyright 1996-1998 by Microware Systems Corporation            #
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
# 01 98/12/16 Cloned from make.tpl to simplify multi hosting of 68xxx mgh   #
#               ports and makefile.                                         #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
#	MWOS make template file for OS9/68xxx system-wide maro definitions.  	#
#   NOTE: This is a very specialized template for use with the original OSK #
#          makefile after slight modification to allow being run on hosts   #
#          other than the original resident host.  This template pull in    #
#          ONLY the host dependant tools and NOT the MWOS definitions or    #
#          the multi-host/multi-target funtionality of the standard         #
#          make template system.                                            #
#   NOTE: -b and -bo are NOT defined as some original OSK makefiles depend  #
#          on the internal rules of Microware's (os9)make                   #
#############################################################################
#	[MWOS] must be predefined to the location of the MWOS directory, by 	#
#	either full or partial pathlist.				    					#
#############################################################################
#	The following targets must be defined in the makefile:		    		#
#       _mkdir:																#
#		_clean:							    								#
#		_purge:							    								#
#############################################################################

#############################################################################
# Conditional calls and universal macros defined here                       #
#############################################################################
# 
# This conditional code allows the makes to be called with -f $(MAKENAME) if   
#   $(MAKENAME) was indeed defined.  os9make will default to a name of makefile
# This is done rather than depend on os9make using the last -f=xxx on the
#   command line, which is needed when makefile want to specify a file to use.
#
# NOTE: We define MAKENAME below AFTER this code to allow makefiles that have
#        dependencies on MAKENAME (without defining it) to continue to work
#        for the moment.  Makefiles that want to specify dependencies on
#        MAKENAME should define it locally!
#
if defined(MAKENAME)
FMAKENAME       =       -f $(MAKENAME)
else
FMAKENAME       =
endif

#
# Current Host Definition
#
include $(MWOS)/MAKETMPL/currhost.tpl

#############################################################################
# LEAVE THE FOLLOWING MACROS ALONE!!!					    #
#############################################################################
# Default Options
MOPTS			=	
AOPTS			=	
COPTS			=	
EOPTS			=   
LOPTS			=	
#MAKENAME		=	makefile
DOCMD 			=	
TOOLSET			=	$(MWOS)
SUBTRGT			= 

#
# Get Host Specific Definitions
#
include $(MWOS)/MAKETMPL/host$(HOST).tpl


#
# Setup rules common to all hosts/Targets (not set in hostxxx.tpl or tfgtxxx.tpl)
#
MAKEOPTS		=
#MAKEOPTS		=   $(MOPTS) "MOPTS=$(MOPTS)" "AOPTS=$(AOPTS)" "COPTS=$(COPTS)" \
#					"EOPTS=$(EOPTS)" "LOPTS=$(LOPTS)" HOST=$(HOST) \
#					"DOCMD=$(DOCMD)"
PASSOPTS		=
#PASSOPTS		= 	TARGET=$(TARGET) SUBTRGT=$(SUBTRGT) $(SUBTRGT)
#PASSOPTS		= 	TARGET=$(TARGET) SUBTRGT=$(SUBTRGT) $(SUBTRGT)
#MAKEIT			=   $(MAKE) $(MAKEOPTS) $(FMAKENAME)
MAKESUB			=	$(MAKE) $(MAKEOPTS) $(PASSOPTS)


#
# Get target depended rules.
#
#if $(TARGET) != "base"
#include $(MWOS)/MAKETMPL/trgtrule.tpl
#endif

#
# Dependencies for OSK compatible makefiles
#
#_nulltrg: .
#	$(COMMENT)

#																			#
#############################################################################
