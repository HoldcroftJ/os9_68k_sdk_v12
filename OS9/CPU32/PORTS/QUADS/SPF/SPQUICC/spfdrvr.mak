# Makefile
############################################################################
#  ID:         %A%
#  Date:       %G%
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
#  01 ??/??/?? Created
#     05/05/97 <----------SPF LAN Comm Pack v3.1 Release----------->
#  02 10/21/97 Moved down to SPF/SPQUICC directory from SPF           pjw
#  03 02/25/98 Added -ao=sc to work around compiler bug (ddts 3646)   rds
#     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
#  04 11/24/98 Added CODO, added makdir, Add COPT_GU0, etc            mgh
############################################################################
# This makefile will make the SPF QUICC ethernet Driver.                   #
############################################################################
MWOS		=	../../../../../..
ALL_TRGTS	=	kc32
TRGTS		=	sp360
MAKENAME	=	spfdrvr.mak

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl
include $(DPIO)/MAKETMPL/dpio.tpl

ODIR		=	../../CMDS/BOOTOBJS/SPF
SDIR		=	$(DRVRDIR)/SPQUICC

NO_CONFIG_PINS  =       # -dNO_CONFIG_PINS
NO_CONFIG_TENA  =       # -dNO_CONFIG_TENA

DEFINES		=	$(NO_CONFIG_PINS) $(NO_CONFIG_TENA) -dMC68360 -dCACHE_INHIBITED_BUFS=1

DEBUG		=

RDIR		=	RELS/SPDRVR
IDIR		=	$(RDIR)

DEFS		=	-v=$(DRVRDIR) $(DPIO_DEFS) -v=. $(BSDDEFS) -v=../..
DEPENDDEFS	=	$(SDIR)/defs.h $(SDIR)/history.h $(SDIR)/qedvr.h \
				$(SDIR)/proto.h

LIBS		=	$(MBUFLIBS) $(CPULIBS) $(CONV_LIBS) $(MWOS_LIBS)

CFILES		=	$(SDIR)/main.c $(SDIR)/entry.c  $(SDIR)/../misc.c \
			$(SDIR)/init.c 	$(SDIR)/isr.c $(SDIR)/term.c \
			$(SDIR)/pins.c ./target.c
RFILES		=	$(DPIO_DRVSTART) $(RDIR)/main.r $(RDIR)/drvr.r
IFILES		=	$(IDIR)/entry.i  $(IDIR)/misc.i $(IDIR)/init.i \
			$(IDIR)/isr.i $(IDIR)/term.i $(IDIR)/pins.i $(IDIR)/target.i

# do not use the -iom option on any file where a function is declared
# which is refered to as a pointer elsewhere.  The -iom option causes
# inlined functions to be discarded, regardless of whether they are
# used by pointer elsewhere.  This gives an unresolved reference during
# linking.
OPTMZ		=	-o=7 -t=1 -s=1 -ao=sc #-iom

COMPILE		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
				$(COPT_CODEADDR) $(DEBUG) $(COPTS)
ASSEMBL		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
				$(COPT_CODEADDR) $(DEBUG) $(LIBS) $(COPTS)
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

$(RDIR)/drvr.r:  $(IFILES)
	$(ASSEMBL) $(IFILES) -fd=drvr.r

$(RDIR)/main.r:  $(SDIR)/main.c  $(DEPENDDEFS) $(MAKENAME)
	$(ASSEMBL) $(SDIR)/main.c -fd=main.r

$(IDIR)/entry.i:  $(SDIR)/entry.c $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) $(SDIR)/entry.c

$(IDIR)/init.i:  $(SDIR)/init.c $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) $(SDIR)/init.c

$(IDIR)/isr.i:  $(SDIR)/isr.c $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) $(SDIR)/isr.c

$(IDIR)/misc.i:  $(SDIR)/../misc.c $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) $(SDIR)/../misc.c

$(IDIR)/term.i:  $(SDIR)/term.c $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) $(SDIR)/term.c

$(IDIR)/pins.i:  $(SDIR)/pins.c $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) $(SDIR)/pins.c

$(IDIR)/target.i:  ./target.c $(DEPENDDEFS) $(MAKENAME)
	$(COMPILE) ./target.c

_makdir:  nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(RDIR)/*.i

_cleanall: nulltrg
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
	$(LINT) $(LINTPARMS) $(DEFS) $(DEFINES) $(CFILES) >$(TRGTS).lnt

nulltrg:  .
	$(COMMENT)
#																			#
#############################################################################
