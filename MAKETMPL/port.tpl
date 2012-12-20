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
# #  Date     Comments                                                 By   #
# -- -------- -------------------------------------------------------- ---- #
# 01 96/08/03 Added edition history.                                   rak  #
#             ---- OS-9000/x86 V2.1 Released ----                           #
#             ---- OS-9000/PPC V2.1.1 Released ----                         #
#             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   #
#             ---- OS-9000/ARMv3 V2.2 Released ----                         #
#             ---- OS-9000/ARMv3 V2.2.1 Released ----                       #
# 02 98/01/28 Added IO_DDIR, IO_DEFS definitions.                      ats  #
#             ---- MBX_BSP Released ----                                    #
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- Makefile Support Sub-component v1.1 Released ----        #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
#             ---- Makefile Support Sub-component v1.1.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2 Released ----        #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2.2 Released ----      #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
# template for all port definitions                                         #
#############################################################################
#                                                                           #

COMMON_ROM      =   $(MWOS)/SRC/ROM

include $(MWOS)/MAKETMPL/cmnrom.tpl

HW_DDIR         =   $(MWOS_DDIR)/HW
HW_DEFS         =   -v=$(HW_DDIR)

IO_DDIR         =   $(MWOS_DDIR)/IO
IO_DEFS         =   -v=$(IO_DDIR)

PORT_CMDDIR     =   $(PORT)/CMDS
PORT_OBJDIR     =   $(PORT_CMDDIR)/BOOTOBJS
PORT_ROMDIR     =   $(PORT_OBJDIR)/ROM

PORT_DEFS       =   -v=$(PORT)

PORT_SYSTYPED   =   $(PORT)/systype.d
PORT_SYSTYPEH   =   $(PORT)/systype.h
PORT_SYSTYPEDES =   $(PORT)/systype.des

OS_SDIR         =   $(MWOS_OSDIR)/SRC
OS_MACSDIR      =   $(OS_SDIR)/MACROS
OS_ROMSDIR      =   $(OS_SDIR)/ROM

OS_MACROS       =   -v=$(OS_MACSDIR)

#                                                                           #
# End of port.tpl                                                           #
#############################################################################
