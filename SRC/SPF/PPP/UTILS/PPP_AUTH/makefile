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
#  02 11/05/98 Add CODO, Add makedirs, Remove ALL_TRGT, add CPU_GU0   mgh
#  03 12/12/00 Removed debug flag from compile.                       djc
############################################################################
# Makefile to produce PPP utility: pppauth
############################################################################

MWOS		=	../../../../..
TRGTS		=	pppauth
MAKENAME	=	./makefile

include $(MWOS)/MAKETMPL/make.com

SDIR		=	.
IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
ODIR		=	$(MWOS_ODIR)

PPPDIR		=	$(MWOS)/SRC/DPIO/SPF/DRVR/SP_PPP
PPPLIBS		=	$(PPPDIR)/LIB/$(TARGET)
PPPCOMMON	=	$(PPPDIR)/COMMON

BRELS		=	$(CSTART) $(RDIR)/pppauth.r
BIRELS		=	$(IDIR)/pppauth.i $(IDIR)/auth_mod.i

DEFS		=	$(MWOS_DEFS) -v=$(MWOS)/SRC/DEFS/UNIX

LIBS		=	$(CONV_LIBS) $(MWOS_CSLLIBS) -l=$(MWOS_LIBDIR)/item.l

OPTMZ		=	-fep -ior -iou #-g

CC_FLG		=	$(COPTS) $(CPARMS) $(DEFS) $(OPTMZ) -efe=$(IDIR)
IL_FLG		=	$(COPTS) $(CPARMS) $(DEFS) $(LIBS) $(OPTMZ) -k -eas=$(RDIR)
LK_FLG		=	$(LOPTS) $(LPARMS) $(LIBS) $(OPTMZ) -k -olS -tp=$(CPUC) $(COPT_GU0)

COMPILE		=	$(CC) $(CC_FLG)
ILINK		=	$(CC) $(IL_FLG)
LINK		=	$(LINKER) $(LK_FLG)

pppauth:  _makdir $(ODIR)/pppauth
	$(COMMENT)

$(ODIR)/pppauth:  $(BRELS)
	-$(CODO) $@
	-$(DEL)  $@
	-$(CODO) $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.dbg
	$(LINK)  $(BRELS) -fd=$@ -oln=pppauth
	$(ATTRE0) $@

$(RDIR)/pppauth.r:  $(BIRELS)
	-$(DEL) $@
	$(ILINK) $(BIRELS) -fd=pppauth.r

$(IDIR)/pppauth.i:  $(SDIR)/pppauth.c $(MAKENAME)
	-$(DEL) $@
	$(COMPILE) $(SDIR)/pppauth.c

$(IDIR)/auth_mod.i:  $(PPPCOMMON)/auth_mod.c $(MAKENAME)
	-$(DEL) $@
	$(COMPILE) $(PPPCOMMON)/auth_mod.c

_makdir:  $(IDIR) $(RDIR) $(ODIR)/STB
$(IDIR) $(RDIR) $(ODIR)/STB:
	-$(MAKDIR) $@

_clean:  .
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  .
	-$(CODO) $(ODIR)/pppauth
	-$(DEL)  $(ODIR)/pppauth
	-$(CODO) $(ODIR)/STB/pppauth.stb
	-$(DEL)  $(ODIR)/STB/pppauth.stb
	-$(DEL)  $(ODIR)/STB/pppauth.dbg

#                                                                           #
#############################################################################
