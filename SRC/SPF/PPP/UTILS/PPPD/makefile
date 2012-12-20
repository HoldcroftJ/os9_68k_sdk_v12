# Makefile
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
############################################################################

MWOS		=	../../../../..
TRGTS		=	pppd
MAKNAME		=	makefile

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl

SDIR		=	.
IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
ODIR		=	$(MWOS_ODIR)

RELS		=	$(CSTART) $(RDIR)/pppd.r
IRELS		=	$(IDIR)/pppd.i 

PPPLIB		=	-l=$(MWOS_LIBDIR)/ppplib.l
PPPLIBS		=	$(PPPLIB) -y=$(MWOS_ILIBDIR)/ppplib.il

DEFS		=	$(BSDDEFS) $(MWOS_DEFS)
LIBS		=	$(PPPLIBS) $(UNIXLIBS) $(CONV_LIBS) $(MWOS_CSLLIBS)

OPTMZ		=	-o=7 -t=1 -s=1


if $(TARGET) == "sprc"
CODEOPTS = $(COPT_LCD)
else
CODEOPTS = 
endif

COMPILE		=	$(CC) $(CPARMS)$(CODEOPTS) $(OPTMZ) -efe=$(IDIR) $(DEFS) $(COPTS)
ILINK		=	$(CC) $(CPARMS)$(CODEOPTS) $(OPTMZ) -eas=$(RDIR) $(DEFS) $(COPTS)
LINK		=	$(LINKER) $(LPARMS) $(OPTMZ) -k -olS $(LIBS) $(LOPTS) $(COPT_GU0)

$(TRGTS): $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/$(TRGTS)
	$(COMMENT)

$(IDIR): nulltrg
	-$(MAKDIR) $@
$(RDIR): nulltrg
	-$(MAKDIR) $@
$(ODIR)/STB: nulltrg
	-$(MAKDIR) $@

$(ODIR)/pppd:  $(RELS) $(MAKENAME)
	-$(CODO) $(ODIR)/pppd
	-$(DEL)  $(ODIR)/pppd
	-$(CODO) $(ODIR)/STB/pppd.stb
	-$(DEL)  $(ODIR)/STB/pppd.stb
	-$(DEL)  $(ODIR)/STB/pppd.dbg
	$(LINK)  $(RELS) -fd=$(ODIR)/pppd -oln=pppd
	$(ATTRE0) $(ODIR)/pppd

$(RDIR)/pppd.r:  $(IRELS) $(MAKENAME)
	-$(DEL) $@
	$(ILINK) $(IRELS) -fd=pppd.r

$(IDIR)/pppd.i:  $(SDIR)/pppd.c $(MAKENAME)
	-$(DEL) $@
	$(COMPILE) $(SDIR)/pppd.c

	
_makdir:  nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  nulltrg
	-$(CODO) $(ODIR)/pppd
	-$(DEL)  $(ODIR)/pppd
	-$(CODO) $(ODIR)/STB/pppd.stb
	-$(DEL)  $(ODIR)/STB/pppd.stb
	-$(DEL)  $(ODIR)/STB/pppd.dbg

nulltrg:  .
	$(COMMENT)

#                                                                           #
#############################################################################
