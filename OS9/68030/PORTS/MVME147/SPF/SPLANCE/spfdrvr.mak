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
# 01  ??/??/?? Created
# 02  02/04/98 Updated to use lancom.tpl                              pjw
#     04/02/98 ****** SPF LAN Comm Pack v3.2 Release*********>              #
# 03  11/24/98 Add CODO, add makdirs, add COPT_GU0, etc.              mgh
#############################################################################
#	This makefile will make the SPF LANCE ethernet driver                   #N
#############################################################################

MWOS		=	../../../../../..
ALL_TRGTS	=	k020 
TRGTS		=	sp147
MAKENAME	=	spfdrvr.mak

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl
include $(DPIO)/MAKETMPL/dpio.tpl

ODIR		=	../../CMDS/BOOTOBJS/SPF
SDIR		=	$(DRVRDIR)/SPLANCE

DEFINES		=	-dVME147 # -dTP32V
DEBUG_LIB	=

RDIR		=	RELS/SPDRVR
IDIR		=	$(RDIR)

DEFS            =       -v=$(DRVRDIR) $(DPIO_DEFS) $(BSDDEFS)
LIBS	        =   $(MBUFLIBS) $(CPULIBS) $(CONV_LIBS) $(MWOS_LIBS) 

CFILES		=	$(SDIR)/main.c $(SDIR)/entry.c  $(SDIR)/isr.c \
				 $(SDIR)/init.c $(SDIR)/term.c $(SDIR)/../misc.c
RFILES		=	$(DPIO_DRVSTART) $(RDIR)/main.r $(RDIR)/drvr.r
IFILES		=	$(IDIR)/entry.i $(IDIR)/isr.i \
				$(IDIR)/init.i $(IDIR)/term.i $(IDIR)/misc.i

#OPTMZ		=	-o=7 -t=1 -s=1
OPTMZ		=	-o=1 -xao

COMPILE		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
				$(COPTS)
ASSEMBL		=	$(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
				$(COPTS)
LINK		=	$(LINKER) $(LPARMS) -tp=$(CPUC) -k -r $(LIBS) $(OPTMZ) $(LOPTS) $(COPT_GU0)

$(TRGTS): _makdir  $(ODIR)/$(TRGTS)
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

$(RDIR)/main.r:  $(SDIR)/main.c $(SDIR)/defs.h $(SDIR)/history.h
	$(ASSEMBL) $(SDIR)/main.c

$(IDIR)/entry.i:  $(SDIR)/entry.c $(SDIR)/defs.h $(SDIR)/history.h $(SDIR)/proto.h
	$(COMPILE) $(SDIR)/entry.c

$(IDIR)/isr.i:  $(SDIR)/isr.c $(SDIR)/defs.h $(SDIR)/history.h
	$(COMPILE) $(SDIR)/isr.c

$(IDIR)/init.i:  $(SDIR)/init.c $(SDIR)/am7990.h
	$(COMPILE) $(SDIR)/init.c

$(IDIR)/term.i: $(SDIR)/term.c $(SDIR)/am7990.h
	$(COMPILE) $(SDIR)/term.c

$(IDIR)/misc.i:  $(SDIR)/../misc.c $(SDIR)/defs.h $(SDIR)/history.h
	$(COMPILE) $(SDIR)/../misc.c

_makdir:  nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_cleanall:  nulltrg
    -$(DEL) $(IDIR)/*.i  
    -$(DEL) $(RDIR)/*.r

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

#
# End of make.tpl
#############################################################################
