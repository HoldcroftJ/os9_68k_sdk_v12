# Makefile
############################################################################
#           Copyright 1989-1998 by Microware Systems Corporation           #
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
#  01 ??/??/?? Created                                                ???
#  02 01/15/99 Added automatic directory creation and checkout        rds
############################################################################

MWOS		=	../../../../..
DPIO		=	$(MWOS)/SRC/DPIO
TRGTS		=	ppstat.il ppstat.l
MAKENAME	=	makefile
EDITION		=	2

include $(MWOS)/MAKETMPL/make.com
include $(DPIO)/MAKETMPL/dpio.tpl

IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
ODIR		=	$(MWOS_LIBDIR)
OIDIR		=	$(MWOS_ILIBDIR)
OPTMZ		=	-o=7 -t=1 -s=1 -ao=sc

RELS		=	$(RDIR)/ppstat.r
IRELS		=	$(IDIR)/ppstat.i


DEFS		=	-v=$(MWOS)/SRC/DEFS/MAUI $(DPIO_DEFS) $(MWOS_DEFS)

COMPILE		=	$(CC) $(CPARMS) $(COPT_CODEADDR) $(COPT_LONGCODE) \
				$(COPT_LONGDATA) -r -eio=$(IDIR) $(DEFS) $(OPTMZ) $(COPTS)

ILINK		=	$(CC) $(CPARMS) $(COPT_CODEADDR) $(COPT_LONGCODE) \
				$(COPT_LONGDATA) -r -eas=$(RDIR) $(DEFS) $(OPTMZ) $(COPTS)

ASSEMBL		=	$(ASM) $(APARMS) $(COPT_CODEADDR) $(COPT_LONGCODE) \
				$(COPT_LONGDATA) -r -eas=$(RDIR) $(DEFS) $(OPTMZ) $(AOPTS)

MKLIB		=	$(LIBGEN) -e=$(EDITION)

MKILIB		=	$(CC) $(CPARMS) $(COPT_CODEADDR) $(COPT_LONGCODE) \
				$(COPT_LONGDATA) -ilm -eil $(COPTS)

ppstat.il: $(IDIR) $(RDIR) $(OIDIR)/ppstat.il
	$(COMMENT)
ppstat.l: $(IDIR) $(RDIR) $(ODIR)/ppstat.l
	$(COMMENT)

$(IDIR): nulltrg
	-$(MAKDIR) $@
$(RDIR): nulltrg
	-$(MAKDIR) $@

$(OIDIR)/ppstat.il:  $(IRELS)
	-$(CODO)  $(OIDIR)/ppstat.il
	-$(ATTRW) $(OIDIR)/ppstat.il
	-$(DEL)   $(OIDIR)/ppstat.il
	$(MKILIB) $(IRELS) -fd=$(OIDIR)/ppstat.il
	$(ATTRR1) $(OIDIR)/ppstat.il

$(ODIR)/ppstat.l:  $(RELS)
	-$(CODO)  $(ODIR)/ppstat.l
	-$(ATTRW) $(ODIR)/ppstat.l
	-$(DEL)   $(ODIR)/ppstat.l
	$(MKLIB)  $(RELS) -o=$(ODIR)/ppstat.l
	$(ATTRR1) $(ODIR)/ppstat.l

$(RDIR)/ppstat.r:  ppstat.c
	$(ILINK) ppstat.c

$(IDIR)/ppstat.i:  ppstat.c
	$(COMPILE) ppstat.c

_clean:  nulltrg
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  nulltrg
	-$(CODO)  $(ODIR)/ppstat.l
	-$(ATTRW) $(ODIR)/ppstat.l
	-$(DEL)   $(ODIR)/ppstat.l
	-$(CODO)  $(OIDIR)/ppstat.il
	-$(ATTRW) $(OIDIR)/ppstat.il
	-$(DEL)   $(OIDIR)/ppstat.il

nulltrg:  .
	$(COMMENT)

#                                                                           #
#############################################################################
