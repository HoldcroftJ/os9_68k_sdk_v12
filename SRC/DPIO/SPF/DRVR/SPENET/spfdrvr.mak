# Makefile
############################################################################
#           Copyright 1989-1997 by Microware Systems Corporation           #
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
#  01 ??/??/?? Created
#  02 04/27/97 Converted from ISP to BSD header files                 rds
#              Added p386 as a target                                 rds
#  03 04/27/97 Remove ../pull.c and ../misc.c, add arm3 as a target   rds
#     05/05/97 <----------SPF LAN Comm Pack v3.1 Release----------->
#     05/05/97 <----------SPF LAN Comm Pack v3.1.1 Release--------->
#  04 02/04/98 Changed RELS directory from SPENET to SPDRVR           rds
#              Use new makefile templates                             rds
#  05 02/25/98 Added -ao=sc to work around compiler bug (ddts 3646)   rds
#     04/01/98 <----------SPF LAN Comm Pack v3.2 Release----------->
#  06 11/10/98 Add CODO, Add makedirs, Remove ALL_TRGT, etc.          mgh
#  07 12/01/98 Remove fixmod                                          mgh
#  08 01/24/99 Removed -ao=sc from OPTMZ and -dMWBSD from DEFINES     rds
#  09 11/22/99 Added p2lib.l, improved icode linking                  rds
#  10 12/06/99 Removed include of make.com since cmnrom.tpl does it   rds
#  11 06/23/00 Added _USE_V3_0_PROCDESC flag.                         djl 
#############################################################################

MWOS		=	../../../../..
DPIO		=	$(MWOS)/SRC/DPIO
SPF			=	$(DPIO)/SPF
TRGTS		=	spenet
MAKENAME	=	spfdrvr.mak

include $(MWOS)/MAKETMPL/cmnrom.tpl
include $(MWOS)/MAKETMPL/lancom.tpl
include $(DPIO)/MAKETMPL/dpio.tpl

ODIR		=	$(MWOS_ODIR)/BOOTOBJS/SPF
DRDIR		=	$(MWOS)/SRC/DPIO/SPF/DRVR
SDIR		=	$(DRDIR)/SPENET

DEFINES		=	-d_USE_V3_0_PROCDESC
DEBUG		=

RDIR		=	RELS/SPDRVR/$(MWOS_LDIR)
IDIR		=	$(RDIR)

DEFS		=	$(BSDDEFS) $(DPIO_DEFS) $(SPFDEFS) $(MWOS_DEFS) -v=$(DRDIR)

DEPENDDEFS	=	$(SDIR)/defs.h $(SDIR)/bsddefs.h $(SDIR)/arp.h

LIBS		=	$(LAN_LIBS) $(SPF_LIBS) $(CPULIB) $(MWOS_LIBS)

CFILES		=	$(SDIR)/main.c \
				$(SDIR)/entry.c \
				$(SDIR)/ethernet.c \
				$(SDIR)/arp.c

RFILES		=	$(DPIO_DRVSTART) $(RDIR)/main.r

IFILES		=	$(IDIR)/entry.i \
				$(IDIR)/ethernet.i \
				$(IDIR)/arp.i

OPTMZ		=	-o=7 -t=1 -s=1 -iom

if $(TARGET) == "sprc"
CODEOPTS = $(COPT_LCD)
else
CODEOPTS = 
endif

COMPILE		=	$(CC) $(CPARMS)$(CODEOPTS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
				$(COPT_CODEADDR) $(DEBUG) $(COPTS)
ASSEMBL		=	$(CC) $(CPARMS)$(CODEOPTS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
				$(COPT_CODEADDR) $(DEBUG) $(LIBS) $(COPTS)
LINK		=	$(LINKER) $(LPARMS)$(CODEOPTS) $(COPT_CODEADDR) -k -r $(OPTMZ) \
				$(DEBUG) $(LIBS) $(LOPTS) $(COPT_GU0)

spenet: $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/spenet
	$(COMMENT)

$(IDIR): nulltrg
	-$(MAKDIR) $@
$(RDIR): nulltrg
	-$(MAKDIR) $@
$(ODIR)/STB: nulltrg
	-$(MAKDIR) $@

$(ODIR)/spenet:  $(RFILES) $(IFILES)
	-$(CODO) $(ODIR)/spenet
	-$(DEL)  $(ODIR)/spenet
	-$(CODO) $(ODIR)/STB/spenet.stb
	-$(DEL)  $(ODIR)/STB/spenet.stb
	-$(DEL)  $(ODIR)/STB/spenet.dbg
	$(LINK)  $(RFILES) $(IFILES) -fd=$(ODIR)/spenet -oln=spenet
	$(CHTYPE_DRVR) $(ODIR)/spenet
	$(ATTRE0) $(ODIR)/spenet

$(RDIR)/main.r:  $(SDIR)/main.c $(DEPENDDEFS) $(MAKENAME) $(SDIR)/history.h
	$(ASSEMBL) $(SDIR)/main.c -fd=main.r

$(IDIR)/entry.i:  $(SDIR)/entry.c $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) $(SDIR)/entry.c

$(IDIR)/ethernet.i:	$(SDIR)/ethernet.c $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) $(SDIR)/ethernet.c

$(IDIR)/arp.i: $(SDIR)/arp.c $(SDIR)/arp.h $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) $(SDIR)/arp.c

_makdir:  nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  nulltrg
	-$(CODO) $(ODIR)/spenet
	-$(DEL)  $(ODIR)/spenet
	-$(CODO) $(ODIR)/STB/spenet.stb
	-$(DEL)  $(ODIR)/STB/spenet.stb
	-$(DEL)  $(ODIR)/STB/spenet.dbg

_lint:  nulltrg
	-$(DEL) spenet.lnt
	$(LINT) $(LINTPARMS) $(DEFS) $(DEFINES) $(CFILES) >spenet.lnt

nulltrg:  .
	$(COMMENT)

#                                                                           #
#############################################################################
