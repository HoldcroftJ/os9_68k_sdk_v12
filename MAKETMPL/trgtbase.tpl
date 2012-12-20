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
# 02 98/07/15 Added MWOS_LDIR to allow MWOS_IDIR and MWOW_RDIR to       mgh #
#             differ from $TARGET (Sparc was first time problem appeared)   #
#             ---- Makefile Support Sub-component v1.2 Released ----        #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2.2 Released ----      #
# 03 98/10/29 added OSDIR, COPTs for module owner, attributes, and      mgh #
#             moved includes for baserule.tpl and trgtrule.tpl              #
#             ---- Makefile Support Sub-component v1.3   Released ----      #  
# 04 98/11/09 added SUBTRGT = _build to help start proper recursion     mgh #
#             added values for MWOS_IDIR, MWOS_RDIR, and MWOS_ODIR see NOTE #
# 05 98/11/23 Remove STBTRGT=_build because of problem with clean.xxxx  mgh #
# 08 00/01/24 Removed ISP macros                                        rds #
# 09 00/09/11 Added CPUDIR.                                             djc #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
#	make template file for all hosts all targets 						    #
#   This file must be refered from make.tpl file when no target name is     #
#   specified.                                                              #
#############################################################################

#############################################################################
#	MWOS must be pre-defined to the location of the MWOS directory by 		#
#	either a full or partial pathlist.										#
#############################################################################
#	HOSTTYPE must be pre-defined to the I-Code Library host type			#
#############################################################################

#############################################################################
#																			#

#
# NOTE: MWOS_IDIR, MWOS_RDIR, and MWOS_ODIR are defined with place holder 
#        values so the first recursion level will find values on the left side
#        of dependencies for IDIR, RDIR, and ODIR in some makefiles.
#        IDIR, RDIR, ODIR (eg IDIR=$(MWOS_IDIR)) are used for makdirs. 
#        MWOS_LDIR is also set to a temp value.
#       This is only for calling makefiles without targets specified for the
#        working/leaf makefiles.

# Directory Definitions
MWOS_LDIR       	= xxxx
MWOS_IDIR		= RELS
MWOS_RDIR		= RELS
MWOS_OSDIR		= 
MWOS_ODIR		= CMDS

MWOS_DDIR		=
MWOS_CDEF		=

MWOS_DFTDEF		=
MWOS_OSDDIR		=

MWOS_DFTLIB		=
MWOS_DFTCLIB	=

MWOS_BASLIB		=
MWOS_BASCLIB	=

MWOS_LIBDIR		=
MWOS_CLIBDIR	=

MWOS_ILIBDIR	=
MWOS_ICLIBDIR	=

EDITMOD_DDIR	=
EDITMOD_ODIR	=

# Definition Directory Search
MWOS_DEFS		=
EDITMOD_DEFS	=

# Host/Target Macros
OS				=
OSDIR			=
CPU				=
CPUDIR			=
CPUC			=

# Library Files
CLIB			=
CLIBS			=
CSL				=
CSLS			=
OS_CSL			=
OS_CSLS			=
OS_LIB			=
OS_LIBS			=
SCLIB			=
SCLIBS			=
SYS_CLIB		=
SYS_CLIBS		=
SYS_CSL			=
SYS_CSLS		=
TERMLIB			=
TERMLIBS		=

SYS				=
USR				=

MWOS_LIBS		=
MWOS_CSLLIBS	=

MBUFLIB			=
MBUFLIBS		=

CONV_LIB		=
CONV_LIBS		=
CPULIB			=
CPULIBS			=
LOCKLIB			=
LOCKLIBS		=

ISDNLIB			=
ISDNLIBS		=
TESTSUITE		=
TESTSUITES		=
TIMERLIB		=
TIMERLIBS		=
X25LIB			=
X25LIBS			=

CDILIB			=
CDILIBS			=

UNIXLIB			=
UNIXLIBS		=

# Root PSects
CSTART			=
SYSCSTART		=
DESCSTART		=
DRVSTART		=
FMSTART			=
SUBSTART		=
SYSTART			=

# Default Compile/Assemble/Link Options
CPARMS			=
CPARMSNC		=
APARMS			=
LPARMS			=

COPT_SC         =
COPT_LC         =
COPT_SD         =
COPT_LD         =
COPT_SCD        =
COPT_LCD        =

COPT_CODEADDR	=
COPT_LONGDATA	=
COPT_LONGCODE	=

COPT_JMPTBL	=
COPT_GU0	=
COPT_GU1	=
COPT_AM0	=
COPT_AM1	=
COPT_AF0	=
COPT_AF1	=

# Link Options
LINTPARMS		=

#																			#
#############################################################################
