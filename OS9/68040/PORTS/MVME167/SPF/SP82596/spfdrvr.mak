# Makefile
#############################################################################
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
#  01 ??/??/?? Creation
#  02 02/25/98 Added -ao=sc to work around compiler bug (ddts 3646)   rds
#     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              #
#  03 11/24/98 Add CODO, add makdir, etc                              mgh
#############################################################################
# This makefile will make the SPF 82596 ethernet Driver.                    #
#############################################################################

MWOS		=	../../../../../..
ALL_TRGTS   	=	k020
TRGTS		=	sp167
MAKENAME	=	spfdrvr.mak

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl
include $(DPIO)/MAKETMPL/dpio.tpl

ODIR		=	../../CMDS/BOOTOBJS/SPF
SDIR		=	$(DRVRDIR)/SP82596
PORTDIR		=	$(MWOS)/OS9/68020/PORTS/MVME167

DEFINES		=	-dvme167 -d_MPF68K -d$(TRGTS) -dPVTRAM -dFlushing
DEBUG		=

RDIR		=	RELS/SPDRVR
IDIR		=	$(RDIR)

DEFS		=	-v=$(PORTDIR) -v=$(DRVRDIR) $(DPIO_DEFS) $(BSDDEFS)
DEPENDDEFS	=	$(SDIR)/defs.h $(SDIR)/history.h $(SDIR)/i82596.h \
				$(SDIR)/proto.h $(SDIR)/ie.h

LIBS		=	$(MBUFLIBS) $(CPULIBS) $(CONV_LIBS) $(MWOS_LIBS)

CFILES		=	$(SDIR)/main.c 	$(SDIR)/entry.c  $(SDIR)/../misc.c \
				$(SDIR)/init.c 	$(SDIR)/isr.c \
				$(SDIR)/chip.c 	$(SDIR)/rirq.c $(SDIR)/xirq.c
RFILES		=	$(DPIO_DRVSTART) $(RDIR)/main.r $(RDIR)/drvr.r
IFILES		=	$(IDIR)/entry.i $(IDIR)/misc.i	$(IDIR)/isr.i \
				$(IDIR)/init.i	$(IDIR)/chip.i	$(IDIR)/rirq.i	$(IDIR)/xirq.i

# do not use the -iom option on any file where a function is declared
# which is refered to as a pointer elsewhere.  The -iom option causes
# inlined functions to be discarded, regardless of whether they are
# used by pointer elsewhere.  This gives an unresolved reference during
# linking.
OPTMZ		=	-o=7 -t=1 -s=1 -iom -ao=sc

COMPILE		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
				$(COPT_CODEADDR) $(DEBUG) $(COPTS)
ASSEMBL		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
				$(COPT_CODEADDR) $(DEBUG) $(COPTS)
LINK		=	$(LINKER) $(LPARMS) $(COPT_CODEADDR) -k -r $(OPTMZ) \
				$(DEBUG) $(LIBS) $(LOPTS) $(COPT_GU0)

$(TRGTS):  _makdir $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS):  $(RFILES)
	-$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL)  $(ODIR)/$(TRGTS)
	-$(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL)  $(ODIR)/STB/$(TRGTS).stb
	-$(DEL)  $(ODIR)/STB/$(TRGTS).dbg
	$(LINK)  $(RFILES) -fd=$(ODIR)/$(TRGTS) -oln=$(TRGTS)
	$(CHTYPE_DRVR) $(ODIR)/$(TRGTS)
	$(ATTRE0) $(ODIR)/$(TRGTS)

$(RDIR)/drvr.r: $(IFILES)
	$(ASSEMBL) $(IFILES) -fd=drvr.r

$(RDIR)/main.r:  $(SDIR)/main.c  $(DEPENDDEFS)
	$(ASSEMBL) $(SDIR)/main.c -fd=main.r

$(IDIR)/entry.i:  $(SDIR)/entry.c $(DEPENDDEFS)
	$(COMPILE) $(SDIR)/entry.c

$(IDIR)/chip.i:  $(SDIR)/chip.c $(DEPENDDEFS)
	$(COMPILE) $(SDIR)/chip.c

$(IDIR)/init.i:  $(SDIR)/init.c $(DEPENDDEFS)
	$(COMPILE) $(SDIR)/init.c

$(IDIR)/isr.i:  $(SDIR)/isr.c $(DEPENDDEFS)
	$(COMPILE) $(SDIR)/isr.c

$(IDIR)/rirq.i:  $(SDIR)/rirq.c $(DEPENDDEFS)
	$(COMPILE) $(SDIR)/rirq.c

$(IDIR)/xirq.i:  $(SDIR)/xirq.c $(DEPENDDEFS)
	$(COMPILE) $(SDIR)/xirq.c

$(IDIR)/misc.i:  $(SDIR)/../misc.c $(SDIR)/history.h
	$(COMPILE) $(SDIR)/../misc.c

_makdir:  nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(RDIR)/*.i

_purge:  nulltrg
	-$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL)  $(ODIR)/$(TRGTS)
	-$(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL)  $(ODIR)/STB/$(TRGTS).stb
	-$(DEL)  $(ODIR)/STB/$(TRGTS).dbg

_lint:  nulltrg
	-$(DEL) $(TRGTS).lnt
	$(LINT) $(LINTPARMS) $(DEFS) $(DEFINES) $(CFILES) >sp167.lnt

nulltrg:  .
	$(COMMENT)
#																			#
#############################################################################
