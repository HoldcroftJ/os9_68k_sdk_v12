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
# #   Date     Comments                                                By   #
# --- -------- ------------------------------------------------------- ---  #
#  01 96/10/24 Added edition history.                                  rak  #
#              ---- OS-9/68K V3.0.3 System-state Debug Support Release ---  #
#              ---- Makefile Support Sub-component v1.2 Released ----       #
#              ---- Makefile Support Sub-component v1.2.1 Released ----     #
#              ---- Makefile Support Sub-component v1.2.2 Released ----     #
#              $$                    <RELEASE_INFO>                     $$  #
#############################################################################
#                                                                           # 

# Include file for port makefiles.

include $(PORT)/../cpu.tpl

MWOS	=	$(PORT)/../../../..

include $(MWOS)/MAKETMPL/port.tpl

