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
#             ---- Modular Rom Sub-system v1.0 Released ----                #
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
#             ---- Modular Rom v1.1 Sub-component Released ----             #
#             ---- Modular ROM v1.2 Sub-component Released ----             #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
#                                                                           #

MWOS    =   $(COMMON_ROM)/../..

include $(MWOS)/MAKETMPL/makesub.com

#                                                                           #
# End of makesub.com                                                        #
#############################################################################
