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
#  02 12/02/98 Added history, CODO, Makdirs, remove ALL_TRGTS,etc     mgh
#  03 01/25/99 Added spfbase.tpl to get $(SPTIMERS)                   rds
############################################################################
# This makefile will make the SPF LoopBack Device Driver.                  #
############################################################################
#                                                                          #

MWOS		=	../../../../..
DPIO		=	$(MWOS)/SRC/DPIO
SPF			=	$(DPIO)/SPF
TRGTS		=	sploop
MAKENAME	=	spfdrvr.mak

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/spfbase.tpl
include $(DPIO)/MAKETMPL/dpio.tpl

ODIR		=	$(MWOS_ODIR)/BOOTOBJS/SPF
DRDIR		=	$(MWOS)/SRC/DPIO/SPF/DRVR
SDIR		=	$(DRDIR)/SPLOOP

DEBUG		=

#DEFINES	=	-d_FMTIMER
DEFINES		=

RDIR		=	RELS/SPDRVR/$(MWOS_LDIR)
IDIR		=	$(RDIR)

DEFS		=	-v=$(DRDIR) -v=$(SPF)/DEFS $(DPIO_DEFS)

LIBS		=	$(SPTIMERS) $(MBUFLIBS) $(CONV_LIBS) $(CPULIB) $(MWOS_LIBS)

CFILES		=	$(SDIR)/main.c $(SDIR)/entry.c $(SDIR)/../misc.c
RFILES		=	$(DPIO_DRVSTART) $(RDIR)/main.r $(RDIR)/drvr.r
IFILES		=	$(IDIR)/entry.i $(IDIR)/misc.i

OPTMZ		=	-o=7 -t=1 -s=1 -iom -ao=sc

COMPILE		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
				$(DEBUG) $(COPT_CODEADDR) $(COPTS)
ASSEMBL		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
				$(DEBUG)  $(COPT_CODEADDR) $(COPTS)
LINK		=	$(LINKER) $(LPARMS) $(COPT_CODEADDR) -k -r $(OPTMZ) \
				$(DEBUG) $(LIBS) $(LOPTS) $(COPT_GU0)

sploop:  $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/sploop
	$(COMMENT)

$(IDIR): nulltrg
	-$(MAKDIR) $@
$(RDIR): nulltrg
	-$(MAKDIR) $@
$(ODIR)/STB: nulltrg
	-$(MAKDIR) $@

$(ODIR)/sploop:  $(RFILES)
	-$(CODO) $(ODIR)/sploop
	-$(DEL)  $(ODIR)/sploop
	-$(CODO) $(ODIR)/STB/sploop.stb
	-$(DEL)  $(ODIR)/STB/sploop.stb
	-$(DEL)  $(ODIR)/STB/sploop.dbg
	$(LINK)  $(RFILES) -fd=$(ODIR)/sploop -oln=sploop
	$(CHTYPE_DRVR) $(ODIR)/sploop
	$(ATTRE0) $(ODIR)/sploop

$(RDIR)/drvr.r:  $(IFILES)
	$(ASSEMBL) $(IFILES) -fd=drvr.r

$(RDIR)/main.r:  $(SDIR)/main.c  $(SDIR)/defs.h $(SDIR)/history.h $(SDIR)/proto.h
	$(ASSEMBL) $(SDIR)/main.c -fd=main.r

$(IDIR)/entry.i:  $(SDIR)/entry.c $(SDIR)/defs.h $(SDIR)/history.h $(SDIR)/proto.h
	$(COMPILE) $(SDIR)/entry.c

$(IDIR)/misc.i:  $(SDIR)/../misc.c $(SDIR)/history.h
	$(COMPILE) $(SDIR)/../misc.c

_makdir:  nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(IDIR)/*.i

_purge:  nulltrg
	-$(CODO) $(ODIR)/sploop
	-$(DEL)  $(ODIR)/sploop
	-$(CODO) $(ODIR)/STB/sploop.stb
	-$(DEL)  $(ODIR)/STB/sploop.stb
	-$(DEL)  $(ODIR)/STB/sploop.dbg

_lint:  nulltrg
	-$(DEL) sploop.lnt
	$(LINT) $(LINTPARMS) $(DEFS) $(DEFINES) $(CFILES) >sploop.lnt

nulltrg:  .
	$(COMMENT)

#                                                                          #
############################################################################
