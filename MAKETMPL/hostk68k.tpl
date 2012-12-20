#############################################################################
#                                                                           #
#          Copyright 1996-1998 by Microware Systems Corporation             #
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
# 02 97/08/07 Added BUFFSIZE and deleted excess tabs.                  lmcb #
# 03 97/09/08 Added MAKDIR and DELDIR macros.                          lmcb #
# 04 97/11/06 Added CODO macro.                                        rak  #
#             ---- MBX_BSP_Beta Released ----                               #
# 05 97/11/19 Enclosed MAKEOPTS macros in quotes.  MHQco02919          ljs  #
# 06 97/12/04 Added EOPTS to MAKEOPTS.                                 ljs  #
#             ---- MBX_BSP Released ----                                    #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
# 07 98/08/26 Updated MAKDIR macro.                         MHQco04606 ljs  #
#             ---- Makefile Support Sub-component v1.2.2 Released ----      #
# 08 98/10/29 Remove -f from MAKEIT Macro                              mgh  #
#             ---- Makefile Support Sub-component v1.3   Released ----      #
# 09 98/12/01 Add $(FMAKENAME) back into MAKEIT Macro                  mgh  #
# 10 98/12/16 move -b,-bo MAKEIT and MAKEOPTS entries to make.tpl      mgh  #
#             Add CIDO macro                                                #
# 11 99/01/05 Add  DIR,IDENT, COPY, RENAME macros                      mgh  #
# 12 99/04/30 Added TMODE, PAUSEON, PAUSEOFF for kernel makefiles      mgh  #
# 13 99/07/06 Added LIST macro (CF2714)                                ajk  #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
#                                                                           #
#############################################################################
#   make template file for OS-9/68000 compatable hosts and all targets      #
#############################################################################

#############################################################################
#   MWOS must be pre-defined to the location of the MWOS directory by       #
#   either a full or partial pathlist.                                      #
#############################################################################

#############################################################################
#                                                                           #

# Shell Comment Character(s)
SHELL_COMMENT =   *

# Temporary File Directory
TEMP          =   /dd

# Compiler Commands
CC            =   cc
ASM           =   $(CC)
LINKER        =   $(CC)

# Other Utility Commands
ATTRR         =   attr -r
ATTRW         =   attr -w
ATTRE         =   attr -e
ATTRR1        =   attr -rnwneprnpwnpe
ATTRW1        =   attr -rwneprpwnpe
ATTRE1        =   attr -rnweprnpwpe
ATTRR0        =   attr -rnwnenprnpwnpe
ATTRW0        =   attr -rwnenprnpwnpe
ATTRE0        =   attr -rnwenprnpwnpe
BINEX         =   binex
BUFFSIZE      =   128k
CHD           =   chd
COMMENT       =   @echo "*"
CIDO          =   $(COMMENT)
CODO          =   $(COMMENT)
COPY          =   copy
DEL           =   del -f
DELDIR        =   deldir -qf
DIR           =   dir -e
DPIO_CHTYPE   =   chtype -t
DPSPLIT_JOIN  =   DPSplit -j
DPSPLIT_SPLIT =   DPSplit -s
ECHO          =   @echo
EDITMOD       =   EditMod
EXBIN         =   exbin
IDBGEN        =   idbgen
IDENT         =   ident
IPGEN         =   ipgen
IFGEN         =   ifgen -l
RENAME        =   rename
RPCDBGEN      =   rpcdbgen
FIXMOD0       =   fixmod -uo=0.0
FIXMOD1       =   fixmod -uo=1.0
LIBGEN        =   libgen -c
LINT          =   setenv LINTDIR $(MWOS)/FLINT;flint
LIST          =   list
MAKDIR        =   makdir -pq
MAKE          =   make
MERGE         =   merge
PADROM        =   padrom
PICC          =   pic -c
PICX          =   pic -x
RENAME        =   rename
RM_VSECT      =   rm_vsect
TMODE         =   tmode
TOUCH         =   touch

# PAUSE CONTROL
PAUSEON       =   $(TMODE) -w=1 pause         # page-pause ON
PAUSEOFF      =   $(TMODE) -w=1 nopause       # page-pause OFF


# Redirection
REDIR_IN      =   <
REDIR_OUT     =   >

# I-Code Library Host Type
HOSTTYPE      =   HOST1

#
# These items moved to MAKE.TPL  (not host dependent)
#

#MAKEOPTS      =   $(MOPTS) "MOPTS=$(MOPTS)" "AOPTS=$(AOPTS)" "COPTS=$(COPTS)" \
#                  "EOPTS=$(EOPTS)" "LOPTS=$(LOPTS)" HOST=$(HOST) \
#                  "DOCMD=$(DOCMD)"
#MAKEIT        =   $(MAKE) $(MAKEOPTS) $(FMAKENAME)
# Turn off default rules
#-b
#-bo

#                                                                           #
# End of hostk68k.tpl                                                       #
#############################################################################
