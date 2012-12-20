#############################################################################
#                                                                           #
#              Copyright 2000 by Microware Systems Corporation              #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, modification, distribution, or creation of    #
#  derivative works in any form to any party other than the Licensee is     #
#  strictly prohibited, unless expressly provided for under the terms of a  #
#  mutually executed written license agreement for this software between    #
#  the end-user and Microware Systems Corporation, or otherwise expressly   #
#  authorized in writing by Microware Systems Corporation.  Licensee shall  #
#  reproduce this copyright notice exactly as provided herein on any copies #
#  of the software that Licensee is authorized to distribute.               #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date     Comments                                                By   #
# --- -------- ------------------------------------------------------- ---- #
#  01 00/10/18 Cloned Genesis.                                         djc  #
#              $$                    <RELEASE_INFO>                     $$  #
#############################################################################
#
# Makefile to make ROM Bootp timer for 68360 Naztec 2070L
#
-bo
-mode=compat

MWOS		= ../../../../..

SDIR		=	$(MWOS)/OS9/SRC/ROM/CBOOT/TIMERS
RDIR		=	RELS

LIBDIR		=	LIB

SYSDEFS		=	$(MWOS)/SRC/DEFS
OSDEFS		=	$(MWOS)/OS9/SRC/DEFS
CPUDEFS		=	$(MWOS)/OS9/CPU32/DEFS/CPU32
PORTDEFS	= 	../DEFS
SYSTYPES	=	..

ALLDEFS		=	-v=$(SYSTYPES) -v=$(OSDEFS) -v=$(SYSDEFS) -v=$(PORTDEFS) \
				-v=$(CPUDEFS) 


TICKERNAME	=	bp_tk360

BRFILES		=	$(TICKERNAME).r

TT			=	#-dTIMER_TEST

include		$(MWOS)/MAKETMPL/makeosk.tpl

CFLAGS		=	-mode=compat -qst=$(TEMP) $(ALLDEFS) -dOSK $(TT) -o=0
RFLAGS		=	-q -o=0

-x


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# compile the component files for "sysboot.l" and merge them
# all together
#
$(TICKERNAME).l _build : makdir $(LIBDIR)/$(TICKERNAME).l
	$(COMMENT)

$(LIBDIR)/$(TICKERNAME).l : $(BRFILES)
	-$(CODO) $@
	-$(DEL)  $@
	$(CHD) $(RDIR); $(MERGE) $(BRFILES) $(REDIR_OUT)../$@



# # # # # # # # # # # # # # # # # # # # # # # # #
# This section recompiles the component files if
# the global defs have changed
#
$(BRFILES) : 

makdir _makdir:
	-$(MAKDIR) $(RDIR)
 
clean _clean:
	-$(DEL) $(RDIR)/$(TICKERNAME).r
    
purge _purge:
	-$(CODO) $(LIBDIR)/$(TICKERNAME).l
	-$(DEL)  $(LIBDIR)/$(TICKERNAME).l
    

