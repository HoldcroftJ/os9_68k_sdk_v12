#############################################################################
#                                                                           #
#              Copyright 1998 by Microware Systems Corporation              #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
# This software is confidential property of Microware Systems Corporation,  #
# and is provided under license for internal development purposes only.     #
# Reproduction, publication, modification, distribution, or creation of     #
# derivative works in any form to any party other than the Licensee is      #
# strictly prohibited, unless expressly provided for under the terms of a   #
# mutually executed written license agreement for this software between the #
# end-usee and Microware Systems Corporation, or otherwise expressly        #
# authorized in writing by Microware Systems Corporation. Licensee shall    #
# reproduce this copyright notice exactly as provided herein on any copies  #
# of the software that Licensee is authorize do distribute.                 #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date     Comments                                                By   #
# --- -------- ------------------------------------------------------- ---  #
#  01 98/06/26 Created.                                                ats  #
#              ---- Makefile Support Sub-component v1.2 Released ----       #
#              ---- Makefile Support Sub-component v1.2.1 Released ----     #
#              $$                    <RELEASE_INFO>                     $$  #
#############################################################################
#                                                                           # 

# Include file for port makefiles.

include $(PORT)/../cpu.tpl

MWOS	=	$(PORT)/../../../..

include $(MWOS)/MAKETMPL/port.tpl

