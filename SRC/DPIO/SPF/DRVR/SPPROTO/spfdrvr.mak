# Makefile
#############################################################################
#           Copyright 1989-1998 by Microware Systems Corporation            #
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
#############################################################################
# This makefile will make the SPF example Protocol Driver.                  #
#############################################################################

MWOS		=	../../../../..
DPIO		=	$(MWOS)/SRC/DPIO
SPF			=	$(DPIO)/SPF
TRGTS		=	spproto
MAKENAME	=	spfdrvr.mak

include $(MWOS)/MAKETMPL/make.com
include $(DPIO)/MAKETMPL/dpio.tpl
include $(MWOS)/MAKETMPL/spfbase.tpl

ODIR		=	$(MWOS_ODIR)/BOOTOBJS/SPF
DRDIR		=	$(MWOS)/SRC/DPIO/SPF/DRVR
SDIR		=	$(DRDIR)/SPPROTO

DEFINES		=

DEBUG		=

RDIR		=	RELS/SPDRVR/$(MWOS_LDIR)
IDIR		=	$(RDIR)

DEFS		=	-v=$(DRDIR) $(DPIO_DEFS)
DEPENDDEFS	=	$(SDIR)/defs.h $(SDIR)/history.h $(SDIR)/proto.h

LIBS		=	$(MBUFLIBS) $(CPULIBS) $(SPTIMER) $(PPSTAT) $(CONV_LIBS) $(MWOS_LIBS)

CFILES		=	$(SDIR)/main.c $(SDIR)/entry.c $(SDIR)/../misc.c
RFILES		=	$(DPIO_DRVSTART) $(RDIR)/main.r $(RDIR)/drvr.r
IFILES		=	$(IDIR)/entry.i $(IDIR)/misc.i

# do not use the -iom option on any file where a function is declared
# which is refered to as a pointer elsewhere. The -iom option causes
# inlined functions to be discarded, regardless of whether they are
# used by pointer elsewhere. This gives an unresolved reference during
# linking.
OPTMZ		=	-o=7 -t=1 -s=1 -iom

COMPILE		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
				$(COPT_CODEADDR) $(DEBUG) $(COPTS)
ASSEMBL		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
				$(COPT_CODEADDR) $(DEBUG) $(COPTS)
LINK		=	$(LINKER) $(LPARMS) $(COPT_CODEADDR) -k -r $(OPTMZ) \
				$(DEBUG) $(LIBS) $(LOPTS) $(COPT_GU0)

spproto:  $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/spproto
	$(COMMENT)

$(IDIR): nulltrg
	-$(MAKDIR) $@

$(RDIR): nulltrg
	-$(MAKDIR) $@

$(ODIR)/STB: nulltrg
	-$(MAKDIR) $@

$(ODIR)/spproto:  $(RFILES)
	-$(CODO) $(ODIR)/spproto
	-$(DEL) $(ODIR)/spproto
	-$(CODO) $(ODIR)/STB/spproto.stb
	-$(DEL) $(ODIR)/STB/spproto.stb
	$(LINK) $(RFILES) -fd=$(ODIR)/spproto -oln=spproto
	$(CHTYPE_DRVR) $(ODIR)/spproto
	$(ATTRE0) $(ODIR)/spproto

$(RDIR)/drvr.r:  $(IFILES)
	$(ASSEMBL) $(IFILES) -fd=drvr.r

$(RDIR)/main.r:  $(SDIR)/main.c $(DEPENDDEFS)
	$(ASSEMBL) $(SDIR)/main.c -fd=main.r

$(IDIR)/entry.i:  $(SDIR)/entry.c $(DEPENDDEFS)
	$(COMPILE) $(SDIR)/entry.c

$(IDIR)/misc.i:  $(SDIR)/../misc.c $(SDIR)/history.h
	$(COMPILE) $(SDIR)/../misc.c

_makdir:  nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/main.r
	-$(DEL) $(RDIR)/drvr.r
	-$(DEL) $(IDIR)/entry.i
	-$(DEL) $(IDIR)/misc.i
	$(COMMENT)

_purge:  nulltrg
	-$(CODO) $(ODIR)/spproto
	-$(DEL) $(ODIR)/spproto
	-$(CODO) $(ODIR)/STB/spproto.stb
	-$(DEL) $(ODIR)/STB/spproto.stb
	$(COMMENT)

_lint:  nulltrg
	-$(DEL) spproto.lnt
	$(LINT) $(LINTPARMS) $(DEFS) $(DEFINES) $(CFILES) >spproto.lnt

nulltrg:  .
	$(COMMENT)
#                                                                           #
#############################################################################
