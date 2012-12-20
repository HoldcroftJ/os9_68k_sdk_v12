############################################################################
#                                                                          #
#           Copyright 1989-2000 by Microware Systems Corporation           #
#                           All Rights Reserved                            #
#                         Reproduced Under License                         #
#                                                                          #
# This software is confidential property of Microware Systems Corporation, #
# and is provided under license for internal development purposes only.    #
# Reproduction, publication, distribution, or creation of derivative works #
# in any form to any party other than the licensee is strictly prohibited, #
# unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                          #
############################################################################
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
#     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  02 11/05/98 Add CODO, Add makedirs, Remove ALL_TRGT etc            mgh
#  03 12/12/00 Removed debug flag from compile.                       djc
############################################################################
# Makefile to produce PPP utility: chat
############################################################################

MWOS		=	../../../../..
TRGTS		=	chat
MAKENAME	=	./makefile

include $(MWOS)/MAKETMPL/make.com

SDIR		=	.
IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
ODIR		=	$(MWOS_ODIR)

PPPCOMMON	=	$(MWOS)/SRC/DPIO/SPF/DRVR/SP_PPP/COMMON
SPFDEFS		=	$(MWOS)/SRC/DEFS/SPF

HFILES		=	$(SDIR)/chat.h $(SDIR)/chat_glb.h

BRELS		=	$(CSTART) $(RDIR)/chat.r

BIRELS		=	$(IDIR)/chat.i \
				$(IDIR)/chat_dev.i \
				$(IDIR)/chat_utl.i \
				$(IDIR)/scf_if.i

DEFS		=	$(MWOS_DEFS) -v=$(MWOS)/SRC/DEFS/UNIX
LIBS		=	$(CONV_LIBS) $(MWOS_CSLLIBS)
OPTMZ		=	-fep -ior -iou #-g


if $(TARGET) == "sprc"
CODEOPTS = $(COPT_LCD)
else
CODEOPTS = 
endif

CC_FLG		=	$(COPTS) $(CPARMS)$(CODEOPTS) $(DEFS) $(OPTMZ) -efe=$(IDIR)
IL_FLG		=	$(COPTS) $(CPARMS)$(CODEOPTS) $(DEFS) $(LIBS) $(OPTMZ) -k -eas=$(RDIR)
LK_FLG		=	$(LOPTS) $(LPARMS) $(LIBS) $(OPTMZ) -k -olS -tp=$(CPUC) $(COPT_GU0)

COMPILE		=	$(CC) $(CC_FLG)
ILINK		=	$(CC) $(IL_FLG)
LINK		=	$(LINKER) $(LK_FLG)

 
$(TRGTS): _makdir $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/chat:  $(BRELS)
	-$(CODO) $@
	-$(DEL)  $@
	-$(CODO) $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.dbg
	$(LINK)  $(BRELS) -fd=$@ -oln=chat
	$(ATTRE0) $@

$(RDIR)/chat.r:  $(BIRELS)
	-$(DEL) $(RDIR)/chat.r
	$(ILINK) $(BIRELS) -fd=chat.r

$(IDIR)/chat.i:  $(SDIR)/chat.c $(HFILES) $(MAKENAME)
	-$(DEL) $(IDIR)/chat.i
	$(COMPILE) $(SDIR)/chat.c

$(IDIR)/chat_dev.i:  $(SDIR)/chat_dev.c $(HFILES) $(MAKENAME)
	-$(DEL) $(IDIR)/chat_dev.i
	$(COMPILE) $(SDIR)/chat_dev.c

$(IDIR)/chat_utl.i:  $(SDIR)/chat_utl.c $(HFILES) $(MAKENAME)
	-$(DEL) $(IDIR)/chat_utl.i
	$(COMPILE) $(SDIR)/chat_utl.c

$(IDIR)/scf_if.i:  $(PPPCOMMON)/scf_if.c $(SPFDEFS)/ppp_scf.h $(MAKENAME)
	-$(DEL) $(IDIR)/scf_if.i
	$(COMPILE) $(PPPCOMMON)/scf_if.c

_makdir:  $(IDIR) $(RDIR) $(ODIR)/STB
$(IDIR) $(RDIR) $(ODIR)/STB:
	-$(MAKDIR) $@

_clean:  .
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  .
	-$(CODO) $(ODIR)/chat
	-$(DEL)  $(ODIR)/chat
	-$(CODO) $(ODIR)/STB/chat.stb
	-$(DEL)  $(ODIR)/STB/chat.stb
	-$(DEL)  $(ODIR)/STB/chat.dbg

#																			#
#############################################################################
