#############################################################################
#                                                                           #
#           Copyright 1996-2001 by Microware Systems Corporation            #
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
# 02 96/08/27 Added support for ARM, SH, and MIPS.                    gdb   #
# 03 96/08/28 Changed pmips to mips.                                  gdb   #
# 04 96/10/15 Changed parm to arm.  Added arm3 and arm4.              gdb   #
# 05 96/10/15 Changed psh to sh.                                      gdb   #
#             ---- OS-9000/PPC V2.1.1 Released ----                         #
#             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   #
# 06 96/12/11 Changed mips to mip3, added mip4                        sep   #
# 07 97/04/01 Made three MIPS targets: mips, mip3 and mip4.           cdg   #
#             ---- OS-9000/ARMv3 V2.2 Released ----                         #
# 08 97/04/28 Added SPARC support.                                    gdb   #
#             ---- OS-9000/ARMv3 V2.2.1 Released ----                       #  
# 09 97/12/04 Added EOPTS.                                            ljs   #
#             ---- MBX_BSP Released ----                                    #
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- Makefile Support Sub-component v1.1 Released ----        #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
#             ---- Makefile Support Sub-component v1.1.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2 Released ----        #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2.2 Released ----      #
# 10 98/10/29 Added include of baserule.tpl and trgtrule.tpl (removed from  #
#              trgtxxx.tpl files.  Reordered includes.                mgh   #
#              Rework recursive calls for V1.3   Compatibility with earlier #
#              subdirectory/intermediate makefiles may occur.               #
# 11 98/11/09 Removed SUBTRGT =  this file, set SUBTRGT=_Build in     mgh   #
#              trgtbase so recursion starts correctly.                      #
#             ---- Makefile Support Sub-component v1.3   Released ----      #
# 12 98/12/02 Added define of default MAKENAME back into file for now. mgh  #
# 13 98/12/16 Moved -b,-bo, MAKEOPTS and MAKEIT here from host files   mgh  #
# 14 99/03/01 Added support for SH4 processor.                         dwj  #
# 15 99/10/06 Added support for sprc and sprclite.                     rmf  #
# 16 99/12/14 Added 8240 (PowerPC) support.                            djc  #
# 17 00/01/26 Added 8260 (PowerPC) support.                            djc  #
# 18 00/08/09 Added mips32 support.									   dwj	#
# 19 01/03/05 Added armv5 support.                                     rry  #
# 20 01/06/14 Added SH-5m support.									   rry  #
# 21 01/11/28 Added PowerPC 405 and 555 support.                       mdu  #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
#	MWOS make template file for system-wide maro definitions.           #
#############################################################################

#############################################################################
#	[MWOS] must be predefined to the location of the MWOS directory, by #
#	either full or partial pathlist.                                    #
#############################################################################
#	[TRGTS] must be predefined to the default target(s) which should be #
#	made by this makefile.                                              #
#############################################################################
#	[ALL_TRGTS] must be predefined to the target dependancies for the   #
#	"make all.all" command.                                             #
#############################################################################
#	The following targets must be defined in the makefile:              #
#		_clean:                                                     #
#		_purge:                                                     #
#############################################################################
#   All rules are defined in 'defrule.tpl' and 'trgtrule.tpl'.              #
#############################################################################

#############################################################################
# Require options for Microware's MAKE/OS9Make go here to keep them out of  #
#   every host file. (compatibility with oskport.tpl.                       #
# NOTE: These templates require (os9)make.                                  #
#############################################################################
# Turn off default rules
-b
-bo

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

#
# All targets definition.
#
include $(MWOS)/MAKETMPL/alltrgts.tpl

#############################################################################
# LEAVE THE FOLLOWING MACROS ALONE!!!					    #
#############################################################################
# Default Options
TARGET			=	base
MOPTS			=	
AOPTS			=	
COPTS			=	
EOPTS			=   
LOPTS			=	
MAKENAME		=	makefile
DOCMD 			=	
TOOLSET			=	$(MWOS)
SUBTRGT			= 

#
# Get Host Specific Definitions
#
include $(MWOS)/MAKETMPL/host$(HOST).tpl

#
# Get Target Specific Definitions
# If no target name is specified, trgtbase.tpl will get for all targets.
#
include $(MWOS)/MAKETMPL/trgt$(TARGET).tpl

#
# Setup rules common to all hosts/Targets (not set in hostxxx.tpl or tfgtxxx.tpl)
# Note: Some of these macros depend on macros in the host/target templates.
#
MAKEOPTS      		=	$(MOPTS) "MOPTS=$(MOPTS)" "AOPTS=$(AOPTS)" \
				"COPTS=$(COPTS)" "EOPTS=$(EOPTS)" "LOPTS=$(LOPTS)" \
                 		HOST=$(HOST) "DOCMD=$(DOCMD)" 
MAKEIT			=	$(MAKE) $(MAKEOPTS) $(FMAKENAME)
PASSOPTS		= 	TARGET=$(TARGET) SUBTRGT=$(SUBTRGT) $(SUBTRGT)
MAKESUB			=	$(MAKE) $(MAKEOPTS) $(PASSOPTS)

#
# Get Installed Target Definitions
#
include $(MWOS)/MAKETMPL/instrgts.tpl

#
# Default uninstalled MAKE target definitions
#
MAKE.k68k		=	$(UNINSTALLED) k68k ; $(SHELL_COMMENT)
MAKE.kc32		=	$(UNINSTALLED) kc32 ; $(SHELL_COMMENT)
MAKE.k020		=	$(UNINSTALLED) k020 ; $(SHELL_COMMENT)
MAKE.k040		=	$(UNINSTALLED) k040 ; $(SHELL_COMMENT)
MAKE.k060		=	$(UNINSTALLED) k060 ; $(SHELL_COMMENT)
MAKE.o386		=	$(UNINSTALLED) o386 ; $(SHELL_COMMENT)
MAKE.p386		=	$(UNINSTALLED) p386 ; $(SHELL_COMMENT)
MAKE.p020		=	$(UNINSTALLED) p020 ; $(SHELL_COMMENT)
MAKE.pppc		=	$(UNINSTALLED) pppc ; $(SHELL_COMMENT)
MAKE.p403		=	$(UNINSTALLED) p403 ; $(SHELL_COMMENT)
MAKE.p405		=	$(UNINSTALLED) p405 ; $(SHELL_COMMENT)
MAKE.p505		=	$(UNINSTALLED) p505 ; $(SHELL_COMMENT)
MAKE.p555		=	$(UNINSTALLED) p555 ; $(SHELL_COMMENT)
MAKE.p601		=	$(UNINSTALLED) p601 ; $(SHELL_COMMENT)
MAKE.p602		=	$(UNINSTALLED) p602 ; $(SHELL_COMMENT)
MAKE.p603		=	$(UNINSTALLED) p603 ; $(SHELL_COMMENT)
MAKE.p604		=	$(UNINSTALLED) p604 ; $(SHELL_COMMENT)
MAKE.p821		=	$(UNINSTALLED) p821 ; $(SHELL_COMMENT)
MAKE.8240		=	$(UNINSTALLED) 8240 ; $(SHELL_COMMENT)
MAKE.8260		=	$(UNINSTALLED) 8260 ; $(SHELL_COMMENT)
MAKE.arm		=	$(UNINSTALLED) arm  ; $(SHELL_COMMENT)
MAKE.arm3		=	$(UNINSTALLED) arm3 ; $(SHELL_COMMENT)
MAKE.arm4		=	$(UNINSTALLED) arm4 ; $(SHELL_COMMENT)
MAKE.arm5		=	$(UNINSTALLED) arm5 ; $(SHELL_COMMENT)
MAKE.sh			=	$(UNINSTALLED) sh   ; $(SHELL_COMMENT)
MAKE.sh4		=	$(UNINSTALLED) sh4  ; $(SHELL_COMMENT)
MAKE.mips		=	$(UNINSTALLED) mips ; $(SHELL_COMMENT)
MAKE.mip3		=	$(UNINSTALLED) mip3 ; $(SHELL_COMMENT)
MAKE.mip32		=	$(UNINSTALLED) mip32 ; $(SHELL_COMMENT)
MAKE.mip4		=	$(UNINSTALLED) mip4 ; $(SHELL_COMMENT)
MAKE.sprc		=	$(UNINSTALLED) sprc ; $(SHELL_COMMENT)
MAKE.sprc8		=	$(UNINSTALLED) sprc8; $(SHELL_COMMENT)
MAKE.sprclite	=	$(UNINSTALLED) sprclite ; $(SHELL_COMMENT)
MAKE.sh5m		=	$(UNINSTALLED) sh5m  ; $(SHELL_COMMENT)

#
# Get target independed rules.
#
include $(MWOS)/MAKETMPL/baserule.tpl

#
# Get target depended rules.
#
if $(TARGET) != "base"
include $(MWOS)/MAKETMPL/trgtrule.tpl
endif

#																			#
#############################################################################
