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
# #   Date    Comments                                                 By   #
# -- -------- -------------------------------------------------------- ---- #
# 01 96/08/03 Added edition history.                                   rak  #
#             ---- OS-9000/x86 V2.1 Released ----                           #
#             ---- OS-9000/PPC V2.1.1 Released ----                         #
#             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   #
#             ---- OS-9000/ARMv3 V2.2 Released ----                         #
# 02 97/08/07 Added BUFFSIZE and deleted excess tabs.                  lmcb #
# 03 97/09/08 Added MAKDIR and DELDIR macros.                          lmcb #
# 04 97/11/06 Added CODO macro.                                        rak  #
#             ---- MBX_BSP_Beta Released ----                               #
# 05 97/11/19 Enclosed MAKEOPTS macros in quotes.  MHQco02919          ljs  #
# 06 97/12/04 Added EOPTS to MAKEOPTS.                                 ljs  #
#             ---- MBX_BSP Released ----                                    #
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- Makefile Support Sub-component v1.1 Released ----        #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
# 07 98/06/24 Removed overrides for SYSANSI_CSTART and ANSI_CSTART.    ats  #
#             ** This change requires the use of UCC v2.1 or newer. **      #
#             ---- Makefile Support Sub-component v1.1.1 Released ----      #
# 08 98/07/15 Changed to allow easier overriding of attribute commands.gdb  #
#             ---- Makefile Support Sub-component v1.2 Released ----        #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
# 1008 98/08/24 Added os9makdir per MAUI request.                      GbG  #
# 09 98/10/29 Remove -f from MAKEIT Macro                              mgh  #
#             ---- Makefile Support Sub-component v1.3   Released ----      #
# 10 98/12/01 Add $(FMAKENAME) back into MAKEIT Macro                  mgh  #
# 11 98/12/09 Add /f option to force dels of readonly files            mgh  #
# 12 99/01/05 Add DIR, IDENT, COPY, and RENAME Macros                  mgh  #
# 13 99/01/11 Move MAKEOPTS, MAKEIT, -b, -bo to make.tpl file          mgh  #
#             REMOVE -mode=ucc from CC, ASM, LINKER macros                  #
# 14 99/03/22 Added LIST, MOVE, and MORE cmds                          mgh  #
# 15 99/03/24 RENAME MACRO was empty ... define to rename              mgh  #
# 16 99/03/29 Change DEL to use os9del. This corrects problems with         #
#             inconsistent del commands in the various WIN OS versions srm  #
# 17 99/04/30 Added TMODE, PAUSEON, PAUSEOFF for kernel makefiles      mgh  #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
#                                                                           #
#############################################################################
#   make template file for MS-DOS hosts and all targets                     #
#############################################################################

#############################################################################
#   MWOS must be pre-defined to the location of the MWOS directory by       #
#   either a full or partial pathlist.                                      #
#############################################################################

#############################################################################
#                                                                           #

# Shell Comment Character(s)
SHELL_COMMENT  =   REM

# Temporary File Directory
TEMP           =   .

# Compiler Commands
#CC             =   xcc -mode=ucc
#ASM            =   xcc -mode=ucc
#LINKER         =   xcc -mode=ucc
CC             =   xcc
ASM            =   xcc
LINKER         =   xcc

# Other Utility Commands
BINEX          =   binex
BUFFSIZE       =   128k
CHD            =   cd
COMMENT        =   @echo "*"
ATTRR          =   $(COMMENT)  #attrib +r
ATTRW          =   $(COMMENT)  #attrib -r
ATTRE          =   $(COMMENT)  #attrib +r
ATTRR1         =   $(COMMENT)  #attrib +r
ATTRW1         =   $(COMMENT)  #attrib -r
ATTRE1         =   $(COMMENT)  #attrib +r
ATTRR0         =   $(COMMENT)  #attrib +r
ATTRW0         =   $(COMMENT)  #attrib -r
ATTRE0         =   $(COMMENT)  #attrib +r
CODO           =   $(COMMENT)
COPY           =   copy
DEL            =   os9del -qf
DELDIR         =   rmdir /s /q  #win nt
DIR            =   dir
DPIO_CHTYPE    =   chtype -t
DPSPLIT_JOIN   =   DPSplit -j
DPSPLIT_SPLIT  =   DPSplit -s
ECHO           =   @echo
EDITMOD        =   EditMod
EXBIN          =   exbin
IDBGEN         =   idbgen
IDENT          =   ident
IPGEN          =   ipgen
IFGEN          =   ifgen -l
LIST           =   type
RPCDBGEN       =   rpcdbgen
FIXMOD0        =   fixmod -uo=0.0
FIXMOD1        =   fixmod -uo=1.0
LIBGEN         =   libgen -c
LINT           =   -$(COMMENT)
MAKDIR         =   os9makdir -pq
MAKE           =   os9make
MERGE          =   os9merge
MORE           =   more
MOVE           =   move
PADROM         =   padrom
PAUSEON        =   $(COMMENT)
PAUSEOFF       =   $(COMMENT)
PICC           =   $(COMMENT)
PICX           =   $(COMMENT)
RENAME         =   rename
RM_VSECT       =   rm_vsect
TMODE          =   $(COMMENT)
TOUCH          =   os9touch

# Redirection
REDIR_IN       =   <
REDIR_OUT      =   >

# I-Code Library Host Type
HOSTTYPE       =   HOST3

#
# These items moved to MAKE.TPL  (not host dependent)
#

# Turn off default rules (OS-9 Make)
#-b
#-bo
#MAKEOPTS      =   $(MOPTS) "MOPTS=$(MOPTS)" "AOPTS=$(AOPTS)" "COPTS=$(COPTS)" \
#                  "EOPTS=$(EOPTS)" "LOPTS=$(LOPTS)" HOST=$(HOST) \
#                  "DOCMD=$(DOCMD)"
#MAKEIT        =   $(MAKE) $(MAKEOPTS) $(FMAKENAME)

#                                                                           #
# End of hostmdos.tpl                                                       #
#############################################################################
