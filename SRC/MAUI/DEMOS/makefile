#############################################################################
#           Copyright 1989-2001 by Microware Systems Corporation            #
#                           All Rights Reserved                             #
#                         Reproduced Under License                          #
#                                                                           #
# This software is confidential property of Microware Systems Corporation,  #
# and is provided under license for internal development purposes only.     #
# Reproduction, publication, distribution, or creation of derivative works  #
# in any form to any party other than the licensee is strictly prohibited,  #
# unless expressly authorized in writing by Microware Systems Corporation.  #
#                                                                           #
#############################################################################
# Change TRGTS to match names of sub-directories to be called               #
#############################################################################
#                                                                           #

MWOS		=	../../..
DIRS		=	\
				LIBSRC \
				ALOHA \
				CDBVAL \
				FCOPY \
				FDRAW \
				GXDEVCAP \
				HELLO \
				INP \
				JVIEW \
				MSG \
				MSGINFO \
				SFONT \
				SIMG \
				SND \
				WINDRAW \
				WININK \
				WINMGR \
				THREAD

TRGTS =
for DIR in $(DIRS)
if exists ($(DIR)/makefile)
TRGTS += $(DIR)
endif
endfor

include $(MWOS)/MAKETMPL/makesub.com

$(TRGTS): .
	-$(CHD) $@ ; $(MAKESUB)

#                                                                           #
#############################################################################
