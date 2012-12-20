############################################################################
#                                                                          #
#           Copyright 1989-2001 by Microware Systems Corporation           #
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
#  01 10/21/98 Created                                                rds
#  02 01/20/99 Moved into clearcase, added $(CODO)'s and $(MAKDIR)'s  rds
#  03 01/23/01 Update makdir.                                         djc
############################################################################

MWOS		=	../../../../..
TRGTS		=	msend mrecv
MAKENAME	=	makefile

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl

IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
ODIR		=	$(MWOS_ODIR)

DEFS		=	$(BSDDEFS) $(MWOS_DEFS)
LIBS		=	$(UNIXLIBS) $(LAN_LIBS) $(CONV_LIBS) $(MWOS_CSLLIBS)

OPTMZ		=	-o=7

DEFINES		=

COMPILE		=	$(CC) $(DEFINES) $(CPARMS) $(OPTMZ) -eas=$(RDIR) $(DEFS) $(COPTS)
ILINK		=	$(CC) $(CPARMS) $(OPTMZ) -k -eas=$(RDIR) $(DEFS) $(COPTS)
LINK		=	$(LINKER) $(LPARMS) $(OPTMZ) -tp=$(CPUC) -k -olS $(LIBS) \
				$(LOPTS) $(COPT_GU1)

msend:  _makdir $(ODIR)/msend
	$(COMMENT)

mrecv:  _makdir $(ODIR)/mrecv
	$(COMMENT)

$(ODIR)/msend:  $(RDIR)/msend.r
	-$(CODO) $@
	-$(DEL)  $@
	-$(CODO) $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.stb
	$(LINK)  $(CSTART) $(RDIR)/msend.r -fd=$@ -oln=msend
	$(ATTRE1) $@

$(ODIR)/mrecv:  $(RDIR)/mrecv.r
	-$(CODO) $@
	-$(DEL)  $@
	-$(CODO) $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.stb
	$(LINK)  $(CSTART) $(RDIR)/mrecv.r -fd=$@ -oln=mrecv
	$(ATTRE1) $@

$(RDIR)/msend.r:  msend.c
	$(COMPILE) msend.c

$(RDIR)/mrecv.r:  mrecv.c
	$(COMPILE) mrecv.c

_makdir:  $(IDIR) $(RDIR) $(ODIR)/STB
$(IDIR) $(RDIR) $(ODIR)/STB:
	-$(MAKDIR) $@

_clean:  .
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  .
	-$(CODO) $(ODIR)/msend
	-$(DEL)  $(ODIR)/msend
	-$(CODO) $(ODIR)/mrecv
	-$(DEL)  $(ODIR)/mrecv
	-$(CODO) $(ODIR)/STB/msend.stb
	-$(DEL)  $(ODIR)/STB/msend.stb
	-$(CODO) $(ODIR)/STB/mrecv.stb
	-$(DEL)  $(ODIR)/STB/mrecv.stb

#                                                                           #
#############################################################################
