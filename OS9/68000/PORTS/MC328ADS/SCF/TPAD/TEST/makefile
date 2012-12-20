#*****************************************************************************
#* ID:     %A%
#* Date:   %G%
#*****************************************************************************
#* This makefile will make the MC68328ADS Touchpad SCF device driver        **
#* test application                                                         **
#*****************************************************************************
#* Copyright 1995 by Microware Systems Corporation                          **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************
#* Edition History:                                                         **
#* #   Date     Comments                                          By        **
#* --- -------- ------------------------------------------------- --------- **
#*   1 01/11/96 creation                                          rws       **
#*   2 07/09/96 modified for new PORT usage                       rws       **
#*   3 99/04/05 support MAKDIR and CODO                           srm       **
#*****************************************************************************

PORT		=	../../..
ONAME		=	tpadtest
TRGTS		=	$(ONAME)

include $(PORT)/../make.com

MAKENAME	=	./makefile
#include $(MWOS)/MAKETMPL/make.com

SDIR		= 	$(MWOS)/OS9/SRC/IO/SCF/DRVR/MC328ADS/TOUCHPAD/TEST
ODIR		= 	$(PORT)/CMDS
LDIR		= 	$(MWOS_LIBDIR)

HEADERS		=	$(SDIR)/../touchpad.h 
FILES		= 	$(CSTART) $(SDIR)/main.c
DEFS		= 	$(MWOS_DEFS) -v=$(SDIR) -v=$(SDIR)/.. -v=$(PORT)/DEFS \
				-v=$(MWOS)/OS9/68000/DEFS/MC6832X
LIBS		= $(CONV_LIBS) $(MWOS_CSLLIBS)

DEFINES		=  # -dTIME_STAMP
OPTMZ	    =	-o=7 -cw
LINK	    =	$(LINKER) $(LPARMS) $(DEFINES) $(DEFS) $(LIBS) $(OPTMZ) \
				$(COPT_JMPTBL) -olS -k $(LOPTS) $(COPT_GU0)

$(ONAME): _makdir $(ODIR)/$(ONAME)
	$(COMMENT)

$(ODIR)/$(ONAME): $(FILES) $(HEADERS) $(MAKENAME)
	$(CODO) $(ODIR)/$(ONAME)
	$(CODO) $(ODIR)/STB/$(ONAME).stb
	$(LINK) $(FILES) -fd=$(ODIR)/$(ONAME) -oln=$(ONAME)

_makdir: .
	$(MAKDIR) $(ODIR)/STB

_clean _purge:  nulltrg
	$(CODO) $(ODIR)/$(ONAME)
	-$(DEL) $(ODIR)/$(ONAME)
	$(CODO) $(ODIR)/STB/$(ONAME).stb
	-$(DEL) $(ODIR)/STB/$(ONAME).stb
	-$(DEL) $(ODIR)/STB/$(ONAME).dbg

nulltrg:  .
	$(COMMENT)

#																			#
#############################################################################
