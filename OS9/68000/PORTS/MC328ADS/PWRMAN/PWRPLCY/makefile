# Makefile
#*****************************************************************************
#* This makefile will make the PowerManager Policy subroutine module    **
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
#*   1 03/18/96 creation                                          rmm       **
#*     01/17/97 <--------------- PwrMan v2.1 --------------->               **
#*   2 04/13/99 support MAKDIR and CODO                           srm       **
#*****************************************************************************

PORT    = ../..
TRGTS   = pwrplcy

include $(PORT)/../make.com

RDIR    = RELS
IDIR    = $(RDIR)
ODIR    = $(PORT)/CMDS/BOOTOBJS
SDIR    = $(MWOS)/SRC/SYSMODS/PWRMAN/PWRPLCY/MC328ADS

DEBUG   = 
DEFINES   = 
DEFS    = -v=. -v=$(SDIR) $(MWOS_DEFS)
LIBS    = -l=$(MWOS_DFTLIB)/pwrman.l $(CPULIBS) $(MWOS_LIBS) 
OPTMZ   = -o=7 -t=1 -s=1 -cw -iom

CFILES    = 
RFILES    = $(RDIR)/plcystrt.r $(RDIR)/icode.r
IFILES    = $(IDIR)/pwrplcy.i

COMPILE   = $(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
				$(DEBUG) -asn $(COPTS)
ASSEMBL   = $(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
				$(DEBUG) $(LIBS) -asn $(COPTS)
LINK    = $(LINKER) $(LPARMS) -k -r $(OPTMZ) $(DEBUG) $(LIBS) $(LOPTS) $(COPT_GU0)

pwrplcy:  _makdir $(ODIR)/pwrplcy
	$(COMMENT)

$(ODIR)/pwrplcy:  $(RFILES)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/pwrplcy.stb
	$(CODO) $(ODIR)/STB/pwrplcy.dbg
	$(LINK) $(RFILES) -fd=$@ -oln=pwrplcy

$(RDIR)/icode.r:  $(IFILES)
	$(ASSEMBL) $(IFILES) -fd=icode.r

$(RDIR)/plcystrt.r:  $(SDIR)/plcystrt.a ./makefile
	$(ASSEMBL) $(SDIR)/plcystrt.a

$(IDIR)/pwrplcy.i:  $(SDIR)/pwrplcy.c $(SDIR)/defs.h ./makefile
	$(COMPILE) $(SDIR)/pwrplcy.c

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  .
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  .
	$(CODO) $(ODIR)/pwrplcy
	-$(DEL) $(ODIR)/pwrplcy
	$(CODO) $(ODIR)/STB/pwrplcy.stb
	-$(DEL) $(ODIR)/STB/pwrplcy.stb
	$(CODO) $(ODIR)/STB/pwrplcy.dbg
	-$(DEL) $(ODIR)/STB/pwrplcy.dbg

_lint:  .
	-$(DEL) pwrplcy.lnt
	$(LINT) $(LINTPARMS) $(DEFS) $(DEFINES) $(CFILES) >pwrplcy.lnt

#
#
#############################################################################
